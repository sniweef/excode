#while getopts ":ab:c:" opt; do
while getopts "ab:c:" opt; do
	case $opt in
	a ) echo -a;;
	b ) echo -b
	echo $OPTARG;;
	c ) echo -c
	echo $OPTARG;;
	* ) echo error;;
	esac
done
echo $(($OPTIND - 1))
