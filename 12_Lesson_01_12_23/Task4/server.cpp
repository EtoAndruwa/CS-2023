#include "server.h"

int main()
{
    Server_struct server;
    
    if (ctor_server(&server) == RETURN_OK) 
    {    
        int err_code = create_socket(&server);
        if (err_code != RETURN_OK)
        {
            return err_code;
        }

        print_server_data(&server);
        logic(&server);
        close(server.socket_fd);
    }
    
    dtor_server(&server);
    return 0;
}
