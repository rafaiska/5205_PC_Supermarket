#ifndef SUPERMERCADO_H_
#define SUPERMERCADO_H_

#define N_FILAS 15			//numero de filas (e de caixas)
#define N_CLIENTES 50000000		//total de clientes atendidos pelos caixas

#include <pthread.h>
#include <stdio.h>

//Vetor que armazena o numero de clientes em cada fila,
//bem como os mutexes associados a essas variaveis
int fila[N_FILAS];
pthread_mutex_t fila_mut[N_FILAS];

//Threads geradores de clientes e threads de caixa
pthread_t cliente_thread[N_FILAS];
pthread_t caixa_thread[N_FILAS];

//Numero de clientes a chegar nas filas. Um mutex para
//sincronizar o acesso a memoria compartilhada
int clientes_a_chegar;
pthread_mutex_t clientes_a_chegar_mutex;

int Inicializar();		//Inicializa componentes do problema
void *Cliente(void *arg);	//Funcao paralela que adiciona clientes na fila
void *Caixa(void *arg);		//Funcao paralela que atende clientes no caixa
void Encerrar();		//Desativa componentes do problema
int Menor_Fila();		//Retorna menor fila de clientes

#endif
