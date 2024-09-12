# Projeto Final

## Assunto: PGM_plain

### Sobre PGM_plain:

.pgm é um tipo de imagem que contém algumas informções dentro dela, como o **tipo** do PGM podendo ser P2 ou P5 (a primeira sendo composta por pixels decimais, ja a segunda com pixels binários, trataremos nesse projeto apenas o caso de arquivos não binários, ou seja, apenas arquivos P2), **comentário** (sendo facultativo a existencia dele), valores especificando as dimensões da imagem(**height** e **width**) e por último o **valor máximo** da escala de cinzas que pode ser achado nos pixels da imagem

um exemplo de um arquivo do tipo .pgm pode ser visto a baixo:

<pre>
P2 
# feep.ascii.pgm 
24 7 
15 
0 0  0  0  0  0  0  0  0 0  0  0  0  0  0  0  0 0  0  0  0  0  0  0
0 3  3  3  3  0  0  7  7 7  7  0  0 11 11 11 11 0  0 15 15 15 15  0
0 3  0  0  0  0  0  7  0 0  0  0  0 11  0  0  0 0  0 15  0  0 15  0
0 3  3  3  0  0  0  7  7 7  0  0  0 11 11 11  0 0  0 15 15 15 15  0
0 3  0  0  0  0  0  7  0 0  0  0  0 11  0  0  0 0  0 15  0  0  0  0
0 3  0  0  0  0  0  7  7 7  7  0  0 11 11 11 11 0  0 15  0  0  0  0
0 0  0  0  0  0  0  0  0 0  0  0  0  0  0  0  0 0  0  0  0  0  0  0
</pre>

### Convolução:

Convolução é uma forma de criar uma sequência derivada de uma sequência inicial, a partir da soma do produto dos elementos que se sobrepõe.

Por exemplo, dado uma sequência u = [1, 2, 3, 4] podemos achar uma sequência final a fazendo a convolução de u com uma outra sequência v (seja v = [4, 3, 2, 1]).
Assim temos, uma sequencia final (A<sub>n</sub>), de modo que:<br>

<pre>
1. A<sub>1</sub>. u:          [1, 2, 3, 4]<br>
       v: [4, 3, 2, 1]

logo, A<sub>1</sub> = 1 * 1 = 1

2. A<sub>2</sub>. u:          [1, 2, 3, 4]<br>
       v:    [4, 3, 2, 1]

logo, A<sub>2</sub> = 1 * 2 +  2 * 1 = 4

3. A<sub>3</sub>. u:          [1, 2, 3, 4]<br>
       v:       [4, 3, 2, 1]

logo, A<sub>3</sub> = 1 * 3 + 2 * 2 + 3 * 1 = 10

4. A<sub>4</sub>. u:          [1, 2, 3, 4]<br>
       v:          [4, 3, 2, 1]

logo, A<sub>4</sub> = 1 * 4 + 2 * 3 + 3 * 2 + 4 * 1 = 20

5. A<sub>5</sub>. u:          [1, 2, 3, 4]<br>
       v:             [4, 3, 2, 1]

logo, A<sub>5</sub> = 2 * 4 + 3 * 3 + 4 * 2 = 25

6. A<sub>6</sub>. u:          [1, 2, 3, 4]<br>
       v:                [4, 3, 2, 1]

logo, A<sub>6</sub> = 3 * 4 + 4 * 3 = 24

7. A<sub>7</sub>. u:          [1, 2, 3, 4]<br>
       v:                   [4, 3, 2, 1]

logo, A<sub>7</sub> = 4 * 4 = 16
</pre>

Por fim temos uma sequência A<sub>n</sub> = [1, 4, 10, 20, 25, 24, 16], ou seja, Convolução(u,v) = A<sub>n</sub>.

Tal ideia pode ser aplicada pra matrizes também, que é o caso do PGM, dado uma matriz de pixels, faremos a convolução dela com outra matriz (kernel), para achar uma nova matriz de pixels, que será impressa em um arquivo, resultando em uma imagem diferente daquela inicial

<hr>

colaboradores: <br/>

[Isaias Castro da Silveira Gonçalves](https://github.com/Isaias00C)<br/>
[Vinicius de Castro](https://github.com/Vinicius-de-Castro)<br/>
[Pedro Miguel](https://github.com/PMiguelit0)<br/>
[Luis André Oliveira Lessa](https://github.com/LuisLessa01)<br/>
