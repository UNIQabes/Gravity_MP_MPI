#!/bin/zsh
#SBATCH -p bdw2
#SBATCH -w ppx2-04
#SBATCH -n 2
#SBATCH -c 14
#SBATCH -J test_openmp
#SBATCH -o ./report/stdout.%J
#SBATCH -e ./report/stderr.%J 
./mpcode_time.sh