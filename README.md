# Problema do Supermercado
Essa aplicação foi desenvolvida como primeiro trabalho da disciplina "5205 - Programação Concorrente", ministrada pelo Departamento de Informática da Universidade Estadual de Maringá.

Professor: Rodolfo Miranda Pereira

Alunos:
	Lucas Marçal Surmani,	RA 84447
	Rafael Cortez Sanchez,	RA 82357


# Arquivos
Os códigos fonte estão no diretório raiz desse pacote, juntamente com esse arquivo README. Nesse diretório também se encontra um relatório sobre o projeto, de nome "relatorio.pdf", no qual estão detalhados o problema do supermercado e como foi modelada uma solução paralela para ele. Os cabeçalhos utilizados se encontram no diretório "include".

Os arquivos da solução paralela são: "main.c", "supermercado.c", "supermercado.h". A solução sequencial está toda no arquivo "supermercado_sequencial.out".

O arquivo testes.sh é um script de comandos para a realização de testes sobre os executáveis. Os testes citados no relatório foram gerados através desse script, e se encontram dentro do diretório "testes", em formato de arquivos de texto.


# Instruções para Compilação
Um Makefile deve ser distribuído junto aos arquivos de código fonte. No diretório em que estão os arquivos, pode-se executar os seguintes comandos para a compilação:

$make supermercado.out : Compila a solução paralela para o problema do supermercado

$make supermercado_sequencial.out: Compila a solução sequencial para o problema do supermercado


# Instruções para Execução
Caso não ocorram erros na compilação, dois executáveis serão criados no diretório "bin": "supermercado.out" e "supermercado_sequencial.out". Eles representam, respectivamente, a solução paralela e a solução sequencial do problema.

Ambos binários podem ser executados com os seguintes parâmetros:

"--echo": Habilita informações impressas na saída padrão em tempo de execução. Se habilitado esse parâmetro em uma execução pelo terminal, esse mostrará quando um múltiplo de 100 clientes for atendido em um dos caixas. Ao final da execução, também mostra quantos clientes foram atendidos em cada caixa.
