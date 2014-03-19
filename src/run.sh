#!/bin/bash

function run {
	for i in 1 $(seq 2 2 $maxthreads); do
		export OMP_NUM_THREADS=$i
		echo -n $i, >> $csvfile
		./$prog > $prog.log
		rt=$(grep "time" $prog.log | cut -d" " -f4 | tr -d -C [0-9.\\n])
		gflops=$(grep "GFLOPS" $prog.log | cut -d" " -f3 | tr -d -C [0-9.\\n])
		echo $rt,$gflops >> $csvfile
	done
	rm $prog.log
}

maxthreads=240

prog=matmul

csvfile=matmul_none.csv
echo "Number of threads,Runtime (s),GFLOPS" > $csvfile
export KMP_AFFINITY=""
run

csvfile=matmul_compact.csv
export KMP_AFFINITY="granularity=fine,compact"
echo "Number of threads,Runtime (s),GFLOPS" > $csvfile
run

csvfile=matmul_scatter.csv
export KMP_AFFINITY="granularity=fine,scatter"
echo "Number of threads,Runtime (s),GFLOPS" > $csvfile
run

csvfile=matmul_balanced.csv
export KMP_AFFINITY="granularity=fine,balanced" # MIC-only
echo "Number of threads,Runtime (s),GFLOPS" > $csvfile
run
