/**
 ******************************************************************************
 *
 * @file rwnx_platform.c
 *
 * Copyright (C) RivieraWaves 2012-2019
 *
 ******************************************************************************
 */
#include <linux/module.h>
#include <linux/firmware.h>
#include <linux/delay.h>

#include "rwnx_platform.h"
#include "reg_access.h"
#include "hal_desc.h"
#include "rwnx_main.h"
#include "virt_net.h"

//#include "rwnx_pci.h"
#ifndef CONFIG_RWNX_FHOST
//#include "ipc_host.h"
#endif /* !CONFIG_RWNX_FHOST */
//#include "rwnx_msg_tx.h"

#ifdef AICWF_SDIO_SUPPORT
#include "aicwf_sdio.h"
#endif

#ifdef AICWF_USB_SUPPORT
#include "aicwf_usb.h"
#endif

// Parser state
#define INIT      0
#define CMD       1
#define PRINT     2
#define GET_VALUE 3

struct rwnx_plat *g_rwnx_plat = NULL;
struct rwnx_vif  g_rwnx_vif   = {{0}};

#ifdef CONFIG_VNET_MODE
extern aicwf_custom_msg_vnet g_custom_msg_vnet;
int rwnx_platform_set_mac_addr(void)
{
    struct custom_msg_set_mac_req mac;
    mac.hdr.cmd_id = CUST_CMD_SET_MAC_ADDR_REQ;
    memcpy(mac.mac_addr, g_custom_msg_vnet.macaddr_cfm.mac_addr, 6);
    printk("mac addr: %02X %02X\r\n",
        g_custom_msg_vnet.macaddr_cfm.mac_addr[0], g_custom_msg_vnet.macaddr_cfm.mac_addr[5]);
    rwnx_tx_msg((u8 *)&mac, sizeof(mac));
    return 0;
}

void rwnx_platform_custom_msg_vnet_init(void)
{
    g_custom_msg_vnet.aic_mode_status = AIC_MODE_IDLE;
    g_custom_msg_vnet.wlan_status     = WLAN_DISCONNECT;
    g_custom_msg_vnet.ap_status       = AIC_AP_CLOSE;
    g_custom_msg_vnet.comp_sign_get_mac_ready  = false;
    g_custom_msg_vnet.comp_sign_get_wlan_ready = false;
    g_custom_msg_vnet.scan_wifi_cfm_ptr = NULL;
}

void rwnx_platform_get_mac_ready(void)
{
    complete(&g_custom_msg_vnet.platform_get_mac_done);
}
int rwnx_platform_get_mac_addr(void)
{
    struct custom_msg_hdr req;
    req.cmd_id = CUST_CMD_GET_MAC_ADDR_REQ;

    // To avoid complete platform_get_mac_done before init_completion
    init_completion(&g_custom_msg_vnet.platform_get_mac_done);
    rwnx_tx_msg( (u8 *)&req, sizeof(req));

    //init_completion(&platform_get_mac_done);
    if ((wait_for_completion_timeout(&g_custom_msg_vnet.platform_get_mac_done,
                                      msecs_to_jiffies(PLATFORM_PREPARE_TIMEOUT)) == 0)) {
        printk("error: platform get mac address timeout\n");
        return -1;
    }
    return 0;
}

void rwnx_platform_get_wlan_ready(void)
{
    complete(&g_custom_msg_vnet.platform_get_wlan_done);
}
int rwnx_platform_get_wlan_status(void)
{
    struct custom_msg_hdr req = {0};
    req.cmd_id = CUST_CMD_GET_WLAN_STATUS_REQ;

    // To avoid complete platform_get_conn_st_done before init_completion
    init_completion(&g_custom_msg_vnet.platform_get_wlan_done);
    rwnx_tx_msg( (u8 *)&req, sizeof(req));

    //init_completion(&platform_get_mac_done);
    if ((wait_for_completion_timeout(&g_custom_msg_vnet.platform_get_wlan_done,
                                      msecs_to_jiffies(PLATFORM_PREPARE_TIMEOUT)) == 0)) {
        printk("error: platform get wlan status timeout\n");
        return -1;
    }
    return 0;
}
#endif

/**
 * rwnx_platform_reset() - Reset the platform
 *
 * @rwnx_plat: platform data
 */
static int rwnx_platform_reset(struct rwnx_plat *rwnx_plat)
{
    u32 regval;

#if defined(AICWF_USB_SUPPORT) || defined(AICWF_SDIO_SUPPORT)
    return 0;
#endif

    /* the doc states that SOFT implies FPGA_B_RESET
     * adding FPGA_B_RESET is clearer */
    RWNX_REG_WRITE(SOFT_RESET | FPGA_B_RESET, rwnx_plat,
                   RWNX_ADDR_SYSTEM, SYSCTRL_MISC_CNTL_ADDR);
    msleep(100);

    regval = RWNX_REG_READ(rwnx_plat, RWNX_ADDR_SYSTEM, SYSCTRL_MISC_CNTL_ADDR);

    if (regval & SOFT_RESET) {
        dev_err(rwnx_platform_get_dev(rwnx_plat), "reset: failed\n");
        return -EIO;
    }

    RWNX_REG_WRITE(regval & ~FPGA_B_RESET, rwnx_plat,
                   RWNX_ADDR_SYSTEM, SYSCTRL_MISC_CNTL_ADDR);
    msleep(100);
    return 0;
}

/**
 * rwmx_platform_save_config() - Save hardware config before reload
 *
 * @rwnx_plat: Pointer to platform data
 *
 * Return configuration registers values.
 */
static void* rwnx_term_save_config(struct rwnx_plat *rwnx_plat)
{
    const u32 *reg_list;
    u32 *reg_value, *res;
    int i, size = 0;

    if (rwnx_plat->get_config_reg) {
        size = rwnx_plat->get_config_reg(rwnx_plat, &reg_list);
    }

    if (size <= 0)
        return NULL;

    res = kmalloc(sizeof(u32) * size, GFP_KERNEL);
    if (!res)
        return NULL;

    reg_value = res;
    for (i = 0; i < size; i++) {
        *reg_value++ = RWNX_REG_READ(rwnx_plat, RWNX_ADDR_SYSTEM, *reg_list++);
    }

    return res;
}

/**
 * rwnx_platform_on() - Start the platform
 *
 * @rwnx_hw: Main driver data
 * @config: Config to restore (NULL if nothing to restore)
 *
 * It starts the platform :
 * - load fw and ucodes
 * - initialize IPC
 * - boot the fw
 * - enable link communication/IRQ
 *
 * Called by 802.11 part
 */
int rwnx_platform_on(struct rwnx_hw *rwnx_hw, void *config)
{
    #if 0
    u8 *shared_ram;
    #endif
    #ifndef CONFIG_ROM_PATCH_EN
    #ifdef CONFIG_DOWNLOAD_FW
    int ret;
    #endif
    #endif
    struct rwnx_plat *rwnx_plat = rwnx_hw->plat;

    RWNX_DBG(RWNX_FN_ENTRY_STR);

    if (rwnx_plat->enabled)
        return 0;

    rwnx_plat->enabled = true;

    return 0;
}

/**
 * rwnx_platform_off() - Stop the platform
 *
 * @rwnx_hw: Main driver data
 * @config: Updated with pointer to config, to be able to restore it with
 * rwnx_platform_on(). It's up to the caller to free the config. Set to NULL
 * if configuration is not needed.
 *
 * Called by 802.11 part
 */
void rwnx_platform_off(struct rwnx_hw *rwnx_hw, void **config)
{
    #if defined(AICWF_USB_SUPPORT) || defined(AICWF_SDIO_SUPPORT)
    rwnx_hw->plat->enabled = false;
    return ;
    #endif

    if (!rwnx_hw->plat->enabled) {
        if (config)
            *config = NULL;
        return;
    }

    if (config)
        *config = rwnx_term_save_config(rwnx_hw->plat);

    rwnx_hw->plat->disable(rwnx_hw);

    tasklet_kill(&rwnx_hw->task);

    rwnx_platform_reset(rwnx_hw->plat);

    rwnx_hw->plat->enabled = false;
}

#ifdef CONFIG_APP_FASYNC
struct rwnx_aic_chardev chardev;
int rwnx_aic_cdev_open(struct inode *inode, struct file *filp)
{
    filp->private_data = &chardev;
    printk("rwnx_aic_cdev_open\r\n");
    return 0;
}

ssize_t rwnx_aic_cdev_read(struct file* filp, char __user* buf, size_t cnt, loff_t* f_pos)
{
    struct rwnx_aic_chardev *pchardev = filp->private_data;
    printk("rwnx_aic_cdev_read\r\n");
    //rk fix if(copy_to_user(buf, pchardev->mem, cnt) < 0) {
    if(copy_to_user(buf, pchardev->mem, cnt)) {
        printk("copy kernel data fail\n");
    }
    return 0;
}

int rwnx_aic_cdev_fasync(int fd, struct file *filp, int on)
{
    struct rwnx_aic_chardev *pchardev = filp->private_data;
    fasync_helper(fd, filp, on, &pchardev->async_queue);
    printk("rwnx_aic_cdev_fasync\r\n");
    return 0;
}

int rwnx_aic_cdev_release(struct inode* inode, struct file* filp)
{
    rwnx_aic_cdev_fasync(-1, filp, 0);
    printk("rwnx_aic_cdev_release\r\n");
    return 0;
}

static struct file_operations aic_cdev_driver_fops = {
    .owner = THIS_MODULE,
    .open = rwnx_aic_cdev_open,
    .read = rwnx_aic_cdev_read,
    .fasync = rwnx_aic_cdev_fasync,
    .release = rwnx_aic_cdev_release,
};

int rwnx_aic_cdev_driver_init(void)
{
    int ret = 0;
    struct device *devices;
    if (alloc_chrdev_region(&chardev.dev, 0, 1, "aic_cdev_ioctl")) {
        printk("%s: alloc_chrdev_region failure\n", __FUNCTION__);
        goto exit;
    }
    chardev.major = MAJOR(chardev.dev);

    // add cdev
    chardev.c_cdev = kzalloc(sizeof(struct cdev), GFP_KERNEL);
    if(IS_ERR(chardev.c_cdev)) {
        printk("%s: kmalloc failure\n", __FUNCTION__);
        ret = PTR_ERR(chardev.c_cdev);
        goto free_chrdev_region;
    }
    cdev_init(chardev.c_cdev, &aic_cdev_driver_fops);
    ret = cdev_add(chardev.c_cdev, chardev.dev, 1);
    if(ret < 0) {
        printk("%s: cdev_add failure\n", __FUNCTION__);
        goto free_chrdev_region;
    }

    // create device_class
    chardev.cdev_class = class_create(THIS_MODULE, "aic_cdev_class");
    if(IS_ERR(chardev.cdev_class)) {
        printk("%s: class_create failure\n", __FUNCTION__);
        ret = PTR_ERR(chardev.cdev_class);
        goto free_cdev;
    }

    // create device
    devices = device_create(chardev.cdev_class, NULL, MKDEV(chardev.major,0), NULL, "aic_cdev");
    if(IS_ERR(devices)) {
        printk("%s: device_create failure\n", __FUNCTION__);
        ret = PTR_ERR(devices);
        goto free_device_class;
    }

    printk("Create device: /dev/aic_cdev_class\n");
    return 0;

free_device_class:
    class_destroy(chardev.cdev_class);
free_cdev:
    cdev_del(chardev.c_cdev);
free_chrdev_region:
    unregister_chrdev_region(chardev.dev, 1);
exit:
    return ret;
}

void rwnx_aic_cdev_driver_deinit(void)
{
    printk("%s\n",__FUNCTION__);

    device_destroy(chardev.cdev_class, MKDEV(chardev.major,0));
    class_destroy(chardev.cdev_class);

    cdev_del(chardev.c_cdev);
    unregister_chrdev_region(chardev.dev, 1);
}
#endif

/**
 * rwnx_platform_init() - Initialize the platform
 *
 * @rwnx_plat: platform data (already updated by platform driver)
 * @platform_data: Pointer to store the main driver data pointer (aka rwnx_hw)
 *                That will be set as driver data for the platform driver
 * Return: 0 on success, < 0 otherwise
 *
 * Called by the platform driver after it has been probed
 */
int rwnx_platform_init(struct rwnx_plat *rwnx_plat, void **platform_data)
{
    RWNX_DBG(RWNX_FN_ENTRY_STR);

    rwnx_plat->enabled = true;
    g_rwnx_plat = rwnx_plat;

    #ifdef CONFIG_VNET_MODE
    rwnx_platform_custom_msg_vnet_init();
    if(rwnx_platform_get_wlan_status() != 0)
        return -1;
    #ifndef CONFIG_FAST_INSMOD
    if(rwnx_platform_get_mac_addr() != 0)
        return -1;
    #endif

    #if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 7, 0)
    memcpy(g_rwnx_vif.wdev.address, g_custom_msg_vnet.macaddr_cfm.mac_addr, 6);
    #else
    memcpy(g_rwnx_vif.address, g_custom_msg_vnet.macaddr_cfm.mac_addr, 6);
    #endif
    if(virt_net_init(&g_rwnx_vif)) {
        printk("vnet_dev init fail.\n");
        return -1;
    }

    #ifdef CONFIG_APP_FASYNC
    rwnx_aic_cdev_driver_init();
    #endif

    #elif defined(CONFIG_RAWDATA_MODE)
    rwnx_nlaic_init(rwnx_plat, platform_data);
    #endif

    return 0;
}

/**
 * rwnx_platform_deinit() - Deinitialize the platform
 *
 * @rwnx_hw: main driver data
 *
 * Called by the platform driver after it is removed
 */
void rwnx_platform_deinit(void)
{
    RWNX_DBG(RWNX_FN_ENTRY_STR);
    #ifdef CONFIG_VNET_MODE
    virt_net_exit();

    #ifdef CONFIG_APP_FASYNC
    rwnx_aic_cdev_driver_deinit();
    #endif

    #elif defined(CONFIG_RAWDATA_MODE)
    rwnx_nlaic_deinit();
    #endif
}

/**
 * rwnx_platform_register_drv() - Register all possible platform drivers
 */
int rwnx_platform_register_drv(void)
{
    return 0;//rwnx_pci_register_drv();
}


/**
 * rwnx_platform_unregister_drv() - Unegister all platform drivers
 */
void rwnx_platform_unregister_drv(void)
{
    //return rwnx_pci_unregister_drv();
}

struct device *rwnx_platform_get_dev(struct rwnx_plat *rwnx_plat)
{
#ifdef AICWF_SDIO_SUPPORT
    return rwnx_plat->sdiodev->dev;
#endif
#ifdef AICWF_USB_SUPPORT
    return rwnx_plat->usbdev->dev;
#endif
    return &(rwnx_plat->pci_dev->dev);
}

#ifndef CONFIG_RWNX_SDM
MODULE_FIRMWARE(RWNX_AGC_FW_NAME);
MODULE_FIRMWARE(RWNX_FCU_FW_NAME);
MODULE_FIRMWARE(RWNX_LDPC_RAM_NAME);
#endif
MODULE_FIRMWARE(RWNX_MAC_FW_NAME);
#ifndef CONFIG_RWNX_TL4
MODULE_FIRMWARE(RWNX_MAC_FW_NAME2);
#endif

