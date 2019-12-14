#ifndef LAB_5_PART3_H
#define LAB_5_PART3_H

struct server_info{
    pid_t process_ID;
    uid_t process_user_ID;
    gid_t process_group_ID;
    double current_time;
    double loadavg[3];
};

const char *path = "/tmp/t8.sock";

#endif //LAB_5_PART3_H
