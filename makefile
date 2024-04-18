data_compare_bin : grav_data/data_compare_bin_repaired.c
	gcc grav_data/data_compare_bin_repaired.c -o data_compare_bin

test1000: data_compare_bin
	./data_compare_bin grav_data/result1000/outx.double grav_data/n1000/resx.double