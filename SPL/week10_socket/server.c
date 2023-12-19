#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 200000

int main (int argc, char *argv[]) {
    int n, listenfd, connfd, caddrlen;
    struct sockaddr_in saddr, caddr;
    char buf[MAXLINE];
    int port = atoi(argv[1]);

    // TODO: socket()
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }

    memset((char *) &saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(port);

    // TODO: bind()
    if (bind(listenfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
        printf("bind() failed.\n");
        exit(2);
    }

    // TODO: listen()
    if (listen(listenfd, 5) < 0) {
        printf("listen() failed.\n");
        exit(3);
    }

    char filename[50];
    FILE *fp;


    while (1) {
        caddrlen = sizeof(caddr);
        
        // TODO: accept()
        if ((connfd = accept(listenfd, (struct sockaddr *)&caddr, (socklen_t *)&caddrlen)) < 0) {
            printf ("accept() failed.\n");
            continue;
        }

        // receive filename
        n = read(connfd, filename, sizeof(filename)-6);  // Reserve space for "_copy"
        filename[n] = '\0'; // Null-terminate the string
        
        // TODO: printf()
        printf ("got %d bytes from client.\n", n);


        if (strchr(filename, '.') != NULL) {  // Check for file extension and ignore
            printf("Received file with extension. Ignoring.\n");
            close(connfd);
            continue;
        }

        strcat(filename, "_copy"); // Append "_copy"

        fp = fopen(filename, "wb");
        if (!fp) {
            printf("Failed to open file.\n");
            close(connfd);
            continue;
        }

        while ((n = read(connfd, buf, MAXLINE)) > 0) {
            fwrite(buf, 1, n, fp);
            // TODO: printf()
            printf ("got %d bytes from client.\n", n);
        }
        
        // TODO: printf()
        printf("File saved: %s\n", filename);

        fclose(fp);
        close(connfd);
    }
}

