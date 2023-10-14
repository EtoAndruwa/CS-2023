#ifndef SERVER_HPP_TIM_AD
#define SERVER_HPP_TIM_AD

#include "info.hpp"

void srv_ctor(SRV_struct* srv_struct_ptr)
{
    for (size_t i = 0; i < srv_struct_ptr->user_number; i++)
    {
        strcpy(srv_struct_ptr->users[i].usr_name, "");
    }
}

void srv_struct_print(const SRV_struct* const srv_struct_ptr)
{
    printf("\n============SRV_DUMP============\n");
    printf("srv_struct_ptr->user_number = %ld\n" , srv_struct_ptr->user_number);
    printf("srv_struct_ptr->users       = %p\n", srv_struct_ptr->users);


    printf("\n+++++++USER DATA+++++++\n");
    for(size_t i = 0; i < srv_struct_ptr->user_number; i++)
    {   
        printf("users[%ld]: usr_name = \'%s\', usr_id = %d\n", i, srv_struct_ptr->users[i].usr_name, srv_struct_ptr->users[i].usr_id);
    }
    printf("+++++++USER DATA+++++++\n");
    printf("============SRV_DUMP============\n");
}

int notify_all(const int msg_id, const pid_t usr_id, const size_t notif_flag, MSG_struct* msg_struct_ptr, SRV_struct* srv_struct_ptr)
{
    char notify_text[50];
    int ret_val = 0;
    msg_struct_ptr->sender_id = 0;

    for(size_t i = 0; i < srv_struct_ptr->user_number; i++)
    {
        if(srv_struct_ptr->users[i].usr_id == usr_id)
        {
            continue;
        }
        if(srv_struct_ptr->users[i].usr_id != 0 && notif_flag == NOTIFY_NEW)
        {
            msg_struct_ptr->msg_type = srv_struct_ptr->users[i].usr_id;
            sprintf(notify_text, "\nUser %ld is now online!\n", usr_id);
            printf("Text: %s\n", notify_text);

            strcpy(msg_struct_ptr->text, notify_text);

            ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
            if (ret_val == -1)
            {
                printf("errno %d\n", errno);
                printf("ERROR: cannot notify the user \n", srv_struct_ptr->users[i].usr_id);
                return -1;
            }
        }
        if(srv_struct_ptr->users[i].usr_id != 0 && notif_flag == NOTIFY_OFF)
        {
            msg_struct_ptr->msg_type = srv_struct_ptr->users[i].usr_id;
            sprintf(notify_text, "\nUser %ld is now offline.\n", usr_id);
            printf("Text: %s\n", notify_text);

            strcpy(msg_struct_ptr->text, notify_text);

            ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
            if (ret_val == -1)
            {
                printf("errno %d\n", errno);
                printf("ERROR: cannot notify the user \n", srv_struct_ptr->users[i].usr_id);
                return -1;
            }
        }
    }
}

int check_for_new(const int msg_id, MSG_struct* msg_struct_ptr, SRV_struct* srv_struct_ptr)
{
    int ret_val = -1; 
    ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, USER_INIT_TYPE, IPC_NOWAIT);

    if(ret_val != -1)
    {
        if (srv_struct_ptr->user_number != INIT_USR_NUM)
        {
            srv_struct_ptr->users[srv_struct_ptr->user_number].usr_id = msg_struct_ptr->sender_id;
            strcpy(srv_struct_ptr->users[srv_struct_ptr->user_number].usr_name, msg_struct_ptr->text);
            srv_struct_ptr->user_number++;
            srv_struct_ptr->free_cell++;


            msg_struct_ptr->logic_package = SUCCESS;
            msg_struct_ptr->msg_type = msg_struct_ptr->sender_id;

            ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
            if (ret_val == -1)
            {
                printf("errno %d\n", errno);
                printf("ERROR: cannot send feedback to the user\n");
                return -1;
            }

            notify_all(msg_id, msg_struct_ptr->sender_id, NOTIFY_NEW, msg_struct_ptr, srv_struct_ptr);
        }
        else
        {
            printf("Unable to connect user to the server\n");
            printf("The signal will be sent to the user\n");

            msg_struct_ptr->logic_package = SERVER_FULL;
            msg_struct_ptr->msg_type = msg_struct_ptr->sender_id;

            ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
            if (ret_val == -1)
            {
                printf("errno %d\n", errno);
                printf("ERROR: cannot send feedback to the user\n");
                return -1;
            }
        }

        printf("\n++++++++++++++++++++USER ADDED++++++++++++++++++++\n");
        srv_struct_print(srv_struct_ptr);
        printf("++++++++++++++++++++USER ADDED++++++++++++++++++++\n");
    }
}

int check_for_offline(const int msg_id, MSG_struct* msg_struct_ptr, SRV_struct* srv_struct_ptr)
{
    int ret_val = -1; 
    ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, USER_DEL_TYPE, IPC_NOWAIT);

    if(ret_val != -1)
    {
        for (size_t i = 0; i < srv_struct_ptr->user_number; i++)
        {
            if (srv_struct_ptr->users[i].usr_id == msg_struct_ptr->sender_id)
            {
                srv_struct_ptr->users[i].usr_id = -1;
                strcpy(srv_struct_ptr->users[i].usr_name, "");
                srv_struct_ptr->user_number--;
                srv_struct_ptr->free_cell = i;
                break;
            }
        }

        printf("\n----------------USER DELETED----------------\n");
        srv_struct_print(srv_struct_ptr);
        printf("----------------USER DELETED----------------\n");

        notify_all(msg_id, msg_struct_ptr->sender_id, NOTIFY_OFF, msg_struct_ptr, srv_struct_ptr);
    }
}

int check_for_retake(const int msg_id, MSG_struct* msg_struct_ptr, SRV_struct* srv_struct_ptr)
{
    int ret_val = -1; 
    ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, USER_MSG_SND_TYPE, IPC_NOWAIT);

    if(ret_val != -1)
    {


    }
}


#endif
