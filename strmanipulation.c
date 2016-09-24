#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"

char* getNextValue(char* line, int spacesToIgnore) {
    int i = 0;

    do {
        //find the first occurence of blank space
        line = strchr(line, ' ');

        //in case we didn't find, break the loop
        if (line == NULL) break;

        //move one memory position to avoid the blank space
        line += 0x1;
        i++;
    }
    while(i < spacesToIgnore);

    printf("line: %s\n", line);

    return line;
}
