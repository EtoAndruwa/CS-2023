#ifndef SERVER_HPP_TIM_AD
#define SERVER_HPP_TIM_AD

#include "info.hpp"

void srv_ctor(SRV_struct* srv_struct_ptr)
{
    for (size_t i = 0; i < srv_struct_ptr->user_number; i++)
    {
        strcpy(srv_struct_ptr->users[i].usr_name, "");
    }
}


void srv_struct_print(const SRV_struct* const srv_struct_ptr)
{
    printf("\n============SRV_DUMP============\n");
    printf("srv_struct_ptr->user_number = %ld\n" , srv_struct_ptr->user_number);
    printf("srv_struct_ptr->users       = %p\n", srv_struct_ptr->users);


    printf("\n+++++++USER DATA+++++++\n");
    for(size_t i = 0; i < srv_struct_ptr->user_number; i++)
    {   
        printf("users[%ld]: usr_name = \'%s\', usr_id = %d\n", i, srv_struct_ptr->users[i].usr_name, srv_struct_ptr->users[i].usr_id);
    }
    printf("+++++++USER DATA+++++++\n");
    printf("============SRV_DUMP============\n");
}


#endif
