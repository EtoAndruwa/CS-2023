#include "server.hpp"

int main()
{
    SRV_struct srv_struct;
    MSG_struct msg_struct;

    int msg_id = get_msg_id();
    srv_ctor(&srv_struct);
    srv_struct_print(&srv_struct);

    int ret_val = -1;   
    while (ret_val != 0)
    {
        check_for_new(msg_id, &msg_struct, &srv_struct);
        check_for_offline(msg_id, &msg_struct, &srv_struct);    
        check_for_retake(msg_id, &msg_struct, &srv_struct);
    }

    msgctl(msg_id, IPC_RMID, NULL);
    return 0;    
}
