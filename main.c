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

    int metodo, qnt_registros, situacao, chave, p;

    metodo = argv[1];
    if (metodo < 1 || metodo > 4) { // verifica se o valor recebido é válido
        printf("Método deve ter os valores entre 1 e 4\n");
        return 1;
    }

    qnt_registros = argv[2];
    situacao = argv[3];
    if (situacao < 1 || situacao > 3) { // verifica se o valor recebido é válido
        printf("Situação deve ter os valores entre 1 e 3\n");
        return 1;
    }

    chave = argv[4];
    if (argc == 6)
        p = argv[argc - 1];

    return 0;
}