#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "reg.h"
#ifndef HASH_H
#define HASH_H



typedef struct s_variable{
    char reg[260];
    char name[260];
} variable;

void initializeHashMap();
void deallocHashMap();

variable* insert(const char* identifier);
variable* find(const char* identifier);



#endif
