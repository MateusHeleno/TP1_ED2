#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "auxiliares.h"
#include "acessoSequencial.h"
#include "arvBinaria.h"
#include "arvB.h"
#include "arvBE.h"

int main(int argc, char *argv[]) {
    if (argc < 5 || argc > 6) {
        printf("O formato é inválido.\nSiga esse formato: ./exe <método> <quantidade> <situação> <chave> [-P]\n");
        exit(1);
    }

    srand(time(NULL));
    int metodo, qnt_registros, situacao, chave, p;

    metodo = atoi(argv[1]);
    if (metodo < 1 || metodo > 4) { // verifica se o valor recebido é válido
        printf("Método deve ter os valores entre 1 e 4\n");
        return 1;
    }

    qnt_registros = atoi(argv[2]);
    situacao = atoi(argv[3]);
    if (situacao < 1 || situacao > 3) { // verifica se o valor recebido é válido
        printf("Situação deve ter os valores entre 1 e 3\n");
        return 1;
    }

    chave = atoi(argv[4]);
    if (argc == 6)
        p = atoi(argv[argc - 1]);

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