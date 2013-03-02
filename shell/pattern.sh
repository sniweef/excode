#!/bin/bash
var=testfortest
echo ${var#t*}  #delete the shortest part that matches
echo ${var##t*} #delete the longest part that matches
echo ${var%t*}
echo ${var%%t*}
echo ${var/test/test2}
echo ${var//test/test2}
echo -e ${PATH//:/'\n'} 
echo ${#var} #the length of var
