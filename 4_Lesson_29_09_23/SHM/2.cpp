#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

// #define DEBUG

int main()
{
    umask(0);

    int shm_key = ftok("./3.cpp", 4);
    #ifdef DEBUG
        printf("shm_key %d\n", shm_key);
    #endif
    int shm_mem = shmget(shm_key, 3 * sizeof(size_t), 0666);
    if (shm_mem == -1)
    {
        #ifdef DEBUG
            printf("shmget 0666|IPC_CREAT\n");
        #endif
        shm_mem = shmget(shm_key, 3 * sizeof(size_t), 0666|IPC_CREAT);
    }
    #ifdef DEBUG
        printf("shm_mem %d\n", shm_mem);
        printf("shmget 0666\n");
    #endif

    void* shm_mem_ptr = shmat(shm_mem, NULL, 0);
    #ifdef DEBUG
        printf("shm_mem_ptr[0] = %d, shm_mem_ptr[1] = %d, shm_mem_ptr[2] = %d\n", ((int*)shm_mem_ptr)[0], ((int*)shm_mem_ptr)[1], ((int*)shm_mem_ptr)[2]);
    #endif
    
    ((int*)shm_mem_ptr)[1] += 1;
    ((int*)shm_mem_ptr)[2] = ((int*)shm_mem_ptr)[1] + ((int*)shm_mem_ptr)[0];

    #ifdef DEBUG
        printf("shm_mem_ptr[0] = %d, shm_mem_ptr[1] = %d, shm_mem_ptr[2] = %d\n", ((int*)shm_mem_ptr)[0], ((int*)shm_mem_ptr)[1], ((int*)shm_mem_ptr)[2]);
    #endif
    
    shmdt(shm_mem_ptr);

    return 0;
}
