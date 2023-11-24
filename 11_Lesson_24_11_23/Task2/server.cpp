#include "libs.h" 

int main()
{
    struct sockaddr_in sock_struct;
    struct sockaddr_in sock_struct_client;

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("pid %d\n", getpid());
    
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
        sock_struct.sin_port        = htons(21000);
        sock_struct.sin_family      = AF_INET;

        int bind_rt = bind(socket_fd, (const sockaddr*)(&sock_struct), sizeof(sock_struct));
    
        if (bind_rt == -1)
        {
            printf("errno %d\n", errno);
            printf("bind failure\n");
        }
        else
        {
            #ifdef DEBUG
                printf("bind success\n");
            #endif

            #ifdef DEBUG
                printf("listen started\n");
            #endif

            bool flag_lstn = true;

            while (flag_lstn)
            {
                int listen_rt = listen(socket_fd, 10);
                if (listen_rt == -1)
                {
                    printf("listen failure\n");
                }
                else
                {
                    #ifdef DEBUG
                        printf("listen success\n");
                    #endif

                    socklen_t sock_len  = sizeof(sock_struct_client);

                    #ifdef DEBUG
                        printf("accept started\n");
                    #endif
                    /*
                    * connect_sok_fd - is a new fd to the created socket
                    * for communicaton between server and client
                    */

                    int connect_sok_fd = accept(socket_fd, (sockaddr*)(&sock_struct_client), &sock_len);
                
                    if (connect_sok_fd == -1)
                    {
                        printf("accept failure\n");
                    }
                    else
                    {
                        #ifdef DEBUG
                            printf("accept success\n");
                            printf("\n===============================\n");
                            printf("sock_struct_client.sin_addr.s_addr: %d\n", sock_struct_client.sin_addr.s_addr);
                            printf("sock_struct_client.sin_family: %d\n", sock_struct_client.sin_family);
                            printf("sock_struct_client.sin_port: %d\n", sock_struct_client.sin_port);
                            printf("===============================\n");
                        #endif

                        char buf[100];

                        /*
                        * here must be used the fd of the new socket that will be 
                        * used in order to communicate between server and client
                        * that RETURN ACCEPT
                        */
                        bool flag = true;

                        while (flag)
                        {
                            int recv_rt = recv(connect_sok_fd, buf, sizeof(buf), 0); 

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
                                printf("Message for the server is: %s\n", buf);
                                printf("========\n");

                                int sendall_rt = sendall(connect_sok_fd, buf, sizeof(buf), 0);
                    
                                if (sendall_rt == -1)
                                {
                                    printf("sendall failure\n");
                                }
                                else
                                {
                                    #ifdef DEBUG
                                        printf("sendall success, send total %d bytes\n", sendall_rt);
                                    #endif
                                }

                                if (!strcmp("close", buf))
                                {
                                    flag = false;
                                }
                                if (!strcmp("close_srv", buf))
                                {
                                    flag = false;
                                    flag_lstn = false;
                                }
                            }
                        }
                        close(connect_sok_fd);
                    } 
                }
            }
        }
    }

    close(socket_fd);
    return 0;
}
