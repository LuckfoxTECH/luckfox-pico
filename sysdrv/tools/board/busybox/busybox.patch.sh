#!/bin/bash

if [ ! -f "busybox_patched_done" ]; then
patch -p1 < 0001-networking-libiproute-use-linux-if_packet.h-instead-.patch
patch -p1 < 0002-Makefile.flags-strip-non-l-arguments-returned-by-pkg.patch
patch -p1 < 0003-Makefile-include-per-arch-Makefile-before-Makefile.f.patch
patch -p1 < 0004-arch-sparc-sparc64-Makefile-define-ARCH_FPIC.patch
patch -p1 < 0005-httpd-fix-handling-of-range-requests.patch
patch -p1 < 0006-bunzip2-fix-runCnt-overflow-from-bug-10431.patch
patch -p1 < 0007-unlzma-fix-SEGV-closes-10436.patch
patch -p1 < 0008-busybox-support-chinese-display-in-terminal.patch
patch -p1 < 0009-halt-Support-rebooting-with-arg.patch
patch -p1 < 0010-Remove-stime-function-calls.patch
touch busybox_patched_done
else
echo "busybox: patched done. skip"
fi
