#include "rwnx_main.h"
#include "rwnx_msg_tx.h"
#include "reg_access.h"
#include "aic_bsp_export.h"

#define RWNX_MAC_RF_PATCH_BASE_NAME_8800DC     "fmacfw_rf_patch_8800dc"
#define RWNX_MAC_RF_PATCH_NAME_8800DC RWNX_MAC_RF_PATCH_BASE_NAME_8800DC".bin"
#define FW_USERCONFIG_NAME_8800DC         "aic_userconfig_8800dc.txt"
#define FW_USERCONFIG_NAME_8800DW         "aic_userconfig_8800dw.txt"

int rwnx_plat_bin_fw_upload_2(struct rwnx_hw *rwnx_hw, u32 fw_addr,
                               char *filename);
int rwnx_request_firmware_common(struct rwnx_hw *rwnx_hw,
	u32** buffer, const char *filename);
void rwnx_plat_userconfig_parsing2(char *buffer, int size);

void rwnx_release_firmware_common(u32** buffer);

u32 wifi_txgain_table_24g_8800dcdw[32] =
{
    0xA4B22189, //index 0
    0x00007825,
    0xA4B2214B, //index 1
    0x00007825,
    0xA4B2214F, //index 2
    0x00007825,
    0xA4B221D5, //index 3
    0x00007825,
    0xA4B221DC, //index 4
    0x00007825,
    0xA4B221E5, //index 5
    0x00007825,
    0xAC9221E5, //index 6
    0x00006825,
    0xAC9221EF, //index 7
    0x00006825,
    0xBC9221EE, //index 8
    0x00006825,
    0xBC9221FF, //index 9
    0x00006825,
    0xBC9221FF, //index 10
    0x00004025,
    0xB792203F, //index 11
    0x00004026,
    0xDC92203F, //index 12
    0x00004025,
    0xE692203F, //index 13
    0x00004025,
    0xFF92203F, //index 14
    0x00004035,
    0xFFFE203F, //index 15
    0x00004832
};

u32 wifi_txgain_table_24g_1_8800dcdw[32] =
{
    0x096E2011, //index 0
    0x00004001,
    0x096E2015, //index 1
    0x00004001,
    0x096E201B, //index 2
    0x00004001,
    0x116E2018, //index 3
    0x00004001,
    0x116E201E, //index 4
    0x00004001,
    0x116E2023, //index 5
    0x00004001,
    0x196E2021, //index 6
    0x00004001,
    0x196E202B, //index 7
    0x00004001,
    0x216E202B, //index 8
    0x00004001,
    0x236E2027, //index 9
    0x00004001,
    0x236E2031, //index 10
    0x00004001,
    0x246E2039, //index 11
    0x00004001,
    0x26922039, //index 12
    0x00004001,
    0x2E92203F, //index 13
    0x00004001,
    0x3692203F, //index 14
    0x00004001,
    0x3FF2203F, //index 15
    0x00004001,
};

u32 wifi_txgain_table_24g_8800dcdw_h[32] =
{
    0xA55629C9, //index 0
    0x00005825,
    0xAE5629C9, //index 1
    0x00005825,
    0xAD5629CD, //index 2
    0x00005825,
    0xAD5629D1, //index 3
    0x00005825,
    0xAD5629D7, //index 4
    0x00005825,
    0xAD5629DE, //index 5
    0x00005825,
    0xAD5629E6, //index 6
    0x00005825,
    0xBD5629E6, //index 7
    0x00005825,
    0xBD5629F0, //index 8
    0x00005825,
    0xCD5629F0, //index 9
    0x00005825,
    0xE55629F0, //index 10
    0x00005825,
    0xE55629FF, //index 11
    0x00005825,
    0xE55629FF, //index 12
    0x00002825,
    0xE75629FF, //index 13
    0x00002825,
    0xFF5629FF, //index 14
    0x00001825,
    0xFF5628FF, //index 15
    0x00001025,
};

u32 wifi_txgain_table_24g_1_8800dcdw_h[32] =
{
    0x941A2048, //index 0
    0x00001825,
    0x961A2048, //index 1
    0x00001825,
    0x9D1A2048, //index 2
    0x00001825,
    0x9A1A204F, //index 3
    0x00001825,
    0x961A204F, //index 4
    0x00001825,
    0x9A1A2057, //index 5
    0x00001825,
    0x9C1A2057, //index 6
    0x00001825,
    0xA31A205B, //index 7
    0x00001825,
    0xAB1A205B, //index 8
    0x00001825,
    0xAD1A205B, //index 9
    0x00001825,
    0xA71A2064, //index 10
    0x00001825,
    0xAD1A2070, //index 11
    0x00001825,
    0xAD72207F, //index 12
    0x00001825,
    0xBCAE207F, //index 13
    0x00001825,
    0xBFB2207F, //index 14
    0x00001825,
    0xD73A207F, //index 15
    0x00001825,
};

u32 wifi_rxgain_table_24g_20m_8800dcdw[64] = {
    0x82f282d1,//index 0
    0x9591a324,
    0x80808419,
    0x000000f0,
    0x42f282d1,//index 1
    0x95923524,
    0x80808419,
    0x000000f0,
    0x22f282d1,//index 2
    0x9592c724,
    0x80808419,
    0x000000f0,
    0x02f282d1,//index 3
    0x9591a324,
    0x80808419,
    0x000000f0,
    0x06f282d1,//index 4
    0x9591a324,
    0x80808419,
    0x000000f0,
    0x0ef29ad1,//index 5
    0x9591a324,
    0x80808419,
    0x000000f0,
    0x0ef29ad3,//index 6
    0x95923524,
    0x80808419,
    0x000000f0,
    0x0ef29ad7,//index 7
    0x9595a324,
    0x80808419,
    0x000000f0,
    0x02f282d2,//index 8
    0x95951124,
    0x80808419,
    0x000000f0,
    0x02f282f4,//index 9
    0x95951124,
    0x80808419,
    0x000000f0,
    0x02f282e6,//index 10
    0x9595a324,
    0x80808419,
    0x000000f0,
    0x02f282e6,//index 11
    0x9599a324,
    0x80808419,
    0x000000f0,
    0x02f282e6,//index 12
    0x959da324,
    0x80808419,
    0x000000f0,
    0x02f282e6,//index 13
    0x959f5924,
    0x80808419,
    0x000000f0,
    0x06f282e6,//index 14
    0x959f5924,
    0x80808419,
    0x000000f0,
    0x0ef29ae6,//index 15
    0x959f5924,           //loft [35:34]=3
    0x80808419,
    0x000000f0
};

u32 wifi_rxgain_table_24g_40m_8800dcdw[64] = {
    0x83428151,//index 0
    0x9631a328,
    0x80808419,
    0x000000f0,
    0x43428151,//index 1
    0x96323528,
    0x80808419,
    0x000000f0,
    0x23428151,//index 2
    0x9632c728,
    0x80808419,
    0x000000f0,
    0x03428151,//index 3
    0x9631a328,
    0x80808419,
    0x000000f0,
    0x07429951,//index 4
    0x9631a328,
    0x80808419,
    0x000000f0,
    0x0f42d151,//index 5
    0x9631a328,
    0x80808419,
    0x000000f0,
    0x0f42d153,//index 6
    0x96323528,
    0x80808419,
    0x000000f0,
    0x0f42d157,//index 7
    0x9635a328,
    0x80808419,
    0x000000f0,
    0x03428152,//index 8
    0x96351128,
    0x80808419,
    0x000000f0,
    0x03428174,//index 9
    0x96351128,
    0x80808419,
    0x000000f0,
    0x03428166,//index 10
    0x9635a328,
    0x80808419,
    0x000000f0,
    0x03428166,//index 11
    0x9639a328,
    0x80808419,
    0x000000f0,
    0x03428166,//index 12
    0x963da328,
    0x80808419,
    0x000000f0,
    0x03428166,//index 13
    0x963f5928,
    0x80808419,
    0x000000f0,
    0x07429966,//index 14
    0x963f5928,
    0x80808419,
    0x000000f0,
    0x0f42d166,//index 15
    0x963f5928,
    0x80808419,
    0x000000f0
};

#define RAM_LMAC_FW_ADDR               0x00150000
#ifdef CONFIG_DPD
#if (defined(CONFIG_DPD) && !defined(CONFIG_FORCE_DPD_CALIB))
extern int is_file_exist(char* name);
#endif
extern rf_misc_ram_lite_t dpd_res;

int aicwf_fdrv_dpd_result_apply_8800dc(struct rwnx_hw *rwnx_hw, rf_misc_ram_lite_t *dpd_res)
{
    int ret = 0;
    uint32_t cfg_base = 0x10164;
    struct dbg_mem_read_cfm cfm;
    uint32_t misc_ram_addr;
    uint32_t ram_base_addr, ram_byte_cnt;
    AICWFDBG(LOGINFO, "bit_mask[1]=%x\n", dpd_res->bit_mask[1]);
    if (dpd_res->bit_mask[1] == 0) {
        AICWFDBG(LOGERROR, "void dpd_res, bypass it.\n");
        return 0;
    }
    if (testmode == 1) {
        cfg_base = RAM_LMAC_FW_ADDR + 0x0164;
    }
    if ((ret = rwnx_send_dbg_mem_read_req(rwnx_hw, cfg_base + 0x14, &cfm))) {
        AICWFDBG(LOGERROR, "rf misc ram[0x%x] rd fail: %d\n", cfg_base + 0x14, ret);
        return ret;
    }
    misc_ram_addr = cfm.memdata;
    AICWFDBG(LOGINFO, "misc_ram_addr: %x\n", misc_ram_addr);
    /* Copy dpd_res on the Embedded side */
    // bit_mask
    AICWFDBG(LOGINFO, "bit_mask[0]=%x\n", dpd_res->bit_mask[0]);
    ram_base_addr = misc_ram_addr + offsetof(rf_misc_ram_t, bit_mask);
    ram_byte_cnt = MEMBER_SIZE(rf_misc_ram_t, bit_mask) + MEMBER_SIZE(rf_misc_ram_t, reserved);
    ret = rwnx_send_dbg_mem_block_write_req(rwnx_hw, ram_base_addr, ram_byte_cnt, (u32 *)&dpd_res->bit_mask[0]);
    if (ret) {
        AICWFDBG(LOGERROR, "bit_mask wr fail: %x, ret:%d\r\n", ram_base_addr, ret);
        return ret;
    }
    // dpd_high
    AICWFDBG(LOGINFO, "dpd_high[0]=%x\n", dpd_res->dpd_high[0]);
    ram_base_addr = misc_ram_addr + offsetof(rf_misc_ram_t, dpd_high);
    ram_byte_cnt = MEMBER_SIZE(rf_misc_ram_t, dpd_high);
    ret = rwnx_send_dbg_mem_block_write_req(rwnx_hw, ram_base_addr, ram_byte_cnt, (u32 *)&dpd_res->dpd_high[0]);
    if (ret) {
        AICWFDBG(LOGERROR, "dpd_high wr fail: %x, ret:%d\r\n", ram_base_addr, ret);
        return ret;
    }
    // loft_res
    AICWFDBG(LOGINFO, "loft_res[0]=%x\n", dpd_res->loft_res[0]);
    ram_base_addr = misc_ram_addr + offsetof(rf_misc_ram_t, loft_res);
    ram_byte_cnt = MEMBER_SIZE(rf_misc_ram_t, loft_res);
    ret = rwnx_send_dbg_mem_block_write_req(rwnx_hw, ram_base_addr, ram_byte_cnt, (u32 *)&dpd_res->loft_res[0]);
    if (ret) {
        AICWFDBG(LOGERROR, "loft_res wr fail: %x, ret:%d\r\n", ram_base_addr, ret);
        return ret;
    }
    return ret;
}

#ifndef CONFIG_FORCE_DPD_CALIB
int aicwf_fdrv_dpd_result_load_8800dc(struct rwnx_hw *rwnx_hw, rf_misc_ram_lite_t *dpd_res)
{
    int ret = 0;
    int size;
    u32 *dst=NULL;
    char *filename = FW_DPDRESULT_NAME_8800DC;
    AICWFDBG(LOGINFO, "dpd_res file path:%s \r\n", filename);
    /* load file */
    size = rwnx_request_firmware_common(rwnx_hw, &dst, filename);
    if (size <= 0) {
        AICWFDBG(LOGERROR, "wrong size of dpd_res file\n");
        dst = NULL;
        return -1;
    }
    AICWFDBG(LOGINFO, "### Load file done: %s, size=%d, dst[0]=%x\n", filename, size, dst[0]);
    memcpy((u8 *)dpd_res, (u8 *)dst, sizeof(rf_misc_ram_lite_t));
    if (dst) {
        rwnx_release_firmware_common(&dst);
    }
    return ret;
}
#endif
#endif

int aicwf_fdrv_misc_ram_init_8800dc(struct rwnx_hw *rwnx_hw)
{
    int ret = 0;
    uint32_t cfg_base = 0x10164;
    struct dbg_mem_read_cfm cfm;
    uint32_t misc_ram_addr;
    uint32_t misc_ram_size = 12;
    int i;

    if (testmode == 1) {
        cfg_base = RAM_LMAC_FW_ADDR + 0x0164;
    }
    // init misc ram
    printk("%s\n", __func__);
    ret = rwnx_send_dbg_mem_read_req(rwnx_hw, cfg_base + 0x14, &cfm);
    if (ret) {
        AICWFDBG(LOGERROR, "rf misc ram[0x%x] rd fail: %d\n", cfg_base + 0x14, ret);
        return ret;
    }
    misc_ram_addr = cfm.memdata;
    AICWFDBG(LOGERROR, "misc_ram_addr=%x\n", misc_ram_addr);
    for (i = 0; i < (misc_ram_size / 4); i++) {
        ret = rwnx_send_dbg_mem_write_req(rwnx_hw, misc_ram_addr + i * 4, 0);
        if (ret) {
            AICWFDBG(LOGERROR, "rf misc ram[0x%x] wr fail: %d\n",  misc_ram_addr + i * 4, ret);
            return ret;
        }
    }
    return ret;
}


int aicwf_set_rf_config_8800dc(struct rwnx_hw *rwnx_hw, struct mm_set_rf_calib_cfm *cfm){
	int ret = 0;

	if ((ret = rwnx_send_txpwr_lvl_req(rwnx_hw))) {
		return -1;
	}

	if ((ret = rwnx_send_txpwr_ofst_req(rwnx_hw))) {
		return -1;
	}


	if (testmode == 0) {
        if (IS_CHIP_ID_H()) {
            if ((ret = rwnx_send_rf_config_req(rwnx_hw, 0,    1, (u8_l *)wifi_txgain_table_24g_8800dcdw_h, 128)))
                return -1;

            if ((ret = rwnx_send_rf_config_req(rwnx_hw, 16,    1, (u8_l *)wifi_txgain_table_24g_1_8800dcdw_h, 128)))
                return -1;
        } else {
            if ((ret = rwnx_send_rf_config_req(rwnx_hw, 0,    1, (u8_l *)wifi_txgain_table_24g_8800dcdw, 128)))
                return -1;

            if ((ret = rwnx_send_rf_config_req(rwnx_hw, 16,    1, (u8_l *)wifi_txgain_table_24g_1_8800dcdw, 128)))
                return -1;
        }

		if ((ret = rwnx_send_rf_config_req(rwnx_hw, 0,	0, (u8_l *)wifi_rxgain_table_24g_20m_8800dcdw, 256)))
			return -1;

		if ((ret = rwnx_send_rf_config_req(rwnx_hw, 32,  0, (u8_l *)wifi_rxgain_table_24g_40m_8800dcdw, 256)))
			return -1;

		if ((ret = rwnx_send_rf_calib_req(rwnx_hw, cfm))) {
			return -1;
		}
	} else if (testmode == 1) {
        if (chip_sub_id >= 1) {
            #ifdef CONFIG_DPD
			#ifndef CONFIG_FORCE_DPD_CALIB
            if (is_file_exist(FW_DPDRESULT_NAME_8800DC) == 1) {
                AICWFDBG(LOGINFO, "%s load dpd bin\n", __func__);
                ret = aicwf_fdrv_dpd_result_load_8800dc(rwnx_hw, &dpd_res);
                if (ret) {
                    AICWFDBG(LOGINFO, "load dpd bin fail: %d\n", ret);
                    return ret;
                }
            }
            #endif
            if (dpd_res.bit_mask[1]) {
                ret = aicwf_fdrv_dpd_result_apply_8800dc(rwnx_hw, &dpd_res);
                if (ret) {
                    AICWFDBG(LOGINFO, "apply dpd bin fail: %d\n", ret);
                    return ret;
                }
            }
            #else
            {
                ret = aicwf_fdrv_misc_ram_init_8800dc(rwnx_hw);
                if (ret) {
                    AICWFDBG(LOGINFO, "misc ram init fail: %d\n", ret);
                    return ret;
                }
            }
            #endif
            ret = rwnx_send_rf_calib_req(rwnx_hw, cfm);
            if (ret) {
                AICWFDBG(LOGINFO, "rf calib req fail: %d\n", ret);
                return ret;
            }
        }
    }

    return 0 ;
}

int	rwnx_plat_userconfig_load_8800dc(struct rwnx_hw *rwnx_hw){
    int size;
    u32 *dst=NULL;
    char *filename = FW_USERCONFIG_NAME_8800DC;

    AICWFDBG(LOGINFO, "userconfig file path:%s \r\n", filename);

    /* load file */
    size = rwnx_request_firmware_common(rwnx_hw, &dst, filename);
    if (size <= 0) {
            AICWFDBG(LOGERROR, "wrong size of firmware file\n");
            dst = NULL;
            return 0;
    }

	/* Copy the file on the Embedded side */
    AICWFDBG(LOGINFO, "### Load file done: %s, size=%d\n", filename, size);

	rwnx_plat_userconfig_parsing2((char *)dst, size);

    rwnx_release_firmware_common(&dst);

    AICWFDBG(LOGINFO, "userconfig download complete\n\n");
    return 0;

}

int	rwnx_plat_userconfig_load_8800dw(struct rwnx_hw *rwnx_hw){
    int size;
    u32 *dst=NULL;
    char *filename = FW_USERCONFIG_NAME_8800DC;

    AICWFDBG(LOGINFO, "userconfig file path:%s \r\n", filename);

    /* load file */
    size = rwnx_request_firmware_common(rwnx_hw, &dst, filename);
    if (size <= 0) {
            AICWFDBG(LOGERROR, "wrong size of firmware file\n");
            dst = NULL;
            return 0;
    }

	/* Copy the file on the Embedded side */
    AICWFDBG(LOGINFO, "### Load file done: %s, size=%d\n", filename, size);

	rwnx_plat_userconfig_parsing2((char *)dst, size);

    rwnx_release_firmware_common(&dst);

    AICWFDBG(LOGINFO, "userconfig download complete\n\n");
    return 0;

}

