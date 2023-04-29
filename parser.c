#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include "tokenizer.h"


reg* xorFunction(reg* operand1, reg* operand2){ // xor function
    reg* new_reg = createRegDefault();
    // TODO: xor operands and store it in new_reg - do this for all binary-unary funcs
    return new_reg;
}
reg* lsFunction(reg* operand, reg* shift_amount){ // ls function
    return operand << shift_amount;
}
reg* rsFunction(reg* operand, reg* shift_amount){ // rs function
    return operand >> shift_amount;
}
reg* lrFunction(reg* operand, reg* rotate_amount){ // lr function
    return (operand << rotate_amount)|(operand >> (64 - rotate_amount));
}
reg* rrFunction(reg* operand, reg* rotate_amount){ // rr function
    return (operand >> rotate_amount)|(operand << (64 - rotate_amount));
}

reg* parseBinaryFunction(const char* operand_symbol){ // binary functions
    reg* result;
    matchToken(LEFT_PAREN);
    reg* operand1 = parseBitwiseOrExpression();
    matchToken(SEPARATOR);
    reg* operand2 = parseBitwiseOrExpression();
    matchToken(RIGHT_PAREN);

    if(strncmp(operand_symbol,"xor",3) == 0){
         return xorFunction(operand1, operand2);

    }
    if(strncmp(operand_symbol,"ls",2) == 0){
         return lsFunction(operand1, operand2);
    }
    if(strncmp(operand_symbol,"rs", 2) == 0){
        return rsFunction(operand1, operand2);
    }
    if(strncmp(operand_symbol,"lr", 2) == 0){
        return lrFunction(operand1, operand2);
    }
    if (strncmp(operand_symbol,"rr", 2) == 0){
        return rrFunction(operand1, operand2);
    }
    // raise error // impossible
    return 0; // impossible

}

reg* parseUnaryFunction(){ // not function
    matchToken(LEFT_PAREN);

    reg* res = parseBitwiseOrExpression();
    res = res ^ -1;
    matchToken(RIGHT_PAREN);
    return res;
}
reg* parseFactor(){
    token t = current_token;
    reg* new_reg = createRegDefault();
    if(t.type == INTEGER){
        matchToken(INTEGER);
        return atoll(t.symbol);
    }
    else if(t.type == IDENTIFIER){
        matchToken(IDENTIFIER);
        variable* var = find(t.symbol);

        if(var == NULL){
            raiseTokenError();
            return NULL;
        }
        else{
            return var->var_reg;
        }
    }
    else if(t.type == STR_OPERATOR_BINARY){
        const char* symbol = t.symbol;
        matchToken(STR_OPERATOR_BINARY);
        return parseBinaryFunction(symbol);
    }
    else if(t.type == STR_OPERATOR_UNARY){
        matchToken(STR_OPERATOR_UNARY);
        return parseUnaryFunction(); // it's only not(<var>) though :D 
    }
    else if (t.type == LEFT_PAREN){
        matchToken(LEFT_PAREN);
        reg* res = parseBitwiseOrExpression();
        matchToken(RIGHT_PAREN);
        return res;
    }
    // raise error from tokenizer.c
    raiseTokenError();
    return 1; 
}
reg* parseTerm(){

    reg* result = parseFactor();  // first factor to be multiplied
    while(current_token.type == OPERATOR_MULTIPLICATIVE){
        token t = current_token;
        matchToken(OPERATOR_MULTIPLICATIVE);
        if(strcmp(t.symbol, "*") == 0)
            result *= parseFactor();    // get second factor to be multiplied with the first factor
        if(strcmp(t.symbol, "/") == 0)
            result /= parseFactor();    // divide res by parseFactor's return value
        if(strcmp(t.symbol, "%") == 0)
            result %= parseFactor();    // get second factor to be modulo'ed with the first factor
    }
    return result;
}

reg* parseExpression(){
    reg* result = parseTerm(); // first term to be multiplied or divided
    while(token_index < token_count && current_token.type == OPERATOR_ADDITIVE){    // add or subtract (lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_ADDITIVE);
        if(strncmp(t.symbol,"+", 1) == 0){
            result += parseTerm() ; // get second term to be added to the first term
        }
        else{
            result -= parseTerm();  // get second term to be subtracted from the first term
        }
    }
    return result;
}
reg* parseVariable(){  // returns the variable pointer
    token t = current_token;
    matchToken(IDENTIFIER);
    variable *var = find(t.symbol); // check if the variable is in the hashmap
    if(var == NULL){
        return insert(t.symbol)->var_reg; // if not, insert it and return the pointer

    }
    else{
        return var->var_reg; // if it is, return the pointer
    }
}

reg* parseBitwiseAndExpression(){
    reg* result = parseExpression();  // first expression to be bitwise 'and'ed
    while (token_index < token_count && strcmp(current_token.symbol, "&")==0){    // bitwise 'or' and 'and' (second lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_BITWISE);
        result &= parseExpression();  // get second expression to be bitwise 'and'ed with the first expression
    }
    return result;
};
reg* parseBitwiseOrExpression(){
    reg* result = parseBitwiseAndExpression();  // first (bitwise and expression) to be bitwise 'or'ed

    while (token_index < token_count && strcmp(current_token.symbol, "|")==0){    // bitwise 'or' and 'and' (second lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_BITWISE);
        result |= parseBitwiseAndExpression();  // get second (bitwise and expression) to be bitwise 'or'ed with the first (bitwise and expression)
    }
    return result;
};


void parseAssignment(){
    bool justInitialized = find(current_token.symbol) == NULL;
    reg* var = parseVariable();
    matchToken(ASSIGNMENT);
    reg* response = parseBitwiseOrExpression();   // get the value of the (bitwise or expression)
    if(!has_error && token_index == token_count){
        storeInVar(var, response);
    }
    else{
        if(justInitialized) var->name = ""; // delete variable from the array if there is an error
        printf("Error!\n");
    }
}

void parseStatement(){  // two types of statements: assignment and bitwise or expression
    if(token_array[1].type == ASSIGNMENT){
        parseAssignment(); // assigns the rhs expr to lhs variable
    }
    else if(token_count != 0){
        reg* result = parseBitwiseOrExpression(); // evaluates the (bitwise or expression)
        if(!has_error && token_index == token_count){ // if there is no error and all tokens are consumed
            printf("%d\n", result);
        }
        else{
            printf("Error!\n"); // if there is an error or there are unconsumed tokens
        }
    }
}
