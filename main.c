#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tokenizer.h"
#include "parser.h"
#include "hash.h"
#include "file.h"
int main(int argc, char *argv[]){

    if(argc != 2){
        printf("File not supplied\n");
        return -1;
    }
    char* input_file_name = argv[1];
    char no_extension_name [50];
    strcpy(no_extension_name, input_file_name);
    strtok(no_extension_name,".");
    char output_file_name [50];
    sprintf(output_file_name,"%s.ll", no_extension_name);
    input_file = fopen(input_file_name,"r");
    output_file = fopen(output_file_name, "w+");

    char input [258];
    initializeHashMap();
    allocateArrayMemory(); // allocate token array mem

    current_reg_id = 0;
    while(fgets(input, 258, input_file) != NULL){
        has_error = false;
        printf("%s", input);
        tokenize(input);
        // printTokens(); // for debug purposes
//        if(!has_error){
//            parseStatement();
//        }
//        else{
//            printf("Error!\n");
//        }
        freeArrayMemory(); // free token array mem
    }
    deallocHashMap();
    return 0;
}
