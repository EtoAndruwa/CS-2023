#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUG
#define MAX_SIZE 10000

void get_text(char* const prog_text);

int main()
{
    printf("=====PROG 2=====\n");
    umask(0);
    char prog_text[MAX_SIZE];   

    int shm_key = ftok("./text_sending/1.cpp", 4);
    #ifdef DEBUG
        printf("shm_key %d\n", shm_key);
    #endif
    int shm_mem = shmget(shm_key, MAX_SIZE, 0666);
    if (shm_mem == -1)
    {
        #ifdef DEBUG
            printf("shmget 0666|IPC_CREAT\n");
        #endif
        shm_mem = shmget(shm_key, MAX_SIZE, 0666|IPC_CREAT);
    }
    #ifdef DEBUG
        printf("shm_mem %d\n", shm_mem);
        printf("shmget 0666\n");
    #endif

    void* shm_mem_ptr = shmat(shm_mem, NULL, 0);
    #ifdef DEBUG
        printf("\n\nprog_text BEFORE: %s\n", prog_text);
    #endif
    
    strcpy(prog_text, ((char*)shm_mem_ptr));

    #ifdef DEBUG
        printf("\n\nprog_text AFTER: %s\n", prog_text);
    #endif
    
    shmdt(shm_mem_ptr);
    shmctl(shm_mem, IPC_RMID, 0);

    return 0;
}


