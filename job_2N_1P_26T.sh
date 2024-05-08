#!/bin/zsh

#SBATCH -p bdw2-mixed
#SBATCH -N 2
#SBATCH -w ppx2-01,ppx2-04
#SBATCH -o ./report/out/%J_2N_1P_26T
#SBATCH -e ./report/err/%J_2N_1P_26T

./echo_avetime.sh "mpirun -rf rankfile/rankfile2N1P26C -report-bindings bin/main_mpi"
