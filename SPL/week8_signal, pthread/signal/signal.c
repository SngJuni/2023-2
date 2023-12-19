#include <stdio.h> 
#include <unistd.h> // fork 
#include <signal.h> // signal 
#include <stdlib.h> // exit code for easy debug 
#include <sys/wait.h>

#define N_BEEPS 5 // just for debug  
#define SLEEP_SEC 1  


enum bool {False, True}; 

void handler(int sig) {
	int beep_cnt = 0;

	while (beep_cnt < N_BEEPS) {
		pid_t pid = fork();

		if (pid == 0) {
			printf("BEEP\n");
			sleep(SLEEP_SEC);
			exit(0);
		}
		else {
			wait(NULL);
			beep_cnt++;
		}
	}
	printf("I'm Alive!\n");

	exit(0); // just for easy debug 
}



int main(int argc, char* argv[]) {

	signal(SIGINT, handler);

	while(True) {
		// DO NOTHING 
	}

	return 0; 
}

