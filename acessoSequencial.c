#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "acessoSequencial.h"
#include "auxiliares.h"


void inicializaMoldura(Moldura *moldura) {
    for (int i = 0; i < NUM_MOLDURA; i++) {
        moldura[i].numPagina = -1;
        moldura[i].frequencia = 0;
        moldura[i].qntItens = -1;
    }
}

int buscarMoldura(Moldura *moldura, int numPagina, Metricas *metricas) {
    for (int i = 0; i < NUM_MOLDURA; i++) {
        if (moldura[i].numPagina == numPagina)
            return i; // conferir se ja tenho a pagina corretas nas minha molduras

    }
    return -1;
}

int escolherVitima(Moldura *moldura,bool *vazia) {
    for (int i = 0; i < NUM_MOLDURA; i++) {
        if (moldura[i].numPagina == -1){
            *vazia = true; // se tiver ocupado uma vazia ela avisa
            return i; // retorna o indice da casa vazia
        }
    }

    *vazia = false; // se tiver ocupado
    int vitima = 0;
    for (int i = 1; i < NUM_MOLDURA; i++) {
        if (moldura[i].frequencia < moldura[vitima].frequencia)
            vitima = i; // retorna o indice da casa que perde na frequencia, mas varrendo todas
    }
    return vitima;
}

int carregarPagina(Moldura *moldura, FILE *arq, int numPagina,
                           int totalPaginas, Config *config, Metricas *metricas) {

    int pagina = buscarMoldura(moldura, numPagina, metricas); // confiro se ja tenho

    if (pagina != -1) {
        moldura[pagina].frequencia++;
        return pagina; // se eu ja tiver retorno a pagina
    }

    //metricas->acessosDisco++;
    bool vazia;
    pagina = escolherVitima(moldura,&vazia);

    if (vazia)
        moldura[pagina].frequencia = 1; // ate preencher todas, inicializa freqeuencia em 1

    else {
        int somaFreq = 0; // so entra aqui se tiver tudo cheio
        for (int i = 0; i < NUM_MOLDURA; i++) {
            if (i != pagina)
                somaFreq += moldura[i].frequencia;
        }
        moldura[pagina].frequencia = somaFreq / (NUM_MOLDURA - 1); // incializa frequencia, com a media
    }

    // tratando ultima pagina
    int qntItens;
    if (numPagina == totalPaginas - 1) { // queremos a ultima pagina, e é um vetor ai tem q retirar um
        int resto = config->qnt_registros % ITENS_PAGINA;
        if(resto == 0)
            qntItens = ITENS_PAGINA; // se a pagina tiver completa
        else
            qntItens = resto; // se nao tiver o total
    } else {
        qntItens = ITENS_PAGINA; // se nao for a ultima pagina
    }

    long deslocamento = (long)(numPagina * ITENS_PAGINA * sizeof(Registro));
    fseek(arq, deslocamento, SEEK_SET);
    fread(moldura[pagina].itens, sizeof(Registro), qntItens, arq);


    moldura[pagina].qntItens = qntItens;
    moldura[pagina].numPagina = numPagina;
    return pagina;
}

void criarIndice(FILE *arq, int *vetorIndice, int totalRegistros) {
    int numPaginas = getNumPaginas(totalRegistros); // pega o total de paginas do arquivo

    int chave;
    for (int i = 0; i < numPaginas; i++) {

        long deslocamento = (long) (i * ITENS_PAGINA * sizeof(Registro)); // pula de pagina em pagina

        fseek(arq, deslocamento, SEEK_SET);

        // Lê apenas o campo 'chave' do primeiro registro da página
        if (fread(&chave, sizeof(int), 1, arq) == 1)
            vetorIndice[i] = chave;


        // no caso eu aproveitaria o fread de cima e preenchrias as primeiras moldura
        // ja que ja estaria acessando o arquivo
        // se meus intens de pagina e moldura fossem dinamico
        // if (i < NUM_MOLDURA) {
        //     fseek(arq, deslocamento, SEEK_SET);  // volta pro início da página
        //     carregarPagina(moldura, arq, i, config, totalPaginas, NULL);
        // }
    }
}

int getNumPaginas(int totalRegistros) {
    int totalPaginas = totalRegistros / ITENS_PAGINA;

    if (totalRegistros % ITENS_PAGINA != 0)
        totalPaginas++;
    return totalPaginas;
}

bool acessoSequencialIndexado(int *vetorIndice, FILE *arq, Registro *reg, int totalPaginas, Moldura *moldura, Config *config, Metricas *metricas) {
    int i = 0;
    int paginaAtual;

    if (config->situacao == 1) {
        while (i < totalPaginas && vetorIndice[i] <= reg->chave) {
            metricas->comparacoes++;
            i++;
        } // achar a posicao

        if (i == 0) // conferir se é 0
            return false;

        int numPaginaAlvo = i - 1; // pa vai ate a chave maior
        paginaAtual = carregarPagina(moldura, arq, numPaginaAlvo,
                                         totalPaginas, config, metricas); // coloca o indice do vet da pagina que queremos
    }
    else if (config->situacao == 2) {
       while (i < totalPaginas && vetorIndice[i] >= reg->chave) {
            metricas->comparacoes++;
            i++;
        } // achar a posicao

        int numPaginaAlvo = i - 1; // pa vai ate a chave maior
        paginaAtual = carregarPagina(moldura, arq, numPaginaAlvo,
                                         totalPaginas, config, metricas); // coloca o indice do vet da pagina que queremos
    }

    return buscaBinaria(moldura, paginaAtual, reg, metricas, config);
}

bool buscaBinaria(Moldura *moldura, int paginaAtual, Registro *reg, Metricas *metricas, Config *config) {
    int esq = 0;
    int dir = moldura[paginaAtual].qntItens - 1; // pq é um vetor

    if (config->situacao == 1) {
        while (esq <= dir) {
            int meio = (esq + dir) / 2; // pega o meio
            metricas->comparacoes++;

            if (moldura[paginaAtual].itens[meio].chave == reg->chave) {
                *reg = moldura[paginaAtual].itens[meio]; // se for igual encontrei
                return true;
            }

            if (reg->chave < moldura[paginaAtual].itens[meio].chave) // decide qual posicao deslocar
                dir = meio - 1;
            else
                esq = meio + 1;
        }
    }

    else if (config->situacao == 2) {
        while (esq <= dir) {
            int meio = (esq + dir) / 2; // pega o meio
            metricas->comparacoes++;

            if (moldura[paginaAtual].itens[meio].chave == reg->chave) {
                *reg = moldura[paginaAtual].itens[meio]; // se for igual encontrei
                return true;
            }

            if (reg->chave > moldura[paginaAtual].itens[meio].chave) // decide qual posicao deslocar
                dir = meio - 1;
            else
                esq = meio + 1;
        }
    }

    return false;
}
