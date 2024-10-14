/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#include <rtdevice.h>
#include <rtthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <drivers/mtd_nor.h>
#include "hal_base.h"

//#include "FwUpdate.h"
#include "fw_analysis.h"

#include "rkpart.h"
#include "vendor_ops.h"

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/
#define D_MODULE_CNT 40

#define RKDEVICE_SN_LEN 60
#define RKDEVICE_UID_LEN 30
#define RKDEVICE_MAC_LEN 6
#define RKDEVICE_WIFI_LEN 6
#define RKDEVICE_BT_LEN 6
#define SEC3_RESERVED_LEN 405

typedef __PACKED_STRUCT tagFLASH_IDB_SEC3
{
    rt_uint16_t  usSNSize;
    rt_uint8_t   sn[RKDEVICE_SN_LEN];
    rt_uint8_t   reserved[SEC3_RESERVED_LEN];
    rt_uint8_t   uidSize;
    rt_uint8_t   uid[RKDEVICE_UID_LEN];
    rt_uint8_t   blueToothSize;
    rt_uint8_t   blueToothAddr[RKDEVICE_BT_LEN];
    rt_uint8_t   macSize;
    rt_uint8_t   macAddr[RKDEVICE_MAC_LEN];
} FLASH_IDB_SEC3, *PFLASH_IDB_SEC3;
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/
FIRMWARE_INF gstFwInf;
uint16_t  UpgradeTimes;
uint16_t  LoaderVer;
rt_uint32_t  SysProgDiskCapacity;
rt_uint32_t  SysProgRawDiskCapacity;
int32_t  FW1Valid, FW2Valid;
rt_uint32_t FwSysOffset;
rt_uint32_t IdbBlockOffset;
rt_sem_t FwOperSem;
rt_sem_t DbOperSem;

extern uint32_t firmware_addr1;
extern uint32_t firmware_addr2;

static fw_ab_data g_fw_ab_data;
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable define
*
*---------------------------------------------------------------------------------------------------------------------
*/



/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function declare
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
unsigned int jshash(unsigned int hash, char *str, unsigned int len)
{
    unsigned int i    = 0;

    for (i = 0; i < len; str++, i++)
    {
        hash ^= ((hash << 5) + (*str) + (hash >> 2));
    }
    return hash;
}

void DumpData(char *buf, int bufSize, int radix)
{
    int i = 0;

    rt_kprintf("\n");
    for (i = 0; i < bufSize; i++)
    {
        if ((i > 0) && ((i % 16) == 0))
            rt_kprintf("\n");

        if (radix == 10)
            rt_kprintf(" %d", buf[i]);
        else
            rt_kprintf(" %02X", buf[i]);
    }
    rt_kprintf("\n");
}

/*******************************************************************************
** Name: fw_GetBtMac
** Input:void * pBtMac
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2017.11.3
** Time: 15:34:42
*******************************************************************************/
rt_err_t fw_GetBtMac(void *pBtMac)
{
    rt_err_t ret;
    char *btbuf = pBtMac;
    ret = get_device_sn(DEV_BT_MAC, btbuf, 127);
    if (ret > 0)
        return ret;

    return RT_EOK;
}

/*******************************************************************************
** Name: fw_GetWifiMac
** Input:void * pBtMac
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2017.11.3
** Time: 15:34:42
*******************************************************************************/
rt_err_t fw_GetWifiMac(void *pWifiMac)
{
    rt_err_t ret;
    ret = get_device_sn(DEV_WLAN_MAC, (char *)pWifiMac, 127);
    if (ret > 0)
        return ret;

    return RT_EOK;
}

/*******************************************************************************
** Name: fw_GetLanMac
** Input:void * pLanMac
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2017.11.3
** Time: 15:34:42
*******************************************************************************/
rt_err_t fw_GetLanMac(void *pLanMac)
{
    rt_err_t ret;
    ret = get_device_sn(DEV_LAN_MAC, (char *)pLanMac, 127);
    if (ret > 0)
        return ret;

    return RT_EOK;
}

/*******************************************************************************
** Name: fw_CodePageDeInit
** Input:void
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2016.9.8
** Time: 15:52:00
*******************************************************************************/
rt_err_t fw_CodePageDeInit(void)
{
    return RT_EOK;
}
/*******************************************************************************
** Name: FwResume
** Input:void
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2016.8.16
** Time: 15:45:06
*******************************************************************************/
rt_err_t FwResume(void)
{
    return RT_EOK;
}

/*******************************************************************************
** Name: FwSuspend
** Input:void
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2016.8.16
** Time: 15:44:39
*******************************************************************************/
rt_err_t FwSuspend(void)
{
    return RT_EOK;
}

/*******************************************************************************
** Name: FwResume
** Input:void
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2016.8.16
** Time: 15:45:06
*******************************************************************************/
rt_err_t DBResume(void)
{
    return RT_EOK;
}

/*******************************************************************************
** Name: FwSuspend
** Input:void
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2016.8.16
** Time: 15:44:39
*******************************************************************************/
rt_err_t DBSuspend(void)
{
    return RT_EOK;
}

/*******************************************************************************
** Name: fw_DBInit
** Input:void
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2016.8.4
** Time: 10:01:13
*******************************************************************************/
rt_err_t fw_DBInit(void)
{
    return RT_EOK;
}

/*******************************************************************************
** Name: Fw_GetModle
** Input:rt_uint32_t *version,the version if current fw.
** Return: the length of current modle string.
** Owner:chad.ma
** Date: 2020.1.6
** Time: 11:48:20
*******************************************************************************/
rt_uint32_t fw_GetModle(void *modle, int fw_slot)
{
    rt_uint32_t addr;
    rt_uint32_t modle_len = 0;
    unsigned char magic[8] = {'R', 'E', 'S', 'C', 0, 0, 0, 0};
    PFIRMWARE_HEADER pFWHead1 = NULL;

    rt_uint8_t *dataBuf = NULL;
    struct rt_mtd_nor_device *snor_dev = RT_NULL;

    rt_kprintf("\n ### %s() Enter ### \n", __func__);

    snor_dev = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_dev ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }

    dataBuf = (rt_uint8_t *)rt_malloc(512);
    if (dataBuf == NULL)
    {
        rt_kprintf("No mem\n");
        goto END;
    }

    if (fw_slot == 0)       //FW A
        addr = firmware_addr1;
    else if (fw_slot == 1)  // FW B
        addr = firmware_addr2;
    else
        goto END;

    if (512 != rt_mtd_nor_read(snor_dev, addr, (rt_uint8_t *)dataBuf, 512))
    {
        rt_kprintf("rt_mtd_nor_read error\n");
        goto END;
    }

    pFWHead1 = (PFIRMWARE_HEADER)dataBuf;
    if (0 != rt_memcmp(pFWHead1->magic, magic, 4))
    {
        rt_kprintf("FW magic ERR addr:0x%x\n", firmware_addr1);
        goto END;
    }

    if (modle)
    {
        rt_memcpy(modle, pFWHead1->model, rt_strlen((const char *)pFWHead1->model));
        modle_len = rt_strlen((const char *)pFWHead1->model);
    }

END:
    if (dataBuf)
        rt_free(dataBuf);
    return modle_len;
}

/*******************************************************************************
** Name: fw_GetChipType
** Input:void *chipType,the buff to save current firmware chip type.
** Return: the length of chip type string
** Owner:chad.ma
** Date: 2020.1.6
** Time: 11:48:20
*******************************************************************************/
rt_uint32_t fw_GetChipType(void *chipType, int fw_slot)
{
    rt_uint32_t addr;
    rt_uint32_t chipLen = 0;
    unsigned char magic[8] = {'R', 'E', 'S', 'C', 0, 0, 0, 0};
    PFIRMWARE_HEADER pFWHead1 = NULL;

    rt_uint8_t *dataBuf = NULL;
    struct rt_mtd_nor_device *snor_dev = RT_NULL;

    rt_kprintf("\n ### %s() Enter ### \n", __func__);

    snor_dev = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_dev ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }

    dataBuf = rt_malloc_align(512, 64);
    if (dataBuf == NULL)
    {
        rt_kprintf("No mem\n");
        goto END;
    }

    if (fw_slot == 0)       //FW A
        addr = firmware_addr1;
    else if (fw_slot == 1)  // FW B
        addr = firmware_addr2;
    else
        goto END;
    rt_kprintf("\n ### fw slot %d, addr = 0x%#x  fw_addr = 0x%#x ### \n", fw_slot, addr,
               fw_slot == 0 ? firmware_addr1 : firmware_addr2);

    if (512 != rt_mtd_nor_read(snor_dev, addr, (uint8_t *)dataBuf, 512))
    {
        rt_kprintf("rt_mtd_nor_read error\n");
        goto END;
    }

    pFWHead1 = (PFIRMWARE_HEADER)dataBuf;
    if (0 != rt_memcmp(pFWHead1->magic, magic, 4))
    {
        rt_kprintf("FW magic ERR addr:0x%x\n", firmware_addr1);
        goto END;
    }

    if (chipType)
    {
        rt_memcpy(chipType, pFWHead1->chip, rt_strlen((const char *)pFWHead1->chip));
        chipLen = rt_strlen((const char *)pFWHead1->chip);
    }

END:
    if (dataBuf)
        rt_free_align(dataBuf);
    return chipLen;
}

/*******************************************************************************
** Name: fw_GetVersion
** Input:rt_uint32_t *version,the version if current fw.
** Return: rt_err_t
** Owner:chad.ma
** Date: 2019.7.3
** Time: 11:48:20
*******************************************************************************/
rt_err_t fw_GetVersion(rt_uint32_t *version, int fw_slot)
{
    unsigned char magic[8] = {'R', 'E', 'S', 'C', 0, 0, 0, 0};
    PFIRMWARE_HEADER pFWHead;
    rt_uint8_t *dataBuf = RT_NULL;
    rt_uint32_t addr;
    rt_uint32_t fwVersion = 0;
    struct rt_mtd_nor_device *snor_dev = RT_NULL;

    rt_kprintf("\n ### %s() Enter ### \n", __func__);

    snor_dev = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_dev ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }

    dataBuf = (rt_uint8_t *)rt_malloc(512);
    if (dataBuf == NULL)
    {
        rt_kprintf("No mem\n");
        goto end;
    }

    if (fw_slot == 0)       //FW A
        addr = firmware_addr1;
    else if (fw_slot == 1)  // FW B
        addr = firmware_addr2;
    else
        goto end;

    if (512 != rt_mtd_nor_read(snor_dev, addr, (uint8_t *)dataBuf, 512))
    {
        rt_kprintf("rt_mtd_nor_read error\n");
        goto end;
    }

    pFWHead = (PFIRMWARE_HEADER)dataBuf;
    if (0 != rt_memcmp(pFWHead->magic, magic, 4))
    {
        rt_kprintf("FW magic ERR addr:0x%x\n", firmware_addr1);
        goto end;
    }

    fwVersion = pFWHead->version.major << 24 |
                pFWHead->version.minor << 16 |
                pFWHead->version.small;
end:
    *version = fwVersion;
    if (dataBuf)
        rt_free(dataBuf);
    return 0;
}

/*******************************************************************************
** Name: fw_GetVersionStr
** Input:char *dest,the buff to save version of current fw.
** Return: the firmware version string length.
** Owner:chad.ma
** Date: 2020.1.6
** Time: 11:48:20
*******************************************************************************/
int fw_GetVersionStr(char *dest, int fw_slot)
{
    /*transform version = 0x1000001 to string like "1.0.0001" */
    rt_uint32_t version = 0;
    int tmp[3] = {0};
    char verStr[13] = {0};

    if (fw_slot != 0 && fw_slot != 1)
        return 0;

    fw_GetVersion(&version, fw_slot);
    tmp[0] = version >> 24 & 0xFF;
    tmp[1] = version >> 16 & 0xFF;
    tmp[2] = version       & 0xFFFF;
    rt_sprintf(verStr, "%1d%c%02d%c%04d", tmp[0], '.', tmp[1], '.', tmp[2]);
    rt_kprintf("Cur Fw Version = %s \n", verStr);
    rt_strncpy(dest, verStr, rt_strlen((const char *)verStr));
    return rt_strlen(verStr);
}

/*******************************************************************************
** Name: fw_CheckHash
** Input:fw slot which will to check.
** Return: the result of check.
** Owner:chad.ma
** Date: 2020.2.28
** Time: 11:48:20
*******************************************************************************/
int fw_CheckHash(rt_uint8_t fw_slot)
{
    unsigned char magic[8] = {'R', 'E', 'S', 'C', 0, 0, 0, 0};
    PFIRMWARE_HEADER pFWHead;
    rt_uint32_t fw_data_size, hash_data, js_hash;
    rt_uint32_t addr, fw_addr;
    rt_uint8_t *dataBuf = RT_NULL;
    struct rt_mtd_nor_device *snor_dev = RT_NULL;
    int ret = -RT_ERROR;

    rt_kprintf("\n### %s() Enter ###\n", __func__);

    snor_dev = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_dev ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return ret;
    }
    rt_kprintf("%s: check fw slot %d\n", __func__, fw_slot);

    dataBuf = (rt_uint8_t *)rt_malloc(4 * 1024);
    if (dataBuf == NULL)
    {
        rt_kprintf("No mem\n");
        goto end;
    }

    if (fw_slot == 0)       //FW A
        fw_addr = firmware_addr1;
    else if (fw_slot == 1)  // FW B
        fw_addr = firmware_addr2;
    else
        goto end;

    if (512 != rt_mtd_nor_read(snor_dev, fw_addr, (rt_uint8_t *)dataBuf, 512))
    {
        rt_kprintf("rt_mtd_nor_read error\n");
        goto end;
    }

    pFWHead = (PFIRMWARE_HEADER)dataBuf;
    if (0 != memcmp(pFWHead->magic, magic, 4))
    {
        rt_kprintf("FW magic ERR addr:0x%x\n", fw_addr);
        goto end;
    }

    rt_kprintf("fw_addr = %d (0x%#x)\n", fw_addr, fw_addr >> 9);
    rt_kprintf("data_size = %d\n",  pFWHead->data_size);
    rt_kprintf("data_offset = %d\n", pFWHead->data_offset);


    /*verify fireware hash*/
    hash_data = js_hash = 0;
    addr = fw_addr + pFWHead->data_offset + pFWHead->data_size;    //move to fw hash offset

    if (4 != rt_mtd_nor_read(snor_dev, addr, (rt_uint8_t *)&js_hash, 4))
    {
        rt_kprintf("rt_mtd_nor_read js hash error\n");
        goto end;
    }

    fw_data_size  = pFWHead->data_size + 512;
    addr = fw_addr;
    rt_kprintf("FW Check start:%#x, data size =%#x, js_hash =%#x\n", addr, fw_data_size, js_hash);

    while (fw_data_size)
    {
        rt_uint32_t len;
        len = MIN(fw_data_size, 4 * 1024);

        if (len != rt_mtd_nor_read(snor_dev, addr, (rt_uint8_t *)dataBuf, len))
        {
            rt_kprintf("rt_mtd_nor_read error\n");
            goto end;
        }

        hash_data = jshash(hash_data, (char *)dataBuf, len);
        addr += len;
        fw_data_size -= len;
    }

    rt_kprintf("FW Check End: calc hash = %#x, read js_hash = %#x\n", hash_data, js_hash);
    if (hash_data != js_hash)
    {
        rt_kprintf("FW hash ERR:0x%x, 0x%x\n", hash_data, js_hash);
        goto end;
    }

    ret = RT_EOK;
end:
    if (dataBuf)
    {
        rt_free(dataBuf);
        dataBuf = RT_NULL;
    }
    return ret;
}

/*******************************************************************************
** Name: fw_GetProductSn
** Input:void *pSn, max len must be 128
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2016.4.26
** Time: 11:48:20
*******************************************************************************/
rt_err_t fw_GetProductSn(void *pSn)
{
    rt_err_t ret;
    ret = get_device_sn(DEV_SN, (char *)pSn, 127);
    if (ret > 0)
    {
        return ret;
    }
    else
    {
        return RT_EOK;
    }
}

/*******************************************************************************
** Name: fw_Resource_DeInit
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2016.3.16
** Time: 17:08:56
*******************************************************************************/
void fw_Resource_DeInit(void)
{
    rt_sem_delete(FwOperSem);
}

/*******************************************************************************
** Name: fw_GetDBInf
** Input:DATABASE_INF * pstDataBaseInf
** Return: void
** Owner:aaron.sun
** Date: 2015.10.28
** Time: 17:35:28
*******************************************************************************/
void fw_GetDBInf(DATABASE_INFO *pstDataBaseInf)
{
}

/*******************************************************************************
** Name: fw_ReadDataBaseBySector
** Input:rt_uint8_t * buf,  rt_uint32_t SecCnt
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2015.10.28
** Time: 16:56:13
*******************************************************************************/
rt_err_t fw_ReadDataBaseBySector(rt_uint32_t LBA, rt_uint8_t *buf,  rt_uint32_t SecCnt)
{
//    return LUNReadDB(LBA, SecCnt, buf);
    return RT_EOK;
}

/*******************************************************************************
** Name: fw_WriteDataBaseBySector
** Input:rt_uint8_t * buf, rt_uint32_t SecCnt
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2015.10.28
** Time: 16:54:24
*******************************************************************************/
rt_err_t fw_WriteDataBaseBySector(rt_uint32_t LBA, rt_uint8_t *buf, rt_uint32_t SecCnt)
{
//    return LUNWriteDB(LBA, SecCnt, buf);
    return RT_EOK;
}

rt_uint8_t   FlashBuf1[1][512];
rt_uint32_t  FlashSec1[1] = {0xffffffff};
/*******************************************************************************
** Name: fw_ReadFirmwaveByByte
** Input:rt_uint32_t Addr, rt_uint8_t *pData, rt_uint32_t length
** Return: rt_err_t
** Owner:aaron.sun
** Date: 2015.10.28
** Time: 15:33:55
*******************************************************************************/
rt_err_t fw_ReadFirmwaveByByte(rt_uint32_t Addr, rt_uint8_t *pData, rt_uint32_t length)
{
    struct rt_mtd_nor_device *snor_dev = RT_NULL;

    rt_kprintf("\n ### %s() Enter ### \n", __func__);
    rt_sem_take(FwOperSem, RT_WAITING_FOREVER);

    snor_dev = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_dev ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }

    if (length == rt_mtd_nor_read(snor_dev, Addr, pData, length))
    {
        rt_sem_release(FwOperSem);
    }
    else
    {
        rt_sem_release(FwOperSem);
        return -RT_ERROR;
    }
    return RT_EOK;
}

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(common) define
*
*---------------------------------------------------------------------------------------------------------------------
*/

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
/*******************************************************************************
** Name: fw_Resource_Init
** Input:void
** Return: void
** Owner:aaron.sun
** Date: 2015.10.28
** Time: 15:45:24
*******************************************************************************/
void fw_Resource_Init(void)
{
    rt_uint8_t  FlashBuf[1024];
    FIRMWARE_HEADER       *pFWHead;
    FIRMWARE_INF *pFW_Resourec_addr;
    struct rt_mtd_nor_device *snor_dev = RT_NULL;

    pFW_Resourec_addr =  &gstFwInf;

    FwOperSem = rt_sem_create("operSem", 1, RT_IPC_FLAG_FIFO);
    rt_kprintf("\n ### %s() Enter ### \n", __func__);

    snor_dev = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_dev ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return;
    }

    if (512 == rt_mtd_nor_read(snor_dev, 0, FlashBuf, 512))
    {
        ////////////////////////////////////////////////////////////////////////////
        //read resource module address.
        pFWHead = (FIRMWARE_HEADER *)FlashBuf;

        pFW_Resourec_addr->CodeLogicAddress  =  0;

        pFW_Resourec_addr->FirmwareYear = pFWHead->release_date.year;
        pFW_Resourec_addr->FirmwareDate = pFWHead->release_date.day;
        pFW_Resourec_addr->MasterVersion = pFWHead->version.major;
        pFW_Resourec_addr->SlaveVersion = pFWHead->version.major;
        pFW_Resourec_addr->SmallVersion = pFWHead->version.small;
    }
}

/*******************************************************************************
** Name: fw_WriteProductSn
** Input:void *pSn, max len must be 128
** Return: rt_err_t
** Owner:chad.ma
** Date: 2016.4.26
** Time: 11:48:20
*******************************************************************************/
rt_err_t fw_WriteProductSn(void *pSn, int len)
{
    rt_err_t ret;
    ret = write_device_sn(DEV_SN, (char *)pSn, len);
    if (ret == RT_EOK)
    {
        return len;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
** Name: fw_WriteLanMac
** Input:void *pLanMac, max len must be 128
** Return: rt_err_t
** Owner:chad.ma
** Date: 2016.4.26
** Time: 11:48:20
*******************************************************************************/
rt_err_t fw_WriteLanMac(void *pLanMac, int len)
{
    rt_err_t ret;
    ret = write_device_sn(DEV_LAN_MAC, (char *)pLanMac, len);
    if (ret == RT_EOK)
    {
        return len;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
** Name: fw_GetCustom
** Input: int snTypeId, ; void *pCustom, max len must be 128
** Return: rt_err_t
** Owner:chad.ma
** Date: 2019-6-13
** Time: 16:52:51
*******************************************************************************/
rt_err_t fw_GetCustom(int snTypeId, void *pCustom, int len)
{
    rt_err_t ret;
    ret = get_device_sn(snTypeId, (char *)pCustom, len);
    if (ret > 0)
    {
        return ret;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
** Name: fw_WriteCustom
** Input:int snTypeId, ; void *pCustom, max len must be 128
** Return: rt_err_t
** Owner:chad.ma
** Date: 2019-6-13
** Time: 16:52:51
*******************************************************************************/
rt_err_t fw_WriteCustom(int snTypeId, void *pCustom, int len)
{
    rt_err_t ret;
    ret = write_device_sn(snTypeId, (char *)pCustom, len);
    if (ret == RT_EOK)
    {
        return len;
    }
    else
    {
        return 0;
    }
}

/*******************************************************************************
** Name: fw_WriteBtMac
** Input:void *pBtMac, max len must be 128
** Return: rt_err_t
** Owner:chad.ma
** Date: 2019-6-13
** Time: 11:48:20
*******************************************************************************/
rt_err_t fw_WriteBtMac(void *pBtMac, int len)
{
    rt_err_t ret;
    ret = write_device_sn(DEV_BT_MAC, (char *)pBtMac, len);
    if (ret == RT_EOK)
    {
        return len;
    }
    else
    {
        return 0;
    }
}

rt_bool_t fw_ab_data_verify(fw_ab_data *src, fw_ab_data *dest)
{
    /* Ensure magic is correct. */
    if (rt_memcmp(src->magic, AB_MAGIC, AB_MAGIC_LEN) != 0)
    {
        rt_kprintf("Magic is incorrect.\n");
        return RT_FALSE;
    }

    rt_memcpy(dest, src, sizeof(fw_ab_data));

    /* Ensure we don't attempt to access any fields if the major version
    * is not supported.
    */
    if (dest->version_major > AB_MAJOR_VERSION)
    {
        rt_kprintf("No support for given major version.\n");
        return RT_FALSE;
    }

    /* Fail if CRC32 doesn't match. */
    if (dest->jshash !=
            jshash(0, (char *)dest, sizeof(fw_ab_data) - sizeof(uint32_t)))
    {
        rt_kprintf("CRC does not match.\n");
        return RT_FALSE;
    }

    return RT_TRUE;
}

void fw_ab_data_init(fw_ab_data *data)
{
    rt_memset(data, '\0', sizeof(fw_ab_data));
    rt_memcpy(data->magic, AB_MAGIC, AB_MAGIC_LEN);
    data->version_major = AB_MAJOR_VERSION;
    data->version_minor = AB_MINOR_VERSION;

    data->slots[0].priority = AB_MAX_PRIORITY;
    data->slots[0].tries_remaining = AB_MAX_TRIES_REMAINING;
    data->slots[0].successful_boot = 0;

    data->slots[1].priority = AB_MAX_PRIORITY - 1;
    data->slots[1].tries_remaining = AB_MAX_TRIES_REMAINING;
    data->slots[1].successful_boot = 0;
}

int fw_ab_data_write(const fw_ab_data *data)
{
    fw_ab_data *dest;
    rt_uint8_t *serialized = RT_NULL;
    rt_size_t write_size;
#ifdef RT_USING_SNOR
    struct rt_mtd_nor_device *snor_device = RT_NULL;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device == RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }
#else
    //other device macro define.
#endif

    serialized = (rt_uint8_t *)rt_malloc_align(4096, 64);
    if (!serialized)
        return -RT_ERROR;
    rt_memset(serialized, 0, sizeof(serialized));

#ifdef RT_USING_SNOR
    if (4096 != rt_mtd_nor_read(snor_device, OS_AB_DATA_PART_OFFSET, serialized, 4096))
    {
        rt_kprintf(" %s: rt_mtd_nor_read happen error\n", __func__);
        if (serialized)
            rt_free_align(serialized);
        return -RT_ERROR;
    }
#else
    //other device macro define.
#endif

    dest = (fw_ab_data *)&serialized[0];
    rt_memcpy(dest, data, sizeof(fw_ab_data));
    dest->jshash = jshash(0, (char *)dest, sizeof(fw_ab_data) - 4);

#ifdef RT_USING_SNOR
    rt_mtd_nor_erase_block(snor_device, OS_AB_DATA_PART_OFFSET, snor_device->block_size);
    write_size = rt_mtd_nor_write(snor_device, OS_AB_DATA_PART_OFFSET, (const rt_uint8_t *)serialized, snor_device->block_size);
    if (write_size != snor_device->block_size)
    {
        rt_kprintf("Fw AB data %#d, write Error!", write_size);
        if (serialized)
            rt_free_align(serialized);
        return -RT_ERROR;
    }
#else
    //other device macro define.
#endif

    if (serialized)
        rt_free_align(serialized);

    return RT_EOK;
}

rt_bool_t fw_slot_is_bootable(fw_ab_slot_data *slot)
{
    return slot->priority > 0 &&
           (slot->successful_boot || (slot->tries_remaining >= 0));
}

int fw_ab_data_read(fw_ab_data *data)
{
    char *fw_data;
    uint32_t size = 512;

#ifdef RT_USING_SNOR
    struct rt_mtd_nor_device *snor_device = RT_NULL;
    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device == RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }
#else
    //other device macro define.
#endif

    fw_data = rt_malloc_align(size, 64);
    if (!fw_data)
        return -RT_ERROR;

#ifdef RT_USING_SNOR
    if (size == rt_mtd_nor_read(snor_device, OS_AB_DATA_PART_OFFSET, (uint8_t *)fw_data, size))
#else
    //other device ops
#endif
    {
        if (!fw_ab_data_verify((fw_ab_data *)fw_data, data))
        {
            rt_kprintf("Error validating A/B metadata from disk.\n");
            fw_ab_data_init(data);

            if (fw_data)
                rt_free_align(fw_data);
            return fw_ab_data_write(data);
        }

        if (!fw_slot_is_bootable(&data->slots[0]) && !fw_slot_is_bootable(&data->slots[1]))
        {
            rt_kprintf("Error A and B metadata unbootable.\n");
            fw_ab_data_init(data);

            if (fw_data)
                rt_free_align(fw_data);
            return fw_ab_data_write(data);
        }
    }

    if (fw_data)
        rt_free_align(fw_data);

    return RT_EOK;
}

int fw_slot_reset_flag(uint32_t slot)
{
    fw_ab_data fw_ab;

    rt_kprintf("%s Enter...\n", __func__);
    if (fw_ab_data_read(&fw_ab) != RT_EOK)
        return -RT_ERROR;

    if (fw_ab.slots[1 - slot].successful_boot == 1)
    {
        fw_ab.slots[1 - slot].tries_remaining = AB_MAX_TRIES_REMAINING;
        fw_ab.slots[1 - slot].successful_boot = 0;

        if (fw_ab_data_write(&fw_ab) != RT_EOK)
            return -RT_ERROR;
    }

    rt_kprintf("%s Exit...\n", __func__);
    return RT_EOK;
}

int fw_slot_set_pending(uint32_t slot)
{
    fw_ab_data fw_ab;

    rt_kprintf("%s Enter...\n", __func__);
    if (fw_ab_data_read(&fw_ab) != RT_EOK)
        return -RT_ERROR;

    fw_ab.slots[slot].priority            = AB_MAX_PRIORITY - 1;
    fw_ab.slots[1 - slot].priority        = AB_MAX_PRIORITY;
    fw_ab.slots[1 - slot].tries_remaining = AB_MAX_TRIES_REMAINING;

    if (fw_ab_data_write(&fw_ab) != RT_EOK)
        return -RT_ERROR;

    rt_kprintf("%s Exit...\n", __func__);
    return RT_EOK;
}

int fw_slot_set_active(uint32_t slot)
{
    fw_ab_data fw_ab;

    rt_kprintf("%s Enter...\n", __func__);
    if (fw_ab_data_read(&fw_ab) != RT_EOK)
        return -RT_ERROR;

    if (fw_ab.slots[slot].successful_boot == 0)
    {
        fw_ab.slots[slot].successful_boot = 1;
        fw_ab.slots[slot].tries_remaining = 0;

        if (fw_ab_data_write(&fw_ab) != RT_EOK)
            return -RT_ERROR;
    }

    rt_kprintf("%s Exit...\n", __func__);
    return RT_EOK;
}

int fw_slot_get_current_running(uint32_t *cur_slot)
{
    fw_ab_data fw_ab;
    uint32_t slot_boot_idx;
    rt_int32_t ret = -RT_ERROR;

    ret = fw_ab_data_read(&fw_ab);
    if (ret != RT_EOK)
        return ret;

    memcpy(&g_fw_ab_data, &fw_ab, sizeof(fw_ab));

    if (fw_slot_is_bootable(&fw_ab.slots[0]) && fw_slot_is_bootable(&fw_ab.slots[1]))
    {
        if (fw_ab.slots[1].priority > fw_ab.slots[0].priority)
        {
            slot_boot_idx = 1;
        }
        else
        {
            slot_boot_idx = 0;
        }
    }
    else if (fw_slot_is_bootable(&fw_ab.slots[0]))
    {
        slot_boot_idx = 0;
    }
    else if (fw_slot_is_bootable(&fw_ab.slots[1]))
    {
        slot_boot_idx = 1;
    }
    else
    {
        /* No bootable slots! */
        rt_kprintf("No bootable slots found.\n");
        return -RT_ERROR;
    }

    *cur_slot = slot_boot_idx;
    rt_kprintf("%s : get current slot is %d.\n", __func__, *cur_slot);
    return RT_EOK;
}

int fw_slot_change(uint32_t boot_slot)
{
    fw_ab_data fw_ab;

    /* if slot B boot fail, don't change it.*/
    if (g_fw_ab_data.slots[1].successful_boot == 0
            && g_fw_ab_data.slots[1].tries_remaining == 0)
    {
        rt_kprintf("The slot 1 fw seem to be damaged!\n");
        return 1;
    }

    fw_ab_data_init(&fw_ab);
    fw_ab.slots[1 - boot_slot].priority    = AB_MAX_PRIORITY - 1;
    fw_ab.slots[boot_slot].priority        = AB_MAX_PRIORITY;
    fw_ab.slots[boot_slot].tries_remaining = AB_MAX_TRIES_REMAINING;

    if (fw_ab_data_write(&fw_ab) != RT_EOK)
    {
        rt_kprintf("write fw ab data fail!\n");
        return 1;
    }

    return 0;
}

fw_ab_data *fw_ab_data_get(void)
{
    return &g_fw_ab_data;
}

static int fw_slot_is_zero(fw_ab_slot_data *slot_data)
{
    if (slot_data->priority == 0
            && slot_data->successful_boot == 0
            && slot_data->tries_remaining == 0)
        return 1;

    return 0;
}

void fw_slot_info_dump(void)
{
    int now_slot = -1;
    rt_uint32_t slot_boot_idx = 0;
    const char *slot_name[2] = {"A", "B"};

    fw_ab_data *fw_slot_info;
#ifdef RT_SUPPORT_ROOT_AB
    user_ab_data *user_slot_info;
#endif
    struct rt_mtd_nor_device *snor_device = RT_NULL;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return;
    }

    rt_kprintf("%s Enter...\n", __func__);
    if (fw_slot_get_current_running(&slot_boot_idx) != RT_EOK)
        return;

    fw_slot_info = fw_ab_data_get();

    now_slot = slot_boot_idx;
    rt_kprintf("\n###########\n");
    rt_kprintf("current OS running slot is %d, OS%s\n", now_slot, slot_name[now_slot]);
    rt_kprintf("slot 0: priority:%d, tries_remaining:%d, successful_boot:%d\n",
               fw_slot_info->slots[0].priority,
               fw_slot_info->slots[0].tries_remaining,
               fw_slot_info->slots[0].successful_boot);

    rt_kprintf("slot 1: priority:%d, tries_remaining:%d, successful_boot:%d\n",
               fw_slot_info->slots[1].priority,
               fw_slot_info->slots[1].tries_remaining,
               fw_slot_info->slots[1].successful_boot);

    if (fw_slot_is_zero(&fw_slot_info->slots[0]))
        rt_kprintf("Slot 0 fw seem to be damaged\n");
    else if (fw_slot_is_zero(&fw_slot_info->slots[1]))
        rt_kprintf("Slot 1 fw seem to be damaged\n");
#ifdef RT_SUPPORT_ROOT_AB
    if (user_slot_get_current_running(&slot_boot_idx) != RT_EOK)
        return;
    user_slot_info = user_ab_data_get();
    now_slot = slot_boot_idx;

    rt_kprintf("current USER running slot is %d, root%s\n", now_slot, slot_name[now_slot]);

    rt_kprintf("root slot 0: priority:%d, tries_remaining:%d, successful_boot:%d\n",
               user_slot_info->slots[0].priority,
               user_slot_info->slots[0].tries_remaining,
               user_slot_info->slots[0].successful);

    rt_kprintf("root slot 1: priority:%d, tries_remaining:%d, successful_boot:%d\n",
               user_slot_info->slots[1].priority,
               user_slot_info->slots[1].tries_remaining,
               user_slot_info->slots[1].successful);
#endif

    rt_kprintf("###########\n\n");
}

#ifdef RT_SUPPORT_ROOT_AB

static user_ab_data g_user_ab_data;

void user_ab_data_init(user_ab_data *data)
{
    rt_memset(data, '\0', sizeof(user_ab_data));
    rt_memcpy(data->magic, AB_MAGIC, AB_MAGIC_LEN);
    data->version_major = AB_MAJOR_VERSION;
    data->version_minor = AB_MINOR_VERSION;

    data->slots[0].priority = AB_MAX_PRIORITY;
    data->slots[0].tries_remaining = AB_MAX_TRIES_REMAINING;
    data->slots[0].successful = 1;

    data->slots[1].priority = AB_MAX_PRIORITY - 1;
    data->slots[1].tries_remaining = AB_MAX_TRIES_REMAINING;
    data->slots[1].successful = 0;

    return;
}

rt_bool_t user_ab_data_verify(user_ab_data *src, user_ab_data *dest)
{
    /* Ensure magic is correct. */
    if (rt_memcmp(src->magic, AB_MAGIC, AB_MAGIC_LEN) != 0)
    {
        rt_kprintf("USER Magic is incorrect.\n");
        return RT_FALSE;
    }

    rt_memcpy(dest, src, sizeof(user_ab_data));

    /* Ensure we don't attempt to access any fields if the major version
    * is not supported.
    */
    if (dest->version_major > AB_MAJOR_VERSION)
    {
        rt_kprintf("No support for given major version.\n");
        return RT_FALSE;
    }

    /* Bail if CRC32 doesn't match. */
    if (dest->jshash !=
            jshash(0, (char *)dest, sizeof(user_ab_data) - sizeof(uint32_t)))
    {
        rt_kprintf("CRC does not match.\n");
        return RT_FALSE;
    }

    return RT_TRUE;
}


int user_ab_data_write(const user_ab_data *data)
{
    user_ab_data *dest = RT_NULL;
    rt_uint8_t *serialized = RT_NULL;
    rt_size_t write_size = 0;
#ifdef RT_USING_SNOR
    struct rt_mtd_nor_device *snor_device = RT_NULL;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device == RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }
#else
    //other device macro define.
#endif

    serialized = (rt_uint8_t *)rt_malloc_align(4096, 64);
    if (!serialized)
        return -RT_ERROR;

    rt_memset(serialized, 0, sizeof(serialized));

#ifdef RT_USING_SNOR
    if (4096 != rt_mtd_nor_read(snor_device, USER_AB_DATA_OFFSET, serialized, 4096))
    {
        rt_kprintf(" %s: rt_mtd_nor_read happen error\n", __func__);
        if (serialized)
            rt_free_align(serialized);
        return -RT_ERROR;
    }
#else
    //other device macro define.
#endif

    dest = (user_ab_data *)&serialized[0];
    rt_memcpy(dest, data, sizeof(user_ab_data));
    dest->jshash = jshash(0, (char *)dest, sizeof(user_ab_data) - 4);

#ifdef RT_USING_SNOR
    rt_mtd_nor_erase_block(snor_device, USER_AB_DATA_OFFSET, snor_device->block_size);
    write_size = rt_mtd_nor_write(snor_device, USER_AB_DATA_OFFSET, (const rt_uint8_t *)serialized, snor_device->block_size);
    if (write_size != snor_device->block_size)
    {
        rt_kprintf("User AB data %#d, write Error!\n", write_size);
        if (serialized)
            rt_free_align(serialized);
        return -RT_ERROR;
    }
#else
    //other device macro define.
#endif

    if (serialized)
        rt_free_align(serialized);

    return RT_EOK;

}

rt_bool_t user_slot_is_bootable(user_ab_slot_data *slot)
{
    return slot->priority > 0 &&
           (slot->successful || (slot->tries_remaining > 0));
}

int user_ab_data_read(user_ab_data *data)
{
    char *user_ab_buff = RT_NULL;
    uint32_t size = 512;

#ifdef RT_USING_SNOR
    struct rt_mtd_nor_device *snor_device = RT_NULL;
    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device == RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return -RT_ERROR;
    }
#else
    //other device macro define.
#endif

    user_ab_buff = rt_malloc_align(size, 64);
    if (!user_ab_buff)
        return -RT_ERROR;

#ifdef RT_USING_SNOR
    if (size == rt_mtd_nor_read(snor_device, USER_AB_DATA_OFFSET, (uint8_t *)user_ab_buff, size))
#else
    //other device ops
#endif
    {
        if (!user_ab_data_verify((user_ab_data *)user_ab_buff, data))
        {
            rt_kprintf("Error validating USER A/B metadata from disk.\n");
            user_ab_data_init(data);

            if (user_ab_buff)
                rt_free_align(user_ab_buff);
            return user_ab_data_write(data);
        }

        if (!user_slot_is_bootable(&data->slots[0]) && !user_slot_is_bootable(&data->slots[1]))
        {
            rt_kprintf("Error A and B metadata unbootable.\n");
            user_ab_data_init(data);

            if (user_ab_buff)
                rt_free_align(user_ab_buff);
            return user_ab_data_write(data);
        }
    }

    if (user_ab_buff)
        rt_free_align(user_ab_buff);

    return RT_EOK;
}

int user_slot_reset_flag(uint32_t slot)
{
    user_ab_data user_ab;

    rt_kprintf("%s Enter...\n", __func__);
    if (user_ab_data_read(&user_ab) != RT_EOK)
        return -RT_ERROR;

    if (user_ab.slots[1 - slot].successful == 1)
    {
        user_ab.slots[1 - slot].tries_remaining = AB_MAX_TRIES_REMAINING;
        user_ab.slots[1 - slot].successful = 0;

        if (user_ab_data_write(&user_ab) != RT_EOK)
            return -RT_ERROR;
    }

    rt_kprintf("%s Exit...\n", __func__);
    return RT_EOK;
}

int user_slot_set_pending(uint32_t slot)
{
    user_ab_data user_ab;

    rt_kprintf("%s Enter...\n", __func__);
    if (user_ab_data_read(&user_ab) != RT_EOK)
        return -RT_ERROR;

    user_ab.slots[slot].priority = AB_MAX_PRIORITY - 1;
    user_ab.slots[1 - slot].priority = AB_MAX_PRIORITY;
    user_ab.slots[1 - slot].tries_remaining = AB_MAX_TRIES_REMAINING;

    if (user_ab_data_write(&user_ab) != RT_EOK)
        return -RT_ERROR;

    rt_kprintf("%s Exit...\n", __func__);
    return RT_EOK;
}

int user_slot_set_active(uint32_t slot)
{
    user_ab_data user_ab;

    rt_kprintf("%s Enter...", __func__);
    if (user_ab_data_read(&user_ab) != RT_EOK)
        return -RT_ERROR;

    if (user_ab.slots[slot].successful == 0)
    {
        user_ab.slots[slot].successful = 1;
        user_ab.slots[slot].tries_remaining = 0;

        if (user_ab_data_write(&user_ab) != RT_EOK)
            return -RT_ERROR;
    }

    rt_kprintf("%s Exit...", __func__);
    return RT_EOK;
}

int user_slot_get_current_running(uint32_t *cur_slot)
{
    user_ab_data user_ab;
    uint32_t slot_curr = -1;
    rt_int32_t ret = -RT_ERROR;

    ret = user_ab_data_read(&user_ab);
    if (ret != RT_EOK)
        return ret;

    if (user_slot_is_bootable(&user_ab.slots[0]) && user_slot_is_bootable(&user_ab.slots[1]))
    {
        if (user_ab.slots[1].priority > user_ab.slots[0].priority)
        {
            slot_curr = 1;
        }
        else
        {
            slot_curr = 0;
        }
    }
    else if (user_slot_is_bootable(&user_ab.slots[0]))
    {
        slot_curr = 0;
    }
    else if (user_slot_is_bootable(&user_ab.slots[1]))
    {
        slot_curr = 1;
    }
    else
    {
        /* No bootable slots! */
        rt_kprintf("No using user slots found. Set default slot 0 using\n");
        slot_curr = 0;
    }

    *cur_slot = slot_curr;
    rt_kprintf(" %s : get current slot is %d.\n", __func__, *cur_slot);
    return RT_EOK;
}

user_ab_data *user_ab_data_get(void)
{
    return &g_user_ab_data;
}

#endif

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   local function(init) define
*
*---------------------------------------------------------------------------------------------------------------------
*/
