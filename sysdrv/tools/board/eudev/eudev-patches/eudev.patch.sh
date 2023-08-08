#!/bin/sh

patch -p1 < 0001-Only-use-pragma-for-ignoring-diagnostics-if-.patch
patch -p1 < 0002-missing.h-add-KEY_ALS_TOGGLE.patch
patch -p1 < 0003-missing.h-add-BTN_DPAD_UP.patch
