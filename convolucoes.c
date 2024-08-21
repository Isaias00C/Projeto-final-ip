//convoluçao (1, 2, 3) * (4, 5, 6) = (4, 13, 28, 27, 18)
//de forma geral convolucao (a * b)n  = somatorio(ai * bj) com i+j = n e n pertencente a [2, a.lenght + b.lenght]
//para imagem: entra com um kernel de dimensoes height e width (normalmente quadrado)
//o valor do pixel de saida é a convolução do pixel de entrada e o kernel 


//aqui vai ficar uma convolucao que sirva pra qualquer coisa, como funcoes, calculo de integrais, entre outras coisas
#include <stdio.h>
#include <stdlib.h>

int* convolutionForVectors(int* a, int*b);

int main(){

}

int* convolutionForVectors(int* a, int* b){
    int* s;

    for(int i = 0; ; i++){
        for(int aux; aux < i; aux++){
            s[i] += a[aux]*[i-aux];
        }
    }

    return s;
} 