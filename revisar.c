//https://www.geeksforgeeks.org/histogram-equalisation-in-c-image-processing/
#include<stdio.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>

void histogramEqualisation(int colunas, int linhas, char* input_file_name, char* output_file_name) {

	unsigned char* image;
	int hist[256] = { 0 };
	int new_gray_level[256] = { 0 };
	int input_file, output_file, coluna, linha, total, curr, i;

	image = (unsigned char*) calloc(colunas, sizeof(unsigned char));

	input_file = open(input_file_name, O_RDONLY);
	if(input_file < 0) {
		printf("Erro ao abrir o arquivo de entrada!\n");
		return;
	}

	output_file = fopen(output_file_name,"r+");

	/* Calculando a frequencia de ocorrencia dos valores para cada pixel */
	
	for (linha = 0; linha < linhas; linha++) {
		read(input_file, &image[0], colunas * sizeof(unsigned char));

		/* Calculando histograma*/
		for (coluna = 0; coluna < colunas; coluna++) {
			hist[(int)image[coluna]]++;
		}
	}
	
	total = colunas * linhas;

	curr = 0;

	/* Calculando frequencia cumulativa e novos niveis de cinza */
	for (i = 0; i < 256; i++) {
		curr += hist[i];

		new_gray_level[i] = round((((float)curr) * 255) / total);
	}

	close(input_file);

	input_file = open(input_file_name, O_RDONLY);

	/* Equalizando o histograma */
	for (linha = 0; linha < linhas; linha++) {
		read(input_file, &image[0], colunas * sizeof(unsigned char));
		
		for (coluna = 0; coluna < colunas; coluna++) {
			image[coluna] = (unsigned char) new_gray_level[image[coluna]];
		}

		write(output_file, &image[0], colunas * sizeof(unsigned char));
	}

	free(image);
	close(input_file);
	close(output_file);
}

int main() {
	char input_file_name[256];
	char output_file_name[256];
	int colunas, linhas;

	printf("Digite o nome do arquivo PGM de entrada: \n");
	scanf("%s",input_file_name);
	printf("Digite o numero de colunas(largura) da imagem PGM\n");
	scanf("%d",&colunas);
	printf("Digite o numero de linhas(altura) da imagem PGM\n");
	scanf("%d",&linhas);
	printf("Digite o nome do arquivo PGM de saida: \n");
	scanf("%s", output_file_name);

	histogramEqualisation(colunas, linhas, input_file_name, output_file_name);

	return 0;
}






	




