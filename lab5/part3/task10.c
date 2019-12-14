/*
 * UNNAMED CHANNELS
 *
 * stdout(main process) -> pipe ;
 * pipe -> stdout(child process)  ;
 *
 * child process: wc
 * main process read file [from arguments]; every second symbol send to the child process
 *
 * int pipe(int fds[2]);
 * --- read from file with fd = 0; write to file with fd = 1
 *
 * int fork();
 * int execl(char *fname, char *arg0, ..., char *argN, NULL);
* int dup2(int oldfd, int newfd);
 * */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <string.h>

#include <unistd.h> // sleep
#include <fcntl.h>

#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char *argv[]){
    int fd, even = 0 ; // 0 - not even; 1 - even
    char symbol[1];

    // 0. check the arguments
    if (argc < 2){
        fprintf(stderr, "ERROR: File name isn't found.\n");
        exit(EXIT_FAILURE);
    }

    // 1. try to crate a pipe
    int fds[2];
    pipe(fds);
    if (errno){
        perror("pipe()");
        exit(EXIT_FAILURE);
    }

    // 2. fork
    int pid;
    char buf[1];
    pid = fork();
    if (errno){
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid == 0){ // child process
        // 2.1.1 copy std descriptor; duplicate file descriptors
        int std_in_copy = dup(0);
        dup2(fds[0],0);

        // 2.1.2 read from stdin
        while (1){
            read(0,buf,1);
            if(buf[0] == '\0') {
                printf("Reading is finished\n");
                break;
            }
            printf("ch: %s\n",buf);
        }
        close(fds[0]);
        dup2(std_in_copy,0);
        close(std_in_copy);
        execl("/usr/bin/wc","wc" ,(char *)0);
    } else{ // parent process
        // 2.2.1 duplicate file descriptors
        dup2(fds[1],1);
        close(0);

        // 2.2.2 open a file
        fd = open(argv[1],O_RDONLY);
        if (errno){
            perror("open()");
            exit(EXIT_FAILURE);
        }

        // 2.2.3 read from the file until eof
        while (read(fd,symbol,1) > 0){
            if (even){ // only even symbols
                write(1,symbol,1);
                //fprintf(stderr, "read: %c\n", symbol[0]);
                even = 0 ;
            }else even = 1 ;
        }
        symbol[0]='\0';
        write(1,symbol,1);

        wait(NULL);
    }
    return 0;
}
