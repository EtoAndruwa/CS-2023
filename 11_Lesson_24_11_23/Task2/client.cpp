#include "libs.h" 

int main()
{
    struct sockaddr_in sock_struct;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (socket_fd == -1)
    {
        printf("socket failure\n");
    }
    else
    {
        #ifdef DEBUG
            printf("socket success, returned %d\n", socket_fd);
        #endif

        sock_struct.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        sock_struct.sin_family      = AF_INET;
        sock_struct.sin_port        = htons(21000);

        socklen_t sock_len = sizeof(sock_struct);

        int connect_rt = connect(socket_fd, (const sockaddr*)(&sock_struct), sock_len);

        if (connect_rt == -1)
        {   
            printf("connect failure\n");
        }
        else
        {
            #ifdef DEBUG
                printf("connect success\n");
            #endif

            bool flag = true;

            while (flag)
            {
                char cmd[2];
                printf("\n=============\n");
                printf("c - close connection\np - to ping\ne - to close server\n");
                printf("=============\n");
                printf("Enter the cmd:");
                scanf("%s", &cmd);
        
                switch (cmd[0])
                {
                case 'p':
                    {
                        pid_t cur_pid = getpid();
                        char buf[100];
                        sprintf(buf, "%d", cur_pid);

                        int sendall_rt = sendall(socket_fd, buf, sizeof(buf), 0);
                        
                        if (sendall_rt == -1)
                        {
                            printf("sendall failure\n");
                        }
                        else
                        {
                            #ifdef DEBUG
                                printf("sendall success, send total %d bytes\n", sendall_rt);
                            #endif

                            int recv_rt = recv(socket_fd, buf, sizeof(buf), 0);

                            if (recv_rt == 0)
                            {
                                printf("recv failure, connection lost\n");
                            }
                            else if (recv_rt == -1)
                            {
                                printf("errno %d\n", errno);
                                printf("recv failure, something happend\n");
                            }
                            else 
                            {
                                #ifdef DEBUG
                                    printf("recv read %d bytes\n", recv_rt);
                                #endif

                                printf("\n========\n");
                                printf("Message for the client is: %s\n", buf);
                                printf("========\n");
                            }
                        }
                        break;
                    }
                case 'c':
                    {
                        char buf[100];
                        sprintf(buf, "close");

                        int sendall_rt = sendall(socket_fd, buf, sizeof(buf), 0);
                        
                        if (sendall_rt == -1)
                        {
                            printf("sendall failure\n");
                        }
                        else
                        {
                            #ifdef DEBUG
                                printf("sendall success, send total %d bytes\n", sendall_rt);
                            #endif

                            int recv_rt = recv(socket_fd, buf, sizeof(buf), 0);

                            if (recv_rt == 0)
                            {
                                printf("recv failure, connection lost\n");
                            }
                            else if (recv_rt == -1)
                            {
                                printf("errno %d\n", errno);
                                printf("recv failure, something happend\n");
                            }
                            else 
                            {
                                #ifdef DEBUG
                                    printf("recv read %d bytes\n", recv_rt);
                                #endif

                                printf("\n========\n");
                                printf("Message for the client is: %s\n", buf);
                                printf("========\n");
                            }
                        }

                        flag = false;
                        break;
                    }
                case 'e':
                    {
                        char buf[100];
                        sprintf(buf, "close_srv");

                        int sendall_rt = sendall(socket_fd, buf, sizeof(buf), 0);
                        
                        if (sendall_rt == -1)
                        {
                            printf("sendall failure\n");
                        }
                        else
                        {
                            #ifdef DEBUG
                                printf("sendall success, send total %d bytes\n", sendall_rt);
                            #endif

                            int recv_rt = recv(socket_fd, buf, sizeof(buf), 0);

                            if (recv_rt == 0)
                            {
                                printf("recv failure, connection lost\n");
                            }
                            else if (recv_rt == -1)
                            {
                                printf("errno %d\n", errno);
                                printf("recv failure, something happend\n");
                            }
                            else 
                            {
                                #ifdef DEBUG
                                    printf("recv read %d bytes\n", recv_rt);
                                #endif

                                printf("\n========\n");
                                printf("Message for the client is: %s\n", buf);
                                printf("========\n");
                            }
                        }

                        flag = false;
                        break;
                    }
                default:
                    printf("ERROR: invalid command, try again\n");
                    break;
                }

            }
        }
    }

    close(socket_fd);
    return 0;
}

