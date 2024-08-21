/*****************************************************************************
**
**  Name:          btm_coex_api.h
**
**  Description:   This file contains the Bluetooth Manager (BTM) API function
**                 external definitions for coexistence functions.
**
**  Copyright (c) 1999-2013, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#ifndef BTM_COEX_API_H
#define BTM_COEX_API_H

#include "hcimsgs.h"
#include "btm_api.h"

/*****************************************************************************
**  EXTERNAL FUNCTION DECLARATIONS
*****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
**
** Function         BTM_BuildAfhChannelClass
**
** Description      This function builds AFH Host Channel Classification bit map.
**                  It sets [first, last] bits to 0 (bad) and all others bits to
**                  1 (unknown).
**
** Parameters:      class_map   - pointer to the map to build
**                  first       - position of the first bit to set to 0
**                  last        - position of the last bit to set to 0
**
** Return           FALSE if first > last or last > HCI_AFH_CHANNEL_MAX_BIT.
**                  In this case bit map is not modified.
**
*******************************************************************************/
BTM_API extern BOOLEAN BTM_BuildAfhChannelClass(UINT8 class_map[HCI_AFH_CHANNEL_MAP_LEN], UINT8 first, UINT8 last);

/*******************************************************************************
**
** Function         BTM_BuildAndSetAfhChannelClass
**
** Description      This function builds AFH Host Channel Classification bit map
**                  and send it to controller.
**                  To build AFH Host Channel Classification bit map it calls
**                  BTM_BuildAfhChannelClass(...).
**                  To send the map to controller it calls BTM_SetAfhChannelClass(...).
**
** Parameters:      first       - position of the first bit to set to 0
**                  last        - position of the last bit to set to 0
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_BuildAndSetAfhChannelClass(UINT8 first, UINT8 last);

/*****************************************************************************
**  Coexistence Functions
*****************************************************************************/
enum
{
    BTM_COEX_SET_AFH_CHANNELS_EVT,          /* received HCI_SET_AFH_CHANNELS command complete or status event */
#if (BLE_INCLUDED == TRUE)
    BTM_COEX_SET_BLE_CHNL_CLASS_EVT,        /* received HCI_BLE_SET_HOST_CHNL_CLASS command complete or status event */
#endif
    BTM_COEX_SET_MWS_CHAN_PARAMS_EVT,       /* received HCI_SET_MWS_CHANNEL_PARAMETERS command complete or status event */
    BTM_COEX_SET_EXTRN_FRAME_CFG_EVT,       /* received HCI_SET_EXTERNAL_FRAME_CONFIGURATION command complete or status event */
    BTM_COEX_SET_MWS_SIGNALING_EVT,         /* received HCI_SET_MWS_SIGNALING command complete or status event */
    BTM_COEX_SET_MWS_TRANS_LAYER_EVT,       /* received HCI_SET_MWS_TRANSPORT_LAYER command complete or status event */
    BTM_COEX_SET_MWS_SCAN_FREQ_TABLE_EVT,   /* received HCI_SET_MWS_SCAN_FREQUENCY_TABLE command complete or status event */
    BTM_COEX_SET_MWS_PATTERN_CFG_EVT,       /* received HCI_SET_MWS_PATTERN_CONFIGURATION command complete or status event */
    BTM_COEX_GET_MWS_TRANS_LAYER_EVT,       /* received HCI_GET_MWS_TRANS_LAYER_CFG command complete or status event */
    BTM_COEX_INVALD_EVT
};
typedef UINT8 tBTM_COEX_EVT;


/* tBTM_COEX_STATUS_PARAM is used with:
    BTM_COEX_SET_AFH_CHANNELS_EVT,
    BTM_COEX_SET_BLE_CHNL_CLASS_EVT,
    BTM_COEX_SET_MWS_CHAN_PARAMS_EVT,
    BTM_COEX_SET_EXTRN_FRAME_CFG_EVT,
    BTM_COEX_SET_MWS_TRANS_LAYER_EVT,
    BTM_COEX_SET_MWS_SCAN_FREQ_TABLE_EVT,
    BTM_COEX_SET_MWS_PATTERN_CFG_EVT,

*/
typedef struct
{
    UINT8   status;
} tBTM_COEX_STATUS_PARAM;

/* data type for BTM_COEX_SET_MWS_SIGNALING_EVT */
typedef struct
{
    UINT8       status;
    UINT16      bl_rx_pri_assert_offset;
    UINT16      bl_rx_pri_assert_jitter;
    UINT16      bl_rx_pri_deassert_offset;
    UINT16      bl_rx_pri_deassert_jitter;
    UINT16      p802_rx_pri_assert_offset;
    UINT16      p802_rx_pri_assert_jitter;
    UINT16      p802_rx_pri_deassert_offset;
    UINT16      p802_rx_pri_deassert_jitter;
    UINT16      bl_tx_on_assert_offset;
    UINT16      bl_tx_on_assert_jitter;
    UINT16      bl_tx_on_deassert_offset;
    UINT16      bl_tx_on_deassert_jitter;
    UINT16      p802_tx_on_assert_offset;
    UINT16      p802_tx_on_assert_jitter;
    UINT16      p802_tx_on_deassert_offset;
    UINT16      p802_tx_on_deassert_jitter;
} tBTM_COEX_SET_MWS_SIGNALING_EVT_PARAMS;

/* data type for BTM_COEX_GET_MWS_TRANS_LAYER_EVT */
#define BTM_MWS_NUM_TRANSPS_SUPPORTED                   HCI_MWS_NUM_TRANSPS_SUPPORTED
#define BTM_MWS_NUM_BAUD_RATES_ON_ONE_TRANSP_SUPPORTED  HCI_MWS_NUM_BAUD_RATES_ON_ONE_TRANSP_SUPPORTED
#define BTM_MWS_OVERALL_NUM_BAUD_RATES_SUPPORTED        (BTM_MWS_NUM_TRANSPS_SUPPORTED * BTM_MWS_NUM_BAUD_RATES_ON_ONE_TRANSP_SUPPORTED)

typedef struct
{
    UINT8       status;
    UINT8       num_transps;
    UINT8       transp_layer[BTM_MWS_NUM_TRANSPS_SUPPORTED];
    UINT8       num_baud_rates[BTM_MWS_NUM_TRANSPS_SUPPORTED];
    UINT32      to_mws_baud_rate[BTM_MWS_OVERALL_NUM_BAUD_RATES_SUPPORTED];
    UINT32      from_mws_baud_rate[BTM_MWS_OVERALL_NUM_BAUD_RATES_SUPPORTED];
} tBTM_COEX_GET_MWS_TRANS_LAYER_EVT_PARAMS;

typedef union
{
    tBTM_COEX_STATUS_PARAM                      set_afh_chan_class_res;         /* BTM_COEX_SET_AFH_CHANNELS_EVT */
#if (BLE_INCLUDED == TRUE)
    tBTM_COEX_STATUS_PARAM                      set_ble_chan_class_res;         /* BTM_COEX_SET_BLE_CHNL_CLASS_EVT */
#endif
    tBTM_COEX_STATUS_PARAM                      set_mws_chan_params_res;        /* BTM_COEX_SET_MWS_CHAN_PARAMS_EVT */
    tBTM_COEX_STATUS_PARAM                      set_extrn_frame_cfg_res;        /* BTM_COEX_SET_EXTRN_FRAME_CFG_EVT */
    tBTM_COEX_SET_MWS_SIGNALING_EVT_PARAMS      set_mws_signaling_res;          /* BTM_COEX_SET_MWS_SIGNALING_EVT */
    tBTM_COEX_STATUS_PARAM                      set_mws_trans_layer_res;        /* BTM_COEX_SET_MWS_TRANS_LAYER_EVT */
    tBTM_COEX_STATUS_PARAM                      set_mws_scan_freq_table_res;    /* BTM_COEX_SET_MWS_SCAN_FREQ_TABLE_EVT */
    tBTM_COEX_STATUS_PARAM                      set_mws_pattern_cfg_res;        /* BTM_COEX_SET_MWS_PATTERN_CFG_EVT */
    tBTM_COEX_GET_MWS_TRANS_LAYER_EVT_PARAMS    get_mws_trans_layer_cfg_res;    /* BTM_COEX_GET_MWS_TRANS_LAYER_EVT */
} tBTM_COEX_EVT_PARAMS;

/* Coexistence events callback */
typedef UINT8(tBTM_COEX_CBACK)(tBTM_COEX_EVT event, UINT8 ctr_id, tBTM_COEX_EVT_PARAMS *p_data);


BTM_API extern void BTM_RegisterCoexCback(tBTM_COEX_CBACK *p_callback);


#define BTM_AFH_CHNL_MAP_SIZE    HCI_AFH_CHANNEL_MAP_LEN
/*******************************************************************************
**
** Function         BTM_SetAfhChannelClass
**
** Description      This function is called to send HCI_SET_AFH_CHANNELS command
**                  to BR/EDR controller.
**
** Parameters:      afh_channel_map     - AFH Host Channel Classification array
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetAfhChannelClass(const UINT8 afh_channel_map[BTM_AFH_CHNL_MAP_SIZE]);


#if BLE_INCLUDED == TRUE
#define BTM_BLE_CHNL_MAP_SIZE    HCI_BLE_CHNL_MAP_SIZE
/*******************************************************************************
**
** Function         BTM_SetBleChannelClass
**
** Description      This function is called to send HCI_BLE_SET_HOST_CHNL_CLASS
**                  command to LE controller.
**
** Parameters:      ble_channel_map     - LE Host Channel Classification array
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetBleChannelClass(const UINT8 ble_channel_map[BTM_BLE_CHNL_MAP_SIZE]);
#endif


/* structure that describes parameters used by BTM_SetMwsChannParam */
typedef tHCI_SET_MWS_CHAN_CMD_PARAMS    tBTM_SET_MWS_CHAN_OP_PARAMS;
/*******************************************************************************
**
** Function         BTM_SetMwsChannParam
**
** Description      This function is called to send HCI_SET_MWS_CHANNEL_PARAMETERS
**                  command to BR_EDR/LE/AMP controller (to inform the controller
**                  about MWS channel configuration).
**
** Parameters:      local_controller_id             - controller ID of the controller to send command to
**                                                    0 - primary (BR/EDR or LE), >0 - secondary (AMP)
**                  p_btm_set_mws_chan_op_params    - pointer to the structure that describes
**                                                    operation parameters.
**                  The structure elements:
**                      channel_enable          - disable (0) / enable (1) MWS channel
**                      rx_center_frequency     - center frequency of MWS downlink channel (MHz)
**                      tx_center_frequency     - center frequency of MWS uplink channel (MHz)
**                      rx_channel_bandwidth    - bandwidth of MWS downlink channel (kHz)
**                      tx_channel_bandwidth    - bandwidth of MWS uplink channel (kHz)
**                      channel_type            - MWS channel type (0 - TDD, 1 - FDD)
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetMwsChannParam(const UINT8 local_controller_id,
        const tBTM_SET_MWS_CHAN_OP_PARAMS *p_btm_set_mws_chan_op_params);


#define BTM_MWS_NUM_PERIODS_SUPPORTED   HCI_MWS_NUM_PERIODS_SUPPORTED
/* structure that describes parameters used by BTM_SetExtFrameConfig */
typedef tHCI_SET_EXT_FRAME_CFG_CMD_PARAMS   tBTM_SET_EXT_FRAME_CFG_OP_PARAMS;
/*******************************************************************************
**
** Function         BTM_SetExtFrameConfig
**
** Description      This function is called to send HCI_SET_EXTERNAL_FRAME_CONFIGURATION
**                  command to BR_EDR/LE/AMP controller (to describe a frame structure
**                  to the the controller).
**
** Parameters:      local_controller_id                 - controller ID of the controller to send command to
**                                                        0 - primary (BR/EDR or LE), >0 - secondary (AMP)
**                  p_btm_set_ext_frame_cfg_op_params   - pointer to the structure that describes
**                                                        operation parameters.
**                  The structure elements:
**                      ext_frame_duration              - frame duration (microseconds)
**                      ext_frame_sync_assert_offset    - the time from the start of the next MWS frame to
**                                                        the FRAME_SYNC sgnal (microseconds)
**                      ext_frame_sync_assert_jitter    - the frame sync jitter (microseconds)
**                      ext_num_periods                 - number of specified periods in the frame
**                      duration_array                  - duration of periods (microseconds)
**                      type_array                      - type of periods
**                                                        0 - downlink
**                                                        1 - uplink
**                                                        2 - bi-directional
**                                                        3 - guard
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetExtFrameConfig(const UINT8 local_controller_id,
        const tBTM_SET_EXT_FRAME_CFG_OP_PARAMS *p_btm_set_ext_frame_cfg_op_params);


/* structure that describes parameters used by BTM_SetMwsSignaling */
typedef tHCI_SET_MWS_SIGNALING_CMD_PARAMS   tBTM_SET_MWS_SIGNALING_OP_PARAMS;
/*******************************************************************************
**
** Function         BTM_SetMwsSignaling
**
** Description      This function is called to send HCI_SET_MWS_SIGNALING command
**                  to BR_EDR/LE/AMP controller (to inform the controller of the
**                  MWS signaling interface logical layer parameters).
**
** Parameters:      local_controller_id                 - controller ID of the controller to send command to
**                                                        0 - primary (BR/EDR or LE), >0 - secondary (AMP)
**                  p_btm_set_mws_signaling_op_params   - pointer to the structure that describes
**                                                        operation parameters.
**                  The structure elements:
**                      rx_assert_offset                    - MWS_RX signal assert offset (microseconds)
**                      rx_assert_jitter                    - MWS_RX signal assert jitter (microseconds)
**                      rx_deassert_offset                  - MWS_RX signal de-assert offset (microseconds)
**                      rx_deassert_jitter                  - MWS_RX signal de-assert jitter (microseconds)
**                      tx_assert_offset                    - MWS_TX signal assert offset (microseconds)
**                      tx_assert_jitter                    - MWS_TX signal assert jitter (microseconds)
**                      tx_deassert_offset                  - MWS_TX signal de_assert offset (microseconds)
**                      tx_deassert_jitter                  - MWS_TX signal de-assert jitter (microseconds)
**                      pattern_assert_offset               - MWS_PATTERN signal assert offset (microseconds)
**                      pattern_assert_jitter               - MWS_PATTERN signal assert jitter (microseconds)
**                      inactivity_duration_assert_offset   - MWS_INACTIVITY_DURATION signal assert offset (microseconds)
**                      inactivity_duration_assert_jitter   - MWS_INACTIVITY_DURATION signal assert jitter (microseconds)
**                      scan_frequency_assert_offset        - MWS_SCAN_FREQUENCY signal assert offset (microseconds)
**                      scan_frequency_assert_jitter        - MWS_SCAN_FREQUENCY signal assert jitter (microseconds)
**                      priority_assert_offset_request      - mimimum advance notification from the beginning of an MWS
**                                                            uplink period before which the BLUETOOTH_RX_PRI signal
**                                                            shall be asserted to be recognized by the MWS (microseconds)
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetMwsSignaling(const UINT8 local_controller_id,
        const tBTM_SET_MWS_SIGNALING_OP_PARAMS *p_btm_set_mws_signaling_op_params);


/* structure that describes parameters used by BTM_SetMwsTransportLayer */
typedef tHCI_SET_MWS_TRANSP_LAYER_CMD_PARAMS    tBTM_SET_MWS_TRANSP_LAYER_OP_PARAMS;
/*******************************************************************************
**
** Function         BTM_SetMwsTransportLayer
**
** Description      This function is called to send HCI_SET_MWS_TRANSPORT_LAYER
**                  command to BR_EDR/LE/AMP controller (to select the MWS coexistence
**                  signaling thransport layer in the controller).
**
** Parameters:      local_controller_id                     - controller ID of the controller to send command to
**                                                            0 - primary (BR/EDR or LE), >0 - secondary (AMP)
**                  p_btm_set_mws_transp_layer_op_params    - pointer to the structure that describes
**                                                            operation parameters.
**                  The structure elements:
**                      transport_layer         - selected transport layer
**                                                0 - disabled
**                                                1 - WCI-1 Transport
**                                                2 - WCI-2 Transport
**                      to_MWS_baud_rate        - baud rate in the Bluetooth to MWS direction (baud)
**                      from_MWS_baud_rate      - baud rate in the MWS to Bluetooth direction (baud)
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetMwsTransportLayer(const UINT8 local_controller_id,
        const tBTM_SET_MWS_TRANSP_LAYER_OP_PARAMS *p_btm_set_mws_transp_layer_op_params);


#define BTM_MWS_NUM_SCAN_FREQS_SUPPORTED    HCI_MWS_NUM_SCAN_FREQS_SUPPORTED
/* structure that describes parameters used by BTM_SetMwsScanFrequencyTable */
typedef tHCI_SET_MWS_SCAN_FREQ_TABLE_CMD_PARAMS tBTM_SET_MWS_SCAN_FREQ_TABLE_OP_PARAMS;
/*******************************************************************************
**
** Function         BTM_SetMwsScanFrequencyTable
**
** Description      This function is called to send HCI_SET_MWS_SCAN_FREQUENCY_TABLE
**                  command to BR_EDR/LE/AMP controller (to configure the MWS scan
**                  frequency table in the controller).
**
** Parameters:      local_controller_id     - controller ID of the controller to send command to
**                                            0 - primary (BR/EDR or LE), >0 - secondary (AMP)
**                  p_btm_set_mws_scan_freq_table_op_params - pointer to the structure that describes
**                                                        operation parameters.
**                  The structure elements:
**                      num_scan_frequency      - number of MWS scan frequencies to be set (0-8)
**                      scan_frequency_low      - lower edges for scan frequencies (MHz)
**                      scan_frequency_high     - upper edges for scan frequencies (MHz)
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetMwsScanFrequencyTable(const UINT8 local_controller_id,
        const tBTM_SET_MWS_SCAN_FREQ_TABLE_OP_PARAMS *p_btm_set_mws_scan_freq_table_op_params);


#define BTM_MWS_PATTERNS_NUM_INTERVS_SUPPORTED  HCI_MWS_PATTERNS_NUM_INTERVS_SUPPORTED
/* structure that describes parameters used by BTM_SetMwsPatternConfiguration */
typedef tHCI_SET_MWS_PATTERN_CMD_PARAMS tBTM_SET_MWS_PATTERN_OP_PARAMS;
/*******************************************************************************
**
** Function         BTM_SetMwsPatternConfiguration
**
** Description      This function is called to send HCI_SET_MWS_PATTERN_CONFIGURATION
**                  command to BR_EDR/LE/AMP controller (to specify the configuration
*                   of the pattern indicated over the MWS Coexistence Transport Layer).
**
** Parameters:      local_controller_id         - controller ID of the controller to send command to
**                                                0 - primary (BR/EDR or LE), >0 - secondary (AMP)
**                  p_btm_set_mws_pattern_op_params - pointer to the structure that describes
**                                                        operation parameters.
**                  The structure elements:
**                  pattern_index               - indexs of the MWS_PATTERN instance to
**                                                be configured (0-2)
**                  pattern_num_intervals       - the number of Bluetooth activity intervals
**                  pattern_intervl_duration    - Bluetooth activity intevals duration (microseconds)
**                  pattern_interval_type       - permitted Bluetooth activities in the intervals
**                                                0 - neither transmission nor reception are allowed
**                                                1 - transmission is allowed
**                                                2 - reception is allowed
**                                                3 - both transmission and reception are allowed
**                                                4 - interwal for MWS frame as defined by the
**                                                    HCI_SET_EXTERNAL_FRAME_CONFIGURATION command
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_SetMwsPatternConfiguration(const UINT8 local_controller_id,
        const tBTM_SET_MWS_PATTERN_OP_PARAMS *p_btm_set_mws_pattern_op_params);

/*******************************************************************************
**
** Function         BTM_GetMwsTransportLayerConfiguration
**
** Description      This function is called to send HCI_GET_MWS_TRANS_LAYER_CFG
**                  command to BR_EDR/LE/AMP controller (to request the baud rates
**                  supported by the controller forb the transport layer).
**
** Parameters:      local_controller_id     - controller ID of the controller to send command to
**                                            0 - primary (BR/EDR or LE), >0 - secondary (AMP)
**
*******************************************************************************/
BTM_API extern tBTM_STATUS BTM_GetMwsTransportLayerConfiguration(UINT8 local_controller_id);

#ifdef __cplusplus
}
#endif

#endif /* BTM_COEX_API_H */
