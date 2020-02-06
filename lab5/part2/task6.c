/*
 * --- MUTEXES ---
 * int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr)
 * int pthread_mutex_lock(pthread_mutex_t *mutex)
 * int pthread_mutex_unlock(pthread_mutex_t *mutex)
 * int pthread_mutex_destroy(pthread_mutex_t *mutex)
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> // sleep

#include <pthread.h>

#include <string.h> // strlen
#include <ctype.h>  // toupper, tilower
#include <signal.h> // ctrl+z

char array[]="stressed";
int delay = 2000000 ; // 2 sec
#define INVERT 0
#define REVERSE 1
#define PRINT 2

pthread_t thread_1;
pthread_t thread_2;
pthread_mutex_t mutex;

void sighandler(int sign_num){
    pthread_mutex_destroy(&mutex);
    if (errno){
        perror("destroy");
        exit(EXIT_FAILURE);
    }
    printf("\nGood bye :(\n");
    exit(EXIT_SUCCESS);
}

void function(int var){
    pthread_mutex_lock(&mutex);

    int length = strlen(array);
    switch (var){
        case INVERT:{
            printf("--- invert ---\n");
            int i;
            for (i = 0; i < length; i++) {
                if (array[i] >= 'A' && array[i] <= 'Z') array[i] = (char) tolower(array[i]);
                else array[i] = (char) toupper(array[i]);
            }
            break;
        }
        case REVERSE:{
            printf("--- reverse ---\n");
            char letter;
            int i;
            for (i = 0; i < length / 2; i++) {
                letter = array[length - 1 - i];
                //printf("Next letter %c\n", letter);
                array[length - 1 - i] = array[i];
                array[i] = letter;
            }
            break;
        }
        case PRINT:{

            printf("%s\n",array);
            break;
        }
        default: printf("unexpected parameter");
    }
    pthread_mutex_unlock(&mutex);
}

void *thr_1(){
    while(1){
        function(INVERT);
        usleep(delay);
    }
}
void *thr_2(){
    while(1){
        function(REVERSE);
        usleep(delay);
    }
}

int main(int argc, char* argv[]){
    signal(SIGTSTP,sighandler) ; // stop by CTRL+Z

    // 1. initialization
    pthread_mutex_init(&mutex,NULL);
    if (errno){
            perror("init");
            exit(EXIT_FAILURE);
    }
    pthread_create(&thread_1,NULL,thr_1,NULL);
    pthread_create(&thread_2,NULL,thr_2,NULL);

    int counter=0;
    while(1){
        function(PRINT);
        counter++;
        usleep(delay);
    }
}
