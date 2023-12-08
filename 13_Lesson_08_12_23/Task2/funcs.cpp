#include "funcs.hpp"

int count_files(const char* const dir_name)
{
    int count = 0;
    struct stat buf;
    size_t dir_name_strlen = strlen(dir_name);

    DIR* dir_fd = opendir(dir_name);

    if (dir_fd == nullptr)
    {
        printf("ERROR: dir_fd is nullptr: %s\n", dir_name);
    }
    else 
    {
        while (dirent* dir_struct_ptr = readdir(dir_fd))
        {   
            stat(dir_struct_ptr->d_name, &buf); 
            if (S_ISDIR(buf.st_mode))
            {
                if (strcmp(dir_struct_ptr->d_name, "..") != 0 && strcmp(dir_struct_ptr->d_name, ".") != 0)
                {
                    if (dir_struct_ptr->d_type == DT_DIR)
                    {
                        size_t dir_name_child_strlen = strlen(dir_struct_ptr->d_name);
                        char* str_ptr = (char*)calloc(dir_name_child_strlen + 2 + dir_name_strlen, sizeof(char));

                        sprintf(str_ptr, "%s", dir_name);
                        strcat(str_ptr, "/");
                        strcat(str_ptr, dir_struct_ptr->d_name);

                        count += count_files(str_ptr);
                    }
                    else 
                    {
                        switch (dir_struct_ptr->d_type) {
                        case DT_BLK: count++;     break;
                        case DT_CHR: count++;     break;
                        case DT_FIFO: count++;    break;
                        case DT_LNK:  count++;    break;
                        case DT_REG:  count++;    break;
                        case DT_SOCK: count++;    break;
                        default:  break;
                        }
                    }
                }
            }
            else 
            {
                switch (dir_struct_ptr->d_type) {
                case DT_BLK: count++;     break;
                case DT_CHR: count++;     break;
                case DT_FIFO: count++;    break;
                case DT_LNK:  count++;    break;
                case DT_REG:  count++;    break;
                case DT_SOCK: count++;    break;
                default:  break;
                }
            }
        }
    }

    closedir(dir_fd);
    return count;
}
