#include <stdio.h>
#include <stdlib.h> // for exit(), printf()

#include <sys/mman.h>

#include <errno.h>

#include <unistd.h> // open syscall
#include <fcntl.h>

#include "task1.h"

int main(int arg, char *argv[]){
    struct server_info *info ;

    int file_desc = open(file_name,O_RDONLY);
    if (errno){
        perror("OPEN");
        exit(EXIT_FAILURE);
    }

    info = (struct server_info*)mmap(NULL, sizeof(struct server_info), PROT_READ| PROT_WRITE, MAP_SHARED, file_desc, 0) ;
    if (errno){
        perror("MMAP");
        exit(EXIT_FAILURE);
    }

    printf("Process ID: %li\tProcess user ID: %li\tProcess group ID: %li\n", (long)info->process_ID, info->process_user_ID, info->process_group_ID);
    printf("%li seconds have passed after start.\n", (long)info->current_time);
    printf("Average system load [1/5/15 minutes]: %.4f, %.4f, %.4f\n", info->loadavg[0],info->loadavg[1], info->loadavg[2]);


    return 0;
}