// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef RKIPC_NTP_H_
#define RKIPC_NTP_H_

#ifdef __cplusplus
extern "C" {
#endif

int rkipc_ntp_update(const char *ntp_server_addr);

#ifdef __cplusplus
}
#endif

#endif // RKIPC_NTP_H_
