#!/bin/bash

values=(3 7 1 19 3 2 98 7 96 65 4 0)
numofvalues=${#values[@]}
echo $numofvalues

for (( i=0; i<numofvalues-1; i++ )); do
	for (( j=0; j<numofvalues-1; j++ )); do
		let k=j+1
		if [ ${values[j]} -gt ${values[k]} ]; then
			tmp=${values[j]}
			values[j]=${values[k]}
			values[k]=$tmp
		fi
	done
done

for i in ${values[@]}; do
	echo -n $i' '
done
echo
