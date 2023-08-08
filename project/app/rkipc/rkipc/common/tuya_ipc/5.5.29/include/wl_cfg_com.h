/***********************************************************
 *  File: wl_cfg_com.h
 *  Author: nzy
 *  Date: 20170222
 ***********************************************************/
#ifndef _WL_CFG_COM_H
#define _WL_CFG_COM_H
#include "netcfg_module.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "tuya_os_adapter.h"
#include "wf_basic_intf.h"

#ifdef __WL_CFG_COM_GLOBALS
#define __WL_CFG_COM_MODULE_EXT
#else
#define __WL_CFG_COM_MODULE_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/
typedef BYTE_T WL_NW_CFG_T;
#define WNC_TEZ_MC 0
#define WNC_TLK_BC 1
#define WNC_AIRKISS_BC 2

#define WL_CHAN_NUM 14

typedef struct {
	BYTE_T ssid[WIFI_SSID_LEN + 1];
	BYTE_T s_len;
	BYTE_T passwd[WIFI_PASSWD_LEN + 1];
	BYTE_T p_len;
	BYTE_T token[WL_TOKEN_LEN + 1]; // tuya link:region(2BYTE) + token(8BYTE) + key(4BYTE) useless
	                                // airkiss:useless
	BYTE_T t_len;
} WL_CFG_COM_IF_S;

typedef VOID (*LOCK_CHAN_CALLBACK)(IN CONST WL_NW_CFG_T wnc, IN CONST BYTE_T *src_mac,
                                   IN CONST BYTE_T *bssid, IN CONST BYTE_T lock_chan);
typedef VOID (*NW_CFG_FIN_CALLBACK)(IN CONST WL_NW_CFG_T wnc, IN CONST WL_CFG_COM_IF_S *wl_cfg,
                                    IN VOID *data);

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/

#ifdef __cplusplus
}
#endif
#endif
