/***********************************************************
 *  File: ez_mc.h
 *  Author: nzy
 *  Date: 20170222
 ***********************************************************/
#ifndef _EZ_MC_H
#define _EZ_MC_H
#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_cloud_types.h"
#include "wl_cfg_com.h"

#ifdef __EZ_MC_GLOBALS
#define __EZ_MC_MODULE_EXT
#else
#define __EZ_MC_MODULE_EXT extern
#endif

/***********************************************************
*************************micro define***********************
***********************************************************/

/***********************************************************
*************************variable define********************
***********************************************************/

/***********************************************************
*************************function define********************
***********************************************************/
/***********************************************************
 *  Function: ez_mc_init
 *  Input: lc_cb ncf_cb
 *  Output:
 *  Return: OPERATE_RET
 ***********************************************************/
__EZ_MC_MODULE_EXT
OPERATE_RET ez_mc_init(IN CONST LOCK_CHAN_CALLBACK lc_cb, IN CONST FN_NETCFG_CB ncf_cb,
                       IN CONST int netcfg_type);

/***********************************************************
 *  Function: ez_mc_parse
 *  Input: dest src bssid len chan(1-14)
 *  Output:
 *  Return: none
 ***********************************************************/
__EZ_MC_MODULE_EXT
OPERATE_RET ez_mc_parse(IN CONST BYTE_T *dest, IN CONST BYTE_T *src, IN CONST BYTE_T *bssid,
                        IN CONST USHORT_T len, IN CONST BYTE_T chan);

/***********************************************************
 *  Function: ez_mc_uninit
 *  Input: none
 *  Output:
 *  Return: none
 ***********************************************************/
__EZ_MC_MODULE_EXT
VOID ez_mc_uninit(VOID);

#ifdef __cplusplus
}
#endif
#endif
