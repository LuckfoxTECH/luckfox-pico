// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __SERVRE_H__
#define __SERVRE_H__

void server_write_cmd(char *cmd, int len);
int face_server_init(void);
int face_server_deinit(void);

#endif
