#include "funcs.hpp"

void logic(int argc, char* argv [])
{
    switch (argc)
    {
    case 2:
        if (!strcmp(argv[1], "ls"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(".");
            if (dir_fd == nullptr)
            {
                printf("case 2: dir_fd is nullptr\n");
            }
            else 
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    if (!S_ISDIR(buf.st_mode))
                    {
                        print_info(dir_struct_ptr, &buf);
                    }
                }
            }

            closedir(dir_fd);
        }
        else 
        {
            printf("ERROR: case 2: is not ls command. Try again\n");
        }   
        break;
    case 3: 
        if (!strcmp(argv[1], "ls") && !strcmp(argv[2], "-a"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(".");
            if (dir_fd == nullptr)
            {
                printf("case 3: dir_fd is nullptr\n");
            }
            else 
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    print_info(dir_struct_ptr, &buf);
                }
            }
        }
        else if (!strcmp(argv[1], "ls") && !strcmp(argv[2], "-l"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(".");
            if (dir_fd == nullptr)
            {
                printf("case 3: dir_fd is nullptr\n");
            }
            else
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    printf("\n===================================\n");
                    print_info(dir_struct_ptr, &buf);
                    print_stat(&buf);
                }
            }
            closedir(dir_fd);
        }
        else if (!strcmp(argv[1], "ls") && !strcmp(argv[2], "-d"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(".");
            if (dir_fd == nullptr)
            {
                printf("case 3: dir_fd is nullptr\n");
            }
            else
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    if (S_ISDIR(buf.st_mode))
                    {
                        print_info(dir_struct_ptr, &buf);
                    }
                }
            }
            closedir(dir_fd);
        }
        else if (!strcmp(argv[1], "ls") && strcmp(argv[2], "-a") && strcmp(argv[2], "-d") && strcmp(argv[2], "-ls"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(argv[2]);
            if (dir_fd == nullptr)
            {
                printf("case 3: dir_fd is nullptr\n");
            }
            else 
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    print_info(dir_struct_ptr, &buf);
                }
            }
            closedir(dir_fd);
        }
        else 
        {
            printf("ERROR: case 3: invalid flag for ls command. Try again\n");
        }
        break;
    case 4:
        if (!strcmp(argv[1], "ls") && !strcmp(argv[2], "-l") && !strcmp(argv[3], "-d"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(".");
            if (dir_fd == nullptr)
            {
                printf("case 2: dir_fd is nullptr\n");
            }
            else 
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    if (S_ISDIR(buf.st_mode))
                    {
                        printf("\n===================================\n");
                        print_info(dir_struct_ptr, &buf);
                        print_stat(&buf);
                    }
                }
            }

            closedir(dir_fd);
        }
        else if (!strcmp(argv[1], "ls") && !strcmp(argv[2], "-l"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(argv[3]);
            if (dir_fd == nullptr)
            {
                printf("case 4: dir_fd is nullptr\n");
            }
            else
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    printf("\n===================================\n");
                    print_info(dir_struct_ptr, &buf);
                    print_stat(&buf);
                }
            }
            closedir(dir_fd);
        }
        else if (!strcmp(argv[1], "ls") && !strcmp(argv[2], "-d"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(argv[3]);
            if (dir_fd == nullptr)
            {
                printf("case 4: dir_fd is nullptr\n");
            }
            else
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    if (S_ISDIR(buf.st_mode))
                    {
                        print_info(dir_struct_ptr, &buf);
                    }
                }
            }
            closedir(dir_fd);
        }
        else if (!strcmp(argv[1], "ls") && !strcmp(argv[2], "-a"))
        {
            struct stat buf;

            DIR* dir_fd = opendir(argv[3]);
            if (dir_fd == nullptr)
            {
                printf("case 3: dir_fd is nullptr\n");
            }
            else 
            {
                while (dirent* dir_struct_ptr = readdir(dir_fd))
                {   
                    stat(dir_struct_ptr->d_name, &buf);
                    print_info(dir_struct_ptr, &buf);
                }
            }
            closedir(dir_fd);
        }
        else
        {
            printf("ERROR: case 4: invalid path for the ls cmd. Try again\n");
        }
        break;
    default:
        printf("ERROR: (default) invalid number of args or invalid ls command. Try again\n");
        break;
    }
}

void print_stat(struct stat* buf)
{
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
    printf("===================================\n\n");
}

void print_info(dirent* dir_struct_ptr, struct stat* buf)
{
    if (S_ISDIR(buf->st_mode))
    {
        if (!strcmp(dir_struct_ptr->d_name, "."))
        {
            printf("%s%s %s\t\t <---- This dir\n", RED, dir_struct_ptr->d_name, RESET);
        }
        else if (!strcmp(dir_struct_ptr->d_name, ".."))
        {
            printf("%s%s %s\t\t <---- The parent dir for this\n", RED,dir_struct_ptr->d_name, RESET);
        }
        else 
        {
            printf("%s%s%s\n", RED, dir_struct_ptr->d_name, RESET);
        }
    }
    else if (strstr(dir_struct_ptr->d_name, ".cpp"))
    {
        printf("%s%s\n%s", CYN, dir_struct_ptr->d_name, RESET);
    }
    else if (strstr(dir_struct_ptr->d_name, ".out"))
    {
        printf("%s%s\n%s", GRN, dir_struct_ptr->d_name, RESET);
    }
    else if (strstr(dir_struct_ptr->d_name, ".o"))
    {
        printf("%s%s\n%s", BLU, dir_struct_ptr->d_name, RESET);
    }
    else 
    {
        printf("%s\n", dir_struct_ptr->d_name);
    }   
}
