#ifndef CLIENT_HPP_TIM_AD
#define CLIENT_HPP_TIM_AD

#include "info.hpp"

int try_to_enter_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr)
{
    msg_struct_ptr->msg_type    = USER_INIT_TYPE;
    msg_struct_ptr->sender_id   = usr_struct_ptr->usr_id;
    strcpy(msg_struct_ptr->text, usr_struct_ptr->usr_name);

    printf("======\n");
    printf("msg_struct_ptr->msg_type    = %ld\n", msg_struct_ptr->msg_type);
    printf("msg_struct_ptr->sender_id   = %ld\n", msg_struct_ptr->sender_id);
    printf("msg_struct_ptr->receiver_id = %ld\n", msg_struct_ptr->receiver_id);
    printf("msg_struct_ptr-> = %s\n", msg_struct_ptr->text);
    printf("======\n");

    printf("msg_id %d\n", msg_id);

    printf("msg_size %d\n", MSG_SIZE);
    int ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);

    if (ret_val == -1)
    {
        printf("errno %d\n", errno);
        printf("ERROR: cannot send message for user initialization\n");
        return -1;
    }
}

int usr_ctor(USR_struct* usr_struct_ptr)
{
    usr_struct_ptr->usr_id = getpid();
    printf("Hello! Enter your username that contains up to 20 chars: ");
    scanf("%s", usr_struct_ptr->usr_name);
}

void print_user(const USR_struct* const usr_struct_ptr)
{
    printf("\n==========USER STRUCT DUMP==========\n");
    printf("usr_struct_ptr->usr_name: %s\n" , usr_struct_ptr->usr_name);
    printf("usr_struct_ptr->usr_id: %ld\n" , usr_struct_ptr->usr_id);
    printf("==========USER STRUCT DUMP==========\n");
}

void print_help()
{
    printf("\n==================HELP==================\n");
    printf("s - to send the message\n");
    printf("q - to quit the messanger\n");
    printf("h - to print help one more time\n");
    printf("==================HELP==================\n");
}

int leave_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr)
{
    msg_struct_ptr->msg_type    = USER_DEL_TYPE;
    msg_struct_ptr->sender_id   = usr_struct_ptr->usr_id;
    msg_struct_ptr->receiver_id = 0;

    printf("======\n");
    printf("msg_struct_ptr->msg_type    = %ld\n", msg_struct_ptr->msg_type);
    printf("msg_struct_ptr->sender_id   = %ld\n", msg_struct_ptr->sender_id);
    printf("msg_struct_ptr->receiver_id = %ld\n", msg_struct_ptr->receiver_id);
    printf("======\n");

    printf("msg_id %d\n", msg_id);

    int msg_size = sizeof(msg_struct_ptr->sender_id) + sizeof(msg_struct_ptr->receiver_id) + sizeof(msg_struct_ptr->text);
    printf("msg_size %d\n", msg_size);
    int ret_val = msgsnd(msg_id, msg_struct_ptr, msg_size, 0);

    if (ret_val == -1)
    {
        printf("errno %d\n", errno);
        printf("ERROR: cannot send message for user deletion\n");
        return -1;
    }
}

int client_logic()
{
    char key = 0;
    print_help();
    while (key != 'q')
    {
        printf("\nEnter the command: ");
        scanf(" %c", &key);

        switch (key)
        {
        case ('s'):
            {

                break;
            }
        case ('h'):
            {
                print_help();
                break;
            }
        case ('q'):
            {
                break;
            }
        default:
            {
                printf("Invalid command, try one more time\n");
                print_help();
            }
        }
    }

    return RETURN_OK;
}

int get_package_from_server(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr)
{
    int ret_val = -1;
    ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, usr_struct_ptr->usr_id, 0);

    if (ret_val == -1)
    {
        printf("errno %d\n", errno);
        printf("ERROR: cannot get the feedback package from the server\n");
        return NO_FEEDBACK;
    }
    else
    {
        if (msg_struct_ptr->logic_package == SERVER_FULL)
        {
            printf("Server is full, try to connect to the server later\n");
            return NO_FEEDBACK;
        }
        else
        {
            printf("You was connected to the server\n");
            return SUCCESS;
        }
    }
}

void connect_to_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr)
{
    bool coonected = false;
    while(!coonected)
    {
        try_to_enter_web(msg_id, msg_struct_ptr, usr_struct_ptr);
        int ret = get_package_from_server(msg_id, msg_struct_ptr, usr_struct_ptr);

        if(ret == SUCCESS)
        {
            coonected = true;
        }
    }
}

#endif
