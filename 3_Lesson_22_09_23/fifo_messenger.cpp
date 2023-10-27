#include "fifo.h"

void get_msg(const char* const usr_name_ptr, int* const fifo_flag)
{
    char msg[TOTAL_MAX_MSG_LENGTH] = "";
    const char* const fifo = cat_fifo(usr_name_ptr);

    int desc = 0;
    if ((desc = open(fifo, O_RDONLY)) == -1)
    {
        printf("\n#######################################\n");
        printf("No messages yet... :,(\n");
        printf("#######################################\n");
    }
    else
    {
        *fifo_flag = 1;

        printf("\n#######################################\n");
        printf("Gettting messages...\n");
        printf("#######################################\n");
        read(desc, msg, TOTAL_MAX_MSG_LENGTH);
        printf("\n%s\n", msg);

        printf("\n#######################################\n");
        printf("Message was received\n");
        printf("#######################################\n");
        close(desc);
    }
}

void wrt_msg(const char* const usr_name_ptr)
{
    char msg[MAX_MSG_LENGTH] = "";
    char usr_name[MAX_NAME_LENGTH] = "";
    printf("To whom you would like send this message?: ");
    scanf(" %[^\n]s", usr_name);
    usr_name[MAX_NAME_LENGTH - 1] = '\0';

    printf("Enter your message here: ");
    scanf(" %[^\n]s", msg);
    msg[MAX_MSG_LENGTH - 1] = '\0';

    printf("\n#######################################\n");
    printf("Sending your message to %s...\n", usr_name);
    printf("#######################################\n");
    
    char* fifo = cat_fifo(usr_name);

    int desc = 0;
    if ((desc = open(fifo, O_WRONLY)) == -1)
    {
        printf("\n#######################################\n");
        printf("There is no such user online...\n");
        printf("#######################################\n");
    }
    else
    {
        char final_msg[MAX_MSG_LENGTH + MAX_NAME_LENGTH + 8] = "From ";
        strcat(final_msg, usr_name_ptr);
        strcat(final_msg, " : ");
        strcat(final_msg, msg);

        write(desc, final_msg, TOTAL_MAX_MSG_LENGTH);
        printf("\n#######################################\n");
        printf("Message was read by %s\n", usr_name);
        printf("#######################################\n");
        close(desc);
    }
}   

void logic(char* const usr_name_ptr, int* const fifo_flag)
{
    char key = 0;
    while (key != 'q')
    {
        printf("\nEnter the command: ");
        scanf(" %c", &key);

        switch (key)
        {
        case ('h'):
            {
                print_info();

                #ifdef DEBUG
                    printf("Key is - %c\n", key);
                #endif
                break;
            }
        case ('r'):
            {
                #ifdef DEBUG
                    printf("Key is - %c\n", key);
                #endif

                get_msg(usr_name_ptr, fifo_flag);
                break;
            }
        case ('w'):
            {
                #ifdef DEBUG
                    printf("Key is - %c\n", key);
                #endif

                wrt_msg(usr_name_ptr);
                break;
            }
        case ('q'):
            {
                #ifdef DEBUG
                    printf("Key is - %c\n", key);
                #endif
                break;
            }
        default:
            {
                printf("\n##################################\n");
                printf("Error: unknown command. Try again!\n");
                printf("##################################\n\n");
                print_info();
                break;
            }
        }
    }
}


void greeting() // OK
{
    printf("\n##############################\n");
    printf("Welcome to the FIFO messanger!\n");
    printf("##############################\n");
}

void get_usr_name(char* const usr_name_ptr) // OK
{
    printf("\nIn order to send and receive messages fifo_messenger must know your user name.\n");
    printf("What is your user name?\n");
    printf("Enter your user name here (max length is 20 chars): "); 

    scanf("%[^\n]s", usr_name_ptr);
    usr_name_ptr[20] = '\0';
    printf("Hello, %s!\n", usr_name_ptr);    
}

void print_info() // OK
{
    printf("\n#########################################################\n");
    printf("Choose from options and write the letter in the console:\n");
    printf("h - print the help info one more time\n");
    printf("r - read messages from another user\n");
    printf("w - write the message to the user\n");
    printf("q - quit from the fifo messanger\n");
    printf("#########################################################\n");
}

void farewell() // OK
{
    printf("\n##############################\n");
    printf("###########Good bye!##########\n");
    printf("##############################\n");
}

char* cat_fifo(const char* const usr_name_ptr) // OK
{
    char fifo[24] = "fifo";
    return strcat(fifo, usr_name_ptr);
}

void del_fifo(const char* const usr_name_ptr) // OK
{
    char fifo[32] = "rm -r fifo";
    strcat(fifo, usr_name_ptr);
    system(fifo);
}

void create_fifo(const char* const usr_name_ptr)
{
    char* fifo = cat_fifo(usr_name_ptr);
    int mk_ret = mkfifo(fifo, 0777); 
}
