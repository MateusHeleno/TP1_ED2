# TP1 - Pesquisa Externa | Estrutura de Dados II (BCC203)

Este projeto consiste na implementação e análise experimental de complexidade de quatro métodos de pesquisa externa, desenvolvido para a disciplina **BCC203 - Estrutura de Dados II** da **Universidade Federal de Ouro Preto (UFOP)**.

## 🎯 Objetivos
O objetivo principal é realizar um estudo comparativo da complexidade de desempenho dos seguintes métodos de pesquisa externa:
1. **Acesso Sequencial Indexado**
2. **Árvore Binária de Pesquisa** (adequada à memória externa)
3. **Árvore B**
4. **Árvore B***

## 🛠️ Especificações Técnicas

### Estrutura do Registro
Os registros são armazenados em arquivos binários e possuem tamanho fixo para permitir o acesso direto via `fseek`. Cada registro contém:
* **Chave:** Inteiro (chave de pesquisa).
* **Dado 1:** Inteiro longo.
* **Dado 2:** Cadeia de 1.000 caracteres.
* **Dado 3:** Cadeia de 5.000 caracteres.

### Fases do Trabalho
* **Fase 1 (Implementação):** Desenvolvimento dos métodos em linguagem **C**.
* **Fase 2 (Análise Experimental):** Coleta de métricas considerando arquivos de 100 até 1.000.000 de registros, em ordens ascendente, descendente e aleatória.

## 📊 Métricas de Análise
O sistema realiza a pesquisa automática de **10 chaves distintas** para calcular a média dos seguintes quesitos:
* **Transferências (Leitura):** Itens movidos da memória externa para a interna.
* **Comparações:** Número de comparações entre chaves de pesquisa.
* **Tempo de Execução:** Diferença entre o término e o início da execução da pesquisa.

## 🚀 Como Executar

O programa deve ser executado via linha de comando no console conforme o padrão definido:

```bash
./pesquisa <método> <quantidade> <situação> <chave> [-P]