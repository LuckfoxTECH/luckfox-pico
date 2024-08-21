#!/bin/bash

if [ -z "$OPTEE_OS_PATH" ]; then
   OPTEE_OS_PATH="../optee_os";
fi

source $OPTEE_OS_PATH/scripts/checkpatch.sh "$@"
