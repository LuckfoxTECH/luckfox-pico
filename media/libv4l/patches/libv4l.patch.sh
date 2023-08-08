#!/bin/sh

patch -p1 < 0001-fixup-lfs-mismatch-in-preload-libraries.patch
patch -p1 < 0002-keytable-fix-EVIOCSCLOCKID-related-compile-failure.patch
patch -p1 < 0003-Build-sdlcam-only-if-jpeg-is-enabled.patch
patch -p1 < 0004-v4l2-compliance-needs-fork.patch
patch -p1 < 0005-libv4l2-Support-mmap-to-libv4l-plugin.patch
patch -p1 < 0006-libv4l-mplane-Filter-out-multiplane-formats.patch
patch -p1 < 0007-libv4l-add-V4L2_MEMORY_DMABUF-memory-support.patch
patch -p1 < 0008-libv4l-mplane-plugin-add-exbuf_ioctl-for-dmabuf.patch
patch -p1 < 0009-video-max-frame-change-from-32-to-64.patch
patch -p1 < 0010-Fix-GCC-10-fno-common.patch
patch -p1 < 0011-v4l2-tpg.patch-rename-min-max-defines-to-tpg_min-max.patch
patch -p1 < 0012-Support-builtin-v4l-plugins.patch
