//
// Created by Aras on 4/30/2023.
//
#include "file.h"

FILE* output_file;
FILE* input_file;

int line_count;

void write_beginning(){
    fprintf(output_file,"; ModuleID = 'advcalc2ir'\n"
                        " declare i32 @printf(i8*, ...)\n"
                        " @print.str = constant [4 x i8] c\"%%d\\0A\\00\"\n"
                        "\n"
                        " define i32 @main() {\n");
}

void write_functions(){
    fprintf(output_file,"define i32* @lsFunction(i32* %operand, i32* %shift_amount) {\n"
                        "    %new_reg = alloca i32\n"
                        "    %val1 = load i32, i32* %operand\n"
                        "    %val2 = load i32, i32* %shift_amount\n"
                        "    %shifted = shl i32 %val1, %val2\n"
                        "    store i32 %shifted, i32* %new_reg\n"
                        "    ret i32* %new_reg\n"
                        "}\n"
                        "define i32* @rsFunction(i32* %operand, i32* %shift_amount) {\n"
                        "    %new_reg = alloca i32\n"
                        "    %val1 = load i32, i32* %operand\n"
                        "    %val2 = load i32, i32* %shift_amount\n"
                        "    %shifted = lshr i32 %val1, %val2\n"
                        "    store i32 %shifted, i32* %new_reg\n"
                        "    ret i32* %new_reg\n"
                        "}define i32* @lrFunction(i32* %operand, i32* %rotate_amount) {\n"
                        "    %new_reg = alloca i32\n"
                        "    %val1 = load i32, i32* %operand\n"
                        "    %val2 = load i32, i32* %rotate_amount\n"
                        "    %rotate1 = lshr i32 %val1, sub i32 32, %val2\n"
                        "    %rotate2 = shl i32 %val1, %val2\n"
                        "    %rotated = or i32 %rotate1, %rotate2\n"
                        "    store i32 %rotated, i32* %new_reg\n"
                        "    ret i32* %new_reg\n"
                        "}\n"
                        "define i32* @rrFunction(i32* %operand, i32* %rotate_amount) {\n"
                        "    %new_reg = alloca i32\n"
                        "    %val1 = load i32, i32* %operand\n"
                        "    %val2 = load i32, i32* %rotate_amount\n"
                        "    %rotate1 = lshr i32 %val1, %val2\n"
                        "    %rotate2 = shl i32 %val1, sub i32 32, %val2\n"
                        "    %rotated = or i32 %rotate1, %rotate2\n"
                        "    store i32 %rotated, i32* %new_reg\n"
                        "    ret i32* %new_reg\n"
                        "}\n"
                        );
}

void write_end() {
    fprintf(output_file, "ret i32 0\n}");
}
