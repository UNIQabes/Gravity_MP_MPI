#!/bin/zsh
#SBATCH -p bdw2
#SBATCH -N 1
#SBATCH -J test_openmpi
#SBATCH -o ./report/stdout.%J
#SBATCH -e ./report/stderr.%J 
mpirun -N 1 -n 1 -cpus-per-proc 5 bin/main_mpi