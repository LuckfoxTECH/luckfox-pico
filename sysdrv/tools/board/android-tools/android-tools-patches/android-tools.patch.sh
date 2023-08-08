#!/bin/sh

patch -p1 < 0001-Fix-makefiles-for-out-of-tree-build.patch
patch -p1 < 0002-Fix-adbd-for-non-Ubuntu-systems.patch
patch -p1 < 0003-Fix-build-issue-with-uclibc.patch
patch -p1 < 0004-Fix-build-issue-with-musl.patch
patch -p1 < 0005-fix-static-link-zlib.patch
patch -p1 < 0006-fix-big-endian-build.patch
patch -p1 < 0007-include-cdefs-h-when-needed.patch
patch -p1 < 0008-support-reboot-loader.patch
patch -p1 < 0009-fix-adb-can-t-ctrl-c.patch
patch -p1 < 0010-Support-usb3.0-adbd.patch
patch -p1 < 0011-package-android-tools-fix-adb-forward-bug.patch
patch -p1 < 0012-usb_linux_client-Compat-with-old-kernel.patch
patch -p1 < 0013-Support-requiring-login-for-adb-shell.patch
patch -p1 < 0014-Specify-login-shell-in-standard-way.patch
patch -p1 < 0015-make-adb-tcp-function-default-enbale.patch
patch -p1 < 0015-support-rsa-key-authentication.patch
patch -p1 < 0016-fix-static-link-dl-lll_unlock_wait_private.patch
