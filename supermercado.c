#include "supermercado.h"

int Inicializar()
{
	int i, err_retorno;
	pthread_attr_t attr;

	sem_init(&clientes_a_chegar, 0, N_CLIENTES);

	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);

	for(i=0; i<N_FILAS; ++i)
	{
		fila[i] = 0;
	}

	for(i=0; i<N_FILAS; i++)
	{
		pthread_mutex_init(&fila_mut[i], NULL);

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
		sem_wait(&clientes_a_chegar);
		menor = Menor_Fila();
		pthread_mutex_lock(&fila_mut[menor]);
		fila[menor] += 1;
		pthread_mutex_unlock(&fila_mut[menor]);
	}

	pthread_exit(NULL);
}

void *Caixa(void *arg)
{
	int tid = (int) arg;
	int i, fila_atual, fila_escolhida;
	int clientes_atendidos = 0;
	uint32_t tempo;
	int clientes_a_chegar_i;

	while(1)
	{
		fila_escolhida = -1;
		fila_atual = tid;
	
		do
		{
			pthread_mutex_lock(&fila_mut[fila_atual]);
				if(fila[fila_atual] > 0)
				{
					for(tempo = 0; tempo < 0xFFFFFF; ++tempo);
					fila[fila_atual] -= 1;
					pthread_mutex_unlock(&fila_mut[fila_atual]);
					fila_escolhida = fila_atual;
					++clientes_atendidos;
					break;
				}
			pthread_mutex_unlock(&fila_mut[fila_atual]);

			++fila_atual;

			if(fila_atual >= N_FILAS)
				fila_atual = 0;


		} while (fila_atual != tid);


		if(fila_escolhida == -1)
		{
			sem_getvalue(&clientes_a_chegar, &clientes_a_chegar_i);
			if(clientes_a_chegar_i == 0)
				break;
			else
				continue;
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
		pthread_join(caixa_thread[i], NULL);
	}

	for(i = 0; i < N_FILAS; ++i)
	{
		pthread_cancel(cliente_thread[i]);
	}
}

int Menor_Fila()
{
	int i;
	int menor_v = fila[0];
	int menor_i = 0;

	for(i=1; i < N_FILAS; ++i)
	{
		if(fila[i] < menor_v)
		{
			menor_i = i;
			menor_v = fila[i];
		}
	}

	return menor_i;
}
