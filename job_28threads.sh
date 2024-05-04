#!/bin/zsh

#SBATCH -p bdw2-mixed
#SBATCH -N 1
#SBATCH -n 1
#SBATCH -c 28
#SBATCH -o ./report/test/out%J_28Threads
#SBATCH -e ./report/test/err%J_28Threads


#export OMP_NUM_THREADS=28
mpirun -map-by node -cpus-per-proc 28 -display-devel-map bin/print_assign_hosts
