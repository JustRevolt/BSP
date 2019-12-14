/*
 * UNIX DOMAIN SOCKET
 * socket
 * bind
 * listen
 * connect
 * accept
 * select
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> // sleep
#include <signal.h> // ctrl+z
#include <string.h>

#include <time.h> // for time(NULL)
#include <sys/types.h> // pid_t
#include <sys/socket.h>
#include <sys/un.h>

#include "part3.h"

time_t  start_time;
struct server_info info ;

void sighandler(int sign_num){
    printf("\nGood bye :(\n");
    exit(EXIT_SUCCESS);
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

int main(int agrc, char *argv[]){
    signal(SIGTSTP,sighandler) ; // stop by CTRL+Z

    start_time = time(NULL);
    info = save_info() ;
    printf("Process ID: %li\tProcess user ID: %li\tProcess group ID: %li\n", (long)info.process_ID, info.process_user_ID, info.process_group_ID);

    int socket_fd, connect_socket_fd;
    struct sockaddr_un serv_addr;

    // SOCKET
    socket_fd = socket(AF_UNIX, SOCK_STREAM,0); // tcp connection
    if (errno){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    memset(&serv_addr,0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    unlink(path);
    strcpy(serv_addr.sun_path, path);

    // BIND
    bind(socket_fd,(struct sockadd*)&serv_addr, sizeof(serv_addr) );
    if (errno){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // LISTEN
    listen(socket_fd,2);
    while(1){
        getloadavg(info.loadavg, 3);
        info.current_time = difftime(time(NULL),start_time);
        printf("last connection at %d\n", (int)info.current_time);
        connect_socket_fd = accept(socket_fd,NULL, NULL);
        write(connect_socket_fd,&info, sizeof(info));
        close(connect_socket_fd);

        sleep(1);
    }

    return 0;

}
