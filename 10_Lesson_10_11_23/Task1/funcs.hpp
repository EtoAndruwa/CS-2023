#ifndef LESSON_10_11_23_TASK1
#define LESSON_10_11_23_TASK1

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void logic(int argc, char* argv []);
void print_stat(struct stat* buf);
void print_info(dirent* dir_struct_ptr, struct stat* buf);



#endif
