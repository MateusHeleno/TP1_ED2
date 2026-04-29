#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "acessoSequencial.h"
#include "auxiliares.h"


Moldura* inicializaMoldura() {
    Moldura *moldura = (Moldura *) malloc(sizeof(Moldura) * NUM_MOLDURA);

    if (!moldura){
        printf("Erro na alocação de memórias");
        exit(1);
    }

    for (int i = 0; i < NUM_MOLDURA; i++) {
        moldura[i].numPagina = -1;
        moldura[i].frequencia = 0;
        moldura[i].qntItens = -1;
    }
    return moldura;
}

void destroiMoldura(Moldura *moldura){
    free(moldura);
}

int buscarMoldura(Moldura *moldura, int numPagina) {
    // for (int i = 0; i < NUM_MOLDURA; i++) {
    //     if (moldura[i].numPagina == numPagina)
    //         return i; // conferir se ja tenho a pagina corretas nas minha molduras
    // }
    // return -1;
    
    return buscaBinaria(moldura,NUM_MOLDURA,numPagina);
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

    int pagina = buscarMoldura(moldura, numPagina); // confiro se ja tenho
    int qntItensPag = getNumItensPagina(config);

    if (pagina != -1) {
        moldura[pagina].frequencia++;
        return pagina; // se eu ja tiver retorno a pagina
    }

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
        int resto = config->qnt_registros % qntItensPag;
        if(resto == 0)
            qntItens = qntItensPag; // se a pagina tiver completa
        else
            qntItens = resto; // se nao tiver o total
    } else {
        qntItens = qntItensPag; // se nao for a ultima pagina
    }

    long deslocamento = (long)(numPagina * qntItensPag * sizeof(Registro));
    fseek(arq, deslocamento, SEEK_SET);
    fread(moldura[pagina].itens, sizeof(Registro), qntItens, arq);

    moldura[pagina].qntItens = qntItens;
    moldura[pagina].numPagina = numPagina;
    return pagina;
}

void criarIndice(FILE *arq, int *vetorIndice, Config* config) {
    int numPaginas = getNumPaginas(config); // pega o total de paginas do arquivo

    int chave;
    for (int i = 0; i < numPaginas; i++) {

        long deslocamento = (long) (i * ITENS_PAGINA * sizeof(Registro)); // pula de pagina em pagina

        fseek(arq, deslocamento, SEEK_SET);

        // Lê apenas o campo 'chave' do primeiro registro da página
        if (fread(&chave, sizeof(int), 1, arq) == 1)
            vetorIndice[i] = chave;

    }
}

int getNumPaginas(Config* config) {
    int qntItensPag = getNumItensPagina(config);
    int totalPaginas = config->qnt_registros / qntItensPag;

    if (config->qnt_registros % qntItensPag != 0)
        totalPaginas++;
    return totalPaginas;
}

bool acessoSequencialIndexado(int *vetorIndice, FILE *arq, Registro *reg, int totalPaginas, Moldura *moldura, Config *config, Metricas *metricas) {
    int i = 0;
    int paginaAtual;

    // busca ascedente
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

    // busca descendente
    else if (config->situacao == 2) {
       while (i < totalPaginas && vetorIndice[i] >= reg->chave) {
            metricas->comparacoes++;
            i++;
        } // achar a posicao

        if (i == 0) // conferir se é 0
            return false;

        int numPaginaAlvo = i - 1; // pa vai ate a chave maior
        paginaAtual = carregarPagina(moldura, arq, numPaginaAlvo,
                                         totalPaginas, config, metricas); // coloca o indice do vet da pagina que queremos
    }

    return buscaBinariaGeral(moldura, paginaAtual, reg,metricas,*config);
}

// bool buscaBinariaGeral(Moldura *moldura, int paginaAtual, Registro *reg, Metricas *metricas, Config *config) {
//     int esq = 0;
//     int dir = moldura[paginaAtual].qntItens - 1; // pq é um vetor


//     // busca ascedente
//     if (config->situacao == 1) {
//         while (esq <= dir) {
//             int meio = (esq + dir) / 2; // pega o meio
//             metricas->comparacoes++;

//             if (moldura[paginaAtual].itens[meio].chave == reg->chave) {
//                 *reg = moldura[paginaAtual].itens[meio]; // se for igual encontrei
//                 return true;
//             }

//             if (reg->chave < moldura[paginaAtual].itens[meio].chave) // decide qual posicao deslocar
//                 dir = meio - 1;
//             else
//                 esq = meio + 1;
//         }
//     }

//     // busca descente
//     else if (config->situacao == 2) {
//         while (esq <= dir) {
//             int meio = (esq + dir) / 2; // pega o meio
//             metricas->comparacoes++;

//             if (moldura[paginaAtual].itens[meio].chave == reg->chave) {
//                 *reg = moldura[paginaAtual].itens[meio]; // se for igual encontrei
//                 return true;
//             }

//             if (reg->chave > moldura[paginaAtual].itens[meio].chave) // decide qual posicao deslocar
//                 dir = meio - 1;
//             else
//                 esq = meio + 1;
//         }
//     }

//     return false;
// }

bool buscaBinariaGeral(Moldura *moldura, int paginaAtual, Registro *reg,Metricas *metricas,Config config) {
    int esq = 0;
    int dir = moldura[paginaAtual].qntItens - 1; // pq é um vetor

    while (esq <= dir) {

        int meio = esq + (dir - esq) / 2; // pega o meio
        
        int chaveMeio = moldura[paginaAtual].itens[meio].chave;
        
        metricas->comparacoes++;

        if (chaveMeio ==  reg->chave) {
            *reg = moldura[paginaAtual].itens[meio]; // se for igual encontrei
            return true;
        }

        
        bool irParaEsquerda;
        if (config.situacao == 1) {
            // busca ascedente, eu vou pra esquerda se for menor
            irParaEsquerda = (reg->chave < chaveMeio);
        } else if (config.situacao == 2){
            // busca ascedente, eu vou pra esquerda se for menor
            irParaEsquerda = (reg->chave > chaveMeio);
        }
        else
            return false;


        if (irParaEsquerda) {
            dir = meio - 1;
        }

        else {
            esq = meio + 1;
        }
    }

    return false;
}

int buscaBinaria(Moldura *moldura, int tamanho, int alvo) {
    int esq = 0;
    int dir = tamanho - 1;

    while (esq <= dir) {
        
        int meio = esq + (dir - esq) / 2; // pega o meio

        
        if (moldura[meio].numPagina == alvo) { // verifica se é ele
            return meio;
        }

        if (moldura[meio].numPagina < alvo) {
            esq = meio + 1; // desloca pra dir
        } 
        else {
            dir = meio - 1; // desloca pra esq
        }
    }

    // Elemento não está presente
    return -1;
}

int getNumItensPagina(Config* config) {
    if (config->qnt_registros == 100)
        return 16;
    else if (config->qnt_registros == 1000)
        return 32;
    else if (config->qnt_registros == 10000)
        return 128;
    else
        return 256;
}