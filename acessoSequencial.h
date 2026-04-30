#ifndef ACESSOSEQUENCIAL_H
#define ACESSOSEQUENCIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "auxiliares.h"

#define ITENS_PAGINA 16
#define NUM_MOLDURA 50

typedef struct {
    Registro itens[ITENS_PAGINA];
    int numPagina;      // -1 = vazia
    int frequencia;     // pra contar acesso, e saber quem retirar
    int qntItens;       // para facilitar manueseio da ultima
} Moldura;

Moldura* inicializaMoldura();
void destroiMoldura(Moldura *moldura);
int buscarMoldura(Moldura *moldura, int numPagina);
int escolherVitima(Moldura *moldura,bool *vazia);
int carregarPagina(Moldura *moldura, FILE *arq, int numPagina, int totalPaginas, Config *config, Metricas *metricas);

void criarIndice(FILE *arq, int *vetorIndice, Config *config, Metricas *metricas);
int getNumPaginas(Config* config);
bool acessoSequencialIndexado(int *vetorIndice, FILE *arq, Registro *reg, int totalPaginas,Moldura *moldura, Config *config,Metricas *metricas);

int buscaBinariaIndices(int *vetorIndice, Config *config, Metricas *metricas, Registro *registro);
int buscaBinariaMoldura(Moldura *moldura, int tamanho, int alvo);
bool buscaBinariaPagina(Moldura *moldura, int paginaAtual, Registro *reg,Metricas *metricas,Config config);

int getNumItensPagina(Config* config);

#endif