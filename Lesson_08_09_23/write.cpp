#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

// #define DEBUG


int main()
{
    umask(0); // local msk clear
    char* file_name = (char*)"Test_file.txt";
    //int open_ret = open(file_name, O_WRONLY|O_CREAT|O_APPEND, 0666);
    int open_ret = open(file_name, O_WRONLY|O_CREAT|O_EXCL, 0666);
   
    #ifdef DEBUG
        printf("open_ret = %d\n", open_ret);
    #endif

    if(open_ret == -1)
    {
        printf("File \"%s\" already exists.\n", file_name);
    }
    else
    {
        char some_text[] = "Hello, World!\n";
    
        #ifdef DEBUG
            printf("text = %s\n", some_text);
            printf("strlen val = %d\n", strlen(some_text));
            printf("count val = %d\n", sizeof(char) * strlen(some_text));
        #endif
        write(open_ret, (void*)some_text, sizeof(char) * strlen(some_text));
    }

    close(open_ret); // closes fd

    return 0;
}