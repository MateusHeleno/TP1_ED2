#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>
#include <time.h>

typedef struct {
    int chave;
    long int dado1;
    char dado2[1000];
    char dado3[5000];
} Registro;

typedef struct {
    int transferencias;   
    int comparacoes;      
    double tempo; 
} Metricas;


#endif