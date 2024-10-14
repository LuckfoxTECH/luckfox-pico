// SPDX-License-Identifier: GPL-2.0-only
/*
 * Rockchip UFS Host Controller driver
 *
 * Copyright (C) 2024 Rockchip Electronics Co.Ltd.
 */

#ifndef _UFS_ROCKCHIP_H_
#define _UFS_ROCKCHIP_H_

#define UFS_MAX_CLKS 3

#define SEL_TX_LANE0 0x0 
#define SEL_TX_LANE1 0x1
#define SEL_TX_LANE2 0x2
#define SEL_TX_LANE3 0x3
#define SEL_RX_LANE0 0x4
#define SEL_RX_LANE1 0x5
#define SEL_RX_LANE2 0x6
#define SEL_RX_LANE3 0x7

#define MIB_T_DBG_CPORT_TX_ENDIAN	0xc022
#define MIB_T_DBG_CPORT_RX_ENDIAN	0xc023

/* Vendor specific attributes */
enum dwc_specific_registers {
	DWC_UFS_REG_HCLKDIV	= 0xFC,
};

/* Clock Divider Values: Hex equivalent of frequency in MHz */
enum clk_div_values {
	DWC_UFS_REG_HCLKDIV_DIV_62_5	= 0x3e,
	DWC_UFS_REG_HCLKDIV_DIV_125	= 0x7d,
	DWC_UFS_REG_HCLKDIV_DIV_200	= 0xc8,
};

/* Selector Index */
enum selector_index {
	SELIND_LN0_TX		= 0x00,
	SELIND_LN1_TX		= 0x01,
	SELIND_LN0_RX		= 0x04,
	SELIND_LN1_RX		= 0x05,
};

struct ufshcd_dme_attr_val {
	u32 attr_sel;
	u32 mib_val;
	u8 peer;
};

struct ufs_rockchip_host {
	struct ufs_hba *hba;
	void __iomem *ufs_phy_ctrl;
	void __iomem *ufs_sys_ctrl;
	void __iomem *mphy_base;
	struct reset_control *rst;
	struct clk ref_out_clk;
	uint64_t caps;
	uint32_t phy_config_mode;
	bool in_suspend;
};

#define	ufs_sys_writel(base, val, reg)		\
	writel((val), (base) + (reg))
#define ufs_sys_readl(base, reg) readl((base) + (reg))
#define ufs_sys_set_bits(base, mask, reg)	\
	ufs_sys_writel((base), ((mask) | (ufs_sys_readl((base), (reg)))), (reg))
#define ufs_sys_ctrl_clr_bits(base, mask, reg)	\
	ufs_sys_writel((base), ((~(mask)) & (ufs_sys_readl((base), (reg)))), (reg))

#endif /* _UFS_ROCKCHIP_H_ */
