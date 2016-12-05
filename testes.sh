for try in {1..3}
do
	/usr/bin/time -v -o ./testes/teste_sequencial_caso_${try}.txt ./bin/supermercado_sequencial.out

	for cores in 3 7 15
	do
		taskset ${cores} /usr/bin/time -v -o ./testes/teste_taskmask${cores}_caso_${try}.txt ./bin/supermercado.out
	done
done
