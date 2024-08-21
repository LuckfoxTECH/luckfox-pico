/****************************************************************************
**
** Name:         brcm_api.h
**
** Description:  Broadcom specific BTE API function external definitions.
**
** Copyright (c) 2009-2014, BROADCOM Inc., All Rights Reserved.
** Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/
#ifndef BRCM_API_H
#define BRCM_API_H

#include "bt_types.h"
#include "btm_api.h"
#include "btm_ble_api.h"

/* max patch data length (Can be overridden by platform for ACL HCI command size) */
#ifndef MAX_PRM_LEN
#define MAX_PRM_LEN 250
#endif

/*****************************************************************************
**  Patch RAM MANAGEMENT
*****************************************************************************/
/****************************
**  Patch RAM Constants
*****************************/
/* BRCM Patch RAM status codes */
enum
{
    BRCM_PRM_STS_CONTINUE = 0,
    BRCM_PRM_STS_COMPLETE,
    BRCM_PRM_STS_ABORT
};
typedef UINT8 tBRCM_PRM_STATUS;

#ifndef BRCM_USE_DELAY
#define BRCM_USE_DELAY TRUE
#endif

/*************************************
**  BRCM Patch RAM Callback Functions
**************************************/
typedef void (tBRCM_PRM_CBACK)(tBRCM_PRM_STATUS status);


/* patch format type */
#define BTE_PRM_FORMAT_BIN    0x00
#define BTE_PRM_FORMAT_HCD    0x01
#define BTE_PRM_FORMAT_FLASH_HCD_NO_MINIDRV 0x02
typedef UINT8 tBRCM_PRM_FORMAT;


enum
{
    BRCM_PRM_ST_IDLE  = 0,
    BRCM_PRM_ST_INITIALIZING,
    BRCM_PRM_ST_INITIALIZING_DONE,
    BRCM_PRM_ST_LOADING_DATA,
    BRCM_PRM_ST_LOADING_DATA_DONE,
    BRCM_PRM_ST_LAUNCHING_RAM
};
typedef UINT8 tBRCM_PRM_STATE;

/*************************************
**  BRCM specific feature bits
**************************************/
#define BTM_BRCM_BPCS_FEAT_TBFC_BIT             0x00000002
#define BTM_BRCM_BPCS_FEAT_BI_DIR_TBFC_BIT      0x00000040
#define BTM_BRCM_BPCS_FEAT_TBFC_FHS_BIT         0x00000080
#define BTM_BRCM_BPCS_FEAT_TBFC_PAGE_BIT        0x00000100
typedef UINT32 tBTM_BRCM_BPCS_FEAT;
/*************************************
**  BRCM RSSI monitor data type
**************************************/

/* RSSI monitor alert mask */
#define BTM_BLE_RSS_ALERT_HI        0
#define BTM_BLE_RSS_ALERT_RANGE     1
#define BTM_BLE_RSS_ALERT_LO        2
typedef UINT8 tBTM_BLE_RSSI_ALERT_TYPE;


#define BTM_BLE_RSSI_ALERT_NONE         0
#define BTM_BLE_RSSI_ALERT_TO_MASK(x)       (1 <<(x))
#define BTM_BLE_RSSI_ALERT_HI_BIT           BTM_BLE_RSSI_ALERT_TO_MASK(BTM_BLE_RSS_ALERT_HI)
#define BTM_BLE_RSSI_ALERT_RANGE_BIT        BTM_BLE_RSSI_ALERT_TO_MASK(BTM_BLE_RSS_ALERT_RANGE)
#define BTM_BLE_RSSI_ALERT_LO_BIT           BTM_BLE_RSSI_ALERT_TO_MASK(BTM_BLE_RSS_ALERT_LO)
#define BTM_BLE_RSSI_MASK_ALL           (BTM_BLE_RSSI_ALERT_HI_BIT|BTM_BLE_RSSI_ALERT_RANGE_BIT|BTM_BLE_RSSI_ALERT_LO_BIT)
typedef UINT8 tBTM_BLE_RSSI_ALERT_MASK;


#define BTM_BLE_PRIVACY_ENABLE          1
#define BTM_BLE_PRIVACY_DISABLE         0

enum
{
    BTM_BLE_SCAN_COND_ADD,
    BTM_BLE_SCAN_COND_DELETE,
    BTM_BLE_SCAN_COND_CLEAR = 2
};
typedef UINT8 tBTM_BLE_SCAN_COND_OP;

/* filter selection bit index  */
#define BTM_BLE_PF_ADDR_FILTER          0
#define BTM_BLE_PF_SRVC_DATA         1
#define BTM_BLE_PF_SRVC_UUID            2
#define BTM_BLE_PF_SRVC_SOL_UUID        3
#define BTM_BLE_PF_LOCAL_NAME           4
#define BTM_BLE_PF_MANU_DATA            5
#if (defined BRCM_ADV_PCF_LEGACY && BRCM_ADV_PCF_LEGACY == TRUE)
#define BTM_BLE_PF_TYPE_ALL             6  /* when passed in payload filter type all, only clear action is applicable */
#define BTM_BLE_PF_TYPE_MAX             7
#define BTM_BLE_PF_SRVC_DATA_PATTERN    8
#else
#define BTM_BLE_PF_SRVC_DATA_PATTERN        6
#define BTM_BLE_PF_TYPE_ALL             7  /* when passed in payload filter type all, only clear action is applicable */
#define BTM_BLE_PF_TYPE_MAX             8
#endif
typedef UINT8   tBTM_BLE_PF_COND_TYPE;

#define BTM_BLE_PF_LOGIC_OR              0
#define BTM_BLE_PF_LOGIC_AND             1
typedef UINT8 tBTM_BLE_PF_LOGIC_TYPE;

/* max number of filter spot for different filter type */
#ifndef BTM_BLE_MAX_UUID_FILTER
#define BTM_BLE_MAX_UUID_FILTER     8
#endif
#ifndef BTM_BLE_MAX_ADDR_FILTER
#define BTM_BLE_MAX_ADDR_FILTER     8
#endif
#ifndef BTM_BLE_PF_STR_COND_MAX
#define BTM_BLE_PF_STR_COND_MAX     4   /* apply to manu data , or local name */
#endif
#ifndef BTM_BLE_PF_STR_LEN_MAX
#define BTM_BLE_PF_STR_LEN_MAX      29  /* match for first 29 bytes */
#endif

#define BTM_BLE_PF_ENABLE       1
#define BTM_BLE_PF_CONFIG       2
typedef UINT8 tBTM_BLE_PF_ACTION;

/* BLE adv payload filtering operation complete callback */
typedef void (tBTM_BLE_PF_CMPL_CBACK)(tBTM_BLE_PF_ACTION action, tBTM_BLE_PF_COND_TYPE cfg_cond, UINT8 hci_status);

typedef union
{
    UINT16              uuid16_mask;
    UINT32              uuid32_mask;
    UINT8               uuid128_mask[LEN_UUID_128];
} tBTM_BLE_PF_COND_MASK;

typedef struct
{
    tBLE_BD_ADDR            *p_target_addr;     /* target address, if NULL, generic UUID filter */
    tBT_UUID                uuid;           /* UUID condition */
    tBTM_BLE_PF_LOGIC_TYPE  cond_logic;    /* AND/OR */
    tBTM_BLE_PF_COND_MASK   *p_uuid_mask;           /* UUID mask */
} tBTM_BLE_PF_UUID_COND;

typedef struct
{
    UINT8                   data_len;       /* <= 20 bytes */
    UINT8                   *p_data;
} tBTM_BLE_PF_LOCAL_NAME_COND;

typedef struct
{
    UINT16                  company_id;     /* company ID */
    UINT8                   data_len;       /* <= 20 bytes */
    UINT8                   *p_pattern;
    UINT16                  company_id_mask;   /* UUID value mask */
    UINT8                   *p_pattern_mask;        /*manufactuer data matching mask, same length as
                                            data pattern, set to all 0xff, match exact the same data */

} tBTM_BLE_PF_MANU_COND;

typedef struct
{
    UINT16                  uuid;           /* service ID */
    UINT8                   data_len;       /* <= 20 bytes */
    UINT8                   *p_pattern;
} tBTM_BLE_PF_SRVC_PATTERN_COND;


typedef union
{
    tBLE_BD_ADDR                            target_addr;
    tBTM_BLE_PF_LOCAL_NAME_COND             local_name; /* lcoal name filtering */
    tBTM_BLE_PF_MANU_COND                   manu_data;  /* manufactuer data filtering */
    tBTM_BLE_PF_UUID_COND                   srvc_uuid;  /* service UUID filtering */
    tBTM_BLE_PF_UUID_COND                   solicitate_uuid;   /* solicitated service UUID filtering */
    tBTM_BLE_PF_SRVC_PATTERN_COND           srvc_data;      /* service data pattern */
} tBTM_BLE_PF_COND_PARAM;


/* adv tx power level */
#define BTM_BLE_ADV_TX_POWER_MIN        0           /* minimum tx power */
#define BTM_BLE_ADV_TX_POWER_LOW        1           /* low tx power     */
#define BTM_BLE_ADV_TX_POWER_MID        2           /* middle tx power  */
#define BTM_BLE_ADV_TX_POWER_UPPER      3           /* upper tx power   */
#define BTM_BLE_ADV_TX_POWER_MAX        4           /* maximum tx power */
typedef UINT8 tBTM_BLE_ADV_TX_POWER;

/* advertising parameter */
typedef struct
{
    UINT16                  adv_int_min;            /* minimum adv interval */
    UINT16                  adv_int_max;            /* maximum adv interval */
    UINT8                   adv_type;               /* adv event type */
    tBTM_BLE_ADV_CHNL_MAP   channel_map;            /* adv channel map */
    tBTM_BLE_AFP            adv_filter_policy;      /* advertising filter policy */
    tBTM_BLE_ADV_TX_POWER   adv_tx_power;           /* adv tx power */

} tBTM_BLE_ADV_PARAMS;

#ifndef BTM_BURST_DATA_OPCODE
#define BTM_BURST_DATA_OPCODE      (GATT_OP_CODE_MAX + 1)
#endif

typedef void (tBTM_BLE_RSSI_CBACK)(BD_ADDR bd_addr, UINT8 alert_type, INT8 rssi);

typedef void (tBTM_BURST_CBACK)(BD_ADDR bd_addr, UINT16 len, UINT8 *p_data);


#ifndef BTM_BLE_MULTI_ADV_MAX
#define BTM_BLE_MULTI_ADV_MAX   4
#endif

#define BTM_BLE_MULTI_ADV_ENB_EVT           1
#define BTM_BLE_MULTI_ADV_DISABLE_EVT       2
#define BTM_BLE_MULTI_ADV_PARAM_EVT         3
#define BTM_BLE_MULTI_ADV_DATA_EVT          4
typedef UINT8 tBTM_BLE_MULTI_ADV_EVT;

typedef void (tBTM_BLE_MULTI_ADV_CBACK)(tBTM_BLE_MULTI_ADV_EVT evt, UINT8 inst_id, void *p_ref,  tBTM_STATUS status);

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
**  Bluetooth VS Command MANAGEMENT FUNCTIONS
*****************************************************************************/
/*******************************************************************************
**
** Function         BTE_BrcmInit
**
** Description      This function initializes Broadcom specific control blocks for BTE
**
** Returns          void
**
*******************************************************************************/
BT_API extern void BTE_BrcmInit(void);


/*****************************************************************************
**  BRCM Patch RAM MANAGEMENT FUNCTIONS
*****************************************************************************/
/*******************************************************************************
**
** Function         BRCM_PrmInit
**
** Description      Register patch ram callback, start to send DOWNLOAD_MINI_DRV
**                  or FLASH_FW_UPGRADE to controller depending on 'format_type' param
**                  to start the download process, if use internal patch,
**                  it will start the download data process from the internal patch, else
**                  application need to provide the patch data
** Input Param      p_cb - callback for download status
**                  use_internal_patch_data - if true patch data is built into code,
**                                          - else application need to provide patch data
**                                          - by calling BRCM_PRM_Load_Data
**                  address - address of patch ram,
**                  format_type - patch format type ( bin, hcd ...)
**
** Returns          TRUE if successful, otherwise FALSE
**
**
*******************************************************************************/
BT_API extern BOOLEAN BRCM_PrmInit(tBRCM_PRM_CBACK *p_cb, UINT8 *p_patch_buf, UINT32 patch_buf_len,
                                   UINT32 address, UINT8 format_type);

/*******************************************************************************
**
** Function         BRCM_PrmLoadData
**
** Description      Download data to controller if application need to provide patch data.
**
** Input Param      p_patch_data            - pointer to patch data
**                  patch_data_len          - patch data len
**
** Returns          TRUE if successful, otherwise FALSE
**
*******************************************************************************/
BT_API extern BOOLEAN BRCM_PrmLoadData(UINT8 *p_patch_data,
                                       UINT16 patch_data_len);

/*******************************************************************************
**
** Function         BRCM_PrmLaunchRam
**
** Description      After all patches are download, lauch ram
** Input Param
**
**
** Returns          TRUE if successful, otherwise FALSE
**
*******************************************************************************/
BT_API extern BOOLEAN BRCM_PrmLaunchRam(void);

/*******************************************************************************
**
** Function         BRCM_PrmGetState
**
** Description      get patch ram state info
** Input Param
**
**
** Returns          patch ram state
**
*******************************************************************************/
BT_API extern UINT8 BRCM_PrmGetState(void);

/*******************************************************************************
**
** Function         BRCM_PRM_Reset_H5
**
** Description      Reset H5
** Input Param
**
**
** Returns          None
**
*******************************************************************************/
BT_API extern void BRCM_PRM_Reset_H5(void);

/*******************************************************************************
**
** Function         BRCM_SetARCMode
**
** Description      Send Audio Routing Control command.
**
** Returns          void
**
*******************************************************************************/
BT_API extern void BRCM_SetARCMode(UINT8 iface, UINT8 arc_mode, tBTM_VSC_CMPL_CB *p_arc_cb);


/*******************************************************************************
**
** Function         BRCM_PCM2Setup_Write
**
** Description      Send PCM2_Setup write command.
**
** Returns          void
**
*******************************************************************************/
BT_API extern void BRCM_PCM2Setup_Write(BOOLEAN clk_master, tBTM_VSC_CMPL_CB *p_arc_cb);

/*******************************************************************************
**
** Function         BTM_ReadBPCSFeatureBits
**
** Description      Read remote device broadcom specific feature
**
** Returns          void
**
*******************************************************************************/
BT_API extern tBTM_BRCM_BPCS_FEAT  BTM_ReadBPCSFeatureBits(BD_ADDR remote_bda);

/*****************************************************************************
**  TBFC FUNCTIONS
*****************************************************************************/
#if BTM_TBFC_INCLUDED == TRUE
/*******************************************************************************
**
** Function         BTM_GetLinkTBFCMode
**
** Description      Get link current TBFC mode
**
** Returns          BTM_UNKNOWN_ADDR if no address founded. otherwise p_mode to
**                  carry the TBFC mode.
**
*******************************************************************************/
BT_API extern tBTM_STATUS BTM_GetLinkTBFCMode(BD_ADDR bd_addr, UINT8 *p_mode);

/*******************************************************************************
**
** Function         BTM_SetRemoteDevTBFCMode
**
** Description      TBFC create connection command
**
** Returns          BTM_ILLEGAL_VALUE/BTM_NO_RESOURCES if suspend failed, otherwise
**                  sucessfully started
**
*******************************************************************************/
BT_API extern tBTM_STATUS BTM_SetRemoteDevTBFCMode(BD_ADDR bd_addr, tBTM_SCAN_MODE scan_mode);

/*******************************************************************************
**
** Function         BTM_TBFCSuspend
**
** Description      TBFC suspend command
**
** Returns          BTM_ILLEGAL_VALUE/BTM_NO_RESOURCES if suspend failed, otherwise
**                  sucessful started
**
*******************************************************************************/
BT_API extern tBTM_STATUS BTM_TBFCSuspend(UINT8 pm_id, BD_ADDR bd_addr);

/*******************************************************************************
**
** Function         BTM_TBFCCreateConnection
**
** Description      TBFC create connection command
**
** Returns          BTM_ILLEGAL_VALUE/BTM_NO_RESOURCES if suspend failed, otherwise
**                  sucessfully started
**
*******************************************************************************/
BT_API extern tBTM_STATUS BTM_TBFCCreateConnection(BD_ADDR bd_addr, UINT16 packet_types);

/*******************************************************************************
**
** Function         BTM_WriteTBFCScanEnable
**
** Description      Enable or Disable TBFC scan on local device
**
** Returns          BTM_NO_RESOURCES if suspend failed, otherwise
**                  sucessfully started
**
*******************************************************************************/
BT_API extern tBTM_STATUS BTM_WriteTBFCScanEnable(BOOLEAN enable);


#if (defined BTM_BI_DIR_TBFC_INCLUDED && BTM_BI_DIR_TBFC_INCLUDED == TRUE)
/*******************************************************************************
**
** Function         BTM_TBFCSuspend
**
** Description      TBFC resume command
**
** Returns          BTM_ILLEGAL_VALUE/BTM_NO_RESOURCES if suspend failed, otherwise
**                  sucessful started
**
*******************************************************************************/
BT_API extern tBTM_STATUS BTM_TBFCResume(BD_ADDR bd_addr);
#endif
#endif
/*******************************************************************************
**              BROADCOM CUSTOMER SPECIFIC BLE FEATURE FUNCTIONS
******************************************************************************/
#if BLE_BRCM_INCLUDED == TRUE
/*******************************************************************************
**
** Function         BTM_BleEnableFilterCondition
**
** Description      This function is called to enable the adv data payload filter
**                  condition.
**
** Parameters       p_target: enabble the filter condition on a target device; if NULL
**                            enable the generic scan condition.
**                  enable: enable or disable the filter condition
**
** Returns          void
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleEnableFilterCondition(BOOLEAN enable,
        tBLE_BD_ADDR *p_target,
        tBTM_BLE_PF_CMPL_CBACK *p_cmpl_cback);

/*******************************************************************************
**
** Function         BTM_BleCfgFilterCondition
**
** Description      This function is called to configure the adv data payload filter
**                  condition.
**
** Parameters       action: to read/write/clear
**                  cond_type: filter condition type.
**                  p_cond: filter condition paramter
**
** Returns          tBTM_STATUS
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleCfgFilterCondition(tBTM_BLE_SCAN_COND_OP action,
        tBTM_BLE_PF_COND_TYPE cond_type,
        tBTM_BLE_PF_COND_PARAM *p_cond,
        tBTM_BLE_PF_CMPL_CBACK *p_cmpl_cback);


/*******************************************************************************
**
** Function         BTM_BleMonitorLinkRSSI
**
** Description      This function start RSSI monitoring on a LE connection
**
** Parameters       conn_addr: BLE connection address to be monitored.
**                  alert_mask: alert mask, if 0 indicate turn off the monitoring
**                  low_threshold: lowest rssi threshold
**                  range: threshold between low and high
**                  hi_threshold: highest threshold
**                  p_cback: ADV RSSI alert indication callback.
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleMonitorLinkRSSI(BD_ADDR conn_addr,
        tBTM_BLE_RSSI_ALERT_MASK  alert_mask,
        INT8 low_threshold,
        INT8 range,
        INT8 hi_threshold,
        tBTM_BLE_RSSI_CBACK *p_rssi_cback);

/*******************************************************************************
**
** Function         BTM_BleMonitorAdvRSSI
**
** Description      This function start RSSI monitoring on a LE connection
**
** Parameters       alert_mask: alert mask, if 0 indicate turn off the monitoring
**                  low_threshold: lowest rssi threshold
**                  range: threshold between low and high
**                  hi_threshold: highest threshold
**                  p_cback: ADV RSSI alert indication callback.
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleMonitorAdvRSSI(tBTM_BLE_RSSI_ALERT_MASK  alert_mask,
        INT8 low_threshold,
        INT8 range,
        INT8 hi_threshold,
        tBTM_BLE_RSSI_CBACK *p_cback);


/*******************************************************************************
**
** Function         BTM_BleScatternetEnable
**
** Description      This function enable scatternet
**
** Parameters       enable: turn on or off
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleScatternetEnable(BOOLEAN enable);

/*******************************************************************************
**
** Function         BTM_BleDynamicScanEnable
**
** Description      This function enable concurrent initator and scanner capability
**
** Parameters       enable: turn on or off
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleDynamicScanEnable(BOOLEAN enable);

/*******************************************************************************
**
** Function         BTM_BleSendBurstData
**
** Description      This function sends a burst of data in GATT channel
**
** Parameters       remote_bda: remote device address
**                  p_buf: pointer to the data buffer to be sent. It requires the
**                         BT_HDR offset set to be (L2CAP_MIN_OFFSET + 1), length
**                         set as the data length, while data to be filled starting
**                         from offset.
**
** Returns          TRUE if data is sent to L2CAP sucessfully, otherwise failed.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BleSendBurstData(BD_ADDR remote_bda, BT_HDR *p_buf);

/*******************************************************************************
**
** Function         BTM_RegisterForBurstData
**
** Description      This function registers the Burst data handler function
**
** Parameters       p_cback: callback function pointer.
**
** Returns          None.
**
*******************************************************************************/
BTM_API extern void BTM_RegisterForBurstData(tBTM_BURST_CBACK *p_cback);

#if 0  /* multi adv is not included */
/*******************************************************************************/
/*                          Multi ADV API                                      */
/*******************************************************************************
**
** Function         BTM_BleEnableAdvInstance
**
** Description      This function enable a Multi-ADV instance with the specified
**                  adv parameters
**
** Parameters       p_params: pointer to the adv parameter structure, set as default
**                            adv parameter when the instance is enabled.
**                  p_cback: callback function for the adv instance.
**                  p_ref:  reference data attach to the adv instance to be enabled.
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleEnableAdvInstance(tBTM_BLE_ADV_PARAMS *p_params,
        tBTM_BLE_MULTI_ADV_CBACK *p_cback,
        void *p_ref);

/*******************************************************************************
**
** Function         BTM_BleUpdateAdvInstParam
**
** Description      This function update a Multi-ADV instance with the specified
**                  adv parameters.
**
** Parameters       inst_id: adv instance ID
**                  p_params: pointer to the adv parameter structure.
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleUpdateAdvInstParam(UINT8 inst_id, tBTM_BLE_ADV_PARAMS *p_params);

/*******************************************************************************
**
** Function         BTM_BleCfgAdvInstData
**
** Description      This function configure a Multi-ADV instance with the specified
**                  adv data or scan response data.
**
** Parameters       inst_id: adv instance ID
**                  is_scan_rsp: is this scacn response, if no set as adv data.
**                  data_mask: adv data mask.
**                  p_data: pointer to the adv data structure.
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleCfgAdvInstData(UINT8 inst_id, BOOLEAN is_scan_rsp,
        tBTM_BLE_AD_MASK data_mask,
        tBTM_BLE_ADV_DATA *p_data);

/*******************************************************************************
**
** Function         BTM_BleDisableAdvInstance
**
** Description      This function disables a Multi-ADV instance.
**
** Parameters       inst_id: adv instance ID
**
** Returns          status
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BleDisableAdvInstance(UINT8 inst_id);
#endif

#endif

#ifdef __cplusplus
}
#endif

#endif
