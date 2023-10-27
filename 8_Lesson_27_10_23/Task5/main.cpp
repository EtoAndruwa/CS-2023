#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main()
{
    umask(0);

    int shm_key = ftok("./main.cpp", 4);
    int shm_mem = shmget(shm_key, sizeof(size_t), 0666);

    if (shm_mem == -1)
    {
        shm_mem = shmget(shm_key, 3 * sizeof(size_t), 0666|IPC_CREAT);
    }

    void* shm_mem_ptr = shmat(shm_mem, NULL, 0);
    
    if (((int*)shm_mem_ptr)[0] < 5)
    {
        ((int*)shm_mem_ptr)[0] += 1;
        printf("Hello, World!\n");
        printf("Shm %d\n", ((int*)shm_mem_ptr)[0]);
    }
    
    shmdt(shm_mem_ptr);

    return 0;
}
