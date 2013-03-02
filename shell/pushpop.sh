#!/bin/bash

DIR_STACK=""
export DIR_STACK
function pushd {
	dirname=$1
	if [ -n "$dirname" ] && [ \( -d "$dirname" \) -a \
		\( -x "$dirname" \) ]; then
		DIR_STACK="$dirname ${DIR_STACK:-$PWD }"
		cd $dirname
		echo $DIR_STACK
	fi
}
function popd {
	DIR_STACK=${DIR_STACK#* } echo ${DIR_STACK:?dir stack is empty!}
	cd ${DIR_STACK%% *}
}
