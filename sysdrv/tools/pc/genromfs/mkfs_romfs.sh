#!/bin/bash

err_handler() {
	ret=$?
	[ "$ret" -eq 0 ] && return

	msg_error "Running ${FUNCNAME[1]} failed!"
	msg_error "exit code $ret from line ${BASH_LINENO[0]}:"
	msg_info "    $BASH_COMMAND"
	exit $ret
}

trap 'err_handler' ERR
if [ -z "$2" -o ! -d "$1" ]; then
	echo "command format: $(basename $0) <source> <dest image>"
	exit 0
fi

genromfs -d $1 -f $2
