#include "funcs.hpp"

int main()
{
    pthread_t thread_id = pthread_self();
    printf("thread_id = %lu\n", thread_id);

    pthread_t new_thread_id = 0;
    printf("arg in main before %d\n", arg);
    int err_code = pthread_create(&new_thread_id, NULL, new_thread_func, NULL);
    
    if (err_code != 0)
    {
        printf("ERROR: cannot pthread_create\n");
    }

    printf("new_thread_id from main %lu\n", new_thread_id);
    printf("arg in main after pthread_create %d\n", arg);
    err_code = pthread_join(new_thread_id, NULL);
    printf("arg in main after pthread_join %d\n", arg);

    if (err_code != 0)
    {
        printf("ERROR: cannot pthread_join\n");
    }
    
    // printf("join was done\n");

    return 0;
}
