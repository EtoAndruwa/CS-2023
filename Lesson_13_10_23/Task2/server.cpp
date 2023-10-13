#include "server.hpp"

int main()
{
    SRV_struct srv_struct;
    srv_ctor(&srv_struct);
    srv_struct_print(&srv_struct);

    MSG_struct msg_struct;
    int msg_id = get_msg_id();
    bool cycle_bool = true;

    while(cycle_bool)
    {
        int ret_val = -1;
        while(ret_val != 0)
        {
            // printf("total size = %ld\n", sizeof(msg_struct.receiver_id) + sizeof(msg_struct.text));
            ret_val = msgrcv(msg_id, &msg_struct, sizeof(msg_struct.receiver_id) + sizeof(msg_struct.text), USER_INIT_TYPE, IPC_NOWAIT);
            printf("INIT msgrcv ret_val = %d\n", ret_val);
            
            if(ret_val == -1)
            {
                ret_val = msgrcv(msg_id, &msg_struct, 0, USER_DEL_TYPE, IPC_NOWAIT);
                printf("USER_DEL_TYPE msgrcv ret_val = %d\n", ret_val);

                if(ret_val == -1)
                {
                    ret_val = msgrcv(msg_id, &msg_struct, 0, USER_MSG_SND_TYPE, IPC_NOWAIT);
                    printf("USER_MSG_SND_TYPE msgrcv ret_val = %d\n", ret_val);

                    if(ret_val == -1)
                    {
                        printf("No requests to the server\n");
                    }
                    else
                    {
                        
                    }
                }
                else
                {


                }
            }
            else
            {
                srv_struct.users[srv_struct.user_number].usr_id   = msg_struct.sender_id;
                strcpy(srv_struct.users[srv_struct.user_number].usr_name, msg_struct.text);
                srv_struct.user_number++;

                ret_val = -1;
            }
        }
    }

    msgctl(msg_id, IPC_RMID, NULL);
    return 0;    
}
