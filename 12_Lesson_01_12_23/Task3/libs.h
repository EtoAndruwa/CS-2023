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
int logic_outer(int port_sd, char* ip_str, int socket_fd, struct sockaddr_in* sock_struct_sd, struct sockaddr_in* sock_struct_rc, size_t size_struct_sd, size_t size_struct_rc);
int logic_inner(int port_sd, char* ip_str, int socket_fd, struct sockaddr_in* sock_struct_sd, struct sockaddr_in* sock_struct_rc, size_t size_struct_sd, size_t size_struct_rc);
int bind_socket(int socket_fd, const sockaddr* sock_str, size_t size_of_sock_str);
void write_msg(char* msg_ptr);
void print_help_outer();
void print_help_chat();
int set_sock_struct(const char* ip_str, int port, in_addr* ip_addr_str, struct sockaddr_in* sock_str);
void get_ip_str(char* ip_str);
void get_port(int* port_ptr);
void print_help_chat_inner();
void set_my_socket(struct sockaddr_in* sock_struct_sd, char* ip_str, int* port, in_addr* ip_addr_sd);
void get_port_my(int* port_ptr);
void get_ip_str_my(char* ip_str);

#endif