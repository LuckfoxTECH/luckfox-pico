/***********************************************************
 *  File: tlink_bc.h
 *  Author: nzy
 *  Date: 20170222
 ***********************************************************/
#ifndef _TLINK_BC_H
#define _TLINK_BC_H

#include "tuya_cloud_types.h"
#include "wl_cfg_com.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __TLINK_BC_GLOBALS
#define __TLINK_BC_MODULE_EXT
#else
#define __TLINK_BC_MODULE_EXT extern
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
 *  Function: tlk_bc_init
 *  Input: lc_cb ncf_cb
 *  Output:
 *  Return: OPERATE_RET
 ***********************************************************/
__TLINK_BC_MODULE_EXT
OPERATE_RET tlk_bc_init(IN CONST LOCK_CHAN_CALLBACK lc_cb, IN CONST FN_NETCFG_CB ncf_cb,
                        IN CONST int netcfg_type);

/***********************************************************
 *  Function: tlk_bc_parse
 *  Input: dest src bssid len chan(1-14)
 *  Output:
 *  Return: none
 ***********************************************************/
__TLINK_BC_MODULE_EXT
OPERATE_RET tlk_bc_parse(IN CONST BYTE_T *src, IN CONST BYTE_T *bssid, IN CONST USHORT_T len,
                         IN CONST BYTE_T chan, IN CONST BC_DA_CHAN_T from);

/***********************************************************
 *  Function: tlk_bc_uninit
 *  Input: none
 *  Output:
 *  Return: none
 ***********************************************************/
__TLINK_BC_MODULE_EXT
VOID tlk_bc_uninit(VOID);

/***********************************************************
 *  Function: mimo_tlk_bc_parse
 *  Input: rssi type mcs len chan(1-14)
 *  Output:
 *  Return: none
 ***********************************************************/
__TLINK_BC_MODULE_EXT
OPERATE_RET mimo_tlk_bc_parse(IN CONST SCHAR_T rssi, IN CONST MIMO_TYPE_E type, IN CONST BYTE_T mcs,
                              IN CONST USHORT_T len, IN CONST BYTE_T chan);

#ifdef __cplusplus
}
#endif
#endif
