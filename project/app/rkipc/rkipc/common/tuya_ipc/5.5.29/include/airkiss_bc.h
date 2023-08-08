/***********************************************************
 *  File: airkiss_bc.h
 *  Author: nzy
 *  Date: 20170222
 ***********************************************************/
#ifndef _AIRKISS_BC_H
#define _AIRKISS_BC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "tuya_os_adapter.h"

#include "netcfg_module.h"
#include "wl_cfg_com.h"

#ifdef __AIRKISS_BC_GLOBALS
#define __AIRKISS_BC_MODULE_EXT
#else
#define __AIRKISS_BC_MODULE_EXT extern
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
 *  Function: ak_bc_init
 *  Input: lc_cb ncf_cb
 *  Output:
 *  Return: OPERATE_RET
 ***********************************************************/
__AIRKISS_BC_MODULE_EXT
OPERATE_RET ak_bc_init(IN CONST LOCK_CHAN_CALLBACK lc_cb, IN CONST FN_NETCFG_CB ncf_cb,
                       IN CONST int netcfg_type
                       /*,IN CONST AP_IF_S *scan_ap,IN CONST BYTE_T scan_ap_num*/);

/***********************************************************
 *  Function: ak_bc_parse
 *  Input: dest src bssid len chan(1-14)
 *  Output:
 *  Return: none
 ***********************************************************/
__AIRKISS_BC_MODULE_EXT
OPERATE_RET ak_bc_parse(IN CONST BYTE_T *src, IN CONST BYTE_T *bssid, IN CONST USHORT_T len,
                        IN CONST BYTE_T chan, IN CONST BC_DA_CHAN_T from);

/***********************************************************
 *  Function: mimo_ak_bc_parse
 *  Input: rssi type mcs len chan(1-14)
 *  Output:
 *  Return: none
 ***********************************************************/
OPERATE_RET mimo_ak_bc_parse(IN CONST SCHAR_T rssi, IN CONST MIMO_TYPE_E type, IN CONST BYTE_T mcs,
                             IN CONST USHORT_T len, IN CONST BYTE_T chan);

/***********************************************************
 *  Function: get_airkiss_random
 *  Input: none
 *  Output:
 *  Return: <0 fail else success
 ***********************************************************/
__AIRKISS_BC_MODULE_EXT
INT_T get_airkiss_random(VOID);

/***********************************************************
 *  Function: ak_bc_uninit
 *  Input: none
 *  Output:
 *  Return: none
 ***********************************************************/
__AIRKISS_BC_MODULE_EXT
VOID ak_bc_uninit(VOID);

#ifdef __cplusplus
}
#endif
#endif
