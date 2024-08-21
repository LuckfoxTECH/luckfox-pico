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

#ifdef RT_USING_OTA_FROM_LOCAL

#include "fw_analysis.h"
#include "rkpart.h"
#ifdef RT_USING_DSP
#include "dsp_fw/dsp_fw_version.h"
#endif
#include "ota.h"
#include "ota_file.h"

#if OTA_OPT_PROTOCOL_FILE

#define DBG_SECTION_NAME    "OTA_FILE"
#include "rkdebug.h"

typedef struct ota_fs_param
{
    char      *url;
    uint32_t  imgsize;
    int       imgfile;
} ota_fs_param_st;

typedef enum
{
    UPDATE_NULL,
    UPDATE_SYS_START,
    UPDATE_SYS_DONE,
    UPDATE_DATA_START,
    UPDATE_DATA_DONE,
    UPDATE_USER_START,
    UPDATE_USER_DONE
} ota_file_state;

extern rt_uint32_t g_fw_curr_slot;
extern rt_uint32_t g_user_curr_slot;
extern rt_uint32_t g_os_real_size;
extern rt_uint32_t g_data_real_size;
extern rt_uint32_t g_user_real_size;


static ota_fs_param_st *g_fs_param;
static rt_uint32_t gImgSize;
static int g_ota_file_state;

rt_bool_t ota_is_exist_fw_file(char *path)
{
    int fd;
    rt_bool_t is_exist = RT_FALSE;

    LOGD("%s: Enter", __func__);
    LOGD("Check %s", path);
    fd = open(path, O_RDONLY, 0);

    if (fd < 0)
    {
        LOGD("ota_is_exist_fw_file: open file for check failed\n");
        return is_exist;
    }
    else
    {
        close(fd);
        is_exist = RT_TRUE;
        LOGD("Found %s", path);
    }

    return is_exist;
}

static ota_status ota_update_file_check(void *url)
{
    uint8_t TmpBuf[512];
    PFIRMWARE_HEADER pFWHead;
    unsigned char magic[8] = {'R', 'E', 'S', 'C', 0, 0, 0, 0};
    ota_status ret = OTA_STATUS_ERR;
    uint32_t addr = 0;
    int fd = -1;
    int length = 0;

    if (g_fs_param == NULL)
        return OTA_STATUS_ERR;

    LOGD("%s : Enter", __func__);
    fd = open((const char *)url, O_RDONLY, 0);
    if (fd < 0)
    {
        LOGE("check: open file for read failed\n");
        return OTA_STATUS_ERR;
    }
    g_fs_param->imgfile = fd;

    LOGD("Current run %d slot fw, will update %d slot fw!", g_fw_curr_slot, 1 - g_fw_curr_slot);
    LOGD("fw1 addr 0x%#x, fw2 addr 0x%#x", firmware_addr1, firmware_addr2);
    LOGD("Data addr 0x%#x", data_start_addr);

    if (g_fw_curr_slot == 0)
        addr = firmware_addr2;
    else if (g_fw_curr_slot == 1)
        addr = firmware_addr1;
    else
        return OTA_STATUS_ERR;

    lseek(fd, addr, SEEK_SET);

    length = read(fd, TmpBuf, sizeof(TmpBuf));
    if (sizeof(TmpBuf) != length)
    {
        LOGE("read image file Error!");
        return OTA_STATUS_ERR;
    }

    pFWHead = (PFIRMWARE_HEADER)TmpBuf;
    if (0 != rt_memcmp(pFWHead->magic, magic, 8))
    {
        LOGE("Fw head Error!");
        return OTA_STATUS_ERR;
    }

    g_fs_param->imgsize = pFWHead->data_size + 512 + 4;
    gImgSize            = pFWHead->data_size + 512 + 4;
    g_os_real_size      = gImgSize;

    LOGD("fw data offset = %u(0x%#x)", pFWHead->data_offset, pFWHead->data_offset);
    LOGD("fw data size = %u(0x%#x)", pFWHead->data_size, pFWHead->data_size);
    LOGD("fw file size = %u(0x%#x)", gImgSize, gImgSize);

    ret = OTA_STATUS_OK;
    g_ota_file_state = UPDATE_SYS_START;

    /* go back to the fw data position */
    lseek(fd, addr, SEEK_SET);
    return ret;
}

ota_status ota_update_file_init(void *url)
{
    LOGD("%s() Enter", __func__);
    /* fw file in emmc device. */
    rt_bool_t is_exist = RT_FALSE;
    int retry_cnt = 0;

    if (g_fs_param == NULL)
    {
        g_fs_param = rt_malloc(sizeof(ota_fs_param_st));
        if (g_fs_param == NULL)
        {
            LOGE("fs param malloc fail");
            return OTA_STATUS_ERR;
        }
    }

RETRY:
    /* Judge whether fw url is exist */
    is_exist = ota_is_exist_fw_file((char *)url);

    /* exist fw */
    if (is_exist)
    {
        g_fs_param->url = rt_strdup((const char *)url);
        if (ota_update_file_check(url) != OTA_STATUS_OK)
            return OTA_STATUS_ERR;
    }
    else
    {
        LOGD("Not found FW in device, retry %d times !!!", retry_cnt + 1);
        rt_thread_delay(1000);

        if (++retry_cnt < 5)
            goto RETRY;
        else
            goto END;
    }

    LOGD("%s() Success", __func__);
    return OTA_STATUS_OK;
END:
    return OTA_STATUS_ERR;
}

static ota_status ota_update_file_os_proc(void)
{
    rt_int32_t ret = -1;
    int fd;

    fd = g_fs_param->imgfile;

    if (g_ota_file_state == UPDATE_SYS_START)
    {
        g_ota_file_state = UPDATE_SYS_DONE;
        //LOGD("Read System %s partiton have End!", g_fw_curr_slot == 0 ? "B" : "A");
        //LOGD("ota_file_state is UPDATE_SYS_DONE");

        if (data_part_size)
        {
            //LOGD("will seek to data part pos = 0x%#x", data_start_addr);
            ret = lseek(fd, data_start_addr, SEEK_SET);
            if (ret < 0)
            {
                LOGE("ERROR: lseek happen err.The data part not exist ???");
                ota_update_file_deinit();
                return OTA_STATUS_ERR;
            }

            /* ready for update data part size */
            gImgSize         = data_part_size;
            /* data partition realsize we don't know. here we use partiton size */
            g_data_real_size = gImgSize;
            g_ota_file_state = UPDATE_DATA_START;
            //LOGD("ota_file_state is UPDATE_DATA_START");
        }
        else
        {
            ota_update_file_deinit();
        }
    }
    else if (g_ota_file_state == UPDATE_DATA_START)
    {
        g_ota_file_state = UPDATE_DATA_DONE;
        //LOGD("Read data part have End!");
        ota_update_file_deinit();
    }

    return OTA_STATUS_OK;
}

#ifdef RT_SUPPORT_ROOT_AB
static ota_status ota_update_file_root_proc(void)
{
    rt_int32_t ret = -1;
    rt_uint32_t user_addr = 0;
    int fd;

    fd = g_fs_param->imgfile;

    if (g_ota_file_state == UPDATE_SYS_START)
    {
        g_ota_file_state = UPDATE_SYS_DONE;
        //LOGD("Read OS FW End!");
    }
    else if (g_ota_file_state == UPDATE_USER_START)
    {
        g_ota_file_state = UPDATE_USER_DONE;
        //LOGD("Read USER FW End!");
        ota_update_file_deinit();
        return OTA_STATUS_OK;
    }

    if (g_user_curr_slot == 0)
    {
        user_addr = user_start_addr2;
        //LOGD("will seek to pos = %d", user_addr);
        ret = lseek(fd, user_addr, SEEK_SET);
        if (ret < 0)
        {
            LOGE("ERROR: lseek happen err. The user fw2 not exist ?");
            ota_update_file_deinit();
            return OTA_STATUS_ERR;
        }

        /* update user part size */
        gImgSize         = user_part_size;
        g_ota_file_state = UPDATE_USER_START;
        //LOGD("user part image 2 size %#d", gImgSize);
    }
    else if (g_user_curr_slot == 1)
    {
        user_addr = user_start_addr1;
        LOGD("will seek to pos = %d", user_addr);
        ret = lseek(fd, user_addr, SEEK_SET);
        if (ret < 0)
        {
            LOGE("ERROR: lseek happen err.The user fw1 not exist ?");
            ota_update_file_deinit();
            return OTA_STATUS_ERR;
        }

        /* update user part size */
        gImgSize         = user_part_size;
        g_ota_file_state = UPDATE_USER_START;
        //LOGD("user part image 1 size %#d", gImgSize);
    }
    else
        return OTA_STATUS_ERR;

    return OTA_STATUS_OK;
}
#endif

ota_status ota_update_file_get(uint8_t *buf, uint32_t buf_size, uint32_t *recv_size, uint8_t *eof_flag)
{
    uint32_t len = 0;
    int fd;

    fd = g_fs_param->imgfile;
    if (gImgSize > 0 && gImgSize <= buf_size)
    {
        buf_size = gImgSize;
    }

    len = read(fd, buf, buf_size);
    //LOGD("len = %u buf_size = %u left = %u",len, buf_size, gImgSize);

    if (len != buf_size && gImgSize != len)
    {
        LOGE("Read FW Error! len = %d buf_size = %d", len, buf_size);
        rt_free(g_fs_param->url);
        rt_free(g_fs_param);
        g_fs_param = NULL;
        return OTA_STATUS_ERR;
    }

    *recv_size = len;
    gImgSize -= len;

    if (gImgSize == 0)
    {
        *eof_flag = 1;

        if (ota_update_file_os_proc() != OTA_STATUS_OK)
            return OTA_STATUS_ERR;

#ifdef RT_SUPPORT_ROOT_AB
        if (ota_update_file_root_proc() != OTA_STATUS_OK)
            return OTA_STATUS_ERR;
#endif
    }
    else
        *eof_flag = 0;

    return OTA_STATUS_OK;
}

ota_status ota_update_file_deinit(void)
{
    close(g_fs_param->imgfile);
    rt_free(g_fs_param->url);
    rt_free(g_fs_param);
    g_fs_param = NULL;
    g_ota_file_state = UPDATE_NULL;
    //LOGD("ota_update_file_deinit done!");
    return OTA_STATUS_OK;
}

ota_status ota_data_check(void *url)
{
    rt_uint8_t *pBuf = RT_NULL;
    ota_status ret = OTA_STATUS_ERR;
    uint32_t addr = 0, data_size = 0;
    int fd = -1;
    int length = 0;
    int rst = -1;
    rt_uint8_t *dataBuf = RT_NULL;
    struct rt_mtd_nor_device *snor_dev = RT_NULL;

    LOGD("%s : Enter", __func__);

    snor_dev = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_dev ==  RT_NULL)
    {
        LOGE("Did not find device: snor....");
        return ret;
    }

    dataBuf = (rt_uint8_t *)rt_malloc(4 * 1024);
    if (dataBuf == NULL)
        return ret;

    pBuf = rt_malloc(OTA_BUF_SIZE);
    if (pBuf == RT_NULL)
        return ret;

    fd = open((const char *)url, O_RDONLY, 0);
    if (fd < 0)
    {
        LOGE("check: open file for read failed");
        goto ERROR;
    }

    rst = lseek(fd, data_start_addr, SEEK_SET);
    if (rst < 0)
    {
        LOGE("ERROR: lseek happen err.The data part not exist ???");
        goto ERROR;
    }

    addr = data_start_addr;
    data_size = data_part_size;
    while (data_size > 0)
    {
        uint32_t len;
        len = MINIMUM(data_size, OTA_BUF_SIZE);
        if (len != rt_mtd_nor_read(snor_dev, addr, (rt_uint8_t *)dataBuf, len))
        {
            LOGE("rt_mtd_nor_read error");
            goto ERROR;
        }

        length = read(fd, pBuf, len);
        if (length != OTA_BUF_SIZE)
        {
            LOGE("Read FW Error! len = %d", length);
            goto ERROR;
        }

        if (memcmp(pBuf, dataBuf, OTA_BUF_SIZE) != 0)
        {
            LOGE("Data part check fail, addr = 0x%#x", addr);
            break;
        }

        data_size -= len;
        addr += len;
    }

    if (data_size == 0)
    {
        ret = OTA_STATUS_OK;
        LOGD("Data partition check Pass");
    }

ERROR:
    if (pBuf)
        rt_free(pBuf);
    if (dataBuf)
        rt_free(dataBuf);

    close(fd);
    return ret;
}

/* read from dsp version from flash (data partiton start 16 bytes),
   compare with dsp_fw_version string.
*/
ota_status ota_dsp_version_cmp(void)
{
#ifdef RT_USING_DSP
    ota_status ret = OTA_STATUS_ERR;
    uint32_t addr = 0;
    int len = 0;
    rt_uint8_t dataBuf[32] = {0};
    struct rt_mtd_nor_device *snor_dev = RT_NULL;

    LOGD("%s : Enter", __func__);

    snor_dev = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_dev ==  RT_NULL)
    {
        LOGE("Did not find device: snor....");
        return ret;
    }

    addr = data_start_addr;
    len = sizeof(dsp_fw_version);
    if (len != rt_mtd_nor_read(snor_dev, addr, (rt_uint8_t *)dataBuf, len))
    {
        LOGE("rt_mtd_nor_read error");
        return ret;
    }

    LOGI("DSP_VERSION flash: %s", dataBuf);
    LOGI("DSP_VERSION sram : %s", dsp_fw_version);

    if (memcmp(dsp_fw_version, dataBuf, sizeof(dsp_fw_version)) == 0)
    {
        ret = OTA_STATUS_OK;
        LOGD("dsp version check ok !!!");
    }

    return ret;
#else
    return OTA_STATUS_OK;
#endif
}


#endif
#endif
