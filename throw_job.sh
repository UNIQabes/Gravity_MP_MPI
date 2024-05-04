#!/bin/zsh

sbatch -p bdw2-mixed -N 1 -n 1 -c 14 -o ./report/out/out%j_1N -e ./report/err/err%j_1N ./jobScript.sh 
sbatch -p bdw2-mixed -N 2 -n 2 -c 14 -o ./report/out/out%j_2N -e ./report/err/err%j_2N ./jobScript.sh 
sbatch -p bdw2-mixed -N 3 -n 3 -c 14 -o ./report/out/out%j_3N -e ./report/err/err%j_3N ./jobScript.sh 
sbatch -p bdw2-mixed -N 1 -n 2 -c 14 -o ./report/out/out%j_1N2P -e ./report/err/err%j_1N2P ./jobScript.sh 
sbatch -p bdw2-mixed -N 2 -n 4 -c 14 -o ./report/out/out%j_2N2P -e ./report/err/err%j_2N2P ./jobScript.sh 
sbatch -p bdw2-mixed -N 3 -n 6 -c 14 -o ./report/out/out%j_3N2P -e ./report/err/err%j_3N2P ./jobScript.sh 