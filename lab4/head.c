#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main (int argc, char **argv){
//---HEAD flags
    int type = 1; //1-lines, 0-bytes
    int title = 0; //1-with, 0-without
    int zero = 0; //1-...[NUL] 0-...\n
    int num = 10; //количество выводимых единиц
    int files[argc-1]; //номера аргументов в которых хранятся имена файлов
    memset(files, 0, sizeof(files));

//---Arguments parsing
    int state = 0; //1-parse flag, 2-parse num, 0-parse file
    char number[12] = {0}; //массив для считывания количества выводимых единиц
    int number_pointer = 0; //указатель на символ в number[]
    int files_pointer = 0; //указатель на файл в files[]
    int cmd_input = 0;

    for (int i=1; i< argc; i++){    // обходим по аргументам командной строки
        int n = 0;                  // номер символа
         while (argv[i][n] != 0){    // перебираем символы аргумента
            switch (state){
                case 0:
                    if(argv[i][n] == '-') { // проверка на флаги или число
                        state = 1;
                        cmd_input = 1;
                    }
                    else {
                        files[files_pointer++] = i; // добавлять номер аргумента в список файлов
                        goto file_saving;           // выход из обхода по символам аргумента
                    }
                    break;
                case 1:
                    cmd_input = 0;
                    switch (argv[i][n]){
                        case 'c':       //вывод num символов
                            type = 0;
                            state = 2;
                            break;
                        case 'n':       //вывод num строк
                            type = 1;
                            state = 2;
                            break;
                        case 'q':       //вывод без заголовка
                            title = 0;
                            break;
                        case 'v':       //вывод с заголовком
                            title = 1;
                            break;
                        case 'z':
                            zero = 1;
                            break;
                        default:
                            //ToDo при вводе числа без параметров (head -1) считывается 1 и запускает с даными по умолчанию
                            //ToDo число стоящее после параметров q v z является некорректным
                            printf("head: invalid option -- '%c'\n",argv[i][n]);
                            break;
                    }
                    break;
                case 2:
                    number[number_pointer] = argv[i][n];
                    number_pointer++;
                    break;
            }
            n++;
        }
        file_saving:
        if (state == 1) {
            if (cmd_input == 1){
                files[files_pointer++] = i;
            }
            state = 0;
        }
        if(state == 2 && number_pointer != 0){ //преобразование массива @number в число после считывания
            num=0;
            int start = 0;
            if (number[0] == 45/* '-' */) start = 1;
            for (int j = start; j < strlen(number); j++) {
                if (number[j] < 58/* '9' */ && number[j] > 47/* '0' */){
                    num = num * 10 + ((int) number[j] - 48/* '0' */);
                } else{
                    printf("head: invalid number of lines: '");
                    write(1, number, number_pointer);
                    printf("'\n");
                    exit(1);
                }
            }
            num *= start==0?1:-1;

            memset(number, 0, sizeof(number));
            number_pointer = 0;
            state = 0;
        }
    }

//---HEAD
    //ToDo если выводится >1 файла то выводим имена (только если не было q)
    int i = 0;
    while (files[i] != 0){
        char buf[1];
        int counter = 0;
        int file_descriptor = open(argv[files[i]], O_RDONLY);
        if (errno){
            perror("head");
            exit(EXIT_FAILURE);
        }
        // Title output
        if (title == 1) {
            if (i != 0) {
                printf("\n");
            }
            printf("==> %s <==\n", argv[files[i]]);
        }

        // Text output
        while (read(file_descriptor,buf,1) > 0 && counter != num){ //ToDo реализовать вывод файла с num<0
            if(errno){
                perror("head");
                exit(EXIT_FAILURE);
            }
            if (zero == 1){
                if (buf[0] == '\n') buf[0] = '\0';
            }
            if (type == 1){
                counter += buf[0]=='\n'?1:0;
            } else counter++;
            write(1,buf,1);
            if (errno){
                perror("head");
            }
        }
        i++;
    }
}