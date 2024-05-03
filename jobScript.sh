#!/bin/zsh
export OMP_NUM_THREADS=$1
mpirun bin/print_assign_hosts
./echo_avetime.sh bin/main_mpi