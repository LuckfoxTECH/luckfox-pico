/*
 *  Copyright (C) 2019, Fuzhou Rockchip Electronics Co., Ltd.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Fuzhou Rockchip Electronics Co., Ltd. nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <rtthread.h>
#include <string.h>
#include <stdlib.h>
#include <rtthread.h>
#include <dfs_posix.h>


//#include "image/fdcm.h"
//#include "image/image.h"
#ifdef PRJCONF_NET_EN
#include "lwip/inet.h"
#include "lwip/ip_addr.h"
//#include "driver/chip/hal_crypto.h"
//#include "efpg/efpg.h"
#endif /* PRJCONF_NET_EN */

#include "sysinfo.h"
// #include "sysinfo_debug.h"

#define DBG_LEVEL                   DBG_LOG
#define DBG_SECTION_NAME            "SYSINFO"
#include  "rkdebug.h"

#define PRJCONF_MAC_ADDR_SOURCE         SYSINFO_MAC_ADDR_CHIPID


static struct sysinfo g_sysinfo;
#ifdef PRJCONF_SYSINFO_SAVE_TO_FLASH
//static fdcm_handle_t *g_fdcm_hdl;
static int g_hdl = -1;
static int gIsExistInfo = 0;
static int gLoaded = 0;
#endif

#ifdef PRJCONF_NET_EN

static uint8_t m_sysinfo_mac_addr[] = { 0x00, 0x80, 0xE1, 0x29, 0xE8, 0xD1 };

static void sysinfo_gen_mac_random(uint8_t mac_addr[6])
{
#if (PRJCONF_CE_EN && PRJCONF_PRNG_INIT_SEED)
    HAL_PRNG_Generate(mac_addr, 6);
#else
    int i;
    for (i = 0; i < 6; ++i)
    {
        mac_addr[i] = (uint8_t)rand();
    }
#endif
    mac_addr[0] &= 0xFC;
}

static void sysinfo_gen_mac_by_chipid(uint8_t mac_addr[6])
{
    /*
        int i;
        uint8_t chipid[16];

        efpg_read(EFPG_FIELD_CHIPID, chipid);

        for (i = 0; i < 2; ++i) {
            mac_addr[i] = chipid[i] ^ chipid[i + 6] ^ chipid[i + 12];
        }
        for (i = 2; i < 6; ++i) {
            mac_addr[i] = chipid[i] ^ chipid[i + 6] ^ chipid[i + 10];
        }
        mac_addr[0] &= 0xFC;
    */
}

static void sysinfo_init_mac_addr(void)
{
    int i;

    LOGD("mac addr source: %d", PRJCONF_MAC_ADDR_SOURCE);

    switch (PRJCONF_MAC_ADDR_SOURCE)
    {
    case SYSINFO_MAC_ADDR_CODE:
        memcpy(g_sysinfo.mac_addr, m_sysinfo_mac_addr, SYSINFO_MAC_ADDR_LEN);
        return;
    case SYSINFO_MAC_ADDR_EFUSE:
        /*
        if (efpg_read(EFPG_FIELD_MAC, g_sysinfo.mac_addr) != 0) {
            LOGW("read mac addr from eFuse fail\n");
            goto random_mac_addr;
        }
        return;
        */
        break;
    case SYSINFO_MAC_ADDR_CHIPID:
        sysinfo_gen_mac_by_chipid(g_sysinfo.mac_addr);
        for (i = 0; i < sizeof(g_sysinfo.mac_addr); ++i)
        {
            if (g_sysinfo.mac_addr[i] != 0)
                return;
        }
        goto random_mac_addr;
#ifdef PRJCONF_SYSINFO_SAVE_TO_FLASH
    case SYSINFO_MAC_ADDR_FLASH:
    {
#if 0
        struct sysinfo *info = malloc(SYSINFO_SIZE);
        if (info == NULL)
        {
            LOGE("malloc fail\n");
            goto random_mac_addr;
        }
        if (fdcm_read(g_fdcm_hdl, info, SYSINFO_SIZE) != SYSINFO_SIZE)
        {
            LOGW("read mac addr from flash fail\n");
            free(info);
            goto random_mac_addr;
        }
#else
        goto random_mac_addr;
#endif

#if 0
        memcpy(g_sysinfo.mac_addr, info->mac_addr, SYSINFO_MAC_ADDR_LEN);
        free(info);
        return;
#endif
    }
#endif
    default:
        LOGE("invalid mac addr source\n");
        goto random_mac_addr;
    }

random_mac_addr:
    LOGD("random mac addr\n");
    sysinfo_gen_mac_random(g_sysinfo.mac_addr);
}
#endif /* PRJCONF_NET_EN */

static void sysinfo_init_value(void)
{
    LOGD("%s ()", __func__);
#ifdef PRJCONF_SYSINFO_SAVE_TO_FLASH
    if (sysinfo_load() != 0)
    {
        sysinfo_default();
        return;
    }
#ifdef PRJCONF_NET_EN
    if (PRJCONF_MAC_ADDR_SOURCE != SYSINFO_MAC_ADDR_FLASH)
    {
        sysinfo_init_mac_addr();
    }
#endif
#else
    sysinfo_default();
#endif
}

/**
 * @brief Initialize the sysinfo module
 * @return 0 on success, -1 on failure
 */
int sysinfo_init(void)
{
    LOGD("%s ()", __func__);
#ifdef PRJCONF_SYSINFO_SAVE_TO_FLASH
    g_hdl = open("/sysinfo.conf", O_RDWR);
    if (g_hdl >= 0)
    {
        gIsExistInfo = 1;
    }
    else
    {
        g_hdl = open("/sysinfo.conf", O_RDWR | O_CREAT);
        if (g_hdl < 0)
        {
            LOGD(" Open sysinfo.conf fail");
            return -1;
        }
    }
#endif /* PRJCONF_SYSINFO_SAVE_TO_FLASH */
    sysinfo_init_value();
    return 0;
}

/**
 * @brief DeInitialize the sysinfo module
 * @return None
 */
void sysinfo_deinit(void)
{
#ifdef PRJCONF_SYSINFO_SAVE_TO_FLASH
    // fdcm_close(g_fdcm_hdl);
    if (g_hdl >= 0)
        close(g_hdl);
#endif
}

/**
 * @brief Set default value to sysinfo
 * @return 0 on success, -1 on failure
 */
int sysinfo_default(void)
{
    LOGD("%s ()", __func__);
    memset(&g_sysinfo, 0, SYSINFO_SIZE);

#ifdef PRJCONF_NET_EN
    /* MAC address */
    sysinfo_init_mac_addr();

    /* wlan mode */
    g_sysinfo.wlan_mode = WLAN_MODE_STA;

    /* netif STA */
    g_sysinfo.sta_use_dhcp = 1;

    /* netif AP */
    IP4_ADDR(&g_sysinfo.netif_ap_param.ip_addr, 192, 168, 51, 1);
    IP4_ADDR(&g_sysinfo.netif_ap_param.net_mask, 255, 255, 255, 0);
    IP4_ADDR(&g_sysinfo.netif_ap_param.gateway, 192, 168, 51, 1);
#endif

    LOGD("set default value");
#ifdef PRJCONF_SYSINFO_SAVE_TO_FLASH
    sysinfo_save();
#endif
    return 0;
}

#ifdef PRJCONF_SYSINFO_SAVE_TO_FLASH
static void dumpSysInfo(struct sysinfo *info)
{
#ifdef PRJCONF_NET_EN
    int i = 0, num = 0, idx = 0;
    num = info->wifi_info_num;
    idx = info->wifi_info_index;

    LOGD("## Total hold %d ssid ## ", num);
    LOGD("## Current use %d ssid ##", idx);
    for (i = 0; i < num; i++)
    {
        LOGD("##[%d] : ssid :%s  psk : %s ##", i, info->wlan_sta_param[i].ssid, info->wlan_sta_param[i].psk);
    }
#endif
}
/**
 * @brief Save sysinfo to flash
 * @return 0 on success, -1 on failure
 */
int sysinfo_save(void)
{
    LOGD("%s ()", __func__);

    if (g_hdl < 0)
    {
        LOGE(" uninitialized, hdl %d", g_hdl);
        // return -1;
        g_hdl = open("/sysinfo.conf", O_RDWR);
        if (g_hdl < 0)
            return -1;
    }

    if (g_hdl >= 0)
    {
        close(g_hdl);
        g_hdl = open("/sysinfo.conf", O_WRONLY);
        if (g_hdl < 0)
            return -1;
    }

    lseek(g_hdl, 0, SEEK_SET);
    dumpSysInfo(&g_sysinfo);

    if (write(g_hdl, &g_sysinfo, SYSINFO_SIZE) != SYSINFO_SIZE)
    {
        LOGE(" Write to sysinfo.conf failed");
        return -1;
    }

    LOGD("%s():save sysinfo to flash", __func__);
    if (g_hdl >= 0)
        close(g_hdl);
    g_hdl = -1;
    if (!gIsExistInfo)
        gIsExistInfo = 1;

    LOGD("save OK");
    return 0;
}

/**
 * @brief Load sysinfo from flash
 * @return 0 on success, -1 on failure
 */
int sysinfo_load(void)
{
    LOGD("%s ()", __func__);

    if (g_hdl >= 0 && gIsExistInfo)
    {
        lseek(g_hdl, 0, SEEK_SET);
        int ret = read(g_hdl, &g_sysinfo, SYSINFO_SIZE);
        if (ret != SYSINFO_SIZE)
        {
            LOGE(" Read sysinfo.conf failed : read = %d", ret);
            close(g_hdl);
            return -1;
        }
    }
    else
    {
        LOGD("load sysinfo from flash Fail");
        return -1;
    }

    gLoaded = 1;
    LOGD("load sysinfo from flash OK");
    close(g_hdl);

    return 0;
}
#endif /* PRJCONF_SYSINFO_SAVE_TO_FLASH */

/**
 * @brief Get the pointer of the sysinfo
 * @return Pointer to the sysinfo, NULL on failure
 */
struct sysinfo *sysinfo_get(void)
{
#ifdef PRJCONF_SYSINFO_SAVE_TO_FLASH
    LOGD("%s () load :%d isExistInfo:%d", __func__, gLoaded, gIsExistInfo);
    if (!gLoaded)
    {
        if (g_hdl < 0)
        {
            LOGD("%s() : 000 ", __func__);
            g_hdl = open("/sysinfo.conf", O_RDONLY);
            if (g_hdl < 0)
            {
                LOGD("%s() : 111 ", __func__);
                return RT_NULL;
            }
        }

        if (sysinfo_load() != 0)
        {
            LOGE(" load sysinfo fail, hdl = %d", g_hdl);
            return RT_NULL;
        }
    }
    else
        return &g_sysinfo;
#endif
    return &g_sysinfo;
}
