#!/bin/bash
if [ -z $1 ]; then
	echo need a number
	exit 1
elif [ -z "$(echo $1 | grep '^[0-9][0-9]*$')" ]; then
	echo argument is not a number
	exit 1
fi

for i in $(cut -f1,3 -d: /etc/passwd); do
	array[${i#*:}]=${i%:*}
done
for i in $(cut -d: -f3,5 /etc/passwd); do
	if [ -n "$(echo $i | grep ':')" ]; then
		index=${i%:*}
		value=${i#*:}
		names[$index]=$value
	else
		names[$index]=${names[$index]}' '$i
	fi
done

if [ -z ${array[$1]} ] ;then 
	echo "$1 not found!"
else
	echo "$1 is ${array[$1]} (${names[$1]})"
fi
#for i in ${!array[@]}; do
#	if [ $i -eq $1 ]; then
#		found=1
#	fi
#done
#
#if [ -z $found ]; then
#	echo $1 not found!
#else
#	echo $1 is ${array[$1]}
#fi


