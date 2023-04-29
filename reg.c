#include "stdio.h"
#include "stdlib.h"
#ifndef REG_H
#define REG_H
#include "reg.h"
reg* createReg(const char* name){
    reg* new_reg = malloc(sizeof(reg));

    new_reg->name = name;
    new_reg->id = current_reg_id;
    current_reg_id++;
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
reg* storeInVar(reg* var, reg* rhs){
    // TODO: print store thingy...
}
#endif