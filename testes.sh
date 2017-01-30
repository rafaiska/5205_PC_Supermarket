mkdir -p ./testes

for icaso in {1..20}
do
	taskset -c 0 /usr/bin/time -v -o ./testes/teste_sequencial_caso_${icaso}.txt ./bin/supermercado_sequencial.out
	taskset -c 0,1 /usr/bin/time -v -o ./testes/teste_2nucleos_caso_${icaso}.txt ./bin/supermercado.out
	taskset -c 0,1,2 /usr/bin/time -v -o ./testes/teste_3nucleos_caso_${icaso}.txt ./bin/supermercado.out
	taskset -c 0,1,2,3 /usr/bin/time -v -o ./testes/teste_4nucleos_caso_${icaso}.txt ./bin/supermercado.out
done
