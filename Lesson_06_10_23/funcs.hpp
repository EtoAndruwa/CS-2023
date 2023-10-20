#pragma once

#include <sys/ipc.h>
#include <stdio.h>
#include <sys/sem.h>

// #define DEBUG

#define char_size sizeof(char)

void func_p(const int* const sem_id, sembuf* struct_ptr)
{  
    struct_ptr[0].sem_num = 0;
    struct_ptr[0].sem_flg = 0;
    struct_ptr[0].sem_op = -1;
    
    printf("Waiting for sem...\n");
    int rtrn = semop(*sem_id, struct_ptr,1);
    
    #ifdef DEBUG
        if (rtrn == -1) 
        {
            printf ("\nsemop FAILURE!\n");
        }

        else 
        {
            printf ("\nsemop SUCCESS!\n");
            printf ("rtrn = %d\n", rtrn);
        }
    #endif
}

void func_v(const int* const sem_id, sembuf* struct_ptr)
{
    struct_ptr[0].sem_flg = 0;
    struct_ptr[0].sem_flg = 0;
    struct_ptr[0].sem_op = 1;

    semop(*sem_id, struct_ptr,1);

    int rtrn = semop(*sem_id, struct_ptr, 1);

    #ifdef DEBUG
        if (rtrn == -1) 
        {
            printf ("\nsemop FAILURE!\n");
        }

        else 
        {
            printf ("\nsemop SUCCESS!\n");
            printf ("rtrn = %d\n", rtrn);
        }
    #endif
}

// void* delete_sh_mem(void* sh_mem_ptr, int sh_mem) // ok
// {
//     shmdt(sh_mem_ptr);
//     shmctl(sh_mem, IPC_RMID, 0);
// }

int get_sem_id() // ok
{
    const int sem_key = ftok("main.cpp", 4);
    #ifdef DEBUG
        printf("sem_key %d\n", sem_key);
    #endif
    int sem_id = semget(sem_key, 1, 0666);
    if (sem_id == -1)
    {
        #ifdef DEBUG
            printf("semget 0666|IPC_CREAT\n");
        #endif
        sem_id = semget(sem_key, 1, 0666|IPC_CREAT);;
    }
    #ifdef DEBUG
        printf("sem_id %d\n", sem_id);
        printf("semget 0666\n");
    #endif

    return sem_id;
}
