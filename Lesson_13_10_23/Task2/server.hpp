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
    char notify_text[60];
    int ret_val = 0;
    msg_struct_ptr->sender_id = 0;
    size_t found_usr_id = 0;

    for(size_t i = 0; i < srv_struct_ptr->user_number; i++)
    {
        if(srv_struct_ptr->users[i].usr_id == usr_id)
        {
            found_usr_id = i;
        }
    }

    for(size_t i = 0; i < srv_struct_ptr->user_number; i++)
    {
        if(srv_struct_ptr->users[i].usr_id == usr_id)
        {
            continue;
        }
        if(srv_struct_ptr->users[i].usr_id != 0 && notif_flag == NOTIFY_NEW)
        {
            msg_struct_ptr->msg_type = srv_struct_ptr->users[i].usr_id;
            sprintf(notify_text, "User \'%s\' with id = \'%ld\' is now online!", srv_struct_ptr->users[found_usr_id].usr_name, usr_id);
            printf("Text: %s\n", notify_text);

            strcpy(msg_struct_ptr->text, notify_text);

            ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
            if (ret_val == -1)
            {
                printf("errno %d\n", errno);
                printf("ERROR: cannot notify the user with id %d\n", srv_struct_ptr->users[i].usr_id);
                return -1;
            }
        }
        if(srv_struct_ptr->users[i].usr_id != 0 && notif_flag == NOTIFY_OFF)
        {
            msg_struct_ptr->msg_type = srv_struct_ptr->users[i].usr_id;
            sprintf(notify_text, "User \'%s\' with id = \'%ld\' is now offline.", srv_struct_ptr->users[found_usr_id].usr_name, usr_id);
            printf("Text: %s\n", notify_text);

            strcpy(msg_struct_ptr->text, notify_text);

            ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
            if (ret_val == -1)
            {
                printf("errno %d\n", errno);
                printf("ERROR: cannot notify the user with id %d\n", srv_struct_ptr->users[i].usr_id);
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

            printf("\n++++++++++++++++++++USER ADDED++++++++++++++++++++\n");
            srv_struct_print(srv_struct_ptr);
            printf("++++++++++++++++++++USER ADDED++++++++++++++++++++\n");
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
    }
}

int check_for_offline(const int msg_id, MSG_struct* msg_struct_ptr, SRV_struct* srv_struct_ptr)
{
    int ret_val = -1; 
    ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, USER_DEL_TYPE, IPC_NOWAIT);

    if(ret_val != -1)
    {
        pid_t usr_id = msg_struct_ptr->sender_id;
        notify_all(msg_id, msg_struct_ptr->sender_id, NOTIFY_OFF, msg_struct_ptr, srv_struct_ptr);

        for (size_t i = 0; i < srv_struct_ptr->user_number; i++)
        {
            if (srv_struct_ptr->users[i].usr_id == usr_id)
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
    }
}

int check_for_retake(const int msg_id, MSG_struct* msg_struct_ptr, SRV_struct* srv_struct_ptr)
{
    int ret_val = -1; 
    ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, USER_MSG_SND_TYPE, IPC_NOWAIT);

    if(ret_val != -1)
    {
        if(msg_struct_ptr->logic_package == MSG_FOR_ALL)
        {
            printf("MSG_FOR_ALL\n");
            for(size_t i = 0; i < srv_struct_ptr->user_number; i++)
            {
                if(srv_struct_ptr->users[i].usr_id == msg_struct_ptr->sender_id)
                {
                    continue;
                }
                if(srv_struct_ptr->users[i].usr_id != 0)
                {
                    msg_struct_ptr->receiver_id = srv_struct_ptr->users[i].usr_id;
                    msg_struct_ptr->msg_type    = srv_struct_ptr->users[i].usr_id;

                    // printf("srv_struct_ptr->users[%d].usr_id = %d\n", i, srv_struct_ptr->users[i].usr_id);
                    // printf("msg_struct_ptr->logic_package: %d\n", msg_struct_ptr->logic_package);
                    // printf("msg_struct_ptr->msg_type: %d\n", msg_struct_ptr->msg_type);
                    // printf("msg_struct_ptr->receiver_id: %d\n", msg_struct_ptr->receiver_id);
                    // printf("msg_struct_ptr->sender_id: %d\n", msg_struct_ptr->sender_id);
                    // printf("msg_struct_ptr->text: %s\n", msg_struct_ptr->text);

                    ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
                    if (ret_val == -1)
                    {
                        printf("errno %d\n", errno);
                        printf("ERROR: cannot resend the message to the user with id %d\n", srv_struct_ptr->users[i].usr_id);
                    }
                }
            } 
        }
        else if(msg_struct_ptr->logic_package == MSG_PRIVATE)
        {
            printf("MSG_PRIVATE\n");
            msg_struct_ptr->msg_type = msg_struct_ptr->receiver_id;

            ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
            if (ret_val == -1)
            {
                printf("errno %d\n", errno);
                printf("ERROR: cannot resend the message to the user with id %d\n", msg_struct_ptr->receiver_id);
                return -1;
            }
        }
    }
}


#endif
