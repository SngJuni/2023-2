#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <signal.h>

#define MAXLINE 80
#define MAXSEAT 256
#define MAXCLIENT 1024

typedef struct _query {
    int user;
    int action;
    int data;
} query;

typedef struct _userinfo {
    int id;
    int pw;
    int seat;
    bool status;
} userinfo;

int seat[MAXSEAT];
userinfo users[MAXCLIENT];
pthread_mutex_t seat_mutex[MAXSEAT];
pthread_mutex_t user_mutex = PTHREAD_MUTEX_INITIALIZER;

void clean_resource() {
    for (int i = 0; i < MAXSEAT; i++) {
        pthread_mutex_destroy(&seat_mutex[i]);
    }
    pthread_mutex_destroy(&user_mutex);
}

void signal_handler(int signo) {
    if (signo == SIGINT) {
        clean_resource();
        exit(0);
    }
}

void init() {
    for (int i = 0; i < MAXSEAT; i++) {
        seat[i] = -1;
        pthread_mutex_init(&seat_mutex[i], NULL);
    }

    for (int i = 0; i < MAXCLIENT; i++) {
        users[i].id = -1;
        users[i].pw = -1;
        users[i].seat = -1;
        users[i].status = false;
    }
}

void *client_handler(void *arg) {   
    int connfd = *((int *)arg);
    int success = 1;
    int fail = -1;
    query q;

    while (read(connfd, &q, sizeof(query)) > 0) {
        switch (q.action) {
            case 1:
                pthread_mutex_lock(&user_mutex);

                if (users[q.user].id == -1) {          // first time -> registration
                    users[q.user].id = q.user;
                    users[q.user].pw = q.data;
                    users[q.user].status = true;
                    send(connfd, (char *)&success, sizeof(int), 0);
                }
                else {                                 // already registered
                    if (users[q.user].pw != q.data || users[q.user].status) { // wrong pw or already logged in
                        send(connfd, (char *)&fail, sizeof(int), 0);
                    }
                    else {
                        users[q.user].status = true;
                        send(connfd, (char *)&success, sizeof(int), 0);
                    }
                }

                pthread_mutex_unlock(&user_mutex);
                break;
            case 2:
                pthread_mutex_lock(&user_mutex);
                pthread_mutex_lock(&seat_mutex[q.data]);

                if (users[q.user].status != 1) {        // before logging in
                    send(connfd, (char *)&fail, sizeof(int), 0);
                }
                else if (q.data < 0 || q.data > 255) {  // wrong seat num
                    send(connfd, (char *)&fail, sizeof(int), 0);
                }
                else if (seat[q.data] != -1) {          // already reserved seat
                    send(connfd, (char *)&fail, sizeof(int), 0);
                }
                else {
                    seat[q.data] = q.user;
                    users[q.user].seat = q.data;
                    send(connfd, (char *)&users[q.user].seat, sizeof(int), 0);
                }

                pthread_mutex_unlock(&seat_mutex[q.data]);
                pthread_mutex_unlock(&user_mutex);
                break;
            case 3:
                pthread_mutex_lock(&user_mutex);

                if (users[q.user].status != 1) {      // before logging in
                    send(connfd, (char *)&fail, sizeof(int), 0);
                }
                else if (users[q.user].seat == -1) {  // did not reserve any seat
                    send(connfd, (char *)&fail, sizeof(int), 0);
                }
                else {
                    send(connfd, (char *)&users[q.user].seat, sizeof(int), 0);
                }

                pthread_mutex_unlock(&user_mutex);
                break;
            case 4:
                pthread_mutex_lock(&user_mutex);
                pthread_mutex_lock(&seat_mutex[q.data]);

                if (users[q.user].status != 1) {      // before logging in
                    send(connfd, (char *)&fail, sizeof(int), 0);
                }
                else if (users[q.user].seat == -1) {  // did not reserve any seat
                    send(connfd, (char *)&fail, sizeof(int), 0);
                }
                else {
                    seat[users[q.user].seat] = -1;
                    send(connfd, (char *)&users[q.user].seat, sizeof(int), 0);
                    users[q.user].seat = -1;
                }

                pthread_mutex_unlock(&seat_mutex[q.data]);
                pthread_mutex_unlock(&user_mutex);
                break;
            case 5:
                pthread_mutex_lock(&user_mutex);

                if (users[q.user].status != 1) {      // before logging in
                    send(connfd, (char *)&fail, sizeof(int), 0);
                }
                else {
                    send(connfd, (char *)&success, sizeof(int), 0);
                    users[q.user].status = false;
                }

                pthread_mutex_unlock(&user_mutex);
                break;
            default:

                break;
        }
    }

    close(connfd);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]) {
    signal(SIGINT, signal_handler);

    int listenfd, connfd, caddrlen;
    struct sockaddr_in saddr, caddr;
    int port = atoi(argv[1]);

    init();

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char *)&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    if (bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }

    if (listen(listenfd, 1024) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    while (1) {
        caddrlen = sizeof(caddr);

        if ((connfd = accept(listenfd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) < 0) {
            printf("accept() failed.\n");
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, client_handler, &connfd) != 0) {
            printf("pthread creation failed.\n");
            exit(1);
        }
    }

    return 0;
}
