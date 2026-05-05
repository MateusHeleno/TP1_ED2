#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "auxiliares.h"
#include "arvBinaria.h"

void criarArvBinaria(Config *config, FILE* arqOriginal, const char *arqArvBin, Metricas *metricas) {
    FILE* arvBin = fopen(arqArvBin, "wb+");
    if (!arvBin) {
        printf("Erro ao abrir/criar arquivo da árvore binária.\n");
        return;
    }

    No novo;
    novo.esq = -1;
    novo.dir = -1;

    fread(&novo.reg, sizeof(Registro), 1, arqOriginal); // lê o primeiro registro do arquivo (a raiz) e salva na árvore binária
    fwrite(&novo, sizeof(No), 1, arvBin); // agora o arquivo tem: registro (raiz), -1, -1

    // aqui a raiz da árvore já está salva e já lemos o primeiro registro do arquivo original
    // temos agora que fazer um loop pelo arquivo e salvando as chaves na esquerda ou direita de acordo com a raiz
    // se o valor de esq ou dir for -1 significa que nao tem filho ainda, entao checamos essa condição

    // enquanto não chegar ao final do arquivo => adiciona os nós
    Registro reg;
    while(fread(&reg, sizeof(Registro), 1, arqOriginal) == 1)
        insereNo(arvBin, reg, metricas);
}

void insereNo(FILE* arvBin, Registro reg, Metricas *metricas) {
    No novo;
    inicializaNo(&novo, reg); // o novo nó recebe o registro atual do arquivo e será posicionado de acordo com sua chave e a de seu nó pai

    No temp;
    inicializaNo(&temp, reg);
    long noPai = -1, noAtual = 0;
    while (noAtual != -1) { // procurar até achar um nodo que nao tem filho
        noPai = noAtual;
        fseek(arvBin, noAtual, SEEK_SET);
        fread(&temp, sizeof(No), 1, arvBin);
        metricas->transferencias++;

        if (reg.chave < temp.reg.chave)
            noAtual = temp.esq;
        else
            noAtual = temp.dir;
    }

    // adiciona o novo no
    fseek(arvBin, 0, SEEK_END);
    long posNovoNo = ftell(arvBin);
    fwrite(&novo, sizeof(No), 1, arvBin);

    // atualiza o ponteiro do nó pai
    fseek(arvBin, noPai, SEEK_SET);
    fread(&temp, sizeof(No), 1, arvBin);
    if (reg.chave < temp.reg.chave)
        temp.esq = posNovoNo;
    else
        temp.dir = posNovoNo;

    fseek(arvBin, noPai, SEEK_SET);
    fwrite(&temp, sizeof(No), 1, arvBin);
}

void inicializaNo(No* no, Registro reg) {
    no->dir = -1;
    no->esq = -1;
    no->reg = reg;
}

bool pesquisaArvoreBinaria(FILE* arvBin, Registro *reg, Metricas *metricas) {
    No alvo;
    long atual = 0;

    while (atual != -1) {
        metricas->transferencias++;
        fseek(arvBin, atual, SEEK_SET); // na primeira iteração vai pra raiz do arquivo, nas próximas se movimenta de acordo com o valor do registro.
        if (fread(&alvo, sizeof(No), 1, arvBin) != 1)
            break;

        metricas->comparacoes++;
        if (reg->chave == alvo.reg.chave) {
            *reg = alvo.reg;
            return true;
        }

        else if (reg->chave < alvo.reg.chave)
            atual = alvo.esq;
        else
            atual = alvo.dir;
    }

    return false;
}