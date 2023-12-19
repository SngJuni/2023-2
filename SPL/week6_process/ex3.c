#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    char *cmd;
    char *arg[100];
    size_t size;

    while (1) {
        getline(&cmd, &size, stdin);    
        cmd[strlen(cmd) - 1] = '\0';

        if (!strcmp(cmd, "quit")) break;

        int i = 0;
        char *ptr = strtok(cmd, " ");

        while (ptr != NULL && i < 99) {
            arg[i++] = ptr;
            ptr = strtok(NULL, " ");
        }
        arg[i] = NULL;

        pid_t child_pid = fork();
        int child_status;

        if (child_pid == -1) {
            perror("fork failed");
            exit(1);
        }

        if (child_pid == 0) {
            char path[100];
            sprintf(path, "/bin/%s", arg[0]);

            execvp(path, arg);
            perror("execvp failed");
            exit(1);
        } else {
            waitpid(child_pid, &child_status, 0);
        }
    }

    return 0;
}
