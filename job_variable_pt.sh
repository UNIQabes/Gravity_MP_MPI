#!/bin/zsh

#SBATCH -p bdw2-mixed
#SBATCH -N 1
#SBATCH -o ./report/test/out%J_var_pt
#SBATCH -e ./report/test/err%J_var_pt

mpirun -n 1 -map-by node -cpus-per-proc 28 bin/print_assign_hosts
echo ""
mpirun -n 2 -map-by socket -cpus-per-proc 14 bin/print_assign_hosts
echo ""
mpirun -n 28 -map-by core -cpus-per-proc 1 bin/print_assign_hosts
echo ""
mpirun -n 1 -map-by node -cpus-per-proc 28 bin/main_mpi
echo ""
mpirun -n 2 -map-by socket -cpus-per-proc 14 bin/main_mpi
echo ""
mpirun -n 28 -map-by core -cpus-per-proc 1 bin/main_mpi




