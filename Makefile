DIRINCLUDE=./include
DIROBJETO=./obj
CC=gcc
CFLAGS=-I$(DIRINCLUDE) -g -pthread

_DEPEND = supermercado.h
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJETO = main.o supermercado.o
OBJETO = $(patsubst %,$(DIROBJETO)/%,$(_OBJETO))

$(DIROBJETO)/%.o: %.c $(DEPEND)
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

supermercado.out: $(OBJETO)
	@mkdir -p bin
	$(CC) -o ./bin/$@ $^ $(CFLAGS)

