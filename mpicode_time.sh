#!/bin/zsh

#procNs=(1 2 4 8)
#threadNs=(1 2 4 8)

procNs=(1 2 4 8 16 32 64)
threadNs=(1 2 4 8 16 32 64)

for procN in $procNs
do
	for threadN in $threadNs
	do
		
		iterC=10
		sum=0
		for i in $(seq $iterC)
		do
			export OMP_NUM_THREADS=$threadN
			sum=$(mpirun -N $procN bin/main_mpi)
		done
		sum=$((sum/iterC))
		echo "P:${threadN} T:${procN} ${sum}"
	done

done