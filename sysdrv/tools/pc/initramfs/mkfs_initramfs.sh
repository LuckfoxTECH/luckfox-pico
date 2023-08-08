#!/bin/bash

# source files
src=$1
# generate image
dst=$2

err_handler() {
	ret=$?
	[ "$ret" -eq 0 ] && return

	msg_error "Running ${FUNCNAME[1]} failed!"
	msg_error "exit code $ret from line ${BASH_LINENO[0]}:"
	msg_info "    $BASH_COMMAND"
	exit $ret
}

trap 'err_handler' ERR
if [ -z "$dst" -o ! -d "$src" ]; then
	echo "command format: $(basename $0) <source> <dest image>"
	exit 0
fi

if ! which cpio; then
	echo "$(basename $0) NOT found cpio"
	echo "$(basename $0) Please install cpio firstly !!!"
fi
# package rootfs in cpio
(cd $src; find . | cpio --quiet -o -H newc > $dst)
