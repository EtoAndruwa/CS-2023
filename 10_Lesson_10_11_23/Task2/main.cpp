#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{   
    int fd = open("file.txt", O_RDWR);
    int size = lseek(fd, 0, SEEK_END); // to get the size of the original file

    //void* file_ptr = mmap(nullptr, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0); Changes the original file while we are working with it in the mmap segmemt
    void* file_ptr = mmap(nullptr, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);  // Creates the local copy of the file in the processMAP_SHARED
    if (file_ptr != nullptr)
    {
        ((char*)file_ptr)[0] = 'A';
        printf("The file is now: %s\n", (char*)file_ptr);

        if (munmap(file_ptr, size) == 0);
        {   
            printf("munmap is ok\n");
        }
    }
    else
    {
        printf("ERROR: unable to mmap the region for the file\n");
    }
    return 0;
}
