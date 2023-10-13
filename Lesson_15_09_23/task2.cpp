#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv [], char* env[])
{
    printf("\n\ntask2.cpp\n");

    printf("args = %ld\n", argc);
    printf("argv array elems:\n");
    for(int i = 0; i < argc; i++)
    {
        printf("Elem #%ld = %s\n", i, argv[i]);
    }

    execl("./task1.out", "./task1.out", (char *) NULL);

    printf("\n\nEnd of task2.cpp\n\n");


    return 0;
};