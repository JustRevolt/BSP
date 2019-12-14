#include <stdio.h>
#include <stdlib.h> // for exit(), printf()

#include <sys/types.h> // key_t, pid_t, etc
#include <sys/ipc.h>
#include <sys/msg.h>

#include <unistd.h>  // for getpid()
#include <time.h> // for time(NULL)
#include <errno.h>
#include <signal.h>

#include "task1.h"


int main(int argc, char *argv[]){

    struct msg msg;
    key_t key ;
    int msg_ID;

    key = KEY;
    msg_ID = msgget(key, 0644 | IPC_CREAT);
    if (errno){
        perror("MSGGET");
        exit(EXIT_FAILURE);
    }

    msgrcv(msg_ID,&msg, sizeof(msg),0,0);
    if (errno){
        perror("MSGRCV");
        exit(EXIT_FAILURE);
    }

    printf("Process ID: %li\tProcess user ID: %li\tProcess group ID: %li\n", (long)msg.message.process_ID, msg.message.process_user_ID, msg.message.process_group_ID);
    printf("%li seconds have passed after start.\n", (long)msg.message.current_time);
    printf("Average system load [1/5/15 minutes]: %.4f, %.4f, %.4f\n", msg.message.loadavg[0],msg.message.loadavg[1], msg.message.loadavg[2]);

    return 0 ;
}

