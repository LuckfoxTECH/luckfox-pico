#include "aic8800d80_compat.h"
#include "aic_bsp_driver.h"

extern struct aicbsp_info_t aicbsp_info;
extern int adap_test;

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
#define AIC_PATCH_ADDR(mem) ((u32)(aic_patch_str_base + AIC_PATCH_OFST(mem)))

u32 aicbsp_syscfg_tbl_8800d80[][2] = {
};

int aicbsp_system_config_8800d80(struct aic_sdio_dev *sdiodev)
{
	int syscfg_num = sizeof(aicbsp_syscfg_tbl_8800d80) / sizeof(u32) / 2;
	int ret, cnt;
	for (cnt = 0; cnt < syscfg_num; cnt++) {
		ret = rwnx_send_dbg_mem_write_req(sdiodev, aicbsp_syscfg_tbl_8800d80[cnt][0], aicbsp_syscfg_tbl_8800d80[cnt][1]);
		if (ret) {
			printk("%x write fail: %d\n", aicbsp_syscfg_tbl_8800d80[cnt][0], ret);
			return ret;
		}
	}
	return 0;
}


u32 adaptivity_patch_tbl_8800d80[][2] = {
	{0x000C, 0x0000320A}, //linkloss_thd
	{0x009C, 0x00000000}, //ac_param_conf
	{0x0168, 0x00010000}, //tx_adaptivity_en
};

u32 patch_tbl_8800d80[][2] = {
	#ifdef USE_5G
	{0x00b4, 0xf3010001},
	#else
	{0x00b4, 0xf3010000},
	#endif
#if defined(CONFIG_AMSDU_RX)
        {0x170, 0x0100000a}
#endif
#if AIC_IRQ_WAKE_FLAG
	{0x00000170, 0x0000010a}, //irqf
#endif
};

#ifdef CONFIG_OOB
// for 8800d40/d80     map data1 isr to gpiob1
u32 gpio_cfg_tbl_8800d40d80[][2] = {
    {0x40504084, 0x00000006},
    {0x40500040, 0x00000000},
    {0x40100030, 0x00000001},
    {0x40241020, 0x00000001},
    {0x40240030, 0x00000004},
    {0x40240020, 0x03020700},
};
#endif

int aicwifi_sys_config_8800d80(struct aic_sdio_dev *sdiodev)
{
#ifdef CONFIG_OOB
    int ret, cnt;
	int gpiocfg_num = sizeof(gpio_cfg_tbl_8800d40d80) / sizeof(u32) / 2;
	for (cnt = 0; cnt < gpiocfg_num; cnt++) {
		ret = rwnx_send_dbg_mem_write_req(sdiodev, gpio_cfg_tbl_8800d40d80[cnt][0], gpio_cfg_tbl_8800d40d80[cnt][1]);
		if (ret) {
			printk("%x write fail: %d\n", gpio_cfg_tbl_8800d40d80[cnt][0], ret);
			return ret;
		}
	}
#endif

	return 0;
}

int aicwifi_patch_config_8800d80(struct aic_sdio_dev *sdiodev)
{
	const u32 rd_patch_addr = RAM_FMAC_FW_ADDR + 0x0198;
	u32 aic_patch_addr;
	u32 config_base, aic_patch_str_base;
	uint32_t start_addr = 0x0016F800;
	u32 patch_addr = start_addr;
	u32 patch_cnt = sizeof(patch_tbl_8800d80)/sizeof(u32)/2;
	struct dbg_mem_read_cfm rd_patch_addr_cfm;
	int ret = 0;
	int cnt = 0;
	//adap test
	int adap_patch_cnt = 0;

	if (adap_test) {
        printk("%s for adaptivity test \r\n", __func__);
		adap_patch_cnt = sizeof(adaptivity_patch_tbl_8800d80)/sizeof(u32)/2;
	}

	aic_patch_addr = rd_patch_addr + 8;

	ret = rwnx_send_dbg_mem_read_req(sdiodev, rd_patch_addr, &rd_patch_addr_cfm);
	if (ret) {
		printk("patch rd fail\n");
		return ret;
	}

	config_base = rd_patch_addr_cfm.memdata;

	ret = rwnx_send_dbg_mem_read_req(sdiodev, aic_patch_addr, &rd_patch_addr_cfm);
	if (ret) {
		printk("patch str rd fail\n");
		return ret;
	}
	aic_patch_str_base = rd_patch_addr_cfm.memdata;

	ret = rwnx_send_dbg_mem_write_req(sdiodev, AIC_PATCH_ADDR(magic_num), AIC_PATCH_MAGIG_NUM);
	if (ret) {
		printk("0x%x write fail\n", AIC_PATCH_ADDR(magic_num));
		return ret;
	}

	ret = rwnx_send_dbg_mem_write_req(sdiodev, AIC_PATCH_ADDR(magic_num_2), AIC_PATCH_MAGIG_NUM_2);
	if (ret) {
		printk("0x%x write fail\n", AIC_PATCH_ADDR(magic_num_2));
		return ret;
	}

	ret = rwnx_send_dbg_mem_write_req(sdiodev, AIC_PATCH_ADDR(pair_start), patch_addr);
	if (ret) {
		printk("0x%x write fail\n", AIC_PATCH_ADDR(pair_start));
		return ret;
	}

	ret = rwnx_send_dbg_mem_write_req(sdiodev, AIC_PATCH_ADDR(pair_count), patch_cnt + adap_patch_cnt);
	if (ret) {
		printk("0x%x write fail\n", AIC_PATCH_ADDR(pair_count));
		return ret;
	}

	for (cnt = 0; cnt < patch_cnt; cnt++) {
		ret = rwnx_send_dbg_mem_write_req(sdiodev, start_addr+8*cnt, patch_tbl_8800d80[cnt][0]+config_base);
		if (ret) {
			printk("%x write fail\n", start_addr+8*cnt);
			return ret;
		}
		ret = rwnx_send_dbg_mem_write_req(sdiodev, start_addr+8*cnt+4, patch_tbl_8800d80[cnt][1]);
		if (ret) {
			printk("%x write fail\n", start_addr+8*cnt+4);
			return ret;
		}
	}

	if (adap_test){
		int tmp_cnt = patch_cnt + adap_patch_cnt;
		for (cnt = patch_cnt; cnt < tmp_cnt; cnt++) {
			int tbl_idx = cnt - patch_cnt;
			ret = rwnx_send_dbg_mem_write_req(sdiodev, start_addr+8*cnt, adaptivity_patch_tbl_8800d80[tbl_idx][0]+config_base);
			if(ret) {
				printk("%x write fail\n", start_addr+8*cnt);
				return ret;
			}
			ret = rwnx_send_dbg_mem_write_req(sdiodev, start_addr+8*cnt+4, adaptivity_patch_tbl_8800d80[tbl_idx][1]);
			if(ret) {
				printk("%x write fail\n", start_addr+8*cnt+4);
				return ret;
			}
		}
	}

	ret = rwnx_send_dbg_mem_write_req(sdiodev, AIC_PATCH_ADDR(block_size[0]), 0);
	if (ret) {
		printk("block_size[0x%x] write fail: %d\n", AIC_PATCH_ADDR(block_size[0]), ret);
		return ret;
	}
	ret = rwnx_send_dbg_mem_write_req(sdiodev, AIC_PATCH_ADDR(block_size[1]), 0);
	if (ret) {
		printk("block_size[0x%x] write fail: %d\n", AIC_PATCH_ADDR(block_size[1]), ret);
		return ret;
	}
	ret = rwnx_send_dbg_mem_write_req(sdiodev, AIC_PATCH_ADDR(block_size[2]), 0);
	if (ret) {
		printk("block_size[0x%x] write fail: %d\n", AIC_PATCH_ADDR(block_size[2]), ret);
		return ret;
	}
	ret = rwnx_send_dbg_mem_write_req(sdiodev, AIC_PATCH_ADDR(block_size[3]), 0);
	if (ret) {
		printk("block_size[0x%x] write fail: %d\n", AIC_PATCH_ADDR(block_size[3]), ret);
		return ret;
	}

	return 0;
}


