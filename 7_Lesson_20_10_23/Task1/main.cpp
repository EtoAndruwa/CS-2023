#include "funcs.hpp"

int main()
{
    pthread_t id = pthread_self();
    printf("thread id = %lu\n", id);

    return 0;
}
