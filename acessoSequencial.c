#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "acessoSequencial.h"
#include "auxiliares.h"

void criarIndice(FILE *arq, int *vetorIndice, int totalRegistros) {
    int numPaginas = getNumPaginas(totalRegistros);

    int chave;
    for (int i = 0; i < numPaginas; i++) {

        long deslocamento = (long) (i * ITENS_PAGINA * sizeof(Registro));

        fseek(arq, deslocamento, SEEK_SET);

        // Lê apenas o campo 'chave' do primeiro registro da página
        if (fread(&chave, sizeof(int), 1, arq) == 1)
            vetorIndice[i] = chave;
    }
}

int getNumPaginas(int totalRegistros) {
    int numPaginas = totalRegistros / ITENS_PAGINA;

    if (totalRegistros % ITENS_PAGINA != 0)
        numPaginas++;
    return numPaginas;
}

bool acessoSequencialIndexado(int *vetorIndice, FILE *arq, Registro *reg, int numPaginas, Metricas *metricas) {
    Registro pagina[ITENS_PAGINA];
    int i = 0, qntItens;
    long deslocamento;

    while (i < numPaginas && vetorIndice[i] <= reg->chave)
        i++;

    if (i == 0)
        return false;
    else
        if (i == numPaginas) {
            fseek(arq, 0, SEEK_END);
            qntItens = (ftell(arq) / sizeof(Registro)) % ITENS_PAGINA;
        }
        else
            qntItens = ITENS_PAGINA;

    deslocamento = (i - 1) * ITENS_PAGINA * sizeof(Registro);
    fseek(arq, deslocamento, SEEK_SET);
    fread(&pagina, sizeof(Registro), qntItens, arq);

    for (i = 0; i < qntItens; i++) {
        metricas->comparacoes++;
        if (reg->chave < pagina[i].chave)
            return false;

        if (pagina[i].chave == reg->chave) {
            *reg = pagina[i];
            return true;
        }
    }

    return false;
}