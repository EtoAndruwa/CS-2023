#include "libs.h"

int get_socket()
{
    return socket(AF_INET, SOCK_DGRAM, 0);
}

int logic_outer(int port_sd, char* ip_str_sd, int socket_fd, struct sockaddr_in* sock_struct_sd, struct sockaddr_in* sock_struct_rc, size_t size_struct_sd, size_t size_struct_rc)
{
    char cmd[2];
    print_help_outer();

    while (cmd[0] != 'q')
    {
        printf("\nEnter the command: ");
        scanf("%s", &cmd);
        cmd[1] = '\0';

        switch (cmd[0])
        {
        case 'c':
            {
                logic_inner(port_sd, ip_str_sd, socket_fd, sock_struct_sd, sock_struct_rc, size_struct_sd, size_struct_rc);
                print_help_outer();
                break;
            }
        case 'q':
            {
                break;
            }
        case 'h':
            {
                print_help_outer();
                break;
            }
        default:
            {
                printf("ERROR: invalid command, try again\n");
                print_help_outer();
                break;
            }
        }
    }
}

int logic_inner(int port_sd, char* ip_str_sd, int socket_fd, struct sockaddr_in* sock_struct_sd, struct sockaddr_in* sock_struct_rc, size_t size_struct_sd, size_t size_struct_rc)
{
    char cmd[2];
    char ip_str[50];
    int  port = 0;

    in_addr ip_addr_rc;


    bool ip_changed   = false;
    bool ip_set       = false;
    bool port_changed = false;
    bool port_set     = false;

    print_help_chat();

    while (cmd[0] != 'q')
    {
        printf("\nEnter the command: ");
        scanf("%s", &cmd);
        cmd[1] = '\0';

        switch (cmd[0])
        {
        case 's':
            {
                if (!ip_set && !port_set || (ip_set && !port_set) || (!ip_set && port_set)) 
                {
                    get_ip_str(ip_str);
                    ip_set = true;

                    get_port(&port);
                    port_set = true;

                    set_sock_struct(ip_str, port, &ip_addr_rc, sock_struct_rc);
                }

                if(ip_changed)
                {
                    set_sock_struct(ip_str, port, &ip_addr_rc, sock_struct_rc);
                    ip_changed = false;
                }

                if(port_changed)
                {
                    set_sock_struct(ip_str, port, &ip_addr_rc, sock_struct_rc);
                    port_changed = false;
                }

                print_help_chat_inner();
                printf("\nJust write here something...\n");

                int cur_pid  = getpid();
                int fork_val = fork();

                if (fork_val == -1)
                {
                    printf("ERROR: cannot enter the chat. Try again a bit later.\n");
                }
                else
                {   
                    int new_pid  = getpid();
                    int new_ppid = getppid();

                    char msg[MAX_MSG_LEN + PRE_MSG_LEN];
                    char pre_msg[PRE_MSG_LEN];
                    sprintf(pre_msg, "From %s %d: ", ip_str_sd, port_sd);

                    bool is_chatting = true;
                    while (is_chatting)
                    {
                        char temp[MAX_MSG_LEN];

                        if (new_ppid == cur_pid)
                        {
                            socklen_t sock_len = 0;
                            int recv_ret = recvfrom(socket_fd, msg, MAX_MSG_LEN + PRE_MSG_LEN, 0, (sockaddr*)(sock_struct_sd), &sock_len); // does not block 
                            if (recv_ret != -1)
                            {
                                printf("%s\n", msg);
                            }
                        }
                        else if (new_pid == cur_pid)
                        {
                            write_msg(temp);
                    
                            if (!strcmp(temp, "_exit_chat"))
                            {
                                is_chatting = false; 
                                kill(fork_val, SIGTERM); // kills the listening process
                                break;                             
                            }
                            if (!strcmp(temp, "_help"))
                            {
                                print_help_chat_inner();
                                continue;                          
                            }

                            sprintf(msg, "%s%s", pre_msg, temp);

                            int sendto_ret = sendto(socket_fd, msg, MAX_MSG_LEN + PRE_MSG_LEN, 0, (const sockaddr*)(sock_struct_rc), size_struct_rc);

                            if (sendto_ret == -1) 
                            {
                                printf("ERROR: cannot send the message to the given user, try again\n");
                            }
                        }
                    }
                }
                break;
            }
        case 'i':
            {
                get_ip_str(ip_str);
                ip_changed = true;
                ip_set     = true;
                break;
            }
        case 'p':
            {
                get_port(&port);
                port_changed = true;
                port_set     = true;
                break;
            }
        case 'q':
            {
                break;
            }
        case 'h':
            {
                print_help_chat();
                break;
            }
        default:
            {
                printf("ERROR: invalid command, try again\n");
                print_help_chat();
                break;
            }
        }
    }
}

int set_sock_struct(const char* ip_str, int port, in_addr* ip_addr_str, struct sockaddr_in* sock_str)
{
    inet_aton(ip_str, ip_addr_str);
    sock_str->sin_addr.s_addr = ip_addr_str->s_addr;
    sock_str->sin_port        = htons(port);
    sock_str->sin_family      = AF_INET;
}

int bind_socket(int socket_fd, const sockaddr* sock_str, size_t size_of_sock_str)
{
    return bind(socket_fd, sock_str, size_of_sock_str);
}

void write_msg(char* msg_ptr)
{
    char c;
    scanf("%c", &c);
    scanf("%[^\n]s", msg_ptr);
    msg_ptr[MAX_MSG_LEN - 1] = '\0';
}

void print_help_outer() // ok
{
    printf("\n==================HELP==================\n");
    printf("c - to chat with other user\n");
    printf("q - to quit from the messanger\n");
    printf("h - to print help one more time\n");
    printf("==================HELP==================\n");
}

void print_help_chat_inner() // ok
{
    printf("\n==================HELP==================\n");
    printf("_help - to print help\n");
    printf("_exit_chat - to exit from the chat\n");
    printf("==================HELP==================\n");
}

void print_help_chat() // ok
{
    printf("\n==================HELP==================\n");
    printf("s - send and receive messages\n");
    printf("i - change the user by IP change\n");
    printf("p - change the user by port change\n");
    printf("q - to quit from the messanger\n");
    printf("h - to print help one more time\n");
    printf("==================HELP==================\n");
}

void get_ip_str(char* ip_str)
{
    printf("Enter the user's new IP: ");
    scanf("%s", ip_str);
    printf("New IP %s\n", ip_str);
}

void get_port(int* port_ptr)
{
    printf("Enter the user's new port: ");
    scanf("%d", port_ptr);
    printf("New port %d\n", *port_ptr);
}

void set_my_socket(struct sockaddr_in* sock_struct_sd, char* ip_str, int* port, in_addr* ip_addr_sd)
{
    get_ip_str_my(ip_str);
    get_port_my(port);
    set_sock_struct(ip_str, *port, ip_addr_sd, sock_struct_sd);
}

void get_ip_str_my(char* ip_str)
{
    printf("Enter the your IP: ");
    scanf("%s", ip_str);
    printf("Your IP %s\n", ip_str);
}

void get_port_my(int* port_ptr)
{
    printf("Enter the your port: ");
    scanf("%d", port_ptr);
    printf("Your port %d\n", *port_ptr);
}
