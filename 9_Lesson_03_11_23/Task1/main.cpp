#include <fcntl.h>

int main()
{
    creat("./file3.txt", 0666);

    return 0;
}
