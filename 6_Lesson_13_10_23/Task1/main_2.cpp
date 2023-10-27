#include "funcs.hpp"

int main()
{
    Message msg;

    int msg_id = get_msg_id();

    analog_P(msg_id, &msg);
    printf("Hello, World!\n");


    msgctl(msg_id, IPC_RMID, NULL);
    return 0;
}
