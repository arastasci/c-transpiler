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
char* createEmptyVarReg(){

    return "";
}
char* createRegDefault(){
    char* reg = malloc(128 * sizeof(char));
    sprintf(reg,"%%reg_%d", current_reg_id);
    current_reg_id++;
    return reg;
}

void storeInVar(char* var, char* rhs){
    fprintf(output_file, "store i32 %s, i32* %s \n", rhs, var);
}

char* loadVar(char* var){
    char* reg = malloc(128 * sizeof(char));
    sprintf(reg,"%%reg_%d", current_reg_id);
    current_reg_id++;
    fprintf(output_file, "%s = load i32, i32* %s \n", reg, var);
    return reg;
}

char* createRegInteger(const char* value){
    char* reg = malloc(128 * sizeof(char));
    strcpy(reg,value);
    return reg;
}
#endif