/*
 * Mapping file (by mmap)
 *
 * include <sys/mman.h>
 * void *mmap (void *addr, size_t LENGTH, int prot, int flags, int fd, off_t offset)
 *
 * */

#include <stdio.h>
#include <stdlib.h> // for exit(), printf()

#include <sys/mman.h>

#include <unistd.h>  // syscall open
#include <fcntl.h>

#include <time.h> // for time(NULL)
#include <errno.h>
#include <signal.h>


#include "task1.h"

time_t start_time, cur_time ;
int file_desc;

struct server_info save_info(){
    struct server_info info;
    info.process_ID = getpid();
    info.process_user_ID = getuid();
    info.process_group_ID = getgid();
    info.current_time = time(NULL) - start_time;
    getloadavg(info.loadavg,3);

    return info ;
}

struct server_info *info ;


void sighandler(int sign_num){
    printf("\nClear the file\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    start_time = time(NULL);

    unlink(file_name);
    // 1. open the file with file_name ;
    file_desc = open(file_name,O_RDWR | O_CREAT, 0644);
    if (file_desc<0){
        perror("OPEN");
        exit(EXIT_FAILURE);
    }

    // 2. mapping
    info = (struct server_info*)mmap(NULL, sizeof(struct server_info), PROT_READ| PROT_WRITE, MAP_SHARED, file_desc, 0) ;
    if (errno){
        perror("MMAP");
        exit(EXIT_FAILURE);
    }

    // 3. save default params
    *info = save_info();
    printf("Process ID: %li\tProcess user ID: %li\tProcess group ID: %li\n", (long)info->process_ID, info->process_user_ID, info->process_group_ID);

    signal(SIGTSTP,sighandler) ; // stop by CTRL+Z

    // 4. loop - reverse data (time , loadavg)
    while(1){
        cur_time = time(NULL);
        info->current_time = cur_time - start_time ;
        printf("%li \n",  cur_time - start_time);
        getloadavg(info->loadavg,3);
        sleep(1);
    }




}