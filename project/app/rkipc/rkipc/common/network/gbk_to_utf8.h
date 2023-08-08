// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef _GBK_TO_UTF8_H_
#define _GBK_TO_UTF8_H_

#ifdef __cplusplus
extern "C" {
#endif

int gbk_to_utf8(unsigned char *src, int len, unsigned char *dst);

#ifdef __cplusplus
}
#endif

#endif
