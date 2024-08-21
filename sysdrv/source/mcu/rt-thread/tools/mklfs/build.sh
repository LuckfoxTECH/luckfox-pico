#!/bin/sh

gcc -static -o mklfs mklfs.c lfs/lfs.c  lfs/lfs_util.c
