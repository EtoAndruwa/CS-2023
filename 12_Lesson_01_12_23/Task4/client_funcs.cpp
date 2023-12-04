#include "client.h"

void auto_get_ip(char* const ip_str) // ok
{
    int interf_fd;
    struct ifreq ifr;
    char interf_str[] = "wlo1"; // wifi interface
 
    interf_fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;

    strncpy(ifr.ifr_name, interf_str, IFNAMSIZ - 1);
    ioctl(interf_fd, SIOCGIFADDR, &ifr); // get the IP address of the device
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
    printf("Your IP: %s\n", client_struct->ip_str);
    client_struct->port = DEFAULT_CLIENT_PORT;
    printf("Your default PORT: %d\n", client_struct->port);

    char c[2];
    printf("Would you like to change the default port value? (y/n): ");
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
        printf("ERROR: invalid command, port will remain default. You can change your port later anyway\n");
        break;
    }

    printf("Enter your login here (up to %d chars): ", MAX_LOGIN_LENGTH);
    scanf("%s", client_struct->login);
    client_struct->login[MAX_LOGIN_LENGTH - 1] = '\0';
    printf("Your login: %s\n", client_struct->login);
}

void change_port(Client_struct* const client_struct) // ok
{
    int port = 0;
    printf("Enter new PORT: ");
    scanf("%d", &port);

    if (port >= 0)
    {   
        printf("Port changed from %d to %d\n", client_struct->port, port);
        client_struct->port = port;
    }
    else
    {
        printf("ERROR: invalid port value. Your port will remain %d\n", client_struct->port);
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
    printf("\n==========CLIENT DATA==========\n");
    printf("LOGIN: %s\n", client_struct->login);
    printf("IP: %s\n", client_struct->ip_str);
    printf("PORT: %d\n", client_struct->port);
    printf("==========CLIENT DATA==========\n");
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
                    printf("ERROR: invalid command, try again\n");
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
                    printf("ERROR: invalid command, try again\n");
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

                web_inner_logic();

                disconnect_from_server(client_struct, server_struct);
                print_web_help();
                close(client_struct->socket_fd);
                break;
            }
        default:
            printf("ERROR: invalid command, try again\n");
            break;
        }
    }
}

void print_web_help()
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
    printf("\n==========SERVER DATA==========\n");
    printf("IP: %s\n", server_struct->ip_str);
    printf("PORT: %d\n", server_struct->port);
    printf("==========SERVER DATA==========\n");
}

void change_server_ip(Server_struct* const server_struct) // ok
{
    printf("Enter new server IP: ");
    char temp_ip[15];
    scanf("%s", temp_ip);
    printf("Server IP was changed from %s to %s\n", server_struct->ip_str, temp_ip);
    temp_ip[14] = '\0';
    strcpy(server_struct->ip_str, temp_ip);
}

void change_server_port(Server_struct* const server_struct) // ok
{
    printf("Enter new server PORT: ");
    int temp_port;
    scanf("%d", &temp_port);

    if (temp_port >= 0)
    {
        printf("Server PORT was changed from %d to %d\n", server_struct->port, temp_port);
        server_struct->port = temp_port;
    }
}

int connect_to_server(Client_struct* const client_struct, Server_struct* const server_struct)
{
    bool init_success_flag = false;
    while (!init_success_flag)  
    {
        Message msg;

        set_init_msg(&msg, client_struct);
        print_msg_data(&msg);
        set_server_sock_struct(server_struct);

        int sendto_ret = sendto(client_struct->socket_fd, (const void*)&msg, sizeof(msg), 0, (const sockaddr*)(&server_struct->sock_struct), sizeof(server_struct->sock_struct));

        if (sendto_ret == -1) 
        {
            printf("ERROR: (ERR_SEND_INIT_MSG) cannot send the init message to the server, try again\n");
            return ERR_SEND_INIT_MSG;
        }

        printf("init msg sent\n");

        socklen_t sock_len = 0;
        struct sockaddr temp_server_struct;

        int recv_ret = recvfrom(client_struct->socket_fd, (void *)&msg, sizeof(msg), 0, &temp_server_struct, &sock_len); // does not block
        printf("recv_ret\n");
        if (recv_ret != -1)
        {
            if (msg.msg_type == CHANGE_LOGIN)
            {   
                printf("CHANGE_LOGIN msg rcv\n");
                change_login(client_struct);
                continue;
            }
            else if (msg.msg_type == INIT_SUCCESS)
            {
                init_success_flag = true;
                printf("You was connected to the server\n");
            }
            else if (msg.msg_type == INIT_RETRY)
            {
                printf("INIT_RETRY msg rcv\n");
                continue;
            }
        }
        else
        {
            printf("ERROR: (ERR_GET_INIT_RESP) unable to connect to the server, try to start messaging one more time\n");
            return ERR_GET_INIT_RESP;
        }
    }

    return RETURN_OK;
}

void print_msg_data(const Message* const msg)
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

int create_socket(Client_struct* const client_struct)
{
    client_struct->socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (client_struct->socket_fd == -1)
    {
        printf("ERROR: get_socket failure\n");
        return ERR_GET_SOCKET;
    }
    else
    {   
        set_sock_struct(client_struct->ip_str, client_struct->port, &client_struct->ip_addr_client, &client_struct->sock_struct);

        int bind_rt = bind(client_struct->socket_fd, (const sockaddr *)&client_struct->sock_struct, sizeof(client_struct->sock_struct));
        if (bind_rt == -1)
        {
            printf("ERROR: bind_socket failure\n");
            return ERR_BIND_SOCKET;
        }

        return RETURN_OK;
    }
}

void set_sock_struct(const char* const ip_str, const int port, in_addr* const ip_addr_str, struct sockaddr_in* const sock_str)
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

void set_server_sock_struct(Server_struct* const server_struct)
{
    server_struct->port = DEFAULT_SERVER_PORT;

    #ifdef LOOP_BACK    
        strcpy(server_struct->ip_str, "INADDR_LOOPBACK");
    #else
        strcpy(server_struct->ip_str, DEFAULT_SERVER_IP);
    #endif
    
    set_sock_struct(server_struct->ip_str, server_struct->port, &server_struct->ip_addr_server, &server_struct->sock_struct);
}

void set_init_msg(Message* const msg, const Client_struct* const client_struct)
{
    msg->msg_type = CLIENT_INIT;
    strcpy(msg->sender_login, client_struct->login);
    strcpy(msg->sender_ip, client_struct->ip_str);
    msg->sender_port = client_struct->port;
}

int disconnect_from_server(const Client_struct* const client_struct, const Server_struct* const server_struct)
{
    Message msg;

    set_del_msg(&msg, client_struct);
    print_msg_data(&msg);

    int sendto_ret = sendto(client_struct->socket_fd, (const void*)&msg, sizeof(msg), 0, (const sockaddr*)(&server_struct->sock_struct), sizeof(server_struct->sock_struct));

    if (sendto_ret == -1) 
    {
        printf("ERROR: (ERR_SEND_DEL_MSG) cannot send the del message to the server, try again\n");
        return ERR_SEND_DEL_MSG;
    }

    return RETURN_OK;
}

void set_del_msg(Message* const msg, const Client_struct* const client_struct)
{
    msg->msg_type = CLIENT_DEL;
    strcpy(msg->sender_login, client_struct->login);
    strcpy(msg->sender_ip, client_struct->ip_str);
    msg->sender_port = client_struct->port;
}

void web_inner_logic()
{
    char c[2];
    printf("web inner\n");
    while (c[0] != 'q')
    {   
        

        switch (c[0])
        {
        case 'q':
            {
                break;
            }    
        case 'h':
            {
                // print help
                break;
            } 
        case 's':
            {
                // send the message to the client 
                break;
            }
        case 'l':
            {
                // ask for user list server
                break;
            }                
        default:
            printf("ERROR: invalid command, try one more time\n");
            break;
        }
    }
}

void print_web_inner_help()
{


}

int send_msg_to_user()
{

}

void debug_print_client_data(const Client_struct* const client_struct)
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

void debug_print_server_data(const Server_struct* const server_struct)
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

void change_login(Client_struct* const client_struct)
{
    printf("The login is used by someone else on the server\n");
    printf("You need to chose another one\n");

    bool new_name_flag = false;
    while (!new_name_flag)
    {
        char new_name[MAX_LOGIN_LENGTH];
        printf("Enter your new login here (up to %d chars): ", MAX_LOGIN_LENGTH);
        scanf("%s", new_name);

        if (!strcmp(new_name, client_struct->login))
        {
            printf("Ops, this name is the same one, you need to chose another one\n");
            continue;
        }
        else
        {
            printf("Your login will be updated\n");
            strcpy(client_struct->login, new_name);
            new_name_flag = true;
        }
    }
}
