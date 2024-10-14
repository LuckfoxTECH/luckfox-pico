#include <linux/version.h>
#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/regulator/consumer.h>

#include <asm/io.h>
#include "ssv_debug.h"


extern int ssv_device_init(void);
extern void ssv_device_exit(void);

int ssv_initWlan(void)
{
    int ret=0;
    SSV_LOG_DBG("wlan.c ssv_initWlan\n");
    ret = ssv_device_init();
    return ret;
}

void ssv_exitWlan(void)
{
    ssv_device_exit();
    return;
}

int ssv_generic_wifi_init_module(void)
{
	return ssv_initWlan();
}

void ssv_generic_wifi_exit_module(void)
{
	ssv_exitWlan();
}

EXPORT_SYMBOL(ssv_generic_wifi_init_module);
EXPORT_SYMBOL(ssv_generic_wifi_exit_module);

#ifdef CONFIG_SSV6X5X //CONFIG_SSV6XXX=y
late_initcall(ssv_generic_wifi_init_module);
#else //CONFIG_SSV6XXX=m or =n
module_init(ssv_generic_wifi_init_module);
#endif
module_exit(ssv_generic_wifi_exit_module);
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);

MODULE_LICENSE("Dual BSD/GPL");
