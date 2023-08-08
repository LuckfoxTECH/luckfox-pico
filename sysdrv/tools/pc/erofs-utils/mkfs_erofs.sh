#!/bin/bash

# source files
src=$1
# generate image
dst=$2
# erofs compression
EROFS_COMP=$3

err_handler() {
	ret=$?
	[ "$ret" -eq 0 ] && return

	msg_error "Running ${FUNCNAME[1]} failed!"
	msg_error "exit code $ret from line ${BASH_LINENO[0]}:"
	msg_info "    $BASH_COMMAND"
	exit $ret
}

trap 'err_handler' ERR
if [ -z "$2" -o ! -d "$src" ]; then
	echo "command format: $(basename $0) <source> <dest image> <compression>"
	echo "        <compression>   lz4|lz4hc (default lz4hc)"
	exit 0
fi

case $EROFS_COMP in
lz4|lz4hc)
	erofs_compression_args=$EROFS_COMP
	;;
*)
	erofs_compression_args=lz4hc
	;;
esac

cwd=$(dirname $(readlink -f $0))
export PATH=$cwd:$PATH

MKEROFS_TOOL=mkfs.erofs

rm -f $dst
mkdir -p $(dirname $dst)

mkerofs_cmd="$MKEROFS_TOOL $dst $src -Enoinline_data"
echo $mkerofs_cmd
eval $mkerofs_cmd
