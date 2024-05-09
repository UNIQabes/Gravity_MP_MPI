#!/bin/zsh

#SBATCH -p bdw2-mixed
#SBATCH -N 1
#SBATCH -w ppx2-01
#SBATCH -o ./report/out/%J_1N_1P_1T
#SBATCH -e ./report/err/%J_1N_1P_1T

./echo_avetime.sh "mpirun -map-by ppr:1:node -cpus-per-proc 1 -report-bindings bin/main_mpi"
