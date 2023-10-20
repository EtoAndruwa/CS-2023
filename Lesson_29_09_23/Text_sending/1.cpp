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
    printf("=====PROG 1=====\n");
    umask(0);
    char prog_text[MAX_SIZE];   
    get_text(prog_text);

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
        printf("\n\nshm_mem_ptr BEFORE: %s\n", ((char*)shm_mem_ptr));
    #endif
    
    strcpy(((char*)shm_mem_ptr), prog_text);

    #ifdef DEBUG
        printf("\n\nshm_mem_ptr AFTER: %s\n", ((char*)shm_mem_ptr));
    #endif
    
    shmdt(shm_mem_ptr);

    return 0;
}

void get_text(char* const prog_text)
{
    FILE* file_ptr = fopen("./1.cpp", "r");

    fread(prog_text, 1, MAX_SIZE, file_ptr);
    prog_text[MAX_SIZE - 1] = '\0';

    fclose(file_ptr);
}
