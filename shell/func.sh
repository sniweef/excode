#!/bin/bash
function afunc {
	#local var1
	echo in function: $0 $1 $2
	var1="var1 of function"
	echo afunc var1: $var1
}
#echo $BASH_VERSION

var1="var1 outside function"
echo var1: $var1
echo $0:$1 $2
afunc funarg1 funarg2
echo var1: $var1
echo $0:$1 $2
