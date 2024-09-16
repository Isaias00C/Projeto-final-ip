#ifndef PHOTOSHOP_H
#define PHOTOSHOP_H

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>

typedef struct PGMimg {
    char type[3];
    char com[100];
    int width;
    int height;
    int maxVal;
    int** pixels;
} PGMimg;

typedef struct kernel {
    int** matrix;
    int totalWeight;
    int radius;
    int positions;
    int size;
} kernel;

typedef enum FilterType {
    MEDIAN = 1,
    GAUSS_3X3 = 2,
    GAUSS_5X5 = 3,
    GAUSS_7X7 = 4,
    SORBEL_X = 5,
    SORBEL_Y = 6,
    INVERTER = 7,
    ROTATE90 = 8,
    CONTRASTE = 9,
    SOBEL = 10
} FilterType;

int readFile(PGMimg* pgm, char* filename);
int writeFile(PGMimg* pgm, char* filename);
int transferData(PGMimg* in, PGMimg* out);
int rotateData(PGMimg* in, PGMimg* out);
int convolution(PGMimg* in, PGMimg* out, kernel* k);
int digits(int n);
void freeKernel(kernel *k);
void freeImage(PGMimg *img);
kernel* createKernel(int n, int aux[n][n], int totalWeight, int positions);
kernel* getKernel(FilterType type);
int inverterCor(PGMimg* in, PGMimg* out);
int rotate90(PGMimg* in,PGMimg* out);
void ignoreComments(FILE* fp);
void normalize(PGMimg* pgm);
void sobel(PGMimg* in, PGMimg* out);
int convolutionPixel(PGMimg* a, int row, int col, int kernel[3][3]);

#endif
