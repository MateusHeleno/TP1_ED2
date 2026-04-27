#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool>

#include "auxiliares.h"
#include "acessoSequencial.h"
#include "arvBinaria.h"
#include "arvB.h"
#include "arvBE.h"

int main(int argc, char *argv[]) {
    
    srand(time(NULL));
    
    Config config;
    if(!validaEntrada(argc,argv,&config))
        return 0;


    
    // nomenclatura dos arquivos: arquivos/bin_{qnt-registros}_{situacao}
    // exemplo: arquivo/bin_100_situacao1 -> arquivo binário com 100 registros e ordenação ascendente
    char nomeArquivo[64];
    sprintf(nomeArquivo, "arquivos/bin_%d_situacao%d.bin", qnt_registros, situacao);

    FILE* arquivo = fopen(nomeArquivo, "rb");
    if (!arquivo) {
        criarArquivo(nomeArquivo, qnt_registros, situacao);
        return 0;
    }

    fclose(arquivo);
    return 0;
}