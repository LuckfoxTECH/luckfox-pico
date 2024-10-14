/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtdevice.h>
#include <rtthread.h>

#include <dfs_posix.h>
#include <drivers/mtd_nor.h>
#include "hal_base.h"
#include "hal_bsp.h"

#ifdef RT_USING_OTA
#include "../fwmgr/fw_analysis.h"
#include "../fwmgr/rkpart.h"
#include "drv_flash_partition.h"

#include "ota_opt.h"
//#include "ota_debug.h"
#ifdef RT_USING_OTA_FROM_LOCAL
#include "ota_file.h"
#endif

#ifdef RT_USING_OTA_FROM_HTTP
#include "ota_http.h"
#endif

#include "ota.h"

#define DBG_SECTION_NAME    "OTA"
#include "rkdebug.h"

#define OTA_IMG_DATA_CORRUPTION_TEST    0
#define OTA_UPDATE_DEBUG_UNIT           (50 * 1024)

rt_uint32_t g_fw_curr_slot;         /* current fw run in slot */
rt_uint32_t g_user_curr_slot;       /* current userdata used slot */
rt_uint32_t g_os_real_size;         /* os part real size */
rt_uint32_t g_data_real_size;       /* data part real size */
rt_uint32_t g_root_real_size;       /* userdata part real size */

static ota_priv_st ota_priv;

/* alloc 1M buffer to save fw data from http server*/
//#define OTA_FW_DATA_SIZE                (1024*1024)
//static char *gOtaFwBuff;

/* indexed by image_seq_t */
static const rt_uint8_t ota_update_seq_policy[IMAGE_SEQ_NUM] =
{
#if (IMAGE_SEQ_NUM == 2)
    /* update policy: 0 -> 1, 1 -> 0 */
    1, 0
#elif (IMAGE_SEQ_NUM == 3)
    /* update policy: 0 -> 1, 1 -> 2, 2 -> 1 */
    1, 2, 1
#else
#error "unsupport image seq num!"
#endif
};

static struct rt_mtd_nor_device *snor_device = RT_NULL;

static rt_uint8_t ota_get_update_seq(void)
{
    rt_uint8_t seq = IMAGE_SEQ_NUM;

    if (ota_priv.fw_running_slot >= IMAGE_SEQ_NUM)
    {
        LOGE("not init, running slot ## %s ##\n", ota_priv.fw_running_slot ? "A" : "B");
        return seq;
    }

    seq = ota_update_seq_policy[ota_priv.fw_running_slot]; /* update sequence */
    LOGD("%s(), update seq %d\n", __func__, seq);
    return seq;
}

/**
 * @brief Initialize the OTA service
 * @retval ota_status_t, OTA_STATUS_OK on success
 */
ota_status ota_init(void)
{
    int32_t ret;
    rt_uint32_t fw_slot_boot;
#ifdef RT_SUPPORT_ROOT_AB
    rt_uint32_t user_slot_boot;
#endif
    const char *slot_name[2] = {"A", "B"};
    rt_uint32_t part_total_num = 0;
    struct rt_flash_partition *part_info = RT_NULL;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device == RT_NULL)
    {
        LOGE("Did not find device: snor...");
        return OTA_STATUS_ERR;
    }

    part_total_num = get_rk_partition(&part_info);
    RT_ASSERT(part_info);

    for (int i = 0; i < part_total_num; i++)
    {
        if (part_info[i].type == PART_FW1)
        {
            firmware_addr1 = part_info[i].offset;
            fw_part_size1   = part_info[i].size;
            LOGI("%s :", part_info[i].name);
            LOGI("FW1: Addr:0x%x, size:%d (0x%x sector)", firmware_addr1,
                 fw_part_size1, fw_part_size1 >> 9);

        }
        else if (part_info[i].type == PART_FW2)
        {
            firmware_addr2 = part_info[i].offset;
            fw_part_size2   = part_info[i].size;
            LOGI("%s :", part_info[i].name);
            LOGI("FW2:Addr:0x%x, size:%d (0x%x sector)", firmware_addr2,
                 fw_part_size2, fw_part_size2 >> 9);
        }
        else if (part_info[i].type == PART_DATA)
        {
            data_start_addr = part_info[i].offset;
            data_part_size  = part_info[i].size;
            LOGI("%s :", part_info[i].name);
            LOGI("DATA:Addr:0x%x, size:%d (0x%x sector)", data_start_addr,
                 data_part_size, data_part_size >> 9);
        }
        else if (part_info[i].type == PART_USER1)
        {
            user_start_addr1 = part_info[i].offset;
            user_part_size   = part_info[i].size;

            LOGI("%s :", part_info[i].name);
            LOGI("USER1:Addr:0x%x, size :%d (0x%x sectors)", user_start_addr1,
                 user_part_size, user_part_size >> 9);
        }
        else if (part_info[i].type == PART_USER2)
        {
            user_start_addr2 = part_info[i].offset;
            user_part_size   = part_info[i].size;

            LOGI("%s :", part_info[i].name);
            LOGI("USER2:Addr:0x%x, size :%d (0x%x sectors)", user_start_addr2,
                 user_part_size, user_part_size >> 9);
        }
    }

    ret = fw_slot_get_current_running(&fw_slot_boot);
    if (ret != OTA_STATUS_OK)
        return OTA_STATUS_ERR;

    rt_memset(&ota_priv, 0, sizeof(ota_priv));
    if (fw_slot_boot == 0)
        ota_priv.os_addr = firmware_addr2;
    else if (fw_slot_boot == 1)
        ota_priv.os_addr = firmware_addr1;
    else
        return OTA_STATUS_ERR;

    LOGI("%s : will update os addr:0x%08x", __func__, ota_priv.os_addr);

    ota_priv.fw_running_slot = fw_slot_boot;
    g_fw_curr_slot = fw_slot_boot;
    LOGD("=== Current rt-thread running %s slot fw ===", slot_name[fw_slot_boot]);

    if (fw_slot_boot == 0)
    {
        ota_priv.img_max_size = fw_part_size2;
        ota_priv.data_max_size = 0;
        ota_priv.data_addr     = data_start_addr;
    }
    else if (fw_slot_boot == 1)
    {
        ota_priv.img_max_size = fw_part_size1;
        ota_priv.data_max_size = data_part_size;
        ota_priv.data_addr     = data_start_addr;
    }

    LOGD("%s():os img size = %u(0x%#x sectors)", __func__, ota_priv.img_max_size,
         ota_priv.img_max_size >> 9);

#ifdef RT_SUPPORT_ROOT_AB
    ret = user_slot_get_current_running(&user_slot_boot);
    if (ret != OTA_STATUS_OK)
        return OTA_STATUS_ERR;

    if (user_slot_boot == 0)
        ota_priv.user_addr = user_start_addr2;
    else if (user_slot_boot == 1)
        ota_priv.user_addr = user_start_addr1;
    else
        return OTA_STATUS_ERR;

    rt_kprintf("%s : will update user addr:0x%08x\n", __func__, ota_priv.user_addr);

    ota_priv.user_running_slot = user_slot_boot;
    g_user_curr_slot = user_slot_boot;
    LOGD(" === Current user running %s slot ===", slot_name[user_slot_boot]);

    ota_priv.user_max_size = user_part_size;
    LOGD("%s(): userdata img size = %u(%#x)", __func__, ota_priv.user_max_size,
         ota_priv.user_max_size);
#endif

    return OTA_STATUS_OK;
}

/**
 * @brief DeInitialize the OTA service
 * @return None
 */
void ota_deinit(void)
{
    rt_memset(&ota_priv, 0, sizeof(ota_priv));
}

/* update os partiton process */
static ota_status ota_update_os_proc(rt_uint8_t *ota_buf, rt_uint8_t *fw_buf,
                                     ota_update_get get_cb)
{
    uint32_t        os_addr;
    uint32_t        os_size;
    uint32_t        debug_size;
    uint32_t        write_size;
    rt_uint8_t      eof_flag;

    ota_status      ret = OTA_STATUS_ERR;
    ota_status      status = OTA_STATUS_ERR;

    os_size      = ota_priv.img_max_size;
    os_addr      = ota_priv.os_addr;
    debug_size   = OTA_UPDATE_DEBUG_UNIT;

    LOGD("%s(), request to update seq %d, will update fw start addr 0x%#x\n",
         __func__, 1 - g_fw_curr_slot, os_addr);
    if (os_addr == 0)
    {
        LOGE("Error: fw%d address is invalide!!!");
        return ret;
    }

    if (os_size == 0)
        return OTA_STATUS_OK;

    LOGI("system image max size %u", os_size);

    /* System Partiton update process */
    LOGI("------------Start update System %s partition------------",
         ota_priv.fw_running_slot == 0 ? "B" : "A");
    rt_kprintf("[OTA] The System partiton update: %2d%%", ota_priv.get_size);
    while (os_size > 0)
    {
        uint32_t len, recvSize;
        len = MINIMUM(os_size, OTA_BUF_SIZE);

        rt_memset(ota_buf, 0, OTA_BUF_SIZE);
        status = get_cb(ota_buf, len, &recvSize, &eof_flag);
//        LOGD("read from file size = %d", recvSize);
        if (status != OTA_STATUS_OK)
        {
            LOGE("ota file get process status %d\n", status);
            break;
        }

        rt_memset(fw_buf, 0, OTA_BUF_SIZE);
        rt_memcpy(fw_buf, ota_buf, OTA_BUF_SIZE);

        //LOGD("next os addr = 0x%x", os_addr);
        rt_mtd_nor_erase_block(snor_device, os_addr, snor_device->block_size);
        write_size = rt_mtd_nor_write(snor_device, os_addr,
                                      (const rt_uint8_t *)fw_buf, snor_device->block_size);
        if (write_size != snor_device->block_size)
        {
            LOGE("os Fw data %#d write Error!", 1 - g_fw_curr_slot);
            break;
        }

        os_addr += OTA_BUF_SIZE;
        os_size -= recvSize;
        ota_priv.get_size += recvSize;
        rt_kprintf("\b\b\b%2d%%", (ota_priv.get_size * 100) / g_os_real_size);

        if (eof_flag)
        {
            ret = OTA_STATUS_OK;
            rt_kprintf("\n\n");
            LOGD("Donwloaded image %u Bytes(%u KB) to End \n",
                 ota_priv.get_size, ota_priv.get_size / 1024);
            break;
        }

        if (ota_priv.get_size >= debug_size)
        {
            //LOGD("OTA: Donwloaded image (%u KB)...", ota_priv.get_size / 1024);
            debug_size += OTA_UPDATE_DEBUG_UNIT;
        }

        rt_thread_delay(10);
        //LOGD("os_size left = %d\n", os_size);
    }

    LOGD("finish loading image %d(0x%#08x)", ota_priv.get_size, ota_priv.get_size);
    return ret;
}

/* update data partiton process */
static ota_status ota_update_data_proc(rt_uint8_t *ota_buf, rt_uint8_t *fw_buf,
                                       ota_update_get get_cb)
{
    uint32_t        data_addr;
    uint32_t        data_size;
    uint32_t        debug_size;
    uint32_t        write_size;
    rt_uint8_t      eof_flag;
    ota_status      ret = OTA_STATUS_ERR;
    ota_status      status = OTA_STATUS_ERR;

    ota_priv.get_size = 0;
    data_size      = ota_priv.data_max_size;
    data_addr      = ota_priv.data_addr;
    debug_size   = OTA_UPDATE_DEBUG_UNIT;

    if (data_size == 0)
        return OTA_STATUS_OK;

    LOGI("data partiton max size %u", data_size);
    LOGI("------------Start update Data partition------------");
    rt_kprintf("[OTA] The Data partiton update: %2d%%", ota_priv.get_size);

    while (data_size > 0)
    {
        uint32_t len, recvSize;
        len = MINIMUM(data_size, OTA_BUF_SIZE);

        rt_memset(ota_buf, 0, OTA_BUF_SIZE);
        status = get_cb(ota_buf, len, &recvSize, &eof_flag);
//        LOGD("read from file size = %d", recvSize);
        if (status != OTA_STATUS_OK)
        {
            LOGE("ota file get process status %d\n", status);
            break;
        }

        rt_memset(fw_buf, 0, OTA_BUF_SIZE);
        rt_memcpy(fw_buf, ota_buf, OTA_BUF_SIZE);

        //LOGD("next addr = 0x%x", data_addr);
        rt_mtd_nor_erase_block(snor_device, data_addr, snor_device->block_size);
        write_size = rt_mtd_nor_write(snor_device, data_addr, (const rt_uint8_t *)fw_buf,
                                      snor_device->block_size);
        if (write_size != snor_device->block_size)
        {
            LOGE("data patition write Error!");
            break;
        }

        //if (!eof_flag)
        {
            data_addr += OTA_BUF_SIZE;
            data_size -= recvSize;
            ota_priv.get_size += recvSize;
            rt_kprintf("\b\b\b%2d%%", (ota_priv.get_size * 100) / g_data_real_size);
        }

        if (eof_flag)
        {
            ret = OTA_STATUS_OK;
            rt_kprintf("\n\n");
            LOGD("Donwloaded image %u Bytes(%u KB) to End",
                 ota_priv.get_size, ota_priv.get_size / 1024);
            break;
        }

        if (ota_priv.get_size >= debug_size)
        {
            //LOGD("OTA: Donwloaded image (%u KB)...", ota_priv.get_size / 1024);
            debug_size += OTA_UPDATE_DEBUG_UNIT;
        }

        rt_thread_delay(10);
        //LOGD("data_size left = %d\n", data_size);
    }

    LOGD("finish loading image %d(0x%#08x)", ota_priv.get_size, ota_priv.get_size);
    return ret;
}

#ifdef RT_SUPPORT_ROOT_AB
/* update Userdata partiton process */
static ota_status ota_update_root_proc(rt_uint8_t *ota_buf, rt_uint8_t *fw_buf,
                                       ota_update_get get_cb)
{
    uint32_t        user_addr = 0;
    uint32_t        user_img_max_size = 0;
    uint32_t        debug_size;
    uint32_t        write_size;
    rt_uint8_t      eof_flag;
    ota_status      status = OTA_STATUS_ERR;
    ota_status      ret = OTA_STATUS_ERR;

    user_img_max_size = ota_priv.user_max_size;
    user_addr         = ota_priv.user_addr;

    LOGD("%s(), will update USER %d, will update USER start addr 0x%#x\n", __func__,
         1 - ota_priv.user_running_slot, user_addr);
    if (user_addr == 0)
    {
        LOGE("Error: fw%d address is invalide!!!");
        return ret;
    }

    ota_priv.user_get_size = 0;
    g_root_real_size       = user_part_size;
    LOGI("------------Start update User partition------------");
    rt_kprintf("[OTA] The User partiton update: %2d%%", ota_priv.user_get_size);

#ifdef RT_USING_OTA_FROM_HTTP
    ota_start_download_task((void *)1); //create root partition update thread.
#endif

    while (user_img_max_size > 0)
    {
        uint32_t len = 0, recvSize = 0;
        len = MINIMUM(user_img_max_size, OTA_BUF_SIZE);

        rt_memset(ota_buf, 0, OTA_BUF_SIZE);

        status = get_cb(ota_buf, len, &recvSize, &eof_flag);
//        LOGD("read from file size = %d", recvSize);
        if (status != OTA_STATUS_OK)
        {
            LOGE("ota file get process status %d\n", status);
            break;
        }

        rt_memset(fw_buf, 0, OTA_BUF_SIZE);
        rt_memcpy(fw_buf, ota_buf, OTA_BUF_SIZE);

//        LOGD("next addr = 0x%x", user_addr);
        rt_mtd_nor_erase_block(snor_device, user_addr, snor_device->block_size);
        write_size = rt_mtd_nor_write(snor_device, user_addr, (const rt_uint8_t *)fw_buf,
                                      snor_device->block_size);
        if (write_size != snor_device->block_size)
        {
            LOGE("userdata partiton %#d write Error!", 1 - g_user_curr_slot);
            break;
        }

        if (!eof_flag)
        {
            user_addr += OTA_BUF_SIZE;
            user_img_max_size -= recvSize;
            ota_priv.user_get_size += recvSize;
            rt_kprintf("\b\b\b%2d%%", (ota_priv.user_get_size * 100) / g_root_real_size);
        }
        else
        {
            ret = OTA_STATUS_OK;
            rt_kprintf("\n\n");
            LOGD("Donwloaded image %u Bytes(%u KB) to End", ota_priv.user_get_size,
                 ota_priv.user_get_size / 1024);
            break;
        }

        if (ota_priv.user_get_size >= debug_size)
        {
            //LOGD("OTA: Donwloaded image (%u KB)...", ota_priv.user_get_size / 1024);
            debug_size += OTA_UPDATE_DEBUG_UNIT;
        }

        rt_thread_delay(10);
        //LOGD("user_img_max_size left = %d\n", user_img_max_size);
    }

#ifdef RT_USING_OTA_FROM_HTTP
    ota_exit_download_task();
#endif
    return ret;
}
#endif

static ota_status ota_update_image_process(uint8_t seq, void *url,
        ota_update_init init_cb,
        ota_update_get get_cb)
{
    uint8_t         *ota_buf;
    ota_status      ret = OTA_STATUS_ERR;
    rt_uint8_t      *p_fwbuf;
    rt_bool_t       bSucc = RT_TRUE;

    struct rt_mtd_nor_device *snor_device = RT_NULL;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device == RT_NULL)
    {
        LOGD("Did not find device: snor....");
        return ret;
    }

    LOGD("block_size = %d", snor_device->block_size);

    ota_buf = rt_malloc_align(OTA_BUF_SIZE, 4);
    if (ota_buf == RT_NULL)
    {
        LOGE("NO memory!");
        return ret;
    }

    p_fwbuf = rt_malloc_align(OTA_BUF_SIZE, 64);
    if (p_fwbuf == RT_NULL)
    {
        LOGE("NO memory!!");
        if (ota_buf)
        {
            rt_free(ota_buf);
            ota_buf = RT_NULL;
        }
        return ret;
    }

    if (init_cb(url) != OTA_STATUS_OK)
    {
        LOGE("ota update init failed");
        bSucc = RT_FALSE;
        goto ota_err;
    }

#ifdef RT_USING_OTA_FROM_HTTP
    ota_start_download_task((void *)0);
#endif

    ret = ota_update_os_proc(ota_buf, p_fwbuf, get_cb);
    if (ret != OTA_STATUS_OK)
    {
        bSucc = RT_FALSE;
        goto  ota_err;
    }

    ret = ota_update_data_proc(ota_buf, p_fwbuf, get_cb);
    if (ret != OTA_STATUS_OK)
    {
        bSucc = RT_FALSE;
        goto  ota_err;
    }

#ifdef RT_USING_OTA_FROM_HTTP
    ota_exit_download_task();
#endif

#ifdef RT_SUPPORT_ROOT_AB
    ret = ota_update_root_proc(ota_buf, p_fwbuf, get_cb);
    if (ret != OTA_STATUS_OK)
    {
        bSucc = RT_FALSE;
        goto  ota_err;
    }
#endif  //RT_SUPPORT_ROOT_AB

ota_err:
    if (ota_buf)
    {
        rt_free_align(ota_buf);
        ota_buf = RT_NULL;
    }

    if (p_fwbuf)
    {
        rt_free_align(p_fwbuf);
        p_fwbuf = RT_NULL;
    }

//    LOGD("finish loading image %d(0x%#08x)", ota_priv.get_size, ota_priv.get_size);

    /* do jhash check here before we set slot activity */
    if (bSucc)
    {
        if (ota_verify_img(OTA_VERIFY_JHASH) != OTA_STATUS_OK)
        {
            LOGE("ota check image failed");
            return OTA_STATUS_ERR;
        }
        LOGD("Finish checking os image.\n");

        /* dsp data and version check,just in fw B to do. */
        if (ota_priv.data_max_size)
        {
            /* Verify data partition firmware : flash data  partiton with local file*/
            if (ota_data_check(url) != OTA_STATUS_OK)
            {
                LOGE("ota check data image failed");
                return OTA_STATUS_ERR;
            }
            LOGD("Finish checking data image.\n");

            if (ota_dsp_version_cmp() != OTA_STATUS_OK)
            {
                LOGE("ota dsp version check failed");
                return OTA_STATUS_ERR;
            }
            LOGD("dsp version is accordant!");
        }

        if (seq == 1)       /* pending os Fw 2 slot*/
            LOGD(" ---- Set system Start from FW2 slot----");
        else if (seq == 0)  /* pending os Fw 1 slot*/
            LOGD(" ---- Set system Start from FW1 slot----");

        fw_slot_set_pending(ota_priv.fw_running_slot);

#ifdef RT_SUPPORT_ROOT_AB
        if (g_user_curr_slot == 0)      /* pending user fs part 2 slot*/
            LOGD(" ---- Set userdata Start from FW2 slot----");
        else if (g_user_curr_slot == 1) /* pending user fs part 1 slot*/
            LOGD(" ---- Set userdata Start from FW1 slot----");
        user_slot_set_pending(ota_priv.user_running_slot);
        change_part_name((ota_priv.user_running_slot == 1) ? 0x40000000 : 0x80000000, "root");
        change_part_name((ota_priv.user_running_slot == 0) ? 0x40000000 : 0x80000000, "root-old");
#endif  //RT_SUPPORT_ROOT_AB
    }
    else
    {
#ifdef RT_USING_OTA_FROM_HTTP
        ota_exit_download_task();
#endif
    }

    return ret;
}

/* Used for test os firmware upgrade
 * read os fw data from file in url,write os data to update-fw.img in flash
 */
RT_UNUSED
static ota_status ota_update_image_process_test(uint8_t seq, void *url,
        ota_update_init init_cb,
        ota_update_get get_cb)
{
    uint32_t        os_addr = 0;
    uint32_t        os_size = 0;
    uint8_t         *ota_buf;
    uint8_t         eof_flag;
    uint32_t        debug_size;
    ota_status      status = OTA_STATUS_ERR;
    ota_status      ret = OTA_STATUS_ERR;
    rt_bool_t       bSucc = RT_TRUE;
    rt_uint8_t      *p_fwbuf;
    int             test_fd;
    int             res;

    struct rt_mtd_nor_device *snor_device = RT_NULL;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device == RT_NULL)
    {
        LOGD("Did not find device: snor....");
        return ret;
    }

    LOGD("block_size = %d", snor_device->block_size);

    os_size      = ota_priv.img_max_size;
    os_addr      = ota_priv.os_addr;
    LOGD("%s(), current seq %d, will update fw start addr 0x%#x\n", __func__, seq, os_addr);
    if (os_addr == 0)
    {
        LOGE("Error: fw%d address is invalide!!!");
        return ret;
    }

    ota_buf = rt_malloc_align(OTA_BUF_SIZE, 4);
    if (ota_buf == RT_NULL)
    {
        LOGE("NO memory!");
        return ret;
    }

    p_fwbuf = rt_malloc_align(OTA_BUF_SIZE, 64);
    if (p_fwbuf == RT_NULL)
    {
        LOGE("NO memory!!");
        if (ota_buf)
        {
            rt_free(ota_buf);
            ota_buf = RT_NULL;
        }
        return ret;
    }

    test_fd = open("/update-fw.img", O_WRONLY | O_CREAT | O_TRUNC, 0);
    if (test_fd < 0)
    {
        LOGE("ota open file /update-test.img Fail");
        bSucc = RT_FALSE;
        goto ota_err;
    }

    if (init_cb(url) != OTA_STATUS_OK)
    {
        LOGE("ota update init failed\n");
        bSucc = RT_FALSE;
        goto ota_err;
    }

    LOGD("OTA: start loading image...\n");
    debug_size = OTA_UPDATE_DEBUG_UNIT;
    ota_priv.get_size = 0;

    LOGD("os image max size %u \n", os_size);

#ifdef RT_USING_OTA_FROM_HTTP
    ota_start_download_task(NULL);
#endif

    while (os_size)
    {
        uint32_t len, readLen;
        len = MINIMUM(os_size, OTA_BUF_SIZE);

        status = get_cb(ota_buf, len, &readLen, &eof_flag);
        if (status != OTA_STATUS_OK)
        {
            LOGE("status %d\n", status);
            break;
        }

        rt_memcpy(p_fwbuf, ota_buf, OTA_BUF_SIZE);
        rt_memset(ota_buf, 0, OTA_BUF_SIZE);

        res = write(test_fd, p_fwbuf, readLen);
        if (res != readLen)
        {
            LOGE("Fw %#d write Error!, size %#d\n", seq, res);
            break;
        }

        if (!eof_flag)
        {
            os_size -= readLen;
            ota_priv.get_size += readLen;
        }
        else
        {
            ret = OTA_STATUS_OK;
            LOGD("Donwloaded image %u Byte (%u KB) to End \n", ota_priv.get_size,
                 ota_priv.get_size / 1024);
            break;
        }

        if (ota_priv.get_size >= debug_size)
        {
            LOGD("OTA: Donwloaded image (%u KB)...\n",
                 ota_priv.get_size / 1024);
            debug_size += OTA_UPDATE_DEBUG_UNIT;
        }

        rt_thread_delay(10);
    }
#ifdef RT_USING_OTA_FROM_HTTP
    ota_exit_download_task();
#endif

    close(test_fd);

ota_err:
    if (ota_buf)
    {
        rt_free_align(ota_buf);
        ota_buf = RT_NULL;
    }

    if (p_fwbuf)
    {
        rt_free_align(p_fwbuf);
        p_fwbuf = RT_NULL;
    }

    if (ret != OTA_STATUS_OK)
    {
        if (os_size == 0)
        {
            /* reach max os size, but not end, continue trying to check sections */
            LOGE("download os img size %u == %u, but not end",
                 ota_priv.get_size, ota_priv.img_max_size);
        }
        else
        {
            LOGE("Happen some error when downlond...");
            return ret;
        }
    }

    LOGD("finish loading image %d(0x%#08x)", ota_priv.get_size, ota_priv.get_size);

    if (bSucc)
    {
        /* do jhash check here before we set slot activity */
        if (ota_verify_img(OTA_VERIFY_JHASH) != OTA_STATUS_OK)
        {
            LOGE("ota check image failed");
            return OTA_STATUS_ERR;
        }
        LOGD("Finish checking os image.\n");

        /* Verify data partition firmware : flash data  partiton with local file*/
        if (ota_data_check(url) != OTA_STATUS_OK)
        {
            LOGE("ota check data image failed");
            return OTA_STATUS_ERR;
        }
        LOGD("Finish checking data image.\n");

        /* dsp version compare */
        if (ota_priv.data_max_size)
        {
            if (ota_dsp_version_cmp() != OTA_STATUS_OK)
            {
                LOGE("ota dsp version check failed");
                return OTA_STATUS_ERR;
            }
            LOGD("dsp version is accordant!");
        }

        if (seq == 1)       /* pending os Fw 2 slot*/
            LOGD(" ---- Set system Start from FW2 slot----");
        else if (seq == 0)  /* pending os Fw 1 slot*/
            LOGD(" ---- Set system Start from FW1 slot----");

        fw_slot_set_pending(ota_priv.fw_running_slot);
    }
    else
    {
#ifdef RT_USING_OTA_FROM_HTTP
        ota_exit_download_task();
#endif
    }

    return ret;
}

static ota_status ota_update_image_todo(void *url,
                                        ota_update_init init_cb,
                                        ota_update_get get_cb)
{
    rt_uint8_t seq;

    seq = ota_get_update_seq();
    if (seq < IMAGE_SEQ_NUM)
    {
#if OTA_TEST
        return ota_update_image_process_test(seq, url, init_cb, get_cb);
#else
        return ota_update_image_process(seq, url, init_cb, get_cb);
#endif
    }
    else
    {
        return OTA_STATUS_ERR;
    }
}

/**
 * @brief Get the image file with the specified protocol and write to flash
 * @param[in] protocol Pointer to the protocol of getting image file
 * @param[in] url URL of the image file
 * @retval ota_status_t, OTA_STATUS_OK on success
 */
ota_status ota_update_image(ota_protocol protocol, void *url)
{
    if (url == NULL)
    {
        LOGD("url %p\n", url);
        return OTA_STATUS_ERR;
    }

    switch (protocol)
    {
#if OTA_OPT_PROTOCOL_FILE
    case OTA_PROTOCOL_FILE:
        return ota_update_image_todo(url, ota_update_file_init, ota_update_file_get);
#endif

#if OTA_OPT_PROTOCOL_HTTP
    case OTA_PROTOCOL_HTTP:
        return ota_update_image_todo(url, ota_update_http_init, ota_update_http_get);
#endif
    default:
        LOGD("invalid protocol %d\n", protocol);
        return OTA_STATUS_ERR;
    }
}

static ota_status ota_verify_image_none(rt_uint8_t fw_slot)
{
    return OTA_STATUS_OK;
}

#if OTA_OPT_VERIFY_JHASH
static ota_status ota_verify_image_hash(rt_uint8_t fw_slot)
{
    int res = -1;

    res = fw_CheckHash(fw_slot);
    if (res == 0)
    {
        return OTA_STATUS_OK;
    }
    else
    {
        return OTA_STATUS_ERR;
    }
}
#endif /* OTA_OPT_EXTRA_VERIFY_SHA256 */

/**
 * @brief Verify the image file
 * @param[in] verify Verification algorithm
 * @param[in] Lun device handle
 * @retval ota_status, OTA_STATUS_OK on success
 */
ota_status ota_verify_img(ota_verify verify)
{
    ota_status    status;
    rt_uint8_t    seq;
//    image_cfg     img_state;

    seq = ota_get_update_seq();
    if (seq >= IMAGE_SEQ_NUM)
    {
        return OTA_STATUS_ERR;
    }

    LOGD("%s(), verify slot %d, size 0x%#x", __func__, seq, ota_priv.get_size);

#if OTA_IMG_DATA_CORRUPTION_TEST
    {
        int time = 3;

        LOGD("ota img data corruption test start, pls power down the device");
        while (time)
        {
            rt_thread_delay(1000);
            LOGD("Pls power down the device: %d s", time);
            time--;
        }
    }
#endif

    switch (verify)
    {
    case OTA_VERIFY_NONE:
        status = ota_verify_image_none(seq);
        break;
#if OTA_OPT_VERIFY_JHASH
    case OTA_VERIFY_JHASH:
        status = ota_verify_image_hash(seq);
        break;
#endif
    default:
        LOGE("invalid verify %d", verify);
        return OTA_STATUS_ERR;
    }
#if OTA_IMG_DATA_CORRUPTION_TEST
    LOGD("ota img data corruption test end");
    rt_thread_delay(1000);
#endif

    if (status != OTA_STATUS_OK)
    {
        LOGE("verify fail, status %d, verify %d", status, verify);
        return OTA_STATUS_ERR;
    }

    /* Verify fimeware, can do reboot */
//    img_state.seq = seq;
//    img_state.state = IMAGE_STATE_VERIFIED;
//    ota_set_img_state(&img_state);

    LOGD("OTA: finish checking image.");
    return  OTA_STATUS_OK;
}


/**
 * @brief reboot device
 * @return None
 */
void ota_reboot(int system_running)
{
    LOGD("reboot......\n");
    rt_thread_delay(300);

    rt_hw_cpu_reset();
}

void ota_main(void *arg)
{
    char *file_url = NULL;
    ota_status ret;
    uint32_t slot_boot_idx;
    fw_ab_data *ab_data;

    LOGD("%s Enter...", __func__);

#ifdef RT_USING_OTA_FROM_LOCAL
    LOGD("This is ota # LOCAL # update ....");
    /* fw file in emmc device. */
    file_url = rt_malloc(256);

    if (!file_url)
    {
        LOGD("malloc memory fail ");
        return;
    }

    /* get current os run seq */
    if (ota_init() != OTA_STATUS_OK)
        goto END;

    if (fw_slot_get_current_running(&slot_boot_idx) != OTA_STATUS_OK)
        goto END;

#ifdef RT_USING_OTA_RECOVERY
    /* if enable recovery mode and running in slot 0, should reboot to slot 1 to
     * run ota task, to do upgrade work.
     */
    if (slot_boot_idx == 0)
    {
        LOGD("!!! OTA in recovery mode !!!");
        if (file_url)
            rt_free(file_url);

        if (!fw_slot_change(1 - slot_boot_idx))
            ota_reboot(1 - slot_boot_idx);
    }
#endif

    /* according os run seq to get local fw path (url)*/
    rt_memset(file_url, 0, 256);
    rt_memcpy(file_url, OTA_FW_LOCAL_PATH, rt_strlen(OTA_FW_LOCAL_PATH));

    fw_slot_reset_flag(slot_boot_idx);

    ret = ota_update_image(OTA_PROTOCOL_FILE, file_url);
    if (OTA_STATUS_OK == ret)
    {
        if (file_url)
            rt_free(file_url);

        //if (!fw_slot_change(1 - slot_boot_idx))
        ota_reboot(1 - slot_boot_idx);
    }
    else
    {
        LOGE("ota update image Fail!!!\n");
        goto END;
    }

#elif (defined RT_USING_OTA_FROM_HTTP)
    LOGD("This is ota http update ....");

    file_url = rt_malloc(256);

    if (!file_url)
    {
        LOGD("malloc memory fail ");
        return;
    }

    rt_memset(file_url, 0, 256);

    /* get current fw run slot */
    if (ota_init() != OTA_STATUS_OK)
        goto END;

    if (fw_slot_get_current_running(&slot_boot_idx) != OTA_STATUS_OK)
        goto END;

    /* according fw run slot to get fw position in one firmare url file*/
    rt_memcpy(file_url, RT_OTA_HTTP_URL, rt_strlen(RT_OTA_HTTP_URL));

    ret = ota_update_image(OTA_PROTOCOL_HTTP, file_url);
    if (OTA_STATUS_OK == ret)
    {
        LOGD("Fw have wrote flash Success!!!.\n");
        if (file_url)
            rt_free(file_url);

        ota_reboot(1 - slot_boot_idx);
    }
    else
        LOGE("ota update image Fail!!!\n");

#endif

END:
    LOGE(">>>>>>>> EXIT ota main");
    if (file_url)
        rt_free(file_url);

    if (slot_boot_idx == 1)
    {
        /* fw A never boot success and not found firmware to update ,
            we switch boot to maskrom
        */
        ab_data = fw_ab_data_get();
        if (ab_data->slots[0].successful_boot == 0
                && ab_data->slots[0].tries_remaining == 0)
        {
            LOGE("###### Fw A boot never Success,will boot to maskrom! ######\n");
            BSP_SetLoaderFlag();
            ota_reboot(0);
        }
    }
}

rt_thread_t ota_task_start(void)
{
    LOGD("%s\n", __func__);
    rt_thread_t ota_task_thread;
    ota_task_thread = rt_thread_create("ota_task",
                                       ota_main,
                                       RT_NULL,
                                       8192 * 2,
                                       15,
                                       20);

    if (!ota_task_thread)
    {
        LOGD("ota task create failed");
        return NULL;
    }
    else
        rt_thread_startup(ota_task_thread);

    return ota_task_thread;
}

void ota_task_exit(rt_thread_t h_vt)
{
    RT_ASSERT(h_vt != NULL);
    rt_thread_delete(h_vt);
}

/* -------------------------------------------------------------------------- */
/*
 * Below code just for aging test
*/
void ota_test_task(void *arg)
{
    char *file_url = NULL;
    ota_status ret;
    uint32_t slot_boot_idx;

    LOGD("%s Enter...", __func__);

#ifdef RT_USING_OTA_FROM_LOCAL
    LOGD("This is ota # LOCAL # test ....");
    /* fw file in emmc device. */
    file_url = rt_calloc(1, 256);
    if (!file_url)
    {
        LOGD("malloc memory fail ");
        return;
    }

    /* get current os run seq */
    if (ota_init() != OTA_STATUS_OK)
        goto END;

    if (fw_slot_get_current_running(&slot_boot_idx) != OTA_STATUS_OK)
        goto END;

    /* according os run seq to get local fw path (url)*/
    rt_memcpy(file_url, OTA_FW_LOCAL_PATH, rt_strlen(OTA_FW_LOCAL_PATH));

    fw_slot_reset_flag(slot_boot_idx);
    ret = ota_update_image(OTA_PROTOCOL_FILE, file_url);
    if (OTA_STATUS_OK != ret)
    {
        LOGE("ota update image Fail!!!\n");
        goto END;
    }

#ifdef RT_USING_OTA_RECOVERY
    /* if enable recovery mode and running in slot 0, should reboot to slot 1 to
     * run ota task, to do upgrade work.
     */
    if (slot_boot_idx == 0 && OTA_STATUS_OK == ret)
    {
        LOGD("!!! OTA type is recovery mode, will reboot to run Fw2 !!!");
        if (file_url)
            rt_free(file_url);

        if (!fw_slot_change(1 - slot_boot_idx))
            ota_reboot(1 - slot_boot_idx);
    }
#endif  //#ifdef RT_USING_OTA_RECOVERY

#endif  //#ifdef RT_USING_OTA_FROM_LOCAL

END:
    if (file_url)
        rt_free(file_url);
}


void ota_test_loop()
{
    LOGD("%s\n", __func__);
    rt_thread_t ota_task_thread;
    ota_task_thread = rt_thread_create("ota_test_task",
                                       ota_test_task,
                                       RT_NULL,
                                       8192 * 2,
                                       15,
                                       20);

    if (!ota_task_thread)
    {
        LOGD("ota test task create failed");
        return;
    }
    else
        rt_thread_startup(ota_task_thread);
}

#endif

