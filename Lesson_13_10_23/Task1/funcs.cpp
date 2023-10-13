#include "funcs.hpp"

void analog_P(const int msg_id, Message* msg_struct_ptr)
{
    int ret_val = -1;
    while(ret_val != 0)
    {
        ret_val = msgrcv(msg_id, msg_struct_ptr, 0, msg_type, 0);
    }
    printf("msgrcv ret_val = %d\n", ret_val);
}


void analog_V(const int msg_id, Message* msg_struct_ptr)
{
    msg_struct_ptr->msg_type = msg_type;
    int ret_val = msgsnd(msg_id, msg_struct_ptr, 0, 0);
    printf("msgsnd ret_val = %d\n", ret_val);
}

int get_msg_id() // ok
{
    umask(0);

    const int msg_key = ftok("main.cpp", 4);
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



