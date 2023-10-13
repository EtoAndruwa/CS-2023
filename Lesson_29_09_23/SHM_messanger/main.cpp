#include "shared_memory.hpp"

int main()
{
    char usr_name[MAX_NAME_LENGTH] = "New user";  
    
    greeting();
    get_usr_name(usr_name);

    int shm_mem = get_shmem();
    void* sh_mem_ptr = create_sh_mem(shm_mem);
    print_info();
    logic(usr_name, sh_mem_ptr);
    farewell();
    delete_sh_mem(sh_mem_ptr, shm_mem);

    return 0;
}
