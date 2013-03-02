#!/bin/bash
array="a b c d e"
select selection in $array; do
	echo $selection
done
