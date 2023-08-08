// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _UTF8_TO_GBK_H_
#define _UTF8_TO_GBK_H_

#ifdef __cplusplus
extern "C" {
#endif

int utf8_to_gbk(unsigned char *src, int len, unsigned char *dst);

#ifdef __cplusplus
}
#endif

#endif
