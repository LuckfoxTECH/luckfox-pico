/***********************************************************
 *  File: wf_nw_cfg.h
 *  Author: nzy
 *  Date: 20170220
 ***********************************************************/
#ifndef _WF_NW_CFG_H
#define _WF_NW_CFG_H

#include "tuya_cloud_types.h"
#include "wl_cfg_com.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __WF_NW_CFG_GLOBALS
#define __WF_NW_CFG_EXT
#else
#define __WF_NW_CFG_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/*
    smart cfg配网初始化接口
*/
int smart_netcfg_init(int netcfg_policy);

#ifdef __cplusplus
}
#endif
#endif
