#!/bin/sh
patch -p1 < 0001-Dont-waste-time-building-manpages-if-we-re-not-going.patch
patch -p1 < 0002-Reproducible-build-do-not-leak-compiler-path.patch
patch -p1 < 0003-Introduce-the-OPENSSL_NO_MADVISE-to-disable-call-to-.patch
patch -p1 < 0004-Configure-use-ELFv2-ABI-on-some-ppc64-big-endian-sys.patch
patch -p1 < 0005-crypto-perlasm-ppc-xlate.pl-add-linux64v2-flavour.patch
