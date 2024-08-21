/*****************************************************************************
**
**  Name:          brcm_ble.h
**
**  Description:   This file contains Broadcom customer specific feature for BLE
**
**
**
**  Copyright (c) 2008-2015, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BRCM_BLE_H
#define BRCM_BLE_H

#include "btm_int.h"
#include "btm_ble_api.h"
#include "brcm_api.h"



/* customer specific features bits */
#define BTM_BLE_META_CS_RESOLVE_ADDR        0x00000001
#define BTM_BLE_META_CS_FEAT_ADV_PF         0x00000002
#define BTM_BLE_META_CS_FEAT_RSSI           0x00000004
#define BTM_BLE_META_CS_FEAT_SENSOR         0x00000008
#define BTM_BLE_META_CS_FEAT_SCATTERNET     0x00001000

#define BTM_BLE_PF_BIT_TO_MASK(x)          (UINT32)(1 << (x))
#define BTM_BLE_PF_SELECT_NONE              0
#define BTM_BLE_PF_ADDR_FILTER_BIT          BTM_BLE_PF_BIT_TO_MASK(BTM_BLE_PF_ADDR_FILTER)
#define BTM_BLE_PF_SRVC_DATA_BIT            BTM_BLE_PF_BIT_TO_MASK(BTM_BLE_PF_SRVC_DATA)
#define BTM_BLE_PF_SRVC_UUID_BIT            BTM_BLE_PF_BIT_TO_MASK(BTM_BLE_PF_SRVC_UUID)
#define BTM_BLE_PF_SRVC_SOL_UUID_BIT        BTM_BLE_PF_BIT_TO_MASK(BTM_BLE_PF_SRVC_SOL_UUID)
#define BTM_BLE_PF_LOCAL_NAME_BIT           BTM_BLE_PF_BIT_TO_MASK(BTM_BLE_PF_LOCAL_NAME)
#define BTM_BLE_PF_MANU_DATA_BIT            BTM_BLE_PF_BIT_TO_MASK(BTM_BLE_PF_MANU_DATA)
#if !(defined BRCM_ADV_PCF_LEGACY && BRCM_ADV_PCF_LEGACY == TRUE)
#define BTM_BLE_PF_SRVC_DATA_PATTERN_BIT    BTM_BLE_PF_BIT_TO_MASK(BTM_BLE_PF_SRVC_DATA_PATTERN)
#endif
typedef UINT8 tBTM_BLE_PF_SEL_MASK;

/* RSSI monitoring mode */
#define BTM_BLE_RSSI_MODE_LINK          0x01
#define BTM_BLE_RSSI_MODE_ADV           0x02
typedef UINT8 tBTM_BLE_RSSI_MODE;

#define BTM_BLE_META_RSSI_MONITOR_LEN   8

#define HCI_BRCM_VSE_SUBCODE_BLE_META_CS       0xE9

#define HCI_BRCM_VSE_BLE_META_RSSI_LINK         0x02
#define HCI_BRCM_VSE_BLE_META_RSSI_ADV          0x03

#define BTM_BLE_VSE_RSSI_ADV_LEN            9
#define BTM_BLE_VSE_RSSI_LINK_LEN           4

#define BTM_BLE_MAX_FILTER_COUNTER  (BTM_BLE_MAX_ADDR_FILTER + 1) /* per device filter + one generic filter indexed by 0 */

#define BTM_BLE_BRCM_WL_ATTR_NONE       0
#define BTM_BLE_BRCM_WL_ATTR_SCAN       1
#define BTM_BLE_BRCM_WL_ATTR_INIT       2
#define BTM_BLE_BRCM_WL_ATTR_ADV        4
#define BTM_BLE_BRCM_WL_ATTR_ALL        (BTM_BLE_BRCM_WL_ATTR_SCAN|BTM_BLE_BRCM_WL_ATTR_INIT|BTM_BLE_BRCM_WL_ATTR_ADV)

typedef struct
{
    BOOLEAN         in_use;
    BD_ADDR         bd_addr;
    UINT32          feat_mask;      /* per BD_ADDR feature mask */
    UINT8           pf_counter[BTM_BLE_PF_TYPE_MAX]; /* number of filter indexed by tBTM_BLE_PF_COND_TYPE */
} tBTM_BLE_PF_COUNT;

/* control block for BLE customer specific feature */
typedef struct
{
    BOOLEAN             enable;

    UINT8               op_type;
    tBTM_BLE_PF_COUNT   addr_filter_count[BTM_BLE_MAX_FILTER_COUNTER]; /* per BDA filter indexed by tBTM_BLE_PF_COND_TYPE */
    tBLE_BD_ADDR        cur_filter_target;
    UINT32              cs_feature;

    tBTM_BLE_RSSI_CBACK *p_adv_rssi_cback;
    tBTM_BLE_PF_CMPL_CBACK *p_adv_pf_cback;
    tBTM_BURST_CBACK        *p_burst_cback;

} tBTM_BLE_BRCM_CB;

#define BTM_BLE_MULTI_ADV_DEFAULT_STD   0

typedef struct
{
    UINT8                       inst_id;
    UINT8                       adv_evt;
    BD_ADDR                     rpa;
    TIMER_LIST_ENT              raddr_timer_ent;
    void                        *p_rpa_cback;
    tBTM_BLE_MULTI_ADV_CBACK    *p_cback;
    void                        *p_ref;

} tBTM_BLE_MULTI_ADV_INST;


typedef struct
{
    UINT8   sub_code[BTM_BLE_MULTI_ADV_MAX];
    UINT8   inst_id[BTM_BLE_MULTI_ADV_MAX];
    UINT8   pending_idx;
    UINT8   next_idx;
} tBTM_BLE_MULTI_ADV_OPQ;

#ifndef BTM_BLE_MULTI_ADV_INST_MAX
#define BTM_BLE_MULTI_ADV_INST_MAX                      5
#endif

typedef struct
{
    tBTM_BLE_MULTI_ADV_INST adv_inst[BTM_BLE_MULTI_ADV_MAX];
    tBTM_BLE_MULTI_ADV_OPQ  op_q;
    UINT8                   adv_inst_max;               /* max adv instance supported in controller */

} tBTM_BLE_MULTI_ADV_CB;


#ifdef __cplusplus
extern "C"
{
#endif

#if BRCM_DYNAMIC_MEMORY == FALSE
BTM_API extern tBTM_BLE_BRCM_CB  btm_ble_brcm_cb;
BTM_API extern tBTM_BLE_MULTI_ADV_CB  btm_multi_adv_cb;
#else
BTM_API extern tBTM_BLE_BRCM_CB *btm_ble_brcm_ptr;
#define btm_ble_brcm_cb (*btm_ble_brcm_ptr)
BTM_API extern tBTM_BLE_MULTI_ADV_CB *btm_multi_adv_ptr;
#define btm_multi_adv_cb (*btm_multi_adv_ptr)
#endif

extern BOOLEAN btm_process_le_burst_data(tBTU_BURST_DATA_PARAM *p_param);
extern BOOLEAN btm_brcm_ble_topology_check(tBTU_TOPOLOGY_CHECK *p_param);
extern void btm_ble_multi_adv_configure_rpa(tBTM_BLE_MULTI_ADV_INST *p_inst);
extern void btm_ble_multi_adv_enb_privacy(BOOLEAN enable);
extern void btm_ble_multi_adv_init(void);
extern void btm_ble_brcm_init(void);

extern BOOLEAN btm_ble_brcm_write_device_wl_attribute(tBLE_ADDR_TYPE addr_type, BD_ADDR bd_addr, UINT8 attribute);
extern tBTM_STATUS btm_ble_enable_brcm_feature(BOOLEAN enable, UINT32 feature_bit);
extern void btm_ble_brcm_set_inq_filter(BD_ADDR inq_filter_addr);

#ifdef __cplusplus
}
#endif

#endif


