#include "funcs.hpp"

 extern int errno;

int main()
{
    struct sembuf sops[1];

    const int sem_id = get_sem_id();

    func_p(&sem_id, sops);

    printf("Hello, World!\n");

    return 0;
}
