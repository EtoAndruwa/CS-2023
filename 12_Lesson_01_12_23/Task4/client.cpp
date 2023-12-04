#include "client.h"

int main()
{   
    Client_struct client;
    Server_struct server;

    print_greeting();
    ctor_client(&client);
    ctor_default_server(&server);

    web_logic(&client, &server);

    dtor_client(&client);
    dtor_default_server(&server);
    return 0;
}
