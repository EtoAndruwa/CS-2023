#pragma once

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

// #define DEBUG
#define MAX_MESSAGE_LENGTH 1000
#define MAX_NAME_LENGTH 21
#define TOTAL_MAX_MSG_LENGTH (MAX_MESSAGE_LENGTH + MAX_NAME_LENGTH + 8)

int write_message(const char* const usr_name_ptr, void* shm_mem_ptr)
{
    char text[MAX_MESSAGE_LENGTH]; 
    printf("Enter your message here: ");
    scanf(" %[^\n]s", text);
    text[MAX_MESSAGE_LENGTH - 1] = '\0';

    char final_msg[TOTAL_MAX_MSG_LENGTH] = "From ";
    strcat(final_msg, usr_name_ptr);
    strcat(final_msg, " : ");
    strcat(final_msg, text);

    #ifdef DEBUG
        printf("\n\nshm_mem_ptr BEFORE: %s\n", ((char*)shm_mem_ptr));
    #endif
    
    strcpy(((char*)shm_mem_ptr), final_msg);

    #ifdef DEBUG
        printf("\n\nshm_mem_ptr AFTER: %s\n", ((char*)shm_mem_ptr));
    #endif
}

int read_message(void* shm_mem_ptr)
{
    char final_msg[TOTAL_MAX_MSG_LENGTH]; 

    #ifdef DEBUG
        printf("\n\nprog_text BEFORE: %s\n", final_msg);
    #endif
    
    strcpy(final_msg, ((char*)shm_mem_ptr));

    printf("%s\n", final_msg);

    #ifdef DEBUG
        printf("\n\nprog_text AFTER: %s\n", final_msg);
    #endif
}

void get_usr_name(char* const usr_name_ptr) // OK
{
    printf("\nIn order to send and receive messages we must know your user name.\n");
    printf("What is your user name?\n");
    printf("Enter your user name here (max length is 20 chars): "); 

    scanf("%[^\n]s", usr_name_ptr);
    usr_name_ptr[20] = '\0';
    printf("\n\nHello, %s!\n", usr_name_ptr);    
}

void print_info() // OK
{
    printf("\n#########################################################\n");
    printf("Choose from options and write the letter in the console:\n");
    printf("h - print the help info one more time\n");
    printf("r - read messages from another user\n");
    printf("w - write the message to the user\n");
    printf("q - quit from the messanger\n");
    printf("#########################################################\n");
}

void farewell() // OK
{
    printf("\n##############################\n");
    printf("###########Good bye!##########\n");
    printf("##############################\n");
}

void logic(char* const usr_name_ptr, void* shm_mem_ptr)
{
    char key = 0;
    while(key != 'q')
    {
        printf("\nEnter the command: ");
        scanf(" %c", &key);

        switch (key)
        {
        case ('h'):
            {
                print_info();
                break;
            }
        case ('r'):
            {
                read_message(shm_mem_ptr);
                break;
            }
        case ('w'):
            {
                write_message(usr_name_ptr, shm_mem_ptr);
                break;
            }
        case ('q'):
            {
                break;
            }
        default:
            {
                printf("\n##################################\n");
                printf("Error: unknown command. Try again!\n");
                printf("##################################\n\n");
                print_info();
                break;
            }
        }
    }
}

void* create_sh_mem(int shm_mem) // ok
{
    void* shm_mem_ptr = shmat(shm_mem, NULL, 0);
    return shm_mem_ptr;
}

void* delete_sh_mem(void* shm_mem_ptr, int shm_mem) // ok
{
    shmdt(shm_mem_ptr);
    shmctl(shm_mem, IPC_RMID, 0);
}

int get_shmem() // ok
{
    int shm_key = ftok("main.cpp", 4);
    #ifdef DEBUG
        printf("shm_key %d\n", shm_key);
    #endif
    int shm_mem = shmget(shm_key, TOTAL_MAX_MSG_LENGTH, 0666);
    if(shm_mem == -1)
    {
        #ifdef DEBUG
            printf("shmget 0666|IPC_CREAT\n");
        #endif
        shm_mem = shmget(shm_key, TOTAL_MAX_MSG_LENGTH, 0666|IPC_CREAT);
    }
    #ifdef DEBUG
        printf("shm_mem %d\n", shm_mem);
        printf("shmget 0666\n");
    #endif

    return shm_mem;
}

void greeting()
{
    printf("\n##############################\n");
    printf("############WELCOME############\n");
    printf("##############################\n");
}
