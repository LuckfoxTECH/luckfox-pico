/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/proc_fs.h> /* for proc_mkdir, create */
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>	/* for copy_from_user */
#include <linux/errno.h>
#include <linux/fs.h>
//#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/ctype.h> /* for isalpha & isdigit */

#include "ssv_cfg.h"
#include "ssvdevice.h"
#include "rftool/ssv_rftool.h"
#include "ssv_debug.h"

#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 1, 0))
	#define get_ds() KERNEL_DS
#endif
#endif


MODULE_AUTHOR("iComm-semi, Ltd");
MODULE_DESCRIPTION("Shared library for SSV wireless LAN cards.");
MODULE_LICENSE("Dual BSD/GPL");

char *stacfgpath = NULL;
EXPORT_SYMBOL(stacfgpath);
module_param(stacfgpath, charp, 0000);
MODULE_PARM_DESC(stacfgpath, "Get path of sta cfg");

char *cfgfirmwarepath = NULL;
EXPORT_SYMBOL(cfgfirmwarepath);
module_param(cfgfirmwarepath, charp, 0000);
MODULE_PARM_DESC(cfgfirmwarepath, "Get firmware path");

char* ssv_initmac = NULL;
EXPORT_SYMBOL(ssv_initmac);
module_param(ssv_initmac, charp, 0644);
MODULE_PARM_DESC(ssv_initmac, "Wi-Fi MAC address");

/*****************/
/* For USB only. */
/*****************/
#ifndef CONFIG_USB_TX_MULTI_URB
int ssv_usb_rx_nr_recvbuff = 2;
#else
int ssv_usb_rx_nr_recvbuff = 5;
#endif
EXPORT_SYMBOL(ssv_usb_rx_nr_recvbuff);
module_param(ssv_usb_rx_nr_recvbuff, int, 0644);
MODULE_PARM_DESC(ssv_usb_rx_nr_recvbuff, "USB RX buffer 1 ~ SSV_USB_MAX_NR_RECVBUFF");

/*****************/
/* For USB only. */
/*****************/
int ssv_rx_use_wq = 0;
EXPORT_SYMBOL(ssv_rx_use_wq);
module_param(ssv_rx_use_wq, int, 0444);
MODULE_PARM_DESC(ssv_rx_use_wq, "USB RX uses workqueue instead of tasklet");

struct ssv6xxx_cfg ssv_cfg;
struct ssv_rftool_cfg rftool_cfg;
EXPORT_SYMBOL(ssv_cfg);

#if 0
static int __string2s32(u8 *val_str, void *val)
{
    char *endp;
    int base=10;
    if (val_str[0]=='0' && ((val_str[1]=='x')||(val_str[1]=='X')))
        base = 16;
    *(int *)val = simple_strtoul(val_str, &endp, base);
    return 0;
}
#endif

static int __string2bool(u8 *u8str, void *val, u32 arg)
{
    char *endp;
	*(u8 *)val = !!simple_strtoul(u8str, &endp, 10);
    return 0;
}

static int __string2u32(u8 *u8str, void *val, u32 arg)
{
    char *endp;
    int base=10;
    if (u8str[0]=='0' && ((u8str[1]=='x')||(u8str[1]=='X')))
        base = 16;
    *(u32 *)val = simple_strtoul(u8str, &endp, base);    
    return 0;
}

static int __string2u8(u8 *u8str, void *val, u32 arg)
{
    char *endp;
    int base=10;
    if (u8str[0]=='0' && ((u8str[1]=='x')||(u8str[1]=='X')))
        base = 16;
    *(u8 *)val = simple_strtoul(u8str, &endp, base);    
    return 0;
}

#if 0
static int __string2s32(u8 *u8str, void *val, u32 arg)
{
    char *endp;
    int base=10;
    *(s32 *)val = simple_strtol(u8str, &endp, base);
    return 0;
}
#endif
static int __string2flag32(u8 *flag_str, void *flag, u32 arg)
{
    u32 *val=(u32 *)flag;

    if (arg >= (sizeof(u32)<<3))
        return -1;
    if (strcmp(flag_str, "on")==0) {
        *val |= (1<<arg);
        
        return 0;
    }
    if (strcmp(flag_str, "off")==0) {
        *val &= ~(1<<arg); 
        return 0;
    }
    return -1;
}
static int __string2mac(u8 *mac_str, void *val, u32 arg)
{
    int s, macaddr[6];
    u8 *mac=(u8 *)val;

    s = sscanf(mac_str, "%02x:%02x:%02x:%02x:%02x:%02x", 
        &macaddr[0], &macaddr[1], &macaddr[2],
        &macaddr[3], &macaddr[4], &macaddr[5]);
    if (s != 6)
        return -1;
    mac[0] = (u8)macaddr[0], mac[1] = (u8)macaddr[1];
    mac[2] = (u8)macaddr[2], mac[3] = (u8)macaddr[3];
    mac[4] = (u8)macaddr[4], mac[5] = (u8)macaddr[5];
    return 0;
}

static int __string2str(u8 *path, void *val, u32 arg)
{
    u8 *temp=(u8 *)val;

    sprintf(temp,"%s",path);

    return 0;
}

static int __string2array5(u8 *array_str, void *val, u32 arg)
{
    int s, array[5];
    int *offset=(int *)val;
    
    s = sscanf(array_str, "%d,%d,%d,%d,%d", 
     &array[0], &array[1], &array[2],
     &array[3], &array[4]);
    //SSV_LOG_DBG("STR %s\n", array_str);
    //SSV_LOG_DBG("ARRAY %d: %d %d %d %d %d\n",s , array[0], array[1], array[2], array[3], array[4]);
    if (s != 5)
        return -1;
        
    offset[0] = array[0];
    offset[1] = array[1];
    offset[2] = array[2];
    offset[3] = array[3];
    offset[4] = array[4];

    return 0;
}

static int __string2array4(u8 *array_str, void *val, u32 arg)
{
    int s, array[5];
    int *offset=(int *)val;
    
    s = sscanf(array_str, "%d,%d,%d,%d", 
     &array[0], &array[1], &array[2],
     &array[3]);
    //SSV_LOG_DBG("STR %s\n", array_str);
    //SSV_LOG_DBG("ARRAY %d: %d %d %d %d %d\n",s , array[0], array[1], array[2], array[3]);
    if (s != 4)
        return -1;
        
    offset[0] = array[0];
    offset[1] = array[1];
    offset[2] = array[2];
    offset[3] = array[3];

    return 0;
}


#if 0
static int __string2configuration(u8 *mac_str, void *val, u32 arg)
{
    unsigned int address,value;
    int i;

    i = sscanf(mac_str, "%08x:%08x", &address, &value);
    if (i != 2)
        return -1;

    for(i=0; i<EXTERNEL_CONFIG_SUPPORT; i++)
    {
        if(ssv_cfg.configuration[i][0] == 0x0)
        {
            ssv_cfg.configuration[i][0] = address;
            ssv_cfg.configuration[i][1] = value;
            return 0;
        }
    }

    return 0;
}
#endif
struct ssv6xxx_cfg_cmd_table cfg_cmds[] = {
    { "hw_mac",                     (void *)&ssv_cfg.maddr[0][0],                   0,      __string2mac        , NULL},
    { "hw_cap_ampdu_rx",            (void *)&ssv_cfg.hw_caps,                       0,      __string2flag32     , "on"},
    { "hw_cap_ampdu_tx",            (void *)&ssv_cfg.hw_caps,                       1,      __string2flag32     , "on"},
    { "hw_cap_hci_rx_aggr",         (void *)&ssv_cfg.hw_caps,                       2,      __string2flag32     , "on"},
    { "hw_cap_hci_tx_aggr",         (void *)&ssv_cfg.hw_caps,                       3,      __string2flag32     , "on"},
    { "scan_period",                (void *)&ssv_cfg.scan_period,                   0,      __string2u32        , "110"},
    { "firmware_path",              (void *)&ssv_cfg.firmware_path[0],              0,      __string2str        , NULL},
    { "firmware_name",              (void *)&ssv_cfg.firmware_name[0],              0,      __string2str        , "ssv6x5x-sw.bin"},
    { "firmware_choice",            (void *)&ssv_cfg.firmware_choice,               0,      __string2u32        , "1"},
    { "hci_trigger_en",             (void *)&ssv_cfg.hci_trigger_en,                0,      __string2u32        , "0"},
    { "hci_trigger_qlen",           (void *)&ssv_cfg.hci_trigger_qlen,              0,      __string2u32        , "10"},
    { "hci_idle_period",            (void *)&ssv_cfg.hci_idle_period,               0,      __string2u32        , "5"},
    { "flowctl",                    (void *)&ssv_cfg.flowctl,                       0,      __string2u32        , "1"},
    { "flowctl_low_threshold",      (void *)&ssv_cfg.flowctl_low_threshold,         0,      __string2u32        , "128"},
    { "flowctl_high_threshold",     (void *)&ssv_cfg.flowctl_high_threshold,        0,      __string2u32        , "256"},
    { "fw_reset",                   (void *)&ssv_cfg.fw_reset,                      0,      __string2u32        , "0"},
    { "fw_status_idle_cnt",         (void *)&ssv_cfg.fw_status_idle_cnt,            0,      __string2u32        , "50"},
    { "filter_duplicate_rx",        (void *)&ssv_cfg.filter_duplicate_rx,           0,      __string2u32        , "1"},
    { "wmm_follow_vo",              (void *)&ssv_cfg.wmm_follow_vo,                 0,      __string2u32        , "0"},
    { "ignore_firmware_version",    (void *)&ssv_cfg.ignore_firmware_version,       0,      __string2u32        , "0"},
	{ "dbg_level",    				(void *)&ssv_cfg.dbg_level,       				0,      __string2u32        , "3"},
	{ "dump_fmac_msg_rx",    		(void *)&ssv_cfg.dump_fmac_msg_rx,       		0,      __string2u32        , "0"},
    { "directly_ack_ctrl",    		(void *)&ssv_cfg.directly_ack_ctrl,       		0,      __string2u32        , "0"},
    { "cca",                        (void *)&ssv_cfg.cca,                           0,      __string2u32        , "1"},
    { "greentx_en",                 (void *)&ssv_cfg.greentx_en,                    0,      __string2u32        , "0"},
    { "disable_fw_thermal",         (void *)&ssv_cfg.disable_fw_thermal,            0,      __string2u32        , "0"},
    { "xtal_clock",    		        (void *)&ssv_cfg.xtal_clock,       		        0,      __string2u32        , "24"},
    { "thermal_xtal_offset",    	(void *)&ssv_cfg.thermal_xtal_offset,       	0,      __string2array5     , "6,6,0,6,30"},
    { "thermal_wifi_gain_offset",   (void *)&ssv_cfg.thermal_wifi_gain_offset,      0,      __string2array5     , "-5,-2,0,1,2"},
    { "thermal_ble_gain_offset",    (void *)&ssv_cfg.thermal_ble_gain_offset,       0,      __string2array5     , "-3,-2,0,1,3"},
    { "thermal_boundary",    		(void *)&ssv_cfg.thermal_boundary,       		0,      __string2array4     , "0,0,0,0"},
	{ "sta_max_reconnect_times",    (void *)&ssv_cfg.sta_max_reconnect_times,       0,      __string2u32        , "2"},
    { "ipc_tx_use_one_hwq",         (void *)&ssv_cfg.ipc_tx_use_one_hwq,            0,      __string2u32        , "3"},
    { "set_rts_method",             (void *)&ssv_cfg.set_rts_method,                0,      __string2u8         , "0"},
    { "ampdu_tx_lft",               (void *)&ssv_cfg.ampdu_tx_lft,                  0,      __string2u32        , "512"},
    { "xtal_clock",    		        (void *)&ssv_cfg.xtal_clock,       		        0,      __string2u32        , "24"},
    { "txq_credit_boundary",        (void *)&ssv_cfg.txq_credit_boundary,           0,      __string2u32        , "255"},
    { "ble_replace_scan_win",       (void *)&ssv_cfg.ble_replace_scan_win,          0,      __string2u32        , "0x30"},
    { "ble_replace_scan_interval",  (void *)&ssv_cfg.ble_replace_scan_interval,     0,      __string2u32        , "0x0140"},
    { "ble_dtm",                    (void *)&ssv_cfg.ble_dtm,                       0,      __string2u32        , "0"},
    { "use_2040",                   (void *)&ssv_cfg.use_2040,                      0,      __string2bool       , "1"},
    { "use_ep0_rw_reg",             (void *)&ssv_cfg.use_ep0_rw_reg,                0,      __string2bool       , "0"},
    { "channel_list_2p4g",          (void *)&ssv_cfg.channel_list_2p4g,             0,      __string2u32        , "0"},
    { "he_on",                      (void *)&ssv_cfg.he_on,                         0,      __string2bool       , "1"},
    { "mac_low_mask",               (void *)&ssv_cfg.mac_low_mask,                  0,      __string2u32        , "0"},
    { "mac_high_mask",              (void *)&ssv_cfg.mac_high_mask,                 0,      __string2u32        , "0x100"},
    { NULL, NULL, 0, NULL, NULL},
};


size_t ssv_read_line(struct file *fp, char *buf, size_t size)
{
	size_t num_read = 0;
	size_t total_read = 0;
	char *buffer;
	char ch;
	size_t start_ignore = 0;

	if (size <= 0 || buf == NULL) {
		total_read = -EINVAL;
		return -EINVAL;
	}

	buffer = buf;

	for (;;) {
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,4,37)
		if (fp->f_op && fp->f_op->read) 
			num_read = fp->f_op->read(fp, &ch, 1, &fp->f_pos);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
        num_read = kernel_read(fp, &ch, 1, &fp->f_pos);
#else    
		num_read = vfs_read(fp, &ch, 1, &fp->f_pos);
#endif    
#endif

		if (num_read < 0) {
			if (num_read == EINTR)
				continue;
			else
				return -1;
		}
		else if (num_read == 0) {
			if (total_read == 0)
				return 0;
			else
				break;
		}
		else {
			if (ch == '#')
				start_ignore = 1;
			if (total_read < size - 1) {
				total_read++;
				if (start_ignore)
					*buffer++ = '\0';
				else
					*buffer++ = ch; 
			}
			if (ch == '\n')
				break;
		}
	}

	*buffer = '\0';
	return total_read;
}

int ssv_ischar(char *c)
{
	int is_char = 1;

	while(*c) {
        if (isalpha(*c) || isdigit(*c) || *c == '_' || *c == ':' || *c == '/' || *c == '.' || *c == '-'|| *c == ',')
			c++;
		else {
			is_char = 0;
			break;
		}	
	}
	return is_char;
}

static void _set_initial_cfg_default(void)
{
    size_t s;
    
 	for(s=0; cfg_cmds[s].cfg_cmd != NULL; s++) {
		if ((cfg_cmds[s].def_val)!= NULL) {
			cfg_cmds[s].translate_func(cfg_cmds[s].def_val, 
				cfg_cmds[s].var, cfg_cmds[s].arg);
		}
	}
}


static void _ssv_import_default_cfg (char *stacfgpath)
{
	struct file *fp = (struct file *) NULL;
	char buf[MAX_CHARS_PER_LINE], cfg_cmd[32], cfg_value[32];
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))	
	mm_segment_t fs;
#endif
	size_t s, read_len = 0, is_cmd_support = 0;
	SSV_LOG_DBG("\n*** %s, %s ***\n\n", __func__, stacfgpath);	

	// Init the buffer with 0
	memset(&ssv_cfg, 0, sizeof(ssv_cfg));

    // set default config value
	_set_initial_cfg_default();

	 
	if (stacfgpath == NULL)
	{
	    //WARN_ON(1);
        SSV_LOG_DBG("Use default ssv configuration\n");
	    return;
    }
	 
	memset(buf, 0, sizeof(buf));
	fp = filp_open(stacfgpath, O_RDONLY, 0);
	
	if (IS_ERR(fp) || fp == NULL) {
		SSV_LOG_DBG("ERROR: filp_open\n");
        WARN_ON(1);
		return;
	}
	if (fp->f_path.dentry == NULL) {
		SSV_LOG_DBG("ERROR: dentry NULL\n");
        WARN_ON(1);
		return;
	}
	do {
		memset(cfg_cmd, '\0', sizeof(cfg_cmd));
		memset(cfg_value, '\0', sizeof(cfg_value));
		// Get current segment descriptor
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		fs = get_fs();
		// Set segment descriptor associated to kernel space
		set_fs(get_ds());
#endif
		// Read the file
		read_len = ssv_read_line(fp, buf, MAX_CHARS_PER_LINE);
		// Restore segment descriptor
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 10, 0))
		set_fs(fs);
#endif
		sscanf(buf, "%s = %s", cfg_cmd, cfg_value);
		if (!ssv_ischar(cfg_cmd) || !ssv_ischar(cfg_value)) {
			SSV_LOG_DBG("ERORR invalid parameter: %s\n", buf);
			WARN_ON(1);
			continue;
		}
		is_cmd_support = 0;
		for(s=0; cfg_cmds[s].cfg_cmd != NULL; s++) {
			if (strcmp(cfg_cmds[s].cfg_cmd, cfg_cmd)==0) {
				cfg_cmds[s].translate_func(cfg_value, 
					cfg_cmds[s].var, cfg_cmds[s].arg);
				//SSV_LOG_DBG("%scmd:%s, value:%s\n",buf, cfg_cmd, cfg_value);
				is_cmd_support = 1;
				break;
			}
		}

		if (!is_cmd_support && strlen(cfg_cmd) > 0) {
			SSV_LOG_DBG("ERROR Unsupported command: %s", cfg_cmd);
			WARN_ON(1);
		}
	} while (read_len > 0); //0: eof, < 0: error

	filp_close(fp, NULL);
}

extern int ssv6xxx_usb_init(void);
extern int ssv6xxx_sdio_init(void);
int ssv_device_init(void)
{
    SSV_LOG_DBG("%s()\n", __FUNCTION__);

    // Initialize default configuration from file.
    _ssv_import_default_cfg(stacfgpath);

#if (HWIF_SUPPORT != 2)
    ssv6xxx_usb_init();
#endif
#if (HWIF_SUPPORT != 1)
    ssv6xxx_sdio_init();
#endif
    return 0;
}

extern void ssv6xxx_usb_exit(void);
extern void ssv6xxx_sdio_exit(void);
void ssv_device_exit(void)
{
    SSV_LOG_DBG("%s()\n", __FUNCTION__);
#if (HWIF_SUPPORT != 2)
    ssv6xxx_usb_exit();
#endif
#if (HWIF_SUPPORT != 1)
    ssv6xxx_sdio_exit();
#endif
}

EXPORT_SYMBOL(ssv_device_init);
EXPORT_SYMBOL(ssv_device_exit);


