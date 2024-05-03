#!/bin/zsh

#procNs=(1 2 4 8)
#threadNs=(1 2 4 8)


threadNs=(1 2 4 7 12 14 16 18 20 22 24 26 28 42 56)


for threadN in $threadNs
do		
	iterC=10
	sum=0
	for i in $(seq $iterC)
	do
		export OMP_NUM_THREADS=$threadN
		a=$($1)
		sum=$((a+sum))
	done
	sum=$((sum/iterC))
	echo "\"${threadN}\", \"${sum}\""
done
