#include "stdio.h"
#include "stdlib.h"
#ifndef REG_H
#define REG_H
#include "reg.h"

int current_reg_id;

reg* createReg(const char* name){
    reg* new_reg = malloc(sizeof(reg));

    new_reg->name = name;
    new_reg->id = current_reg_id;
    current_reg_id++;
    // TODO: refactor code later for assigning and incrementing current_reg_id
    return new_reg;
}

reg* createRegDefault(){
    reg* new_reg = malloc(sizeof(reg));
    char buffer[100];
    sprintf(buffer,"reg_%d", current_reg_id);
    new_reg->name = buffer;

    new_reg->id = current_reg_id;
    current_reg_id++;
    return new_reg;
}

void storeInVar(reg* var, reg* rhs){
    // TODO: print store thingy...
    fprintf(output_file, "store i32 %%%s, i32* %%%s", rhs->name, var->name);
}

reg* createRegInteger(const char* value){
    reg* new_reg = malloc(sizeof(reg));
    new_reg->name = value;
    return new_reg;
}
#endif