#!/bin/bash

readtargets() {
	count=0
	while read -r line ; do 
		a=${line##\t*}
		echo -e \$a=$a

		if  [ -z "$a" ]; then
			cmd[count]=${cmd[count]}'\n'$line			
		else
			let count=count+1
			targets[count]=${line%% *}
			line=${line#* : }
			sources[count]=$line	
		fi
	done
	IFS='\n'
	for eachCmd in $cmd; do
		echo $eachCmd
	done
}
readtargets
