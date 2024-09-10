#ifndef PHOTOSHOP_H
#define PHOTOSHOP_H

using namespace std;

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <vector>
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
    CONTRASTE = 9
} FilterType;

int photoshop(const char* filename);
int readFile(PGMimg* pgm, const char* filename);
int writeFile(PGMimg* pgm, char* filename);
int transferData(PGMimg* in, PGMimg* out);
int rotateData(PGMimg* in, PGMimg* out);
int convolution(PGMimg* in, PGMimg* out, kernel* k);
int digits(int n);
void freeKernel(kernel *k);
void freeImage(PGMimg *img);
kernel* createKernel(int n, vector<vector<int>> aux, int totalWeight, int positions);
kernel* getKernel(int type);
void inverterCor(PGMimg* in, PGMimg* out);
void rotate90(PGMimg* in,PGMimg* out);
void ignoreComments(FILE* fp);
void normalize(PGMimg* pgm);

#endif
