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

#ifdef ECLIPSE
#include <ssv_mod_conf.h>
#endif // ECLIPSE
#include <linux/version.h>
#include <linux/platform_device.h>

#include <linux/string.h>
#include <ssv_chip_id.h>
#include <ssv6200.h>
#include <smac/dev.h>
#include <hal.h>
#include <smac/ssv_skb.h>
#include <hci/hctrl.h>

//include firmware binary header
#include <include/ssv6x5x-sw.h>
static void ssv6xxx_cmd_cali(struct ssv_hw *sh, int argc, char *argv[])
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_cmd_rc(struct ssv_hw *sh, int argc, char *argv[])
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_cmd_efuse(struct ssv_hw *sh, int argc, char *argv[])
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_set_sifs(struct ssv_hw *sh, int band)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_cmd_hwinfo(struct ssv_hw *sh, int argc, char *argv[])
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}
static int ssv6xxx_get_tkip_mmic_err(struct sk_buff *skb)
{
    return 0;
}

static void ssv6xxx_cmd_txgen(struct ssv_hw *sh, u8 drate)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_cmd_rf(struct ssv_hw *sh, int argc, char *argv[])
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_cmd_hwq_limit(struct ssv_hw *sh, int argc, char *argv[])
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_init_gpio_cfg(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}    

static void ssv6xxx_flash_read_all_map(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}    

static void ssv6xxx_write_efuse(struct ssv_hw *sh, u8 *data, u8 data_length)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_update_rf_table(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}    

void ssv6xxx_set_on3_enable(struct ssv_hw *sh, bool val)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}    

static int ssv6xxx_init_hci_rx_aggr(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
    return -1;
}

static int ssv6xxx_reset_hw_mac(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
    return -1;
}

static void ssv6xxx_set_crystal_clk(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_wait_usb_rom_ready(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_detach_usb_hci(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_pll_chk(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
}

static bool ssv6xxx_put_mic_space_for_hw_ccmp_encrypt(struct ssv_softc *sc, struct sk_buff *skb) 
{

    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__); 
    return false;
}

#ifdef CONFIG_PM
static void ssv6xxx_save_clear_trap_reason(struct ssv_softc *sc)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__); 

}

static void ssv6xxx_restore_trap_reason(struct ssv_softc *sc)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__); 

}

static void ssv6xxx_ps_save_reset_rx_flow(struct ssv_softc *sc)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__); 

}

static void ssv6xxx_ps_restore_rx_flow(struct ssv_softc *sc)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__); 
}

static void ssv6xxx_pmu_awake(struct ssv_softc *sc)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__); 
}

static void ssv6xxx_ps_hold_on3(struct ssv_softc *sc, int value)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__); 
}
#endif

static int ssv6xxx_get_sram_mode(struct ssv_hw *sh)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, 
	    "%s is not supported for this model!!\n",__func__);
    return 0;
}

static void ssv6xxx_rc_mac80211_tx_rate_idx(struct ssv_softc *sc, int hw_rate_idx, struct ieee80211_tx_info *tx_info)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL,
        "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_update_mac80211_chan_info(struct ssv_softc *sc, 
            struct sk_buff *skb, struct ieee80211_rx_status *rxs)
{
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL,
        "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_read_allid_map(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL,
        "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_read_txid_map(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL,
        "%s is not supported for this model!!\n",__func__);
}

static void ssv6xxx_read_rxid_map(struct ssv_hw *sh, u32 *id0, u32 *id1, u32 *id2, u32 *id3)
{
    struct ssv_softc *sc = sh->sc;
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL,
        "%s is not supported for this model!!\n",__func__);
}

static void *ssv6xxx_open_firmware(char *user_mainfw)
{
    struct file *fp;

    fp = filp_open(user_mainfw, O_RDONLY, 0);
    if (IS_ERR(fp)) 
        fp = NULL;

    return fp;
}

static int ssv6xxx_read_fw_block(char *buf, int len, void *image)
{
    struct file *fp = (struct file *)image;
    int rdlen;

    if (!image)
        return 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
    rdlen = kernel_read(fp, buf, len, &fp->f_pos);
#else    
    rdlen = kernel_read(fp, fp->f_pos, buf, len);
    if (rdlen > 0)
        fp->f_pos += rdlen;
#endif    

    return rdlen;
}

static void ssv6xxx_close_firmware(void *image)
{
    if (image)
        filp_close((struct file *)image, NULL);
}

static u32 _ssv6xxx_get_sram_mapping_address(bool need_chk, u32 sram_addr)
{
#define REMAPPING_ADDR_THRESHOLD    (0x00010000)
#define EXTRA_MAPPING_ADDR          (0x00100000-REMAPPING_ADDR_THRESHOLD)
    if((true == need_chk) && (REMAPPING_ADDR_THRESHOLD <= sram_addr))
    {
        return sram_addr+EXTRA_MAPPING_ADDR;
    }
    return sram_addr;
}

static int _ssv6xxx_load_firmware(struct ssv_hw *sh, u8 *firmware_name, u8 openfile)
{
    int ret = 0;
    u8   *fw_buffer = NULL;
    u32   sram_addr = FW_START_SRAM_ADDR;
    u32   block_count = 0;
    u32   res_size=0, len=0, tolen=0;
    void *fw_fp = NULL;
    u8    interface = HCI_DEVICE_TYPE(sh->hci.hci_ctrl);
#ifdef ENABLE_FW_SELF_CHECK
    u32   checksum = FW_CHECKSUM_INIT;
    u32   fw_checksum, fw_clkcnt;
    u32   retry_count = 3;
    u32  *fw_data32;
#else
    int   writesize = 0;
    u32   retry_count = 1;
#endif
    u32   word_count, i;
    u32   fw_res_size = ssv6x5x_sw_bin_len;
    u32   rsize = 0;
    bool need_chk_map = false;
    struct ssv_softc *sc = sh->sc;

    //if (hci_ctrl->redownload == 1) {
    /* force mcu jump to rom for always*/
    // Redownload firmware
    dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "Re-download FW\n");
    if (interface == SSV_HWIF_INTERFACE_USB)
        HAL_JUMP_TO_ROM(sh->sc);
    //}

	/* Get SRAM mapping mode:
	 * 0: ILM  64KB, DLM 128KB 
	 * 1: ILM 160KB, DLM 32KB
	 */
    if(HAL_GET_SRAM_MODE(sh) == 0)
    {
        need_chk_map = true;
    }
    else
    {
        need_chk_map = false;
    }

    // Load firmware
    if(openfile)
    {
        fw_fp = ssv6xxx_open_firmware(firmware_name);
        if (!fw_fp) {
            dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "failed to find firmware (%s)\n", firmware_name);
            ret = -1;
            goto out;
        }
    }
    else
    {
        fw_fp = (void *)ssv6x5x_sw_bin;
    }

    // Allocate buffer firmware aligned with FW_BLOCK_SIZE and padding with 0xA5 in empty space.
    fw_buffer = (u8 *)kzalloc(FW_BLOCK_SIZE, GFP_KERNEL);
    if (fw_buffer == NULL) {
        dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "Failed to allocate buffer for firmware.\n");
        goto out;
    }

    do {
#ifdef ENABLE_FW_SELF_CHECK
        checksum = FW_CHECKSUM_INIT;
        tolen = 0;

        if (openfile)
            ((struct file *)fw_fp)->f_pos = 0;
#endif

        // Disable MCU (USB ROM code must be alive for downloading FW, so USB doesn't do it)
        if (!(interface == SSV_HWIF_INTERFACE_USB)) {
            ret = HAL_LOAD_FW_DISABLE_MCU(sh);
            if (ret == -1)
                goto out;
        }

        // Write firmware to SRAM address 0
        dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "Writing firmware to SSV6XXX...\n");
        memset(fw_buffer, 0xA5, CHECKSUM_BLOCK_SIZE);
        
        while(fw_res_size)
        {
            rsize = (fw_res_size>CHECKSUM_BLOCK_SIZE)?CHECKSUM_BLOCK_SIZE:fw_res_size;
            if(openfile)
            {
                if(!(len = ssv6xxx_read_fw_block((char*)fw_buffer, rsize, fw_fp)))
                {
                    break;
                }
            }
            else
            {
                if(0 == fw_res_size)
                {
                    break;
                }

                len = rsize;
                memcpy((void *)fw_buffer, (const void *)fw_fp, len);
                fw_fp += CHECKSUM_BLOCK_SIZE;
                fw_res_size -= len;
            }
            tolen += len;
            //printk("read len=%d,sram_addr=%d\n",len,sram_addr);
            //if (len < CHECKSUM_BLOCK_SIZE) {
              //  res_size = len;
               // break;
            //}
            fw_data32 = (u32 *)fw_buffer;
            //Add to avoid USB load code with turismo C0/D0 failed, skip 0x10000~0x10aff.
            if((SSV_HWIF_INTERFACE_USB == interface) &&
                (0x10000 <= sram_addr) && (0x10b00 > sram_addr))
            {
                if(0x10b00 < (sram_addr+len))
                {
                    if ((ret = SMAC_LOAD_FW(sh, _ssv6xxx_get_sram_mapping_address(need_chk_map, 0x10b00), (u8 *)fw_buffer+(0x10b00-sram_addr), (sram_addr+len-0x10b00))) != 0)
                    {
                        break;
                    }
                    word_count = (CHECKSUM_BLOCK_SIZE / sizeof(u32));
                    for (i = ((0x10b00-sram_addr)/sizeof(u32)); i < word_count; i++) {
                        checksum += fw_data32[i];
                    }
                }
            }
            else
            {
                if ((ret = SMAC_LOAD_FW(sh, _ssv6xxx_get_sram_mapping_address(need_chk_map, sram_addr), (u8 *)fw_buffer, CHECKSUM_BLOCK_SIZE)) != 0)
                {
                    break;
                }
                word_count = (CHECKSUM_BLOCK_SIZE / sizeof(u32));
                for (i = 0; i < word_count; i++) {
                    checksum += fw_data32[i];
                }
            }
            sram_addr += CHECKSUM_BLOCK_SIZE;
            //printk("\nper blk cks=%x\n",checksum);
            //memset(fw_buffer, 0xA5, CHECKSUM_BLOCK_SIZE);
        }

        // Calculate the final checksum.
        checksum = ((checksum >> 24) + (checksum >> 16) + (checksum >> 8) + checksum) & 0x0FF;
        checksum <<= 16;

        if (ret == 0) {
            // Reset CPU for USB switching ROM to firmware
            if (interface == SSV_HWIF_INTERFACE_USB) {
                ret = HAL_RESET_CPU(sh);
                if (ret == -1)
                    goto out;
            }
            // Set ILM/DLM
            HAL_SET_SRAM_MODE(sh, SRAM_MODE_ILM_160K_DLM_32K);

            block_count = tolen / CHECKSUM_BLOCK_SIZE;
            res_size = tolen % CHECKSUM_BLOCK_SIZE;
            if(res_size)
                block_count++;
            // Inform FW that how many blocks is downloaded such that FW can calculate the checksum.
            HAL_LOAD_FW_SET_STATUS(sh, (block_count << 16));
            HAL_LOAD_FW_GET_STATUS(sh, &fw_clkcnt);
            dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "(block_count << 16) = %x,reg =%x\n", (block_count << 16),fw_clkcnt);
            // Release reset to let CPU run.
            HAL_LOAD_FW_ENABLE_MCU(sh);
            dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "Firmware \"%s\" loaded\n", firmware_name);
            // Wait FW to calculate checksum.
            msleep(50);
            // Check checksum result and set to complement value if checksum is OK.
            HAL_LOAD_FW_GET_STATUS(sh, &fw_checksum);
            fw_checksum = fw_checksum & FW_STATUS_MASK;
            if (fw_checksum == checksum) {
                HAL_LOAD_FW_SET_STATUS(sh, (~checksum & FW_STATUS_MASK));
                ret = 0;
                dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "Firmware check OK.%04x = %04x\n", fw_checksum, checksum);
                break;
            } else {
                dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "FW checksum error: %04x != %04x\n", fw_checksum, checksum);
                ret = -1;
            }

        } else {
            dbgprint(&sc->cmd_data, sc->log_ctrl, LOG_HAL, "Firmware \"%s\" download failed. (%d)\n", firmware_name, ret);
            ret = -1;
        }
    } while (--retry_count);

    if (ret)
        goto out;

        // hci_ctrl->redownload = 1;

    ret = 0;

out:
    if(openfile)
    {
        if(fw_fp)
        {
            ssv6xxx_close_firmware(fw_fp);
        }
    }

    if (fw_buffer != NULL)
    {
        kfree(fw_buffer);
    }

    return ret;
}

static int ssv6xxx_hci_load_firmware_openfile(struct ssv_hw *sh, u8 *firmware_name)
{
    return _ssv6xxx_load_firmware(sh, firmware_name, 1);
}

#if 0 //Keep it, but not used.
static int ssv6xxx_hci_get_firmware(struct device *dev, char *user_mainfw, const struct firmware **mainfw)
{
    int ret;

    //hBUG_ON(helper == NULL);
    BUG_ON(mainfw == NULL);

    /* Try user-specified firmware first */
    if (*user_mainfw) {
        ret = request_firmware(mainfw, user_mainfw, dev);
        if (ret) {
            goto fail;
        }
        if (*mainfw)
            return 0;
    }

fail:
    /* Failed */
    if (*mainfw) {
        release_firmware(*mainfw);
        *mainfw = NULL;
    }

    return -ENOENT;
}

static int ssv6xxx_hci_load_firmware_request(struct ssv6xxx_hci_ctrl *hci_ctrl, u8 *firmware_name)
{
    int ret = 0;
    const struct firmware *ssv6xxx_fw = NULL;
    u8   *fw_buffer = NULL;
    u32   sram_addr = FW_START_SRAM_ADDR;
    u32   block_count = 0;
    u32   block_idx = 0;
    u32   res_size;
    u8   *fw_data;
    u8    interface = HCI_DEVICE_TYPE(hci_ctrl);
#ifdef ENABLE_FW_SELF_CHECK
    u32   checksum = FW_CHECKSUM_INIT;
    u32   fw_checksum;
    u32   retry_count = 3;
    u32  *fw_data32;
#else
    int   writesize = 0;
    u32   retry_count = 1;
#endif

    //if (hci_ctrl->redownload == 1) {
    /* force mcu jump to rom for always*/
    // Redownload firmware    
    HCI_DBG_PRINT(hci_ctrl, "Re-download FW\n");
    HCI_JUMP_TO_ROM(hci_ctrl);    
    //}

    // Load firmware
    ret = ssv6xxx_hci_get_firmware(hci_ctrl->shi->dev, firmware_name, &ssv6xxx_fw);
    if (ret) {
        HCI_DBG_PRINT(hci_ctrl, "failed to find firmware (%d)\n", ret);
        goto out;
    }

    // Allocate buffer firmware aligned with FW_BLOCK_SIZE and padding with 0xA5 in empty space.
    fw_buffer = (u8 *)kzalloc(FW_BLOCK_SIZE, GFP_KERNEL);
    if (fw_buffer == NULL) {
        HCI_DBG_PRINT(hci_ctrl, "Failed to allocate buffer for firmware.\n");
        goto out;
    }

#ifdef ENABLE_FW_SELF_CHECK
    block_count = ssv6xxx_fw->size / CHECKSUM_BLOCK_SIZE;
    res_size = ssv6xxx_fw->size % CHECKSUM_BLOCK_SIZE;
    {
        int word_count = (int)(block_count * CHECKSUM_BLOCK_SIZE / sizeof(u32));
        int i;
        fw_data32 = (u32 *)ssv6xxx_fw->data;
        for (i = 0; i < word_count; i++)
            checksum += fw_data32[i];

        if (res_size) {
            memset(fw_buffer, 0xA5, CHECKSUM_BLOCK_SIZE);
            memcpy(fw_buffer, &ssv6xxx_fw->data[block_count * CHECKSUM_BLOCK_SIZE], res_size);

            // Accumulate checksum for the incomplete block
            word_count = (int)(CHECKSUM_BLOCK_SIZE / sizeof(u32));
            fw_data32 = (u32 *)fw_buffer;
            for (i = 0; i < word_count; i++) {
                checksum += fw_data32[i];
            }
        }
    }

    // Calculate the final checksum.
    checksum = ((checksum >> 24) + (checksum >> 16) + (checksum >> 8) + checksum) & 0x0FF;
    checksum <<= 16;
#endif // ENABLE_FW_SELF_CHECK

    do {                
        // Disable MCU (USB ROM code must be alive for downloading FW, so USB doesn't do it)        
        if (!(interface == SSV_HWIF_INTERFACE_USB)) {
            ret = SSV_LOAD_FW_DISABLE_MCU(hci_ctrl);
            if (ret == -1)
                goto out;
        }

        // Write firmware to SRAM address 0
#ifdef ENABLE_FW_SELF_CHECK
        block_count = ssv6xxx_fw->size / FW_BLOCK_SIZE;
        res_size = ssv6xxx_fw->size % FW_BLOCK_SIZE;

        HCI_DBG_PRINT(hci_ctrl, "Writing %d blocks to SSV6XXX...", block_count);
        for (block_idx = 0, fw_data = (u8 *)ssv6xxx_fw->data, sram_addr = 0;block_idx < block_count;
                block_idx++, fw_data += FW_BLOCK_SIZE, sram_addr += FW_BLOCK_SIZE) {
            memcpy(fw_buffer, fw_data, FW_BLOCK_SIZE);
            if ((ret = HCI_LOAD_FW(hci_ctrl, sram_addr, (u8 *)fw_buffer, FW_BLOCK_SIZE)) != 0)
                break;
        }

        if(res_size) {
            memset(fw_buffer, 0xA5, FW_BLOCK_SIZE);
            memcpy(fw_buffer, &ssv6xxx_fw->data[block_count * FW_BLOCK_SIZE], res_size);
            if ((ret = HCI_LOAD_FW(hci_ctrl, sram_addr, (u8 *)fw_buffer,
                            ((res_size/CHECKSUM_BLOCK_SIZE)+1)*CHECKSUM_BLOCK_SIZE)) != 0)
                break;
        }
#else // ENABLE_FW_SELF_CHECK
        block_count = ssv6xxx_fw->size / FW_BLOCK_SIZE;
        res_size = ssv6xxx_fw->size % FW_BLOCK_SIZE;
        writesize = sdio_align_size(func,res_size);

        HCI_DBG_PRINT(hci_ctrl, "Writing %d blocks to SSV6XXX...", block_count);
        for (block_idx = 0, fw_data = (u8 *)ssv6xxx_fw->data, sram_addr = 0;block_idx < block_count;
                block_idx++, fw_data += FW_BLOCK_SIZE, sram_addr += FW_BLOCK_SIZE) {
            memcpy(fw_buffer, fw_data, FW_BLOCK_SIZE);
            if ((ret = HCI_LOAD_FW(hci_ctrl, sram_addr, (u8 *)fw_buffer, FW_BLOCK_SIZE)) != 0)
                break;
        }
        if(res_size) {
            memcpy(fw_buffer, &ssv6xxx_fw->data[block_count * FW_BLOCK_SIZE], res_size);
            if ((ret = HCI_LOAD_FW(hci_ctrl, sram_addr, (u8 *)fw_buffer, writesize)) != 0)
                break;
        }
#endif // ENABLE_FW_SELF_CHECK

        if (ret == 0) {
            // Reset CPU for USB switching ROM to firmware
            if (interface == SSV_HWIF_INTERFACE_USB) {
                ret = SSV_RESET_CPU(hci_ctrl);
                if (ret == -1)
                    goto out;
            }

#ifdef ENABLE_FW_SELF_CHECK
            block_count = ssv6xxx_fw->size / CHECKSUM_BLOCK_SIZE;
            res_size = ssv6xxx_fw->size % CHECKSUM_BLOCK_SIZE;
            if(res_size)
                block_count++;
            // Inform FW that how many blocks is downloaded such that FW can calculate the checksum.
            SSV_LOAD_FW_SET_STATUS(hci_ctrl, (block_count << 16));
#endif // ENABLE_FW_SELF_CHECK

            // Release reset to let CPU run.
            SSV_LOAD_FW_ENABLE_MCU(hci_ctrl);
            HCI_DBG_PRINT(hci_ctrl, "Firmware \"%s\" loaded\n", firmware_name);
#ifdef ENABLE_FW_SELF_CHECK
            // Wait FW to calculate checksum.
            msleep(50);
            // Check checksum result and set to complement value if checksum is OK.
            SSV_LOAD_FW_GET_STATUS(hci_ctrl, &fw_checksum);
            fw_checksum = fw_checksum & FW_STATUS_MASK;
            if (fw_checksum == checksum) {
                SSV_LOAD_FW_SET_STATUS(hci_ctrl, (~checksum & FW_STATUS_MASK));
                ret = 0;
                HCI_DBG_PRINT(hci_ctrl, "Firmware check OK.\n");
                break;
            } else {
                HCI_DBG_PRINT(hci_ctrl, "FW checksum error: %04x != %04x\n", fw_checksum, checksum);
                ret = -1;
            }
#endif
        } else {
            HCI_DBG_PRINT(hci_ctrl, "Firmware \"%s\" download failed. (%d)\n", firmware_name, ret);
            ret = -1;
        }
    } while (--retry_count);

    if (ret)
        goto out;

    // hci_ctrl->redownload = 1;

    ret = 0;    

out:
    if (ssv6xxx_fw)
        release_firmware(ssv6xxx_fw);

    if (fw_buffer != NULL)
        kfree(fw_buffer);

    return ret;
}
#endif

static int ssv6xxx_hci_load_firmware_fromheader(struct ssv_hw *sh, u8 *firmware_name)
{
    return _ssv6xxx_load_firmware(sh, firmware_name, 0);
}

static int ssv6xxx_load_fw(struct ssv_hw *sh, u8 *firmware_name, u8 openfile)
{
    int ret = 0;

    HAL_LOAD_FW_PRE_CONFIG_DEVICE(sh);

    if (openfile)
        ret = ssv6xxx_hci_load_firmware_openfile(sh, firmware_name);
    else
        ret = ssv6xxx_hci_load_firmware_fromheader(sh, firmware_name);
#if 0 //Keep it, but not used.
    ret = ssv6xxx_hci_load_firmware_request(hci_ctrl, firmware_name);
#endif

    // Sleep to let SSV6XXX get ready.
    msleep(50);

    if (ret == 0)
        HAL_LOAD_FW_POST_CONFIG_DEVICE(sh);

    return ret;
}
static void ssv6xxx_attach_common_hal (struct ssv_hal_ops  *hal_ops)
{
    hal_ops->cmd_cali = ssv6xxx_cmd_cali;
    hal_ops->cmd_rc = ssv6xxx_cmd_rc;
    hal_ops->cmd_efuse = ssv6xxx_cmd_efuse;
    hal_ops->set_sifs = ssv6xxx_set_sifs;
	hal_ops->cmd_hwinfo = ssv6xxx_cmd_hwinfo;
	hal_ops->get_tkip_mmic_err = ssv6xxx_get_tkip_mmic_err;
	hal_ops->cmd_txgen = ssv6xxx_cmd_txgen;
	hal_ops->cmd_rf = ssv6xxx_cmd_rf;
	hal_ops->cmd_hwq_limit = ssv6xxx_cmd_hwq_limit;
    hal_ops->init_gpio_cfg = ssv6xxx_init_gpio_cfg;
    hal_ops->flash_read_all_map = ssv6xxx_flash_read_all_map;
    hal_ops->write_efuse = ssv6xxx_write_efuse;
    hal_ops->update_rf_table = ssv6xxx_update_rf_table;
    hal_ops->set_on3_enable = ssv6xxx_set_on3_enable;
    hal_ops->init_hci_rx_aggr = ssv6xxx_init_hci_rx_aggr;
    hal_ops->reset_hw_mac = ssv6xxx_reset_hw_mac;
    hal_ops->set_crystal_clk = ssv6xxx_set_crystal_clk;
    hal_ops->wait_usb_rom_ready = ssv6xxx_wait_usb_rom_ready;
    hal_ops->detach_usb_hci = ssv6xxx_detach_usb_hci;
    hal_ops->pll_chk = ssv6xxx_pll_chk;
    hal_ops->put_mic_space_for_hw_ccmp_encrypt = ssv6xxx_put_mic_space_for_hw_ccmp_encrypt;
#ifdef CONFIG_PM
	hal_ops->save_clear_trap_reason = ssv6xxx_save_clear_trap_reason;
	hal_ops->restore_trap_reason = ssv6xxx_restore_trap_reason;
	hal_ops->ps_save_reset_rx_flow = ssv6xxx_ps_save_reset_rx_flow;
	hal_ops->ps_restore_rx_flow = ssv6xxx_ps_restore_rx_flow;
	hal_ops->pmu_awake = ssv6xxx_pmu_awake;
	hal_ops->ps_hold_on3 = ssv6xxx_ps_hold_on3;
#endif
    hal_ops->get_sram_mode = ssv6xxx_get_sram_mode;
    hal_ops->rc_mac80211_tx_rate_idx = ssv6xxx_rc_mac80211_tx_rate_idx;
    hal_ops->update_mac80211_chan_info = ssv6xxx_update_mac80211_chan_info;
    hal_ops->read_allid_map = ssv6xxx_read_allid_map;
    hal_ops->read_txid_map = ssv6xxx_read_txid_map;
    hal_ops->read_rxid_map = ssv6xxx_read_rxid_map;
    hal_ops->load_fw = ssv6xxx_load_fw;
}


int ssv6xxx_init_hal(struct ssv_softc *sc)
{
    struct ssv_hw *sh;
    int ret = 0;
    struct ssv_hal_ops *hal_ops = NULL;
    extern void ssv_attach_ssv6006(struct ssv_softc *sc, struct ssv_hal_ops *hal_ops);
    bool chip_supportted = false;
    struct ssv6xxx_platform_data *priv = sc->dev->platform_data;

	// alloc hal_ops memory
	hal_ops = kzalloc(sizeof(struct ssv_hal_ops), GFP_KERNEL);
	if (hal_ops == NULL) {
		printk("%s(): Fail to alloc hal_ops\n", __FUNCTION__);
		return -ENOMEM;
	}

    // load common HAL layer function;
    ssv6xxx_attach_common_hal(hal_ops);

#ifdef SSV_SUPPORT_SSV6006
    if (   strstr(priv->chip_id, SSV6006) 
        	 || strstr(priv->chip_id, SSV6006C)
        	 || strstr(priv->chip_id, SSV6006D)) {
        
        printk(KERN_INFO"%s\n", sc->platform_dev->id_entry->name);
        printk(KERN_INFO"Attach SSV6006 family HAL function  \n");

        ssv_attach_ssv6006(sc, hal_ops);
        chip_supportted = true;
    }
#endif    
    if (!chip_supportted) {

        printk(KERN_ERR "Chip \"%s\" is not supported by this driver\n", sc->platform_dev->id_entry->name);
        ret = -EINVAL;
        goto out;
    }

    sh = hal_ops->alloc_hw();
    if (sh == NULL) {
        ret = -ENOMEM;
        goto out;
    }

    memcpy(&sh->hal_ops, hal_ops, sizeof(struct ssv_hal_ops));
    sc->sh = sh;
    sh->sc = sc;
    sh->priv = sc->dev->platform_data;
    sh->hci.dev = sc->dev;
    sh->hci.if_ops = sh->priv->ops;
    sh->hci.skb_alloc = ssv_skb_alloc;
    sh->hci.skb_free = ssv_skb_free;
    sh->hci.hci_rx_cb = ssv6200_rx;
    sh->hci.hci_is_rx_q_full = ssv6200_is_rx_q_full;

    sh->priv->skb_alloc = ssv_skb_alloc_ex;
    sh->priv->skb_free = ssv_skb_free;
    sh->priv->skb_param = sc;

    // Set jump to rom functions for HWIF
    sh->priv->enable_usb_acc = ssv6xxx_enable_usb_acc;    
    sh->priv->disable_usb_acc = ssv6xxx_disable_usb_acc;
    sh->priv->jump_to_rom = ssv6xxx_jump_to_rom;
    sh->priv->usb_param = sc;
    
    sh->priv->rx_mode = ssv6xxx_rx_mode;
    sh->priv->rx_mode_param = sc;

    sh->hci.sc = sc;
    sh->hci.sh = sh;

out:
	kfree(hal_ops);
    return ret;
}

