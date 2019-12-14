#ifndef LAB_5_TASK1_H
#define LAB_5_TASK1_H

struct server_info{
    pid_t process_ID;
    uid_t process_user_ID;
    gid_t process_group_ID;
    time_t current_time;
    double loadavg[3];
};

struct msg{
    long type ;
    struct server_info message;
};

const char *file_name="./t3";

#define KEY 777

#endif //LAB_5_TASK1_H
