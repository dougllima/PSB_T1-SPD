#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

char* getNextValue(char* line, int spacesToIgnore) {
    char* ref = line;
    char* tok;
    char teste[TAM_LINE];

    do {
        //find the first occurence of blank space
        ref = strchr(ref, ' ');

        //in case we didn't find, break the loop
        if (ref == NULL) break;

        //move one memory position to avoid the blank space
        ref++;
        spacesToIgnore--;
    }
    while(spacesToIgnore > 0);

    //after here, we do know where the string should start

    //make a copy of the string
    strcpy(teste, ref);

    //get the entire string until we find either a blank space or a new line
    tok = strtok(teste, " ");
    tok = strtok(teste, "\n");

    return tok;
}
