#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 4096
#define ARG_MAX 1048576

int main(int argc, char *argv[]) {
    //array for new long command
    char *arguments = calloc(1, ARG_MAX) ;
    // default command after xargs - echo
    if (argc == 1) strcat(arguments,"echo ") ;
    else { // user command
        for(int i=1; i<argc; i++){ // format "arg1_arg2_..."
            strcat(arguments, argv[i]);
            strcat(arguments, " ") ;
        }
    }
    int arguments_length = strlen(arguments) ;
    char read_buf[BUFFER_SIZE];
    int read_size;
    // read input data from command line ( before | )
    while (
            (read_size = read(0, read_buf, BUFFER_SIZE)) > 0  &&
            arguments_length < ARG_MAX - 1
            ) {
        for (int i = 0; i < read_size; i++) {
            if (read_buf[i] == '\n') arguments[arguments_length++] = ' ';
            else arguments[arguments_length++] = read_buf[i];
        }
    }      
    return system(arguments) ;
    }
