#!/bin/bash
function output {
	local expr="$1"
	num=${expr%%["+-*/%"]*}
	if [ -n "$num" ]; then
		echo -n $num' '
	fi
	expr=${expr#$num}
	while [ -n "$expr" ]; do
		operator=${expr:0:1}
		expr=${expr#"$operator"}
		unset num
		num=${expr%%["+-*/%"]*}
		if [ -n "$num" ]; then
			echo -n $num' '
			expr=${expr#$num}
		fi
		echo -n $operator' '
		#sleep 2
	done
}
function convert {
	local subexpr=$1
	local leftexpr=${1%%(*)*}
	local rightexpr=${1##*(*)}
	#echo \$subexpr=$subexpr
	if [ "$leftexpr" != "$rightexpr" ]; then
		subexpr=${1#$leftexpr(}
		subexpr=${subexpr%)$rightexpr}
		convert "$subexpr"
		output $leftexpr
		output $rightexpr
	elif [ -z "${subexpr##*)*(*}" ]; then
		local leftsubexpr=${subexpr%%)*}
		convert "$leftsubexpr"
		local rightsubexpr=${subexpr##*(}
		convert "$rightsubexpr"
		local midsubexpr=${subexpr#$leftsubexpr)}
		midsubexpr=${midsubexpr%($rightsubexpr}
		convert "$midsubexpr"
	else
		output "$subexpr"
	fi
	#firstNum=${subexpr%%["+-*/%"]*}	
	#echo $firstNum
}
read expr
#operator=[+-*/%]
#echo $operator
convert $expr
echo 
