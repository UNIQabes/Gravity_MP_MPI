bin/data_compare_bin : grav_data/data_compare_bin_repaired.c
	gcc grav_data/data_compare_bin_repaired.c -o bin/data_compare_bin

test1000: bin/data_compare_bin
	bin/data_compare_bin grav_data/result1000/outx.double grav_data/n1000/resx.double

bin/main_par:main.c
	gcc -I /opt/homebrew/opt/libomp/include -L /opt/homebrew/opt/libomp/lib -Xpreprocessor -fopenmp -lomp main.c  -o bin/main_par

bin/main:main.c
	gcc -I /opt/homebrew/opt/libomp/include -L /opt/homebrew/opt/libomp/lib main.c  -o bin/main

bin/main_mpi:main_mpi.c second.c
	mpicc -I /opt/homebrew/opt/libomp/include -L /opt/homebrew/opt/libomp/lib -Xpreprocessor -fopenmp -lomp main_mpi.c -o bin/main_mpi