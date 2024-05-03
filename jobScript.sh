#!/bin/zsh

module load openmpi/3.1.0  
export OMP_NUM_THREADS=$1
mpirun bin/print_assign_hosts

date +%Y%m%d_%H%M%S_%N 
bin/main_par
date +%Y%m%d_%H%M%S_%N 
bin/main_mpi
date +%Y%m%d_%H%M%S_%N 
mpirun bin/main_mpi
date +%Y%m%d_%H%M%S_%N 

#./echo_avetime.sh "bin/main_mpi"