#include "funcs.hpp"

void* new_thread_func(void* arg)
{
    pthread_t new_thread_id = pthread_self();
    printf("new_thread_id from new_thread_func = %lu\n", new_thread_id);
    scanf("%d");
}
