#!/bin/bash

# source files
src=$1
# generate image
dst=$2
# squashfs compression
SQUASHFS4_COMP=$3

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
	echo "        <compression>   lz4|lzo|lzma|xz|gzip (default gzip)"
	exit 0
fi

case $SQUASHFS4_COMP in
lz4|lzo|lzma|xz|gzip)
	squashfs_compression_args=$SQUASHFS4_COMP
	;;
*)
	squashfs_compression_args=xz
	;;
esac

cwd=$(dirname $(readlink -f $0))
export PATH=$cwd:$PATH

MKSQUASHFS_TOOL=mksquashfs

rm -f $dst
mkdir -p $(dirname $dst)

parallel_jobs=$((1 + `getconf _NPROCESSORS_ONLN 2>/dev/null || echo 4`))

if [ "$squashfs_compression_args" = "lz4" ]; then
	echo "$MKSQUASHFS_TOOL $src $dst -noappend -processors $parallel_jobs -comp $squashfs_compression_args -Xhc"
	$MKSQUASHFS_TOOL $src $dst -noappend -processors $parallel_jobs -comp $squashfs_compression_args -Xhc
else
	echo "$MKSQUASHFS_TOOL $src $dst -noappend -processors $parallel_jobs -comp $squashfs_compression_args"
	$MKSQUASHFS_TOOL $src $dst -noappend -processors $parallel_jobs -comp $squashfs_compression_args
fi
