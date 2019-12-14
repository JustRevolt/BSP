#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h> // sleep
#include <string.h>

#include <time.h> // for time(NULL)
#include <sys/types.h> // pid_t
#include <sys/socket.h>
#include <sys/un.h>

#include "part3.h"

int main(int argc, char *argv[]){
    struct server_info info ;
    int socket_fd;
    int read_n;
    struct sockaddr_un serv_addr;

    socket_fd = socket(AF_UNIX, SOCK_STREAM,0);
    if (errno){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, path);

    connect(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (errno){
        perror("connect");
        exit(EXIT_FAILURE);
    }

    read_n = read(socket_fd, &info, sizeof(info));
    if (read_n != sizeof(info)) printf("Error: read()\n");

    printf("Process ID: %li\tProcess user ID: %li\tProcess group ID: %li\n", (long)info.process_ID, info.process_user_ID, info.process_group_ID);
    printf("%d seconds have passed after start.\n", (int)info.current_time);
    printf("Average system load [1/5/15 minutes]: %.4f, %.4f, %.4f\n", info.loadavg[0],info.loadavg[1], info.loadavg[2]);

    return 0;
}
