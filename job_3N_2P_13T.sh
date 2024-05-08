#!/bin/zsh

#SBATCH -p bdw2-mixed
#SBATCH -N 3
#SBATCH -o ./report/out/%J_3N_2P_13T
#SBATCH -e ./report/err/%J_3N_2P_13T

./echo_avetime.sh "mpirun -map-by ppr:1:socket -cpus-per-proc 13  -report-bindings bin/main_mpi"
