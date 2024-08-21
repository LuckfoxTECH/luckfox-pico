// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef SMART_IR_SERVER_H_
#define SMART_IR_SERVER_H_

int smart_ir_server_start(const void *aiq_ctx, int init_ir_status);
void smart_ir_server_stop(const void *aiq_ctx);
void smart_ir_calib(const void *aiq_ctx);

#endif
