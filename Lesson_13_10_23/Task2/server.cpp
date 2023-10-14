#include "server.hpp"

int main()
{
    SRV_struct srv_struct;
    srv_ctor(&srv_struct);
    srv_struct_print(&srv_struct);

    MSG_struct msg_struct;
    int msg_id = get_msg_id();
    bool cycle_bool = true;

    while (cycle_bool)
    {
        int ret_val = -1;   
        while (ret_val != 0)
        {
            ret_val = msgrcv(msg_id, &msg_struct, MSG_SIZE, USER_INIT_TYPE, IPC_NOWAIT);
            
            if (ret_val == -1)
            {
                ret_val = msgrcv(msg_id, &msg_struct, MSG_SIZE, USER_DEL_TYPE, IPC_NOWAIT);
                // printf("USER_DEL_TYPE msgrcv ret_val = %d\n", ret_val);

                if (ret_val == -1)
                {
                    ret_val = msgrcv(msg_id, &msg_struct, MSG_SIZE, USER_MSG_SND_TYPE, IPC_NOWAIT);
                    // printf("USER_MSG_SND_TYPE msgrcv ret_val = %d\n", ret_val);

                    if(ret_val == -1)
                    {
                        // printf("No requests to the server\n");
                    }
                    else
                    {
                        
                    }
                }
                else
                {
                    for (size_t i = 0; i < srv_struct.user_number; i++)
                    {
                        if (srv_struct.users[i].usr_id == msg_struct.sender_id)
                        {
                            srv_struct.users[i].usr_id = -1;
                            strcpy(srv_struct.users[i].usr_name, "");
                            srv_struct.free_cell = i;
                            srv_struct.user_number--;
                            srv_struct.free_cell = i;

                            break;
                        }
                    }
                    srv_struct_print(&srv_struct);
                }
            }
            else
            {
                if (srv_struct.user_number != INIT_USR_NUM)
                {
                    srv_struct.users[srv_struct.user_number].usr_id = msg_struct.sender_id;
                    strcpy(srv_struct.users[srv_struct.user_number].usr_name, msg_struct.text);
                    srv_struct.user_number++;
                    srv_struct.free_cell++;


                    msg_struct.logic_package = SUCCESS;
                    msg_struct.msg_type = msg_struct.sender_id;

                    int ret_val = msgsnd(msg_id, &msg_struct, MSG_SIZE, 0);
                    if (ret_val == -1)
                    {
                        printf("errno %d\n", errno);
                        printf("ERROR: cannot send feedback to the user\n");
                        return -1;
                    }

                    srv_struct_print(&srv_struct);
                }
                else
                {
                    printf("Unable to connect user to the server\n");
                    printf("The signal will be sent to the user\n");

                    srv_struct_print(&srv_struct);
                }
            }
        }
    }

    msgctl(msg_id, IPC_RMID, NULL);
    return 0;    
}
