#!/bin/zsh
array=( 1 2 4 8 16 32 64 )

for i in $array
do
	sum=0
	export OMP_NUM_THREADS=$i
	iterC=10
	for v in $(seq $iterC)
	do
		a=$(bin/main_par)
		sum=$((a+sum))
	done
	sum=$((sum/iterC))
	echo "$i threads:"$sum
done



