#!/bin/zsh
array=( 1 2 4 8 16 32 64 )

for i in $array
do
	sum=0
	OMP_NUM_THREADS=$i
	for v in $(seq 3)
	do
		a=$(bin/main_par)
		sum=$((a+sum))
	done
	sum=$((sum/3))
	echo "$i thread:"$sum
done



