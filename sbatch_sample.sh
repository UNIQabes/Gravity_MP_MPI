#!/bin/zsh
#SBATCH -p g006m
#SBATCH -n 1
#SBATCH -c 12
#SBATCH -J test_openmp
#SBATCH -o ./report/stdout.%J
#SBATCH -e ./report/stderr.%J 
./main