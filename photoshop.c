#include "photoshop.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
//Função temporária pra testes
void teste(){
    
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    int i, j;
    
    PGMimg* a = (PGMimg*)malloc(sizeof(PGMimg));
    char name[100];
    printf("Qual o nome do input?\n");
    scanf("%s", name);
    if (readFile(a, name) == 1) {
        PGMimg* b = (PGMimg*)malloc(sizeof(PGMimg));
        transferData(a, b);
        printf("Qual o nome do output?\n");
        char outName[100];
        scanf("%s", outName);
        int op;
        printf("Qual o filtro?\n");
        printf("%d) Mediana\n", MEDIAN);
        printf("%d) Gauss Low\n", GAUSS_3X3);
        printf("%d) Gauss Mid\n", GAUSS_5X5);
        printf("%d) Gauss High\n", GAUSS_7X7);
        printf("%d) Sorbel Horizontal\n", SORBEL_X);
        printf("%d) Sorbel Vertical\n", SORBEL_Y);
        printf("%d) Cor Invertida\n",INVERTER);
        printf(">>>>>>>> ");
        scanf("%d", &op);
        if (op >= 1 && op <= 6) {
            kernel *k = getKernel(op);
            if (k == NULL) {
                printf("Erro ao obter filtro/kernel\n");
            } else {
                transferData(a, b);
                if (b == NULL) {
                    freeImage(a);
                    printf("Erro ao fazer cópia temporária.\n");
                    return 1;
                }

                int loops;
                printf("Aplicar filtro quantas vezes? ");
                scanf("%d", &loops);

                for (int i = 0; i < loops; ++i) {
                    convolution(a, b, k);
                    int **tmp = a->pixels;
                    a->pixels = b->pixels;
                    b->pixels = tmp;
                }
                writeFile(b, outName);
                freeImage(b);
            }
            freeKernel(k);
        }
        else if(op == 7){
            inverterCor(a,b);
            writeFile(b,outName);
            freeImage(b);
        }
    }

    freeImage(a);

    return 0;
}

int readFile(PGMimg* pgm, char* filename){

    //Abrindo o arquivo
    FILE *imgFile = fopen(filename, "r");

    //Conferindo se o arquivo existe
    if (imgFile == NULL){
        printf("Arquivo não encontrado!\n");
        return 0;
    }
    
    //Lendo o tipo de PMG do arquivo
    fscanf(imgFile, "%s", pgm->type);

    //Se não for um plain PMG, recusa o arquivo
    if (strcmp(pgm->type, "P2")){
        printf("Arquivo de tipo inválido!\n");
        return 0;
    }

    //Lendo o comentário, as dimensões e o Maxval do arquivo
    char aux = fgetc(imgFile);
    aux = fgetc(imgFile);
    fseek(imgFile, -1, SEEK_CUR);
    if (aux == '#'){
        fscanf(imgFile, " %80[^\n]s", pgm->com);
    }
    fscanf(imgFile, "%d %d", &(pgm->width), &(pgm->height));
    fscanf(imgFile, "%d", &(pgm->maxVal));

    //Checando se as dimensões do arquivo são válidas
    if (pgm->height <= 0 || pgm->width <= 0){
        printf("Arquivo de dimensões inválidas!\n");
        return 0;
    }

    //Alocando memória para a imagem
    pgm->pixels = (int**)malloc(pgm->height * sizeof(int*));
    if (pgm->pixels == NULL) {
        printf("Malloc falhou\n");
        return 0;
    }
    int i;
    for (i = 0; i < pgm->height; i++){
        pgm->pixels[i] = (int*)malloc(pgm->width * sizeof(int));
        if (pgm->pixels[i] == NULL) {
            printf("Malloc falhou\n");
            return 0;
        }
        //Lendo os pixels da imagem
        int j;
        for (j = 0; j < pgm->width; j++){
            fscanf(imgFile, "%d", &pgm->pixels[i][j]);
        }
    }

    //Fechando o arquivo
    fclose(imgFile);

    return 1;
}
int writeFile(PGMimg* pgm, char* filename){
    //PS: Isso vai sobreescrever qualquer arquivo que tenha o nome recebido pela função
    //Abrindo o arquivo
    FILE *imgFile = fopen(filename, "w+");
    printf("Arquivo lido\n");
    
    //Escrevendo as informações no arquivo
    fprintf(imgFile, "%s\n", pgm->type);
    fprintf(imgFile, "#%s\n", pgm->com);
    fprintf(imgFile, "%d %d\n", pgm->width, pgm->height);
    fprintf(imgFile, "%d\n", pgm->maxVal);

    //Coloca a informação da imagem no arquivo
    int i, j, k, nums = 0;
    for (i = 0; i < pgm->height; i++){
        for (j = 0; j < pgm->width; j++){
            fprintf(imgFile, "%d", pgm->pixels[i][j]);
            for(k = 0; k < 1 + (digits(pgm->maxVal) - digits(pgm->pixels[i][j])); k++){
                fputc(' ', imgFile);
            }
            nums++;
            if (nums == 11){
                fputc('\n', imgFile);
                nums = 0;
            }
        }
    }

    //Fechando o arquivo
    fclose(imgFile);

    return 1;
}
int transferData(PGMimg* in, PGMimg* out){
    //Transfere as informações de uma imagem para outra
    strcpy(out->type, in->type);
    strcpy(out->com, in->com);
    out->maxVal = in->maxVal;
    out->width = in->width;
    out->height = in->height;

    out->pixels = (int**)malloc(out->height * sizeof(int*));
    if (out->pixels == NULL) {
        printf("Malloc falhou\n");
        return 0;
    }
    int i;
    for (i = 0; i < out->height; i++){
        out->pixels[i] = (int*)malloc(out->width * sizeof(int));
        if (out->pixels[i] == NULL) {
            printf("Malloc falhou\n");
            return 0;
        }
        int j;
        for (j = 0; j < out->width; j++){
            out->pixels[i][j] = in->pixels[i][j];
        }
    }
    printf("Informação transferida entre imagens!\n");

    return 1;
}
int convolution(PGMimg* in, PGMimg* out, kernel* k){
    //Testando se as imagens tem o mesmo tamanho;
    if (in->width != out->width || in->height != out->height){
        printf("Algo deu errado na convolução\n");
        return 0;
    }
    //Realizando a convolução no arquivo in e colocando os resultados no arquivo out
    printf("Iniciando convolução\n");
    int i, j, a, b;
    for (i = 0; i < in->height; i++){
        for (j = 0; j < in->width; j++){
            int validPos = k->positions;
            int weights = k->totalWeight;
            int light = 0; //Oh nome ruim kkkkkk
            for (a = -k->radius; a <= k->radius; a++){
                for (b = -k->radius; b <= k->radius; b++){
                    if ((i + a <= -1) || (i + a >= in->height) || (j + b <= -1) || (j + b >= in->width)){
                        validPos--;
                        weights -= k->matrix[k->radius+a][k->radius+b];
                    }
                    else {
                        light+= (in->pixels)[i+a][j+b] * k->matrix[k->radius+a][k->radius+b];
                    }
                }
            }
            //Calculando o resultado de out[i][j]
            out->pixels[i][j] = light/weights;
        }
        printf("Linha %d concluída\n", i);
    }
    printf("Convolução concluída\n");

    return 1;
}
int digits(int n){
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
    printf("ERRO!!");
    return 0;
}
void freeKernel(kernel *k) {
    if (k == NULL)
        return;
    if (k->matrix != NULL) {
        int i;
        for (i = 0; i < k->size; ++i)
            if (k->matrix[i] != NULL)
                free(k->matrix[i]);
        free(k->matrix);
    }

    free(k);
}
void freeImage(PGMimg *img) {
    if (img == NULL)
        return;
    if (img->pixels != NULL) {
        int i;
        for (i = 0; i < img->height; ++i)
            if (img->pixels[i] != NULL)
                free(img->pixels[i]);
        free(img->pixels);
    }
    free(img);
}
kernel *createKernel(int n, int aux[n][n], int totalWeight, int positions) {
    kernel *k = malloc(sizeof(kernel));
    if (k == NULL) {
        printf("Falha ao alocar memória pra estrutura do Kernel.\n");
        free(k);
        return NULL;
    }
    k->size = n;
    k->totalWeight = totalWeight;
    k->radius = n / 2;
    k->positions = positions;
    k->matrix = malloc(n * sizeof(int *));
    if (k->matrix == NULL) {
        freeKernel(k);
        return NULL;
    }
    int i, len = n * sizeof(int);
    for (i = 0; i < n; ++i) {
        k->matrix[i] = malloc(len);
        if (k->matrix[i] == NULL) {
            freeKernel(k);
            return NULL;
        }
        memmove(k->matrix[i], aux[i], len);
    }
    return k;
}
kernel *getKernel(FilterType type) {
    if (type == MEDIAN) {
        int matrix[][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
        return createKernel(3, matrix, 9, 9);
    } else if (type == GAUSS_3X3) {
        int matrix[][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
        return createKernel(3, matrix, 16, 9);
    } else if (type == GAUSS_5X5) {
        int matrix[][5] = {{1, 4, 7, 4, 1},
                           {4, 16, 26, 16, 4},
                           {7, 26, 41, 26, 7},
                           {4, 16, 26, 16, 4},
                           {1, 4, 7, 4, 1}};
        return createKernel(5, matrix, 273, 25);

    } else if (type == GAUSS_7X7) {
        int matrix[][7] = {
            {0, 0, 1, 2, 1, 0, 0},      {0, 3, 13, 22, 13, 3, 0},
            {1, 13, 59, 97, 59, 13, 1}, {2, 22, 97, 159, 97, 22, 2},
            {1, 13, 59, 97, 59, 13, 1}, {0, 3, 13, 22, 13, 3, 0},
            {0, 0, 1, 2, 1, 0, 0}};
        return createKernel(7, matrix, 1003, 49);
    } else if (type == SORBEL_X) {
        int matrix[][3] = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
        return createKernel(3, matrix, 0, 9);

    } else if (type == SORBEL_Y) {
        int matrix[][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
        return createKernel(3, matrix, 0, 9);
    }
    printf("Filtro inválido!\n");
    return NULL;
}
int inverterCor(PGMimg* in,PGMimg* out){
    int i,j,aux;
    aux = in->maxVal;
    for(i=0;i < in->height;i++){
        for(j=0;j < in->width;j++){
            (out->pixels)[i][j]=aux-(in->pixels)[i][j];
        }
    }

}
