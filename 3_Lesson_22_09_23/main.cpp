#include "fifo.h"

int main(int argc, char* argv[], char* env[])
{
    char usr_name[MAX_NAME_LENGTH] = "New user";
    int fifo_flag = 0;

    #ifdef DEBUG
        printf("\nUser name:\"%s\"\n", usr_name);
        printf("Message:\"%s\"\n", msg);
    #endif

    greeting();
    get_usr_name(usr_name);
    create_fifo(usr_name);

    #ifdef DEBUG
        printf("\nUser name:\"%s\"\n", usr_name);
        printf("Message:\"%s\"\n", msg);
    #endif

    print_info();
    logic(usr_name, &fifo_flag);
    farewell();

    del_fifo(usr_name);
    return 0;
}
