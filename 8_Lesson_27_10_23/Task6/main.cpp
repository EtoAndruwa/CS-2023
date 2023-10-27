#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>

int get_sem_id();
void func_v(const int* const sem_id, sembuf* struct_ptr, int num_of_proc);
void func_p(const int* const sem_id, sembuf* struct_ptr);

int main(int argc, char* argv[])
{
    struct sembuf sops[1];

    if (argc == 2)
    {   
        int proc_num = atoi(argv[1]);
        printf("proc_num %d\n", proc_num);

        int parent_id = getpid();

        for (size_t i = 0; i < proc_num; i++)
        {
            if (getpid() == parent_id)
            {   
                int fork_val = fork();

                if (fork_val == -1)
                {
                    printf("Cannot start new process\n");
                }  
            }
        }

        const int sem_id = get_sem_id();

        if (parent_id == getpid())
        {
            while (true)
            {
                func_v(&sem_id, sops, proc_num);
                // printf("s now %d\n", s);
                sleep(5);
            }
        }
        else
        {
            while (true)
            {
                func_p(&sem_id, sops);
                sleep(5);
            }
            
        }
    }
    else
    {
        printf("ERR: invalid number of argc\nEnter ./file_name.out num_of_proc\n");
    }


    return 0;
}

int get_sem_id()
{
    const int sem_key = ftok("main.cpp", 4);

    int sem_id = semget(sem_key, 1, 0666);
    if (sem_id == -1)
    {
        sem_id = semget(sem_key, 1, 0666|IPC_CREAT);;
    }

    return sem_id;
}

void func_p(const int* const sem_id, sembuf* struct_ptr)
{  
    struct_ptr[0].sem_num = 0;
    struct_ptr[0].sem_flg = 0;
    struct_ptr[0].sem_op  = -1;

    int rtrn = semop(*sem_id, struct_ptr,1);
    
    if (rtrn == -1) 
    {
        printf("Process with id %d did not receive signal\n", getpid());
    }

    else 
    {
        printf("Process with id %d received signal\n", getpid());
    }
}

void func_v(const int* const sem_id, sembuf* struct_ptr, int num_of_proc)
{
    struct_ptr[0].sem_num = 0;
    struct_ptr[0].sem_flg = 0;
    struct_ptr[0].sem_op  = num_of_proc;

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
