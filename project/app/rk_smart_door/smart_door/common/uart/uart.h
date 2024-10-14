// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __SMART_DOOR_UART__
#define __SMART_DOOR_UART__

typedef void (*uart_recv_callback)(unsigned char str);

int smart_door_uart(void);
int smart_door_uart_init(void *param);
int smart_door_uart_deinit(void);
void smart_door_uart_sendbyte(uint8_t byte);

#endif
