/*
 * $ Copyright Cypress Semiconductor $
 */

#include <string.h>
#include "wiced_result.h"
#include "wwd_debug.h"
#include "wwd_assert.h"
#include "wiced_framework.h"
#include "wiced_rtos.h"
#include "wiced_bt_dev.h"
#include "bluetooth_nv.h"
#include "app_dct.h"
#include "wiced_dct_common.h"

/******************************************************
 *                      Macros
 ******************************************************/
#define PAIRED_DEVICE_INFO_ENTRY_OFFSET( index )   ( BT_NV_PAIRED_DEVICES_TABLE_OFFSET+index*sizeof( bt_audio_paired_device_info_t ) )
#define GET_INDEX_TO_DEVICE_ENTRY( i )             ( (dev_info_index_list[i]&0x7F) - 1 )

/******************************************************
 *                    Constants
 ******************************************************/
#define SIZEOF_HASH_TABLE                  ( sizeof( bt_audio_hash_table_t ) )
#define SIZEOF_PAIRED_DEVICES_TABLE        ( sizeof( bt_audio_paired_device_info_t ) * BT_AUDIO_NV_MAX_LAST_PAIRED_DEVICES )
#define SIZEOF_LOCAL_ID_KEYS               ( sizeof(  wiced_bt_local_identity_keys_t ) )

#define BT_NV_HASH_TABLE_OFFSET            ( app_dct_get_offset(APP_DCT_BT) + OFFSETOF(bt_dct_t, bt_hash_table) )
#define BT_NV_PAIRED_DEVICES_TABLE_OFFSET  ( app_dct_get_offset(APP_DCT_BT) + OFFSETOF(bt_dct_t, bt_paired_device_info) )
#define BT_NV_LOCAL_ID_KEYS_OFFSET         ( app_dct_get_offset(APP_DCT_BT) + OFFSETOF(bt_dct_t, bt_local_id_keys) )

/******************************************************
*                   Enumerations
******************************************************/

/******************************************************
*                 Type Definitions
******************************************************/

/******************************************************
*               Function Declarations
******************************************************/

/******************************************************
*               Variables Definitions
******************************************************/
/*index indicates the priority ( i.e., lowest index, most recently used )
    value indicates:
    first 7 bits  from lsb - the entry's index in the info table
    the 8th bit indicates if the entry is used ( set ) or not ( reset ).
*/
static uint8_t *dev_info_index_list;
static uint8_t dev_list_used_num;
static wiced_mutex_t bt_nv_mutex;

/******************************************************
*               Function Definitions
******************************************************/
void bt_audio_nv_init(void)
{
    UINT8 i;
    wiced_result_t result;

    result = wiced_rtos_init_mutex(&bt_nv_mutex);
    wiced_assert("MUTEX init failed", result == WICED_SUCCESS);
    if (result != WICED_SUCCESS)
    {
        APPL_TRACE_ERROR1("bt_audio_nv_init: NV mutex initialization failed with error code %d", result);
        return;
    }

    //find out how many entries are used up.
    result = wiced_dct_read_lock((void **)&dev_info_index_list, WICED_TRUE, DCT_APP_SECTION, BT_NV_HASH_TABLE_OFFSET, SIZEOF_HASH_TABLE);
    wiced_assert("index list read failed", result == WICED_SUCCESS);
    if (result != WICED_SUCCESS)
    {
        APPL_TRACE_ERROR1("bt_audio_nv_init: NV mutex initialization failed with error code %d", result);
        return;
    }

    wiced_rtos_lock_mutex(&bt_nv_mutex);
    dev_list_used_num = 0;
    for (i = 0; i < BT_AUDIO_NV_MAX_LAST_PAIRED_DEVICES; i++)
    {
        if (dev_info_index_list[i] & 0x80)
        {
            dev_list_used_num++;
        }
    }
    APPL_TRACE_DEBUG1("bt_audio_nv_init: NV device info list initialized, no of devices in NV=%d", dev_list_used_num);
    wiced_rtos_unlock_mutex(&bt_nv_mutex);
}

void bt_audio_nv_deinit(void)
{
    wiced_rtos_deinit_mutex(&bt_nv_mutex);
    wiced_dct_read_unlock((void *)dev_info_index_list, WICED_TRUE);
}

wiced_result_t bt_audio_nv_update_device_link_key(wiced_bt_device_link_keys_t *in_device)
{
    int i, free_index = 0;
    UINT8 table_index;
    bt_audio_paired_device_info_t *device;

    if (!in_device)
    {
        APPL_TRACE_ERROR0("bt_audio_nv_update_device_link_key: NULL argument");
        return WICED_BADARG;
    }

    wiced_rtos_lock_mutex(&bt_nv_mutex);
    for (i = 0; i < dev_list_used_num; i++)
    {
        table_index = GET_INDEX_TO_DEVICE_ENTRY(i);

        wiced_dct_read_lock((void **)&device, WICED_TRUE, DCT_APP_SECTION,
                            PAIRED_DEVICE_INFO_ENTRY_OFFSET(table_index), sizeof(bt_audio_paired_device_info_t));

        if (!memcmp(device->device_link.bd_addr, in_device->bd_addr, sizeof(wiced_bt_device_address_t)))
        {
            APPL_TRACE_DEBUG2("bt_audio_nv_update_device_link_key: Device to be updated found @ dev_info_index_list[%d]=%d\n", i, table_index);
            wiced_dct_write((const void *) in_device, DCT_APP_SECTION,
                            PAIRED_DEVICE_INFO_ENTRY_OFFSET(table_index) + OFFSETOF(bt_audio_paired_device_info_t, device_link), sizeof(wiced_bt_device_link_keys_t));
            wiced_dct_read_unlock((void *) device, WICED_TRUE);
            dev_info_index_list[i] = (table_index + 1) | 0x80;
            break; //end loop
        }
        wiced_dct_read_unlock((void *) device, WICED_TRUE);
    }

    if (i == dev_list_used_num)
    {
        if (dev_list_used_num < BT_AUDIO_NV_MAX_LAST_PAIRED_DEVICES)
        {
            free_index = dev_list_used_num++;
            dev_info_index_list[free_index] = free_index + 1;
        }
        else
        {
            //get the last entry
            free_index = dev_list_used_num - 1;
        }
        table_index = GET_INDEX_TO_DEVICE_ENTRY(free_index);
        APPL_TRACE_DEBUG3("bt_audio_nv_update_device_link_key: new entry, free_index=%d, table_index=%d, no. of devices=%d", free_index, table_index, dev_list_used_num);

        wiced_dct_write((void *)in_device, DCT_APP_SECTION,
                        PAIRED_DEVICE_INFO_ENTRY_OFFSET(table_index) + OFFSETOF(bt_audio_paired_device_info_t, device_link), sizeof(wiced_bt_device_link_keys_t));
        dev_info_index_list[free_index] = (table_index + 1) | 0x80;

        for (i = 0; i < dev_list_used_num; i++)
        {
            APPL_TRACE_DEBUG2("bt_audio_nv_update_device_link_key: dev_info_index_list[%d]=%d", i, GET_INDEX_TO_DEVICE_ENTRY(i));
        }
        APPL_TRACE_DEBUG2("bt_audio_nv_update_device_link_key: Added device @ table position %d, no of devices in NV=%d", table_index, dev_list_used_num);
    }

    wiced_dct_write(dev_info_index_list, DCT_APP_SECTION, BT_NV_HASH_TABLE_OFFSET, SIZEOF_HASH_TABLE);
    wiced_rtos_unlock_mutex(&bt_nv_mutex);

    bt_audio_nv_update_last_connected_device(in_device->bd_addr);
    return WICED_SUCCESS;
}


wiced_result_t bt_audio_nv_update_last_connected_device(wiced_bt_device_address_t address)
{
    int i, j;
    UINT8 table_index;
    bt_audio_paired_device_info_t *device;
    wiced_result_t ret = WICED_ERROR;

    wiced_rtos_lock_mutex(&bt_nv_mutex);
    for (i = 0; i < dev_list_used_num; i++)
    {
        table_index = GET_INDEX_TO_DEVICE_ENTRY(i);

        wiced_dct_read_lock((void **)&device, WICED_FALSE, DCT_APP_SECTION,
                            PAIRED_DEVICE_INFO_ENTRY_OFFSET(table_index), sizeof(bt_audio_paired_device_info_t));

        if (!memcmp(device->device_link.bd_addr, address, sizeof(wiced_bt_device_address_t)))
        {
            APPL_TRACE_DEBUG2("bt_audio_nv_update_last_connected_device: Device found @ dev_info_index_list[%d]=%d", i, table_index);
            if (i != 0)  //to avoid write if the entry is already at the top
            {
                for (j = i; j > 0; j--)
                {
                    dev_info_index_list[j] = dev_info_index_list[j - 1];
                }
                dev_info_index_list[0] = (table_index + 1) | 0x80;

                wiced_dct_write(dev_info_index_list, DCT_APP_SECTION, BT_NV_HASH_TABLE_OFFSET, SIZEOF_HASH_TABLE);
                APPL_TRACE_DEBUG2("bt_audio_nv_update_last_connected_device: updated position dev_info_index_list[0]=%d, no of devices in NV=%d", table_index, dev_list_used_num);
            }
            else
            {
                APPL_TRACE_DEBUG0("bt_audio_nv_update_last_connected_device: already at 0th position in dev_info_index_list");
            }
            ret = WICED_SUCCESS;
            wiced_dct_read_unlock((void *) device, WICED_FALSE);
            break;
        }
        wiced_dct_read_unlock((void *) device, WICED_FALSE);
    }
    wiced_rtos_unlock_mutex(&bt_nv_mutex);

    if (ret != WICED_SUCCESS)
        APPL_TRACE_ERROR0("bt_audio_nv_update_last_connected_device: Device not found");

    return ret;
}

wiced_result_t bt_audio_nv_get_device_info_by_addr(wiced_bt_device_address_t *address, bt_audio_paired_device_info_t *out_device)
{
    int i;
    UINT8 table_index;
    bt_audio_paired_device_info_t *device;
    wiced_result_t ret = WICED_ERROR;

    if (!out_device)
    {
        APPL_TRACE_ERROR0("bt_audio_nv_get_device_info_by_addr: NULL argument");
        return WICED_BADARG;
    }

    wiced_rtos_lock_mutex(&bt_nv_mutex);
    for (i = 0; i < dev_list_used_num; i++)
    {
        table_index = GET_INDEX_TO_DEVICE_ENTRY(i);

        wiced_dct_read_lock((void **)&device, WICED_FALSE, DCT_APP_SECTION,
                            PAIRED_DEVICE_INFO_ENTRY_OFFSET(table_index), sizeof(bt_audio_paired_device_info_t));

        if (!memcmp(device->device_link.bd_addr, address, sizeof(wiced_bt_device_address_t)))
        {
            APPL_TRACE_DEBUG2("bt_audio_nv_get_device_info_by_addr: Device found @ dev_info_index_list[%d]=%d", i, table_index);
            memcpy(out_device, device, sizeof(bt_audio_paired_device_info_t));

            wiced_dct_read_unlock((void *) device, WICED_FALSE);
            ret = WICED_SUCCESS;
            break;
        }
        wiced_dct_read_unlock((void *) device, WICED_FALSE);
    }
    wiced_rtos_unlock_mutex(&bt_nv_mutex);

    if (ret != WICED_SUCCESS)
        APPL_TRACE_ERROR0("bt_audio_nv_get_device_info_by_addr: Device not found");

    return ret;
}


wiced_result_t bt_audio_nv_get_device_info_by_index(uint8_t index, bt_audio_paired_device_info_t *out_device)
{
    UINT8 table_index;
    bt_audio_paired_device_info_t *device;

    if (index >= dev_list_used_num)
        return WICED_BADARG;

    if (!out_device)
        return WICED_BADARG;

    wiced_rtos_lock_mutex(&bt_nv_mutex);

    table_index = GET_INDEX_TO_DEVICE_ENTRY(index);

    wiced_dct_read_lock((void **)&device, WICED_FALSE, DCT_APP_SECTION,
                        PAIRED_DEVICE_INFO_ENTRY_OFFSET(table_index), sizeof(bt_audio_paired_device_info_t));
    memcpy(out_device, device, sizeof(bt_audio_paired_device_info_t));
    wiced_dct_read_unlock((void *) device, WICED_FALSE);
    wiced_rtos_unlock_mutex(&bt_nv_mutex);

    APPL_TRACE_DEBUG2("bt_audio_nv_get_device_info_by_index: Device found @ dev_info_index_list[%d]=%d", index, table_index);
    return WICED_SUCCESS;
}

wiced_result_t bt_audio_nv_delete_device_info(wiced_bt_device_address_t address)
{
    int i, j;
    UINT8 table_index;
    bt_audio_paired_device_info_t *device;
    wiced_result_t ret = WICED_ERROR;

    wiced_rtos_lock_mutex(&bt_nv_mutex);
    for (i = 0; i < dev_list_used_num; i++)
    {
        table_index = GET_INDEX_TO_DEVICE_ENTRY(i);

        wiced_dct_read_lock((void **)&device, WICED_FALSE, DCT_APP_SECTION,
                            PAIRED_DEVICE_INFO_ENTRY_OFFSET(table_index), sizeof(bt_audio_paired_device_info_t));
        if (!memcmp(device->device_link.bd_addr, address, sizeof(wiced_bt_device_address_t)))
        {
            APPL_TRACE_DEBUG2("bt_audio_nv_delete_device_info: Device found @ dev_info_index_list[%d]=%d", i, table_index);
            wiced_dct_read_unlock((void *) device, WICED_FALSE);

            dev_list_used_num--;
            for (j = i; j < dev_list_used_num; j++)
            {
                dev_info_index_list[j] = dev_info_index_list[j + 1];
            }
            dev_info_index_list[dev_list_used_num] = table_index & 0x7F;

            wiced_dct_write(dev_info_index_list, DCT_APP_SECTION, BT_NV_HASH_TABLE_OFFSET, SIZEOF_HASH_TABLE);
            ret = WICED_SUCCESS;
            break;
        }
        wiced_dct_read_unlock((void *) device, WICED_FALSE);
    }
    wiced_rtos_unlock_mutex(&bt_nv_mutex);

    if (ret != WICED_SUCCESS)
        APPL_TRACE_ERROR0("bt_audio_nv_delete_device_info: Device not found");

    return ret;
}


wiced_result_t bt_audio_nv_delete_device_info_list(void)
{
    wiced_rtos_lock_mutex(&bt_nv_mutex);
    memset(dev_info_index_list, 0, BT_AUDIO_NV_MAX_LAST_PAIRED_DEVICES);
    wiced_dct_write(dev_info_index_list, DCT_APP_SECTION, BT_NV_HASH_TABLE_OFFSET, SIZEOF_HASH_TABLE);
    APPL_TRACE_DEBUG0("bt_audio_nv_delete_device_info_list: deleted device list");
    wiced_rtos_unlock_mutex(&bt_nv_mutex);
    return WICED_SUCCESS;
}

wiced_result_t bt_audio_nv_update_local_id_keys(wiced_bt_local_identity_keys_t *keys)
{
    if (keys == NULL)
    {
        return WICED_BADARG;
    }

    wiced_rtos_lock_mutex(&bt_nv_mutex);
    wiced_dct_write(keys, DCT_APP_SECTION, BT_NV_LOCAL_ID_KEYS_OFFSET, SIZEOF_LOCAL_ID_KEYS);
    APPL_TRACE_DEBUG0("bt_audio_nv_update_local_id_keys: update_local_id_keys");
    wiced_rtos_unlock_mutex(&bt_nv_mutex);
    return WICED_SUCCESS;
}

wiced_result_t bt_audio_nv_get_local_id_keys(wiced_bt_local_identity_keys_t *keys)
{
    wiced_bt_local_identity_keys_t *key_in_dct;
    if (keys == NULL)
    {
        return WICED_BADARG;
    }

    wiced_rtos_lock_mutex(&bt_nv_mutex);
    APPL_TRACE_DEBUG0("bt_audio_nv_get_local_id_keys: get_local_id_keys");
    wiced_dct_read_lock((void **)&key_in_dct, WICED_FALSE, DCT_APP_SECTION, BT_NV_LOCAL_ID_KEYS_OFFSET, SIZEOF_LOCAL_ID_KEYS);
    memcpy(keys, key_in_dct, SIZEOF_LOCAL_ID_KEYS);

    wiced_rtos_unlock_mutex(&bt_nv_mutex);
    return WICED_SUCCESS;
}
