#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

//Struct da Imagem
typedef struct PGMimg {
    char type[3];
    char com[100];
    int width;
    int height;
    int maxVal;
    int** pixels;
} PGMimg;

int openFile(PGMimg* pgm, char* filename);

void teste(PGMimg* pgm){
    char name[100];
    scanf("%s", name);
    openFile(pgm, name);

    int i, j;
    for (i = 0; i < pgm->height; i++){
        for (j = 0; j < pgm->width; j++){
            printf(" %d", pgm->pixels[i][j]);
        }
        printf("\n");
    }
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    PGMimg* pgm = (PGMimg*)malloc(sizeof(PGMimg));
    teste(pgm);

    return 0;
}

int openFile(PGMimg* pgm, char* filename){

    //Abrindo o arquivo
    FILE *imgFile;
    imgFile = fopen(filename, "r");

    //Conferindo se o arquivo existe
    if (imgFile == NULL){
        printf("Arquivo não encontrado!\n");
        return 0;
    }
    
    //Lendo o tipo
    fscanf(imgFile, "%s", pgm->type);

    //Se não for um plain PMG, recusa o arquivo
    if (strcmp(pgm->type, "P2")){
        printf("Arquivo inválido!\n");
        return 0;
    }

    //Lê o comentário, as dimensões e o Maxval do arquivo
    fscanf(imgFile, " %80[^\n]s", pgm->com);
    fscanf(imgFile, "%d %d", &(pgm->width), &(pgm->height));
    fscanf(imgFile, "%d", &(pgm->maxVal));

    //Checa se as dimensões do arquivo são válidas
    if (pgm->height <= 0 || pgm->width <= 0){
        printf("Arquivo inválido!\n");
        return 0;
    }

    //Aloca memória para a imagem
    pgm->pixels = (int**)malloc(pgm->height * sizeof(int*));
    if (pgm->pixels == NULL) {
        printf("Malloc falhou1\n");
        return 0;
    }
    for (int i = 0; i < pgm->height; i++){
        pgm->pixels[i] = (int*)malloc(pgm->width * sizeof(int));
        if (pgm->pixels[i] == NULL) {
            printf("Malloc falhou2\n");
            return 0;
        }
        for (int j = 0; j < pgm->width; j++){
            fscanf(imgFile, "%d", &pgm->pixels[i][j]);
        }
    }

    //Fechando o arquivo
    fclose(imgFile);

    return 1;
}