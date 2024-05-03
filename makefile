bin/data_compare_bin : grav_data/data_compare_bin_repaired.c
	gcc grav_data/data_compare_bin_repaired.c -o bin/data_compare_bin

test1000: bin/data_compare_bin
	bin/data_compare_bin grav_data/result1000/outx.double grav_data/n1000/resx.double

bin/main_par:main.c second.c
	gcc -I /opt/homebrew/opt/libomp/include -L /opt/homebrew/opt/libomp/lib -Xpreprocessor -fopenmp -lm -lgomp main.c  -o bin/main_par

bin/main:main.c second.c
	gcc -I /opt/homebrew/opt/libomp/include -L /opt/homebrew/opt/libomp/lib  -lm main.c  -o bin/main

bin/main_mpi:main_mpi.c second.c
	mpicc -I /opt/homebrew/opt/libomp/include -L /opt/homebrew/opt/libomp/lib -Xpreprocessor -fopenmp -lm -lgomp main_mpi.c -o bin/main_mpi
	
bin/print_assign_hosts:print_assign_hosts.c
	mpicc print_assign_hosts.c -lgomp -Xpreprocessor -fopenmp  -o bin/print_assign_hosts 