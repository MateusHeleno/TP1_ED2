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
int carregarPagina(Moldura *moldura, FILE *arq, int numPagina,
                           int totalPaginas, Config *config, Metricas *metricas);

void criarIndice(FILE *arq, int *vetorIndice, Config *config);
int getNumPaginas(Config* config);
bool acessoSequencialIndexado(int *vetorIndice, FILE *arq, Registro *reg, int totalPaginas,Moldura *moldura, Config *config,Metricas *metricas);
bool buscaBinaria(Moldura *moldura, int paginaAtual, Registro *reg, Metricas *metricas, Config *config);
int getNumItensPagina(Config* config);

#endif

// bool buscaBinariaGeral(int *moldura, int paginaAtual, Registro *reg) {
//     int esq = 0;
//     int dir = tamanho - 1;

//     while (esq <= dir) {

//         int meio = esq + (dir - esq) / 2;
// metricas->comparacoes++;
//         if (moldura[paginaAtual].itens[meio].chave ==  reg->chave) {
//             *reg = moldura[paginaAtual].itens[meio]; // se for igual encontrei
//             return true;
//         }

//         if (reg->chave < moldura[paginaAtual].itens[meio].chave) {
//             esq = meio + 1;
//         }

//         else {
//             dir = meio - 1;
//         }
//     }

//     return -1;
// }