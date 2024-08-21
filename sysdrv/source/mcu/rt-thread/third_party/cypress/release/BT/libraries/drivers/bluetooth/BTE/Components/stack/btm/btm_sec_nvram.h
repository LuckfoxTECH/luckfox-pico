/*****************************************************************************
**
**  Name:          btm_sec_nvram.h
**
**  Description:   This file contains definition of managing link key storage inside bt stack
**
**  Copyright (c) 1999-2014, Broadcom Corp., All Rights Reserved
**  Broadcom Bluetooth Core. Proprietary and confidential.
******************************************************************************/
#pragma once

#include "btm_int.h"
#include "btm_nvram_access.h"

#ifndef BTM_SEC_NVRAM_KEYBLOB_MAX_LEN
/* Maximum buffer size needed to hold a bonded devce's link keys */
#define BTM_SEC_NVRAM_KEYBLOB_MAX_LEN           146  /*(sizeof(tBTM_SEC_KEY_ELEM_HDR)*7 + 6*BT_OCTET16_LEN + BT_OCTET8_LEN + + BD_ADDR_LEN + 22 )*/
#endif

#ifndef BTM_SEC_NVRAM_BONDED_DEVICE_ENTRY_MAX_LEN
/* Size of paired device entry (header + keyblob maxlen) */
#define BTM_SEC_NVRAM_BONDED_DEVICE_ENTRY_MAX_LEN  (sizeof(wiced_bt_nvram_access_entry_t) - 1 + BTM_SEC_NVRAM_KEYBLOB_MAX_LEN)
#endif


/* Default nvram access functions for saving/retrieving link keys */
extern void wiced_bt_nvram_access_init(void);
extern wiced_result_t wiced_bt_nvram_access_get_bonded_devices(wiced_bt_dev_bonded_device_info_t paired_device_list[], uint16_t *p_num_devices);
extern wiced_result_t wiced_bt_nvram_access_save_bonded_device_key(wiced_bt_device_address_t bd_addr, wiced_bt_ble_address_type_t addr_type, uint8_t device_type, uint8_t *p_keyblobs, uint16_t key_len);
extern wiced_result_t wiced_bt_nvram_access_load_bonded_device_keys(wiced_bt_device_address_t bd_addr, wiced_bt_nvram_access_entry_t *p_key_entry, uint8_t entry_max_length);
extern wiced_result_t wiced_bt_nvram_access_delete_bonded_device(wiced_bt_device_address_t bd_addr);
extern wiced_result_t wiced_bt_nvram_access_load_local_identity_keys(wiced_bt_local_identity_keys_t *p_lkeys);
extern wiced_result_t wiced_bt_nvram_access_save_local_identity_keys(wiced_bt_local_identity_keys_t *p_lkeys);
extern wiced_bool_t wiced_bt_nvram_access_key_storage_available(wiced_bt_device_address_t bd_addr, int req_size);
extern wiced_result_t wiced_bt_nvram_access_enum_bonded_device_keys(int8_t *p_index, wiced_bt_nvram_access_entry_t *p_key_entry, uint8_t entry_max_length);

extern wiced_bt_nvram_access_t              *p_btm_nvram_access;     /* current nvram access functions */

/* Stored key type for KEY_ELEM_HDR */
#define BTM_STORE_KEY_TYPE_BR_EDR_LKEY      0x00ff    /* BR-EDR link key*/
#define BTM_STORE_KEY_TYPE_BLE_PENC_KEY     0x0002    /* Peer encryption Key*/
#define BTM_STORE_KEY_TYPE_BLE_PCSRK_KEY    0x0004    /* Peer SRK */
#define BTM_STORE_KEY_TYPE_BLE_LENC_KEY     0x0008    /* Master role security information: div*/
#define BTM_STORE_KEY_TYPE_BLE_LCSRK_KEY    0x0010    /* Master device ID key*/
#define BTM_STORE_KEY_TYPE_BLE_PID_KEY      0x0020    /* Peer Identity key*/


typedef struct
{
    UINT16 key_type;
} tBTM_SEC_KEY_ELEM_HDR;

typedef struct
{
    tBTM_SEC_KEY_ELEM_HDR  key_hdr;
    UINT8  key_data[1];  /* Maximum key length to be stored used for local variables */
} tBTM_SEC_KEY_ELEM;

/*******************************************************************************
**
** Function         btm_store_link_key_blobs
**
** Description      This function converted linkkeys and store to nvram
**
** Returns          tBTM_STATUS
**
*******************************************************************************/
tBTM_STATUS btm_sec_nvram_store_linkkey_blobs(BD_ADDR nvram_entry_bd_addr, tBLE_ADDR_TYPE addr_type, tBTM_SEC_DEV_REC *p_dev_rec);





/*******************************************************************************
**
** Function         btm_restore_link_key_from_keyblobs
**
** Description      This function converted keyblobs stored in nvram to link keys
**
** Returns
**
*******************************************************************************/
tBTM_STATUS btm_sec_nvram_get_linkkey_from_blobs(tBTM_SEC_DEV_REC  *p_dev_rec);



/*******************************************************************************
**
** Function         btm_sec_nvram_get_key_by_keytype
**
** Description      This function converted keyblobs stored in DCT to link keys
**
** Returns
**
*******************************************************************************/
tBTM_STATUS btm_sec_nvram_get_key_by_keytype(BD_ADDR bd_addr, tBTM_LE_KEY_TYPE key_type, tBTM_SEC_KEY_VALUE *p_sec_keys);

#if (BTM_BLE_PRIVACY_SPT == TRUE )
/*******************************************************************************
**
** Function         btm_sec_nvram_load_pid_db
**
** Description      This function loads PID keys from nvram to ram
**
** Returns
**
********************************************************************************/
void btm_sec_nvram_load_pid_db(void);
#endif

/*******************************************************************************
**
** Function         btm_sec_nvram_get_keys_from_blobs
**
** Description      This function retrieves key type from stored key blobs in nvram
**
** Returns
**
*******************************************************************************/
tBTM_STATUS btm_sec_nvram_get_keymask_from_blobs(BD_ADDR bd_addr, UINT16 *p_key_mask);
