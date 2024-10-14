/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _HTTP_DBG_H_
#define _HTTP_DBG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_DBG_ON      0
#define HTTP_WRN_ON      0
#define HTTP_ERR_ON      1

#define HTTP_SYSLOG      rt_kprintf

#define HTTP_LOG(flags, fmt, arg...) \
    do {                            \
        if (flags)                  \
            HTTP_SYSLOG(fmt, ##arg); \
    } while (0)

#define HTTP_DBG(fmt, arg...)    HTTP_LOG(HTTP_DBG_ON, "\n[HTTP] "fmt, ##arg)
#define HTTP_WRN(fmt, arg...)    HTTP_LOG(HTTP_WRN_ON, "\n[HTTP WRN] "fmt, ##arg)
#define HTTP_ERR(fmt, arg...)    HTTP_LOG(HTTP_ERR_ON, "\n[HTTP ERR] %s():%d, "fmt,   \
                                                    __func__, __LINE__, ##arg);

#ifdef __cplusplus
}
#endif

#endif /* _HTTP_DBG_H_ */

