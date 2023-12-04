#include "client.h"

void auto_get_ip(char* const ip_str) // ok
{
    int interf_fd;
    struct ifreq ifr;
    char interf_str[] = "wlo1"; // wifi interface
 
    interf_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, interf_str, IFNAMSIZ - 1);
    ioctl(interf_fd, SIOCGIFADDR, &ifr); // gets the IP address of the device
    close(interf_fd);
    sprintf(ip_str, "%s", inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr)); // converts back to the string
}

void ctor_client(Client_struct* const client_struct) // ok
{
    #ifdef LOOP_BACK
        strcpy(client_struct->ip_str, "INADDR_LOOPBACK");
    #else
        auto_get_ip(client_struct->ip_str);
    #endif

    printf("\n==================INFO==================\n");
    printf("Your IP: %s\n", client_struct->ip_str);
    client_struct->port = DEFAULT_CLIENT_PORT;
    printf("Your default PORT: %d\n", client_struct->port);
    printf("==================INFO==================\n");

    char c[2];
    printf("\nWould you like to change the default port value? (y/n): ");
    scanf("%s", c);

    switch (c[0])
    {
    case 'y':
        {
            change_port(client_struct);
            break;
        }
    case 'n':
        {
            break;
        }
    default:
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: invalid command, port will remain default.\nYou can change your port later anyway\n");
        printf("==================ERROR MESSAGE==================\n");
        break;
    }

    printf("\nEnter your login here (up to %d chars): ", MAX_LOGIN_LENGTH - 1);
    scanf("%s", client_struct->login);

    printf("\n==================INFO==================\n");
    client_struct->login[MAX_LOGIN_LENGTH - 1] = '\0';
    printf("Your login: %s\n", client_struct->login);
    printf("==================INFO==================\n");
}

void change_port(Client_struct* const client_struct) // ok
{
    int port = 0;
    printf("\nEnter new PORT: ");
    scanf("%d", &port);

    if (port >= 0)
    {   
        printf("\n==============NOTIFICATION==============\n");
        printf("Port changed from\n%d to %d\n", client_struct->port, port);
        printf("==============NOTIFICATION==============\n");
        client_struct->port = port;
    }
    else
{        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: invalid port value.\nYour port will remain %d\n", client_struct->port);
        printf("==================ERROR MESSAGE==================\n");
    }
}

void dtor_client(Client_struct* const client_struct) // ok
{
    strcpy(client_struct->ip_str, "");
    strcpy(client_struct->login, "");
    client_struct->port = 0;
}

void print_greeting() // ok
{
    printf("\n=================GREEETING=================\n");
    printf("Welcome to the RLW - Real Local Web\n");
    printf("To start chatting with others we\n");
    printf("need identify you. IP address will\n");
    printf("be automatically identified. You \n");
    printf("can enter login and port. Server IP\n");
    printf("and port will be set to default values\n");
    printf("=================GREEETING=================\n");
}

void print_client_data(const Client_struct* const client_struct) // ok
{
    printf("\n==========MY INFO==========\n");
    printf("LOGIN: %s\n", client_struct->login);
    printf("IP: %s\n", client_struct->ip_str);
    printf("PORT: %d\n", client_struct->port);
    printf("===========MY INFO==========\n");
}

int web_logic(Client_struct* const client_struct, Server_struct* const server_struct)
{
    print_web_help();
    char c[3];

    while (c[0] != 'q')
    {
        printf("\nEnter your command here: ");
        scanf("%s", c);

        switch (c[0])
        {
        case 'q':
            {
                break;
            }
        case 'h':
            {
                print_web_help();
                break;
            }
        case 'p':
            {
                switch (c[1])
                {
                case 'c':
                    {
                        print_client_data(client_struct);
                        break;
                    }
                case 's':
                    {
                        print_server_data(server_struct);
                        break;
                    }
                
                default:
                    printf("\n==================ERROR MESSAGE==================\n");
                    printf("ERROR: invalid command, try again\n");
                    printf("==================ERROR MESSAGE==================\n");
                    break;
                }
                break;
            }
        case 'c':
            {
                switch (c[1])
                {
                case '\0':
                    {
                        change_port(client_struct);
                        break;
                    }
                case 'p':
                    {
                        change_server_port(server_struct);
                        break;
                    }
                case 'i':
                    {
                        change_server_ip(server_struct);
                        break;
                    }
                default:
                    printf("\n==================ERROR MESSAGE==================\n");
                    printf("ERROR: invalid command, try again\n");
                    printf("==================ERROR MESSAGE==================\n");
                    break;
                }
                break;
            }
        case 'm':
            {
                int err_code = create_socket(client_struct);
                if (err_code != RETURN_OK)
                {
                    return err_code;
                }
                connect_to_server(client_struct, server_struct);

                web_inner_logic(client_struct, server_struct);

                disconnect_from_server(client_struct, server_struct);
                print_web_help();
                close(client_struct->socket_fd);
                break;
            }
        default:
            printf("\n==================ERROR MESSAGE==================\n");
            printf("ERROR: invalid command, try again\n");
            printf("==================ERROR MESSAGE==================\n");
            break;
        }
    }
}

void print_web_help() // ok
{
    printf("\n=========================HELP==========================\n");
    printf("h  - print help one more time\n");
    printf("c  - change my port\n");
    printf("pc - print my data\n");
    printf("ps - print server data\n");
    printf("cp - set new server port\n");
    printf("ci - set new server ip\n");
    printf("m  - start messaging\n");
    printf("q  - quit the program\n");
    printf("=========================HELP==========================\n");
}

void ctor_default_server(Server_struct* const server_struct) // ok
{
    server_struct->port = DEFAULT_SERVER_PORT;
    
    #ifdef LOOP_BACK
        strcpy(server_struct->ip_str, "INADDR_LOOPBACK");
    #else
        strcpy(server_struct->ip_str, DEFAULT_SERVER_IP);
    #endif
}

void dtor_default_server(Server_struct* const server_struct) // ok
{
    server_struct->port = 0;
    strcpy(server_struct->ip_str, "");
}

void print_server_data(const Server_struct* const server_struct) // ok
{
    printf("\n==========SERVER INFO===========\n");
    printf("IP: %s\n", server_struct->ip_str);
    printf("PORT: %d\n", server_struct->port);
    printf("==========SERVER INFO===========\n");
}

void change_server_ip(Server_struct* const server_struct) // ok
{
    printf("\nEnter new server IP: ");
    char temp_ip[MAX_IP_LENGTH];
    scanf("%s", temp_ip);
    temp_ip[MAX_IP_LENGTH - 1] = '\0';

    if (!stpcpy(temp_ip, server_struct->ip_str))
    {
        printf("\n==============NOTIFICATION==============\n");
        printf("Entered server IP is the same one,\nIP will remain unchanged\n");
        printf("==============NOTIFICATION==============\n");
    }
    else
    {
        printf("\n==============NOTIFICATION==============\n");
        printf("Server IP was changed\nfrom %s to %s\n", server_struct->ip_str, temp_ip);
        printf("==============NOTIFICATION==============\n");
        strcpy(server_struct->ip_str, temp_ip);
    }
}

void change_server_port(Server_struct* const server_struct) // ok
{
    printf("\nEnter new server PORT: ");
    int temp_port;
    scanf("%d", &temp_port);

    if (temp_port >= 0 && temp_port != server_struct->port)
    {
        printf("\n==============NOTIFICATION==============\n");
        printf("Server PORT was changed\nfrom %d to %d\n", server_struct->port, temp_port);
        printf("==============NOTIFICATION==============\n");
        server_struct->port = temp_port;
    }
    else if (temp_port == server_struct->port)
    {
        printf("\n==============NOTIFICATION==============\n");
        printf("You entered the same PORT.\nPORT will remain unchanged\n");
        printf("==============NOTIFICATION==============\n");
    }
    else if (temp_port < 0)
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: entered port value is negative.\nPORT will remain unchanged\n");
        printf("==================ERROR MESSAGE==================\n");
    }
    
}

int connect_to_server(Client_struct* const client_struct, Server_struct* const server_struct)
{
    bool init_success_flag = false;
    while (!init_success_flag)  
    {
        Message msg;

        set_init_msg(&msg, client_struct);
        set_server_sock_struct(server_struct);

        int sendto_ret = sendto(client_struct->socket_fd, (const void*)&msg, sizeof(msg), 0, (const sockaddr*)(&server_struct->sock_struct), sizeof(server_struct->sock_struct));

        if (sendto_ret == -1) 
        {
            printf("\n==================ERROR MESSAGE==================\n");
            printf("ERROR: (ERR_SEND_INIT_MSG) cannot send \nthe init message to the server, try again\n");
            printf("==================ERROR MESSAGE==================\n");
            return ERR_SEND_INIT_MSG;
        }

        socklen_t sock_len = 0;
        struct sockaddr temp_struct;

        int recv_ret = recvfrom(client_struct->socket_fd, (void *)&msg, sizeof(msg), 0, &temp_struct, &sock_len); // does not block
        if (recv_ret != -1)
        {
            if (msg.msg_type == CHANGE_LOGIN)
            {   
                change_login(client_struct);
                continue;
            }
            else if (msg.msg_type == INIT_SUCCESS)
            {
                init_success_flag = true;
                printf("\n==============NOTIFICATION==============\n");
                printf("You was connected to the server\n");
                printf("==============NOTIFICATION==============\n");
            }
            else if (msg.msg_type == INIT_RETRY)
            {
                continue;
            }
        }
        else
        {
            printf("\n==================ERROR MESSAGE==================\n");
            printf("ERROR: (ERR_GET_INIT_RESP) unable to connect \nto the server, try to start messaging one more time\n");
            printf("==================ERROR MESSAGE==================\n");
            return ERR_GET_INIT_RESP;
        }
    }

    return RETURN_OK;
}

void print_msg_data(const Message* const msg) // ok
{
    printf("\n========================MSG DATA========================\n");
    printf("msg_type: %d\n", msg->msg_type);
    printf("msg_text: %s\n", msg->msg_text);
    printf("receiver_ip: %d\n", msg->receiver_ip);
    printf("receiver_login: %s\n", msg->receiver_login);
    printf("receiver_port: %d\n", msg->receiver_port);
    printf("sender_ip: %s\n", msg->sender_ip);
    printf("sender_login: %s\n", msg->sender_login);
    printf("sender_port: %d\n", msg->sender_port);
    printf("========================MSG DATA========================\n");
}

int create_socket(Client_struct* const client_struct) // ok
{
    client_struct->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_struct->socket_fd == -1)
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_GET_SOCKET) get_socket failure\n");
        printf("==================ERROR MESSAGE==================\n");
        return ERR_GET_SOCKET;
    }
    else
    {   
        set_sock_struct(client_struct->ip_str, client_struct->port, &client_struct->ip_addr_client, &client_struct->sock_struct);

        int bind_rt = bind(client_struct->socket_fd, (const sockaddr *)&client_struct->sock_struct, sizeof(client_struct->sock_struct));
        if (bind_rt == -1)
        {
            printf("\n==================ERROR MESSAGE==================\n");
            printf("ERROR: (ERR_BIND_SOCKET) bind_socket failure\n");
            printf("==================ERROR MESSAGE==================\n");
            return ERR_BIND_SOCKET;
        }

        return RETURN_OK;
    }
}

void set_sock_struct(const char* const ip_str, const int port, in_addr* const ip_addr_str, struct sockaddr_in* const sock_str) // ok
{
    #ifdef LOOP_BACK
        sock_str->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    #else
        inet_aton(ip_str, ip_addr_str);
        sock_str->sin_addr.s_addr = ip_addr_str->s_addr;
    #endif
    sock_str->sin_port        = htons(port);
    sock_str->sin_family      = AF_INET;
}

void set_server_sock_struct(Server_struct* const server_struct) // ok
{
    server_struct->port = DEFAULT_SERVER_PORT;

    #ifdef LOOP_BACK    
        strcpy(server_struct->ip_str, "INADDR_LOOPBACK");
    #else
        strcpy(server_struct->ip_str, DEFAULT_SERVER_IP);
    #endif
    
    set_sock_struct(server_struct->ip_str, server_struct->port, &server_struct->ip_addr_server, &server_struct->sock_struct);
}

void set_init_msg(Message* const msg, const Client_struct* const client_struct) // ok
{
    msg->msg_type = CLIENT_INIT;
    strcpy(msg->sender_login, client_struct->login);
    strcpy(msg->sender_ip, client_struct->ip_str);
    msg->sender_port = client_struct->port;
}

int disconnect_from_server(const Client_struct* const client_struct, const Server_struct* const server_struct) // ok
{
    Message msg;

    set_del_msg(&msg, client_struct);

    int sendto_ret = sendto(client_struct->socket_fd, (const void*)&msg, sizeof(msg), 0, (const sockaddr*)(&server_struct->sock_struct), sizeof(server_struct->sock_struct));

    if (sendto_ret == -1) 
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_SEND_DEL_MSG) cannot send the \ndel message to the server, try again\n");
        printf("==================ERROR MESSAGE==================\n");
        return ERR_SEND_DEL_MSG;
    }

    return RETURN_OK;
}

void set_del_msg(Message* const msg, const Client_struct* const client_struct) // ok
{
    msg->msg_type = CLIENT_DEL;
    strcpy(msg->sender_login, client_struct->login);
    strcpy(msg->sender_ip, client_struct->ip_str);
    msg->sender_port = client_struct->port;
}

void web_inner_logic(const Client_struct* const client_struct, const Server_struct* const server_struct)
{
    int cur_client_pid = getpid();
    print_web_inner_help();
    int fork_val = fork();

    if (fork_val == -1)
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: cannot send requests to the server.\nTry to connect to the server again\n");
        printf("==================ERROR MESSAGE==================\n");
    }
    else
    {
        int new_pid = getpid();
        int new_ppid = getppid();

        bool client_online = true;

        while (client_online)
        {
            if (new_ppid == cur_client_pid)
            {
                listen(client_struct);
            }
            else if (new_pid == cur_client_pid)
            {
                char receiver_login[MAX_LOGIN_LENGTH];
                scanf("%s", receiver_login);
                receiver_login[MAX_LOGIN_LENGTH - 1] = '\0';

                if (!strcmp(receiver_login, "help"))
                {
                    print_web_inner_help();
                    continue;
                }
                if (!strcmp(receiver_login, "list"))
                {
                    get_client_list(client_struct, server_struct);
                    continue;                           
                }
                if (!strcmp(receiver_login, "stop"))
                {
                    client_online = false; 
                    kill(fork_val, SIGTERM); // kills the listening process
                    break;
                }
            
                char msg_text[MAX_MESSAGE_LENGTH];
                scanf("%[^\n]s", msg_text);
                msg_text[MAX_MESSAGE_LENGTH] = '\0';
                send_msg_to_user(client_struct, server_struct, receiver_login, msg_text);

                strcpy(msg_text, "");
                strcpy(receiver_login, "");
            }
        }
    }
}

int get_client_list(const Client_struct* const client_struct, const Server_struct* const server_struct)
{
    Message msg;

    msg.msg_type = CLIENT_LIST;
    strcpy(msg.sender_login, client_struct->login);
    strcpy(msg.sender_ip, client_struct->ip_str);
    msg.sender_port = client_struct->port;

    int sendto_ret = sendto(client_struct->socket_fd, (const void*)&msg, sizeof(msg), 0, (const sockaddr*)(&server_struct->sock_struct), sizeof(server_struct->sock_struct));

    if (sendto_ret == -1) 
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_SEND_LIST_REQ) cannot send \nthe client list message to the server, try again\n");
        printf("==================ERROR MESSAGE==================\n");
        return ERR_SEND_LIST_REQ;
    }
}

void print_web_inner_help() // ok
{
    printf("\n======================HELP======================\n");
    printf("\n======================NOTE======================\n");
    printf("Message structure is: User_login msg_text\n");
    printf("Example: Andrey(User_login) hello!(msg_text)\n");   
    printf("======================NOTE======================\n\n");
    printf("help - to print help again\n");
    printf("list - to get the list of clients on the server\n");
    printf("stop - to stop chatting\n");
    printf("======================HELP======================\n\n");
}

int send_msg_to_user(const Client_struct* const client_struct, const Server_struct* const server_struct, const char* const receiver_login, const char* const msg_text)
{
    Message msg;

    strcpy(msg.msg_text, msg_text);
    msg.msg_type = MSG_ROUTE;
    strcpy(msg.receiver_login, receiver_login);
    strcpy(msg.sender_login, client_struct->login);
    strcpy(msg.sender_ip, client_struct->ip_str);
    msg.sender_port = client_struct->port;

    int sendto_ret = sendto(client_struct->socket_fd, (const void*)&msg, sizeof(msg), 0, (const sockaddr*)(&server_struct->sock_struct), sizeof(server_struct->sock_struct));

    if (sendto_ret == -1) 
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_SEND_MSG) unable to\nsent the message to the server. Try again\n");
        printf("==================ERROR MESSAGE==================\n");
        return ERR_SEND_MSG;
    }
}

void debug_print_client_data(const Client_struct* const client_struct) // ok
{
    printf("\n=============CLIENT DEBUG DATA=============\n");
    printf("login: %s\n", client_struct->login);
    printf("ip_str: %s\n", client_struct->ip_str);
    printf("port\n", client_struct->port);
    printf("socket_fd: %d\n", client_struct->socket_fd);
    printf("\n", client_struct->ip_addr_client.s_addr);

    printf("\n=======sock_struct=======\n");
    printf("sock_struct.sin_addr.s_addr: %d\n", client_struct->sock_struct.sin_addr.s_addr);
    printf("sock_struct.sin_family: %d\n", client_struct->sock_struct.sin_family);
    printf("sock_struct.sin_port: %d\n", client_struct->sock_struct.sin_port);
    printf("=======sock_struct=======\n");
    printf("=============CLIENT DEBUG DATA=============\n");
}

void debug_print_server_data(const Server_struct* const server_struct) // ok
{
    printf("\n=============SERVER DEBUG DATA=============\n");
    printf("ip_str: %s\n", server_struct->ip_str);
    printf("port: %d\n", server_struct->port);
    printf("cur_user_number: %d\n", server_struct->cur_user_number);
    printf("max_user_number: %d\n", server_struct->max_user_number);
    printf("client_arr: %p\n", server_struct->client_arr);
    printf("socket_fd: %d\n", server_struct->socket_fd);

    printf("\n=======sock_struct=======\n");
    printf("sock_struct.sin_addr: %s\n", inet_ntoa(server_struct->sock_struct.sin_addr));
    printf("sock_struct.sin_family: %d\n", server_struct->sock_struct.sin_family);
    printf("sock_struct.sin_port: %d\n", server_struct->sock_struct.sin_port);
    printf("=======sock_struct=======\n");
    printf("=============SERVER DEBUG DATA=============\n");
}

void change_login(Client_struct* const client_struct) // ok
{
    printf("\n==============NOTIFICATION==============\n");       
    printf("The login is used by someone else on the \nserver.");
    printf("You need to chose another login \nin order to connect to the server\n");
    printf("==============NOTIFICATION==============\n");

    bool new_name_flag = false;
    while (!new_name_flag)
    {
        char new_name[MAX_LOGIN_LENGTH];
        printf("\nEnter your new login here (up to %d chars): ", MAX_LOGIN_LENGTH - 1);
        scanf("%s", new_name);

        if (!strcmp(new_name, client_struct->login))
        {
            printf("\n==============NOTIFICATION==============\n");
            printf("Ops, this name is the same one, you\nneed to chose another one\n");
            printf("==============NOTIFICATION==============\n");
            continue;
        }
        else
        {
            printf("\n===================NOTIFICATION===================\n");
            printf("Your login will be updated from %s to %s\n", client_struct->login, new_name);
            printf("===================NOTIFICATION===================\n");
            strcpy(client_struct->login, new_name);
            new_name_flag = true;
        }
    }
}

void listen(const Client_struct* const client_struct) // ok
{
    Message msg;

    socklen_t sock_len = 0;
    struct sockaddr temp_struct;

    int recv_ret = recvfrom(client_struct->socket_fd, (void *)&msg, sizeof(msg), 0, &temp_struct, &sock_len); // does not block
    if (recv_ret != -1)
    {
        if (msg.msg_type == MSG_ROUTE) 
        {
            printf("From %s:%s\n", msg.sender_login, msg.msg_text);
        }
        else if (msg.msg_type == NO_SUCH_CLIENT)
        {   
            printf("\n==================ERROR MESSAGE==================\n");
            printf("No such user with entered login,\ncheck login again or ask for user list\n");
            printf("==================ERROR MESSAGE==================\n");
        }
        else if (msg.msg_type == CLIENT_LIST)
        {
            printf("\n==============CLIENT LIST==============\n");
            printf("Available users: %s\n", msg.msg_text);
            printf("==============CLIENT LIST==============\n");
        }
    }
    else 
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: cannot get the message from the server\n");
        printf("==================ERROR MESSAGE==================\n"); 
    }
}
