/*
 * Analogix DP (Display port) core register interface driver.
 *
 * Copyright (C) 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 * Copyright (C) 2012 Samsung Electronics Co., Ltd.
 * Author: Jingoo Han <jg1.han@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#include <config.h>
#include <common.h>
#include <errno.h>
#include <malloc.h>
#include <asm/unaligned.h>
#include <linux/list.h>
#include <dm/device.h>
#include <syscon.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <linux/iopoll.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "analogix_dp.h"

#define COMMON_INT_MASK_1	0
#define COMMON_INT_MASK_2	0
#define COMMON_INT_MASK_3	0
#define COMMON_INT_MASK_4	(HOTPLUG_CHG | HPD_LOST | PLUG)
#define INT_STA_MASK		INT_HPD

static void analogix_dp_write(struct analogix_dp_device *dp, u32 reg, u32 val)
{
	readl(dp->reg_base);
	writel(val, dp->reg_base + reg);
	writel(val, dp->reg_base + reg);
}

static u32 analogix_dp_read(struct analogix_dp_device *dp, u32 reg)
{
	readl(dp->reg_base + reg);

	return readl(dp->reg_base + reg);
}

void analogix_dp_enable_video_mute(struct analogix_dp_device *dp, bool enable)
{
	u32 reg;

	if (enable) {
		reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_1);
		reg |= HDCP_VIDEO_MUTE;
		analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_1, reg);
	} else {
		reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_1);
		reg &= ~HDCP_VIDEO_MUTE;
		analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_1, reg);
	}
}

void analogix_dp_stop_video(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_1);
	reg &= ~VIDEO_EN;
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_1, reg);
}

static void analogix_dp_set_lane_map(struct analogix_dp_device *dp)
{
	u32 i, reg = 0;

	for (i = 0; i < dp->video_info.max_lane_count; i++)
		reg |= dp->lane_map[i] << (2 * i);

	analogix_dp_write(dp, ANALOGIX_DP_LANE_MAP, reg);
}

void analogix_dp_init_analog_param(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = TX_TERMINAL_CTRL_50_OHM;
	analogix_dp_write(dp, ANALOGIX_DP_ANALOG_CTL_1, reg);

	reg = SEL_24M | TX_DVDD_BIT_1_0625V;
	analogix_dp_write(dp, ANALOGIX_DP_ANALOG_CTL_2, reg);

	if (dp->plat_data.dev_type == ROCKCHIP_DP) {
		reg = REF_CLK_24M;
		if (dp->plat_data.subdev_type == RK3288_DP ||
		    dp->plat_data.subdev_type == RK3368_EDP)
			reg ^= REF_CLK_MASK;

		analogix_dp_write(dp, ANALOGIX_DP_PLL_REG_1, reg);
		analogix_dp_write(dp, ANALOGIX_DP_PLL_REG_2, 0x99);
		analogix_dp_write(dp, ANALOGIX_DP_PLL_REG_3, 0x40);
		analogix_dp_write(dp, ANALOGIX_DP_PLL_REG_4, 0x58);
		analogix_dp_write(dp, ANALOGIX_DP_PLL_REG_5, 0x22);
		analogix_dp_write(dp, ANALOGIX_DP_BIAS, 0x44);
	}

	reg = DRIVE_DVDD_BIT_1_0625V | VCO_BIT_600_MICRO;
	analogix_dp_write(dp, ANALOGIX_DP_ANALOG_CTL_3, reg);

	reg = PD_RING_OSC | AUX_TERMINAL_CTRL_50_OHM |
		TX_CUR1_2X | TX_CUR_16_MA;
	analogix_dp_write(dp, ANALOGIX_DP_PLL_FILTER_CTL_1, reg);

	reg = CH3_AMP_400_MV | CH2_AMP_400_MV |
		CH1_AMP_400_MV | CH0_AMP_400_MV;
	analogix_dp_write(dp, ANALOGIX_DP_TX_AMP_TUNING_CTL, reg);
}

void analogix_dp_init_interrupt(struct analogix_dp_device *dp)
{
	/* Set interrupt pin assertion polarity as high */
	analogix_dp_write(dp, ANALOGIX_DP_INT_CTL, INT_POL1 | INT_POL0);

	/* Clear pending regisers */
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_STA_1, 0xff);
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_STA_2, 0x4f);
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_STA_3, 0xe0);
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_STA_4, 0xe7);
	analogix_dp_write(dp, ANALOGIX_DP_INT_STA, 0x63);

	/* 0:mask,1: unmask */
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_1, 0x00);
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_2, 0x00);
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_3, 0x00);
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_4, 0x00);
	analogix_dp_write(dp, ANALOGIX_DP_INT_STA_MASK, 0x00);
}

void analogix_dp_reset(struct analogix_dp_device *dp)
{
	u32 reg;

	analogix_dp_stop_video(dp);
	analogix_dp_enable_video_mute(dp, 0);

	reg = MASTER_VID_FUNC_EN_N | SLAVE_VID_FUNC_EN_N |
		AUD_FIFO_FUNC_EN_N | AUD_FUNC_EN_N |
		HDCP_FUNC_EN_N | SW_FUNC_EN_N;
	analogix_dp_write(dp, ANALOGIX_DP_FUNC_EN_1, reg);

	reg = SSC_FUNC_EN_N | AUX_FUNC_EN_N |
		SERDES_FIFO_FUNC_EN_N |
		LS_CLK_DOMAIN_FUNC_EN_N;
	analogix_dp_write(dp, ANALOGIX_DP_FUNC_EN_2, reg);

	udelay(30);

	analogix_dp_set_lane_map(dp);

	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_1, 0x0);
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_2, 0x40);
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_3, 0x0);
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_4, 0x0);

	analogix_dp_write(dp, ANALOGIX_DP_PKT_SEND_CTL, 0x0);
	analogix_dp_write(dp, ANALOGIX_DP_HDCP_CTL, 0x0);

	analogix_dp_write(dp, ANALOGIX_DP_HPD_DEGLITCH_L, 0x5e);
	analogix_dp_write(dp, ANALOGIX_DP_HPD_DEGLITCH_H, 0x1a);

	analogix_dp_write(dp, ANALOGIX_DP_LINK_DEBUG_CTL, 0x10);

	analogix_dp_write(dp, ANALOGIX_DP_PHY_TEST, 0x0);

	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_FIFO_THRD, 0x0);
	analogix_dp_write(dp, ANALOGIX_DP_AUDIO_MARGIN, 0x20);

	analogix_dp_write(dp, ANALOGIX_DP_M_VID_GEN_FILTER_TH, 0x4);
	analogix_dp_write(dp, ANALOGIX_DP_M_AUD_GEN_FILTER_TH, 0x2);

	analogix_dp_write(dp, ANALOGIX_DP_SOC_GENERAL_CTL, 0x00000101);
}

void analogix_dp_swreset(struct analogix_dp_device *dp)
{
	analogix_dp_write(dp, ANALOGIX_DP_TX_SW_RESET, RESET_DP_TX);
}

void analogix_dp_config_interrupt(struct analogix_dp_device *dp)
{
	u32 reg;

	/* 0: mask, 1: unmask */
	reg = COMMON_INT_MASK_1;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_1, reg);

	reg = COMMON_INT_MASK_2;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_2, reg);

	reg = COMMON_INT_MASK_3;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_3, reg);

	reg = COMMON_INT_MASK_4;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_4, reg);

	reg = INT_STA_MASK;
	analogix_dp_write(dp, ANALOGIX_DP_INT_STA_MASK, reg);
}

void analogix_dp_mute_hpd_interrupt(struct analogix_dp_device *dp)
{
	u32 reg;

	/* 0: mask, 1: unmask */
	reg = analogix_dp_read(dp, ANALOGIX_DP_COMMON_INT_MASK_4);
	reg &= ~COMMON_INT_MASK_4;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_4, reg);

	reg = analogix_dp_read(dp, ANALOGIX_DP_INT_STA_MASK);
	reg &= ~INT_STA_MASK;
	analogix_dp_write(dp, ANALOGIX_DP_INT_STA_MASK, reg);
}

void analogix_dp_unmute_hpd_interrupt(struct analogix_dp_device *dp)
{
	u32 reg;

	/* 0: mask, 1: unmask */
	reg = COMMON_INT_MASK_4;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_MASK_4, reg);

	reg = INT_STA_MASK;
	analogix_dp_write(dp, ANALOGIX_DP_INT_STA_MASK, reg);
}

enum pll_status analogix_dp_get_pll_lock_status(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_DEBUG_CTL);
	if (reg & PLL_LOCK)
		return PLL_LOCKED;
	else
		return PLL_UNLOCKED;
}

void analogix_dp_set_pll_power_down(struct analogix_dp_device *dp, bool enable)
{
	u32 reg;

	if (enable) {
		reg = analogix_dp_read(dp, ANALOGIX_DP_PLL_CTL);
		reg |= DP_PLL_PD;
		analogix_dp_write(dp, ANALOGIX_DP_PLL_CTL, reg);
	} else {
		reg = analogix_dp_read(dp, ANALOGIX_DP_PLL_CTL);
		reg &= ~DP_PLL_PD;
		analogix_dp_write(dp, ANALOGIX_DP_PLL_CTL, reg);
	}
}

void analogix_dp_set_analog_power_down(struct analogix_dp_device *dp,
				       enum analog_power_block block,
				       bool enable)
{
	u32 reg;
	u32 phy_pd_addr = ANALOGIX_DP_PHY_PD;

	if (dp->plat_data.dev_type == ROCKCHIP_DP)
		phy_pd_addr = ANALOGIX_DP_PD;

	switch (block) {
	case AUX_BLOCK:
		if (enable) {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg |= AUX_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		} else {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg &= ~AUX_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		}
		break;
	case CH0_BLOCK:
		if (enable) {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg |= CH0_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		} else {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg &= ~CH0_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		}
		break;
	case CH1_BLOCK:
		if (enable) {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg |= CH1_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		} else {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg &= ~CH1_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		}
		break;
	case CH2_BLOCK:
		if (enable) {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg |= CH2_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		} else {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg &= ~CH2_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		}
		break;
	case CH3_BLOCK:
		if (enable) {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg |= CH3_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		} else {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg &= ~CH3_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		}
		break;
	case ANALOG_TOTAL:
		if (enable) {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg |= DP_PHY_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		} else {
			reg = analogix_dp_read(dp, phy_pd_addr);
			reg &= ~DP_PHY_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		}
		break;
	case POWER_ALL:
		if (enable) {
			reg = DP_PHY_PD | AUX_PD | CH3_PD | CH2_PD |
				CH1_PD | CH0_PD;
			analogix_dp_write(dp, phy_pd_addr, reg);
		} else {
			analogix_dp_write(dp, phy_pd_addr, 0x00);
		}
		break;
	default:
		break;
	}
}

void analogix_dp_init_analog_func(struct analogix_dp_device *dp)
{
	u32 reg;

	analogix_dp_set_analog_power_down(dp, POWER_ALL, 0);

	reg = PLL_LOCK_CHG;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_STA_1, reg);

	reg = analogix_dp_read(dp, ANALOGIX_DP_DEBUG_CTL);
	reg &= ~(F_PLL_LOCK | PLL_LOCK_CTRL);
	analogix_dp_write(dp, ANALOGIX_DP_DEBUG_CTL, reg);

	/* Power up PLL */
	analogix_dp_set_pll_power_down(dp, 0);

	/* Enable Serdes FIFO function and Link symbol clock domain module */
	reg = analogix_dp_read(dp, ANALOGIX_DP_FUNC_EN_2);
	reg &= ~(SERDES_FIFO_FUNC_EN_N | LS_CLK_DOMAIN_FUNC_EN_N
		| AUX_FUNC_EN_N);
	analogix_dp_write(dp, ANALOGIX_DP_FUNC_EN_2, reg);
}

void analogix_dp_clear_hotplug_interrupts(struct analogix_dp_device *dp)
{
	u32 reg;

	if (dm_gpio_is_valid(&dp->hpd_gpio))
		return;

	reg = HOTPLUG_CHG | HPD_LOST | PLUG;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_STA_4, reg);

	reg = INT_HPD;
	analogix_dp_write(dp, ANALOGIX_DP_INT_STA, reg);
}

void analogix_dp_init_hpd(struct analogix_dp_device *dp)
{
	u32 reg;

	if (dm_gpio_is_valid(&dp->hpd_gpio))
		return;

	analogix_dp_clear_hotplug_interrupts(dp);

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_3);
	reg &= ~(F_HPD | HPD_CTRL);
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_3, reg);
}

void analogix_dp_force_hpd(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_3);
	reg |= (F_HPD | HPD_CTRL);
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_3, reg);
}

enum dp_irq_type analogix_dp_get_irq_type(struct analogix_dp_device *dp)
{
	u32 reg;

	if (dm_gpio_is_valid(&dp->hpd_gpio)) {
		reg = dm_gpio_get_value(&dp->hpd_gpio);
		if (reg)
			return DP_IRQ_TYPE_HP_CABLE_IN;
		else
			return DP_IRQ_TYPE_HP_CABLE_OUT;
	} else {
		/* Parse hotplug interrupt status register */
		reg = analogix_dp_read(dp, ANALOGIX_DP_COMMON_INT_STA_4);

		if (reg & PLUG)
			return DP_IRQ_TYPE_HP_CABLE_IN;

		if (reg & HPD_LOST)
			return DP_IRQ_TYPE_HP_CABLE_OUT;

		if (reg & HOTPLUG_CHG)
			return DP_IRQ_TYPE_HP_CHANGE;

		return DP_IRQ_TYPE_UNKNOWN;
	}
}

void analogix_dp_reset_aux(struct analogix_dp_device *dp)
{
	u32 reg;

	/* Disable AUX channel module */
	reg = analogix_dp_read(dp, ANALOGIX_DP_FUNC_EN_2);
	reg |= AUX_FUNC_EN_N;
	analogix_dp_write(dp, ANALOGIX_DP_FUNC_EN_2, reg);
}

void analogix_dp_init_aux(struct analogix_dp_device *dp)
{
	u32 reg;

	/* Clear inerrupts related to AUX channel */
	reg = RPLY_RECEIV | AUX_ERR;
	analogix_dp_write(dp, ANALOGIX_DP_INT_STA, reg);

	analogix_dp_reset_aux(dp);

	/* Disable AUX transaction H/W retry */
	if (dp->plat_data.dev_type == ROCKCHIP_DP)
		reg = AUX_BIT_PERIOD_EXPECTED_DELAY(0) |
		      AUX_HW_RETRY_COUNT_SEL(3) |
		      AUX_HW_RETRY_INTERVAL_600_MICROSECONDS;
	else
		reg = AUX_BIT_PERIOD_EXPECTED_DELAY(3) |
		      AUX_HW_RETRY_COUNT_SEL(0) |
		      AUX_HW_RETRY_INTERVAL_600_MICROSECONDS;
	analogix_dp_write(dp, ANALOGIX_DP_AUX_HW_RETRY_CTL, reg);

	/* Receive AUX Channel DEFER commands equal to DEFFER_COUNT*64 */
	reg = DEFER_CTRL_EN | DEFER_COUNT(1);
	analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_DEFER_CTL, reg);

	/* Enable AUX channel module */
	reg = analogix_dp_read(dp, ANALOGIX_DP_FUNC_EN_2);
	reg &= ~AUX_FUNC_EN_N;
	analogix_dp_write(dp, ANALOGIX_DP_FUNC_EN_2, reg);
}

int analogix_dp_detect(struct analogix_dp_device *dp)
{
	u32 reg;

	if (dm_gpio_is_valid(&dp->hpd_gpio))
		return dm_gpio_get_value(&dp->hpd_gpio);

	if (dp->force_hpd)
		analogix_dp_force_hpd(dp);

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_3);
	if (reg & HPD_STATUS)
		return 1;

	return 0;
}

void analogix_dp_enable_sw_function(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_FUNC_EN_1);
	reg &= ~SW_FUNC_EN_N;
	analogix_dp_write(dp, ANALOGIX_DP_FUNC_EN_1, reg);
}

int analogix_dp_get_plug_in_status(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_3);
	if (reg & HPD_STATUS)
		return 0;

	return -EINVAL;
}

int analogix_dp_start_aux_transaction(struct analogix_dp_device *dp)
{
	int reg;
	int retval = 0;
	int timeout_loop = 0;

	/* Enable AUX CH operation */
	reg = analogix_dp_read(dp, ANALOGIX_DP_AUX_CH_CTL_2);
	reg |= AUX_EN;
	analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_2, reg);

	/* Is AUX CH command reply received? */
	reg = analogix_dp_read(dp, ANALOGIX_DP_INT_STA);
	while (!(reg & RPLY_RECEIV)) {
		timeout_loop++;
		if (DP_TIMEOUT_LOOP_COUNT < timeout_loop) {
			dev_err(dp->dev, "AUX CH command reply failed!\n");
			return -ETIMEDOUT;
		}

		reg = analogix_dp_read(dp, ANALOGIX_DP_INT_STA);
		udelay(11);
	}

	/* Clear interrupt source for AUX CH command reply */
	analogix_dp_write(dp, ANALOGIX_DP_INT_STA, reg);

	/* Clear interrupt source for AUX CH access error */
	reg = analogix_dp_read(dp, ANALOGIX_DP_INT_STA);
	if (reg & AUX_ERR) {
		analogix_dp_write(dp, ANALOGIX_DP_INT_STA, AUX_ERR);
		return -EREMOTEIO;
	}

	/* Check AUX CH error access status */
	reg = analogix_dp_read(dp, ANALOGIX_DP_AUX_CH_STA);
	if ((reg & AUX_STATUS_MASK) != 0) {
		dev_err(dp->dev,
			"AUX CH error happens: %d\n", reg & AUX_STATUS_MASK);
		return -EREMOTEIO;
	}

	return retval;
}

int analogix_dp_write_byte_to_dpcd(struct analogix_dp_device *dp,
				   unsigned int reg_addr,
				   unsigned char data)
{
	u32 reg;
	int i;
	int retval;

	for (i = 0; i < 3; i++) {
		/* Clear AUX CH data buffer */
		reg = BUF_CLR;
		analogix_dp_write(dp, ANALOGIX_DP_BUFFER_DATA_CTL, reg);

		/* Select DPCD device address */
		reg = AUX_ADDR_7_0(reg_addr);
		analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_7_0, reg);
		reg = AUX_ADDR_15_8(reg_addr);
		analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_15_8, reg);
		reg = AUX_ADDR_19_16(reg_addr);
		analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_19_16, reg);

		/* Write data buffer */
		reg = (unsigned int)data;
		analogix_dp_write(dp, ANALOGIX_DP_BUF_DATA_0, reg);

		/*
		 * Set DisplayPort transaction and write 1 byte
		 * If bit 3 is 1, DisplayPort transaction.
		 * If Bit 3 is 0, I2C transaction.
		 */
		reg = AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_WRITE;
		analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_1, reg);

		/* Start AUX transaction */
		retval = analogix_dp_start_aux_transaction(dp);
		if (retval == 0)
			break;
	}

	return retval;
}

int analogix_dp_read_byte_from_dpcd(struct analogix_dp_device *dp,
				    unsigned int reg_addr,
				    unsigned char *data)
{
	u32 reg;
	int i;
	int retval;

	for (i = 0; i < 3; i++) {
		/* Clear AUX CH data buffer */
		reg = BUF_CLR;
		analogix_dp_write(dp, ANALOGIX_DP_BUFFER_DATA_CTL, reg);

		/* Select DPCD device address */
		reg = AUX_ADDR_7_0(reg_addr);
		analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_7_0, reg);
		reg = AUX_ADDR_15_8(reg_addr);
		analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_15_8, reg);
		reg = AUX_ADDR_19_16(reg_addr);
		analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_19_16, reg);

		/*
		 * Set DisplayPort transaction and read 1 byte
		 * If bit 3 is 1, DisplayPort transaction.
		 * If Bit 3 is 0, I2C transaction.
		 */
		reg = AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_READ;
		analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_1, reg);

		/* Start AUX transaction */
		retval = analogix_dp_start_aux_transaction(dp);
		if (retval == 0)
			break;
	}

	/* Read data buffer */
	reg = analogix_dp_read(dp, ANALOGIX_DP_BUF_DATA_0);
	*data = (unsigned char)(reg & 0xff);

	return retval;
}

int analogix_dp_write_bytes_to_dpcd(struct analogix_dp_device *dp,
				    unsigned int reg_addr,
				    unsigned int count,
				    unsigned char data[])
{
	u32 reg;
	unsigned int start_offset;
	unsigned int cur_data_count;
	unsigned int cur_data_idx;
	int i;
	int retval = 0;

	/* Clear AUX CH data buffer */
	reg = BUF_CLR;
	analogix_dp_write(dp, ANALOGIX_DP_BUFFER_DATA_CTL, reg);

	start_offset = 0;
	while (start_offset < count) {
		/* Buffer size of AUX CH is 16 * 4bytes */
		if ((count - start_offset) > 16)
			cur_data_count = 16;
		else
			cur_data_count = count - start_offset;

		for (i = 0; i < 3; i++) {
			/* Select DPCD device address */
			reg = AUX_ADDR_7_0(reg_addr + start_offset);
			analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_7_0, reg);
			reg = AUX_ADDR_15_8(reg_addr + start_offset);
			analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_15_8, reg);
			reg = AUX_ADDR_19_16(reg_addr + start_offset);
			analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_19_16, reg);

			for (cur_data_idx = 0; cur_data_idx < cur_data_count;
			     cur_data_idx++) {
				reg = data[start_offset + cur_data_idx];
				analogix_dp_write(dp, ANALOGIX_DP_BUF_DATA_0 +
				       4 * cur_data_idx, reg);
			}

			/*
			 * Set DisplayPort transaction and write
			 * If bit 3 is 1, DisplayPort transaction.
			 * If Bit 3 is 0, I2C transaction.
			 */
			reg = AUX_LENGTH(cur_data_count) |
				AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_WRITE;
			analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_1, reg);

			/* Start AUX transaction */
			retval = analogix_dp_start_aux_transaction(dp);
			if (retval == 0)
				break;
		}

		start_offset += cur_data_count;
	}

	return retval;
}

int analogix_dp_read_bytes_from_dpcd(struct analogix_dp_device *dp,
				     unsigned int reg_addr,
				     unsigned int count,
				     unsigned char data[])
{
	u32 reg;
	unsigned int start_offset;
	unsigned int cur_data_count;
	unsigned int cur_data_idx;
	int i;
	int retval = 0;

	/* Clear AUX CH data buffer */
	reg = BUF_CLR;
	analogix_dp_write(dp, ANALOGIX_DP_BUFFER_DATA_CTL, reg);

	start_offset = 0;
	while (start_offset < count) {
		/* Buffer size of AUX CH is 16 * 4bytes */
		if ((count - start_offset) > 16)
			cur_data_count = 16;
		else
			cur_data_count = count - start_offset;

		/* AUX CH Request Transaction process */
		for (i = 0; i < 3; i++) {
			/* Select DPCD device address */
			reg = AUX_ADDR_7_0(reg_addr + start_offset);
			analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_7_0, reg);
			reg = AUX_ADDR_15_8(reg_addr + start_offset);
			analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_15_8, reg);
			reg = AUX_ADDR_19_16(reg_addr + start_offset);
			analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_19_16, reg);

			/*
			 * Set DisplayPort transaction and read
			 * If bit 3 is 1, DisplayPort transaction.
			 * If Bit 3 is 0, I2C transaction.
			 */
			reg = AUX_LENGTH(cur_data_count) |
				AUX_TX_COMM_DP_TRANSACTION | AUX_TX_COMM_READ;
			analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_1, reg);

			/* Start AUX transaction */
			retval = analogix_dp_start_aux_transaction(dp);
			if (retval == 0)
				break;
		}

		for (cur_data_idx = 0; cur_data_idx < cur_data_count;
		    cur_data_idx++) {
			reg = analogix_dp_read(dp, ANALOGIX_DP_BUF_DATA_0
						 + 4 * cur_data_idx);
			data[start_offset + cur_data_idx] =
				(unsigned char)reg;
		}

		start_offset += cur_data_count;
	}

	return retval;
}

int analogix_dp_select_i2c_device(struct analogix_dp_device *dp,
				  unsigned int device_addr,
				  unsigned int reg_addr)
{
	u32 reg;
	int retval;

	/* Set EDID device address */
	reg = device_addr;
	analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_7_0, reg);
	analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_15_8, 0x0);
	analogix_dp_write(dp, ANALOGIX_DP_AUX_ADDR_19_16, 0x0);

	/* Set offset from base address of EDID device */
	analogix_dp_write(dp, ANALOGIX_DP_BUF_DATA_0, reg_addr);

	/*
	 * Set I2C transaction and write address
	 * If bit 3 is 1, DisplayPort transaction.
	 * If Bit 3 is 0, I2C transaction.
	 */
	reg = AUX_TX_COMM_I2C_TRANSACTION | AUX_TX_COMM_MOT |
		AUX_TX_COMM_WRITE;
	analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_1, reg);

	/* Start AUX transaction */
	retval = analogix_dp_start_aux_transaction(dp);
	if (retval < 0)
		return retval;

	return 0;
}

int analogix_dp_read_byte_from_i2c(struct analogix_dp_device *dp,
				   unsigned int device_addr,
				   unsigned int reg_addr,
				   unsigned int *data)
{
	u32 reg;
	int i;
	int retval;

	for (i = 0; i < 3; i++) {
		/* Clear AUX CH data buffer */
		reg = BUF_CLR;
		analogix_dp_write(dp, ANALOGIX_DP_BUFFER_DATA_CTL, reg);

		/* Select EDID device */
		retval = analogix_dp_select_i2c_device(dp, device_addr,
						       reg_addr);
		if (retval != 0)
			continue;

		/*
		 * Set I2C transaction and read data
		 * If bit 3 is 1, DisplayPort transaction.
		 * If Bit 3 is 0, I2C transaction.
		 */
		reg = AUX_TX_COMM_I2C_TRANSACTION |
			AUX_TX_COMM_READ;
		analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_1, reg);

		/* Start AUX transaction */
		retval = analogix_dp_start_aux_transaction(dp);
		if (retval == 0)
			break;
	}

	/* Read data */
	if (retval == 0)
		*data = analogix_dp_read(dp, ANALOGIX_DP_BUF_DATA_0);

	return retval;
}

int analogix_dp_read_bytes_from_i2c(struct analogix_dp_device *dp,
				    unsigned int device_addr,
				    unsigned int reg_addr,
				    unsigned int count,
				    unsigned char edid[])
{
	u32 reg;
	unsigned int i, j;
	unsigned int cur_data_idx;
	unsigned int defer = 0;
	int retval = 0;

	for (i = 0; i < count; i += 16) {
		for (j = 0; j < 3; j++) {
			/* Clear AUX CH data buffer */
			reg = BUF_CLR;
			analogix_dp_write(dp, ANALOGIX_DP_BUFFER_DATA_CTL, reg);

			/* Set normal AUX CH command */
			reg = analogix_dp_read(dp, ANALOGIX_DP_AUX_CH_CTL_2);
			reg &= ~ADDR_ONLY;
			analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_2, reg);

			/*
			 * If Rx sends defer, Tx sends only reads
			 * request without sending address
			 */
			if (!defer)
				retval = analogix_dp_select_i2c_device(dp,
						device_addr, reg_addr + i);
			else
				defer = 0;

			if (retval == 0) {
				/*
				 * Set I2C transaction and write data
				 * If bit 3 is 1, DisplayPort transaction.
				 * If Bit 3 is 0, I2C transaction.
				 */
				reg = AUX_LENGTH(16) |
					AUX_TX_COMM_I2C_TRANSACTION |
					AUX_TX_COMM_READ;
				analogix_dp_write(dp, ANALOGIX_DP_AUX_CH_CTL_1,
						  reg);

				/* Start AUX transaction */
				retval = analogix_dp_start_aux_transaction(dp);
				if (retval == 0)
					break;
			}
			/* Check if Rx sends defer */
			reg = analogix_dp_read(dp, ANALOGIX_DP_AUX_RX_COMM);
			if (reg == AUX_RX_COMM_AUX_DEFER ||
			    reg == AUX_RX_COMM_I2C_DEFER) {
				dev_dbg(dp->dev, "Defer: %d\n\n", reg);
				defer = 1;
			}
		}

		for (cur_data_idx = 0; cur_data_idx < 16; cur_data_idx++) {
			reg = analogix_dp_read(dp, ANALOGIX_DP_BUF_DATA_0
						 + 4 * cur_data_idx);
			edid[i + cur_data_idx] = (unsigned char)reg;
		}
	}

	return retval;
}

bool analogix_dp_ssc_supported(struct analogix_dp_device *dp)
{
	/* Check if SSC is supported by both sides */
	return dp->plat_data.ssc && dp->link_train.ssc;
}

void analogix_dp_set_link_bandwidth(struct analogix_dp_device *dp, u32 bwtype)
{
	union phy_configure_opts phy_cfg;
	u32 status;
	int ret;

	analogix_dp_write(dp, ANALOGIX_DP_LINK_BW_SET, bwtype);

	phy_cfg.dp.lanes = dp->link_train.lane_count;
	phy_cfg.dp.link_rate =
		drm_dp_bw_code_to_link_rate(dp->link_train.link_rate) / 100;
	phy_cfg.dp.ssc = analogix_dp_ssc_supported(dp);
	phy_cfg.dp.set_lanes = false;
	phy_cfg.dp.set_rate = true;
	phy_cfg.dp.set_voltages = false;
	ret = generic_phy_configure(&dp->phy, &phy_cfg);
	if (ret) {
		dev_err(dp->dev, "%s: phy_configure() failed: %d\n",
			__func__, ret);
		return;
	}

	ret = readx_poll_timeout(analogix_dp_get_pll_lock_status, dp, status,
				 status != PLL_UNLOCKED,
				 120 * DP_TIMEOUT_LOOP_COUNT);
	if (ret) {
		dev_err(dp->dev, "Wait for pll lock failed %d\n", ret);
		return;
	}
}

void analogix_dp_get_link_bandwidth(struct analogix_dp_device *dp, u32 *bwtype)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_LINK_BW_SET);
	*bwtype = reg;
}

void analogix_dp_set_lane_count(struct analogix_dp_device *dp, u32 count)
{
	union phy_configure_opts phy_cfg;
	u32 reg;
	int ret;

	reg = count;
	analogix_dp_write(dp, ANALOGIX_DP_LANE_COUNT_SET, reg);

	phy_cfg.dp.lanes = dp->link_train.lane_count;
	phy_cfg.dp.set_lanes = true;
	phy_cfg.dp.set_rate = false;
	phy_cfg.dp.set_voltages = false;
	ret = generic_phy_configure(&dp->phy, &phy_cfg);
	if (ret) {
		dev_err(dp->dev, "%s: phy_configure() failed: %d\n",
			__func__, ret);
		return;
	}
}

void analogix_dp_get_lane_count(struct analogix_dp_device *dp, u32 *count)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_LANE_COUNT_SET);
	*count = reg;
}

void analogix_dp_set_lane_link_training(struct analogix_dp_device *dp)
{
	union phy_configure_opts phy_cfg;
	u8 lane;
	int ret;

	for (lane = 0; lane < dp->link_train.lane_count; lane++) {
		u8 training_lane = dp->link_train.training_lane[lane];
		u8 vs, pe;

		analogix_dp_write(dp,
				  ANALOGIX_DP_LN0_LINK_TRAINING_CTL + 4 * lane,
				  dp->link_train.training_lane[lane]);

		vs = (training_lane & DP_TRAIN_VOLTAGE_SWING_MASK) >>
		     DP_TRAIN_VOLTAGE_SWING_SHIFT;
		pe = (training_lane & DP_TRAIN_PRE_EMPHASIS_MASK) >>
		     DP_TRAIN_PRE_EMPHASIS_SHIFT;
		phy_cfg.dp.voltage[lane] = vs;
		phy_cfg.dp.pre[lane] = pe;
	}

	phy_cfg.dp.lanes = dp->link_train.lane_count;
	phy_cfg.dp.link_rate =
		drm_dp_bw_code_to_link_rate(dp->link_train.link_rate) / 100;
	phy_cfg.dp.set_lanes = false;
	phy_cfg.dp.set_rate = false;
	phy_cfg.dp.set_voltages = true;
	ret = generic_phy_configure(&dp->phy, &phy_cfg);
	if (ret) {
		dev_err(dp->dev, "%s: phy_configure() failed: %d\n",
			__func__, ret);
		return;
	}
}

u32 analogix_dp_get_lane_link_training(struct analogix_dp_device *dp, u8 lane)
{
	return analogix_dp_read(dp,
				ANALOGIX_DP_LN0_LINK_TRAINING_CTL + 4 * lane);
}

void analogix_dp_enable_enhanced_mode(struct analogix_dp_device *dp,
				      bool enable)
{
	u32 reg;

	if (enable) {
		reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_4);
		reg |= ENHANCED;
		analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_4, reg);
	} else {
		reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_4);
		reg &= ~ENHANCED;
		analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_4, reg);
	}
}

void analogix_dp_set_training_pattern(struct analogix_dp_device *dp,
				      enum pattern_set pattern)
{
	u32 reg;

	switch (pattern) {
	case PRBS7:
		reg = SCRAMBLING_ENABLE | LINK_QUAL_PATTERN_SET_PRBS7;
		analogix_dp_write(dp, ANALOGIX_DP_TRAINING_PTN_SET, reg);
		break;
	case D10_2:
		reg = SCRAMBLING_ENABLE | LINK_QUAL_PATTERN_SET_D10_2;
		analogix_dp_write(dp, ANALOGIX_DP_TRAINING_PTN_SET, reg);
		break;
	case TRAINING_PTN1:
		reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN1;
		analogix_dp_write(dp, ANALOGIX_DP_TRAINING_PTN_SET, reg);
		break;
	case TRAINING_PTN2:
		reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN2;
		analogix_dp_write(dp, ANALOGIX_DP_TRAINING_PTN_SET, reg);
		break;
	case TRAINING_PTN3:
		reg = SCRAMBLING_DISABLE | SW_TRAINING_PATTERN_SET_PTN3;
		analogix_dp_write(dp, ANALOGIX_DP_TRAINING_PTN_SET, reg);
		break;
	case DP_NONE:
		reg = SCRAMBLING_ENABLE |
			LINK_QUAL_PATTERN_SET_DISABLE |
			SW_TRAINING_PATTERN_SET_NORMAL;
		analogix_dp_write(dp, ANALOGIX_DP_TRAINING_PTN_SET, reg);
		break;
	default:
		break;
	}
}

void analogix_dp_reset_macro(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_PHY_TEST);
	reg |= MACRO_RST;
	analogix_dp_write(dp, ANALOGIX_DP_PHY_TEST, reg);

	/* 10 us is the minimum reset time. */
	udelay(20);

	reg &= ~MACRO_RST;
	analogix_dp_write(dp, ANALOGIX_DP_PHY_TEST, reg);
}

void analogix_dp_init_video(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = VSYNC_DET | VID_FORMAT_CHG | VID_CLK_CHG;
	analogix_dp_write(dp, ANALOGIX_DP_COMMON_INT_STA_1, reg);

	reg = 0x0;
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_1, reg);

	reg = CHA_CRI(4) | CHA_CTRL;
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_2, reg);

	if (dp->video_info.force_stream_valid) {
		reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_3);
		reg |= VALID_CTRL | F_VALID;
		analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_3, reg);
	}

	reg = VID_HRES_TH(2) | VID_VRES_TH(0);
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_8, reg);
}

void analogix_dp_set_video_color_format(struct analogix_dp_device *dp)
{
	u32 reg;

	/* Configure the input color depth, color space, dynamic range */
	reg = (dp->video_info.dynamic_range << IN_D_RANGE_SHIFT) |
		(dp->video_info.color_depth << IN_BPC_SHIFT) |
		(dp->video_info.color_space << IN_COLOR_F_SHIFT);
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_2, reg);

	/* Set Input Color YCbCr Coefficients to ITU601 or ITU709 */
	reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_3);
	reg &= ~IN_YC_COEFFI_MASK;
	if (dp->video_info.ycbcr_coeff)
		reg |= IN_YC_COEFFI_ITU709;
	else
		reg |= IN_YC_COEFFI_ITU601;
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_3, reg);
}

int analogix_dp_is_slave_video_stream_clock_on(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_1);
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_1, reg);

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_1);

	if (!(reg & DET_STA))
		return -EINVAL;

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_2);
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_2, reg);

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_2);

	if (reg & CHA_STA)
		return -EINVAL;

	return 0;
}

void analogix_dp_set_video_cr_mn(struct analogix_dp_device *dp,
				 enum clock_recovery_m_value_type type,
				 u32 m_value, u32 n_value)
{
	u32 reg;

	if (type == REGISTER_M) {
		reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_4);
		reg |= FIX_M_VID;
		analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_4, reg);
		reg = m_value & 0xff;
		analogix_dp_write(dp, ANALOGIX_DP_M_VID_0, reg);
		reg = (m_value >> 8) & 0xff;
		analogix_dp_write(dp, ANALOGIX_DP_M_VID_1, reg);
		reg = (m_value >> 16) & 0xff;
		analogix_dp_write(dp, ANALOGIX_DP_M_VID_2, reg);

		reg = n_value & 0xff;
		analogix_dp_write(dp, ANALOGIX_DP_N_VID_0, reg);
		reg = (n_value >> 8) & 0xff;
		analogix_dp_write(dp, ANALOGIX_DP_N_VID_1, reg);
		reg = (n_value >> 16) & 0xff;
		analogix_dp_write(dp, ANALOGIX_DP_N_VID_2, reg);
	} else  {
		reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_4);
		reg &= ~FIX_M_VID;
		analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_4, reg);

		analogix_dp_write(dp, ANALOGIX_DP_N_VID_0, 0x00);
		analogix_dp_write(dp, ANALOGIX_DP_N_VID_1, 0x80);
		analogix_dp_write(dp, ANALOGIX_DP_N_VID_2, 0x00);
	}
}

void analogix_dp_set_video_timing_mode(struct analogix_dp_device *dp, u32 type)
{
	u32 reg;

	if (type == VIDEO_TIMING_FROM_CAPTURE) {
		reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_10);
		reg &= ~FORMAT_SEL;
		analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_10, reg);
	} else {
		reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_10);
		reg |= FORMAT_SEL;
		analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_10, reg);
	}
}

void analogix_dp_enable_video_master(struct analogix_dp_device *dp, bool enable)
{
	u32 reg;

	if (enable) {
		reg = analogix_dp_read(dp, ANALOGIX_DP_SOC_GENERAL_CTL);
		reg &= ~VIDEO_MODE_MASK;
		reg |= VIDEO_MASTER_MODE_EN | VIDEO_MODE_MASTER_MODE;
		analogix_dp_write(dp, ANALOGIX_DP_SOC_GENERAL_CTL, reg);
	} else {
		reg = analogix_dp_read(dp, ANALOGIX_DP_SOC_GENERAL_CTL);
		reg &= ~VIDEO_MODE_MASK;
		reg |= VIDEO_MODE_SLAVE_MODE;
		analogix_dp_write(dp, ANALOGIX_DP_SOC_GENERAL_CTL, reg);
	}
}

void analogix_dp_start_video(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_1);
	reg |= VIDEO_EN;
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_1, reg);
}

int analogix_dp_is_video_stream_on(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_3);
	analogix_dp_write(dp, ANALOGIX_DP_SYS_CTL_3, reg);

	reg = analogix_dp_read(dp, ANALOGIX_DP_SYS_CTL_3);
	if (!(reg & STRM_VALID))
		return -EINVAL;

	return 0;
}

void analogix_dp_config_video_slave_mode(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_FUNC_EN_1);
	reg &= ~(MASTER_VID_FUNC_EN_N | SLAVE_VID_FUNC_EN_N);
	reg |= MASTER_VID_FUNC_EN_N;
	analogix_dp_write(dp, ANALOGIX_DP_FUNC_EN_1, reg);

	reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_10);
	reg &= ~INTERACE_SCAN_CFG;
	reg |= (dp->video_info.interlaced << 2);
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_10, reg);

	reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_10);
	reg &= ~VSYNC_POLARITY_CFG;
	reg |= (dp->video_info.v_sync_polarity << 1);
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_10, reg);

	reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_10);
	reg &= ~HSYNC_POLARITY_CFG;
	reg |= (dp->video_info.h_sync_polarity << 0);
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_10, reg);

	reg = AUDIO_MODE_SPDIF_MODE | VIDEO_MODE_SLAVE_MODE;
	analogix_dp_write(dp, ANALOGIX_DP_SOC_GENERAL_CTL, reg);
}

void analogix_dp_enable_scrambling(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_TRAINING_PTN_SET);
	reg &= ~SCRAMBLING_DISABLE;
	analogix_dp_write(dp, ANALOGIX_DP_TRAINING_PTN_SET, reg);
}

void analogix_dp_disable_scrambling(struct analogix_dp_device *dp)
{
	u32 reg;

	reg = analogix_dp_read(dp, ANALOGIX_DP_TRAINING_PTN_SET);
	reg |= SCRAMBLING_DISABLE;
	analogix_dp_write(dp, ANALOGIX_DP_TRAINING_PTN_SET, reg);
}

void analogix_dp_set_video_format(struct analogix_dp_device *dp,
				  const struct drm_display_mode *mode)
{
	unsigned int hsw, hfp, hbp, vsw, vfp, vbp;

	dp->video_info.interlaced = !!(mode->flags & DRM_MODE_FLAG_INTERLACE);

	if (dp->plat_data.subdev_type == RK3588_EDP) {
		dp->video_info.v_sync_polarity = true;
		dp->video_info.h_sync_polarity = true;
	} else {
		dp->video_info.v_sync_polarity = !!(mode->flags & DRM_MODE_FLAG_NVSYNC);
		dp->video_info.h_sync_polarity = !!(mode->flags & DRM_MODE_FLAG_NHSYNC);
	}

	hsw = mode->hsync_end - mode->hsync_start;
	hfp = mode->hsync_start - mode->hdisplay;
	hbp = mode->htotal - mode->hsync_end;
	vsw = mode->vsync_end - mode->vsync_start;
	vfp = mode->vsync_start - mode->vdisplay;
	vbp = mode->vtotal - mode->vsync_end;

	/* Set Video Format Parameters */
	analogix_dp_write(dp, ANALOGIX_DP_TOTAL_LINE_CFG_L,
			  TOTAL_LINE_CFG_L(mode->vtotal));
	analogix_dp_write(dp, ANALOGIX_DP_TOTAL_LINE_CFG_H,
			  TOTAL_LINE_CFG_H(mode->vtotal >> 8));
	analogix_dp_write(dp, ANALOGIX_DP_ACTIVE_LINE_CFG_L,
			  ACTIVE_LINE_CFG_L(mode->vdisplay));
	analogix_dp_write(dp, ANALOGIX_DP_ACTIVE_LINE_CFG_H,
			  ACTIVE_LINE_CFG_H(mode->vdisplay >> 8));
	analogix_dp_write(dp, ANALOGIX_DP_V_F_PORCH_CFG,
			  V_F_PORCH_CFG(vfp));
	analogix_dp_write(dp, ANALOGIX_DP_V_SYNC_WIDTH_CFG,
			  V_SYNC_WIDTH_CFG(vsw));
	analogix_dp_write(dp, ANALOGIX_DP_V_B_PORCH_CFG,
			  V_B_PORCH_CFG(vbp));
	analogix_dp_write(dp, ANALOGIX_DP_TOTAL_PIXEL_CFG_L,
			  TOTAL_PIXEL_CFG_L(mode->htotal));
	analogix_dp_write(dp, ANALOGIX_DP_TOTAL_PIXEL_CFG_H,
			  TOTAL_PIXEL_CFG_H(mode->htotal >> 8));
	analogix_dp_write(dp, ANALOGIX_DP_ACTIVE_PIXEL_CFG_L,
			  ACTIVE_PIXEL_CFG_L(mode->hdisplay));
	analogix_dp_write(dp, ANALOGIX_DP_ACTIVE_PIXEL_CFG_H,
			  ACTIVE_PIXEL_CFG_H(mode->hdisplay >> 8));
	analogix_dp_write(dp, ANALOGIX_DP_H_F_PORCH_CFG_L,
			  H_F_PORCH_CFG_L(hfp));
	analogix_dp_write(dp, ANALOGIX_DP_H_F_PORCH_CFG_H,
			  H_F_PORCH_CFG_H(hfp >> 8));
	analogix_dp_write(dp, ANALOGIX_DP_H_SYNC_CFG_L,
			  H_SYNC_CFG_L(hsw));
	analogix_dp_write(dp, ANALOGIX_DP_H_SYNC_CFG_H,
			  H_SYNC_CFG_H(hsw >> 8));
	analogix_dp_write(dp, ANALOGIX_DP_H_B_PORCH_CFG_L,
			  H_B_PORCH_CFG_L(hbp));
	analogix_dp_write(dp, ANALOGIX_DP_H_B_PORCH_CFG_H,
			  H_B_PORCH_CFG_H(hbp >> 8));
}

void analogix_dp_video_bist_enable(struct analogix_dp_device *dp)
{
	u32 reg;

	/* Enable Video BIST */
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_4, BIST_EN);

	/*
	 * Note that if BIST_EN is set to 1, F_SEL must be cleared to 0
	 * although video format information comes from registers set by user.
	 */
	reg = analogix_dp_read(dp, ANALOGIX_DP_VIDEO_CTL_10);
	reg &= ~FORMAT_SEL;
	analogix_dp_write(dp, ANALOGIX_DP_VIDEO_CTL_10, reg);
}
