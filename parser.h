#ifndef PARSE_H
#define PARSE_H
#include "tokenizer.h"
#include "hash.h"
#include "reg.h"
void parseStatement();
char* parseExpression();
char* parseTerm();
char* parseFactor();
char* parseBitwiseOrExpression();
char* parseBitwiseAndExpression();
char* makeOperation(char* result, char* descendent_term, const char* format);



#endif
