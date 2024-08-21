/*
 * $ Copyright Cypress Semiconductor $
 */

#ifndef BT_NV_H
#define BT_NV_H

#include "wiced_bt_dev.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                     Macros
 ******************************************************/
#define BT_AUDIO_NV_MAX_LAST_PAIRED_DEVICES ( 8 )


/******************************************************
*                     Typedefs
******************************************************/
typedef    uint8_t  bt_audio_hash_table_t[BT_AUDIO_NV_MAX_LAST_PAIRED_DEVICES];

/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *            Structures
 ******************************************************/
typedef struct
{
    uint32_t                       services_mask;      /**< Supported services*/
    wiced_bt_device_link_keys_t    device_link;            /**< BD address of the peer device. */
} bt_audio_paired_device_info_t;



typedef struct
{
    bt_audio_hash_table_t              bt_hash_table;
    bt_audio_paired_device_info_t      bt_paired_device_info[BT_AUDIO_NV_MAX_LAST_PAIRED_DEVICES];
    wiced_bt_local_identity_keys_t     bt_local_id_keys;
} bt_dct_t;

/******************************************************
 *            Function Declarations
 ******************************************************/

void bt_audio_nv_init(void);
void bt_audio_nv_deinit(void);
wiced_result_t bt_audio_nv_update_device_link_key(wiced_bt_device_link_keys_t *in_device);
wiced_result_t bt_audio_nv_update_last_connected_device(wiced_bt_device_address_t address);
wiced_result_t bt_audio_nv_get_device_info_by_addr(wiced_bt_device_address_t *address, bt_audio_paired_device_info_t *out_device);
wiced_result_t bt_audio_nv_get_device_info_by_index(uint8_t index, bt_audio_paired_device_info_t *out_device);
wiced_result_t bt_audio_nv_delete_device_info(wiced_bt_device_address_t address);
wiced_result_t bt_audio_nv_delete_device_info_list(void);
wiced_result_t bt_audio_nv_update_local_id_keys(wiced_bt_local_identity_keys_t *keys);
wiced_result_t bt_audio_nv_get_local_id_keys(wiced_bt_local_identity_keys_t *keys);

#ifdef __cplusplus
} /*extern "C" */
#endif

#endif //BT_NV_H
