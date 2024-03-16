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
#include <linux/vmalloc.h>

#include "rwnx_platform.h"
#include "reg_access.h"
#include "hal_desc.h"
#include "rwnx_main.h"
#include "rwnx_pci.h"
#ifndef CONFIG_RWNX_FHOST
#include "ipc_host.h"
#endif /* !CONFIG_RWNX_FHOST */
#include "rwnx_msg_tx.h"

#ifdef AICWF_SDIO_SUPPORT
#include "aicwf_sdio.h"
#endif

#ifdef AICWF_USB_SUPPORT
#include "aicwf_usb.h"
#endif
#include "md5.h"
#include "aicwf_compat_8800dc.h"
#include "aicwf_compat_8800d80.h"

#ifdef CONFIG_USE_FW_REQUEST
#include <linux/firmware.h>
#endif

#define FW_PATH_MAX_LEN 200
extern char aic_fw_path[FW_PATH_MAX_LEN];

//Parser state
#define INIT 0
#define CMD 1
#define PRINT 2
#define GET_VALUE 3


struct rwnx_plat *g_rwnx_plat;

typedef struct
{
    txpwr_lvl_conf_t txpwr_lvl;
    txpwr_lvl_conf_v2_t txpwr_lvl_v2;
    txpwr_lvl_conf_v3_t txpwr_lvl_v3;
	txpwr_loss_conf_t txpwr_loss;
    txpwr_ofst_conf_t txpwr_ofst;
	txpwr_ofst2x_conf_t txpwr_ofst2x;
    xtal_cap_conf_t xtal_cap;
} userconfig_info_t;

userconfig_info_t userconfig_info = {
    .txpwr_lvl = {
        .enable           = 1,
        .dsss             = 9,
        .ofdmlowrate_2g4  = 8,
        .ofdm64qam_2g4    = 8,
        .ofdm256qam_2g4   = 8,
        .ofdm1024qam_2g4  = 8,
        .ofdmlowrate_5g   = 11,
        .ofdm64qam_5g     = 10,
        .ofdm256qam_5g    = 9,
        .ofdm1024qam_5g   = 9
    },
    .txpwr_lvl_v2 = {
        .enable             = 1,
        .pwrlvl_11b_11ag_2g4 =
            //1M,   2M,   5M5,  11M,  6M,   9M,   12M,  18M,  24M,  36M,  48M,  54M
            { 20,   20,   20,   20,   20,   20,   20,   20,   18,   18,   16,   16},
        .pwrlvl_11n_11ac_2g4 =
            //MCS0, MCS1, MCS2, MCS3, MCS4, MCS5, MCS6, MCS7, MCS8, MCS9
            { 20,   20,   20,   20,   18,   18,   16,   16,   16,   16},
        .pwrlvl_11ax_2g4 =
            //MCS0, MCS1, MCS2, MCS3, MCS4, MCS5, MCS6, MCS7, MCS8, MCS9, MCS10,MCS11
            { 20,   20,   20,   20,   18,   18,   16,   16,   16,   16,   15,   15},
    },
    .txpwr_lvl_v3 = {
        .enable             = 1,
        .pwrlvl_11b_11ag_2g4 =
            //1M,   2M,   5M5,  11M,  6M,   9M,   12M,  18M,  24M,  36M,  48M,  54M
            { 20,   20,   20,   20,   20,   20,   20,   20,   18,   18,   16,   16},
        .pwrlvl_11n_11ac_2g4 =
            //MCS0, MCS1, MCS2, MCS3, MCS4, MCS5, MCS6, MCS7, MCS8, MCS9
            { 20,   20,   20,   20,   18,   18,   16,   16,   16,   16},
        .pwrlvl_11ax_2g4 =
            //MCS0, MCS1, MCS2, MCS3, MCS4, MCS5, MCS6, MCS7, MCS8, MCS9, MCS10,MCS11
            { 20,   20,   20,   20,   18,   18,   16,   16,   16,   16,   15,   15},
         .pwrlvl_11a_5g =
            //NA,   NA,   NA,   NA,   6M,   9M,   12M,  18M,  24M,  36M,  48M,  54M
            { 0x80, 0x80, 0x80, 0x80, 20,   20,   20,   20,   18,   18,   16,   16},
        .pwrlvl_11n_11ac_5g =
            //MCS0, MCS1, MCS2, MCS3, MCS4, MCS5, MCS6, MCS7, MCS8, MCS9
            { 20,   20,   20,   20,   18,   18,   16,   16,   16,   15},
        .pwrlvl_11ax_5g =
            //MCS0, MCS1, MCS2, MCS3, MCS4, MCS5, MCS6, MCS7, MCS8, MCS9, MCS10,MCS11
            { 20,   20,   20,   20,   18,   18,   16,   16,   16,   15,   14,   14},
    },
	.txpwr_loss = {
		.loss_enable	  = 1,
		.loss_value 	  = 0,
	},
    .txpwr_ofst = {
        .enable       = 1,
        .chan_1_4     = 0,
        .chan_5_9     = 0,
        .chan_10_13   = 0,
        .chan_36_64   = 0,
        .chan_100_120 = 0,
        .chan_122_140 = 0,
        .chan_142_165 = 0,
    },
    .txpwr_ofst2x = {
        .enable       = 0,
        .pwrofst2x_tbl_2g4 =
        { // ch1-4, ch5-9, ch10-13
            {   0,    0,    0   }, // 11b
            {   0,    0,    0   }, // ofdm_highrate
            {   0,    0,    0   }, // ofdm_lowrate
        },
        .pwrofst2x_tbl_5g =
        { // ch42,  ch58, ch106,ch122,ch138,ch155
            {   0,    0,    0,    0,    0,    0   }, // ofdm_lowrate
            {   0,    0,    0,    0,    0,    0   }, // ofdm_highrate
            {   0,    0,    0,    0,    0,    0   }, // ofdm_midrate
        },
    },
    .xtal_cap = {
        .enable        = 0,
        .xtal_cap      = 24,
        .xtal_cap_fine = 31,
    },
};


#ifdef CONFIG_RWNX_TL4
/**
 * rwnx_plat_tl4_fw_upload() - Load the requested FW into embedded side.
 *
 * @rwnx_plat: pointer to platform structure
 * @fw_addr: Virtual address where the fw must be loaded
 * @filename: Name of the fw.
 *
 * Load a fw, stored as a hex file, into the specified address
 */
static int rwnx_plat_tl4_fw_upload(struct rwnx_plat *rwnx_plat, u8 *fw_addr,
								   char *filename)
{
	struct device *dev = rwnx_platform_get_dev(rwnx_plat);
	const struct firmware *fw;
	int err = 0;
	u32 *dst;
	u8 const *file_data;
	char typ0, typ1;
	u32 addr0, addr1;
	u32 dat0, dat1;
	int remain;

	err = request_firmware(&fw, filename, dev);
	if (err) {
		return err;
	}
	file_data = fw->data;
	remain = fw->size;

	/* Copy the file on the Embedded side */
	dev_dbg(dev, "\n### Now copy %s firmware, @ = %p\n", filename, fw_addr);

	/* Walk through all the lines of the configuration file */
	while (remain >= 16) {
		u32 data, offset;

		if (sscanf(file_data, "%c:%08X %04X", &typ0, &addr0, &dat0) != 3)
			break;
		if ((addr0 & 0x01) != 0) {
			addr0 = addr0 - 1;
			dat0 = 0;
		} else {
			file_data += 16;
			remain -= 16;
		}
		if ((remain < 16) ||
			(sscanf(file_data, "%c:%08X %04X", &typ1, &addr1, &dat1) != 3) ||
			(typ1 != typ0) || (addr1 != (addr0 + 1))) {
			typ1 = typ0;
			addr1 = addr0 + 1;
			dat1 = 0;
		} else {
			file_data += 16;
			remain -= 16;
		}

		if (typ0 == 'C') {
			offset = 0x00200000;
			if ((addr1 % 4) == 3)
				offset += 2*(addr1 - 3);
			else
				offset += 2*(addr1 + 1);

			data = dat1 | (dat0 << 16);
		} else {
			offset = 2*(addr1 - 1);
			data = dat0 | (dat1 << 16);
		}
		dst = (u32 *)(fw_addr + offset);
		*dst = data;
	}

	release_firmware(fw);

	return err;
}
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0)
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
#endif

#if 0
/**
 * rwnx_plat_bin_fw_upload() - Load the requested binary FW into embedded side.
 *
 * @rwnx_plat: pointer to platform structure
 * @fw_addr: Virtual address where the fw must be loaded
 * @filename: Name of the fw.
 *
 * Load a fw, stored as a binary file, into the specified address
 */
static int rwnx_plat_bin_fw_upload(struct rwnx_plat *rwnx_plat, u8 *fw_addr,
							   char *filename)
{
	const struct firmware *fw;
	struct device *dev = rwnx_platform_get_dev(rwnx_plat);
	int err = 0;
	unsigned int i, size;
	u32 *src, *dst;

	err = request_firmware(&fw, filename, dev);
	if (err) {
		return err;
	}

	/* Copy the file on the Embedded side */
	dev_dbg(dev, "\n### Now copy %s firmware, @ = %p\n", filename, fw_addr);

	src = (u32 *)fw->data;
	dst = (u32 *)fw_addr;
	size = (unsigned int)fw->size;

	/* check potential platform bug on multiple stores vs memcpy */
	for (i = 0; i < size; i += 4) {
		*dst++ = *src++;
	}

	release_firmware(fw);

	return err;
}
#endif

#define MD5(x) x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]
#define MD5PINRT "file md5:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\r\n"

static int rwnx_load_firmware(u32 **fw_buf, const char *name, struct device *device)
{
#ifdef CONFIG_USE_FW_REQUEST
	const struct firmware *fw = NULL;
	u32 *dst = NULL;
	void *buffer=NULL;
	MD5_CTX md5;
	unsigned char decrypt[16];
	int size = 0;
	int ret = 0;

	printk("%s: request firmware = %s \n", __func__ ,name);

	ret = request_firmware(&fw, name, NULL);

	if (ret < 0) {
		printk("Load %s fail\n", name);
		release_firmware(fw);
		return -1;
	}

	size = fw->size;
	dst = (u32 *)fw->data;

	if (size <= 0) {
		printk("wrong size of firmware file\n");
		release_firmware(fw);
		return -1;
	}


	buffer = vmalloc(size);
	memset(buffer, 0, size);
	memcpy(buffer, dst, size);

	*fw_buf = buffer;

	MD5Init(&md5);
	MD5Update(&md5, (unsigned char *)buffer, size);
	MD5Final(&md5, decrypt);
	printk(MD5PINRT, MD5(decrypt));

	release_firmware(fw);

	return size;
#else
    void *buffer = NULL;
    char *path = NULL;
    struct file *fp = NULL;
    int size = 0, len = 0;// i = 0;
    ssize_t rdlen = 0;
    //u32 *src = NULL, *dst = NULL;
	MD5_CTX md5;
	unsigned char decrypt[16];

    /* get the firmware path */
    path = __getname();
    if (!path) {
        *fw_buf = NULL;
        return -1;
    }
	
	len = snprintf(path, FW_PATH_MAX_LEN, "%s/%s", aic_fw_path, name);

    //len = snprintf(path, FW_PATH_MAX_LEN, "%s", name);
    if (len >= FW_PATH_MAX_LEN) {
        AICWFDBG(LOGERROR, "%s: %s file's path too long\n", __func__, name);
        *fw_buf = NULL;
        __putname(path);
        return -1;
    }

    AICWFDBG(LOGINFO, "%s :firmware path = %s  \n", __func__, path);

    /* open the firmware file */
    fp = filp_open(path, O_RDONLY, 0);
    if (IS_ERR_OR_NULL(fp)) {
        AICWFDBG(LOGERROR, "%s: %s file failed to open\n", __func__, name);
        *fw_buf = NULL;
        __putname(path);
        fp = NULL;
        return -1;
    }

    size = i_size_read(file_inode(fp));
    if (size <= 0) {
        AICWFDBG(LOGERROR, "%s: %s file size invalid %d\n", __func__, name, size);
        *fw_buf = NULL;
        __putname(path);
        filp_close(fp, NULL);
        fp = NULL;
        return -1;
    }

    /* start to read from firmware file */
    buffer = kzalloc(size, GFP_KERNEL);
    if (!buffer) {
        *fw_buf = NULL;
        __putname(path);
        filp_close(fp, NULL);
        fp = NULL;
        return -1;
    }

    #if LINUX_VERSION_CODE > KERNEL_VERSION(4, 13, 16)
    rdlen = kernel_read(fp, buffer, size, &fp->f_pos);
    #else
    rdlen = kernel_read(fp, fp->f_pos, buffer, size);
    #endif

    if (size != rdlen) {
        AICWFDBG(LOGERROR, "%s: %s file rdlen invalid %d\n", __func__, name, (int)rdlen);
        *fw_buf = NULL;
        __putname(path);
        filp_close(fp, NULL);
        fp = NULL;
        kfree(buffer);
        buffer = NULL;
        return -1;
    }
    if (rdlen > 0) {
        fp->f_pos += rdlen;
    }

#if 0
    /*start to transform the data format*/
    src = (u32 *)buffer;
    dst = (u32 *)kzalloc(size, GFP_KERNEL);

    if (!dst) {
        *fw_buf = NULL;
        __putname(path);
        filp_close(fp, NULL);
        fp = NULL;
        kfree(buffer);
        buffer = NULL;
        return -1;
    }

    for (i = 0; i < (size/4); i++) {
        dst[i] = src[i];
    }
#endif

    __putname(path);
    filp_close(fp, NULL);
    fp = NULL;
    //kfree(buffer);
    //buffer = NULL;
    *fw_buf = (u32*)buffer;

	MD5Init(&md5);
	MD5Update(&md5, (unsigned char *)buffer, size);
	MD5Final(&md5, decrypt);

	AICWFDBG(LOGINFO, MD5PINRT, MD5(decrypt));

    return size;
#endif
}



/* buffer is allocated by kzalloc */
int rwnx_request_firmware_common(struct rwnx_hw *rwnx_hw, u32** buffer, const char *filename)
{
    int size;

    AICWFDBG(LOGINFO, "### Load file %s\n", filename);

    size = rwnx_load_firmware(buffer, filename, NULL);

    return size;
}

static void rwnx_restore_firmware(u32 **fw_buf)
{
#ifdef CONFIG_USE_FW_REQUEST
	vfree(*fw_buf);
#else
	kfree(*fw_buf);
#endif
    *fw_buf = NULL;
}


void rwnx_release_firmware_common(u32** buffer)
{
    rwnx_restore_firmware(buffer);
}


/**
 * rwnx_plat_bin_fw_upload_2() - Load the requested binary FW into embedded side.
 *
 * @rwnx_hw: Main driver data
 * @fw_addr: Address where the fw must be loaded
 * @filename: Name of the fw.
 *
 * Load a fw, stored as a binary file, into the specified address
 */
int rwnx_plat_bin_fw_upload_2(struct rwnx_hw *rwnx_hw, u32 fw_addr,
							   char *filename)
{
	int err = 0;
	unsigned int i = 0, size;
//	  u32 *src;
	u32 *dst=NULL;

	/* Copy the file on the Embedded side */
	AICWFDBG(LOGINFO, "### Upload %s firmware, @ = %x\n", filename, fw_addr);

	size = rwnx_request_firmware_common(rwnx_hw, &dst, filename);
	if (!dst) {
		AICWFDBG(LOGERROR, "No such file or directory\n");
		return -1;
	}
	if (size <= 0) {
			AICWFDBG(LOGERROR, "wrong size of firmware file\n");
			dst = NULL;
			err = -1;
	}

	AICWFDBG(LOGINFO, "size=%d, dst[0]=%x\n", size, dst[0]);
	if (size > 512) {
		for (; i < (size - 512); i += 512) {
			//printk("wr blk 0: %p -> %x\r\n", dst + i / 4, fw_addr + i);
			err = rwnx_send_dbg_mem_block_write_req(rwnx_hw, fw_addr + i, 512, dst + i / 4);
			if (err) {
				AICWFDBG(LOGERROR, "bin upload fail: %x, err:%d\r\n", fw_addr + i, err);
				break;
			}
		}
	}
	if (!err && (i < size)) {
		//printk("wr blk 1: %p -> %x\r\n", dst + i / 4, fw_addr + i);
		err = rwnx_send_dbg_mem_block_write_req(rwnx_hw, fw_addr + i, size - i, dst + i / 4);
		if (err) {
			AICWFDBG(LOGERROR, "bin upload fail: %x, err:%d\r\n", fw_addr + i, err);
		}
	}

	if (dst) {
		rwnx_release_firmware_common(&dst);
	}

	return err;
}



typedef struct {
	txpwr_idx_conf_t txpwr_idx;
	txpwr_ofst_conf_t txpwr_ofst;
	xtal_cap_conf_t xtal_cap;
} nvram_info_t;

nvram_info_t nvram_info = {
	.txpwr_idx = {
		.enable           = 1,
		.dsss             = 9,
		.ofdmlowrate_2g4  = 8,
		.ofdm64qam_2g4    = 8,
		.ofdm256qam_2g4   = 8,
		.ofdm1024qam_2g4  = 8,
		.ofdmlowrate_5g   = 11,
		.ofdm64qam_5g     = 10,
		.ofdm256qam_5g    = 9,
		.ofdm1024qam_5g   = 9
	},
	.txpwr_ofst = {
		.enable       = 1,
		.chan_1_4     = 0,
		.chan_5_9     = 0,
		.chan_10_13   = 0,
		.chan_36_64   = 0,
		.chan_100_120 = 0,
		.chan_122_140 = 0,
		.chan_142_165 = 0,
	},
	.xtal_cap = {
        .enable        = 0,
        .xtal_cap      = 24,
        .xtal_cap_fine = 31,
    },
};

void get_userconfig_txpwr_ofst_in_fdrv(txpwr_ofst_conf_t *txpwr_ofst)
{
    txpwr_ofst->enable       = userconfig_info.txpwr_ofst.enable;
    txpwr_ofst->chan_1_4     = userconfig_info.txpwr_ofst.chan_1_4;
    txpwr_ofst->chan_5_9     = userconfig_info.txpwr_ofst.chan_5_9;
    txpwr_ofst->chan_10_13   = userconfig_info.txpwr_ofst.chan_10_13;
    txpwr_ofst->chan_36_64   = userconfig_info.txpwr_ofst.chan_36_64;
    txpwr_ofst->chan_100_120 = userconfig_info.txpwr_ofst.chan_100_120;
    txpwr_ofst->chan_122_140 = userconfig_info.txpwr_ofst.chan_122_140;
    txpwr_ofst->chan_142_165 = userconfig_info.txpwr_ofst.chan_142_165;

    AICWFDBG(LOGINFO, "%s:enable      :%d\r\n", __func__, txpwr_ofst->enable);
    AICWFDBG(LOGINFO, "%s:chan_1_4    :%d\r\n", __func__, txpwr_ofst->chan_1_4);
    AICWFDBG(LOGINFO, "%s:chan_5_9    :%d\r\n", __func__, txpwr_ofst->chan_5_9);
    AICWFDBG(LOGINFO, "%s:chan_10_13  :%d\r\n", __func__, txpwr_ofst->chan_10_13);
    AICWFDBG(LOGINFO, "%s:chan_36_64  :%d\r\n", __func__, txpwr_ofst->chan_36_64);
    AICWFDBG(LOGINFO, "%s:chan_100_120:%d\r\n", __func__, txpwr_ofst->chan_100_120);
    AICWFDBG(LOGINFO, "%s:chan_122_140:%d\r\n", __func__, txpwr_ofst->chan_122_140);
    AICWFDBG(LOGINFO, "%s:chan_142_165:%d\r\n", __func__, txpwr_ofst->chan_142_165);
}

void get_userconfig_txpwr_ofst2x_in_fdrv(txpwr_ofst2x_conf_t *txpwr_ofst2x)
{
    int type, ch_grp;
    *txpwr_ofst2x = userconfig_info.txpwr_ofst2x;
    AICWFDBG(LOGINFO, "%s:enable      :%d\r\n", __func__, txpwr_ofst2x->enable);
    AICWFDBG(LOGINFO, "pwrofst2x 2.4g: [0]:11b, [1]:ofdm_highrate, [2]:ofdm_lowrate\n"
        "  chan=" "\t1-4" "\t5-9" "\t10-13");
    for (type = 0; type < 3; type++) {
        AICWFDBG(LOGINFO, "\n  [%d] =", type);
        for (ch_grp = 0; ch_grp < 3; ch_grp++) {
            AICWFDBG(LOGINFO, "\t%d", txpwr_ofst2x->pwrofst2x_tbl_2g4[type][ch_grp]);
        }
    }
    AICWFDBG(LOGINFO, "\npwrofst2x 5g: [0]:ofdm_lowrate, [1]:ofdm_highrate, [2]:ofdm_midrate\n"
        "  chan=" "\t36-50" "\t51-64" "\t98-114" "\t115-130" "\t131-146" "\t147-166");
    for (type = 0; type < 3; type++) {
        AICWFDBG(LOGINFO, "\n  [%d] =", type);
        for (ch_grp = 0; ch_grp < 6; ch_grp++) {
            AICWFDBG(LOGINFO, "\t%d", txpwr_ofst2x->pwrofst2x_tbl_5g[type][ch_grp]);
        }
    }
    AICWFDBG(LOGINFO, "\n");
}

void get_userconfig_txpwr_idx(txpwr_idx_conf_t *txpwr_idx)
{
	memcpy(txpwr_idx, &(nvram_info.txpwr_idx), sizeof(txpwr_idx_conf_t));
}

void get_userconfig_txpwr_ofst(txpwr_ofst_conf_t *txpwr_ofst)
{
	memcpy(txpwr_ofst, &(nvram_info.txpwr_ofst), sizeof(txpwr_ofst_conf_t));
}

void get_userconfig_xtal_cap(xtal_cap_conf_t *xtal_cap)
{
    if(nvram_info.xtal_cap.enable){
        *xtal_cap = nvram_info.xtal_cap;
    }
    
    if(userconfig_info.xtal_cap.enable){
        *xtal_cap = userconfig_info.xtal_cap;
    }
    
    AICWFDBG(LOGINFO, "%s:enable       :%d\r\n", __func__, xtal_cap->enable);
    AICWFDBG(LOGINFO, "%s:xtal_cap     :%d\r\n", __func__, xtal_cap->xtal_cap);
    AICWFDBG(LOGINFO, "%s:xtal_cap_fine:%d\r\n", __func__, xtal_cap->xtal_cap_fine);
}


#define MATCH_NODE(type, node, cfg_key) {cfg_key, offsetof(type, node)}

struct parse_match_t {
	char keyname[64];
	int  offset;
};

static const char *parse_key_prefix[] = {
	[0x01] = "module0_",
	[0x21] = "module1_",
};

static const struct parse_match_t parse_match_tab[] = {
	MATCH_NODE(nvram_info_t, txpwr_idx.enable,           "enable"),
	MATCH_NODE(nvram_info_t, txpwr_idx.dsss,             "dsss"),
	MATCH_NODE(nvram_info_t, txpwr_idx.ofdmlowrate_2g4,  "ofdmlowrate_2g4"),
	MATCH_NODE(nvram_info_t, txpwr_idx.ofdm64qam_2g4,    "ofdm64qam_2g4"),
	MATCH_NODE(nvram_info_t, txpwr_idx.ofdm256qam_2g4,   "ofdm256qam_2g4"),
	MATCH_NODE(nvram_info_t, txpwr_idx.ofdm1024qam_2g4,  "ofdm1024qam_2g4"),
	MATCH_NODE(nvram_info_t, txpwr_idx.ofdmlowrate_5g,   "ofdmlowrate_5g"),
	MATCH_NODE(nvram_info_t, txpwr_idx.ofdm64qam_5g,     "ofdm64qam_5g"),
	MATCH_NODE(nvram_info_t, txpwr_idx.ofdm256qam_5g,    "ofdm256qam_5g"),
	MATCH_NODE(nvram_info_t, txpwr_idx.ofdm1024qam_5g,   "ofdm1024qam_5g"),

	MATCH_NODE(nvram_info_t, txpwr_ofst.enable,          "ofst_enable"),
	MATCH_NODE(nvram_info_t, txpwr_ofst.chan_1_4,        "ofst_chan_1_4"),
	MATCH_NODE(nvram_info_t, txpwr_ofst.chan_5_9,        "ofst_chan_5_9"),
	MATCH_NODE(nvram_info_t, txpwr_ofst.chan_10_13,      "ofst_chan_10_13"),
	MATCH_NODE(nvram_info_t, txpwr_ofst.chan_36_64,      "ofst_chan_36_64"),
	MATCH_NODE(nvram_info_t, txpwr_ofst.chan_100_120,    "ofst_chan_100_120"),
	MATCH_NODE(nvram_info_t, txpwr_ofst.chan_122_140,    "ofst_chan_122_140"),
	MATCH_NODE(nvram_info_t, txpwr_ofst.chan_142_165,    "ofst_chan_142_165"),

	MATCH_NODE(nvram_info_t, xtal_cap.enable,          	"xtal_enable"),
	MATCH_NODE(nvram_info_t, xtal_cap.xtal_cap,        	"xtal_cap"),
	MATCH_NODE(nvram_info_t, xtal_cap.xtal_cap_fine,    "xtal_cap_fine"),
};

static int parse_key_val(const char *str, const char *key, char *val)
{
	const char *p = NULL;
	const char *dst = NULL;
	int keysize = 0;
	int bufsize = 0;

	if (str == NULL || key == NULL || val == NULL)
		return -1;

	keysize = strlen(key);
	bufsize = strlen(str);
	if (bufsize <= keysize)
		return -1;

	p = str;
	while (*p != 0 && *p == ' ')
		p++;

	if (*p == '#')
		return -1;

	if (str + bufsize - p <= keysize)
		return -1;

	if (strncmp(p, key, keysize) != 0)
		return -1;

	p += keysize;

	while (*p != 0 && *p == ' ')
		p++;

	if (*p != '=')
		return -1;

	p++;
	while (*p != 0 && *p == ' ')
		p++;

	if (*p == '"')
		p++;

	dst = p;
	while (*p != 0)
		p++;

	p--;
	while (*p == ' ')
		p--;

	if (*p == '"')
		p--;

	while (*p == '\r' || *p == '\n')
		p--;

	p++;
	strncpy(val, dst, p -dst);
	val[p - dst] = 0;
	return 0;
}


int rwnx_atoi(char *value)
{
    int len = 0;
    int i = 0;
    int result = 0;
    int flag = 1;

    if (value[0] == '-') {
        flag = -1;
        value++;
    }
    len = strlen(value);

    for (i = 0;i < len ;i++) {
        result = result * 10;
        if (value[i] >= 48 && value[i] <= 57) {
            result += value[i] - 48;
        } else {
            result = 0;
            break;
        }
    }

    return result * flag;
}


void rwnx_plat_nvram_set_value(char *command, char *value)
{
    //TODO send command
    AICWFDBG(LOGINFO, "%s:command=%s value=%s\n", __func__, command, value);
    if (!strcmp(command, "enable")) {
        userconfig_info.txpwr_lvl.enable = rwnx_atoi(value);
        userconfig_info.txpwr_lvl_v2.enable = rwnx_atoi(value);
    } else if (!strcmp(command, "dsss")) {
        userconfig_info.txpwr_lvl.dsss = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdmlowrate_2g4")) {
        userconfig_info.txpwr_lvl.ofdmlowrate_2g4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm64qam_2g4")) {
        userconfig_info.txpwr_lvl.ofdm64qam_2g4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm256qam_2g4")) {
        userconfig_info.txpwr_lvl.ofdm256qam_2g4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm1024qam_2g4")) {
        userconfig_info.txpwr_lvl.ofdm1024qam_2g4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdmlowrate_5g")) {
        userconfig_info.txpwr_lvl.ofdmlowrate_5g = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm64qam_5g")) {
        userconfig_info.txpwr_lvl.ofdm64qam_5g = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm256qam_5g")) {
        userconfig_info.txpwr_lvl.ofdm256qam_5g = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm1024qam_5g")) {
        userconfig_info.txpwr_lvl.ofdm1024qam_5g = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_1m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_2m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_5m5_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_11m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_6m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_9m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_12m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_18m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_24m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_36m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_48m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[10] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_54m_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11b_11ag_2g4[11] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs0_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs1_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs2_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs3_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs4_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs5_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs6_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs7_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs8_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs9_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11n_11ac_2g4[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs0_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs1_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs2_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs3_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs4_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs5_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs6_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs7_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs8_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs9_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs10_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[10] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs11_2g4")) {
        userconfig_info.txpwr_lvl_v2.pwrlvl_11ax_2g4[11] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_enable")) {
        userconfig_info.txpwr_ofst.enable = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_1_4")) {
        userconfig_info.txpwr_ofst.chan_1_4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_5_9")) {
        userconfig_info.txpwr_ofst.chan_5_9 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_10_13")) {
        userconfig_info.txpwr_ofst.chan_10_13 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_36_64")) {
        userconfig_info.txpwr_ofst.chan_36_64 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_100_120")) {
        userconfig_info.txpwr_ofst.chan_100_120 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_122_140")) {
        userconfig_info.txpwr_ofst.chan_122_140 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_142_165")) {
        userconfig_info.txpwr_ofst.chan_142_165 = rwnx_atoi(value);
    } else if (!strcmp(command, "xtal_enable")) {
        userconfig_info.xtal_cap.enable = rwnx_atoi(value);
    } else if (!strcmp(command, "xtal_cap")) {
        userconfig_info.xtal_cap.xtal_cap = rwnx_atoi(value);
    } else if (!strcmp(command, "xtal_cap_fine")) {
        userconfig_info.xtal_cap.xtal_cap_fine = rwnx_atoi(value);
    } else {
        AICWFDBG(LOGERROR, "invalid cmd: %s\n", command);
    }
}

void rwnx_plat_nvram_set_value_v3(char *command, char *value)
{
    //TODO send command
    AICWFDBG(LOGINFO, "%s:command=%s value=%s\n", __func__, command, value);
    if (!strcmp(command, "enable")) {
        userconfig_info.txpwr_lvl.enable = rwnx_atoi(value);
        userconfig_info.txpwr_lvl_v3.enable = rwnx_atoi(value);
    } else if (!strcmp(command, "dsss")) {
        userconfig_info.txpwr_lvl.dsss = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdmlowrate_2g4")) {
        userconfig_info.txpwr_lvl.ofdmlowrate_2g4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm64qam_2g4")) {
        userconfig_info.txpwr_lvl.ofdm64qam_2g4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm256qam_2g4")) {
        userconfig_info.txpwr_lvl.ofdm256qam_2g4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm1024qam_2g4")) {
        userconfig_info.txpwr_lvl.ofdm1024qam_2g4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdmlowrate_5g")) {
        userconfig_info.txpwr_lvl.ofdmlowrate_5g = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm64qam_5g")) {
        userconfig_info.txpwr_lvl.ofdm64qam_5g = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm256qam_5g")) {
        userconfig_info.txpwr_lvl.ofdm256qam_5g = rwnx_atoi(value);
    } else if (!strcmp(command, "ofdm1024qam_5g")) {
        userconfig_info.txpwr_lvl.ofdm1024qam_5g = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_1m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_2m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_5m5_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_11m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_6m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_9m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_12m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_18m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_24m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_36m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_48m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[10] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11b_11ag_54m_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11b_11ag_2g4[11] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs0_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs1_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs2_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs3_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs4_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs5_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs6_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs7_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs8_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs9_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_2g4[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs0_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs1_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs2_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs3_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs4_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs5_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs6_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs7_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs8_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs9_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs10_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[10] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs11_2g4")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_2g4[11] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_1m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_2m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_5m5_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_11m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_6m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_9m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_12m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_18m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_24m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_36m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_48m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[10] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11a_54m_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11a_5g[11] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs0_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs1_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs2_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs3_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs4_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs5_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs6_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs7_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs8_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11n_11ac_mcs9_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11n_11ac_5g[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs0_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[0] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs1_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[1] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs2_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[2] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs3_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[3] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs4_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[4] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs5_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[5] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs6_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[6] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs7_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[7] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs8_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[8] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs9_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[9] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs10_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[10] = rwnx_atoi(value);
    } else if (!strcmp(command,     "lvl_11ax_mcs11_5g")) {
        userconfig_info.txpwr_lvl_v3.pwrlvl_11ax_5g[11] = rwnx_atoi(value);
    } else if (!strcmp(command, "loss_enable")) {
        userconfig_info.txpwr_loss.loss_enable = rwnx_atoi(value);
    } else if (!strcmp(command, "loss_value")) {
        userconfig_info.txpwr_loss.loss_value = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_enable")) {
        userconfig_info.txpwr_ofst.enable = rwnx_atoi(value);
		userconfig_info.txpwr_ofst2x.enable = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_1_4")) {
        userconfig_info.txpwr_ofst.chan_1_4 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_5_9")) {
        userconfig_info.txpwr_ofst.chan_5_9 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_10_13")) {
        userconfig_info.txpwr_ofst.chan_10_13 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_36_64")) {
        userconfig_info.txpwr_ofst.chan_36_64 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_100_120")) {
        userconfig_info.txpwr_ofst.chan_100_120 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_122_140")) {
        userconfig_info.txpwr_ofst.chan_122_140 = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_chan_142_165")) {
        userconfig_info.txpwr_ofst.chan_142_165 = rwnx_atoi(value);
	} else if (!strcmp(command, "ofst_2g4_11b_chan_1_4")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[0][0] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_2g4_11b_chan_5_9")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[0][1] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_2g4_11b_chan_10_13")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[0][2] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_2g4_ofdm_highrate_chan_1_4")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[1][0] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_2g4_ofdm_highrate_chan_5_9")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[1][1] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_2g4_ofdm_highrate_chan_10_13")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[1][2] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_2g4_ofdm_lowrate_chan_1_4")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[2][0] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_2g4_ofdm_lowrate_chan_5_9")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[2][1] = rwnx_atoi(value);
	} else if (!strcmp(command, "ofst_2g4_ofdm_lowrate_chan_10_13")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_2g4[2][0] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_lowrate_chan_42")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[0][0] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_lowrate_chan_58")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[0][1] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_lowrate_chan_106")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[0][2] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_lowrate_chan_122")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[0][3] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_lowrate_chan_138")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[0][4] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_lowrate_chan_155")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[0][5] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_highrate_chan_42")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[1][0] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_highrate_chan_58")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[1][1] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_highrate_chan_106")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[1][2] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_highrate_chan_122")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[1][3] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_highrate_chan_138")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[1][4] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_highrate_chan_155")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[1][5] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_midrate_chan_42")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[2][0] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_midrate_chan_58")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[2][1] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_midrate_chan_106")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[2][2] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_midrate_chan_122")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[2][3] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_midrate_chan_138")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[2][4] = rwnx_atoi(value);
    } else if (!strcmp(command, "ofst_5g_ofdm_midrate_chan_155")) {
        userconfig_info.txpwr_ofst2x.pwrofst2x_tbl_5g[2][5] = rwnx_atoi(value);
    } else if (!strcmp(command, "xtal_enable")) {
        userconfig_info.xtal_cap.enable = rwnx_atoi(value);
    } else if (!strcmp(command, "xtal_cap")) {
        userconfig_info.xtal_cap.xtal_cap = rwnx_atoi(value);
    } else if (!strcmp(command, "xtal_cap_fine")) {
        userconfig_info.xtal_cap.xtal_cap_fine = rwnx_atoi(value);
    } else {
        AICWFDBG(LOGERROR, "invalid cmd: %s\n", command);
    }
}

void rwnx_plat_userconfig_parsing2(char *buffer, int size)
{
    int i = 0;
    int parse_state = 0;
    char command[30];
    char value[100];
    int char_counter = 0;

    memset(command, 0, 30);
    memset(value, 0, 100);

    for (i = 0; i < size; i++) {
        //Send command or print nvram log when char is \r or \n
        if (buffer[i] == 0x0a || buffer[i] == 0x0d) {
            if (command[0] != 0 && value[0] != 0) {
                if (parse_state == PRINT) {
                    AICWFDBG(LOGINFO, "%s:%s\r\n", __func__, value);
                } else if (parse_state == GET_VALUE) {
                    rwnx_plat_nvram_set_value(command, value);
                }
            }
            //Reset command value and char_counter
            memset(command, 0, 30);
            memset(value, 0, 100);
            char_counter = 0;
            parse_state = INIT;
            continue;
        }

        //Switch parser state
        if (parse_state == INIT) {
            if (buffer[i] == '#') {
                parse_state = PRINT;
                continue;
            } else if (buffer[i] == 0x0a || buffer[i] == 0x0d) {
                parse_state = INIT;
                continue;
            } else {
                parse_state = CMD;
            }
        }

        //Fill data to command and value
        if (parse_state == PRINT) {
            command[0] = 0x01;
            value[char_counter] = buffer[i];
            char_counter++;
        } else if (parse_state == CMD) {
            if (command[0] != 0 && buffer[i] == '=') {
                parse_state = GET_VALUE;
                char_counter = 0;
                continue;
            }
            command[char_counter] = buffer[i];
            char_counter++;
        } else if (parse_state == GET_VALUE) {
            value[char_counter] = buffer[i];
            char_counter++;
        }
    }
}

void rwnx_plat_userconfig_parsing3(char *buffer, int size)
{
    int i = 0;
    int parse_state = 0;
    char command[64];
    char value[100];
    int char_counter = 0;

    memset(command, 0, 64);
    memset(value, 0, 100);

    for (i = 0; i < size; i++) {
        //Send command or print nvram log when char is \r or \n
        if (buffer[i] == 0x0a || buffer[i] == 0x0d) {
            if (command[0] != 0 && value[0] != 0) {
                if (parse_state == PRINT) {
                    AICWFDBG(LOGINFO, "%s:%s\r\n", __func__, value);
                } else if (parse_state == GET_VALUE) {
                    rwnx_plat_nvram_set_value_v3(command, value);
                }
            }
            //Reset command value and char_counter
            memset(command, 0, 64);
            memset(value, 0, 100);
            char_counter = 0;
            parse_state = INIT;
            continue;
        }

        //Switch parser state
        if (parse_state == INIT) {
            if (buffer[i] == '#') {
                parse_state = PRINT;
                continue;
            } else if (buffer[i] == 0x0a || buffer[i] == 0x0d) {
                parse_state = INIT;
                continue;
            } else {
                parse_state = CMD;
            }
        }

        //Fill data to command and value
        if (parse_state == PRINT) {
            command[0] = 0x01;
            value[char_counter] = buffer[i];
            char_counter++;
        } else if (parse_state == CMD) {
            if (command[0] != 0 && buffer[i] == '=') {
                parse_state = GET_VALUE;
                char_counter = 0;
                continue;
            }
            command[char_counter] = buffer[i];
            char_counter++;
        } else if (parse_state == GET_VALUE) {
            if(buffer[i] != 0x2D && (buffer[i] < 0x30 || buffer[i] > 0x39)) {
                continue;
            }
            value[char_counter] = buffer[i];
            char_counter++;
        }
    }
}

void rwnx_plat_userconfig_parsing(struct rwnx_hw *rwnx_hw, char *buffer, int size)
{
	char conf[100], keyname[64];
	char *line;
        char *data;
        int  i = 0, err, len = 0;
	long val;

        if (size <= 0) {
                pr_err("Config buffer size %d error\n", size);
                return;
        }

	printk("%s rwnx_hw->vendor_info:0x%02X \r\n", __func__, rwnx_hw->vendor_info);
	if (rwnx_hw->vendor_info == 0x00 ||
		(rwnx_hw->vendor_info > (sizeof(parse_key_prefix) / sizeof(parse_key_prefix[0]) - 1))) {
		printk("Unsuppor vendor info config\n");
		printk("Using module0 config\n");
		rwnx_hw->vendor_info = 0x01;
		//return;
	}

	data = vmalloc(size + 1);
        if (!data) {
                pr_err("vmalloc fail\n");
                return;
        }

        memcpy(data, buffer, size);
        buffer = data;

	while (1) {
		line = buffer;
                if (*line == 0)
                       break;

                while (*buffer != '\r' && *buffer != '\n' && *buffer != 0 && len++ < size)
                        buffer++;

                while ((*buffer == '\r' || *buffer == '\n') && len++ < size)
                        *buffer++ = 0;

                if (len >= size)
                        *buffer = 0;

		// store value to data struct
		for (i = 0; i < sizeof(parse_match_tab) / sizeof(parse_match_tab[0]); i++) {
			sprintf(&keyname[0], "%s%s", parse_key_prefix[rwnx_hw->vendor_info], parse_match_tab[i].keyname);
			if (parse_key_val(line, keyname, conf) == 0) {
				err = kstrtol(conf, 0, &val);
				*(unsigned long *)((unsigned long)&nvram_info + parse_match_tab[i].offset) = val;
				printk("%s, %s = %ld\n",  __func__, parse_match_tab[i].keyname, val);
				break;
			}
		}

	}
	vfree(data);
}

static int aic_load_firmware(u32 ** fw_buf, char *fw_path,const char *name, struct device *device)
{
#ifdef CONFIG_USE_FW_REQUEST
	const struct firmware *fw = NULL;
	u32 *dst = NULL;
	void *buffer=NULL;
	MD5_CTX md5;
	unsigned char decrypt[16];
	int size = 0;
	int ret = 0;
	
	AICWFDBG(LOGINFO, "%s: request firmware = %s \n", __func__ ,name);
	
	ret = request_firmware(&fw, name, NULL);
		
	if (ret < 0) {
		AICWFDBG(LOGERROR, "Load %s fail\n", name);
		release_firmware(fw);
		return -1;
	}
		
	size = fw->size;
	dst = (u32 *)fw->data;
	
	if (size <= 0) {
		AICWFDBG(LOGERROR, "wrong size of firmware file\n");
		release_firmware(fw);
		return -1;
	}
	
	buffer = vmalloc(size);
	memset(buffer, 0, size);
	memcpy(buffer, dst, size);
		
	*fw_buf = buffer;
	
	MD5Init(&md5);
	MD5Update(&md5, (unsigned char *)buffer, size);
	MD5Final(&md5, decrypt);
	AICWFDBG(LOGINFO, MD5PINRT, MD5(decrypt));
		
	release_firmware(fw);
		
	return size;
#else
    void *buffer=NULL;
    char *path=NULL;
    struct file *fp=NULL;
    int size = 0, len=0;//, i=0;
    ssize_t rdlen=0;
    //u32 *src=NULL, *dst = NULL;

    /* get the firmware path */
    path = __getname();
    if (!path){
            *fw_buf=NULL;
            return -1;
    }

	len = sprintf(path, "%s/%s",fw_path, name);

    AICWFDBG(LOGINFO, "%s :firmware path = %s  \n", __func__ ,path);


    /* open the firmware file */
    fp=filp_open(path, O_RDONLY, 0);
    if(IS_ERR(fp) || (!fp)){
		printk("%s: %s file failed to open\n", __func__, name);
		if(IS_ERR(fp)){
			printk("is_Err\n");
		}
		if((!fp)){
			printk("null\n");
		}
		*fw_buf=NULL;
		__putname(path);
 		fp=NULL;
		return -1;
    }

    size = i_size_read(file_inode(fp));
    if(size<=0){
            printk("%s: %s file size invalid %d\n", __func__, name, size);
            *fw_buf=NULL;
            __putname(path);
            filp_close(fp,NULL);
            fp=NULL;
            return -1;
	}

    /* start to read from firmware file */
    buffer = vmalloc(size);
    memset(buffer, 0, size);
    if(!buffer){
            *fw_buf=NULL;
            __putname(path);
            filp_close(fp,NULL);
            fp=NULL;
            return -1;
    }


    #if LINUX_VERSION_CODE > KERNEL_VERSION(4, 13, 16)
    rdlen = kernel_read(fp, buffer, size, &fp->f_pos);
    #else
    rdlen = kernel_read(fp, fp->f_pos, buffer, size);
    #endif

    if(size != rdlen){
            printk("%s: %s file rdlen invalid %d %d\n", __func__, name, (int)rdlen, size);
            *fw_buf=NULL;
            __putname(path);
            filp_close(fp,NULL);
            fp=NULL;
            vfree(buffer);
            buffer=NULL;
            return -1;
    }
    if(rdlen > 0){
            fp->f_pos += rdlen;
            //printk("f_pos=%d\n", (int)fp->f_pos);
    }

#if 0
   /*start to transform the data format*/
    src = (u32*)buffer;
    //printk("malloc dst\n");
    dst = (u32*)vmalloc(size);
    memset(dst, 0, size);

    if(!dst){
            *fw_buf=NULL;
            __putname(path);
            filp_close(fp,NULL);
            fp=NULL;
            vfree(buffer);
            buffer=NULL;
            return -1;
    }

    for(i=0;i<(size/4);i++){
            dst[i] = src[i];
    }
#endif

    __putname(path);
    filp_close(fp,NULL);
    fp=NULL;
    //vfree(buffer);
    //buffer=NULL;
    *fw_buf = (u32 *)buffer;

    return size;
#endif
}



#define FW_USERCONFIG_NAME       "aic_userconfig.txt"

int rwnx_plat_userconfig_upload_android(struct rwnx_hw *rwnx_hw, char *fw_path, char *filename)
{
    int size;
    u32 *dst=NULL;

	printk("userconfig file path:%s \r\n", filename);

    /* load aic firmware */
    size = aic_load_firmware(&dst, fw_path ,filename, NULL);
    if(size <= 0){
            printk("wrong size of firmware file\n");
            vfree(dst);
            dst = NULL;
            return 0;
    }

	/* Copy the file on the Embedded side */
    printk("### Upload %s userconfig, size=%d\n", filename, size);

	rwnx_plat_userconfig_parsing(rwnx_hw, (char *)dst, size);

	if (dst) {
        vfree(dst);
        dst = NULL;
    }

	printk("userconfig download complete\n\n");

	return 0;

}

/**
 * rwnx_plat_fmac_load() - Load FW code
 *
 * @rwnx_hw: Main driver data
 */
 #if 0
static int rwnx_plat_fmac_load(struct rwnx_hw *rwnx_hw, char *fw_path)
{
	int ret = 0;

	RWNX_DBG(RWNX_FN_ENTRY_STR);
	ret = rwnx_plat_userconfig_upload_android(rwnx_hw, fw_path, FW_USERCONFIG_NAME);
	return ret;
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
static void *rwnx_term_save_config(struct rwnx_plat *rwnx_plat)
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
		*reg_value++ = RWNX_REG_READ(rwnx_plat, RWNX_ADDR_SYSTEM,
									 *reg_list++);
	}

	return res;
}

#if 0
/**
 * rwmx_platform_restore_config() - Restore hardware config after reload
 *
 * @rwnx_plat: Pointer to platform data
 * @reg_value: Pointer of value to restore
 * (obtained with rwmx_platform_save_config())
 *
 * Restore configuration registers value.
 */
static void rwnx_term_restore_config(struct rwnx_plat *rwnx_plat,
									 u32 *reg_value)
{
	const u32 *reg_list;
	int i, size = 0;

	if (!reg_value || !rwnx_plat->get_config_reg)
		return;

	size = rwnx_plat->get_config_reg(rwnx_plat, &reg_list);

	for (i = 0; i < size; i++) {
		RWNX_REG_WRITE(*reg_value++, rwnx_plat, RWNX_ADDR_SYSTEM,
					   *reg_list++);
	}
}
#endif

#ifndef CONFIG_RWNX_FHOST
#if 0
static int rwnx_check_fw_compatibility(struct rwnx_hw *rwnx_hw)
{
	struct ipc_shared_env_tag *shared = rwnx_hw->ipc_env->shared;
	#ifdef CONFIG_RWNX_FULLMAC
	struct wiphy *wiphy = rwnx_hw->wiphy;
	#endif //CONFIG_RWNX_FULLMAC
	#ifdef CONFIG_RWNX_OLD_IPC
	int ipc_shared_version = 10;
	#else //CONFIG_RWNX_OLD_IPC
	int ipc_shared_version = 11;
	#endif //CONFIG_RWNX_OLD_IPC
	int res = 0;

	if (shared->comp_info.ipc_shared_version != ipc_shared_version) {
		wiphy_err(wiphy, "Different versions of IPC shared version between driver and FW (%d != %d)\n ",
				  ipc_shared_version, shared->comp_info.ipc_shared_version);
		res = -1;
	}

	if (shared->comp_info.radarbuf_cnt != IPC_RADARBUF_CNT) {
		wiphy_err(wiphy, "Different number of host buffers available for Radar events handling "\
				  "between driver and FW (%d != %d)\n", IPC_RADARBUF_CNT,
				  shared->comp_info.radarbuf_cnt);
		res = -1;
	}

	if (shared->comp_info.unsuprxvecbuf_cnt != IPC_UNSUPRXVECBUF_CNT) {
		wiphy_err(wiphy, "Different number of host buffers available for unsupported Rx vectors "\
				  "handling between driver and FW (%d != %d)\n", IPC_UNSUPRXVECBUF_CNT,
				  shared->comp_info.unsuprxvecbuf_cnt);
		res = -1;
	}

	#ifdef CONFIG_RWNX_FULLMAC
	if (shared->comp_info.rxdesc_cnt != IPC_RXDESC_CNT) {
		wiphy_err(wiphy, "Different number of shared descriptors available for Data RX handling "\
				  "between driver and FW (%d != %d)\n", IPC_RXDESC_CNT,
				  shared->comp_info.rxdesc_cnt);
		res = -1;
	}
	#endif /* CONFIG_RWNX_FULLMAC */

	if (shared->comp_info.rxbuf_cnt != IPC_RXBUF_CNT) {
		wiphy_err(wiphy, "Different number of host buffers available for Data Rx handling "\
				  "between driver and FW (%d != %d)\n", IPC_RXBUF_CNT,
				  shared->comp_info.rxbuf_cnt);
		res = -1;
	}

	if (shared->comp_info.msge2a_buf_cnt != IPC_MSGE2A_BUF_CNT) {
		wiphy_err(wiphy, "Different number of host buffers available for Emb->App MSGs "\
				  "sending between driver and FW (%d != %d)\n", IPC_MSGE2A_BUF_CNT,
				  shared->comp_info.msge2a_buf_cnt);
		res = -1;
	}

	if (shared->comp_info.dbgbuf_cnt != IPC_DBGBUF_CNT) {
		wiphy_err(wiphy, "Different number of host buffers available for debug messages "\
				  "sending between driver and FW (%d != %d)\n", IPC_DBGBUF_CNT,
				  shared->comp_info.dbgbuf_cnt);
		res = -1;
	}

	if (shared->comp_info.bk_txq != NX_TXDESC_CNT0) {
		wiphy_err(wiphy, "Driver and FW have different sizes of BK TX queue (%d != %d)\n",
				  NX_TXDESC_CNT0, shared->comp_info.bk_txq);
		res = -1;
	}

	if (shared->comp_info.be_txq != NX_TXDESC_CNT1) {
		wiphy_err(wiphy, "Driver and FW have different sizes of BE TX queue (%d != %d)\n",
				  NX_TXDESC_CNT1, shared->comp_info.be_txq);
		res = -1;
	}

	if (shared->comp_info.vi_txq != NX_TXDESC_CNT2) {
		wiphy_err(wiphy, "Driver and FW have different sizes of VI TX queue (%d != %d)\n",
				  NX_TXDESC_CNT2, shared->comp_info.vi_txq);
		res = -1;
	}

	if (shared->comp_info.vo_txq != NX_TXDESC_CNT3) {
		wiphy_err(wiphy, "Driver and FW have different sizes of VO TX queue (%d != %d)\n",
				  NX_TXDESC_CNT3, shared->comp_info.vo_txq);
		res = -1;
	}

	#if NX_TXQ_CNT == 5
	if (shared->comp_info.bcn_txq != NX_TXDESC_CNT4) {
		wiphy_err(wiphy, "Driver and FW have different sizes of BCN TX queue (%d != %d)\n",
				NX_TXDESC_CNT4, shared->comp_info.bcn_txq);
		res = -1;
	}
	#else
	if (shared->comp_info.bcn_txq > 0) {
		wiphy_err(wiphy, "BCMC enabled in firmware but disabled in driver\n");
		res = -1;
	}
	#endif /* NX_TXQ_CNT == 5 */

	if (shared->comp_info.ipc_shared_size != sizeof(ipc_shared_env)) {
		wiphy_err(wiphy, "Different sizes of IPC shared between driver and FW (%zd != %d)\n",
				  sizeof(ipc_shared_env), shared->comp_info.ipc_shared_size);
		res = -1;
	}

	if (shared->comp_info.msg_api != MSG_API_VER) {
		wiphy_warn(wiphy, "WARNING: Different supported message API versions between "\
				   "driver and FW (%d != %d)\n", MSG_API_VER, shared->comp_info.msg_api);
	}

	return res;
}
#endif
#endif /* !CONFIG_RWNX_FHOST */


void get_userconfig_txpwr_lvl_in_fdrv(txpwr_lvl_conf_t *txpwr_lvl)
{
    txpwr_lvl->enable           = userconfig_info.txpwr_lvl.enable;
    txpwr_lvl->dsss             = userconfig_info.txpwr_lvl.dsss;
    txpwr_lvl->ofdmlowrate_2g4  = userconfig_info.txpwr_lvl.ofdmlowrate_2g4;
    txpwr_lvl->ofdm64qam_2g4    = userconfig_info.txpwr_lvl.ofdm64qam_2g4;
    txpwr_lvl->ofdm256qam_2g4   = userconfig_info.txpwr_lvl.ofdm256qam_2g4;
    txpwr_lvl->ofdm1024qam_2g4  = userconfig_info.txpwr_lvl.ofdm1024qam_2g4;
    txpwr_lvl->ofdmlowrate_5g   = userconfig_info.txpwr_lvl.ofdmlowrate_5g;
    txpwr_lvl->ofdm64qam_5g     = userconfig_info.txpwr_lvl.ofdm64qam_5g;
    txpwr_lvl->ofdm256qam_5g    = userconfig_info.txpwr_lvl.ofdm256qam_5g;
    txpwr_lvl->ofdm1024qam_5g   = userconfig_info.txpwr_lvl.ofdm1024qam_5g;

    AICWFDBG(LOGINFO, "%s:enable:%d\r\n",          __func__, txpwr_lvl->enable);
    AICWFDBG(LOGINFO, "%s:dsss:%d\r\n",            __func__, txpwr_lvl->dsss);
    AICWFDBG(LOGINFO, "%s:ofdmlowrate_2g4:%d\r\n", __func__, txpwr_lvl->ofdmlowrate_2g4);
    AICWFDBG(LOGINFO, "%s:ofdm64qam_2g4:%d\r\n",   __func__, txpwr_lvl->ofdm64qam_2g4);
    AICWFDBG(LOGINFO, "%s:ofdm256qam_2g4:%d\r\n",  __func__, txpwr_lvl->ofdm256qam_2g4);
    AICWFDBG(LOGINFO, "%s:ofdm1024qam_2g4:%d\r\n", __func__, txpwr_lvl->ofdm1024qam_2g4);
    AICWFDBG(LOGINFO, "%s:ofdmlowrate_5g:%d\r\n",  __func__, txpwr_lvl->ofdmlowrate_5g);
    AICWFDBG(LOGINFO, "%s:ofdm64qam_5g:%d\r\n",    __func__, txpwr_lvl->ofdm64qam_5g);
    AICWFDBG(LOGINFO, "%s:ofdm256qam_5g:%d\r\n",   __func__, txpwr_lvl->ofdm256qam_5g);
    AICWFDBG(LOGINFO, "%s:ofdm1024qam_5g:%d\r\n",  __func__, txpwr_lvl->ofdm1024qam_5g);
}

void get_userconfig_txpwr_lvl_v2_in_fdrv(txpwr_lvl_conf_v2_t *txpwr_lvl_v2)
{
    *txpwr_lvl_v2 = userconfig_info.txpwr_lvl_v2;

    AICWFDBG(LOGINFO, "%s:enable:%d\r\n",               __func__, txpwr_lvl_v2->enable);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_1m_2g4:%d\r\n",  __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_2m_2g4:%d\r\n",  __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_5m5_2g4:%d\r\n", __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_11m_2g4:%d\r\n", __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_6m_2g4:%d\r\n",  __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_9m_2g4:%d\r\n",  __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_12m_2g4:%d\r\n", __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_18m_2g4:%d\r\n", __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_24m_2g4:%d\r\n", __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_36m_2g4:%d\r\n", __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_48m_2g4:%d\r\n", __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[10]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_54m_2g4:%d\r\n", __func__, txpwr_lvl_v2->pwrlvl_11b_11ag_2g4[11]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs0_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs1_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs2_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs3_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs4_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs5_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs6_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs7_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs8_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs9_2g4:%d\r\n",__func__, txpwr_lvl_v2->pwrlvl_11n_11ac_2g4[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs0_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs1_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs2_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs3_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs4_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs5_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs6_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs7_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs8_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs9_2g4:%d\r\n",    __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs10_2g4:%d\r\n",   __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[10]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs11_2g4:%d\r\n",   __func__, txpwr_lvl_v2->pwrlvl_11ax_2g4[11]);
}

void get_userconfig_txpwr_lvl_v3_in_fdrv(txpwr_lvl_conf_v3_t *txpwr_lvl_v3)
{
    *txpwr_lvl_v3 = userconfig_info.txpwr_lvl_v3;

    AICWFDBG(LOGINFO, "%s:enable:%d\r\n",               __func__, txpwr_lvl_v3->enable);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_1m_2g4:%d\r\n",  __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_2m_2g4:%d\r\n",  __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_5m5_2g4:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_11m_2g4:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_6m_2g4:%d\r\n",  __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_9m_2g4:%d\r\n",  __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_12m_2g4:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_18m_2g4:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_24m_2g4:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_36m_2g4:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_48m_2g4:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[10]);
    AICWFDBG(LOGINFO, "%s:lvl_11b_11ag_54m_2g4:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11b_11ag_2g4[11]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs0_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs1_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs2_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs3_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs4_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs5_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs6_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs7_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs8_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs9_2g4:%d\r\n",__func__, txpwr_lvl_v3->pwrlvl_11n_11ac_2g4[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs0_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs1_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs2_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs3_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs4_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs5_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs6_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs7_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs8_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs9_2g4:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs10_2g4:%d\r\n",   __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[10]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs11_2g4:%d\r\n",   __func__, txpwr_lvl_v3->pwrlvl_11ax_2g4[11]);

    AICWFDBG(LOGINFO, "%s:lvl_11a_1m_5g:%d\r\n",        __func__, txpwr_lvl_v3->pwrlvl_11a_5g[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_2m_5g:%d\r\n",        __func__, txpwr_lvl_v3->pwrlvl_11a_5g[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_5m5_5g:%d\r\n",       __func__, txpwr_lvl_v3->pwrlvl_11a_5g[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_11m_5g:%d\r\n",       __func__, txpwr_lvl_v3->pwrlvl_11a_5g[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_6m_5g:%d\r\n",        __func__, txpwr_lvl_v3->pwrlvl_11a_5g[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_9m_5g:%d\r\n",        __func__, txpwr_lvl_v3->pwrlvl_11a_5g[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_12m_5g:%d\r\n",       __func__, txpwr_lvl_v3->pwrlvl_11a_5g[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_18m_5g:%d\r\n",       __func__, txpwr_lvl_v3->pwrlvl_11a_5g[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_24m_5g:%d\r\n",       __func__, txpwr_lvl_v3->pwrlvl_11a_5g[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_36m_5g:%d\r\n",       __func__, txpwr_lvl_v3->pwrlvl_11a_5g[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_48m_5g:%d\r\n",       __func__, txpwr_lvl_v3->pwrlvl_11a_5g[10]);
    AICWFDBG(LOGINFO, "%s:lvl_11a_54m_5g:%d\r\n",       __func__, txpwr_lvl_v3->pwrlvl_11a_5g[11]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs0_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs1_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs2_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs3_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs4_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs5_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs6_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs7_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs8_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11n_11ac_mcs9_5g:%d\r\n", __func__, txpwr_lvl_v3->pwrlvl_11n_11ac_5g[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs0_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[0]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs1_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[1]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs2_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[2]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs3_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[3]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs4_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[4]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs5_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[5]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs6_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[6]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs7_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[7]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs8_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[8]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs9_5g:%d\r\n",     __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[9]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs10_5g:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[10]);
    AICWFDBG(LOGINFO, "%s:lvl_11ax_mcs11_5g:%d\r\n",    __func__, txpwr_lvl_v3->pwrlvl_11ax_5g[11]);
}

/**
 * rwnx_plat_userconfig_load  ---Load aic_userconfig.txt
 *@filename name of config
*/
static int rwnx_plat_userconfig_load(struct rwnx_hw *rwnx_hw) {

	if(rwnx_hw->sdiodev->chipid == PRODUCT_ID_AIC8801){
		
		rwnx_plat_userconfig_upload_android(rwnx_hw, aic_fw_path, FW_USERCONFIG_NAME);
	}else if(rwnx_hw->sdiodev->chipid == PRODUCT_ID_AIC8800DC){
		rwnx_plat_userconfig_load_8800dc(rwnx_hw);
	}else if(rwnx_hw->sdiodev->chipid == PRODUCT_ID_AIC8800DW){
        rwnx_plat_userconfig_load_8800dw(rwnx_hw);
    }else if(rwnx_hw->sdiodev->chipid == PRODUCT_ID_AIC8800D80){
		rwnx_plat_userconfig_load_8800d80(rwnx_hw);
	}
	return 0;
}

void get_userconfig_txpwr_loss(txpwr_loss_conf_t *txpwr_loss)
{
    txpwr_loss->loss_enable      = userconfig_info.txpwr_loss.loss_enable;
    txpwr_loss->loss_value       = userconfig_info.txpwr_loss.loss_value;

    AICWFDBG(LOGINFO, "%s:loss_enable:%d\r\n",     __func__, txpwr_loss->loss_enable);
    AICWFDBG(LOGINFO, "%s:loss_value:%d\r\n",      __func__, txpwr_loss->loss_value);
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
	int ret;
	struct rwnx_plat *rwnx_plat = rwnx_hw->plat;
	(void)ret;

	RWNX_DBG(RWNX_FN_ENTRY_STR);

	if (rwnx_plat->enabled)
		return 0;

	#ifndef CONFIG_ROM_PATCH_EN
	#ifdef CONFIG_DOWNLOAD_FW
	ret = rwnx_plat_fmac_load(rwnx_hw, (char*)config);
	if (ret)
		return ret;
	#endif /* !CONFIG_ROM_PATCH_EN */
	#endif

#if 0
		ret = rwnx_plat_patch_load(rwnx_hw);
		if (ret) {
			return ret;
		}
#endif


	rwnx_plat_userconfig_load(rwnx_hw);

	//rwnx_plat->enabled = true;

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
	tasklet_kill(&rwnx_hw->task);
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

	rwnx_plat->enabled = false;
	g_rwnx_plat = rwnx_plat;

#if defined CONFIG_RWNX_FULLMAC
	return rwnx_cfg80211_init(rwnx_plat, platform_data);
#elif defined CONFIG_RWNX_FHOST
	return rwnx_fhost_init(rwnx_plat, platform_data);
#endif
}

/**
 * rwnx_platform_deinit() - Deinitialize the platform
 *
 * @rwnx_hw: main driver data
 *
 * Called by the platform driver after it is removed
 */
void rwnx_platform_deinit(struct rwnx_hw *rwnx_hw)
{
	RWNX_DBG(RWNX_FN_ENTRY_STR);

#if defined CONFIG_RWNX_FULLMAC
	rwnx_cfg80211_deinit(rwnx_hw);
#elif defined CONFIG_RWNX_FHOST
	rwnx_fhost_deinit(rwnx_hw);
#endif
}

/**
 * rwnx_platform_register_drv() - Register all possible platform drivers
 */
int rwnx_platform_register_drv(void)
{
	return rwnx_pci_register_drv();
}


/**
 * rwnx_platform_unregister_drv() - Unegister all platform drivers
 */
void rwnx_platform_unregister_drv(void)
{
	return rwnx_pci_unregister_drv();
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


