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
    char no_extension_name [990];
    strcpy(no_extension_name, input_file_name);
    strtok(no_extension_name,".");
    char output_file_name [1000];
    sprintf(output_file_name,"%s.ll", no_extension_name);
    input_file = fopen(input_file_name,"r");
    output_file = fopen(output_file_name, "w+");

    char input [258];

    current_reg_id = 0;
    initializeHashMap();

    line_count = 1;

    write_beginning();

    has_error = false;
    bool has_any_error = false;
    while(fgets(input, 258, input_file) != NULL){
        // printf("%s", input);
        has_error = false;
        tokenize(input);
        //printTokens(); // for debug purposes
        parseStatement();

        if(has_error){
            printf("Error in line %d!\n", line_count);
            has_any_error = true;
        }

        resetTokenArray(); // free token array mem
        line_count++;
    }
    write_end();

    if(has_any_error){
        remove(output_file_name);
    }
    else{
        fclose(output_file);
    }
    fclose(input_file);
    deallocHashMap();
    return 0;
}
