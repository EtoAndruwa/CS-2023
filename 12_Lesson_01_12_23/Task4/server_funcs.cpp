#include "server.h"

int ctor_server(Server_struct* const server_struct) // ok
{
    server_struct->max_user_number = MIN_NUM_OF_CLIENTS_IN_SERV;
    server_struct->cur_user_number = 0;

    #ifdef LOOP_BACK
        stpcpy(server_struct->ip_str, "INADDR_LOOPBACK");
    #else
        auto_get_ip(server_struct->ip_str);
    #endif

    printf("\n==================INFO==================\n");
    printf("Server IP: %s\n", server_struct->ip_str);
    server_struct->port = DEFAULT_SERVER_PORT;
    printf("Server default PORT: %d\n", server_struct->port);
    printf("==================INFO==================\n");

    char c[2];
    printf("\nWould you like to change the server's default port value? (y/n): ");
    scanf("%s", c);

    switch (c[0])
    {
    case 'y':
    {
        change_port(server_struct);
        break;
    }
    case 'n':
    {
        break;
    }
    default:
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: invalid command, port will\nremain default value\n");
        printf("==================ERROR MESSAGE==================\n"); 
        break;
    }

    server_struct->client_arr = (Client_struct* )calloc(server_struct->max_user_number, sizeof(Client_struct));

    if (server_struct->client_arr == nullptr)
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_CALLOC_CLIENT_ARR) cannot calloc\nthe memory for client_arr. Server will shut down...\n");
        printf("==================ERROR MESSAGE==================\n"); 
        return ERR_CALLOC_CLIENT_ARR;
    }

    for (size_t i = 0; i < server_struct->max_user_number; i++)
    {
        server_struct->client_arr[i].port = 0;
        strcpy(server_struct->client_arr[i].ip_str, "DEFAULT_IP");
        strcpy(server_struct->client_arr[i].login, "DEFAULT_LOGIN");
    }

    return RETURN_OK;
}

void dtor_server(Server_struct* const server_struct) // ok
{
    server_struct->cur_user_number = 0;
    server_struct->max_user_number = 0;
    server_struct->port = 0;
    free(server_struct->client_arr);
    server_struct->client_arr = nullptr;
    strcpy(server_struct->ip_str, "");
}

void change_port(Server_struct* const server_struct) // ok
{
    int port = 0;
    printf("\nEnter new server PORT: ");
    scanf("%d", &port);

    if (port >= 0)
    {
        printf("\n==============NOTIFICATION==============\n");
        printf("Port was changed\nfrom %d to %d\n", server_struct->port, port);
        printf("==============NOTIFICATION==============\n");
        server_struct->port = port;
    }
    else
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: invalid port value.\nServer port will remain %d\n", server_struct->port);
        printf("==================ERROR MESSAGE==================\n"); 
    }
}

void print_server_data(const Server_struct* const server_struct) // ok
{
    printf("\n==================SERVER DATA==================\n");
    printf("Server IP: %s\n", server_struct->ip_str);
    printf("Server PORT: %d\n", server_struct->port);
    printf("Current user number: %d\n", server_struct->cur_user_number);
    printf("Max user number: %d\n", server_struct->max_user_number);
    printf("Address of array with clients' data: %p\n", server_struct->client_arr);
    printf("==================SERVER DATA==================\n");
}

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
    sprintf(ip_str, "%s", inet_ntoa(((struct sockaddr_in* )&ifr.ifr_addr)->sin_addr)); // converts back to the string
}

void print_clients(const Server_struct* const server_struct) // ok
{
    for (size_t i = 0; i < server_struct->max_user_number; i++)
    {
        printf("\n===========CLIENT's #%d DATA===========\n", i);
        printf("Client's LOGIN: %s\n", server_struct->client_arr[i].login);
        printf("Client's IP: %s\n", server_struct->client_arr[i].ip_str);
        printf("Client's PORT: %d\n", server_struct->client_arr[i].port);
        printf("===========CLIENT's #%d DATA===========\n", i);
    }
}

void logic(const Server_struct* const server_struct)
{
    int cur_serv_pid = getpid();
    print_maintain_help();
    int fork_val = fork();

    if (fork_val == -1)
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: cannot start the server. Server will shut down\n");
        printf("==================ERROR MESSAGE==================\n"); 
    }
    else
    {
        int new_pid = getpid();
        int new_ppid = getppid();

        bool server_online = true;

        while (server_online)
        {
            if (new_ppid == cur_serv_pid)
            {
                server_routing(server_struct);
            }
            else if (new_pid == cur_serv_pid)
            {
                if (server_maintaining(fork_val, server_struct) == 1)
                {
                    server_online = false;
                    break;
                }
            }
        }
    }
}

int create_socket(Server_struct* const server_struct) // ok
{
    server_struct->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (server_struct->socket_fd == -1)
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_GET_SOCKET) get_socket failure\n");
        printf("==================ERROR MESSAGE==================\n"); 
        return ERR_GET_SOCKET;
    }
    else
    {
        set_sock_struct(server_struct->ip_str, server_struct->port, &server_struct->ip_addr_server, &server_struct->sock_struct);

        int bind_rt = bind(server_struct->socket_fd, (const sockaddr* )&server_struct->sock_struct, sizeof(server_struct->sock_struct));
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

int set_sock_struct(const char* const ip_str, const int port, in_addr* const ip_addr_str, struct sockaddr_in* const sock_str) // ok
{
    #ifdef LOOP_BACK
        sock_str->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    #else
        inet_aton(ip_str, ip_addr_str);
        sock_str->sin_addr.s_addr = ip_addr_str->s_addr;
    #endif
    sock_str->sin_port   = htons(port);
    sock_str->sin_family = AF_INET;
}

int bind_socket(const int socket_fd, const sockaddr* const sock_str, const size_t size_of_sock_str) // ok
{
    return bind(socket_fd, sock_str, size_of_sock_str);
}

int server_maintaining(int fork_val, const Server_struct* const server_struct) // ok
{
    char cmd[30];

    bool server_status = true;
    while (server_status)
    {
        printf("\nEnter the command here: ");
        scanf("%s", cmd);

        if (!strcmp(cmd, "stop_server"))
        {
            server_status = false;
            kill(fork_val, SIGTERM); // kills the listening process
            return 1;
        }
        if (!strcmp(cmd, "help"))
        {
            print_maintain_help();
            continue;
        }
    }
}

int server_routing(const Server_struct* const server)
{
    while (true)
    {
        Message msg;

        socklen_t sock_len = 0;
        int recv_ret = recvfrom(server->socket_fd, (void *)&msg, sizeof(msg), 0, (sockaddr*)(&server->sock_struct), &sock_len); // does not block
        if (recv_ret != -1)
        {
            switch (msg.msg_type)
            {
            case CLIENT_INIT:
            {
                if (server->cur_user_number == server->max_user_number)
                {
                    if (realloc_clients_arr_up((Server_struct* const)server) == RETURN_OK)
                    {
                        if (check_client(server, &msg) == LOGIN_FREE)
                        {
                            add_the_client((Server_struct* const)server, &msg);
                            debug_print_server_data(server);
                            send_init_confirm(server, &msg);
                        }
                        else
                        {
                            send_change_login((Server_struct* const)server, &msg);
                        }
                    }
                    else 
                    {
                        send_init_retry(server, &msg);
                    }
                }
                else
                {
                    if (check_client(server, &msg) == LOGIN_FREE)
                    {
                        add_the_client((Server_struct* const)server, &msg);
                        debug_print_server_data(server);
                        send_init_confirm(server, &msg);
                    }
                    else
                    {
                        send_change_login((Server_struct* const)server, &msg);
                    }
                }

                print_client_list(server);
                break;
            }
            case CLIENT_DEL:
            {
                delete_client((Server_struct* const)server, &msg);
                debug_print_server_data(server);
                break;
            }
            case MSG_ROUTE:
            {
                send_routing_msg(server, &msg);
                break;
            }
            case CLIENT_LIST:
            {
                send_client_list(server, &msg);
                break;
            }
            default:
                break;
            }
        }
    }
}

void print_maintain_help() // ok
{
    printf("\n============MAINTAIN HELP============\n");
    printf("stop_server - to stop the server\n");
    printf("help - to print help\n");
    printf("============MAINTAIN HELP============\n");
}

void print_client_list(const Server_struct* const server_struct) // ok
{
    for (size_t i = 0; i < server_struct->cur_user_number; i++)
    {
        printf("\n===============CLIENT %d DATA===============\n", i);
        printf("Login: %s\n", server_struct->client_arr[i].login);
        printf("IP: %s\n", server_struct->client_arr[i].ip_str);
        printf("PORT: %d\n", server_struct->client_arr[i].port);
        printf("===============CLIENT %d DATA===============\n", i);
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
    printf("sock_struct.sin_port: %d\n", client_struct->port);
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
    printf("sock_struct.sin_port: %d\n", server_struct->port);
    printf("=======sock_struct=======\n");
    printf("=============SERVER DEBUG DATA=============\n");
}

int check_client(const Server_struct* const server_struct, const Message* const msg) // ok
{
    for (size_t i = 0; i < server_struct->cur_user_number; i++)
    {
        if (!strcmp(server_struct->client_arr[i].login, msg->sender_login))
        {
            return LOGIN_USED;
        }
        else
        {
            return LOGIN_FREE;
        }
    }
}

int realloc_clients_arr_up(Server_struct* const server_struct) // ok
{
    int new_max_usr_num = server_struct->max_user_number * 2;
    Client_struct* new_client_arr = (Client_struct*)realloc(server_struct->client_arr, new_max_usr_num * sizeof(Client_struct));

    if (new_client_arr != nullptr)
    {
        server_struct->max_user_number = new_max_usr_num;
        server_struct->client_arr = new_client_arr;
        return RETURN_OK;
    }
    else 
    {
        return ERR_REALLOC_CLIENT_ARR;
    }
}

int realloc_clients_arr_down(Server_struct* const server_struct, size_t new_max_num) // ok
{   
    Client_struct* new_client_arr = (Client_struct*)realloc(server_struct->client_arr, new_max_num * sizeof(Client_struct));

    if (new_client_arr != nullptr)
    {
        server_struct->max_user_number = new_max_num;
        server_struct->client_arr = new_client_arr;
        return RETURN_OK;
    }
    else 
    {
        return ERR_REALLOC_CLIENT_ARR;
    }
}

int add_the_client(Server_struct* const server_struct, const Message* const msg) // ok
{
    Client_struct* new_client = &server_struct->client_arr[server_struct->cur_user_number];

    new_client->port = msg->sender_port;
    strcpy(new_client->login, msg->sender_login);
    strcpy(new_client->ip_str, msg->sender_ip);

    server_struct->cur_user_number++;

    print_new_client_data(new_client);
}

void delete_client(Server_struct* const server_struct, const Message* const msg) // ok
{
    Client_struct* client = &server_struct->client_arr[0];
    
    for (size_t i = 0; i < server_struct->cur_user_number; i++)
    {
        if (!strcmp(client->login, msg->sender_login))
        {
            strcpy(client->login, "");
            strcpy(client->ip_str, "");
            client->port = 0;
            client->socket_fd = 0;
            client->sock_struct.sin_port = 0;
            client->sock_struct.sin_addr.s_addr = 0;
            client->sock_struct.sin_family = 0;
            client->ip_addr_client.s_addr  = 0;
            break;
        }
    }

    server_struct->cur_user_number--;

    if (server_struct->max_user_number != MIN_NUM_OF_CLIENTS_IN_SERV)
    {
        if (server_struct->max_user_number / 2 >= MIN_NUM_OF_CLIENTS_IN_SERV && server_struct->cur_user_number <= server_struct->max_user_number / 2)
        {
            realloc_clients_arr_down(server_struct, server_struct->max_user_number / 2);
        }
    }
}

int send_init_confirm(const Server_struct* const server_struct, const Message* const msg) // ok
{
    Client_struct client;
    Message new_msg;

    set_confirm_structs(&client, &new_msg, INIT_SUCCESS, msg);

    int sendto_ret = sendto(server_struct->socket_fd, (const void*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client.sock_struct), sizeof(client.sock_struct));

    if (sendto_ret == -1) 
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_SEND_INIT_PROOF) cannot send\nthe init message to the server, try again\n");
        printf("==================ERROR MESSAGE==================\n"); 
        return ERR_SEND_INIT_PROOF;
    }

    return RETURN_OK;
}

int send_change_login(const Server_struct* const server_struct, const Message* const msg) // ok
{
    Client_struct client;
    Message new_msg;

    set_confirm_structs(&client, &new_msg, CHANGE_LOGIN, msg);

    int sendto_ret = sendto(server_struct->socket_fd, (const void*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client.sock_struct), sizeof(client.sock_struct));

    if (sendto_ret == -1) 
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_SEND_CHANGE_LOGIN) cannot send\nthe change login message to the server, try again\n");
        printf("==================ERROR MESSAGE==================\n"); 
        return ERR_SEND_CHANGE_LOGIN;
    }

    return RETURN_OK;
}


int send_init_retry(const Server_struct* const server_struct, const Message* const msg) // ok
{
    Client_struct client;
    Message new_msg;

    set_confirm_structs(&client, &new_msg, INIT_RETRY, msg);

    int sendto_ret = sendto(server_struct->socket_fd, (const void*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client.sock_struct), sizeof(client.sock_struct));

    if (sendto_ret == -1) 
    {
        printf("\n==================ERROR MESSAGE==================\n");
        printf("ERROR: (ERR_SEND_INIT_RETRY) cannot send\nthe init retry message to the server, try again\n");
        printf("==================ERROR MESSAGE==================\n"); 
        return ERR_SEND_INIT_RETRY;
    }

    return RETURN_OK;
}

void print_new_client_data(const Client_struct* const new_client) // ok
{
    printf("\n\n===========NEW CLIENT===========\n");
    printf("LOGIN: %s\n", new_client->login);
    printf("IP: %s\n", new_client->ip_str);
    printf("PORT: %d\n", new_client->port);
    printf("===========NEW CLIENT===========\n");
}

int send_routing_msg(const Server_struct* const server_struct, const Message* const msg)
{
    Message new_msg;
    Client_struct client;
    Client_struct* temp = nullptr;

    new_msg.msg_type = MSG_ROUTE;
    strcpy(new_msg.msg_text, msg->msg_text);
    strcpy(new_msg.sender_login, msg->sender_login);

    bool user_found = false;

    for (size_t i = 0; i < server_struct->cur_user_number; i++)
    {
        if (!strcmp(msg->receiver_login, server_struct->client_arr[i].login))
        {
            temp = &server_struct->client_arr[i];
            user_found = true;
            break;
        }
    }

    if (user_found)
    {
        // to the receiver
        set_sock_struct(temp->ip_str, temp->port, &client.ip_addr_client, &client.sock_struct);
        int sendto_ret = sendto(server_struct->socket_fd, (const void*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client.sock_struct), sizeof(client.sock_struct));

        if (sendto_ret == -1) 
        {       
            printf("\n==================ERROR MESSAGE==================\n");
            printf("ERROR: (ERR_SEND_ROUTING_MSG) cannot\nsend the routing message\n");
            printf("==================ERROR MESSAGE==================\n"); 
            return ERR_SEND_ROUTING_MSG;
        }

        // to the sender
        new_msg.msg_type = MSG_SENT;
        set_sock_struct(msg->sender_ip, msg->sender_port, &client.ip_addr_client, &client.sock_struct);
        sendto_ret = sendto(server_struct->socket_fd, (const void*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client.sock_struct), sizeof(client.sock_struct));

        if (sendto_ret == -1) 
        {       
            printf("\n==================ERROR MESSAGE==================\n");
            printf("ERROR: (ERR_SEND_ROUTING_MSG) cannot\nsend the routing message\n");
            printf("==================ERROR MESSAGE==================\n"); 
            return ERR_SEND_ROUTING_MSG;
        }
    }
    else
    {
        new_msg.msg_type = NO_SUCH_CLIENT;
        set_sock_struct(msg->sender_ip, msg->sender_port, &client.ip_addr_client, &client.sock_struct);
        int sendto_ret = sendto(server_struct->socket_fd, (const void*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client.sock_struct), sizeof(client.sock_struct));

        if (sendto_ret == -1) 
        {       
            printf("\n==================ERROR MESSAGE==================\n");
            printf("ERROR: (ERR_SEND_ROUTING_MSG) cannot\nsend the routing message\n");
            printf("==================ERROR MESSAGE==================\n"); 
            return ERR_SEND_ROUTING_MSG;
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

void set_confirm_structs(Client_struct* const client, Message* msg, size_t msg_type, const Message* const msg_main_data) // ok
{
    msg->msg_type = msg_type;
    set_sock_struct(msg_main_data->sender_ip, msg_main_data->sender_port, &client->ip_addr_client, &client->sock_struct);
}

int send_client_list(const Server_struct* const server_struct, const Message* const msg)
{
    size_t sent_login_num = 0;
    Client_struct client;
    Message new_msg;
    set_confirm_structs(&client, &new_msg, CLIENT_LIST, msg);

    while (sent_login_num != server_struct->cur_user_number)
    {
        sprintf(new_msg.msg_text, "%s, ", server_struct->client_arr[sent_login_num].login);
        new_msg.msg_text[strlen(server_struct->client_arr[sent_login_num].login) + 2] = '\0';
        sent_login_num++;
        printf("sent_login_num %d\n", sent_login_num);
        printf("msg_text %s\n", new_msg.msg_text);
        printf("size of msg %d\n", strlen(msg->msg_text));

        for (sent_login_num; sent_login_num < server_struct->cur_user_number; sent_login_num++)
        {
            printf("sizeof(new_msg.msg_text) %d\n", sizeof(new_msg.msg_text));
            printf("sizeof(login) %d\n", sizeof(server_struct->client_arr[sent_login_num].login));
            if (strlen(new_msg.msg_text) + strlen(server_struct->client_arr[sent_login_num].login + 2) <= MAX_MESSAGE_LENGTH)
            {
                strcat(new_msg.msg_text, server_struct->client_arr[sent_login_num].login);
                strcat(new_msg.msg_text, ", ");
            }
            else
            {
                break;
            }
        }

        printf("Ready msg_text: %s\n", new_msg.msg_text);

        int sendto_ret = sendto(server_struct->socket_fd, (const void*)&new_msg, sizeof(new_msg), 0, (const sockaddr*)(&client.sock_struct), sizeof(client.sock_struct));

        if (sendto_ret == -1) 
        {
            printf("ERROR: (ERR_SEND_CLIENT_LIST) cannot send the init message to the server, try again\n");
            return ERR_SEND_CLIENT_LIST;
        }
    }

    return RETURN_OK;
}
