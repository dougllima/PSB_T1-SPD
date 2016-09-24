#ifndef TAM_STRING

#define TAM_STRING 255
#define TAM_LINE 10000
#define TAM_MAX 20

//Single Structures
typedef struct {
    int verticeInicial, verticeFinal;
} Segmento;

typedef struct {
    double x, y, z;
} Marker;

typedef struct  {
    double time;
    int numberOfMarkers;
    Marker** markers;
} Frame;

//Matrix Structures
typedef struct {
    int numberOfFrames;
    Frame** frames;
} Frames;

typedef struct {
    int numberOfSegments;
    Segmento** segments;
} Segments;

#endif

