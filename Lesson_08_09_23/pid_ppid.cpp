#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

// #define DEBUG

int main()
{
    int cur_pid = getpid();
    int cur_ppid = getppid();
    int fork_val = fork();

    if (fork_val == -1)
    {
        printf("Cannot start new process\n");
    }
    else
    {   
        #ifdef DEBUG     
            printf("Current process id: %ld\n", getpid());
            printf("Current process PARENT id: %ld\n", getppid());
            printf("fork_val = %ld\n\n", fork_val);
        #endif

        int new_pid = getpid();
        int new_ppid = getppid();

        if (new_ppid == cur_pid)
        {
            printf("I am CHILD process. PID = %lu, PPID = %lu\n", new_pid, new_ppid);
        }
        else if (new_pid == cur_pid)
        {
            printf("I am PARENT process. PID = %lu, PPID = %lu\n", cur_pid, cur_ppid);
        }
    }

    return 0;
}
