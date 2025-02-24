#!/bin/bash
##
## Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
## Use of this source code is governed by a BSD-style license that can be
## found in the LICENSE file.
##

find . -name "*.c" -o -name "*.h" | xargs clang-format -style=file -i
find . -name "*.cpp" -o -name "*.h" | xargs clang-format -style=file -i
