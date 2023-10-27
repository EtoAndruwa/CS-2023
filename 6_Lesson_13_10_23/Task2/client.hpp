#ifndef CLIENT_HPP_TIM_AD
#define CLIENT_HPP_TIM_AD

#include "info.hpp"

int get_user_list(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr) // ?
{
    msg_struct_ptr->msg_type  = USER_GET_LIST;
    msg_struct_ptr->sender_id = usr_struct_ptr->usr_id;

    int ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
    if (ret_val == -1)
    {
        printf("errno %d\n", errno);
        printf("ERROR: cannot get user list. Try again a bit later.\n");
    }
}

void get_msg(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr) // ok
{
    int ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, usr_struct_ptr->usr_id, IPC_NOWAIT);
    if (ret_val != -1 && msg_struct_ptr->sender_id == 0)
    {
        printf("\n====NOTIFICATION====\n");
        printf("%s\n", msg_struct_ptr->text);
        printf("====NOTIFICATION====\n\n");
    }
    else if (ret_val != -1)
    {
        if (msg_struct_ptr->logic_package == MSG_PRIVATE)
        {
            printf("(PRIVATE MESSAGE) ");
        }
        else if (msg_struct_ptr->logic_package == MSG_FOR_ALL)
        {
            printf("(GLOBAL MESSAGE) ");
        }
        printf("%s\n", msg_struct_ptr->text);
    }
}

int try_to_enter_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr) // ok
{
    msg_struct_ptr->msg_type  = USER_INIT_TYPE;
    msg_struct_ptr->sender_id = usr_struct_ptr->usr_id;
    sprintf(msg_struct_ptr->text, "%s", usr_struct_ptr->usr_name);

    int ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
    if (ret_val == -1)
    {
        printf("errno %d\n", errno);
        printf("ERROR: cannot send message for user initialization to the server.\n");
    }
}

int usr_ctor(USR_struct* usr_struct_ptr) // ok
{
    usr_struct_ptr->usr_id = getpid();
    printf("Hello! Enter your username that contains up to 20 chars: ");
    scanf("%s", usr_struct_ptr->usr_name);
}

void print_user(const USR_struct* const usr_struct_ptr) // ok
{
    printf("\n==========USER STRUCT DUMP==========\n");
    printf("usr_struct_ptr->usr_name: %s\n" , usr_struct_ptr->usr_name);
    printf("usr_struct_ptr->usr_id: %ld\n" , usr_struct_ptr->usr_id);
    printf("==========USER STRUCT DUMP==========\n");
}

void print_help() // ok
{
    printf("\n==================HELP==================\n");
    printf("c - to chat with others\n");
    printf("q - to quit the messanger\n");
    printf("h - to print help one more time\n");
    printf("==================HELP==================\n");
}

void print_help_chat() // ok
{
    printf("\n==================HELP==================\n");

    printf("\n-------NOTE-------\n");
    printf("Template for the message: user_id + text + enter\n");
    printf("Print \'0\' if you want to send the message to all users in the web\n");
    printf("Example: 13902 Hello! - this message will be sent to the user with id 13902\n");
    printf("-------NOTE-------\n\n");   

    printf("quit - to quit the messanger\n");
    printf("help - to print help one more time\n");
    printf("list - to print the list of online users\n");
    printf("==================HELP==================\n\n");
}

int leave_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr) // ok
{
    msg_struct_ptr->msg_type    = USER_DEL_TYPE;
    msg_struct_ptr->sender_id   = usr_struct_ptr->usr_id;
    msg_struct_ptr->receiver_id = 0;

    int ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);

    if (ret_val == -1)
    {
        printf("errno %d\n", errno);
        printf("ERROR: cannot send message for user deletion to the server.\n");
    }
}

int client_logic(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr) // ok
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
                print_help_chat();
                int fork_val = fork();

                if (fork_val == -1)
                {
                    printf("ERROR: cannot enter the chat. Try again a bit later.\n");
                }
                else
                {   
                    int new_pid = getpid();
                    int new_ppid = getppid();

                    bool is_chatting = true;
                    while (is_chatting)
                    {
                        if (new_ppid == cur_pid)
                        {
                            get_msg(msg_id, msg_struct_ptr, usr_struct_ptr);
                        }
                        else if (new_pid == cur_pid)
                        {
                            msg_struct_ptr->logic_package = MSG_PRIVATE;
                            char text[MAX_TEXT_LENGTH];

                            int receiver_id;
                            scanf("%d ", &receiver_id);
                            msg_struct_ptr->receiver_id = receiver_id;

                            scanf("%[^\n]s", text);
                            text[MAX_TEXT_LENGTH - 1] = '\0';

                            if (strcasecmp("quit", text) == 0) // if user wants to quit from the chat
                            {
                                is_chatting = false;
                                key = 'q';
                                strcpy(text, "");
                                break;
                            }
                            if (strcasecmp("help", text) == 0) // if user wants to quit from the chat
                            {
                                print_help_chat();
                                strcpy(text, "");
                                continue;
                            }
                            if (strcasecmp("list", text) == 0) // if user wants to quit from the chat
                            {
                                get_user_list(msg_id, msg_struct_ptr, usr_struct_ptr);
                                strcpy(text, "");
                                continue;
                            }
                            
                            msg_struct_ptr->msg_type  = USER_MSG_SND_TYPE;
                            msg_struct_ptr->sender_id = usr_struct_ptr->usr_id;

                            if (receiver_id == 0)
                            {
                                msg_struct_ptr->logic_package = MSG_FOR_ALL;
                            }
                            
                            sprintf(msg_struct_ptr->text, "From %s (id %ld): %s", usr_struct_ptr->usr_name, msg_struct_ptr->sender_id, text);

                            int ret_val = msgsnd(msg_id, msg_struct_ptr, MSG_SIZE, 0);
                            if (ret_val == -1)
                            {
                                printf("errno %d\n", errno);
                                printf("ERROR: cannot send the message. Try again a bit later.\n");
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
                printf("ERROR: invalid command, try one more time.\n");
                print_help();
            }
        }
    }

    return RETURN_OK;
}

int get_package_from_server(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr) // ok
{
    int ret_val = msgrcv(msg_id, msg_struct_ptr, MSG_SIZE, usr_struct_ptr->usr_id, 0);

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
            printf("ERROR: server is full, try to connect to the server later.\n");
            return NO_FEEDBACK;
        }
        else
        {
            printf("You was connected to the server.\n");
            return SUCCESS;
        }
    }
}

int connect_to_web(const int msg_id, MSG_struct* msg_struct_ptr, USR_struct* usr_struct_ptr) // ok
{
    try_to_enter_web(msg_id, msg_struct_ptr, usr_struct_ptr);
    int ret = get_package_from_server(msg_id, msg_struct_ptr, usr_struct_ptr);

    if (ret != SUCCESS)
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
                    
                    if (ret == SUCCESS)
                    {
                        return SUCCESS;
                    }

                    printf("ERROR: enable to connect to the server right now, try again later.\n");;
                    break;
                }
            case ('q'):
                {
                    return USER_QUITS;
                    break;
                }
            default:
                {
                    printf("ERROR: invalid command, try one more time.\n");
                }
            }
        }   
    }
}

#endif
