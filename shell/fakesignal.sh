#!/bin/bash
errtrap() {
	es=$?
	echo "ERROR line $1: $es"
}
testf(){
	return 9
}
debugtrap(){
	echo debug
}
trap "echo Bye" EXIT
trap 'errtrap $LINENO' ERR 
trap debugtrap DEBUG
echo Hello
trap - DEBUG
