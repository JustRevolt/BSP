/*
 * A message queue is a linked list of messages stored within the kernel and identified by a message queue identifier.
 * A new queue is created or an existing queue opened by msgget().
 * New messages are added to the end of a queue by msgsnd().
 * Messages are fetched from a queue by msgrcv()
 *
 *
 * */

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

time_t start_time, cur_time ;

// for message queue
key_t key;
int msg_ID;

// message struct : type + message
struct msg msg;

struct server_info save_info(){
    struct server_info info;
    info.process_ID = getpid();
    info.process_user_ID = getuid();
    info.process_group_ID = getgid();
    info.current_time = time(NULL) - start_time;
    getloadavg(info.loadavg,3);

    return info ;
}



void sighandler(int sign_num){
    msgctl(msg_ID,IPC_RMID,NULL);
    printf("\nClear the queue\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    start_time = time(NULL);

    key = KEY;

    msg_ID = msgget(key, IPC_CREAT | 0644);
    if (errno){
        perror("MSGGET");
        exit(EXIT_FAILURE);
    }

    msg.message = save_info();
    msg.type = 1 ;

    printf("Process ID: %li\tProcess user ID: %li\tProcess group ID: %li\n", (long)msg.message.process_ID, msg.message.process_user_ID, msg.message.process_group_ID);


    signal(SIGTSTP,sighandler) ; // stop by CTRL+Z

    while (1){
        cur_time= time(NULL);
        msg.message.current_time = cur_time - start_time ;
        printf("%li \n",  cur_time - start_time);
        getloadavg(msg.message.loadavg,3);
        msgsnd(msg_ID,&msg, sizeof(msg),0);
        if (errno){
            perror("MSGSND");
            exit(EXIT_FAILURE);
        }
        sleep(1);
    }

}