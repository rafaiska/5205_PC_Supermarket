#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

//CONSTANTES UTILIZADAS:

#define N_CLIENTES 			6400	//total de clientes atendidos pelos caixas

#define TAG_MSG_REQ_INF_FILA		0	//Requisitar informacao de fila
#define TAG_MSG_REQ_ALT_FILA		1	//Requisitar alteracao na fila

//FUNCOES DO PROGRAMA:
void Cliente(int rank, int n_ranks);		//Funcao paralela que adiciona clientes na fila
void Caixa(int rank, int n_ranks);		//Funcao paralela que atende clientes no caixa
int Menor_Fila(int *fila, int n);		//Retorna menor fila de clientes
int Atende_Fila(int *fila, int n, int rank);	//Escolhe uma fila de clientes para atender
int check_Fim(int *fila, int n);		//Checa se as filas estao indicando FIM DO PROGRAMA
int set_Fim(int *fila, int n);			//Seta configuracao das filas para FIM DO PROGRAMA

//VARIAVEIS GLOBAIS:
int N_FILAS;					//Numero de filas

void Cliente(int rank, int n_ranks)
{
	MPI_Status status;
	int msg[2], filas[N_FILAS], escolhida, spawnados = 0;

	while(spawnados < N_CLIENTES/N_FILAS)
	{	
		MPI_Send(&rank, 1, MPI_INT, 0, TAG_MSG_REQ_INF_FILA, MPI_COMM_WORLD);
		MPI_Recv(filas, N_FILAS, MPI_INT, 0, TAG_MSG_REQ_INF_FILA, MPI_COMM_WORLD, &status);

		if(check_Fim(filas, N_FILAS))
			break;

		escolhida = Menor_Fila(filas, N_FILAS);
		msg[0] = escolhida;	//Acrescenta um cliente
		msg[1] = 1;		//na menor fila

		MPI_Send(&msg, 2, MPI_INT, 0, TAG_MSG_REQ_ALT_FILA, MPI_COMM_WORLD);
		++spawnados;
	}
}

void Caixa(int rank, int n_ranks)
{
	MPI_Status status;
	int msg[2], filas[N_FILAS], escolhida, tempo;

	while(1)
	{	
		MPI_Send(&rank, 1, MPI_INT, 0, TAG_MSG_REQ_INF_FILA, MPI_COMM_WORLD);
		MPI_Recv(filas, N_FILAS, MPI_INT, 0, TAG_MSG_REQ_INF_FILA, MPI_COMM_WORLD, &status);

		if(check_Fim(filas, N_FILAS))
			break;

		escolhida = Atende_Fila(filas, N_FILAS, rank);
		for(tempo = 0; tempo < 0xFFFFFF; ++tempo);

		msg[0] = escolhida;	//Atende um cliente
		msg[1] = -1;		//na fila escolhida

		MPI_Send(&msg, 2, MPI_INT, 0, TAG_MSG_REQ_ALT_FILA, MPI_COMM_WORLD);
	}
}

void Monitor(int n_ranks)
{
	MPI_Status status;
	int *filas, clientes_atendidos, msg[2], rank_solicitante, i;

	filas = (int*) malloc(N_FILAS * sizeof(int));

	clientes_atendidos = 0;
	for(i=0; i< N_FILAS; ++i)
		filas[i] = 0;

	while(clientes_atendidos < N_CLIENTES)
	{	
		MPI_Recv(&rank_solicitante, 1, MPI_INT, MPI_ANY_SOURCE, TAG_MSG_REQ_INF_FILA, MPI_COMM_WORLD, &status);
		MPI_Send(filas, N_FILAS, MPI_INT, rank_solicitante, TAG_MSG_REQ_INF_FILA, MPI_COMM_WORLD);

		MPI_Recv(&msg, 2, MPI_INT, MPI_ANY_SOURCE, TAG_MSG_REQ_ALT_FILA, MPI_COMM_WORLD, &status);

		if(msg[0] != -1)
		{
			filas[msg[0]] += msg[1];

			if(msg[1] < 0)
			{
				clientes_atendidos -= msg[1];

				if(clientes_atendidos % 100 == 0)
					printf("%d clientes atendidos até agora\n", clientes_atendidos);
			}
		}
	}

	set_Fim(filas, N_FILAS);

	for(i=1; i<n_ranks; i+=2)
		MPI_Send(filas, N_FILAS, MPI_INT, i, TAG_MSG_REQ_INF_FILA, MPI_COMM_WORLD);	

	free(filas);
}

int Menor_Fila(int *fila, int n)
{
	int i;
	int menor_i = 0;
	int menor_v = fila[0];

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

int Atende_Fila(int *fila, int n, int rank)
{
	int fila_desse_caixa = (rank-1)/2;
	int ret;

	if(fila[fila_desse_caixa] > 0)
		return fila_desse_caixa;

	ret = fila_desse_caixa;
	
	do
	{
		++ret;

		if(ret >= n)
			ret = 0;

		if(fila[ret] > 0)
			break;
	}while(ret != fila_desse_caixa);

	if(ret == fila_desse_caixa)
		return -1;
	else
		return ret;
}

int check_Fim(int *fila, int n)
{
	int i;

	for(i=0; i<n; ++i)
		if(fila[i] == -1)
			return 1;

	return 0;	
}

int set_Fim(int *fila, int n)
{
	int i;

	for(i=0; i<n; ++i)
		fila[i] = -1;
}

int main(int argc, char *argv[])
{
	int rank, size;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(size < 3 || size %2 == 0)
	{
		MPI_Abort(MPI_COMM_WORLD, 1);
		printf("Erro: sao necessarios pelo menos três processos e um numero impar de processos\n");
		exit(1);
	}

	N_FILAS = (size-1)/2;

	if(rank == 0)
	{
		Monitor(size);
	}
	else if(rank % 2 == 0)
	{
		Cliente(rank, size);
	}
	else
	{
		Caixa(rank, size);
	}

	MPI_Finalize();
}
