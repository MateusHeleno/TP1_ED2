#ifndef ACESSOSEQUENCIAL_H
#define ACESSOSEQUENCIAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "auxiliares.h"

#define ITENS_PAGINA 8
#define NUM_MOLDURA 50


void inicializaMoldura(Moldura *moldura);
int buscarMoldura(Moldura *moldura, int numPagina);
int escolherVitima(Moldura *moldura,bool *vazia);
int carregarPagina(Moldura *moldura, FILE *arq, int numPagina,
                           int totalPaginas, Config *config, Metricas *metricas);

void criarIndice(FILE *arq, int *vetorIndice, int totalRegistros);
int getNumPaginas(int totalRegistros);
bool acessoSequencialIndexado(int *vetorIndice, FILE *arq, Registro *reg, int totalPaginas,Moldura *moldura, Config *config,Metricas *metricas);
#endif