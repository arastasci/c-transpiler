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

void write_end() {
    fprintf(output_file, "ret i32 0\n}");
}
