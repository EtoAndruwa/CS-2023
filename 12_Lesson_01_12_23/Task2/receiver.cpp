#include "libs.h"

int main()
{
    struct sockaddr_in sock_struct_rv;
    struct sockaddr_in sock_struct_sd;

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (socket_fd == -1)
    {
        printf("socket failure\n");
    }
    else
    {
        printf("socket is ok\n");

        in_addr addr1;
        inet_aton("192.168.31.36", &addr1);
        sock_struct_rv.sin_addr.s_addr = addr1.s_addr;
        sock_struct_rv.sin_port        = htons(21000);
        sock_struct_rv.sin_family      = AF_INET;

        int bind_rt = bind(socket_fd, (const sockaddr*)(&sock_struct_rv), sizeof(sock_struct_rv));
        if (bind_rt == -1)
        {
            printf("bind failure\n");
        }
        else
        {
            printf("bind is ok\n");
            
            char buf[19];
            socklen_t sock_len = 0;

            printf("recv started\n");
            int recv_ret = recvfrom(socket_fd, buf, 19, 0, (sockaddr*)(&sock_struct_sd), &sock_len);
            printf("recv_ret1 %d %s\n", recv_ret, buf);

            recv_ret = recvfrom(socket_fd, buf, 19, 0, (sockaddr*)(&sock_struct_sd), &sock_len);
            printf("recv_ret2 %d %s\n", recv_ret, buf);
        }
    }

    close(socket_fd);

    return 0;
}
