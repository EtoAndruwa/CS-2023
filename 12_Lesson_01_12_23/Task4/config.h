#ifndef TASK4_CONFIG_H
#define TASK4_CONFIG_H

#define MAX_LOGIN_LENGTH 15
#define MAX_PASSWORD_LENGTH 10
#define MAX_MESSAGE_LENGTH 201
#define MAX_IP_LENGTH 15

#define DEFAULT_CLIENT_PORT 21000
#define DEFAULT_SERVER_IP "192.168.31.227"
#define DEFAULT_SERVER_PORT 25000
#define MIN_NUM_OF_CLIENTS_IN_SERV 1

#define LOOP_BACK

enum msg_types
{
    CLIENT_INIT,
    CLIENT_DEL,
    MSG_ROUTE,
    INIT_SUCCESS,
    CHANGE_LOGIN,
    INIT_RETRY,
    CLIENT_LIST,
    NO_SUCH_CLIENT,
    MSG_SENT
};

typedef struct Message
{
    size_t msg_type;
    char msg_text[MAX_MESSAGE_LENGTH];
    char receiver_login[MAX_LOGIN_LENGTH];
    char receiver_ip[MAX_IP_LENGTH];
    size_t receiver_port;
    char sender_login[MAX_LOGIN_LENGTH];
    char sender_ip[MAX_IP_LENGTH];
    size_t sender_port;
}Message;

typedef struct Client_struct
{
    char ip_str[MAX_IP_LENGTH];
    size_t port;
    char login[MAX_LOGIN_LENGTH];
    struct sockaddr_in sock_struct;
    in_addr ip_addr_client;
    int socket_fd;
}Client_struct;

typedef struct Server_struct
{
    int socket_fd;
    char ip_str[MAX_IP_LENGTH];
    size_t port;
    size_t max_user_number;
    size_t cur_user_number;
    Client_struct* client_arr;
    struct sockaddr_in sock_struct;
    in_addr ip_addr_server;
}Server_struct;

#endif
