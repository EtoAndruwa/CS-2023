#include "funcs.hpp"

int main()
{
    struct sembuf sops[1];
    const int sem_id = get_sem_id();


    printf("Print \'s\' to set semafor\n");
    char key = 0;
    while(key != 's')
    {
        printf("\nEnter the command: ");
        scanf(" %c", &key);

        switch (key)
        {
        case ('s'):
            {
                func_v(&sem_id, sops);
                break;
            }
        default:
            {
                printf("Print \'s\' to set semafor\n");
            }
        }
    }

    return 0;
}
