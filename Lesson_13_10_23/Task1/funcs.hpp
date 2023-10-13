#ifndef FUNCS_HPP_TIM_AD
#define FUNCS_HPP_TIM_AD

#include <stdio.h>
#include <sys/msg.h>
#include <sys/stat.h>

// #define DEBUG
const int msg_type = 1;

struct Message
{
    long int msg_type;
};


void analog_P(const int msg_id, Message* msg_struct_ptr);
void analog_V(const int msg_id, Message* msg_struct_ptr);
int get_msg_id();

#endif
