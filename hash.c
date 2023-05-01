#include "hash.h"

variable* variables;
void initializeHashMap(){   // initialize the hashmap and assign all the names to "" and values to 0
    variables = malloc(128 * (sizeof(variable)));
    for(int i = 0; i < 128; i++){
        strcpy(variables[i].name, "");
        strcpy(variables[i].reg, createEmptyVarReg());
    }
}
int hash(const char* identifier){ // djb2
    unsigned long hash = 5381;
    int c;

    while (c = *identifier++){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % 128;

}

variable* insert(const char* identifier){   // insert a variable into the hashmap
    int firstHashVal = hash(identifier);
    for(int i = 0; i < 128; i++){   // linear probing
        if(strcmp(variables[(firstHashVal + i) % 128].name, "" ) == 0){
            variable* var = &variables[(firstHashVal + i) % 128];
            char* buf = malloc(sizeof(char) *128);
            sprintf(buf,"%%%s",identifier);
            strcpy(var->name, identifier);
            strcpy(var->reg, buf);
            return var;
        }
    }
    return NULL; // impossible
}
variable* find(const char* identifier){
    int hashVal = hash(identifier);
    for(int i = 0; i < 128; i++){   // find with linear probing
        if(strcmp(variables[(hashVal + i) % 128].name, identifier) == 0)
            return &variables[(hashVal + i) % 128];
    }
    return NULL;
}

void deallocHashMap(){  // free the hashmap
    free(variables);
}
