#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#ifndef STRUCT_DATA
#define STRUCT_DATA

//structs
struct Question
{
    char* quest;
    char* ans;
};


#endif

//functions
struct Question* read_gag();
