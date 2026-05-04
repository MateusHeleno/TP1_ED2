#include <stdio.h>
#include <stdlib.h>

#include "auxiliares.h"
#include "arvBinaria.h"

void criarArvBinaria(Config *config, FILE* arqOriginal, const char *arqArvBin) {
    FILE* arvBin = fopen(arqArvBin, "wb+");
    if (!arvBin) {
        printf("Erro ao abrir/criar arquivo da árvore binária.\n");
        return;
    }

    

    No noAtual;
}