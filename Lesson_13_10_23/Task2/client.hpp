#ifndef CLIENT_HPP_TIM_AD
#define CLIENT_HPP_TIM_AD

#include "info.hpp"

int try_to_enter_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr)
{
    msg_struct_ptr->msg_type    = USER_INIT_TYPE;
    msg_struct_ptr->sender_id   = usr_struct_ptr->usr_id;
    strcpy(msg_struct_ptr->text, usr_struct_ptr->usr_name);

    // printf("======\n");
    // printf("msg_struct_ptr->msg_type    = %ld\n", msg_struct_ptr->msg_type);
    // printf("msg_struct_ptr->sender_id   = %ld\n", msg_struct_ptr->sender_id);
    // printf("msg_struct_ptr->receiver_id = %ld\n", msg_struct_ptr->receiver_id);
    // printf("msg_struct_ptr-> = %s\n", msg_struct_ptr->text);
    // printf("======\n");
    // printf("msg_id %d\n", msg_id);
    // printf("msg_size %d\n", MSG_SIZE);

    int ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);

    if (ret_val == -1)
    {
        printf("errno %d\n", errno);
        printf("ERROR: cannot send message for user initialization to the server.\n");
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
    printf("c - to chat with others\n");
    printf("q - to quit the messanger\n");
    printf("h - to print help one more time\n");
    printf("==================HELP==================\n");
}

int leave_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr)
{
    msg_struct_ptr->msg_type    = USER_DEL_TYPE;
    msg_struct_ptr->sender_id   = usr_struct_ptr->usr_id;
    msg_struct_ptr->receiver_id = 0;

    // printf("======\n");
    // printf("msg_struct_ptr->msg_type    = %ld\n", msg_struct_ptr->msg_type);
    // printf("msg_struct_ptr->sender_id   = %ld\n", msg_struct_ptr->sender_id);
    // printf("msg_struct_ptr->receiver_id = %ld\n", msg_struct_ptr->receiver_id);
    // printf("======\n");
    // printf("msg_id %d\n", msg_id);

    int ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);

    if (ret_val == -1)
    {
        printf("errno %d\n", errno);
        printf("ERROR: cannot send message for user deletion to the server.\n");
        return -1;
    }
}

int client_logic(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr)
{
    char key = 0;
    print_help();
    while (key != 'q')
    {
        printf("\nEnter the command: ");
        scanf(" %c", &key);

        switch (key)
        {
        case ('c'):
            {
                int cur_pid = getpid();
                int fork_val = fork();

                if(fork_val == -1)
                {
                    printf("Cannot start new process\n");
                }
                else
                {   
                    // #ifdef DEBUG     
                    //     printf("Current process id: %ld\n", getpid());
                    //     printf("Current process PARENT id: %ld\n", getppid());
                    //     printf("fork_val = %ld\n\n", fork_val);
                    // #endif

                    int new_pid = getpid();
                    int new_ppid = getppid();

                    bool is_chatting = true;
                    while (is_chatting)
                    {
                        if(new_ppid == cur_pid)
                        {
                            int ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, usr_struct_ptr->usr_id, IPC_NOWAIT);
                            if(ret_val != -1 && msg_struct_ptr->sender_id == 0)
                            {
                                printf("\n====NOTIFICATION====\n");
                                printf("%s\n", msg_struct_ptr->text);
                                printf("====NOTIFICATION====\n\n");
                            }
                            else if(ret_val != -1)
                            {
                                printf("%s\n", msg_struct_ptr->text);
                            }
                        }
                        else if(new_pid == cur_pid)
                        {
                            msg_struct_ptr->logic_package = MSG_PRIVATE;
                            char text[MAX_TEXT_LENGTH];

                            int receiver_id;
                            scanf("%d ", &receiver_id);
                            msg_struct_ptr->receiver_id = receiver_id;
                            // printf("msg_struct_ptr->receiver_id %ld\n", msg_struct_ptr->receiver_id);

                            scanf("%[^\n]s", text);
                            text[MAX_TEXT_LENGTH - 1] = '\0';
                            if(strcasecmp("quit", text) == 0)
                            {
                                is_chatting = false;
                                key = 'q';
                                break;
                            }
                            // printf("text %s\n", text);
                            
                            msg_struct_ptr->msg_type  = USER_MSG_SND_TYPE;
                            msg_struct_ptr->sender_id = usr_struct_ptr->usr_id;

                            if(receiver_id == 0)
                            {
                                msg_struct_ptr->logic_package = MSG_FOR_ALL;
                            }
                            
                            sprintf(msg_struct_ptr->text, "From %s (id %ld): %s\n", usr_struct_ptr->usr_name, msg_struct_ptr->sender_id, text);
                            printf("TEXT SENT: %s", msg_struct_ptr->text);

                            int ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
                            if (ret_val == -1)
                            {
                                printf("errno %d\n", errno);
                                printf("ERROR: cannot send the message to the server\n");
                                return -1;
                            }
                            {
                                printf("Message was sent\n");
                            }
                        }
                    }
                }
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
        printf("ERROR: cannot get the feedback package from the server.\n");
        return NO_FEEDBACK;
    }
    else
    {
        if (msg_struct_ptr->logic_package == SERVER_FULL)
        {
            printf("\nServer is full, try to connect to the server later.\n");
            return NO_FEEDBACK;
        }
        else
        {
            printf("You was connected to the server.\n");
            return SUCCESS;
        }
    }
}

int connect_to_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr)
{
    try_to_enter_web(msg_id, msg_struct_ptr, usr_struct_ptr);
    int ret = get_package_from_server(msg_id, msg_struct_ptr, usr_struct_ptr);

    if(ret != SUCCESS)
    {   
        char key = 0;
        while (key != 'q')
        {
            printf("Enter the command 'c' to connect to the server or 'q' to quit: ");
            scanf(" %c", &key);

            switch (key)
            {
            case ('c'):
                {
                    try_to_enter_web(msg_id, msg_struct_ptr, usr_struct_ptr);
                    ret = get_package_from_server(msg_id, msg_struct_ptr, usr_struct_ptr);
                    
                    if(ret == SUCCESS)
                    {
                        return SUCCESS;
                    }

                    printf("Enable to connect to the server right now, try again later.\n");;
                    break;
                }
            case ('q'):
                {
                    return USER_QUITS;
                    break;
                }
            default:
                {
                    printf("Invalid command, try one more time\n");
                }
            }
        }   
    }

    return SUCCESS;
}

#endif
