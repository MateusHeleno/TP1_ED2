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
    if (!arquivo) {
        criarArquivo(nomeArquivo, config.qnt_registros, config.situacao);
        return 0;
    }

    switch(config.metodo){
        // Acesso sequencial indexado
        case 1: {
            Metricas metricas;
            inicializaMetricas(&metricas);

            Registro reg;
            reg.chave = config.chave;

            clock_t comeco, fim;
            comeco = clock();

            int numPaginas = getNumPaginas(config.qnt_registros);
            int *vetorIndices = criaVetor(numPaginas);

            criarIndice(arquivo, vetorIndices, config.qnt_registros);
            
            fim = clock();
            metricas.tempo = (double) (fim - comeco) / CLOCKS_PER_SEC;

            Moldura moldura[NUM_MOLDURA];
            inicializaMoldura(moldura);
            
            bool encontrado = acessoSequencialIndexado(vetorIndices, arquivo, &reg, numPaginas,moldura,&config, &metricas);
            printRegistro(reg, metricas, encontrado, nomeArquivo);

            destroiVetor(vetorIndices);

            break;
        }

        // Árvore Binária de Pesquisa
        case 2:
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