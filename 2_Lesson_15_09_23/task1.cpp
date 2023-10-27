#include <stdio.h>

int main(int argc, char* argv [], char* env [])
{
    printf("\n\nTask1.cpp\n");
    printf("args = %ld\n", argc);
    printf("argv array elems:\n");

    int i = 0;
    while (argv[i] != nullptr)
    {
        printf("Elem #%ld = %s\n", i, argv[i]);
        i++;
    }

    printf("\n\nenv array elems:\n");
    i = 0;
    while (env[i] != nullptr)
    {
        printf("Elem #%ld = %s\n", i, env[i]);
        i++;
    }
    printf("Elem #%ld = %s\n", i, "nullptr");
    printf("\nEnd of task1.cpp\n");

    return 0;
}
