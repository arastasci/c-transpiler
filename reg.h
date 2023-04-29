//
// Created by Aras on 4/29/2023.
//

#ifndef CMPE230P2_REG_H
#define CMPE230P2_REG_H
extern int current_reg_id;
typedef struct r {
    int id;
    const char* name;
} reg;
reg* createReg(const char*);
reg* createRegDefault();
#endif //CMPE230P2_REG_H
