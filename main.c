#include "supermercado.h"

int main()
{
	int i;

	Inicializar();
	Encerrar();

	for(i = 0; i<N_FILAS; ++i)
		printf("Fila[%d] = %d\n", i, fila[i]);
	printf("Clientes a chegar: %d\n", clientes_a_chegar);

	return 0;
}
