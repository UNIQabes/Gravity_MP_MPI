data_compare_bin : grav_data/data_compare_bin_repaired.c
	gcc grav_data/data_compare_bin_repaired.c -o data_compare_bin

test1000: data_compare_bin
	./data_compare_bin grav_data/result1000/outx.double grav_data/n1000/resx.double

main_par:main.c
	gcc -I /opt/homebrew/opt/libomp/include -L /opt/homebrew/opt/libomp/lib -Xpreprocessor -fopenmp -lomp main.c  -o main_par

main:main.c
	gcc -I /opt/homebrew/opt/libomp/include -L /opt/homebrew/opt/libomp/lib main.c  -o main_par

