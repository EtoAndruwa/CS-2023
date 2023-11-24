#ifndef TASK1_LIBS_H
#define TASK1_LIBS_H    

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

// #define DEBUG 

int sendall(int socket_fd, char* buf_ptr, int buf_len, int flags)
{
    int total = 0;
    ssize_t n = 0;

    while (total < buf_len)
    {
        n = send(socket_fd, buf_ptr, buf_len, 0);
        if (n == -1)
        {
            break;
        }
        total += n;
    }

    return (n == -1 ? -1: total);
}

#endif
