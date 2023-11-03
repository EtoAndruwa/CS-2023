#include <stdio.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

void print_stat(struct stat* buf);

int main(int argc, char* argv[])
{
    struct stat buf;
    if (argc == 2)
    {
        stat(argv[1], &buf);
        print_stat(&buf);
    }
    else
    {
        printf("No file path\n");
    }
    
    return 0;
}

void print_stat(struct stat* buf)
{
    printf("\n===================================\n");
    printf("Устройство: %ld\n", buf->st_dev);
    printf("Inode: %ld\n", buf->st_ino);
    printf("Режим доступа: %ld\n", buf->st_mode);
    printf("Кол-во жестких ссылок: %ld\n", buf->st_nlink);
    printf("Идентификатор пользователя-владельца: %ld\n", buf->st_uid);
    printf("Идентификатор группы-владельца: %ld\n", buf->st_gid);
    printf("Тип устройства: %ld\n", buf->st_rdev);
    printf("Общий размер в байтах: %ld\n", buf->st_size);
    printf("Размер блока вводы-вывода: %ld\n", buf->st_blksize);
    printf("Кол-во выделенных блоков: %ld\n", buf->st_blocks);
    printf("Время последнего доступа: %s", ctime(&buf->st_atim.tv_sec));
    printf("Время последнего модификации: %s", ctime(&buf->st_mtim.tv_sec));
    printf("Время последнего изменения: %s", ctime(&buf->st_ctim.tv_sec));
    printf("===================================\n");
}


