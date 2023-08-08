#!/bin/sh

patch -p1 < 0001-throw-in-funcdef.patch
patch -p1 < 0002-isprint.patch
patch -p1 < 0003-fix_build_with_c99_compilers.patch
