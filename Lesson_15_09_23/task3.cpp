#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


int main()
{
    int pipefd[2];
    pipe(pipefd);

    const int NUM = 100;
    char str[NUM];
    
    if(fork())
    {
        close(pipefd[0]);
        int x = read(pipefd[0], str, NUM);
        printf("Parent read %d\n", x);
        strcpy(str, "Hello, World!");
        x = write(pipefd[1], str, NUM);
        printf("Parent write %d\n", x);
        close(pipefd[1]);
        x = write(pipefd[1], str, NUM);
        printf("Parent write %d\n", x);
    }
    else
    {
        close(pipefd[1]);
        int x = write(pipefd[1], "Hello", 6);
        printf("\nChild write %d\n", x);
        x = read(pipefd[0], str, NUM);
        printf("Child read %d\n", x);
        printf("%s\n", str);
        close(pipefd[0]);
        x = read(pipefd[0], str, NUM);
        printf("Child read %d\n", x);
    }


    return 0;
}