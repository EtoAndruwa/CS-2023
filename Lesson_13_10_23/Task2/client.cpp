#include "client.hpp"

int main()
{
    MSG_struct msg_struct;
    USR_struct usr_struct; 

    int msg_id = get_msg_id();
    usr_ctor(&usr_struct);
    print_user(&usr_struct);

    bool coonected = false;
    while(!coonected)
    {
        enter_web(msg_id, &msg_struct, &usr_struct);
        
    }

    client_logic();
    leave_web(msg_id, &msg_struct, &usr_struct);
    return 0;
}
