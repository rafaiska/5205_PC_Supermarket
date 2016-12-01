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

	while(1)
	{
		fila_escolhida = -1;
		fila_atual = tid;
	
		do
		{
			pthread_mutex_lock(&fila_mut[fila_atual]);
				if(fila[fila_atual] > 0)
				{
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
			if(clientes_a_chegar == 0)
				break;
			else
				continue;
		}
	}

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
