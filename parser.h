#ifndef PARSE_H
#define PARSE_H
#include "tokenizer.h"
#include "hash.h"
#include "reg.h"
void parseStatement();
reg* parseExpression();
reg* parseTerm();
reg* parseFactor();
reg* parseBitwiseOrExpression();
reg* parseBitwiseAndExpression();
reg* makeOperation(reg* result, reg* descendent_term, const char* format);



#endif
