// ipc1_skeleton code

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
    char *cmd = NULL, *argv[16], *pipe_argv[16], path[100], pipe_path[100];
    size_t buf_size = 0;

    while (1) {
        fflush(stdout);

        ssize_t read_size = getline(&cmd, &buf_size, stdin);
        if(read_size == -1) {
            perror("getline");
            continue;
        }
        cmd[read_size - 1] = '\0'; 

        if (strcmp(cmd, "quit") == 0) {
            while (wait(NULL) > 0);
            break;
        }

        int i = 0, j = 0;
        char *input_file = NULL;
        char *output_file = NULL;
        char *pipe_command = NULL;

        char* p = strtok(cmd, " ");
        while (p != NULL) {
            // TODO: Tokenize the command and handle "<", ">", and "|"
            if (strcmp(p, "<") == 0) {
                p = strtok(NULL, " ");
                input_file = p;
            } 
            else if (strcmp(p, ">") == 0) {
                p = strtok(NULL, " ");
                output_file = p;
            }
            else if (strcmp(p, "|") == 0) {
                p = strtok(NULL, " ");
                pipe_command = p;
		pipe_argv[j++] = p;

                while (p != NULL) {
                    p = strtok(NULL, " ");
                    pipe_argv[j++] = p;
                }
                pipe_argv[j] = NULL;
            }
            else {
                argv[i++] = p;
            }
            p = strtok(NULL, " ");
        }
        argv[i] = NULL;

        sprintf(path, "/bin/%s", argv[0]);

        // TODO: If there's a pipe command, create a pipe and set up `pipe_path`
        int pipefd[2];
        if (pipe_command) {
            if (pipe(pipefd) < 0) {
                perror("pipe failed");
                exit(1);
            }
            sprintf(pipe_path, "/bin/%s", pipe_command);
        }

        if (fork() == 0) {
            // TODO: Handle input redirection ("<")
            if (input_file) {
                int in = open(input_file, O_RDONLY);
                if (in < 0) {
                    perror("inputfile failed");
                    exit(1);
                }

                dup2(in, 0);
                close(in);
            }
            
            // TODO: If there's a pipe command, set up for writing to the pipe
            // Otherwise, handle output redirection (">")
            if (pipe_command) {
                dup2(pipefd[1], 1);
                close(pipefd[0]);
                close(pipefd[1]);
            } else if (output_file) {
                int out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (out < 0) {
                    perror("outputfile failed");
                    exit(1);
                }           
                dup2(out, 1);
                close(out);     
            }

            execv(path, argv);
            perror("execv");
            exit(1); 
        }

        // TODO: If there's a pipe command, fork a child for the command after the pipe
        if (pipe_command) {
            if (fork() == 0) {
                close(pipefd[1]);

                dup2(pipefd[0], 0);

                close(pipefd[0]);
                execv(pipe_path, pipe_argv);
                perror("execv for pipe command");
                exit(1);
            }
            close(pipefd[0]);
            close(pipefd[1]);
        }
    }

    free(cmd);
    return 0;
}

