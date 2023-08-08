#!/bin/sh

patch -p1 < 0001-no-mmu.patch
patch -p1 < 0002-dlmisc.patch
patch -p1 < 0003-conditional-enabling-of-libdl-in-m4.patch
patch -p1 < 0004-add-dcbfilter-16bit-process.patch
patch -p1 < 0005-add-gain-for-audio-input.patch
patch -p1 < 0006-alsa-lib-fix-the-overflow-if-DCB_FILTER_16BIT-is-def.patch
patch -p1 < 0007-pcm-Fix-two-bugs-in-snd_pcm_area_silence.patch
