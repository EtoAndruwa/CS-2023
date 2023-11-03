#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * 
 * This prog is analog ln -s
 * 
*/

int main(int argc, char* argv[])
{
    if (argc == 3)
    {
        symlink(argv[1], argv[2]);   
    }
    else 
    {
        printf("ERROR: invalid number of arguments\n");
    }

    return 0;
}
