#!/bin/zsh

mpirun -cpus-per-proc 14 bin/print_assign_hosts
./eval_perf_each_threads.sh "mpirun -cpus-per-proc 14 bin/main_mpi"