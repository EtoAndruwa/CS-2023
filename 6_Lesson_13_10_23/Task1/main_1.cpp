#include "funcs.hpp"

int main()
{
    Message msg;
    
    int msg_id = get_msg_id();

    printf("Print \'m\' to set msg\n");
    char key = 0;
    while (key != 'm')
    {
        printf("\nEnter the command: ");
        scanf(" %c", &key);

        switch (key)
        {
        case ('m'):
            {
                analog_V(msg_id, &msg);
                break;
            }
        default:
            {
                printf("Print \'m\' to set msg\n");
            }
        }
    }
    

    msgctl(msg_id, IPC_RMID, NULL);
    return 0;
}

