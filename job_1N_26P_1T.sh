#!/bin/zsh

#SBATCH -p bdw2-mixed
#SBATCH -N 1
#SBATCH -o ./report/out/%J_1N_26P_1T
#SBATCH -e ./report/err/%J_1N_26P_1T


./echo_avetime.sh "mpirun -map-by ppr:13:socket -bind-to core -report-bindings bin/main_mpi"
