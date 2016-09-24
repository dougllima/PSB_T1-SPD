#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "strmanipulation.h"

char relativeSPDFilePath[TAM_STRING] = "examples/small.spd";
Segmento** segments;
int numberOfFrames;
int numberOfSegments;

int isValidFile(char file[]) {
    FILE* fp = fopen(file, "r");

    return (fp == NULL ? 0 : 1);
}

void checkFilePath() {
    if (isValidFile(relativeSPDFilePath) == 0) {
        printf("Arquivo não encontrado.\n");
        exit(1);
    }
}

int getIntFromLine(char* line, int blankSpaces) {
    char* value;
    int retValue;

    //get value in line
    value = getNextValue(line, blankSpaces);

    //parse
    retValue = atoi(value);

    return retValue;

}

void loadNumberOfFrames(FILE* fp) {
    char line[TAM_LINE];

    //read line
    fgets(line, TAM_LINE -1, fp);

    //parse to int
    numberOfFrames = getIntFromLine(&line, 2);

    printf("frames: %d\n", numberOfFrames);
}

void loadNumberOfSegments(FILE* fp) {
    char line[TAM_LINE];

    //read line
    fgets(line, TAM_LINE -1, fp);

    //parse to int
    numberOfSegments = getIntFromLine(&line, 1);

    //alloc memory space to handle it
    segments = malloc(numberOfSegments * sizeof(Segmento));

    printf("segments: %d\n", numberOfSegments);
}

void loadSegments(FILE* fp) {
    char line[TAM_LINE];

    for (int i = 0; i < numberOfSegments; i++) {
        Segmento* segment = malloc(sizeof(Segmento));

        //read line
        fgets(line, TAM_LINE -1, fp);

        //get values
        segment->verticeInicial = getIntFromLine(&line, 1);
        segment->verticeFinal = getIntFromLine(&line, 2);

        //insert the memory position in the pointer
        segments[i] = segment;

        printf("position %d: %p, inicio: %d, fim: %d\n", i, segments[i], segments[i]->verticeInicial, segments[i]->verticeFinal);
    }
}

void loadFileAndInitializeVars() {
    FILE* fp = fopen(relativeSPDFilePath, "r");
    char line[TAM_LINE];

    //ignore first five lines
    for (int i = 0; i < 5; i++)
        fgets(line, TAM_LINE -1, fp);

    //get number of frames
    loadNumberOfFrames(fp);

    //load frames
    for (int i = 0; i < numberOfFrames; i++)
        fgets(line, TAM_LINE -1, fp);

    //get number of segments
    loadNumberOfSegments(fp);

    //load segments
    loadSegments(fp);

    fclose(fp);
}
