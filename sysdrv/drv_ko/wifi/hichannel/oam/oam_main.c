/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2020-2020. All rights reserved.
 * Description: oam main implementation.(non-rom).
 * Author: Hisilicon
 * Create: 2020-08-04
 */

/* 头文件包含 */
#include "oam_main.h"
#include "hi_types_base.h"
#include "securec.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

/* 全局变量定义 */
static struct kobject *g_sysfs_hi110x_oam = HI_NULL;
hi_u32 g_level_log = 2;

/* 函数实现 */
hi_u32 oam_get_log_level(hi_void)
{
    return g_level_log;
}

/*****************************************************************************
 功能描述  : OAM模块初始化总入口，包含OAM模块内部所有特性的初始化。
 返 回 值  : 初始化返回值，成功或失败原因
*****************************************************************************/
static ssize_t log_level_show(struct kobject *kobj,
                              struct kobj_attribute *attr, char *buf)
{
    if (buf == HI_NULL) {
        return -HI_FAILURE;
    }

    return snprintf_s(buf, PAGE_SIZE, PAGE_SIZE - 1, "loglevel:             \n"
                     " 0    close log           \n"
                     " 1    ERROR               \n"
                     " 2    WARN                \n"
                     " 3    INFO                \n");
}

STATIC ssize_t store_log_level_set(void *dev, struct kobj_attribute *attr, const char *buf, size_t count)
{
    hi_s32 input;
    if (buf == HI_NULL) {
        return -HI_FAILURE;
    }

    input = oal_atoi(buf);
    if (input < 0 || input > 5) {    /* input must range [0 5] */
        return -HI_FAILURE;
    }

    g_level_log = (hi_u32)input;
    return count;
}

STATIC struct kobj_attribute g_oam_host_log_attr =
__ATTR(loglevel, 0664, (void *)log_level_show, (void *)store_log_level_set);    /* mode 0664 */

static struct attribute *g_oam_log_attrs[] = {
    &g_oam_host_log_attr.attr,
#ifdef _SDIO_TEST
    &oam_sdio_test_attr.attr,
#endif
    NULL
};

static struct attribute_group g_oam_state_group = {
    .attrs = g_oam_log_attrs,
};

hi_s32 oam_user_ctrl_init(void)
{
    hi_s32 ret;
    g_sysfs_hi110x_oam = kobject_create_and_add("hi3881_debug", HI_NULL);
    if (g_sysfs_hi110x_oam == HI_NULL) {
        printk("kobject_create_and_add fail!ret=%d", -ENOMEM);
        return -ENOMEM;
    }

    ret = sysfs_create_group(g_sysfs_hi110x_oam, &g_oam_state_group);
    if (ret) {
        printk("sysfs_create_group fail!ret=%d", ret);
    }
    return ret;
}

static hi_s32 oam_user_ctrl_exit(hi_void)
{
    if (g_sysfs_hi110x_oam) {
        sysfs_remove_group(g_sysfs_hi110x_oam, &g_oam_state_group);
        kobject_put(g_sysfs_hi110x_oam);
    }
    return HI_SUCCESS;
}

hi_s32 oam_main_init(hi_void)
{
    hi_s32 ret = oam_user_ctrl_init();
    if (ret != HI_SUCCESS) {
        return ret;
    }
    oam_warning_log0("oam_main_init SUCCESSFULLY");
    return HI_SUCCESS;
}

/*****************************************************************************
 功能描述  : OAM模块卸载
 返 回 值  : 模块卸载返回值，成功或失败原因
*****************************************************************************/
hi_void oam_main_exit(hi_void)
{
    hi_s32 ret = oam_user_ctrl_exit();
    if (ret != HI_SUCCESS) {
        oam_warning_log0("oam_main_exit:: oam_user_ctrl_exit fail!");
    }
    return;
}

#ifdef __cplusplus
#if __cplusplus
    }
#endif
#endif
