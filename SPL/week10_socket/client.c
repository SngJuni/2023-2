#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 200000

int main (int argc, char *argv[]) {
    int n, cfd;
    struct hostent *h;
    struct sockaddr_in saddr;
    char buf[MAXLINE];
    char *host = argv[1];
    int port = atoi(argv[2]);
    
    // TODO: socket()
    if ((cfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket() failed.\n");
        exit(1);
    }
    

    if ((h = gethostbyname(host)) == NULL) {
            printf("invalid hostname %s\n", host);
            exit(2);
        }

        // TODO: connect()

        memset((char *)&saddr, 0, sizeof(saddr));
        saddr.sin_family = AF_INET;
        memcpy((char *)&saddr.sin_addr.s_addr, (char *)h->h_addr, h->h_length);
        saddr.sin_port = htons(port);

        if (connect(cfd, (struct sockaddr *)&saddr, sizeof(saddr)) < 0) {
            printf("connect() failed.\n");
            exit(3);
        }

        char filename[50];
        FILE *fp;

        while (1) {
            printf("Enter filename (or 'quit' to exit): ");
            fgets(filename, sizeof(filename), stdin);
            
            filename[strlen(filename) - 1] = '\0';

            if (strcmp(filename, "quit") == 0) {
                break;
            }
            
            if (strchr(filename, '.') != NULL) {  
                printf("Invalid file name. Ensure no extensions are present.\n");
                continue;
            }

            printf("File name: %s\n", filename);

            fp = fopen(filename, "rb");
            if (!fp) {
                printf("Failed to open file: %s\n", filename);
                continue;
            }

            write(cfd, filename, strlen(filename));

            while ((n = fread(buf, 1, MAXLINE, fp)) > 0) {
                write(cfd, buf, n);
            }

            fclose(fp);
            sleep(1);
        }
        close(cfd);
}

