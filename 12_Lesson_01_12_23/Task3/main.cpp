#include "libs.h"

int main()
{
    struct sockaddr_in sock_struct_sd;
    struct sockaddr_in sock_struct_rc;

    char ip_str[50];
    int  port;
    in_addr ip_addr_sd;

    int socket_fd = get_socket();

    if (socket_fd == -1)
    {
        printf("ERROR: get_socket failure\n");
    }
    else
    {   
        #ifdef DEBUG
            printf("socket is ok\n");

        #endif


        set_my_socket(&sock_struct_sd, ip_str, &port, &ip_addr_sd);

        int bind_rt = bind_socket(socket_fd, (const sockaddr*)(&sock_struct_sd), sizeof(sock_struct_sd));
        if (bind_rt == -1)
        {
            printf("ERROR: bind_socket failure\n");
        }
        else
        {
            #ifdef DEBUG
                printf("bind is ok\n");
            #endif
            logic_outer(port, ip_str, socket_fd, &sock_struct_sd, &sock_struct_rc, sizeof(sock_struct_sd), sizeof(sock_struct_rc));
        }
    }

    close(socket_fd);

    return 0;
}
