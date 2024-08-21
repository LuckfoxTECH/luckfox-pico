#include "aic_txrxif.h"
#include "aicwf_usb.h"
#include "aicbluetooth.h"
#include "aic_compat_8800d80.h"
#include "aicwf_debug.h"
extern int ble_scan_wakeup_reboot_time;
extern uint32_t ad_data_filter_mask;
extern uint32_t gpio_num;//default select gpiob2 for fw_wakeup_host
extern uint32_t gpio_dft_lvl;//0:defalut pull down,  1:default pull up

int rwnx_plat_bin_fw_upload_2(struct aic_usb_dev *usbdev, u32 fw_addr,
                               char *filename);
int rwnx_request_firmware_common(struct aic_usb_dev *usbdev,
	u32** buffer, const char *filename);
void rwnx_plat_userconfig_parsing(char *buffer, int size);
void rwnx_release_firmware_common(u32** buffer);

extern int testmode;
extern int chip_id;
u8 chip_mcu_id = 0;

typedef u32 (*array2_tbl_t)[2];

#define AIC_PATCH_MAGIG_NUM     0x48435450 // "PTCH"
#define AIC_PATCH_MAGIG_NUM_2   0x50544348 // "HCTP"
#define AIC_PATCH_BLOCK_MAX     4

typedef struct {
    uint32_t magic_num;
    uint32_t pair_start;
    uint32_t magic_num_2;
    uint32_t pair_count;
    uint32_t block_dst[AIC_PATCH_BLOCK_MAX];
    uint32_t block_src[AIC_PATCH_BLOCK_MAX];
    uint32_t block_size[AIC_PATCH_BLOCK_MAX]; // word count
} aic_patch_t;


#define AIC_PATCH_OFST(mem) ((size_t) &((aic_patch_t *)0)->mem)
#define AIC_PATCH_ADDR(mem) ((u32) (aic_patch_str_base + AIC_PATCH_OFST(mem)))

u32 patch_tbl_d80[][2] =
{
    #ifdef USE_5G
    {0x00b4, 0xf3010001},
    #else
    {0x00b4, 0xf3010000},
    #endif
    {0x0170, 0x00000001},//rx aggr counter
};

//adap test
u32 adaptivity_patch_tbl_d80[][2] = {
    {0x000C, 0x0000320A}, //linkloss_thd
    {0x009C, 0x00000000}, //ac_param_conf
    {0x0168, 0x00010000}, //tx_adaptivity_en
};

u32 syscfg_tbl_masked_8800d80[][3] = {
};

u32 syscfg_tbl_8800d80[][2] = {
    #ifdef CONFIG_PMIC_SETTING
    {0x70001408, 0x00000000}, // stop wdg
    #endif /* CONFIG_PMIC_SETTING */
};

extern int adap_test;

int aicwf_patch_config_8800d80(struct aic_usb_dev *usb_dev)
{
    u32 rd_patch_addr;
    u32 aic_patch_addr;
    u32 config_base, aic_patch_str_base;
    uint32_t start_addr = 0x001D7000;
    u32 patch_addr = start_addr;
    u32 patch_cnt = sizeof(patch_tbl_d80) / 4 / 2;
    struct dbg_mem_read_cfm rd_patch_addr_cfm;
    int ret = 0;
    int cnt = 0;
    //adap test
    int adap_patch_cnt = 0;

    if (adap_test) {
        AICWFDBG(LOGINFO, "%s adap test \r\n", __func__);
        adap_patch_cnt = sizeof(adaptivity_patch_tbl_d80)/sizeof(u32)/2;
    }

    if (chip_id == CHIP_REV_U01) {
        rd_patch_addr = RAM_FMAC_FW_ADDR_8800D80 + 0x0198;
    } else {
        rd_patch_addr = RAM_FMAC_FW_ADDR_8800D80_U02 + 0x0198;
    }
    aic_patch_addr = rd_patch_addr + 8;

    AICWFDBG(LOGERROR, "Read FW mem: %08x\n", rd_patch_addr);
    if ((ret = rwnx_send_dbg_mem_read_req(usb_dev, rd_patch_addr, &rd_patch_addr_cfm))) {
        AICWFDBG(LOGERROR, "setting base[0x%x] rd fail: %d\n", rd_patch_addr, ret);
        return ret;
    }
    AICWFDBG(LOGERROR, "%x=%x\n", rd_patch_addr_cfm.memaddr, rd_patch_addr_cfm.memdata);
    config_base = rd_patch_addr_cfm.memdata;

    if ((ret = rwnx_send_dbg_mem_read_req(usb_dev, aic_patch_addr, &rd_patch_addr_cfm))) {
        AICWFDBG(LOGERROR, "patch_str_base[0x%x] rd fail: %d\n", aic_patch_addr, ret);
        return ret;
    }
    AICWFDBG(LOGERROR, "%x=%x\n", rd_patch_addr_cfm.memaddr, rd_patch_addr_cfm.memdata);
    aic_patch_str_base = rd_patch_addr_cfm.memdata;

    if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, AIC_PATCH_ADDR(magic_num), AIC_PATCH_MAGIG_NUM))) {
        AICWFDBG(LOGERROR, "maigic_num[0x%x] write fail: %d\n", AIC_PATCH_ADDR(magic_num), ret);
        return ret;
    }

    if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, AIC_PATCH_ADDR(magic_num_2), AIC_PATCH_MAGIG_NUM_2))) {
        AICWFDBG(LOGERROR, "maigic_num[0x%x] write fail: %d\n", AIC_PATCH_ADDR(magic_num_2), ret);
        return ret;
    }

    if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, AIC_PATCH_ADDR(pair_start), patch_addr))) {
        AICWFDBG(LOGERROR, "pair_start[0x%x] write fail: %d\n", AIC_PATCH_ADDR(pair_start), ret);
        return ret;
    }

    if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, AIC_PATCH_ADDR(pair_count), patch_cnt + adap_patch_cnt))) {
        AICWFDBG(LOGERROR, "pair_count[0x%x] write fail: %d\n", AIC_PATCH_ADDR(pair_count), ret);
        return ret;
    }

    for (cnt = 0; cnt < patch_cnt; cnt++) {
        if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, start_addr+8*cnt, patch_tbl_d80[cnt][0]+config_base))) {
            AICWFDBG(LOGERROR, "%x write fail\n", start_addr+8*cnt);
            return ret;
        }
        if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, start_addr+8*cnt+4, patch_tbl_d80[cnt][1]))) {
            AICWFDBG(LOGERROR, "%x write fail\n", start_addr+8*cnt+4);
            return ret;
        }
    }

    if (adap_test){
        int tmp_cnt = patch_cnt + adap_patch_cnt;
        for (cnt = patch_cnt; cnt < tmp_cnt; cnt++) {
            int tbl_idx = cnt - patch_cnt;
            if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, start_addr+8*cnt, adaptivity_patch_tbl_d80[tbl_idx][0]+config_base))) {
                AICWFDBG(LOGERROR, "%x write fail\n", start_addr+8*cnt);
            return ret;
            }
            if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, start_addr+8*cnt+4, adaptivity_patch_tbl_d80[tbl_idx][1]))) {
                AICWFDBG(LOGERROR, "%x write fail\n", start_addr+8*cnt+4);
            return ret;
            }
        }
    }

    /*
     *  Patch block 0 ~ 3, that is void by default, can be set as:
     *
     *  const u32 patch_block_0[3] = {0x11223344, 0x55667788, 0xaabbccdd};
     *  if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, (u32)(&aic_patch->block_dst[0]), 0x160000))) {
     *      printk("block_dst [0x%x] write fail: %d\n", (u32)(&aic_patch->block_dst[0]), ret);
     *  }
     *  if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, (u32)(&aic_patch->block_src[0]), 0x307000))) {
     *      printk("block_src [0x%x] write fail: %d\n", (u32)(&aic_patch->block_src[0]), ret);
     *  }
     *  if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, (u32)(&aic_patch->block_size[0]), sizeof(patch_block_0) / sizeof(u32)))) {
     *      printk("block_size[0x%x] write fail: %d\n", (u32)(&aic_patch->block_size[0]), ret);
     *  }
     *  if ((ret = rwnx_send_dbg_mem_block_write_req(usb_dev, 0x307000, sizeof(patch_block_0), patch_block_0))) {
     *      printk("blk set fail: %d\n", ret);
     *  }
     */
    if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, AIC_PATCH_ADDR(block_size[0]), 0))) {
        AICWFDBG(LOGERROR, "block_size[0x%x] write fail: %d\n", AIC_PATCH_ADDR(block_size[0]), ret);
        return ret;
    }
    if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, AIC_PATCH_ADDR(block_size[1]), 0))) {
        AICWFDBG(LOGERROR, "block_size[0x%x] write fail: %d\n", AIC_PATCH_ADDR(block_size[1]), ret);
        return ret;
    }
    if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, AIC_PATCH_ADDR(block_size[2]), 0))) {
        AICWFDBG(LOGERROR, "block_size[0x%x] write fail: %d\n", AIC_PATCH_ADDR(block_size[2]), ret);
        return ret;
    }
    if ((ret = rwnx_send_dbg_mem_write_req(usb_dev, AIC_PATCH_ADDR(block_size[3]), 0))) {
        AICWFDBG(LOGERROR, "block_size[0x%x] write fail: %d\n", AIC_PATCH_ADDR(block_size[3]), ret);
        return ret;
    }

    return 0;
}


#if 0
extern char aic_fw_path[200];

int rwnx_plat_userconfig_load_8800d80(struct aic_usb_dev *usb_dev){
    int size;
    u32 *dst=NULL;
    char *filename = FW_USERCONFIG_NAME_8800D80;

    AICWFDBG(LOGINFO, "userconfig file path:%s \r\n", filename);

    /* load file */
    size = rwnx_request_firmware_common(usb_dev, &dst, filename);
    if (size <= 0) {
            AICWFDBG(LOGERROR, "wrong size of firmware file\n");
            dst = NULL;
            return 0;
    }

	/* Copy the file on the Embedded side */
    AICWFDBG(LOGINFO, "### Load file done: %s, size=%d\n", filename, size);

	rwnx_plat_userconfig_parsing((char *)dst, size);

    rwnx_release_firmware_common(&dst);

    AICWFDBG(LOGINFO, "userconfig download complete\n\n");
    return 0;

}
#endif
int system_config_8800d80(struct aic_usb_dev *usb_dev){
		int syscfg_num;
		int ret, cnt;
		const u32 mem_addr = 0x40500000;
		struct dbg_mem_read_cfm rd_mem_addr_cfm;
		ret = rwnx_send_dbg_mem_read_req(usb_dev, mem_addr, &rd_mem_addr_cfm);
		if (ret) {
			printk("%x rd fail: %d\n", mem_addr, ret);
			return ret;
		}
        if (((rd_mem_addr_cfm.memdata >> 25) & 0x01UL) == 0x00UL) {
            chip_mcu_id = 1;
        }
		chip_id = (u8)(rd_mem_addr_cfm.memdata >> 16);
		printk("chip_id=%x, chip_mcu_id = %d\n", chip_id, chip_mcu_id);
    #if 1
		syscfg_num = sizeof(syscfg_tbl_8800d80) / sizeof(u32) / 2;
		for (cnt = 0; cnt < syscfg_num; cnt++) {
			ret = rwnx_send_dbg_mem_write_req(usb_dev, syscfg_tbl_8800d80[cnt][0], syscfg_tbl_8800d80[cnt][1]);
			if (ret) {
				printk("%x write fail: %d\n", syscfg_tbl_8800d80[cnt][0], ret);
				return ret;
			}
		}
		syscfg_num = sizeof(syscfg_tbl_masked_8800d80) / sizeof(u32) / 3;
		for (cnt = 0; cnt < syscfg_num; cnt++) {
			ret = rwnx_send_dbg_mem_mask_write_req(usb_dev,
				syscfg_tbl_masked_8800d80[cnt][0], syscfg_tbl_masked_8800d80[cnt][1], syscfg_tbl_masked_8800d80[cnt][2]);
			if (ret) {
				printk("%x mask write fail: %d\n", syscfg_tbl_masked_8800d80[cnt][0], ret);
				return ret;
			}
		}
    #endif
    return 0;
}


int aicfw_download_fw_8800d80(struct aic_usb_dev *usb_dev)
{
    struct aicbt_patch_table *head = NULL;
    struct aicbt_patch_info_t patch_info = {
        .info_len          = 0,
        .adid_addrinf      = 0,
        .addr_adid         = 0,
        .patch_addrinf     = 0,
        .addr_patch        = 0,
        .reset_addr        = 0,
        .reset_val         = 0,
        .adid_flag_addr    = 0,
        .adid_flag         = 0,
    };

    int i = 0;

    if (chip_id == CHIP_REV_U01) {
        head = aicbt_patch_table_alloc(usb_dev, FW_PATCH_TABLE_NAME_8800D80);
    } else {
        head = aicbt_patch_table_alloc(usb_dev, FW_PATCH_TABLE_NAME_8800D80_U02);
    }
    if (head == NULL){
        printk("aicbt_patch_table_alloc fail\n");
        return -1;
    }

    if(head == NULL){
        return -1;
    }
    if (chip_id == CHIP_REV_U01) {
        patch_info.addr_adid = FW_RAM_ADID_BASE_ADDR_8800D80;
        patch_info.addr_patch = FW_RAM_PATCH_BASE_ADDR_8800D80;
    } else if (chip_id == CHIP_REV_U02 || chip_id == CHIP_REV_U03) {
        patch_info.addr_adid = FW_RAM_ADID_BASE_ADDR_8800D80_U02;
        patch_info.addr_patch = FW_RAM_PATCH_BASE_ADDR_8800D80_U02;
    }
    aicbt_patch_info_unpack(&patch_info, head);
    if(patch_info.info_len == 0) {
        printk("%s, aicbt_patch_info_unpack fail\n", __func__);
        return -1;
    }

    printk("addr_adid 0x%x, addr_patch 0x%x\n", patch_info.addr_adid, patch_info.addr_patch);

    if(testmode == FW_NORMAL_MODE){

        if (chip_id != CHIP_REV_U01){
            if(rwnx_plat_bin_fw_upload_android(usb_dev, patch_info.addr_adid, FW_ADID_BASE_NAME_8800D80_U02)) {
                return -1;
            }
            if(rwnx_plat_bin_fw_upload_android(usb_dev, patch_info.addr_patch, FW_PATCH_BASE_NAME_8800D80_U02)) {
                return -1;
            }
            #if 0
            if (rwnx_plat_bin_fw_patch_table_upload_android(usb_dev, FW_PATCH_TABLE_NAME_8800D80_U02)) {
                return -1;
            }
            #else
            if (aicbt_patch_table_load(usb_dev, head)) {
                return -1;
            }
            #endif
            if(rwnx_plat_bin_fw_upload_android(usb_dev, RAM_FMAC_FW_ADDR_8800D80_U02, FW_BASE_NAME_8800D80_U02)) {
                return -1;
            }
            #if 0
            if(rwnx_plat_bin_fw_upload_android(usb_dev, FW_RAM_CALIBMODE_ADDR_8800D80_U02, FW_CALIBMODE_NAME_8800D80_U02)) {
                return -1;
            }
            if (rwnx_send_dbg_mem_write_req(usb_dev, 0x40500048, 0x1e0000))
                return -1;
            #endif
            if (aicwf_patch_config_8800d80(usb_dev)) {
                return -1;
            }
            if (rwnx_send_dbg_start_app_req(usb_dev, RAM_FMAC_FW_ADDR_8800D80_U02, HOST_START_APP_AUTO)) {
                return -1;
            }
        }else {
            if(rwnx_plat_bin_fw_upload_android(usb_dev, patch_info.addr_adid, FW_ADID_BASE_NAME_8800D80)) {
                return -1;
            }
            if(rwnx_plat_bin_fw_upload_android(usb_dev, patch_info.addr_patch, FW_PATCH_BASE_NAME_8800D80)) {
                return -1;
            }
            #if 0
            if (rwnx_plat_bin_fw_patch_table_upload_android(usb_dev, FW_PATCH_TABLE_NAME_8800D80)) {
                return -1;
            }
            #else
            if (aicbt_patch_table_load(usb_dev, head)) {
                return -1;
            }
            #endif
            if(rwnx_plat_bin_fw_upload_android(usb_dev, RAM_FMAC_FW_ADDR_8800D80, FW_BASE_NAME_8800D80)) {
                return -1;
             }
            if (rwnx_send_dbg_start_app_req(usb_dev, RAM_FMAC_FW_ADDR_8800D80, HOST_START_APP_AUTO)) {
                return -1;
            }
        }
    }else if(testmode == FW_TEST_MODE){
        if (chip_id != CHIP_REV_U01){

            if(rwnx_plat_bin_fw_upload_android(usb_dev, patch_info.addr_adid, FW_ADID_BASE_NAME_8800D80_U02)) {
                return -1;
            }

            if(rwnx_plat_bin_fw_upload_android(usb_dev, patch_info.addr_patch, FW_PATCH_BASE_NAME_8800D80_U02)) {
                return -1;
            }
#if 0
            if (rwnx_plat_bin_fw_patch_table_upload_android(usb_dev, FW_PATCH_TABLE_NAME_8800D80_U02)) {
                return -1;
            }
#else
            if (aicbt_patch_table_load(usb_dev, head)) {
                return -1;
            }
#endif

            if (chip_mcu_id) {
                int ret = 0;
                ret = rwnx_plat_flash_bin_upload_android(usb_dev, FLASH_BIN_ADDR_8800M80, FLASH_BIN_8800M80);
                if (ret && ret!= ENOENT) {
                    AICWFDBG(LOGERROR,"%s flash bin download fail \r\n", __func__);
                    return -1;
                }
            }

			if(rwnx_plat_bin_fw_upload_android(usb_dev, RAM_FMAC_RF_FW_ADDR_8800D80_U02, FW_RF_BASE_NAME_8800D80_U02)) {
				AICWFDBG(LOGERROR,"%s wifi fw download fail \r\n", __func__);
				return -1;
			}
			if (rwnx_send_dbg_start_app_req(usb_dev, RAM_FMAC_RF_FW_ADDR_8800D80_U02, HOST_START_APP_AUTO)) {
				return -1;
			}
	    } else {
            if(rwnx_plat_bin_fw_upload_android(usb_dev, RAM_FMAC_RF_FW_ADDR_8800D80, FW_RF_BASE_NAME_8800D80)) {
             AICWFDBG(LOGERROR,"%s wifi fw download fail \r\n", __func__);
                    return -1;
            }
            if (rwnx_send_dbg_start_app_req(usb_dev, RAM_FMAC_RF_FW_ADDR_8800D80, HOST_START_APP_AUTO)) {
                    return -1;
            }
	    }
    }else if(testmode == FW_BLE_SCAN_AD_FILTER_MODE){
/*
            data and ad_data_filter_mask instructions for use
            ex.
            data[18] = {0x46,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x30,0xff,0xff,0xff,0x43,0x52,0x45,0x4c,0x42};
            mask = 1100 0000 0111 1111 1100 0000 0000 0000 = 0xc07fc000

            data  = 0x46,0x00,0x00,0xff,0xff,0xff,0xff,0xff,0xff,0x30,0xff,0xff,0xff,0x43,0x52,0x45,0x4c,0x42
            mask =  1      1       0     0     0    0     0     0    0     1      1     1    1    1      1      1      1      1      0     0...... fill 0

            data & mask = "0x46 0x00" 0x00 0x00 0x00 0x00 0x00 0x00 0x00 "0x30 0xff 0xff 0x43 0x52 0x45 0x4c 0x42"
            using data & mask value condition to wakeup host_wake_bt gpio
*/
            struct ble_wakeup_param_t* wakeup_param = (struct ble_wakeup_param_t*)kmalloc(sizeof(struct ble_wakeup_param_t), GFP_KERNEL);
            uint32_t *write_blocks = (uint32_t *)wakeup_param;

            printk("%s ble scan wakeup \r\n", __func__);

            memset(wakeup_param, 0, sizeof(struct ble_wakeup_param_t));
            rwnx_plat_bin_fw_upload_android(usb_dev, RAM_FW_BLE_SCAN_WAKEUP_ADDR_8800D80, FW_BLE_SCAN_AD_FILTER_NAME);
            wakeup_param->magic_num = 0x53454C42;//magic_num
            wakeup_param->delay_scan_to = 1000;//delay start scan time(ms)
            wakeup_param->reboot_to = ble_scan_wakeup_reboot_time;//reboot time
            /******************************************************************/
            ///gpio_trigger_idx : 0    if wakeup_param->gpio_dft_lvl[0]=0xfe,this idx will be invalid.
            wakeup_param->gpio_num[0] = gpio_num;////default select gpiob2 for fw_wakeup_host
            wakeup_param->gpio_dft_lvl[0] = gpio_dft_lvl;////0:defalut pull down,  1:default pull up
            ///gpio_trigger_idx : 1    if wakeup_param->gpio_dft_lvl[1]=0xfe,this idx will be invalid.
            wakeup_param->gpio_num[1] = 3;////default select gpiob2 for fw_wakeup_host
            wakeup_param->gpio_dft_lvl[1] = 1;////0:defalut pull down,  1:default pull up
            /********************************************************************/
            //MAX_AD_FILTER_NUM=5 :num 0
            {
                const uint8_t data[11] = {0x59,0x4B,0x32,0x42,0x41,0x5F,0x54,0x45,0x53,0x54,0x33};
                wakeup_param->ad_filter[0].ad_len = 12;
                wakeup_param->ad_filter[0].ad_type = 0x09;
                memcpy(wakeup_param->ad_filter[0].ad_data, data,wakeup_param->ad_filter[0].ad_len-1);// 1111 1111 1110 0000 0000 0000 0000 0000 //0xffe00000
                wakeup_param->ad_filter[0].ad_data_mask = 0xffe00000;
                wakeup_param->ad_filter[0].ad_role = ROLE_COMBO|(COMBO_0<<4);
                wakeup_param->ad_filter[0].gpio_trigger_idx = TG_IDX_0;//0: match for wakeup_param->gpio_num[0]       1: match for wakeup_param->gpio_num[1]
            }
            /********************************************************************/
            //MAX_AD_FILTER_NUM=5 :num 1
            {
                const uint8_t data[2] = {0x12,0x18};
                wakeup_param->ad_filter[1].ad_len = 3;
                wakeup_param->ad_filter[1].ad_type = 0x3;
                memcpy(wakeup_param->ad_filter[1].ad_data, data,wakeup_param->ad_filter[1].ad_len-1);// 1100 0000 0000 0000 0000 0000 0000 0000 //0xc0000000
                wakeup_param->ad_filter[1].ad_data_mask = 0xc0000000;
                wakeup_param->ad_filter[1].ad_role = ROLE_COMBO|(COMBO_0<<4);
                wakeup_param->ad_filter[1].gpio_trigger_idx = TG_IDX_0;//0: match for wakeup_param->gpio_num[0]       1: match for wakeup_param->gpio_num[1]
            }
            /********************************************************************/
            //MAX_AD_FILTER_NUM=5 :num 2
            {
                //const uint8_t data[11] = {0x59,0x4B,0x32,0x42,0x41,0x5F,0x54,0x45,0x53,0x54,0x33};
                wakeup_param->ad_filter[2].ad_len = 0;
                wakeup_param->ad_filter[2].ad_type = 0;
                //memcpy(wakeup_param->ad_filter[2].ad_data, data,wakeup_param->ad_filter[2].ad_len-1);// 1100 0000 0111 1111 1100 0000 0000 0000 //0xc07fc000
                wakeup_param->ad_filter[2].ad_data_mask = 0;
                wakeup_param->ad_filter[2].ad_role = ROLE_ONLY;
                wakeup_param->ad_filter[2].gpio_trigger_idx = TG_IDX_0;//0: match for wakeup_param->gpio_num[0]       1: match for wakeup_param->gpio_num[1]
            }
            /********************************************************************/
            //MAX_AD_FILTER_NUM=5 :num 3
            {
                //const uint8_t data[11] = {0x59,0x4B,0x32,0x42,0x41,0x5F,0x54,0x45,0x53,0x54,0x33};
                wakeup_param->ad_filter[3].ad_len = 0;
                wakeup_param->ad_filter[3].ad_type = 0;
                //memcpy(wakeup_param->ad_filter[2].ad_data, data,wakeup_param->ad_filter[2].ad_len-1);// 1100 0000 0111 1111 1100 0000 0000 0000 //0xc07fc000
                wakeup_param->ad_filter[3].ad_data_mask = 0;
                wakeup_param->ad_filter[3].ad_role = ROLE_COMBO|(COMBO_1<<4);
                wakeup_param->ad_filter[3].gpio_trigger_idx = TG_IDX_0;//0: match for wakeup_param->gpio_num[0]       1: match for wakeup_param->gpio_num[1]
            }
            /********************************************************************/
            //MAX_AD_FILTER_NUM=5 :num 4
            {
                //const uint8_t data[11] = {0x59,0x4B,0x32,0x42,0x41,0x5F,0x54,0x45,0x53,0x54,0x33};
                wakeup_param->ad_filter[4].ad_len = 0;
                wakeup_param->ad_filter[4].ad_type = 0x09;
                //memcpy(wakeup_param->ad_filter[4].ad_data, data,wakeup_param->ad_filter[4].ad_len-1);// 1111 1111 1110 0000 0000 0000 0000 0000 //0xffe00000
                wakeup_param->ad_filter[4].ad_data_mask = 0xffe00000;
                wakeup_param->ad_filter[4].ad_role = ROLE_COMBO|(COMBO_1<<4);
                wakeup_param->ad_filter[4].gpio_trigger_idx = TG_IDX_0|TG_IDX_1;//0: match for wakeup_param->gpio_num[0]       1: match for wakeup_param->gpio_num[1]
            }

            for(i = 0; i < (sizeof(struct ble_wakeup_param_t)/4 +1); i++){
                printk("write_blocks[%d]:0x%08X \r\n", i, write_blocks[i]);
                rwnx_send_dbg_mem_write_req(usb_dev, 0x15FF00 + (4 * i), write_blocks[i]);
            }
            rwnx_send_dbg_start_app_req(usb_dev, RAM_FW_BLE_SCAN_WAKEUP_ADDR_8800D80, HOST_START_APP_AUTO);
            kfree(wakeup_param);

            return -1;
        }
    return 0;
}


