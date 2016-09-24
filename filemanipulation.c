#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "strmanipulation.h"

char relativeSPDFilePath[TAM_STRING] = "examples/small.spd";

Segments G_S;
Frames G_F;

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

double getDoubleFromLine(char* line, int blankSpaces) {
    char* value;
    double retValue;

    //get value in line
    value = getNextValue(line, blankSpaces);

    //parse
    retValue = atof(value);

    return retValue;
}

void loadNumberOfFrames(FILE* fp) {
    char line[TAM_LINE];

    //read line
    fgets(line, TAM_LINE -1, fp);

    //parse to int
    G_F.numberOfFrames = getIntFromLine(&line, 2);

    //alloc memory space to handle it
    G_F.frames = malloc(G_F.numberOfFrames * sizeof(Frame*));

    printf("frames: %d\n", G_F.numberOfFrames);
}

void loadNumberOfSegments(FILE* fp) {
    char line[TAM_LINE];

    //read line
    fgets(line, TAM_LINE -1, fp);

    //parse to int
    G_S.numberOfSegments = getIntFromLine(&line, 1);

    //alloc memory space to handle it
    G_S.segments = malloc(G_S.numberOfSegments * sizeof(Segmento*));

    printf("segments: %d\n", G_S.numberOfSegments);
}

void loadMarkersOfFrame(Frame* frame, char* line) {
    int initialPosition = 5;

    for (int i = 0; i < frame->numberOfMarkers; i++) {
        Marker* marker = malloc(sizeof(Marker));

        marker->x = getDoubleFromLine(line, initialPosition++);
        marker->y = getDoubleFromLine(line, initialPosition++);
        marker->z = getDoubleFromLine(line, initialPosition++);

        //jump another one
        initialPosition++;

        //attach into the matrix
        frame->markers[i] = marker;

        printf("pos %d: %p, x: %.10f, y: %.10f, z: %.10f\n", i, marker, marker->x, marker->y, marker->z);
    }
}

void loadFrames(FILE* fp) {
    char line[TAM_LINE];

    for (int i = 0; i < G_F.numberOfFrames; i++) {
        Frame* frame = malloc(sizeof(Frame));

        //read line
        fgets(line, TAM_LINE -1, fp);

        //get values
        frame->time = getDoubleFromLine(&line, 2);
        frame->numberOfMarkers = getIntFromLine(&line, 4);
        frame->markers = malloc(sizeof(Marker*) * frame->numberOfMarkers);

        //attach into the matrix
        G_F.frames[i] = frame;

        printf("position %d: %p, time: %.10f, numberOfMarkers: %d\n", i, frame, frame->time, frame->numberOfMarkers);

        //get markers
        loadMarkersOfFrame(frame, line);
    }
}

void loadSegments(FILE* fp) {
    char line[TAM_LINE];

    for (int i = 0; i < G_S.numberOfSegments; i++) {
        Segmento* segment = malloc(sizeof(Segmento));

        //read line
        fgets(line, TAM_LINE -1, fp);

        //get values
        segment->verticeInicial = getIntFromLine(&line, 1);
        segment->verticeFinal = getIntFromLine(&line, 2);

        //insert the memory position in the pointer
        G_S.segments[i] = segment;

        printf("position %d: %p, inicio: %d, fim: %d\n", i, segment, segment->verticeInicial, segment->verticeFinal);
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
    loadFrames(fp);

    //get number of segments
    loadNumberOfSegments(fp);

    //load segments
    loadSegments(fp);

    fclose(fp);
}

Segments* getSegments() {
    return &G_S;
}

Frames* getFrames() {
    return &G_F;
}
