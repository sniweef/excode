#!/bin/bash
array=(test1 test2 [4]=test4 hello world)
for ((i=0; i<=5; i++)); do 
	echo ${array[i]}
done
for i in ${array[@]};do
	echo $i':'${#array[i]}
done

echo ${#array[@]}
echo ${!array[@]}
echo ${array[*]}
