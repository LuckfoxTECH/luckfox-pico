#!/bin/bash
#
# Generate link for shared .c files in softmac and fullmac directories
# Note:
# links are not mandatory and we could easily update softmac (fullmac)
# Makefile to compile shared file directly but this would imply to recompile
# shared code eveytime when both driver are enabled
#

set -e

dir_list=

if [ $1 ] && [ $1 = "clean" ]
then
    action="clean"
else
    action="create"
fi

if [ -d softmac ]
then
    dir_list="softmac"
fi

if [ -d fullmac ]
then
    dir_list="$dir_list fullmac"
fi

if [ -d fhost ]
then
    dir_list="$dir_list fhost"
fi

for f in $(find . -maxdepth 1 -name "*.c")
do
    for d in $dir_list
    do
	if [ $action = "clean" ]
	then
	    # only delete link
	    if [ -L $d/${f#./} ]
	    then
		rm -f $d/${f#./}
	    fi
	else
	    # only create link if file doesn't exist
	    # (whether it is a link or not)
	    if [ ! -e $d/${f#./} ]
	    then
		ln -sf .$f $d/${f#./}
	    fi
	fi
    done
done
