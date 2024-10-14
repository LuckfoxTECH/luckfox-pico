/*
 * Copyright (c) 2020 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * @file ssv_ctl_common.h
 * @brief Common defines and declarations for host driver for all platforms.
 */


#ifndef __SSV_CTL_COMMON_H__
#define __SSV_CTL_COMMON_H__
#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 *         Include Files
 ******************************************************************************/
#include <linux/genetlink.h>


/*******************************************************************************
 *         Defines
 ******************************************************************************/
/// SSV control netlink ID.
#define SSV_CTL_NL_ID                       (998)
#define SSV_WPAS_NL_ID                      (999)

#ifndef _SSV_TYPES_H_
//SSV PACK Definition
#define SSV_PACKED_STRUCT_BEGIN
#define SSV_PACKED_STRUCT               //__attribute__ ((packed))
#define SSV_PACKED_STRUCT_END           //__attribute__((packed))
#define SSV_PACKED_STRUCT_STRUCT        __attribute__ ((packed))
#define SSV_PACKED_STRUCT_FIELD(x)      x
#endif

#define WIFI_DRV_CMD_HDR_LEN            ((size_t)(((ST_WIFI_DRV_CMD *)0)->data))
#define WIFI_DRV_EVT_HDR_LEN            ((size_t)(((ST_WIFI_DRV_EVT *)0)->data))

/*******************************************************************************
 *         Enumerations
 ******************************************************************************/
/// SSV control netlink command.
typedef enum _en_ssv_ctl_cmd
{
    E_SSV_CTL_CMD_UNSPEC,                   ///< Unspecified.
    /** Linux APP process to Linux kernel driver */
    E_SSV_CTL_CMD_TO_DRV_OPERACMD,          ///< Operation command to drvier.
    E_SSV_CTL_CMD_SSV_NIMBLE_ENABLE,
    E_SSV_CTL_CMD_SSV_NIMBLE_DISABLE,
    E_SSV_CTL_CMD_TO_SSV_NIMBLE,
    E_SSV_CTL_CMD_FROM_SSV_NIMBLE,
    E_SSV_CTL_CMD_MAX                       ///< Maximum.
} EN_SSV_CTL_CMD;

/// SSV control netlink attribute.
typedef enum _en_ssv_ctl_attr 
{
    E_SSV_CTL_ATTR_UNSPEC,                  ///< Unspecified.
    E_SSV_CTL_ATTR_OPERACMD,                ///< Operation command.
    E_SSV_CTL_ATTR_OPERACMD_RSP,            ///< Operation response.
    E_SSV_CTL_ATTR_SSV_NIMBLE_ENABLE,         ///< NIMBLE
    E_SSV_CTL_ATTR_SSV_NIMBLE_DISABLE,        ///< NIMBLE
    E_SSV_CTL_ATTR_TO_SSV_NIMBLE,             ///< NIMBLE
    E_SSV_CTL_ATTR_FROM_SSV_NIMBLE,           ///< NIMBLE
    E_SSV_CTL_ATTR_MAX                      ///< Maximum.
} EN_SSV_CTL_ATTR;

///SSV operation command
typedef enum _en_ssv_oper_cmd
{
    E_SSV_OPER_UNSPEC,
    E_SSV_OPER_REGR,
    E_SSV_OPER_REGW,
    E_SSV_OPER_MAX
} EN_SSV_OPER_CMD;


/// SSV wpa_supplicant control netlink command.
typedef enum _en_ssv_wpas_ctl_cmd
{
    E_SSV_WPAS_CTL_CMD_UNSPEC,              ///< Unspecified.
    E_SSV_WPAS_CTL_CMD_EXTAUTH,             ///< External Auth status to driver
    E_SSV_WPAS_CTL_CMD_MAX                  ///< Maximum.
} EN_SSV_WPAS_CTL_CMD;
 
/// SSV wpa_supplicant control netlink attribute.
typedef enum _en_ssv_wpas_ctl_attr 
{
    E_SSV_WPAS_CTL_ATTR_UNSPEC,             ///< Unspecified.
    E_SSV_WPAS_CTL_ATTR_EXTAUTH,            ///< External Auth status.
    E_SSV_WPAS_CTL_ATTR_EXTAUTH_RSP,        ///< External Auth require.
    E_SSV_WPAS_CTL_ATTR_MAX                 ///< Maximum.
} EN_SSV_WPAS_CTL_ATTR;


/*******************************************************************************
 *         Structures
 ******************************************************************************/
/// Structure of SSV control driver command. .
SSV_PACKED_STRUCT_BEGIN
typedef struct st_wifi_drv_cmd
{
    unsigned int cmdid;
    unsigned int datalen;
    unsigned char data[0];
}SSV_PACKED_STRUCT_STRUCT ST_WIFI_DRV_CMD;
SSV_PACKED_STRUCT_END

/// Structure of SSV control driver event.
SSV_PACKED_STRUCT_BEGIN
typedef struct st_wifi_drv_evt
{
    unsigned int evtid;
    unsigned int datalen;
    unsigned char data[0];
}SSV_PACKED_STRUCT_STRUCT ST_WIFI_DRV_EVT;
SSV_PACKED_STRUCT_END

/// Structure of SSV control regr parameter.
SSV_PACKED_STRUCT_BEGIN
typedef struct st_wifi_reg_param
{
    unsigned int address;
    unsigned int value;
}SSV_PACKED_STRUCT_STRUCT ST_WIFI_REG_PARAM;
SSV_PACKED_STRUCT_END


/*******************************************************************************
 *         Variables
 ******************************************************************************/


/*******************************************************************************
 *         Functions
 ******************************************************************************/


#ifdef __cplusplus
}
#endif
#endif /* __SSV_CTL_COMMON_H__ */
