#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h> 
#include <signal.h>
#include <sys/wait.h>

#define T_MSG 20
#define MAX_LEN 512
#define ACK_FLAG -1  // Define a flag to represent acknowledgment

struct msg {
    long msgtype;
    int sender;
    char text[MAX_LEN];
};

int main() {
    key_t key = ftok(".", 'a');
    int my_id, receiver_id, pid;

    printf("My id: "); 
    scanf("%d", &my_id);
    getchar();  
    printf("Receiver's id: ");
    scanf("%d", &receiver_id); 
    getchar();

    int qid = msgget(key, IPC_CREAT | 0x1FF); 

    if (qid < 0) {
	    perror("msgget faild");
        exit(1);
    }
    /* Please do not modify up to this point */

    if ((pid = fork()) == 0) {
        // Child process: Handles receiving messages and sending acks
        struct msg received_msg;
        while (1) {
            // Receive a message addressed to this process
            if (msgrcv(qid, &received_msg, sizeof(received_msg) - sizeof(long), my_id, IPC_NOWAIT) != -1) {
                if (received_msg.sender == ACK_FLAG) {
                    printf("%d read message at:\t%s\n", receiver_id, received_msg.text);
                }
                else {
                    printf("RECEIVED %s\n", received_msg.text);
                    time_t now = time(NULL);
                    strncpy(received_msg.text, ctime(&now), MAX_LEN);
                    received_msg.sender = ACK_FLAG;  // Marking the message as an ACK
                    msgsnd(qid, &received_msg, sizeof(received_msg) - sizeof(long), 0);
                }
            }
            sleep(1);
        }
    }
    else {
        // Parent process: Handles sending messages        
        struct msg message_to_send, received_msg;
        message_to_send.msgtype = receiver_id;
        message_to_send.sender = my_id;
        char text_input[MAX_LEN];
        while (1) {
            // Check for any received messages before sending
            if (msgrcv(qid, &received_msg, sizeof(received_msg) - sizeof(long), my_id, IPC_NOWAIT) != -1) {
                if (received_msg.sender != ACK_FLAG) {
                    printf("RECEIVED %s\n", received_msg.text);
                }
                // Only display if it's not an acknowledgment message
            }

            fgets(text_input, MAX_LEN, stdin);
            text_input[strcspn(text_input, "\n")] = '\0'; // remove newline character
            strncpy(message_to_send.text, text_input, MAX_LEN);
            msgsnd(qid, &message_to_send, sizeof(message_to_send) - sizeof(long), IPC_NOWAIT);
            
            sleep(1);
        }
    }	

    msgctl(qid, IPC_RMID, NULL);

    return 0;
}

