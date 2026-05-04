#ifndef ARVBINARIA_H
#define ARVBINARIA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "auxiliares.h"

typedef struct {
    Registro reg;
    long esq;
    long dir;
    // -1 em algum dos campos acima significa que o nodo não possui filhos
} No;

void criarArvBinaria(Config* config, FILE* arqOriginal, const char *arqArvBin, Metricas *metricas);
void insereNo(FILE* arvBin, Registro reg, Metricas *metricas);
void inicializaNo(No* no, Registro reg);

#endif