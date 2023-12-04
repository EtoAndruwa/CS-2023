#ifndef TASK4_CLIENT_H
#define TASK4_CLIENT_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>

#include "config.h"

enum ret_codes
{
    RETURN_OK,
    ERR_SEND_INIT_MSG,
    ERR_SEND_DEL_MSG,
    ERR_SEND_MSG,
    ERR_GET_SOCKET,
    ERR_BIND_SOCKET,
    ERR_GET_INIT_RESP,
};

void auto_get_ip(char* const ip_str);
void print_greeting();
void ctor_client(Client_struct* const client_struct);
void dtor_client(Client_struct* const client_struct);
void print_client_data(const Client_struct* const client_struct);
void change_port(Client_struct* const client_struct);
void print_web_help();
int web_logic(Client_struct* const client_struct, Server_struct* const server_struct);
void ctor_default_server(Server_struct* const server_struct);
void dtor_default_server(Server_struct* const server_struct);
void print_server_data(const Server_struct* const server_struct);
void change_server_port(Server_struct* const server_struct);
void change_server_ip(Server_struct* const server_struct);
void print_msg_data(const Message* const msg);
int connect_to_server(Client_struct* const client_struct, Server_struct* const server_struct);
void set_sock_struct(const char* const ip_str, const int port, in_addr* const ip_addr_str, struct sockaddr_in* const sock_str);
void set_init_msg(Message* const msg, const Client_struct* const client_struct);
void set_del_msg(Message* const msg, const Client_struct* const client_struct);
int disconnect_from_server(const Client_struct* const client_struct, const Server_struct* const server_struct);
int create_socket(Client_struct* const client_struct);
void set_server_sock_struct(Server_struct* const server_struct);
void change_login(Client_struct* const client_struct);
void web_inner_logic(const Client_struct* const client_struct, const Server_struct* const server_struct);
void print_web_inner_help();
void listen(const Client_struct* const client_struct);
int send_msg_to_user(const Client_struct* const client_struct, const Server_struct* const server_struct, const char* const receiver_login, const char* const msg_text);

#endif
