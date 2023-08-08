/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: HMAC module HCC layer adaptation.
 * Author: kongcheng
 * Create: 2020-08-04
 */

/* 头文件包含 */
#include <linux/module.h>
#include "hi_types_base.h"
#include "oam_ext_if.h"
#include "wal_net.h"
#include "hcc_adapt.h"
#include "wal_netlink.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* WLAN_GPIO_INT */
int hi_rk_irq_gpio = -1;
module_param(hi_rk_irq_gpio, int, S_IRUGO);

/* 函数定义 */
/* insmod hichannel.ko 入口函数 */
static hi_s32 __init wlan_init(void)
{
    hi_s32 ret;
    oam_warning_log0("hichannel 2021-06-11 18:00:00");

    ret = oal_netlink_init();
    if (ret != HI_SUCCESS) {
        oam_error_log1("wlan_init:: oal_netlink_init FAILED[%d]", ret);
        goto fail;
    }

    /* 维测初始化 */
    ret = oam_main_init();
    if (ret != HI_SUCCESS) {
        oam_error_log1("wlan_init:: oam_main_init FAILED[%d]", ret);
        goto oam_main_init_fail;
    }

    ret = hcc_adapt_init();
    if (ret != HI_SUCCESS) {
        goto hcc_host_init_fail;
    }

    ret = netdev_register();
    if (ret != HI_SUCCESS) {
        oam_error_log0("wlan_init:: netdev_register failed");
        return ret;
    }

    oam_warning_log0("wlan drv insmod SUCCESSFULLY");
    return HI_SUCCESS;

hcc_host_init_fail:
    oam_main_exit();
oam_main_init_fail:
    oal_netlink_deinit();
fail:
    return -HI_FAILURE;
}

/* rmmdo hichannel.ko 入口函数 */
static void __exit wlan_deinit(void)
{
    netdev_unregister();
    hcc_adapt_exit();
    oam_main_exit();
    oal_netlink_deinit();
    return;
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hisilicon Wlan Driver");
MODULE_AUTHOR("Hisilicon Wifi Team");
MODULE_VERSION("V1.0.0_000.20200902");

module_init(wlan_init);
module_exit(wlan_deinit);
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
