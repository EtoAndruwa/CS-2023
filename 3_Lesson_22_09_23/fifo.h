#ifndef FIFO_H
#define FIFO_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_MSG_LENGTH 100
#define MAX_NAME_LENGTH 21
#define TOTAL_MAX_MSG_LENGTH (MAX_MSG_LENGTH + MAX_NAME_LENGTH + 8)
// #define DEBUG

void greeting();
void print_info();
void farewell();
void logic(char* usr_name_ptr, int* const fifo_flag);
void get_usr_name(char* const usr_name_ptr);
void wrt_msg(const char* const usr_name_ptr);
void get_msg(const char* const usr_name_ptr, int* const fifo_flag);
char* cat_fifo(const char* const usr_name_ptr);
void del_fifo(const char* const usr_name_ptr);
void create_fifo(const char* const usr_name_ptr);

#endif
