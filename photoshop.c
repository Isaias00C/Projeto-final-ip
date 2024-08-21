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

int readFile(PGMimg* pgm, char* filename);
int writeFile(PGMimg* pgm, char* filename);
int transferData(PGMimg* in, PGMimg* out);
int convolution(PGMimg* in, PGMimg* out, int** kernel, int radius, int totalWeight);

//Função temporária pra testes
void teste(){
    PGMimg* a = (PGMimg*)malloc(sizeof(PGMimg));
    char name[100];
    scanf("%s", name);
    if (readFile(a, name) == 1) {
        PGMimg* b = (PGMimg*)malloc(sizeof(PGMimg));
        transferData(a, b);
        printf("Qual o nome do output?\n");
        char outName[100];
        scanf("%s", outName);
        /*//GAMBIARRA ABSURDA!!!!!
        int** gauss = (int**)malloc(3*sizeof(int*));
        int i;
        for (i = 0; i < 3; i++){
            gauss[i] = (int*)malloc(3*sizeof(int));
        }
        gauss[0][0] = 1;
        gauss[0][1] = 2;
        gauss[0][2] = 1;
        gauss[1][0] = 2;
        gauss[1][1] = 4;
        gauss[1][2] = 2;
        gauss[2][0] = 1;
        gauss[2][1] = 2;
        gauss[2][2] = 1;
        */

        int** kernel;
        int* rows;

        printf("Escolha um Kernel: ");
        printf("1.Gauss: \n{1, 2, 1\n2, 4, 2\n1, 2, 1\n}.\n");
        printf("2.Entre com seu kernel.\n");
        int op, dimensity;
        scanf("%d", &op);
        switch(op){
            case 1:
                int** gauss = (int**)malloc(3*sizeof(int*));
                int i;
                for (i = 0; i < 3; i++){
                    gauss[i] = (int*)malloc(3*sizeof(int));
                }
                gauss[0][0] = 1;
                gauss[0][1] = 2;
                gauss[0][2] = 1;
                gauss[1][0] = 2;
                gauss[1][1] = 4;
                gauss[1][2] = 2;
                gauss[2][0] = 1;
                gauss[2][1] = 2;
                gauss[2][2] = 1;
                break;
            case 2:
                printf("Digite a dimensao do seu Kernel: ");
                scanf("%d", &dimensity);
                kernel = (int**)malloc(sizeof(int*) * dimensity);
                int row;
                for(row = 0; row < dimensity; row++){
                    rows = (int*)malloc(sizeof(int) * dimensity);
                    int column;
                    for(column = 0; column < dimensity; column++){
                        scanf("%d", &kernel[row][column]);
                    }
                }
                break;
        }
        convolution(a, b, kernel, 1, 16);
        writeFile(b, outName);

        int row, column;
        for(row = 0; row < dimensity; row++){
            free(rows);
        }
        free(kernel);
    }
}

int main(){
    setlocale(LC_ALL, "Portuguese");
    teste();

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
        printf("Arquivo inválido!\n");
        return 0;
    }

    //Lendo o comentário, as dimensões e o Maxval do arquivo
    fscanf(imgFile, " %80[^\n]s", pgm->com);
    fscanf(imgFile, "%d %d", &(pgm->width), &(pgm->height));
    fscanf(imgFile, "%d", &(pgm->maxVal));

    //Checando se as dimensões do arquivo são válidas
    if (pgm->height <= 0 || pgm->width <= 0){
        printf("Arquivo inválido!\n");
        return 0;
    }

    //Alocando memória para a imagem
    pgm->pixels = (int**)malloc(pgm->height * sizeof(int*));
    if (pgm->pixels == NULL) {
        printf("Malloc falhou\n");
        return 0;
    }
    for (int i = 0; i < pgm->height; i++){
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
    int i, j;
    for (i = 0; i < pgm->height; i++){
        for (j = 0; j < pgm->width; j++){
            if (j != 0) {
                fprintf(imgFile, "%c", ' ');
                if (j % 6 == 0) fprintf(imgFile, "%c", '\n'); //Não fica bonito mas funciona
            }
            fprintf(imgFile, "%d", pgm->pixels[i][j]);
        }
        fprintf(imgFile, "%c", '\n');
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
    for (int i = 0; i < out->height; i++){
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

int convolution(PGMimg* in, PGMimg* out, int** kernel, int radius, int totalWeight){
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
            int validPos = (radius * 2 + 1) * (radius * 2 + 1);
            int weights = totalWeight;
            int light = 0; //Oh nome ruim kkkkkk
            for (a = -radius; a <= radius; a++){
                for (b = -radius; b <= radius; b++){
                    if ((i + a <= -1) || (i + a >= in->height) || (j + b <= -1) || (j + b >= in->width)){
                        validPos--;
                        weights -= kernel[radius+a][radius+b];
                    }
                    else {
                        light+= (in->pixels)[i+a][j+b] * kernel[radius+a][radius+b];
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
