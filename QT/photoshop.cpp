#include "photoshop.h"

using namespace std;

int photoshop(const char* filename, int op){

    PGMimg* a = (PGMimg*)malloc(sizeof(PGMimg));
    if (readFile(a, filename) == 1) {
        PGMimg* b = (PGMimg*)malloc(sizeof(PGMimg));
        char outname[200];
        strcpy(outname, filename);
        outname[strlen(outname) - 4] = '\0';
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

                int i, loops = max(1, ((a->height * a->width) / 10000));
                for (i = 0; i < loops; ++i) {
                    convolution(a, b, k);
                    int **tmp = a->pixels;
                    a->pixels = b->pixels;
                    b->pixels = tmp;
                }
                switch (op) {
                    case 1: strcat(outname, "_median.pgm"); break;
                    case 2: strcat(outname, "_gauss3.pgm"); break;
                    case 3: strcat(outname, "_gauss5.pgm"); break;
                    case 4: strcat(outname, "_gauss7.pgm"); break;
                    case 5: strcat(outname, "_sobel_x.pgm"); break;
                    case 6: strcat(outname, "_sobel_y.pgm"); break;
                }
                writeFile(b, outname);
                freeImage(b);
            }
            freeKernel(k);
        }
        else if(op == 7){
            transferData(a,b);
            inverterCor(a,b);
            strcat(outname, "_inverted.pgm");
            writeFile(b,outname);
            freeImage(b);
        }
        else if(op == 8){
            rotateData(a,b);
            rotate90(a, b);
            strcat(outname, "_rotated.pgm");
            writeFile(b, outname);
            freeImage(b);
        }
        else if(op == 9){
            normalize(a);
            transferData(a, b);
            strcat(outname, "_equalized.pgm");
            writeFile(b, outname);
            freeImage(b);
        }
    }

    freeImage(a);

    return 0;
}

int readFile(PGMimg* pgm, const char* filename){

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
    /*char aux = fgetc(imgFile);
    aux = fgetc(imgFile);
    fseek(imgFile, -1, SEEK_CUR);
    if (aux == '#'){
        fscanf(imgFile, " %80[^\n]s", pgm->com);
    }*/
    ignoreComments(imgFile);
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

int rotateData(PGMimg* in, PGMimg* out){
    //Transfere as informações de uma imagem para outra
    strcpy(out->type, in->type);
    strcpy(out->com, in->com);
    out->maxVal = in->maxVal;
    out->height = in->width;
    out->width = in->height;

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
        //printf("Linha %d concluída\n", i);
    }
    //printf("Convolução concluída\n");

    return 1;
}

int digits(int n){
    if (n < 10) return 1;
    if (n < 100) return 2;
    if (n < 1000) return 3;
    if (n < 10000) return 4;
    if (n < 100000) return 5;
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

kernel *createKernel(int n, vector<vector<int>> aux, int totalWeight, int positions) {
    kernel *k = (kernel*)malloc(sizeof(kernel));
    if (k == NULL) {
        printf("Falha ao alocar memória pra estrutura do Kernel.\n");
        free(k);
        return NULL;
    }
    k->size = n;
    k->totalWeight = totalWeight;
    k->radius = n / 2;
    k->positions = positions;
    k->matrix = (int**)malloc(n * sizeof(int *));
    if (k->matrix == NULL) {
        freeKernel(k);
        return NULL;
    }
    int i, j, len = n * sizeof(int);
    for (i = 0; i < n; ++i) {
        k->matrix[i] = (int*)malloc(len);
        if (k->matrix[i] == NULL) {
            freeKernel(k);
            return NULL;
        }
        for (j = 0; j < n; ++j){
            k->matrix[i][j] = aux[i][j];
        }
    }
    return k;
}

kernel *getKernel(int type) {
    if (type == MEDIAN) {
        vector<vector<int>> matrix {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
        return createKernel(3, matrix, 9, 9);
    } else if (type == GAUSS_3X3) {
        vector<vector<int>> matrix {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
        return createKernel(3, matrix, 16, 9);
    } else if (type == GAUSS_5X5) {
        vector<vector<int>> matrix {{1, 4, 7, 4, 1},
                           {4, 16, 26, 16, 4},
                           {7, 26, 41, 26, 7},
                           {4, 16, 26, 16, 4},
                           {1, 4, 7, 4, 1}};
        return createKernel(5, matrix, 273, 25);

    } else if (type == GAUSS_7X7) {
        vector<vector<int>> matrix {
                           {0, 0, 1, 2, 1, 0, 0},      {0, 3, 13, 22, 13, 3, 0},
                           {1, 13, 59, 97, 59, 13, 1}, {2, 22, 97, 159, 97, 22, 2},
                           {1, 13, 59, 97, 59, 13, 1}, {0, 3, 13, 22, 13, 3, 0},
                           {0, 0, 1, 2, 1, 0, 0}};
        return createKernel(7, matrix, 1003, 49);
    } else if (type == SOBEL_X) {
        vector<vector<int>> matrix {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
        return createKernel(3, matrix, 0, 9);

    } else if (type == SOBEL_Y) {
        vector<vector<int>> matrix {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
        return createKernel(3, matrix, 0, 9);
    }
    printf("Filtro inválido!\n");
    return NULL;
}

void inverterCor(PGMimg* in,PGMimg* out){
    int i,j,aux;
    aux = in->maxVal;
    for(i=0;i < in->height;i++){
        for(j=0;j < in->width;j++){
            (out->pixels)[i][j]=aux-(in->pixels)[i][j];
        }
    }

}

void rotate90(PGMimg* in,PGMimg* out){
    int i,j;
    for(i=0;i < in->height;i++){
        for(j=0;j < in->width;j++){
            (out->pixels)[j][in->height - i]=(in->pixels)[i][j];
        }
    }
}

void ignoreComments(FILE* fp) {
    int ch;
    char line[100];

    while ((ch = fgetc(fp)) != EOF) {
        if (!isspace(ch)) {

            if (ch == '#') {
                if (fgets(line, sizeof(line), fp) == NULL) {

                    return;
                }

                continue;
            } else {

                ungetc(ch, fp);
                return;
            }
        }
    }
}

void normalize(PGMimg* pgm){
    double* hist = (double*)malloc(sizeof(double)*(pgm->maxVal + 1));

    for(int i = 0; i < pgm->height; i++){
        for(int j = 0; j < pgm->width; j++){
            hist[pgm->pixels[i][j]]++;
        }
    }

    for(int i = 0; i < pgm->maxVal + 1; i++){
        hist[i] = hist[i] / (float)(pgm->width * pgm->width);
    }

    for(int i = 0; i < pgm->maxVal + 1; i++){
        hist[i] = hist[i] + hist[i-1];
    }

    for(int i = 0; i < pgm->maxVal + 1; i++){
        hist[i] = hist[i] * pgm->maxVal;
    }

    for(int i = 0; i < pgm->height; i++){
        for(int j = 0; j < pgm->width; j++){
            pgm->pixels[i][j] = hist[pgm->pixels[i][j]];
        }
    }

    free(hist);
}