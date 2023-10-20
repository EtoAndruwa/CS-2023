#ifndef LESSON_20_10_23_TIM_AD
#define LESSON_20_10_23_TIM_AD

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int arg;

void* new_thread_func(void* void_arg)
{
    pthread_t new_thread_id = pthread_self();
    printf("\nnew_thread_id from new_thread_func = %lu\n", new_thread_id);
    printf("arg in tread before %d\n", arg);
    arg++;
    printf("arg in tread after %d\n", arg);
    scanf("%d");
}

#endif
