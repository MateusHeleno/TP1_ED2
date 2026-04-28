#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "acessoSequencial.h"
#include "auxiliares.h"

typedef struct {
    Registro itens[ITENS_PAGINA];
    int numPagina;      // -1 = vazia
    int frequencia;     // pra retirar
    int qntItens;        // para facilitar manueseio da ultima
} Moldura;

void inicializaMoldura(Moldura *moldura) {
    for (int i = 0; i < NUM_MOLDURA; i++) {
        moldura[i].numPagina = -1;
        moldura[i].frequencia = 0;
        moldura[i].qntItens = -1;
    }
}

int buscarMoldura(Moldura *moldura, int numPagina) {  
    for (int i = 0; i < NUM_MOLDURA; i++) {
        if (moldura[i].numPagina == numPagina)
            return i;
    }
    return -1;  
}

int escolherVitima(Moldura *moldura,bool *vazia) {
    for (int i = 0; i < NUM_MOLDURA; i++) {
        if (moldura[i].numPagina == -1){
            *vazia = true;
            return i;
        }
    }

    *vazia = false;
    int vitima = 0;  // faltava ponto e vírgula
    for (int i = 1; i < NUM_MOLDURA; i++) {
        if (moldura[i].frequencia < moldura[vitima].frequencia)
            vitima = i;
    }
    return vitima;
}

int carregarPagina(Moldura *moldura, FILE *arq, int numPagina,
                           Config *config, int totalPaginas, Metricas *metricas) {

    int pagina = buscarMoldura(moldura, numPagina); // confiro se ja tenho

    if (pagina != -1) {
        moldura[pagina].frequencia++;
        return pagina;
    }

    //metricas->acessosDisco++;
    bool vazia;
    pagina = escolherVitima(moldura,&vazia);

    if (vazia) {
        moldura[pagina].frequencia = 1;
    } 
    else 
    {
        int somaFreq = 0; // so entra aqui se tiver tudo cheio
        for (int i = 0; i < NUM_MOLDURA; i++) {
            if (i != pagina)
                somaFreq += moldura[i].frequencia;
        }
    moldura[pagina].frequencia = somaFreq / (NUM_MOLDURA - 1);
    }

    // tratando ultima pagina 
    int qntItens;
    if (numPagina == totalPaginas - 1) { // pq é um vetor e o indice 0
        int resto = config->qnt_registros % ITENS_PAGINA; 
        if(resto == 0)
            qntItens = ITENS_PAGINA;
        else
            qntItens = resto; // se nao tiver o total
    } else {
        qntItens = ITENS_PAGINA;
    }

    long deslocamento = (long)(numPagina * ITENS_PAGINA * sizeof(Registro));
    fseek(arq, deslocamento, SEEK_SET);
    fread(moldura[pagina].itens, sizeof(Registro), qntItens, arq);


    moldura[pagina].qntItens = qntItens;
    moldura[pagina].numPagina = numPagina;
    return pagina;
}

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
    int totalPaginas = totalRegistros / ITENS_PAGINA;

    if (totalRegistros % ITENS_PAGINA != 0)
        totalPaginas++;
    return totalPaginas;
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