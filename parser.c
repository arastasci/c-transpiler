#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include "tokenizer.h"
#include "file.h"

const char* printFormat = "call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s ) \n";
//char* makeBitshiftOperation(char* operand, char* shift_amount, const char* format){
//    // write result multiplied by factor2
//    // then result = the new reg
//    // <result> = <operation> <ty> <op1>, <op2>
//    char* shifted_temp_result = createRegDefault();
//    fprintf(output_file, format, shifted_temp_result, operand, shift_amount);
////    free(operand);
////    free(shift_amount);
//    free(operand);
//    free(shift_amount);
//    return shifted_temp_result;
//}

char* makeLeftRotateOperation(char* operand, char* rotate_amount){
    const char* format =     "%s = sub i32 32, %s\n"
                             "%s = lshr i32 %s, %s\n"
                             "%s = shl i32 %s, %s\n"
                             "  %s = or i32 %s, %s\n";
    char* temp_rotate1 = createRegDefault();
    char* temp_rotate2 = createRegDefault();
    char* temp_sub = createRegDefault();
    char* rotated_temp_result = createRegDefault();
    fprintf(output_file, format, 
    temp_sub, rotate_amount,
    temp_rotate1, operand, temp_sub ,
    temp_rotate2, operand, rotate_amount ,
    rotated_temp_result, temp_rotate1, temp_rotate2);
//    free(operand);
//    free(rotate_amount);

    // %temp_sub = sub i32 32, %val2
    // %rotate1 = lshr i32 %val1, temp_sub 
    // %rotate2 = shl i32 %val1, %val2
    // %rotated = or i32 %rotate1, %rotate2
    free(operand);
    free(rotate_amount);
    free(temp_rotate1);
    free(temp_rotate2);
    free(temp_sub);
    return rotated_temp_result;
}
char* makeRightRotateOperation(char* operand, char* rotate_amount){
    const char* format =  "%s = lshr i32 %s, %s\n"
                          "%s = sub i32 32, %s\n"
                          "%s = shl i32 %s, %s\n"
                          "%s = or i32 %s, %s\n";
    char* temp_rotate1 = createRegDefault();
    char* temp_rotate2 = createRegDefault();
    char* temp_sub = createRegDefault();
    char* rotated_temp_result = createRegDefault();
    fprintf(output_file, format, 
    temp_rotate1, operand, rotate_amount ,
    temp_sub, rotate_amount,
    temp_rotate2, operand, temp_sub ,
    rotated_temp_result, temp_rotate1, temp_rotate2);
    free(operand);
    free(rotate_amount);
    free(temp_rotate1);
    free(temp_rotate2);
    free(temp_sub);

    // %rotate1 = lshr i32 %val1, %val2
    // %temp_sub i32 32, %val2
    // %rotate2 = shl i32 %val1, %temp_sub 
    // %rotated = or i32 %rotate1, %rotate2
    return rotated_temp_result;
}

char* xorFunction(char* operand1, char* operand2){ // xor function
    // char* new_reg = createRegDefault();
    // // TODO: xor operands and store it in new_reg - do this for all binary-unary funcs
    // fprintf(output_file, "%s = xor i32 %s, %s \n",new_reg, operand1, operand2);

    char* result = makeOperation(operand1, operand2, "%s = xor i32 %s, %s \n");
    return result;
}

char* lsFunction(char* operand, char* shift_amount){ // ls function
    char* result = makeOperation(operand, shift_amount, "%s = shl i32 %s, %s \n");
    return result;
}

char* rsFunction(char* operand, char* shift_amount){ // rs function
    char* result = makeOperation(operand, shift_amount, "%s = lshr i32 %s, %s \n");
    return result;
}

char* lrFunction(char* operand, char* rotate_amount){ // lr function

    char* result = makeLeftRotateOperation(
            operand, rotate_amount);
    // %temp_sub = sub i32 32, %val2
    // %rotate1 = lshr i32 %val1, temp_sub 
    // %rotate2 = shl i32 %val1, %val2
    // %rotated = or i32 %rotate1, %rotate2
    return result;
}

char* rrFunction(char* operand, char* rotate_amount){ // rr function

    char* result = makeRightRotateOperation(operand, rotate_amount);
    return result;
}

char* makeOperation(char* result, char* term, const char* format){

    // write result multiplied by factor2
    // then result = the new reg
    // <result> = <operation> <ty> <op1>, <op2>
    char* temp_result = createRegDefault();
    fprintf(output_file, format, temp_result, result, term);
    free(term);
    free(result);

    return temp_result;
}

char* parseBinaryFunction(const char* operand_symbol){ // binary functions
    matchToken(LEFT_PAREN);
    char* operand1 = parseBitwiseOrExpression();
    matchToken(SEPARATOR);
    char* operand2 = parseBitwiseOrExpression();
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
    // raise error
    raiseTokenError();
    return 0; // impossible
}

char* parseUnaryFunction(){ // not function
    matchToken(LEFT_PAREN);

    char* res = parseBitwiseOrExpression();
   // res = res ^ -1;
    // TODO: find not op and do that :D
    char* temp_res = createRegDefault();
    fprintf(output_file,"%s = xor i32 %s, -1 \n", temp_res,res);
    matchToken(RIGHT_PAREN);
    free(res);
    return temp_res;
}
char* parseFactor(){
    token t = current_token;
    // char* new_reg = createRegDefault();
    if(t.type == INTEGER){
        matchToken(INTEGER);
        char* int_reg = createRegInteger(t.symbol);
        return int_reg;
    }
    else if(t.type == IDENTIFIER){
        matchToken(IDENTIFIER);
        variable* var = find(t.symbol);

        if(var == NULL){
            raiseTokenError();
            return createRegDefault();
        }
        else{
            // load the var

            return loadVar(var->reg);
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
        char* res = parseBitwiseOrExpression();
        matchToken(RIGHT_PAREN);
        return res;
    }
    // raise error from tokenizer.c
    raiseTokenError();
    // printf("factor not parsed in line 99. token error\n");
    return createRegDefault();
}
char* parseTerm(){

    char* result = parseFactor();  // first factor to be multiplied
    while(current_token.type == OPERATOR_MULTIPLICATIVE){
        token t = current_token;
        matchToken(OPERATOR_MULTIPLICATIVE);
        if(strcmp(t.symbol, "*") == 0){
            result = makeOperation(result, parseFactor(),"%s = mul i32 %s, %s\n");
        }

        if(strcmp(t.symbol, "/") == 0){
            result = makeOperation(result, parseFactor(), "%s = sdiv i32 %s, %s\n");
        }
        if(strcmp(t.symbol, "%") == 0){
            // TODO: with the above syntax, make a modulo operation.
            result = makeOperation(result, parseFactor(), "%s = srem i32 %s, %s\n");
        }
    }
    return result;
}

char* parseExpression(){
    char* result = parseTerm(); // first term to be multiplied or divided
    while(token_index < token_count && current_token.type == OPERATOR_ADDITIVE){    // add or subtract (lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_ADDITIVE);
        if(strncmp(t.symbol,"+", 1) == 0){
            result = makeOperation(result, parseTerm(), "%s = add i32 %s, %s \n");
        }
        else{ // subtraction
            result = makeOperation(result, parseTerm(), "%s = sub i32 %s, %s \n");

        }
    }
    return result;
}
char* parseVariable(){  // returns the variable pointer
    token t = current_token;
    matchToken(IDENTIFIER);
    variable *var = find(t.symbol); // check if the variable is in the hashmap
    if(var == NULL){
        char* inserted_var = insert(t.symbol)->reg;
        fprintf(output_file,"%s = alloca i32 \n", inserted_var);
        return inserted_var; // if not, insert it and return the pointer

    }
    else{
        return var->reg; // if it is, return the pointer
    }
}

char* parseBitwiseAndExpression(){
    char* result = parseExpression();  // first expression to be bitwise 'and'ed
    while (token_index < token_count && strcmp(current_token.symbol, "&")==0){    // bitwise 'or' and 'and' (second lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_BITWISE);
        result = makeOperation(result, parseExpression(),"%s = and i32 %s, %s \n");
    }
    return result;
};
char* parseBitwiseOrExpression(){
    char* result = parseBitwiseAndExpression();  // first (bitwise and expression) to be bitwise 'or'ed

    while (token_index < token_count && strcmp(current_token.symbol, "|")==0){    // bitwise 'or' and 'and' (second lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_BITWISE);
        result = makeOperation(result, parseBitwiseAndExpression(), "%s = or i32 %s, %s \n");
    }
    return result;
};


void parseAssignment(){
    bool justInitialized = find(current_token.symbol) == NULL;
    char* var = parseVariable();
    matchToken(ASSIGNMENT);
    char* response = parseBitwiseOrExpression(); // get the value of the (bitwise or expression)
    if(!has_error && token_index == token_count){
        storeInVar(var, response);
    }
    else{
        if(justInitialized) var = ""; // delete variable from the array if there is an error
    }
    free(response);
}

void parseStatement(){  // two types of statements: assignment and bitwise or expression
    if(token_array[1].type == ASSIGNMENT){
        parseAssignment(); // assigns the rhs expr to lhs variable
    }
    else if(token_count != 0){
        char* result = parseBitwiseOrExpression(); // evaluates the (bitwise or expression)
        if(!has_error && token_index == token_count){ // if there is no error and all tokens are consumed
            // TODO: write the statement that calls printf from llvm
            fprintf(output_file, printFormat, result);
        }
//        else{
//            // printf("Error on line %d!\n",line_count); // if there is an error or there are unconsumed tokens
//        }
        free(result);
    }
}
