#!/bin/zsh

#SBATCH -p bdw2-mixed
#SBATCH -N 3
#SBATCH -o ./report/out/%J_3N_1P_26T
#SBATCH -e ./report/err/%J_3N_1P_26T

./echo_avetime.sh "mpirun -rf rankfile/rankfile3N1P26C -report-bindings bin/main_mpi"
