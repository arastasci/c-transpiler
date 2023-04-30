#include "stdio.h"
#include "stdlib.h"
#ifndef REG_H
#define REG_H
#include "reg.h"
#include "string.h"
int current_reg_id;

//reg* createReg(char* name){
//    reg* new_reg = malloc(sizeof(reg));
//    char buf[128];
//    sprintf(buf,"%s",name);
//    new_reg->name = name;
//    new_reg->id = current_reg_id;
//    current_reg_id++;
//    // TODO: refactor code later for assigning and incrementing current_reg_id
//    return new_reg;
//}
reg* createEmptyVarReg(){
    reg* new_reg = malloc(sizeof(reg));
    new_reg->name = "";
    return new_reg;
}
reg* createRegDefault(){
    reg* new_reg = malloc(sizeof(reg));
    char* buffer = malloc(128 * sizeof(char));
    sprintf(buffer,"%%reg_%d", current_reg_id);
    new_reg->name = buffer;

    new_reg->id = current_reg_id;
    current_reg_id++;
    return new_reg;
}

void storeInVar(reg* var, reg* rhs){
    fprintf(output_file, "store i32 %s, i32* %s \n", rhs->name, var->name);
}

reg* loadVar(reg* var){
    reg* new_reg = malloc(sizeof(reg));
    char* buffer = malloc(128 * sizeof(char));
    sprintf(buffer,"%%reg_%d", current_reg_id);
    new_reg->name = buffer;

    new_reg->id = current_reg_id;
    current_reg_id++;
    fprintf(output_file, "%s = load i32, i32* %s \n", new_reg->name, var->name);
    return new_reg;
}

reg* createRegInteger(const char* value){
    reg* new_reg = malloc(sizeof(reg));
    new_reg->name = value;
    return new_reg;
}
#endif