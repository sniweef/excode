#!/bin/bash
#set -o noexec
#set -o verbose
#set -o xtrace
#set +o xtrace
#echo $BASH_VERSION
unset this
if [ -n "$this" ];then 
	echo hello
fi

object=person
person=alice
echo ${!object}

a=`ls`
echo $a
