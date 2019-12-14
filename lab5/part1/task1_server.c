/**
 *
 * Two threads - server ; client
 * shared memory
 *
 * (memory segment id) = int shmget(key_t key, size_t size, int shmflg);
 * void *shmat(int shmid, const void *shmaddr, int shmflg);   - attach shared memory segment
 * int shmdt(const void *shmaddr);   - detach shared memory segment
 * int shmctl(int shmid, int cmd, struct shmid_ds *buf);
 */

#include <stdlib.h> // for exit(), printf()
#include <stdio.h>

#include <sys/types.h> // key_t, pid_t, etc
#include <sys/ipc.h> // and below -> shmget()
#include <sys/shm.h>

#include <unistd.h>  // for getpid()
#include <time.h> // for time(NULL)
#include <errno.h>
#include <signal.h>

#include "task1.h"

struct  server_info *info ;
time_t start_time, cur_time ;

// for shmget()
key_t key ;
int mem_ID;

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
    printf("\nRemove the segment\n");
    shmctl(mem_ID, IPC_RMID, NULL);
    printf("Free the array\n");
    free(info) ;
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]){
    start_time = time(NULL);

    key = (key_t)KEY;
    mem_ID = shmget(key, sizeof(info),IPC_CREAT | 0644);
    if (errno){
        perror("SHMGET");
        exit(EXIT_FAILURE);
    }

    info = (struct server_info*) shmat(mem_ID, NULL, 0);
    if (errno){
        perror("SHMAT");
        exit(EXIT_FAILURE);
    }
    if(info->process_ID){
        printf("Server has already start\n");
        exit(EXIT_SUCCESS);
    }else {
        *info = save_info();
        printf("Process ID: %li\tProcess user ID: %li\tProcess group ID: %li\n", (long)info->process_ID, info->process_user_ID, info->process_group_ID);

        signal(SIGTSTP,sighandler) ; // stop by CTRL+Z
        while(1){
            cur_time = time(NULL);
            info->current_time = cur_time - start_time ;
            printf("%li \n",  cur_time - start_time);
            getloadavg(info->loadavg,3);
            sleep(1);
        }
    }


}