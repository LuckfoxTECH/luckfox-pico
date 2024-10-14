/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _SYS_OTA_OPT_H_
#define _SYS_OTA_OPT_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef RT_USING_OTA_FROM_LOCAL
#define OTA_OPT_PROTOCOL_FILE       1
#else
#define OTA_OPT_PROTOCOL_FILE       0
#endif

#ifdef RT_USING_OTA_FROM_HTTP
#define OTA_OPT_PROTOCOL_HTTP       1
#else
#define OTA_OPT_PROTOCOL_HTTP       0
#endif

#define OTA_OPT_VERIFY_JHASH        1
#define OTA_OPT_VERIFY_CRC32        0

/* just used for http test on rockchip inner http server */
#define RK_OTA

#define OTA_CHECK_SIZE        (4*1024)
#define OTA_BUF_SIZE          (OTA_CHECK_SIZE*1)


#ifdef __cplusplus
}
#endif

#endif /* _SYS_OTA_OPT_H_ */

