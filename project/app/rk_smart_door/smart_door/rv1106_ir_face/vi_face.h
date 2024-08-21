// Copyright 2023 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __VI_FACE_H__
#define __VI_FACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define CAM_NUM	2

int smart_door_face_init(void);
void smart_door_face_deinit(void);
void smart_door_face_delete_one(int id);
void smart_door_face_delete_all(void);
int smart_door_face_unlock_flow(int mode, int id);

#ifdef __cplusplus
} //extern "C"
#endif

#endif