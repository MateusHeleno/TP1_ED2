#ifndef AUXILIARES_H
#define AUXILIARES_H

#include <stdio.h>   
#include <stdlib.h>  
#include <string.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int chave;
    long int dado1;
    char dado2[1000];
    char dado3[5000];
} Registro;

typedef struct {
    bool encontrado;
    int transferencias;   
    int comparacoes;      
    double tempo; 
} Metricas;

typedef struct{
    int metodo;
    int qnt_registros;
    int situacao;
    int chave;
    bool p;
} Config;

bool validaEntrada(int argc, char *argv[],Config *config);
void criarArquivo(const char* caminho, int qnt_registros, int situacao)

#endif