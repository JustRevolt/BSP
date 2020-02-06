/*
 * -- rw lock
 * pthread_rwlock_init([pthread_rwloc_t]rwlock,NULL);
 * int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
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
#define LENGTH 8
int delay = 2000000; //

pthread_t thread_1;
pthread_t thread_2;
pthread_t thread_3;
pthread_rwlock_t rw_lock = PTHREAD_RWLOCK_INITIALIZER ;

void sighandler(int sign_num){
    pthread_rwlock_destroy(&rw_lock);
    if (errno){
        perror("destroy");
        exit(EXIT_FAILURE);
    }
    printf("\nGood bye :(\n");
    exit(EXIT_SUCCESS);
}

void *invert(){
    char str[10];
    while (1) {
        // READ
        pthread_rwlock_rdlock(&rw_lock);
        strcpy(str, array);
        printf("inv <- %s\n",str);
        pthread_rwlock_unlock(&rw_lock);
        // END READ
        int i;
        for (i = 0; i < LENGTH; i++) {
            if (str[i] >= 'A' && str[i] <= 'Z') str[i] = (char) tolower(str[i]);
            else str[i] = (char) toupper(str[i]);
        }
        printf("inv -> %s\n", str);

        // WRITE
        pthread_rwlock_wrlock(&rw_lock);
        strcpy(array,str);
        pthread_rwlock_unlock(&rw_lock);
        // END WRITE
        usleep(delay);
    }
}

void *reverse(){
    char str[10];
    char letter ;
    while(1) {
        // READ
        pthread_rwlock_rdlock(&rw_lock);
        strcpy(str, array);
        printf("rev <- %s\n",str);
        pthread_rwlock_unlock(&rw_lock);
        // END READ
        int i;
        for (i = 0; i < LENGTH / 2; i++) {
            letter = str[LENGTH - 1 - i];
            str[LENGTH - 1 - i] = str[i];
            str[i] = letter;
        }
        printf("rev -> %s\n", str);

        pthread_rwlock_wrlock(&rw_lock);
        strcpy(array,str);
        pthread_rwlock_unlock(&rw_lock);
        usleep(delay);
    }
}

void *count_up(){
    char str[10] ;
    int counter;
    while(1){
        pthread_rwlock_rdlock(&rw_lock);
        strcpy(str, array);
        printf("count <- %s\n", str);
        pthread_rwlock_unlock(&rw_lock);

        counter = 0 ;
        int i;
        for (i = 0; i < LENGTH; i++) {
            if (str[i] >= 'A' && str[i] <= 'Z') counter++;
        }
        printf("Current number of letters in upper case is %d\n", counter);
        usleep(delay);
    }

}

int main(int argc, char* argv[]){
    signal(SIGTSTP,sighandler) ; // stop by CTRL+Z

    pthread_create(&thread_1, NULL, invert, NULL);
    pthread_create(&thread_2, NULL, reverse, NULL);
    pthread_create(&thread_3, NULL, count_up, NULL);

    int sec = 0;
    while (1){
        pthread_rwlock_rdlock(&rw_lock);
        printf("### %d, %s\n",++sec, array );
        pthread_rwlock_unlock(&rw_lock);
        usleep(delay);
    }

}
