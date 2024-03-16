#include <linux/module.h>
#include <linux/inetdevice.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/platform_device.h>
#include "lpm.h"
#include "rfkill.h"

#define DRV_CONFIG_FW_NAME    "fw.bin"
#define DRV_DESCRIPTION       "AIC BLUETOOTH"
#define DRV_COPYRIGHT         "Copyright(c) 2015-2020 AICSemi"
#define DRV_AUTHOR            "AICSemi"
#define DRV_VERS_MOD          "1.0"

static struct platform_device *aicbt_pdev;

static struct platform_driver aicbt_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "aic_bt",
	},
	//.probe = aicbt_probe,
	//.remove = aicbt_remove,
};

static int __init aic_bluetooth_mod_init(void)
{
	int ret;
	printk("%s\n", __func__);
	ret = platform_driver_register(&aicbt_driver);
	if (ret) {
		pr_err("register platform driver failed: %d\n", ret);
		return ret;
	}

	aicbt_pdev = platform_device_alloc("aic-bt", -1);
	ret = platform_device_add(aicbt_pdev);
	if (ret) {
		pr_err("register platform device failed: %d\n", ret);
		goto err0;
	}

	ret = rfkill_bluetooth_init(aicbt_pdev);
	if (ret) {
		pr_err("rfkill init fail\n");
		goto err1;
	}
#if defined(ANDROID_PLATFORM) && !defined(CONFIG_PLATFORM_ROCKCHIP) && !defined(CONFIG_PLATFORM_ROCKCHIP2)
	ret = bluesleep_init(aicbt_pdev);
	if (ret) {
		pr_err("bluesleep init fail\n");
		goto err2;
	}
#endif

	return 0;

#if defined(ANDROID_PLATFORM) && !defined(CONFIG_PLATFORM_ROCKCHIP) && !defined(CONFIG_PLATFORM_ROCKCHIP2)
err2:
#endif
	rfkill_bluetooth_remove(aicbt_pdev);
err1:
	platform_device_del(aicbt_pdev);
err0:
	platform_driver_unregister(&aicbt_driver);
	return ret;
}

static void __exit aic_bluetooth_mod_exit(void)
{
	printk("%s\n", __func__);
#if defined(ANDROID_PLATFORM) && !defined(CONFIG_PLATFORM_ROCKCHIP) && !defined(CONFIG_PLATFORM_ROCKCHIP2)
	bluesleep_exit(aicbt_pdev);
#endif
	rfkill_bluetooth_remove(aicbt_pdev);
	platform_device_del(aicbt_pdev);
	platform_driver_unregister(&aicbt_driver);
}

module_init(aic_bluetooth_mod_init);
module_exit(aic_bluetooth_mod_exit);

MODULE_DESCRIPTION(DRV_DESCRIPTION);
MODULE_VERSION(DRV_VERS_MOD);
MODULE_AUTHOR(DRV_COPYRIGHT " " DRV_AUTHOR);
MODULE_LICENSE("GPL");
