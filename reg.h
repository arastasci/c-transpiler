//
// Created by Aras on 4/29/2023.
//

#ifndef CMPE230P2_REG_H
#define CMPE230P2_REG_H
#include "file.h"
extern int current_reg_id;
typedef struct r {
    int id;
    char* name;
} reg;
reg* createReg(const char*);
reg* createRegDefault();
reg* createEmptyVarReg();
reg* createRegInteger(const char*);
void storeInVar(reg* var, reg* rhs);
reg* loadVar(reg* var);
#endif //CMPE230P2_REG_H
