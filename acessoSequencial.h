#ifndef ACESSOSEQUENCIAL_H
#define ACESSOSEQUENCIAL_H

#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>
#include <time.h>
#include "auxiliares.h"

#define ITENS_PAGINA 8
#define NUM_PAGINA 50


int criarIndice(FILE *arq, int *vetorIndice, int totalRegistros);

#endif