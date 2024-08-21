/*****************************************************************************
**
**  Name:          wiced_bt_int.h
**
**  Description:   Internal definitions for wiced bt wrapper
**
**
**  Copyright (c) 2014, Broadcom Corp, All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#pragma once

/* WICED -> BTE Conversion  */
#define int8_t          INT8
#define int16_t         INT16
#define int32_t         INT32
#define uint8_t         UINT8
#define uint16_t        UINT16
#define uint32_t        UINT32
/*
 * TODO: wiced_bool_t(enum) should not be redefined to BOOLEAN(uint8).
 * It will lead to certain data-type mismatch errors and wrong calculations in DCT if wiced_bt_int.h
 * overrides the Wiced-SDK's definition of wiced_bool_t.
 * This change might possibly break BTEWICED build and need further discussion.
 */
//#define wiced_bool_t    BOOLEAN

#define wiced_bt_dev_status_t                                   tBTM_STATUS
#define wiced_bt_management_evt_t                               tBTM_EVENT
#define wiced_bt_dev_class_t                                    DEV_CLASS
#define wiced_bt_device_address_t                               BD_ADDR
#define wiced_bt_device_address_ptr_t                           BD_ADDR_PTR
#define wiced_bt_dev_io_cap_t                                   tBTM_IO_CAP
#define wiced_bt_dev_oob_data_t                                 tBTM_OOB_DATA
#define wiced_bt_dev_le_auth_req_t                              tBTM_LE_AUTH_REQ
#define wiced_bt_dev_le_key_type_t                              tBTM_LE_KEY_TYPE
#define wiced_bt_dev_auth_req_t                                 tBTM_AUTH_REQ
#define wiced_bt_device_type_t                                  tBT_DEVICE_TYPE
#define wiced_bt_dev_bonded_device_info_t                       tBT_BONDED_DEVICE_INFO_TYPE
#define wiced_bt_transport_t                                    tBT_TRANSPORT
#define wiced_bt_ble_address_t                                  tBLE_BD_ADDR
#define wiced_bt_ble_address_type_t                             tBLE_ADDR_TYPE
#define wiced_bt_ble_conn_mode_t                                tBLE_CONN_MODE
#define wiced_bt_dev_passkey_entry_type_t                       tBTM_SP_KEY_TYPE
#define wiced_bt_connection_status_change_cback_t               tBTM_ACL_DB_CHANGE_CB   /* modified from bte */
#define wiced_bt_dev_inq_parms_t                                tBTM_INQ_PARMS          /* modified from bte */
#define wiced_bt_dev_inquiry_scan_result_t                      tBTM_INQ_RESULTS        /* modified from bte */
#define wiced_bt_inquiry_result_cback_t                         tBTM_INQ_RESULTS_CB
#define wiced_bt_dev_vendor_specific_command_complete_cback_t   tBTM_VSC_CMPL_CB
#ifndef wiced_bt_dev_vendor_specific_command_complete_params_t
#define wiced_bt_dev_vendor_specific_command_complete_params_t  tBTM_VSC_CMPL
#endif
#define wiced_bt_dev_vendor_specific_event_callback_t           tBTM_VS_EVT_CB
#define wiced_bt_ble_advert_elem_t                              tBTM_BLE_ADV_ELEM
#define wiced_bt_ble_conn_type_t                                tBTM_BLE_CONN_TYPE
#define wiced_bt_ble_selective_conn_cback_t                     tBTM_BLE_SEL_CBACK
#define wiced_bt_ble_scan_type_t                                tBTM_BLE_SCAN_TYPE          /* new */
#define wiced_bt_ble_advert_mode_t                              tBTM_BLE_AVERT_MODE         /* new */
#define wiced_bt_ble_scan_result_cback_t                        tBTM_BLE_SCAN_RESULT_CBACK  /* new */
#define wiced_bt_ble_scan_results_t                             tBTM_BLE_SCAN_RESULT        /* new */
#define wiced_bt_ble_scan_mode_t                                tBTM_BLE_SCAN_MODE
#define wiced_bt_ble_advert_chnl_map_t                          tBTM_BLE_ADV_CHNL_MAP
#define wiced_dev_ble_signature_t                               BLE_SIGNATURE
#define wiced_bt_ble_advert_filter_policy_t                     tBTM_BLE_AFP
#define wiced_bt_device_link_keys_t                             tBTM_PAIRED_DEVICE_LINK_KEYS
#define wiced_bt_local_identity_keys_t                          tBTM_LOCAL_IDENTITY_KEYS
#define wiced_bt_ble_scanner_filter_policy_t                    tBTM_BLE_SFP


/* l2cap wiced-to-bte translation */
#define wiced_bt_l2c_appl_info_t                                tL2CAP_APPL_INFO
#define wiced_bt_l2c_fixed_chnl_reg_t                           tL2CAP_FIXED_CHNL_REG
#define wiced_bt_l2cap_le_appl_information_t                    tL2CAP_LE_APPL_INFO
#define wiced_bt_l2c_cfg_info_t                                 tL2CAP_CFG_INFO
#define wiced_bt_l2c_ch_cfg_bits_t                              tL2CAP_CH_CFG_BITS
#define wiced_bt_l2cap_ertm_information_t                       tL2CAP_ERTM_INFO
#define wiced_bt_l2cap_chnl_priority_t                          tL2CAP_CHNL_PRIORITY
#define wiced_bt_l2cap_chnl_data_rate_t                         tL2CAP_CHNL_DATA_RATE

#define wiced_bt_gatt_appearance_t                              tGATT_APPEARANCE
#define wiced_bt_gatt_status_t                                  tGATT_STATUS
#define wiced_bt_gatt_write_t                                   tGATT_WRITE_REQ
#define wiced_bt_gatt_read_t                                    tGATT_READ_REQ
#define wiced_bt_gatt_exec_flag_t                               tGATT_EXEC_FLAG
#define wiced_gattdb_entry_t                                    LEGATTDB_ENTRY_HDR

#define wiced_bt_hidd_status_t                                  tHID_STATUS
#define wiced_bt_hidd_reg_info_t                                tHID_DEV_REG_INFO
#define wiced_bt_hidd_callback_t                                tHID_DEV_CBACK_DATA

#define wiced_bt_sco_enh_esco_params_t                          tBTM_ENH_ESCO_PARAMS

#define wiced_bt_gap_ble_attr_value_t                           tGAP_BLE_ATTR_VALUE
#ifdef MPAF_CUSTOM_STACK
/* HCI trace call back */
#define wiced_bt_hci_trace_cback_t                              BTU_HCI_TRACE_CALLBACK
#define wiced_bt_hci_trace_type_t                               bte_glue_hci_trace_type_t
#endif

#define wiced_bt_link_key_t                                     LINK_KEY
#define wiced_bt_ble_keys_t                                     tBTM_SEC_BLE_KEYS
#define wiced_bt_device_sec_keys_t                              tBTM_SEC_KEYS
