#include "funcs.hpp"

int main()
{
    get_inp();
    // prnt_inp();

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
    else
    {
        pthread_t new_thread_id2 = 0;
        pthread_t new_thread_id3 = 0;
        pthread_t new_thread_id4 = 0;

        int err_code = pthread_create(&new_thread_id2, NULL, new_thread_func, NULL);
        if (err_code != 0)
        {
            printf("ERROR: cannot pthread_create new_thread_id2\n");
        }

        err_code = pthread_create(&new_thread_id3, NULL, new_thread_func, NULL);
        if (err_code != 0)
        {
            printf("ERROR: cannot pthread_create new_thread_id3\n");
        }

        err_code = pthread_create(&new_thread_id4, NULL, new_thread_func, NULL);
        if (err_code != 0)
        {
            printf("ERROR: cannot pthread_create new_thread_id4\n");
        }

        err_code = pthread_join(new_thread_id2, NULL);
        if (err_code != 0)
        {
            printf("ERROR: cannot pthread_join new_thread_id2\n");
        }

        err_code = pthread_join(new_thread_id3, NULL);
        if (err_code != 0)
        {
            printf("ERROR: cannot pthread_join new_thread_id3\n");
        }

        err_code = pthread_join(new_thread_id4, NULL);
        if (err_code != 0)
        {
            printf("ERROR: cannot pthread_join new_thread_id4\n");
        }

        res_val++;
        print_res_val();
    }

    return 0;
}
