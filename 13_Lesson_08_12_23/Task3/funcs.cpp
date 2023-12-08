#include "funcs.hpp"

void print_dir(const char* const dir_name)
{
    struct stat buf;
    size_t dir_name_strlen = strlen(dir_name);

    DIR* dir_fd = opendir(dir_name);

    if (dir_fd == nullptr)
    {
        printf("ERROR: dir_fd is nullptr\n");
    }
    else 
    {
        while (dirent* dir_struct_ptr = readdir(dir_fd))
        {   
            stat(dir_struct_ptr->d_name, &buf);
            if (S_ISDIR(buf.st_mode))
            {
                if (!strcmp(dir_struct_ptr->d_name, ".") && !strcmp(dir_name, "."))
                {
                    printf("%s%s %s\t\t <---- This dir\n", RED, dir_struct_ptr->d_name, RESET);
                }
                else if (!strcmp(dir_struct_ptr->d_name, "..") && !strcmp(dir_name, "."))
                {
                    printf("%s%s %s\t\t <---- The parent dir for this\n", RED,dir_struct_ptr->d_name, RESET);
                }
                else if (strcmp(dir_struct_ptr->d_name, "..") != 0 && strcmp(dir_struct_ptr->d_name, ".") != 0)
                {
                    if (dir_struct_ptr->d_type == DT_DIR)
                    {
                        printf("%s%s %s\t\t\n", RED,dir_struct_ptr->d_name, RESET);

                        size_t dir_name_child_strlen = strlen(dir_struct_ptr->d_name);
                        char* str_ptr = (char*)calloc(dir_name_child_strlen + 2 + dir_name_strlen, sizeof(char));

                        sprintf(str_ptr, "%s", dir_name);
                        strcat(str_ptr, "/");
                        strcat(str_ptr, dir_struct_ptr->d_name);

                        print_dir(str_ptr);
                    }
                }
            }
        }
    }

    closedir(dir_fd);
}
