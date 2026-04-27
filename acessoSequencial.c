#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "acessoSequencial.h"
#include "auxiliares.h"

void criarIndice(FILE *arq, int **vetorIndice, int totalRegistros) {
    int numPaginas = getNumPaginas(totalRegistros);

    int chave;
    for (int i = 0; i < numPaginas; i++) {

        long deslocamento = (long) (i * ITENS_PAGINA * sizeof(Registro));

        fseek(arq, deslocamento, SEEK_SET);

        // Lê apenas o campo 'chave' do primeiro registro da página
        if (fread(&chave, sizeof(int), 1, arq) == 1)
            *(vetorIndice[i]) = chave;
    }
}

int getNumPaginas(int totalRegistros) {
   int numPaginas = totalRegistros / ITENS_PAGINA;

    if (totalRegistros % ITENS_PAGINA != 0)
        numPaginas++;
    return numPaginas;
}