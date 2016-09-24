#include <stdio.h>
#include <stdlib.h>
#include "global.h"

char relativeSPDFilePath[TAM_STRING] = "examples/small.spd";

void checkFilePath() {
    if (isValidFile(relativeSPDFilePath) == 0) {
        printf("Arquivo não encontrado.\n");
        exit(1);
    }
}

int isValidFile(char file[]) {
    FILE* fp = fopen(file, "r");

    return (fp == NULL ? 0 : 1);
}

void loadFileAndInitializeVars() {
    FILE* fp = fopen(relativeSPDFilePath, "r");
    char line[TAM_LINE];

    //ignore first two lines
    fgets(line, TAM_LINE -1, fp);
    fgets(line, TAM_LINE -1, fp);

    //3rd line (START)
    fgets(line, TAM_LINE -1, fp);
    char *start;
    start = getNextValue(&line, 1);

    printf("start: %s\n", start);


    //4th line (END)

    fclose(fp);
}
