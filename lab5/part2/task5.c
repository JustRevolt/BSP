#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> // sleep
/*
 * int semget(key_t key, int nsems, int semflg);
 * int semctl(int semid, int semnum, int cmd, ...);
 * int semop(int semid, struct sembuf *sops, unsigned nsops);
 * --> struct sembuf { short sem_num, short sem_op - operation, short sem_flg = {IPC_UNDO}  }
 *
 * */
#include <pthread.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <string.h> // strlen
#include <ctype.h>  // toupper, tilower
#include <signal.h> // ctrl+z

char array[]="stressed";

#define INVERT 0
#define REVERSE 1
#define PRINT 2

/* SEMAPHORES */
key_t key = 777;
int sem_ID;
// ------------
struct sembuf sem_print_ex = {PRINT,1,SEM_UNDO};


/* THREADS */
pthread_t thread_1 ;
pthread_t thread_2 ;

void sighandler(int sign_num){
    printf("\nGood bye :(\n");
    exit(EXIT_SUCCESS);
}

void *invert() {
    int length = strlen(array);

    struct sembuf sem_lock = {INVERT,-1,SEM_UNDO};
    while(semop(sem_ID,&sem_lock,1) != -1) {
        //printf("--- invert ---\n");
        int i;
        for (i = 0; i < length; i++) {
            if (array[i] >= 'A' && array[i] <= 'Z') array[i] = (char) tolower(array[i]);
            else array[i] = (char) toupper(array[i]);
        }
        semop(sem_ID,&sem_print_ex,1);
    }
    return NULL;
}

void *reverse(){
    int length = strlen(array);

    struct sembuf sem_lock = {REVERSE,-1,SEM_UNDO};
    while(semop(sem_ID,&sem_lock,1) != -1) {

        //printf("--- reverse ---\n");
        char letter;
        int i;
        for (i = 0; i < length / 2; i++) {
            letter = array[length - 1 - i];
            //printf("Next letter %c\n", letter);
            array[length - 1 - i] = array[i];
            array[i] = letter;
        }

        semop(sem_ID,&sem_print_ex,1);
    }
    return NULL ;
}



int main(int argc, char* argv[]){

    // 1. semget -> get sem ID
    sem_ID = semget(key, 3, IPC_CREAT | 0600); // only user has rights
    if(errno){
        perror("SEMGET");
        exit(EXIT_FAILURE);
    }

    // 2. semctl
    /*
     * getall =>
     * - all semaphores from arg.array get semval value
     * - 2nd argument is ignored
     */
    unsigned short semun_array[] = {0,0,0}; // there are 3 semaphores
    semctl(sem_ID, 0, GETALL, semun_array);
    if (errno){
        perror("SEMCTL");
        exit(EXIT_FAILURE);
    }

    // 3. threads creation
    pthread_create(&thread_1, NULL, invert, NULL);
    pthread_create(&thread_2, NULL, reverse, NULL);

    signal(SIGTSTP,sighandler) ; // stop by CTRL+Z

    struct sembuf sem_funct = {0,1,SEM_UNDO};
    struct sembuf sem_print_lock = {PRINT,-1,SEM_UNDO};
    int thread_num=1;
    while(1){
        if (thread_num){
            sem_funct.sem_num = INVERT;
            thread_num=INVERT;
        }else {
            sem_funct.sem_num = REVERSE;
            thread_num=REVERSE;
        }
        semop(sem_ID, &sem_funct, 1);
        semop(sem_ID, &sem_print_lock, 1);
        printf("%s\n",array);

        sleep(1);
    }

}
