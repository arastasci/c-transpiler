//
// Created by Aras on 4/29/2023.
//

#ifndef CMPE230P2_REG_H
#define CMPE230P2_REG_H
#include "file.h"
extern int current_reg_id;

char* createReg(const char*);
char* createRegDefault();
char* createEmptyVarReg();
char* createRegInteger(const char*);
void storeInVar(char* var, char* rhs);
char* loadVar(char* var);
#endif //CMPE230P2_REG_H
