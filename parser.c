#include <stdio.h>
#include <stdbool.h>
#include "parser.h"
#include "tokenizer.h"
#include "file.h"

const char* printFormat = "call i32 (i8*, ...) @printf(i8* getelementptr ([4 x i8], [4 x i8]* @print.str, i32 0, i32 0), i32 %s )";
reg* makeBitshiftOperation(reg* operand, reg* shift_amount, const char* format){
    // write result multiplied by factor2
    // then result = the new reg
    // <result> = <operation> <ty> <op1>, <op2>
    reg* shifted_temp_result = createRegDefault();
    fprintf(output_file, format, shifted_temp_result->name, operand->name, shift_amount->name);
    free(operand);
    free(shift_amount);
    return shifted_temp_result;
}

reg* makeBitRotateOperation(reg* operand, reg* rotate_amount, reg* temp_rotate1, reg* temp_rotate2, const char* format){
    reg* rotated_temp_result = createRegDefault();
    fprintf(output_file, format, temp_rotate1->name, operand->name, rotate_amount->name , temp_rotate2->name,
            operand->name, rotate_amount->name , rotated_temp_result->name, temp_rotate1->name, temp_rotate2->name);
    free(operand);
    free(rotate_amount);
    return rotated_temp_result;
}

reg* xorFunction(reg* operand1, reg* operand2){ // xor function
    // reg* new_reg = createRegDefault();
    // // TODO: xor operands and store it in new_reg - do this for all binary-unary funcs
    // fprintf(output_file, "%s = xor i32 %s, %s \n",new_reg->name, operand1->name, operand2->name);

    reg* result = makeBitshiftOperation(operand1, operand2, "%s = xor i32 %s, %s \n");
    return result;
}

reg* lsFunction(reg* operand, reg* shift_amount){ // ls function
    reg* result = makeBitshiftOperation(operand, shift_amount, "%s = shl i32 %s, %s \n");
    return result;
}

reg* rsFunction(reg* operand, reg* shift_amount){ // rs function
    reg* result;
    result = makeBitshiftOperation(operand, shift_amount, "%s = lshr i32 %s, %s \n");
    return result;
}

reg* lrFunction(reg* operand, reg* rotate_amount){ // lr function
    reg* temp_rotate1 = createRegDefault();
    reg* temp_rotate2 = createRegDefault();
    reg* result = makeBitRotateOperation(
            operand, rotate_amount, temp_rotate1, temp_rotate2,
    "%s = lshr i32 %s, sub i32 32, %s\n"
    "%s = shl i32 %s, %s\n"
    "  %s = or i32 %s, %s\n"
                             );
    // %rotate1 = lshr i32 %val1, sub i32 32, %val2
    // %rotate2 = shl i32 %val1, %val2
    // %rotated = or i32 %rotate1, %rotate2
    return result;
}

reg* rrFunction(reg* operand, reg* rotate_amount){ // rr function
    reg* temp_rotate1 = createRegDefault();
    reg* temp_rotate2 = createRegDefault();
    reg* result = makeBitRotateOperation(
            operand, rotate_amount,  temp_rotate1, temp_rotate2,
            "%s = lshr i32 %s, %s\n"
            "%s = shl i32 %s, sub i32 32, %s\n"
            "%s = or i32 %s, %s\n"
            );
    // %rotate1 = lshr i32 %val1, %val2
    // %rotate2 = shl i32 %val1, sub i32 32, %val2
    // %rotated = or i32 %rotate1, %rotate2
    return result;
}

reg* makeOperation(reg* result, reg* term, const char* format){

    // write result multiplied by factor2
    // then result = the new reg
    // <result> = <operation> <ty> <op1>, <op2>
    reg* temp_result = createRegDefault();
    fprintf(output_file, format, temp_result->name, result->name, term->name);
//    free(result);
//    free(term);
// TODO: free the memory goddzayyummit...
    return temp_result;
}

reg* parseBinaryFunction(const char* operand_symbol){ // binary functions
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
    // raise error
    return 0; // impossible
}

reg* parseUnaryFunction(){ // not function
    matchToken(LEFT_PAREN);

    reg* res = parseBitwiseOrExpression();
   // res = res ^ -1;
    // TODO: find not op and do that :D
    reg* temp_res = createRegDefault();
    fprintf(output_file,"%s = xor i32 %s, -1 \n", temp_res->name,res->name);
    matchToken(RIGHT_PAREN);
    return temp_res;
}
reg* parseFactor(){
    token t = current_token;
    // reg* new_reg = createRegDefault();
    if(t.type == INTEGER){
        matchToken(INTEGER);
        reg* int_reg = createRegInteger(t.symbol);
        return int_reg;
    }
    else if(t.type == IDENTIFIER){
        matchToken(IDENTIFIER);
        variable* var = find(t.symbol);

        if(var == NULL){
            raiseTokenError();
            return NULL;
        }
        else{
            // load the var

            return loadVar(&var->var_reg);
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
    printf("factor not parsed in line 99. token error\n");
    return NULL;
}
reg* parseTerm(){

    reg* result = parseFactor();  // first factor to be multiplied
    while(current_token.type == OPERATOR_MULTIPLICATIVE){
        token t = current_token;
        matchToken(OPERATOR_MULTIPLICATIVE);
        if(strcmp(t.symbol, "*") == 0){
            result = makeOperation(result, parseFactor(),"%s = mul i32 %s, %s \n");
        }

        if(strcmp(t.symbol, "/") == 0){
            result = makeOperation(result, parseFactor(), "%s = sdiv i32 %s, %s\n");
        }
        if(strcmp(t.symbol, "%") == 0){
            // TODO: with the above syntax, make a modulo operation.
        }
    }
    return result;
}

reg* parseExpression(){
    reg* result = parseTerm(); // first term to be multiplied or divided
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
reg* parseVariable(){  // returns the variable pointer
    token t = current_token;
    matchToken(IDENTIFIER);
    variable *var = find(t.symbol); // check if the variable is in the hashmap
    if(var == NULL){
        reg* inserted_var = &insert(t.symbol)->var_reg;
        fprintf(output_file,"%s = alloca i32 \n", inserted_var->name);
        return inserted_var; // if not, insert it and return the pointer

    }
    else{
        return &var->var_reg; // if it is, return the pointer
    }
}

reg* parseBitwiseAndExpression(){
    reg* result = parseExpression();  // first expression to be bitwise 'and'ed
    while (token_index < token_count && strcmp(current_token.symbol, "&")==0){    // bitwise 'or' and 'and' (second lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_BITWISE);
        makeOperation(result, parseExpression(),"%s = and i32 %s, %s \n");
    }
    return result;
};
reg* parseBitwiseOrExpression(){
    reg* result = parseBitwiseAndExpression();  // first (bitwise and expression) to be bitwise 'or'ed

    while (token_index < token_count && strcmp(current_token.symbol, "|")==0){    // bitwise 'or' and 'and' (second lowest precedency in the grammar)
        token t = current_token;
        matchToken(OPERATOR_BITWISE);
        makeOperation(result, parseBitwiseAndExpression(), "%s = or i32 %s, %s \n");
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
        // TODO: delete the .ll file
    }
}

void parseStatement(){  // two types of statements: assignment and bitwise or expression
    if(token_array[1].type == ASSIGNMENT){
        parseAssignment(); // assigns the rhs expr to lhs variable
    }
    else if(token_count != 0){
        reg* result = parseBitwiseOrExpression(); // evaluates the (bitwise or expression)
        if(!has_error && token_index == token_count){ // if there is no error and all tokens are consumed
            // TODO: write the statement that calls printf from llvm
            fprintf(output_file, printFormat, result->name);
        }
        else{
            printf("Error!\n"); // if there is an error or there are unconsumed tokens
        }
    }
}
