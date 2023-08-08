// Copyright 2021 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
unsigned char *get_assiic_table(char c);
void read_timestamp_from_uart(char *timestamp_char);
void draw_buffer(RK_U32 *ColorData, char *timestamp_char);