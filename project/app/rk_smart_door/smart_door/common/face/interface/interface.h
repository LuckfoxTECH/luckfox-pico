// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef __INTERFACE_H__
#define __INTERFACE_H__


#define FACE_DIRECTION_UP       0x10       // face up
#define FACE_DIRECTION_DOWN     0x08       // face down
#define FACE_DIRECTION_LEFT     0x04       // face left
#define FACE_DIRECTION_RIGHT    0x02       // face right
#define FACE_DIRECTION_MIDDLE   0x01       // face middle
#define FACE_DIRECTION_UNDEFINE 0x00       // face undefine

/*****************************
   face cmd parser
*****************************/
void intf_cmd_parser(char *data, int len);

/*****************************
   face module to ap interface
*****************************/
void intf_module_ready();
void intf_module_send_verify_result(int result, int usr_id);
void intf_module_send_enroll_result(int result, int usr_id);
void intf_module_send_face_state(int result);

/*****************************
   ap to face module interface
*****************************/
int intf_set_module_face_verify(int seconds);
int intf_set_module_face_enroll(int seconds);
int intf_set_module_clr_face_data(int face_id);
int intf_set_module_face_exit(void);
int intf_set_module_face_enroll_multi_dir(int seconds, int dir);

#endif
