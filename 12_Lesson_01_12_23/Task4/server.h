#ifndef TASK4_SERVER_H
#define TASK4_SERVER_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h>

#include "config.h"

enum err_codes
{
    RETURN_OK,
    ERR_CALLOC_CLIENT_ARR,
    ERR_SEND_MSG,
    ERR_GET_SOCKET,
    ERR_BIND_SOCKET,
    ERR_SEND_INIT_PROOF,
    ERR_SEND_CHANGE_LOGIN,
    ERR_REALLOC_CLIENT_ARR,
    ERR_SEND_INIT_RETRY,
    ERR_SEND_CLIENT_LIST,
    ERR_SEND_ROUTING_MSG
};

enum ret_codes
{
    LOGIN_FREE,
    LOGIN_USED,
};

int ctor_server(Server_struct* const server_struct);
void dtor_server(Server_struct* const server_struct);
void print_server_data(const Server_struct* const server_struct);
void change_port(Server_struct* const server_struct);
void auto_get_ip(char* const ip_str);
void print_clients(const Server_struct* const server_struct);
int get_socket();
int set_sock_struct(const char* const ip_str, const int port, in_addr* const ip_addr_str, struct sockaddr_in* const sock_str);
int bind_socket(const int socket_fd, const sockaddr* const sock_str, const size_t size_of_sock_str);
int create_socket(Server_struct* const server_struct);
void print_maintain_help();
void print_client_list(const Server_struct* const server_struct);
int server_maintaining(int fork_val, const Server_struct* const server_struct);
int server_routing(const Server_struct* const server);
void debug_print_server_data(const Server_struct* const server_struct);
void debug_print_client_data(const Client_struct* const client_struct);
void set_server_sock_struct(Server_struct* const server_struct);
void logic(const Server_struct* const server_struct);
int check_client(const Server_struct* const server_struct, const Message* const msg);
int add_the_client(Server_struct* const server_struct, const Message* const msg);
int realloc_clients_arr_up(Server_struct* const server_struct);
void set_confirm_structs(Client_struct* const client, Message* msg, size_t msg_type, const Message* const msg_main_data);
int send_change_login(const Server_struct* const server_struct, const Message* const msg);
int send_init_confirm(const Server_struct* const server_struct, const Message* const msg);
void delete_client(Server_struct* const server_struct, const Message* const msg);
int realloc_clients_arr_down(Server_struct* const server_struct, size_t new_max_num);
int send_init_retry(const Server_struct* const server_struct, const Message* const msg);
void print_new_client_data(const Client_struct* const new_client);
// int send_client_list(const Server_struct* const server_struct, const Message* const msg);
int send_routing_msg(const Server_struct* const server_struct, const Message* const msg);
void print_msg_data(const Message* const msg);

#endif
