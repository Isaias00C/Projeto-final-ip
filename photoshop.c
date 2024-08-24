#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
// teste do pedrinho
//Struct da Imagem
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
} kernel;

int readFile(PGMimg* pgm, char* filename);
int writeFile(PGMimg* pgm, char* filename);
int transferData(PGMimg* in, PGMimg* out);
int convolution(PGMimg* in, PGMimg* out, kernel* k);
int digits(int n);
void createKernel(kernel* k, int n, int totalWeight, int radius, int positions);

//Função temporária pra testes
void teste(){
    
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    int i, j;
    //Inicializando o kernel do filtro da mediana
    kernel* median = (kernel*)malloc(sizeof(kernel));
    int auxMedian[3][3] = {
        {1, 1, 1}, 
        {1, 1, 1}, 
        {1, 1, 1}
    };
    createKernel(median, 3, 9, 2, 9);
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            median->matrix[i][j] = auxMedian[i][j];
        }
    }
    //Inicializando o kernel do filtro gaussiano 3x3
    kernel* gauss3 = (kernel*)malloc(sizeof(kernel));
    int auxGauss3[3][3] = {
        {1, 2, 1}, 
        {2, 4, 2}, 
        {1, 2, 1}
    };
    createKernel(gauss3, 3, 16, 2, 9);
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            gauss3->matrix[i][j] = auxGauss3[i][j];
        }
    }
    //Inicializando o kernel do filtro gaussiano 5x5
    kernel* gauss5 = (kernel*)malloc(sizeof(kernel));
    int auxGauss5[5][5] = {
        {1, 4, 7, 4, 1}, 
        {4, 16, 26, 16, 4}, 
        {7, 26, 41, 26, 7}, 
        {4, 16, 26, 16, 4}, 
        {1, 4, 7, 4, 1}
    };
    createKernel(gauss5, 5, 273, 3, 25);
    for (i = 0; i < 5; i++){
        for (j = 0; j < 5; j++){
            gauss5->matrix[i][j] = auxGauss5[i][j];
        }
    }
    //Inicializando o kernel do filtro gaussiano 7x7
    kernel* gauss7 = (kernel*)malloc(sizeof(kernel));
    int auxGauss7[7][7] = {
        {0, 0, 1, 2, 1, 0, 0},
        {0, 3, 13, 22, 13, 3, 0},
        {1, 13, 59, 97, 59, 13, 1},
        {2, 22, 97, 159, 97, 22, 2},
        {1, 13, 59, 97, 59, 13, 1},
        {0, 3, 13, 22, 13, 3, 0},
        {0, 0, 1, 2, 1, 0, 0}
    };
    createKernel(gauss7, 7, 1003, 4, 49);
    for (i = 0; i < 7; i++){
        for (j = 0; j < 7; j++){
            gauss7->matrix[i][j] = auxGauss7[i][j];
        }
    }
    //Inicializando o kernel horizontal do filtro Sorbel
    kernel* sobelX = (kernel*)malloc(sizeof(kernel));
    int auxSobelX[3][3] = {
        {1, 0, -1}, 
        {2, 0, -2}, 
        {1, 0, -1}
    };
    createKernel(sobelX, 3, 0, 2, 9);
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            sobelX->matrix[i][j] = auxSobelX[i][j];
        }
    }
    //Inicializando o kernel vertical do filtro Sobel
    kernel* sobelY = (kernel*)malloc(sizeof(kernel));
    int auxSobelY[3][3] = {
        {1, 2, 1}, 
        {0, 0, 0}, 
        {-1, -2, -1}
    };
    createKernel(sobelY, 3, 0, 2, 9);
    for (i = 0; i < 3; i++){
        for (j = 0; j < 3; j++){
            sobelY->matrix[i][j] = auxSobelY[i][j];
        }
    }
    
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
        printf("Qual o filtro? 1 = Mediana, 2 = Gauss Low, 3 = Gauss Mid, 4 = Gauss High\n");
        scanf("%d", &op);
        switch (op) {
        case 1:
            convolution(a, b, median);
            break;
        case 2:
            convolution(a, b, gauss3);
            break;
        case 3:
            convolution(a, b, gauss5);
            break;
        case 4:
            convolution(a, b, gauss7);
            break;
        }
        writeFile(b, outName);
    }

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
    fprintf(imgFile, "%s\n", pgm->com);
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
void createKernel(kernel* k, int n, int totalWeight, int radius, int positions){
    k->totalWeight = totalWeight;
    k->radius = radius;
    k->positions = positions;
    k->matrix = (int**)malloc(n*sizeof(int*));
    int i;
    for (i = 0; i < n; i++){
        k->matrix[i] = (int*)malloc(n*sizeof(int));
    }
}
