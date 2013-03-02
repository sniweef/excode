#!/bin/bash
function recdir {
	tab=$tab'\t'

	for file in "$@"; do
		echo -en $tab$file
		path=$path/$file

		if [ -d "$path" ]; then
			echo \\
			recdir $(command ls $path)
		else
			echo
		fi

		path=${path%/*}
	done
	tab=${tab%'\t'}
}
maxdepth=${2:-8}
for eacharg in "$@"; do
	echo $eacharg
	if [ -d "$eacharg" ]; then
		path=$eacharg
		recdir $(command ls $eacharg)
	fi
done
