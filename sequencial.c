#include "supermercado.h"
#include <string.h>
#include <stdlib.h>

int clientes_a_chegar_i;

int Cliente_Seq(int tid)
{
	int i, menor;


	if(clientes_a_chegar_i > 0)
	{
		clientes_a_chegar_i -= 1;
		menor = Menor_Fila();
		fila[menor] += 1;
		return 0;
	}

	return 1;
}

int Caixa_Seq(int tid)
{
	int i, fila_atual, fila_escolhida;
	uint32_t tempo;

	fila_escolhida = -1;
	fila_atual = tid;	

	do
	{
		if(fila[fila_atual] > 0)
		{
			for(tempo = 0; tempo < 0xFFFFFF; ++tempo);
			fila[fila_atual] -= 1;
			fila_escolhida = fila_atual;
			break;
		}

			++fila_atual;

			if(fila_atual >= N_FILAS)
				fila_atual = 0;

	} while (fila_atual != tid);

	return fila_escolhida;
}

int main(int argc, char *argv[])
{
	int i;
	int atendeu, atendidos=0;
	int atendidos_no_caixa[N_FILAS];

	clientes_a_chegar_i = N_CLIENTES;
	ECHO = 0;

	for(int i=0; i<argc; ++i)
	{
		if(!strcmp(argv[i],"--echo"))
			ECHO = 1;
	//	else if(!strcmp(argv[i],"-f") && argc > i+1)
	//		N_FILAS = atoi(argv[i+1]);
	}


	for(i=0; i<N_FILAS; ++i)
	{
		fila[i] = 0;
		atendidos_no_caixa[i] = 0;
	}

	while(1)
	{
		for(i=0; i<N_FILAS; ++i)
			Cliente_Seq(i);

		for(i=0; i<N_FILAS; ++i)
		{
			atendeu = Caixa_Seq(i);
			if(atendeu == -1)
			{
				break;
			}
			else
			{
				++atendidos;
				++(atendidos_no_caixa[i]);
				if(ECHO && atendidos_no_caixa[i] % 100 == 0)
					printf("%d clientes atendidos pelo caixa %d ateh agora\n", atendidos_no_caixa[i], i);
			}
		}

		if(atendeu == -1 && clientes_a_chegar_i == 0)
			break;
	}

	if(ECHO)
		printf("Clientes atendidos: %d\n", atendidos);

	return 0;
}
