TP1 - Pesquisa Externa (BCC203)
Este projeto consiste na implementação e análise experimental de complexidade de quatro métodos de pesquisa externa, desenvolvido para a disciplina BCC203 - Estrutura de Dados II da Universidade Federal de Ouro Preto (UFOP).

🎯 Objetivos
O objetivo principal é realizar um estudo comparativo da complexidade de desempenho dos seguintes métodos:


Acesso Sequencial Indexado;


Árvore Binária de Pesquisa (adequada à memória externa);


Árvore B;


Árvore B*.

🛠️ Especificações Técnicas
Estrutura do Registro
Os registros são armazenados em arquivos binários e possuem a seguinte estrutura:


Chave: Inteiro (chave de pesquisa);


Dado 1: Inteiro longo;


Dado 2: Cadeia de 1000 caracteres;


Dado 3: Cadeia de 5000 caracteres.

Fases do Trabalho

Implementação: Desenvolvimento dos métodos em linguagem C.


Análise Experimental: Coleta de dados sobre o número de transferências (leitura), número de comparações entre chaves e tempo total de execução.
📊 Análise de DesempenhoPara a fase de testes, o sistema realiza a pesquisa automática de 10 chaves distintas para calcular a média dos seguintes quesitos:Transferências entre memória interna e externa;Comparações entre chaves;Tempo de execução (excluindo o tempo de criação do arquivo binário).