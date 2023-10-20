#ifndef LESSON_20_10_23_TIM_AD
#define LESSON_20_10_23_TIM_AD

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <stdio.h>

double x_val          = 0;
int    degree         = 0;
double res_val        = 0;
int    cur_thread_num = 1;

void get_inp()
{
    printf("Enter x: ");
    scanf("%lf", &x_val);
    printf("Enter n: ");
    scanf("%d", &degree);
}

void prnt_inp()
{
    printf("Entered n = %d\n", degree);
    printf("Entered x = %lf\n", x_val);
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

    for (size_t i = 0; i <= (degree - cur_thread_num_) / 3; i++)
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
        n_val += 3;
    }
}

void print_res_val()
{
    printf("\n==========================\n");
    printf("res_val %lf\n", res_val);
    printf("==========================\n");
}


#endif
