#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define DEBUG


int main()
{
    char* file_name = (char*)"Test_file.txt";
    int open_ret = open(file_name, O_RDONLY); // opens file
   
    if(open_ret == -1)
    {
        printf("File \"%s\" does not exist.\n", file_name);
    }
    else
    {
        char buff[20];
        int read_ret = read(open_ret, (void*)buff, 20);
        
        #ifdef DEBUG
            if(read_ret == -1)
            {
                printf("ERROR -1 while reading file.\n");
            }
            else if(read_ret == 0)
            {
                printf("EOF.\n");
            }
            else
            {
                printf("File was read.\n");
                printf("Buff = %s\n", buff);
            }
        #endif
    }
  
    close(open_ret); // closes fd

    return 0;
}