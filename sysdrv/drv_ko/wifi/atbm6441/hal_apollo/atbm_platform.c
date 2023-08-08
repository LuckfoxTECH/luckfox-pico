#ifdef LINUX_OS
#include <linux/version.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/delay.h>
#include <linux/mmc/host.h>
#include <linux/mmc/sdio_func.h>
#include <linux/mmc/card.h>
#include <linux/mmc/sdio.h>
#include <linux/spinlock.h>
//#include <asm/mach-types.h>
#include <net/atbm_mac80211.h>
#include <linux/kthread.h>
#endif

#include "apollo.h"
#include "sbus.h"
#include "apollo_plat.h"

#if (ATBM_WIFI_PLATFORM == 10)
#define PLATFORMINF     "rockchip"
//extern int rockchip_wifi_power(int on);
//extern int rockchip_wifi_set_carddetect(int val);
#endif

#ifndef PLATFORMINF
#define PLATFORMINF		"no platform"
#endif

static const char *platform = PLATFORMINF;

#pragma	message(PLATFORMINF)

static int atbm_platform_power_ctrl(const struct atbm_platform_data *pdata,bool enabled)
{
	int ret = 0; 
	atbm_printk_platform("[%s]:platform set power [%d]\n",platform,enabled);
	return ret;
}
static int atbm_platform_insert_crtl(const struct atbm_platform_data *pdata,bool enabled)
{
	int ret = 0;
	 atbm_printk_platform("[%s]:platform insert ctrl [%d]\n",platform,enabled);
	return ret;
}

int atbm_power_ctrl(const struct atbm_platform_data *pdata,bool enabled)
{
	return atbm_platform_power_ctrl(pdata,enabled);

}
int atbm_insert_crtl(const struct atbm_platform_data *pdata,bool enabled)
{	
	return atbm_platform_insert_crtl(pdata,enabled);
}
#ifdef SDIO_BUS
#ifdef CONFIG_ATBM_APOLLO_USE_GPIO_IRQ
extern  irqreturn_t atbm_gpio_hardirq(int irq, void *dev_id);

extern irqreturn_t atbm_gpio_irq(int irq, void *dev_id);

extern unsigned int atbm_gpio_irq_hera(void *arg);

int atbm_plat_request_gpio_irq(const struct atbm_platform_data *pdata,struct sbus_priv *self,int * atbm_bgf_irq)
{
	int bgf_irq =0;
	int ret =0;

	if (gpio_is_valid(pdata->irq_gpio)) {
		ret = gpio_request(pdata->irq_gpio, "apollo wifi BGF EINT");
		if (ret) {
			atbm_printk_platform("BGF_EINT gpio request fail, ret = %d\n", ret);
		}
	} else {
			atbm_printk_platform("invalid BGF_EINT gpio: %d,self %p\n", pdata->irq_gpio,self);
	}
	bgf_irq = gpio_to_irq(pdata->irq_gpio);
	ret =  enable_irq_wake(bgf_irq);
	if (WARN_ON(ret))
		goto err;
	/* Request the IRQ */
	ret =  request_threaded_irq(bgf_irq, atbm_gpio_hardirq,
					atbm_gpio_irq,
					IRQF_TRIGGER_RISING | IRQF_ONESHOT, /* IRQF_TRIGGER_HIGH | IRQF_ONESHOT,*/
					"atbm_wlan_irq", self);
	if (WARN_ON(ret))
		goto err;

	*atbm_bgf_irq = bgf_irq;

err:

	return ret;
}

void atbm_plat_free_gpio_irq(const struct atbm_platform_data *pdata,struct sbus_priv *self,int atbm_bgf_irq)
{
	disable_irq_wake(atbm_bgf_irq);
	free_irq(atbm_bgf_irq,self);
	gpio_free(pdata->irq_gpio);

	return ;
}

#endif  //CONFIG_ATBM_APOLLO_USE_GPIO_IRQ
#endif

struct atbm_platform_data platform_data = {
	.mmc_id       = "mmc1",

	.clk_ctrl     = NULL,
	.power_ctrl   = atbm_power_ctrl,
	.insert_ctrl  = atbm_insert_crtl,
	.reset_gpio = 0,
};

struct atbm_platform_data *atbm_get_platform_data(void)
{
	return &platform_data;

}


