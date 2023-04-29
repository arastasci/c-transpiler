#ifndef PARSE_H
#define PARSE_H
#include "tokenizer.h"
#include "hash.h"
void parseStatement();
int parseExpression();
int parseTerm();
int parseFactor();
int parseBitwiseOrExpression();
int parseBitwiseAndExpression();

extern int current_reg_id;
typedef struct reg {
    reg_id;
    reg_name;
    value;
} reg;


#endif
