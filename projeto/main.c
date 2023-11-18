#include <string.h>
#include <stdlib.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

//struct para coordenada e pontuação do player

/*ideia para movimentação do player:
-> inicialmente, printa a matriz com o ponto presente no quadrado 0x0
-> caso o jogador clique para se mover, limpa terminal e printa denovo o ponto na outra coordenada
*/

/*funcao para printar o cenario:
-> cenario consiste em uma matriz desenhada
-> utilizar de loops

exemplo:
 __ __ __ ___
|__|__|__|__|
|__|__|__|__|
|__|__|__|__|

porem, em larga escala. (talvez exista algum codigo na internet que printe tabelas em C)
*/