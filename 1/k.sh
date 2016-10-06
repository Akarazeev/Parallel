#!/bin/bash

gcc main.c -o a.out
gcc h.c -o h.out

for size in {3,10,100,500,1000,3000,5000,10000};
do 	
	# echo "Size: $size"
	./h.out $size;
	for num_threads in {1,2,3,4,6,8,16,24,32};
	do 	
		# echo "Num_threads: $num_threads";
		TIMES="$(./a.out $num_threads)";
		# echo $TIMES
		tmp=( $TIMES )
		# echo "${tmp[0]}"
		echo "$size,$num_threads,${tmp[0]},${tmp[1]}"
	done;
done;