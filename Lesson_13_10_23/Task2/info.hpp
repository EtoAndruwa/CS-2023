#ifndef INFO_HPP_TIM_AD
#define INFO_HPP_TIM_AD

#include <stdio.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define INIT_USR_NUM 100
#define MAX_TEXT_LENGTH 100

enum MSG_types
{
    USER_INIT_TYPE    = 0,
    USER_DEL_TYPE     = 1,
    USER_MSG_SND_TYPE = 2,
};

typedef struct USR_str
{
    char usr_name[21];
    char usr_id = 0;
}USR_str;

typedef struct SRV_struct
{
    size_t user_number = 0;
    USR_str users[INIT_USR_NUM];
}SRV_struct;

typedef struct MSG
{
    long int msg_type;
    size_t receiver_id;
    size_t sender_id;
    char text[MAX_TEXT_LENGTH]; 
}MSG_struct;


int get_msg_id() // ok
{
    umask(0);

    const int msg_key = ftok("server.cpp", 4);
    #ifdef DEBUG
        printf("msg_key %d\n", msg_key);
    #endif
    int msg_id = msgget(msg_key, 0666);
    if(msg_id == -1)
    {
        #ifdef DEBUG
            printf("msg_key 0666|IPC_CREAT\n");
        #endif
        msg_id = msgget(msg_key, 0666|IPC_CREAT);;
    }
    #ifdef DEBUG
        printf("msg_id %d\n", msg_id);
        printf("msgget 0666\n");
    #endif

    return msg_id;
}



#endif
