/******************************************************************************
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author              Notes
 * 2019-05-10     mark.huang && xyp   first implementation
 *
 ******************************************************************************/


/****************************************************************************
*
* This file is for Classic Bluetooth device and service discovery Demo.
*
****************************************************************************/

#include <stdint.h>
#include <string.h>

#include "rk_log.h"
#include "rk_bt.h"
#include "rk_bt_main.h"
#include "rk_bt_device.h"
#include "rk_gap_bt_api.h"
#include "osi/types.h"
#include "osi/bt_config.h"
#include "btHwControl.h"
#define GAP_TAG          "GAP"

typedef enum
{
    APP_GAP_STATE_IDLE = 0,
    APP_GAP_STATE_DEVICE_DISCOVERING,
    APP_GAP_STATE_DEVICE_DISCOVER_COMPLETE,
    APP_GAP_STATE_SERVICE_DISCOVERING,
    APP_GAP_STATE_SERVICE_DISCOVER_COMPLETE,
} app_gap_state_t;

typedef struct
{
    bool dev_found;
    uint8_t bdname_len;
    uint8_t eir_len;
    uint8_t rssi;
    uint32_t cod;
    uint8_t eir[RK_BT_GAP_EIR_DATA_LEN];
    uint8_t bdname[RK_BT_GAP_MAX_BDNAME_LEN + 1];
    rk_bd_addr_t bda;
    app_gap_state_t state;
} app_gap_cb_t;

static app_gap_cb_t m_dev_info;

#ifdef BT_CHIP_AP6212
extern bt_hw_control_t ap6212_hw_control;
bt_hw_control_t *p_hci_if_gap = &ap6212_hw_control;
#else
//koala
extern bt_hw_control_t koala_hw_control;
bt_hw_control_t *p_hci_if_gap = &koala_hw_control;
#endif

static char *bda2str(rk_bd_addr_t bda, char *str, size_t size)
{
    if (bda == NULL || str == NULL || size < 18)
    {
        return NULL;
    }

    uint8_t *p = bda;
    sprintf(str, "%02x:%02x:%02x:%02x:%02x:%02x",
            p[0], p[1], p[2], p[3], p[4], p[5]);
    return str;
}

static char *uuid2str(rk_bt_uuid_t *uuid, char *str, size_t size)
{
    if (uuid == NULL || str == NULL)
    {
        return NULL;
    }

    if (uuid->len == 2 && size >= 5)
    {
        sprintf(str, "%04x", uuid->uuid.uuid16);
    }
    else if (uuid->len == 4 && size >= 9)
    {
        sprintf(str, "%08x", uuid->uuid.uuid32);
    }
    else if (uuid->len == 16 && size >= 37)
    {
        uint8_t *p = uuid->uuid.uuid128;
        sprintf(str, "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
                p[15], p[14], p[13], p[12], p[11], p[10], p[9], p[8],
                p[7], p[6], p[5], p[4], p[3], p[2], p[1], p[0]);
    }
    else
    {
        return NULL;
    }

    return str;
}

static bool get_name_from_eir(uint8_t *eir, uint8_t *bdname, uint8_t *bdname_len)
{
    uint8_t *rmt_bdname = NULL;
    uint8_t rmt_bdname_len = 0;

    if (!eir)
    {
        return false;
    }

    rmt_bdname = rk_bt_gap_resolve_eir_data(eir, RK_BT_EIR_TYPE_CMPL_LOCAL_NAME, &rmt_bdname_len);
    if (!rmt_bdname)
    {
        rmt_bdname = rk_bt_gap_resolve_eir_data(eir, RK_BT_EIR_TYPE_SHORT_LOCAL_NAME, &rmt_bdname_len);
    }

    if (rmt_bdname)
    {
        if (rmt_bdname_len > RK_BT_GAP_MAX_BDNAME_LEN)
        {
            rmt_bdname_len = RK_BT_GAP_MAX_BDNAME_LEN;
        }

        if (bdname)
        {
            memcpy(bdname, rmt_bdname, rmt_bdname_len);
            bdname[rmt_bdname_len] = '\0';
        }
        if (bdname_len)
        {
            *bdname_len = rmt_bdname_len;
        }
        return true;
    }

    return false;
}

static void update_device_info(rk_bt_gap_cb_param_t *param)
{
    char bda_str[18];
    uint32_t cod = 0;
    int32_t rssi = -129; /* invalid value */
    rk_bt_gap_dev_prop_t *p;

    RK_LOGI(GAP_TAG, "Device found: %s\n", bda2str(param->disc_res.bda, bda_str, 18));
    for (int i = 0; i < param->disc_res.num_prop; i++)
    {
        p = param->disc_res.prop + i;
        switch (p->type)
        {
        case RK_BT_GAP_DEV_PROP_COD:
            cod = *(uint32_t *)(p->val);
            RK_LOGI(GAP_TAG, "--Class of Device: 0x%x\n", cod);
            break;
        case RK_BT_GAP_DEV_PROP_RSSI:
            rssi = *(int8_t *)(p->val);
            RK_LOGI(GAP_TAG, "--RSSI: %d\n", rssi);
            break;
        case RK_BT_GAP_DEV_PROP_BDNAME:
        default:
            break;
        }
    }

    /* search for device with MAJOR service class as "rendering" in COD */
    app_gap_cb_t *p_dev = &m_dev_info;
    if (p_dev->dev_found && 0 != memcmp(param->disc_res.bda, p_dev->bda, RK_BD_ADDR_LEN))
    {
        return;
    }

    if (!rk_bt_gap_is_valid_cod(cod) ||
            !(rk_bt_gap_get_cod_major_dev(cod) == RK_BT_COD_MAJOR_DEV_PHONE))
    {
        return;
    }

    memcpy(p_dev->bda, param->disc_res.bda, RK_BD_ADDR_LEN);
    p_dev->dev_found = true;
    for (int i = 0; i < param->disc_res.num_prop; i++)
    {
        p = param->disc_res.prop + i;
        switch (p->type)
        {
        case RK_BT_GAP_DEV_PROP_COD:
            p_dev->cod = *(uint32_t *)(p->val);
            break;
        case RK_BT_GAP_DEV_PROP_RSSI:
            p_dev->rssi = *(int8_t *)(p->val);
            break;
        case RK_BT_GAP_DEV_PROP_BDNAME:
        {
            uint8_t len = (p->len > RK_BT_GAP_MAX_BDNAME_LEN) ? RK_BT_GAP_MAX_BDNAME_LEN :
                          (uint8_t)p->len;
            memcpy(p_dev->bdname, (uint8_t *)(p->val), len);
            p_dev->bdname[len] = '\0';
            p_dev->bdname_len = len;
            break;
        }
        case RK_BT_GAP_DEV_PROP_EIR:
        {
            memcpy(p_dev->eir, (uint8_t *)(p->val), p->len);
            p_dev->eir_len = p->len;
            break;
        }
        default:
            break;
        }
    }

    if (p_dev->eir && p_dev->bdname_len == 0)
    {
        get_name_from_eir(p_dev->eir, p_dev->bdname, &p_dev->bdname_len);
        RK_LOGI(GAP_TAG, "Found a target device, address %s, name %s\n", bda_str, p_dev->bdname);
        p_dev->state = APP_GAP_STATE_DEVICE_DISCOVER_COMPLETE;
        RK_LOGI(GAP_TAG, "Cancel device discovery ...\n");
        rk_bt_gap_cancel_discovery();
    }
}

void bt_app_gap_init(void)
{
    app_gap_cb_t *p_dev = &m_dev_info;
    memset(p_dev, 0, sizeof(app_gap_cb_t));
}

void bt_gap_cb(rk_bt_gap_cb_event_t event, rk_bt_gap_cb_param_t *param)
{
    app_gap_cb_t *p_dev = &m_dev_info;
    char bda_str[18];
    char uuid_str[37];

    switch (event)
    {
    case RK_BT_GAP_DISC_RES_EVT:
    {
        update_device_info(param);
        break;
    }
    case RK_BT_GAP_DISC_STATE_CHANGED_EVT:
    {
        if (param->disc_st_chg.state == RK_BT_GAP_DISCOVERY_STOPPED)
        {
            RK_LOGI(GAP_TAG, "Device discovery stopped.\n");
            if ((p_dev->state == APP_GAP_STATE_DEVICE_DISCOVER_COMPLETE ||
                    p_dev->state == APP_GAP_STATE_DEVICE_DISCOVERING)
                    && p_dev->dev_found)
            {
                p_dev->state = APP_GAP_STATE_SERVICE_DISCOVERING;
                RK_LOGI(GAP_TAG, "Discover services ...\n");
                rk_bt_gap_get_remote_services(p_dev->bda);
            }
        }
        else if (param->disc_st_chg.state == RK_BT_GAP_DISCOVERY_STARTED)
        {
            RK_LOGI(GAP_TAG, "Discovery started.\n");
        }
        break;
    }
    case RK_BT_GAP_RMT_SRVCS_EVT:
    {
        if (memcmp(param->rmt_srvcs.bda, p_dev->bda, RK_BD_ADDR_LEN) == 0 &&
                p_dev->state == APP_GAP_STATE_SERVICE_DISCOVERING)
        {
            p_dev->state = APP_GAP_STATE_SERVICE_DISCOVER_COMPLETE;
            if (param->rmt_srvcs.stat == RK_BT_STATUS_SUCCESS)
            {
                RK_LOGI(GAP_TAG, "Services for device %s found\n",  bda2str(p_dev->bda, bda_str, 18));
                for (int i = 0; i < param->rmt_srvcs.num_uuids; i++)
                {
                    rk_bt_uuid_t *u = param->rmt_srvcs.uuid_list + i;
                    RK_LOGI(GAP_TAG, "--%s", uuid2str(u, uuid_str, 37));
                    // RK_LOGI(GAP_TAG, "--%d", u->len);
                }
            }
            else
            {
                RK_LOGI(GAP_TAG, "Services for device %s not found\n",  bda2str(p_dev->bda, bda_str, 18));
            }
        }
        break;
    }
    case RK_BT_GAP_RMT_SRVC_REC_EVT:
    default:
    {
        RK_LOGI(GAP_TAG, "event: %d\n", event);
        break;
    }
    }
    return;
}

void bt_app_gap_start_up(void)
{
    char *dev_name = "RK_BT_RTT01";
    rk_bt_dev_set_device_name(dev_name);

    /* set discoverable and connectable mode, wait to be connected */
    rk_bt_gap_set_scan_mode(RK_BT_CONNECTABLE, RK_BT_GENERAL_DISCOVERABLE);

    /* register GAP callback function */
    rk_bt_gap_register_callback(bt_gap_cb);

    /* inititialize device information and status */
    app_gap_cb_t *p_dev = &m_dev_info;
    memset(p_dev, 0, sizeof(app_gap_cb_t));

    /* start to discover nearby Bluetooth devices */
    p_dev->state = APP_GAP_STATE_DEVICE_DISCOVERING;
    rk_bt_gap_start_discovery(RK_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
}

void _bt_discovrey(void *arg)
{
    /* Initialize NVS — it is used to store PHY calibration data */
    osi_err_t ret;
    // osi_err_t ret = nvs_flash_init();
    // if (ret == RK_ERR_NVS_NO_FREE_PAGES || ret == RK_ERR_NVS_NEW_VERSION_FOUND) {
    //     RK_ERROR_CHECK(nvs_flash_erase());
    //     ret = nvs_flash_init();
    // }
    // RK_ERROR_CHECK( ret );

    //RK_ERROR_CHECK(rk_bt_controller_mem_release(RK_BT_MODE_BLE));

    rk_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    if ((ret = rk_bt_controller_init(&bt_cfg, p_hci_if_gap)) != OSI_EOK)
    {
        RK_LOGE(GAP_TAG, "%s initialize controller failed\n", __func__);
        return;
    }

    if ((ret = rk_bt_controller_enable(RK_BT_MODE_CLASSIC_BT)) != OSI_EOK)
    {
        RK_LOGE(GAP_TAG, "%s enable controller failed:\n", __func__);
        return;
    }

    if ((ret = rk_bluedroid_init()) != OSI_EOK)
    {
        RK_LOGE(GAP_TAG, "%s initialize bluedroid failed\n", __func__);
        return;
    }

    if ((ret = rk_bluedroid_enable()) != OSI_EOK)
    {
        RK_LOGE(GAP_TAG, "%s enable bluedroid failed\n", __func__);
        return;
    }

    bt_app_gap_start_up();
}
static rt_thread_t tid1;
void app_main_bt_discovrey()
{
    rt_thread_delay(100);
    tid1 = rt_thread_create("_bt_discovrey",
                            _bt_discovrey, RT_NULL,
                            2048,
                            25, 10);

    /* 如果获得线程控制块，启动这个线程 */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

}

MSH_CMD_EXPORT(app_main_bt_discovrey, bt discovrey);
