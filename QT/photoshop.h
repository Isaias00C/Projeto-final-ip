#ifndef PHOTOSHOP_H
#define PHOTOSHOP_H

using namespace std;

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include <algorithm>
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
    GAUSS = 2,
    INVERTER = 3,
    ROTATE90 = 4,
    CONTRASTE = 5,
    SOBEL_X = 6,
    SOBEL_Y = 7,
} FilterType;

int photoshop(const char* filename, int op);
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
