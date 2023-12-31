#ifndef INFO_HPP_TIM_AD
#define INFO_HPP_TIM_AD

#include <stdio.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>

enum ret_codes
{
    RETURN_OK   = 0,
    NO_FEEDBACK = 1,
    SERVER_FULL = 2,
    SUCCESS     = 3,
    USER_QUITS  = 4,
};

// #define DEBUG
#define INIT_USR_NUM 100
#define PREFIX_TEXT_LENGTH 60
#define MAX_TEXT_LENGTH 70 * INIT_USR_NUM
#define MSG_TOTAL_LENGHT (MAX_TEXT_LENGTH + PREFIX_TEXT_LENGTH)
#define MSG_SIZE (sizeof(pid_t) * 2 + sizeof(size_t) + sizeof(char) * MSG_TOTAL_LENGHT)


enum MSG_types
{
    USER_INIT_TYPE    = 1,
    USER_DEL_TYPE     = 2,
    USER_MSG_SND_TYPE = 3,
    USER_GET_LIST     = 4,
};

enum flags
{
    NOTIFY_NEW  = 1,
    NOTIFY_OFF  = 2,
    MSG_FOR_ALL = 3,
    MSG_PRIVATE = 4,
    USER_LIST   = 5,
};

typedef struct USR_struct
{
    char usr_name[21];
    pid_t usr_id = 0;
}USR_struct;

typedef struct SRV_struct
{
    size_t user_number = 0;
    size_t free_cell   = 0;
    USR_struct users[INIT_USR_NUM];
}SRV_struct;

typedef struct MSG_struct
{
    long int msg_type;
    pid_t receiver_id;
    pid_t sender_id;
    size_t logic_package;
    char text[MSG_TOTAL_LENGHT]; 
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
