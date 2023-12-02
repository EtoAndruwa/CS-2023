#include "libs.h"

int main()
{
    struct sockaddr_in sock_struct_sd;
    struct sockaddr_in sock_struct_rc;

    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if (socket_fd == -1)
    {
        printf("socket failure\n");
    }
    else
    {   
        printf("socket is ok\n");

        sock_struct_sd.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        sock_struct_sd.sin_port        = htons(21000);
        sock_struct_sd.sin_family      = AF_INET;

        int bind_rt = bind(socket_fd, (const sockaddr*)(&sock_struct_sd), sizeof(sock_struct_sd));
        if (bind_rt == -1)
        {
            printf("bind failure\n");
        }
        else
        {
            printf("bind is ok\n");

            char buf1[] = "Hello 1, receiver!";
            char buf2[] = "Hello 2, receiver!";

            sock_struct_rc.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
            sock_struct_rc.sin_port        = htons(22000);
            sock_struct_rc.sin_family      = AF_INET;

            int send_num = sendto(socket_fd, buf1, sizeof(buf1), 0, (const sockaddr*)(&sock_struct_rc), sizeof(sock_struct_rc));
            printf("send_num1 %d\n", send_num);

            send_num = sendto(socket_fd, buf2, sizeof(buf1), 0, (const sockaddr*)(&sock_struct_rc), sizeof(sock_struct_rc));
            printf("send_num2 %d\n", send_num);
        }
    }

    close(socket_fd);

    return 0;
}
