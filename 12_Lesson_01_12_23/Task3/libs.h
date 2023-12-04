#ifndef TASK3_LIBS_H
#define TASK3_LIBS_H    

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>


// #define DEBUG 
#define MAX_MSG_LEN 201
#define PRE_MSG_LEN 50

enum ret_codes
{
    BREAK_PING,
    NO_BREAK_PING
};

int get_socket();
int logic_outer(const int port_sd, const char* const ip_str, const int socket_fd, struct sockaddr_in* const sock_struct_sd, struct sockaddr_in* const sock_struct_rc, const size_t size_struct_sd, const size_t size_struct_rc);
int logic_inner(const int port_sd, const char* const ip_str, const int socket_fd, struct sockaddr_in* const sock_struct_sd, struct sockaddr_in* const sock_struct_rc, const size_t size_struct_sd, const size_t size_struct_rc);
int bind_socket(const int socket_fd, const sockaddr* const sock_str, const size_t size_of_sock_str);
void write_msg(char* const msg_ptr);
void print_help_outer();
void print_help_chat();
int set_sock_struct(const char* const ip_str, const int port, in_addr* const ip_addr_str, struct sockaddr_in* const sock_str);
void get_ip_str(char* const ip_str);
void get_port(int* const port_ptr);
void print_help_chat_inner();
void set_my_socket(struct sockaddr_in* const sock_struct_sd, char* const ip_str, int* const port, in_addr* const ip_addr_sd);
void get_port_my(int* const port_ptr);
void get_ip_str_my(char* const ip_str);

#endif