#include "funcs.hpp"

int main()
{
    get_inp();
    prnt_inp();

    if (x_val == 0)
    {
        print_res_val();
        return 0;
    }
    
    if (degree < 0)
    {
        bool degree_flag = true;
        while (degree_flag)
        {
            printf("Please, reenter the n for approximation: ");
            scanf("%d", &degree);
            if (degree >= 0)
            {
                degree_flag = false;
            }
        }
    }

    if (degree == 0)
    {
        res_val = 1;
        print_res_val();
        return 0;
    }
    
    if (total_thread_num < 0)
    {
        bool thread_flag = true;
        while (thread_flag)
        {
            printf("Please, reenter the number of threads for approximation: ");
            scanf("%d", &total_thread_num);
            if (total_thread_num >= 0)
            {
                thread_flag = false;
            }
        }
    }
    
    if (total_thread_num == 0)
    {
        return one_thread_calc();
    }
    else
    {
        return dynamic_threads();
    }
}
