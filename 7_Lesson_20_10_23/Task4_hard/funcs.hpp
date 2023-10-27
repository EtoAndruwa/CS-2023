#ifndef LESSON_20_10_23_TIM_AD
#define LESSON_20_10_23_TIM_AD

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>

double x_val            = 0;
int    degree           = 0;
double res_val          = 0;
int    cur_thread_num   = 1;
int    total_thread_num = 0;

void get_inp()
{
    printf("Enter x: ");
    scanf("%lf", &x_val);
    printf("Enter n: ");
    scanf("%d", &degree);
    printf("Enter the number of threads: ");
    scanf("%d", &total_thread_num);
}

void prnt_inp()
{
    printf("Entered n = %d\n", degree);
    printf("Entered x = %lf\n", x_val);
    printf("Entered total_thread_num = %d\n", total_thread_num);
}

unsigned int factorial(unsigned int n_)
{
    int result = 1, i;
 
    for (i = 2; i <= n_; i++) {
        result *= i;
    }
 
    return result;
}

void* new_thread_func(void* void_arg)
{
    pthread_t id = pthread_self();
    // printf("thread started with id %lu\n", id);
    int cur_thread_num_ = cur_thread_num;
    cur_thread_num++;
    // printf("cur_thread_num_ %d\n", cur_thread_num_);
    double cal_res = 0;
    int n_val = cur_thread_num_;

    for (size_t i = 0; i <= (degree - cur_thread_num_) / total_thread_num; i++)
    {
        cal_res = (pow(x_val, n_val) / factorial(n_val));
        if (isfinite(cal_res))
        {
            // printf("n_thread = %d n_val = %d val = %lf\n", cur_thread_num_, n_val, cal_res);
            res_val += cal_res;
        }
        else 
        {
            // printf("skipped n_thread = %d i = %d val = %lf\n", cur_thread_num_, n_val, cal_res);
        }
        n_val += total_thread_num;
    }
}

void print_res_val()
{
    printf("\n==========================\n");
    printf("res_val %lf\n", res_val);
    printf("==========================\n");
}

int one_thread_calc()
{
    double cal_res = 0;
    for (size_t i = 0; i <= degree; i++)
    {
        cal_res = (pow(x_val, i) / factorial(i));
        if (isfinite(cal_res))
        {
            res_val += cal_res;
        }
    }
    print_res_val();
    return 0;
}

int dynamic_threads()
{
    pthread_t* threads_ptr = (pthread_t*)calloc(total_thread_num, sizeof(pthread_t));
    if (threads_ptr == nullptr)
    {
        printf("ERROR: cannot calloc threads_ptr\n");
        printf("Calculating on this main thread...\n");

        return one_thread_calc();
    }

    int err_code = 0;
    for (size_t i = 0; i < total_thread_num; i++)
    {
        err_code = pthread_create((&threads_ptr[i]), NULL, new_thread_func, NULL);
        if (err_code != 0)
        {
            printf("ERROR: cannot pthread_create new_thread_id%d\n", i);
        }
    }

    for (size_t i = 0; i < total_thread_num; i++)
    {
        err_code = pthread_join(threads_ptr[i], NULL);
        if (err_code != 0)
        {
            printf("ERROR: cannot pthread_join new_thread_id%d\n", i);
        }
    }

    res_val++;
    print_res_val();

    free(threads_ptr);
    return 0;
}

#endif
