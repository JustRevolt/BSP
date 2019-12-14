/*
 * INITIAL A UNNAMED SEMAPHORE
 * int sem_init(sem_t *sem, int pshared, unsigned int value)
 * (pshere = 0 => the semaphore is shared between threads of a process)
 *
 *
 *
 *
 * */

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h> // sleep();

#include <semaphore.h>
#include <pthread.h>

#include <string.h> // strup
#include <ctype.h>

#include <errno.h>
#include <signal.h>

char array[]="stressed";

pthread_t thread_1;
pthread_t thread_2;

#define INVERT 0
#define REVERSE 1
#define PRINT 2
sem_t sem[3];

void *invert() {
    int length = strlen(array);
    while(sem_wait(&sem[INVERT]) == 0) {

        //printf("--- invert ---\n");
        for (int i = 0; i < length; i++) {
            if (array[i] >= 'A' && array[i] <= 'Z') array[i] = (char) tolower(array[i]);
            else array[i] = (char) toupper(array[i]);
        }

        sem_post(&sem[PRINT]);
    }
    return NULL;
}

void *reverse(){
    int length = strlen(array);
    while(sem_wait(&sem[REVERSE])==0) {

        //printf("--- reverse ---\n");
        char letter;
        for (int i = 0; i < length / 2; i++) {
            letter = array[length - 1 - i];
            //printf("Next letter %c\n", letter);
            array[length - 1 - i] = array[i];
            array[i] = letter;
        }

        sem_post(&sem[PRINT]);
    }
    return NULL ;
}

void sighandler(int sign_num){
    printf("\nGood bye :(\n");
    for(int i=0; i<3;i++){
        sem_destroy(&sem[i]);
    }
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){

    // 1 initialization
    for(int i=0; i<3;i++){
        sem_init(&sem[i], 0,0);
        if (errno){
            perror("INIT");
            exit(EXIT_FAILURE);
        }
    }
    //printf("---------------------- after init --- \n");

    pthread_create(&thread_1, NULL, invert, NULL);
    pthread_create(&thread_2, NULL, reverse, NULL);
    //printf("---------------------- after creation --- \n");
    signal(SIGTSTP,sighandler) ; // stop by CTRL+Z

    int counter=0;
    int thread_num=1;
    while(1){

        if (thread_num) thread_num = INVERT;
        else thread_num = REVERSE;

      //  printf("thread # %d\n", thread_num) ;
        sem_post(&sem[thread_num]); // inc thread num

        sem_wait(&sem[PRINT]); // dec print
        printf("%d, %s\n", counter,array);

        counter++ ;
        sleep(1);
    }
}
