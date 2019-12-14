/*
 * SIGNALS
 * HUP - SIGHUP  - handing the process
 * INT - SIGINT (ctrl-c) - interruption the process
 * TERM - SIGTERM - finish
 * USR1, USR2
 *
 * int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);
 * */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> // sleep
#include <signal.h> // ctrl+z
#include <string.h>
#include <time.h> // for time(NULL)


#include "part3.h"

time_t  start_time;
struct server_info info ;

void sighandler(int sign_num){
    switch (sign_num){
        case SIGINT: { // ctrl - c
            printf("\tsigint: Process ID: %d\n",info.process_ID);
            break;
        }
        case SIGHUP:{
            printf("\tsighup: %d seconds after start\n",(int)info.current_time);
            break;
        }
        case SIGTERM:{
            printf("\tsigterm: Average system load [1/5/15 minutes]: %.4f, %.4f, %.4f\n", info.loadavg[0],info.loadavg[1], info.loadavg[2]);
            break;
        }
        case SIGUSR1:{ // kill(pid, SIGUSR1);
            printf("\tsigusr1: Process user ID: %d\n",info.process_user_ID);
            break;
        }
        case SIGUSR2: { // ==||==||==
            printf("\tsigusr2: Process group ID: %d\n",info.process_user_ID);
            break;
        }
        case SIGTSTP:{
            printf("\tGood bye :(\n");
            exit(EXIT_SUCCESS);
        }
    }
}


struct server_info save_info(){
    struct server_info info;
    info.process_ID = getpid();
    info.process_user_ID = getuid();
    info.process_group_ID = getgid();
    info.current_time = difftime(time(NULL),start_time);
    getloadavg(info.loadavg,3);

    return info ;
}

int main(int argc, char *agrv[]){
    start_time = time(NULL);
    info = save_info();

    printf("Signals: [kill -n pid]"
           "\n\tint  #2  process ID (ctrl-c)"
           "\n\thup  #1  current time"
           "\n\tterm #15 loadavg"
           "\n\tusr1 #10 process user ID"
           "\n\tusr2 #12 process group ID"
           "\n\ttstp #20 exit (ctrl-z)\n");

    signal(SIGINT, sighandler);
    signal(SIGHUP,sighandler);
    signal(SIGTERM,sighandler);
    signal(SIGUSR1,sighandler);
    signal(SIGUSR2,sighandler);
    signal(SIGTSTP,sighandler);

    while(1){
        info.current_time = difftime(time(NULL), start_time);
        getloadavg(info.loadavg, 3);
        sleep(1);
    }

    return 0;
}