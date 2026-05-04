#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "auxiliares.h"
#include "acessoSequencial.h"
#include "arvBinaria.h"
#include "arvB.h"
#include "arvBE.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));

    Config config;
    if(!validaEntrada(argc,argv, &config))
        return 1;

    // nomenclatura dos arquivos: arquivos/bin_{qnt-registros}_{situacao}
    char nomeArquivo[64];
    sprintf(nomeArquivo, "arquivos/bin_%d_situacao%d.bin", config.qnt_registros, config.situacao);

    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) { // se o arquivo não existe, cria ele
        criarArquivo(nomeArquivo, config.qnt_registros, config.situacao);
        printf("A criação foi um sucesso, digite o comando novamente da pesquisa e terá o resultado\n");
        return 0;
    }

    switch(config.metodo){
        // Acesso sequencial indexado
        case 1: {

            if(config.situacao == 3){
                printf("Esse metodo nao aceita esse tipo de ordenação.\n");
                return 0;
            }

            Metricas metricas;
            inicializaMetricas(&metricas);

            Registro reg;
            reg.chave = config.chave;

            clock_t comeco, fim;
            comeco = clock();

            int numPaginas = getNumPaginas(&config);
            int *vetorIndices = criaVetor(numPaginas);

            criarIndice(arquivo, vetorIndices, &config, &metricas);

            Moldura *moldura = inicializaMoldura();
            bool encontrado = acessoSequencialIndexado(vetorIndices, arquivo, &reg, numPaginas, moldura, &config, &metricas);

            fim = clock();
            metricas.tempo = (double) (fim - comeco) / CLOCKS_PER_SEC;

            printRegistro(reg, metricas, encontrado, nomeArquivo, config);

            destroiVetor(vetorIndices);
            destroiMoldura(moldura);

            break;
        }

        // Árvore Binária de Pesquisa
        case 2:
            if (config.metodo != 3) {
                printf("Esse método não aceita esse tipo de ordenação.\n");
                return 0;
            }

            Metricas metricas;
            inicializaMetricas(&metricas);

            Registro reg;
            reg.chave = config.chave;

            char nomeArq[64];
            sprintf(nomeArq, "arvBin/bin_%dreg.bin", config.qnt_registros);
            FILE* arqArv = fopen(nomeArq, "rb");
            if (!arqArv) {
                criarArvBinaria(&config, arquivo, nomeArq);
                printf("A criação foi um sucesso, digite o comando novamente da pesquisa e terá o resultado\n");
                return 0;
            }

            

            break;

        // Árvore B
        case 3:
            break;

        // Arvore B*
        case 4:
            break;
    }

    fclose(arquivo);
    return 0;
}