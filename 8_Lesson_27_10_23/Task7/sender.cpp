#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <string.h>
#include "info.hpp"

int main()
{
    umask(0);

    char cmd_text[CMD_MAX_LENGHT];
    int shm_key = ftok(FTOK_STR, FTOK_VAL);
    int shm_mem = shmget(shm_key, sizeof(size_t), 0666);

    if (shm_mem == -1)
    {
        shm_mem = shmget(shm_key, CMD_MAX_LENGHT * sizeof(char), 0666|IPC_CREAT);
    }

    void* shm_mem_ptr = shmat(shm_mem, NULL, 0);
    
    scanf("%[^\n]s", cmd_text);
    strcpy((char*)shm_mem_ptr, cmd_text);
    ((char*)shm_mem_ptr)[CMD_MAX_LENGHT - 1] = '\0';

    shmdt(shm_mem_ptr);

    return 0;
}
