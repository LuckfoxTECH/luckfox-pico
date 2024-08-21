/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    fwanalysis_test.c
  * @version V1.0
  * @brief   fw analysis test
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-08-27     Chad.ma         the first version
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_FWANALYSIS

#include <drivers/mtd_nor.h>
#include "hal_base.h"
#include "../fwmgr/fw_analysis.h"
#include "../fwmgr/rkpart.h"
#include "../drivers/drv_flash_partition.h"

typedef struct rt_mtd_nor_device SNOR_DEV;

static SNOR_DEV *snor_device = RT_NULL;

extern uint32_t firmware_addr1;
extern uint32_t firmware_addr2;


static void fw_info_test_show_usage(void)
{
    rt_kprintf("fw_into_test Usage:\n");
    rt_kprintf("\t 1. fw_into_test: show usage\n");
    rt_kprintf("\t 2. fw_into_test head (1 | 2): show fw (1 | 2) head infomation\n");
    rt_kprintf("\t 3. fw_into_test model (1 | 2): show fw (1 | 2 ) model\n");
    rt_kprintf("\t 4. fw_into_test version (1 | 2): show fw (1 | 2 ) version\n");
    rt_kprintf("\t 5. fw_into_test chip (1 | 2): show fw (1 | 2 ) chip type\n");
    rt_kprintf("\t 6. fw_into_test sn_read (1/2/3...) : read vendor ID 1/2/3... from flash\n");
    rt_kprintf("\t 7. fw_into_test sn_write (1/2/3...) : write vendor ID 1/2/3... to flash\n");
    rt_kprintf("\t 8. fw_into_test custom  : test vendor custom ID (write then read)\n");
    rt_kprintf("\t 9. fw_into_test chg (0x8(OSA) |0x10(OSB) |0x40000000(rootA)|0x80000000(rootB)): change the name of specific type id\n");
    rt_kprintf("\t10. fw_into_test partinfo: list all partition info\n");
}

rt_err_t fw_get_head(void *dev, uint8_t fw_seq)
{
    uint32_t addr = 0, pos = 0;
    uint8_t buf[256];
    uint8_t fwHeadBuf[512];
    uint32_t ret = 0;
    PFIRMWARE_HEADER pFWHead;
    unsigned char magic[8] = {'R', 'E', 'S', 'C', 0, 0, 0, 0};
    SNOR_DEV *snor_dev;

    snor_dev = (SNOR_DEV *)dev;
    RT_ASSERT(snor_dev != NULL);

    if (fw_seq == 1)
        addr = firmware_addr1;
    else if (fw_seq == 2)
        addr = firmware_addr2;
    else
        return -RT_ERROR;

    ret = rt_mtd_nor_read(snor_dev, addr, (uint8_t *)fwHeadBuf, 512);
    RT_ASSERT(ret == 512);

    pFWHead = (PFIRMWARE_HEADER)fwHeadBuf;
    if (0 != rt_memcmp(pFWHead->magic, magic, 4))
    {
        rt_kprintf("\n%s() FW magic ERR addr:0x%x\n", __func__, addr);
        return -RT_ERROR;
    }

    /* Chip */
    rt_memset(buf, 0, sizeof(buf));
    rt_memcpy(buf, "Chip:", 5);
    pos = 5;

    rt_memcpy(buf + pos, pFWHead->chip, rt_strlen((const char *)pFWHead->chip));
    pos += rt_strlen((const char *)pFWHead->chip);
    buf[pos++] = '\r';
    buf[pos++] = '\n';

    /* Modle */
    rt_memcpy(buf + pos, "Model:", rt_strlen("Model:"));
    pos += rt_strlen("Model:");
    rt_memcpy(buf + pos, pFWHead->model, rt_strlen((const char *)pFWHead->model));
    pos += rt_strlen((const char *)pFWHead->model);
    buf[pos++] = '\r';
    buf[pos++] = '\n';

    /* Desc */
    rt_memcpy(buf + pos, "Desc:", rt_strlen("Desc:"));
    pos += rt_strlen("Desc:");
    rt_memcpy(buf + pos, pFWHead->desc, sizeof(pFWHead->desc));
    pos += sizeof(pFWHead->desc);
    buf[pos++] = '\r';
    buf[pos++] = '\n';

    /*fw version*/
    rt_memcpy(buf + pos, "Ver:", rt_strlen("Ver:"));
    pos += rt_strlen("Ver:");
    buf[pos++] = pFWHead->version.major + '0';
    buf[pos++] = '.';
    rt_sprintf((char *)&buf[pos], "%02d", pFWHead->version.minor);
    //buf[pos++] = pFWHead->version.minor + '0';
    pos += 2;
    buf[pos++] = '.';
    rt_sprintf((char *)&buf[pos], "%04d", pFWHead->version.small);
    pos += 4;
    buf[pos++] = '\r';
    buf[pos++] = '\n';

    uint8_t date[8] = {0};
    rt_memset(date, 0, sizeof(date));

    /*fw date*/
    rt_memcpy(buf + pos, "Date:", rt_strlen("Date:"));
    pos += rt_strlen("Date:");
    /* Year*/
    itoa(pFWHead->release_date.year, (char *)date, 10);
    rt_memcpy(buf + pos, date, rt_strlen((const char *)date));
    pos += rt_strlen((const char *)date);
    rt_memset(date, 0, sizeof(date));
    buf[pos++] = '-';
    /* month*/
    itoa(pFWHead->release_date.month, (char *)date, 10);
    rt_memcpy(buf + pos, date, rt_strlen((const char *)date));
    pos += rt_strlen((const char *)date);
    rt_memset(date, 0, sizeof(date));
    buf[pos++] = '-';
    /* day*/
    itoa(pFWHead->release_date.day, (char *)date, 10);
    rt_memcpy(buf + pos, date, rt_strlen((const char *)date));
    pos += rt_strlen((const char *)date);
    rt_memset(date, 0, sizeof(date));
    buf[pos++] = '\r';
    buf[pos++] = '\n';

    rt_kprintf("\n%s", buf);

    /*fw data size*/
    rt_kprintf("\nfw %d: data size : 0x%#x ", fw_seq, pFWHead->data_size);
    rt_kprintf("\nfw %d: pos(Sectors):%u(0x%#04x)", fw_seq, addr >> 9, addr >> 9);
    rt_kprintf("\n");

    return RT_EOK;
}

/*******************************************************************************
** Name: fw_sn_read
** Input:void* dev, dev_sn_type_t vendor_id
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2016.4.26
** Time: 14:36:56
*******************************************************************************/
rt_err_t fw_sn_read(void *dev, dev_sn_type_t vendor_id)
{
    rt_uint8_t buf[512];
    int32_t size;

    rt_kprintf("%s Enter, Vendor_ID = %d\n", __func__, vendor_id);
    if (vendor_id == DEV_SN)
    {
        size = fw_GetProductSn(buf);
        if (size > 0)
            rt_kprintf("\nproduct sn: %s\n", buf);
        else
            rt_kprintf("\nnot find sn! \n");
    }
    else if (vendor_id == DEV_BT_MAC)
    {
        size = fw_GetBtMac(buf);
        if (size > 0)
        {
            char btMac[64] = {0};
            sprintf(btMac, "%02x:%02x:%02x:%02x:%02x:%02x", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
            rt_kprintf(dev, "\nbt mac address: %s\n", btMac);
        }
        else
            rt_kprintf("\nnot find bt mac address! \n");
    }
    else if (vendor_id == DEV_WLAN_MAC)
    {
        size = fw_GetWifiMac(buf);
        if (size > 0)
        {
            char wifiMac[64] = {0};
            sprintf(wifiMac, "%02x:%02x:%02x:%02x:%02x:%02x", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
            rt_kprintf("\nwifi mac address: %s\n", wifiMac);
        }
        else
            rt_kprintf("\nnot find Wifi mac address! \n");
    }
    else
    {
        rt_kprintf("\nNot realize for now... \n");
    }

    rt_kprintf("\n");
    return RT_EOK;
}

/*******************************************************************************
** Name: fw_sn_read
** Input:void* dev, dev_sn_type_t vendor_id
** Return: rk_err_t
** Owner:aaron.sun
** Date: 2016.4.26
** Time: 14:36:56
*******************************************************************************/
rt_err_t fw_sn_write(dev_sn_type_t vendor_id, char *buf, uint32_t len)
{
    rt_uint32_t size = 0;
    rt_kprintf("%s Enter, Vendor_ID = %d\n", __func__, vendor_id);

    if (vendor_id == DEV_SN)
    {
        size = fw_WriteProductSn(buf, len);
        if (size > 0)
            rt_kprintf("\n Write SN: OK! \n");
        else
            rt_kprintf("\n Write SN: Fail! \n");
    }
    else if (vendor_id == DEV_BT_MAC)
    {
        size = fw_WriteBtMac(buf, len);
        if (size > 0)
            rt_kprintf("\n Write BT Mac: OK! \n");
        else
            rt_kprintf("\n Write BT Mac! \n");
    }
    else
    {
        rt_kprintf("\n Not realize for now... \n");
    }

    rt_kprintf("\n");
    return RT_EOK;
}


/*******************************************************************************
** Name: FWShellCustom
** Input:HDC dev, uint8 * pstr
** Return: rk_err_t
** Owner:chad.ma add
** Date: 2019.11.21
** Time: 11:36:56
*******************************************************************************/
rt_err_t fw_custom_test(void *dev, uint8_t *pstr)
{
    rt_uint8_t buf[512];
    int32_t size;
    int i = 0;

    for (i = 0; i < 64; i++)
    {
        buf[i] = i + 1;
    }
    buf[i] = '\0';

    size = fw_WriteCustom(DEV_AUTH, buf, 64);
    if (size > 0)
        rt_kprintf("\n Write custom AUTH: OK! \n");
    else
        rt_kprintf("\n Write custom AUTH: Fail! \n");

    memset(buf, 0, sizeof(buf));

    size = fw_GetCustom(DEV_AUTH, buf, 64);
    if (size > 0)
    {
        rt_kprintf("\n Get custom AUTH len: %d\n", size);
        rt_kprintf("\n Get custom AUTH success!\n");

        //check
        for (i = 0; i < size; i++)
        {
            rt_kprintf("%d ", buf[i]);
        }
        if (i == size)
            rt_kprintf("\n");
    }
    else
        rt_kprintf("\n Get custom AUTH: Fail! \n");

    return RT_EOK;
}

void fw_info_test(int argc, char **argv)
{
    char *cmd;
    char *content;
    rt_uint8_t fw_seq = 0;
    rt_uint32_t vendor_id = 0;
    rt_uint32_t part_total_num = 0;
    struct rt_flash_partition *part_info = RT_NULL;

    if (argc < 2)
        goto out;

    snor_device = (struct rt_mtd_nor_device *)rt_device_find("snor");
    if (snor_device ==  RT_NULL)
    {
        rt_kprintf("Did not find device: snor....\n");
        return;
    }

    //rk_partition_init();
    part_total_num = get_rk_partition(&part_info);
    RT_ASSERT(part_info);

    for (int i = 0; i < part_total_num; i++)
    {
        if (part_info[i].type == PART_FW1)
            firmware_addr1 = part_info[i].offset;
        else if (part_info[i].type == PART_FW2)
            firmware_addr2 = part_info[i].offset;
    }

    cmd = argv[1];
    if (!rt_strcmp(cmd, "head"))
    {
        if (argc < 3)
            goto out;
        fw_seq = atoi(argv[2]);
        if (fw_get_head(snor_device, fw_seq) != RT_EOK)
            goto out;
    }

    if (!rt_strcmp(cmd, "model"))
    {
        if (argc < 3)
            goto out;
        fw_seq = atoi(argv[2]);
        char model_str[32] = {0};
        fw_GetModle(model_str, fw_seq - 1);
        rt_kprintf("\n Fw %d model is %s\n", fw_seq, model_str);
    }

    if (!rt_strcmp(cmd, "version"))
    {
        if (argc < 3)
            goto out;
        fw_seq = atoi(argv[2]);
        char ver_str[32] = {0};
        fw_GetVersionStr(ver_str, fw_seq - 1);
        rt_kprintf("\n Fw %d version is %s\n", fw_seq, ver_str);
    }

    if (!rt_strcmp(cmd, "chip"))
    {
        if (argc < 3)
            goto out;
        fw_seq = atoi(argv[2]);
        char chip_str[32] = {0};
        fw_GetChipType(chip_str, fw_seq - 1);
        rt_kprintf("\n Fw %d chip type is %s\n", fw_seq, chip_str);
    }

    if (!rt_strcmp(cmd, "sn_read"))
    {
        if (argc < 3)
            goto out;

        vendor_id = atoi(argv[2]);
        if (vendor_id > 32)
        {
            rt_kprintf("Vendor ID could not beyond 32\n");
            goto out;
        }
        fw_sn_read(snor_device, vendor_id);
    }
    else if (!rt_strcmp(cmd, "sn_write"))
    {
        if (argc < 4)
            goto out;

        vendor_id = atoi(argv[2]);
        content = argv[3];
        fw_sn_write(vendor_id, content, rt_strlen((const char *)content));
    }

    if (!rt_strcmp(cmd, "custom"))
    {
        fw_custom_test(snor_device, NULL);
    }

    /* just test user part and os part */
    if (!rt_strcmp(cmd, "chg"))
    {
        rt_uint32_t type_id = 0;
        char *new_name = RT_NULL;

        if (argc < 4)
        {
            if (argc < 3)
                rt_kprintf("Miss part type id\n");
            else
                rt_kprintf("Miss part new name\n");
            goto out;
        }
        type_id = atoi(argv[2]);
        new_name = argv[3];
        rt_kprintf("Input type id is %#010x, change to new name :%s\n", type_id, new_name);

        if (type_id >> 30 || type_id >> 31)
        {
            /* user part change name */
            if (!change_part_name(type_id, new_name))
                rt_kprintf("Chang OK\n");
            else
                rt_kprintf("Change Fail\n");
        }
        else if (type_id >> 3 || type_id >> 4)
        {
            /* os part change name */
            if (!change_part_name(type_id, new_name))
                rt_kprintf("Change OK\n");
            else
                rt_kprintf("Change Fail\n");
        }
        else
        {
            rt_kprintf("Not support this type id to change name.\n");
            goto out;
        }
    }

    /* list echo partition info */
    if (!rt_strcmp(cmd, "partinfo"))
    {
        if (argc != 2)
            goto out;

        for (int i = 0; i < part_total_num; i++)
        {
            rt_kprintf("partition[%d] flags=%08x type= %08x off=%08x size=%08x(%#d sector) %s\n",
                       i,
                       part_info[i].mask_flags,
                       part_info[i].type,
                       part_info[i].offset,
                       part_info[i].size,
                       part_info[i].size >> 9,
                       part_info[i].name);
        }
    }

    return;
out:
    fw_info_test_show_usage();
    return;
}


#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(fw_info_test, fw analysis test demo);
#endif

#endif  // #ifdef CONFIG_FWANALYSIS
