#include "supermercado.h"

int Inicializar()
{
	int i, err_retorno;
	pthread_attr_t attr;

	pthread_mutex_init(&clientes_a_chegar_mutex, NULL);
	clientes_a_chegar = N_CLIENTES;

	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

	for(i=0; i<N_FILAS; ++i)
	{
		sem_init(&fila[i], 0, 0);
	}

	for(i=0; i<N_FILAS; i++)
	{
        	err_retorno = pthread_create(&cliente_thread[i], &attr, Cliente, (void *)i);

		if(err_retorno)
			break;
	}

	for(i=0; i<N_FILAS; i++)
	{
        	err_retorno = pthread_create(&caixa_thread[i], &attr, Caixa, (void *)i);

		if(err_retorno)
			break;
	}

	return err_retorno;
}

void *Cliente(void *arg)
{
	int tid = (int)arg;
	int menor;

	while(1)
	{
		pthread_mutex_lock(&clientes_a_chegar_mutex);

			if(clientes_a_chegar > 0)
			{
				clientes_a_chegar -= 1;
				pthread_mutex_unlock(&clientes_a_chegar_mutex);
			}
			else
			{
				pthread_mutex_unlock(&clientes_a_chegar_mutex);
				break;
			}


		menor = Menor_Fila();
		sem_post(&fila[menor]);
	}

	pthread_exit(NULL);
}

void *Caixa(void *arg)
{
	int tid = (int) arg;
	int i, fila_atual, fila_escolhida;
	int clientes_atendidos = 0;
	uint32_t tempo;
	int npessoas;

	while(1)
	{
		fila_escolhida = -1;
		fila_atual = tid;
	
		do
		{
			sem_getvalue(&fila[fila_atual], &npessoas);
			if(npessoas > 0)
			{
				sem_wait(&fila[fila_atual]);
				for(tempo = 0; tempo < 0xFFFFFF; ++tempo);
				fila_escolhida = fila_atual;
				++clientes_atendidos;
				break;
			}

			++fila_atual;

			if(fila_atual >= N_FILAS)
				fila_atual = 0;

		} while (fila_atual != tid);


		if(fila_escolhida == -1)
		{
			pthread_mutex_lock(&clientes_a_chegar_mutex);
			if(clientes_a_chegar == 0)
			{
				pthread_mutex_unlock(&clientes_a_chegar_mutex);
				break;
			}
			else
			{
				pthread_mutex_unlock(&clientes_a_chegar_mutex);
				continue;
			}
		}
		else if(ECHO && clientes_atendidos % 100 == 0)
		{
			printf("%d clientes atendidos pelo caixa %d ateh agora\n", clientes_atendidos, tid);
		}
	}

	if(ECHO)
		printf("Caixa %d atendeu %d clientes. Fechando agora.\n", tid, clientes_atendidos);

	pthread_exit(NULL);
}

void Encerrar()
{
	int i;
	
	for(i = 0; i < N_FILAS; ++i)
	{
		pthread_join(cliente_thread[i], NULL);
	}

	for(i = 0; i < N_FILAS; ++i)
	{
		pthread_join(caixa_thread[i], NULL);
	}
}

int Menor_Fila()
{
	int i;
	int v;
	int menor_i = 0;
	int menor_v;

	sem_getvalue(&fila[0], &menor_v);

	for(i=1; i < N_FILAS; ++i)
	{
		sem_getvalue(&fila[i], &v);
 
		if(v < menor_v)
		{
			menor_i = i;
			sem_getvalue(&fila[i], &menor_v);
		}
	}

	return menor_i;
}
