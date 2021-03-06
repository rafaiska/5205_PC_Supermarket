#ifndef SUPERMERCADO_H_
#define SUPERMERCADO_H_

#define N_FILAS 8		//total de filas para os caixas
#define N_CLIENTES 6400		//total de clientes atendidos pelos caixas

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdint.h>

//Variavel para habilitar/desabilitar informacoes impressas na
//saida padrao
int ECHO;

//Numero de clientes em cada fila, representado por semaforos
sem_t fila[N_FILAS];

//Threads geradores de clientes e threads de caixa
pthread_t cliente_thread[N_FILAS];
pthread_t caixa_thread[N_FILAS];

//Numero de clientes a chegar nas filas, bem como um mutex para
//sincronizacao
int clientes_a_chegar;
pthread_mutex_t clientes_a_chegar_mutex;

int Inicializar();		//Inicializa componentes do problema
void *Cliente(void *arg);	//Funcao paralela que adiciona clientes na fila
void *Caixa(void *arg);		//Funcao paralela que atende clientes no caixa
void Encerrar();		//Desativa componentes do problema
int Menor_Fila();		//Retorna menor fila de clientes

#endif
