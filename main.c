#include "supermercado.h"
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
	int i;

	ECHO = 0;

	for(int i=0; i<argc; ++i)
	{
		if(!strcmp(argv[i],"--echo"))
			ECHO = 1;
	//	else if(!strcmp(argv[i],"-f") && argc > i+1)
	//		N_FILAS = atoi(argv[i+1]);
	}

	Inicializar();
	Encerrar();

	return 0;
}
