#!/bin/zsh

#SBATCH -p bdw2-mixed
#SBATCH -N 1
#SBATCH -w ppx2-01
#SBATCH -o ./report/out/%J_1N_1P_26T
#SBATCH -e ./report/err/%J_1N_1P_26T

./echo_avetime.sh "mpirun -rf rankfile/rankfile1N1P26C -report-bindings bin/main_mpi"
