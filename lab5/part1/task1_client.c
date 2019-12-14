#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h> // and below -> shmget()
#include <sys/shm.h>
#include <errno.h>

#include "task1.h"


int main(int arg, char *argv[]){
    struct server_info *info ;
    key_t key = KEY;
    int mem_ID ;

    mem_ID= shmget(key, sizeof(info),0644);
    if (errno){
        perror("SHMGET");
        exit(EXIT_FAILURE);
    }

    info = (struct server_info*) shmat(mem_ID, NULL, 0);
    if (errno){
        perror("SHMAT");
        exit(EXIT_FAILURE);
    }
    printf("Process ID: %li\tProcess user ID: %li\tProcess group ID: %li\n", (long)info->process_ID, info->process_user_ID, info->process_group_ID);
    printf("%li seconds have passed after start.\n", (long)info->current_time);
    printf("Average system load [1/5/15 minutes]: %.4f, %.4f, %.4f\n", info->loadavg[0],info->loadavg[1], info->loadavg[2]);

    return EXIT_SUCCESS;
}