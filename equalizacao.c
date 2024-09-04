#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
void pmf(double *hist, int totalPixels, int max)
{
  int i;
  for(i = 0; i < max+1; i++)
  {
    hist[i] = hist[i] / (float) totalPixels;
  }
}
 
void cdf(double *hist, int max)
{
  int i;
  for(i = 1; i < max+1; i++)
  {
    hist[i] = hist[i] + hist[i-1];
  }
}
 
void normalize(double *hist, int max)
{
  int i;
  for(i = 0; i < max+1; i++)
  {
    hist[i] = hist[i] * max;
  }
}
 
int main()
{
  FILE *image; // Ponteiro para o arquivo que contem a imagem que vamos tratar
  FILE *newImage; // Ponteiro para o arquivo onde vamos salvar a imagem tratada
  char image_name[256];
  char newImage_name[256];
  char key[128];
  int i, j, m, n, max, totalPixels;
 
  printf("Digite o nome do arquivo PGM de entrada: ");
  scanf("%s", image_name); // Le o nome do arquivo de entrada
  printf("Digite o nome do arquivo PGM de saida: ");
  scanf("%s", newImage_name);
 
 
  image = fopen(image_name , "r") ; // Abre o arquivo no modo leitura
  if(image == NULL) // Verifica se o arquivo existe e foi aberto
  {
    printf("Erro na abertura do arquivo %s\n", image_name);
    return 0 ;
  }
 
  // Le dados do cabeçalho
  fscanf(image, "%s", key);
 
  // Verifica se a imagem esta em PGM
  if(strcmp(key,"P2") != 0)
  {
    printf("Arquivo nao e um PGM\n") ;
    fclose(image) ;
    return 0 ;
  }
 
  // Le os outros dados do cabecalho
  fscanf(image, "%d %d %d", &m, &n, &max) ;

  printf("%d %d %d\n", m ,n, max);
  // m guarda a largura da imagem (colunas da matriz)
  // n guarda a altura da imagem (linhas da matriz)
  // max guarda o valor do maior tom de cinza
 
  // Aloca memoria para guardar o valor dos pixels
  // nas celulas da matriz M
  int **M;
  M = (int**) malloc(n * sizeof(int*));
  for(i = 0; i < n; i++)
  {
    M[i] = (int*) malloc(m * sizeof(int));
  }
 
  // Aloca memoria para o histograma
  double *hist = (double*) malloc((max+1) * sizeof(double));
  for(i = 0; i < max+1; i++)
    hist[i] = 0;
 
  // Le os dados da imagem e armazena na matriz M
  // E preenche o histograma
  for(i = 0; i < n; i++)
  {
    for(j = 0; j < m ;j++)
    {
      fscanf(image, "%d ", &M[i][j]);
      hist[M[i][j]]++;  //(1)
    }
  }
 
  totalPixels = m * n;
  pmf(hist, totalPixels, max);
  cdf(hist, max);
  normalize(hist, max);
 
  for(i = 0; i < n; i++)
  {
    for(j = 0; j < m; j++)
    {
      M[i][j] = hist[M[i][j]]; //(2)
    }
  }
 
  newImage = fopen(newImage_name , "w"); // Abre o arquivo no modo escrita
  fprintf(newImage,"P2\n"); // Escreve o cabecalho
  fprintf(newImage, "%d %d %d\n", m, n, max); // Escreve o cabecalho
  printf("%d %d %d\n", m ,n, max);
 
  for(i = 0; i < n; i++)
  {
    for(j = 0; j < m; j++)
    {
      fprintf(newImage, "%d ", M[i][j]); // Escreve os valores de cada
    }                                          // pixel no arquivo
  }
 
  fclose(image); // Fecha o arquivo da imagem original
  fclose(newImage); // Fecha o arquivo da nova imagem
  free(hist); // Libera a memória alocada para o histograma
 
  // Libera a memória alocada para a matriz M
  for(i = 0; i < n; i++)
  {
    free(M[i]);
  }
  free(M);
  return 0;
}