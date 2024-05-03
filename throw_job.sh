#!/bin/zsh

sbatch -p bdw2-mixed -N 2 -n 2 -o ./report/out/out%j_2N13T -e ./report/err/err%j_2N13T ./jobScript.sh 13
sbatch -p bdw2-mixed -N 2 -n 2 -o ./report/out/out%j_2N26T -e ./report/err/err%j_2N26T ./jobScript.sh 26