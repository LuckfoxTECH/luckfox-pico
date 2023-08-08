// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 Rockchip Electronics Co. Ltd.
 *
 * Author: Wenping Zhang <wenping.zhang@rock-chips.com>
 */
#include <common.h>
#include <clk.h>
#include <dm.h>
#include <dm/pinctrl.h>
#include <pwm.h>
#include <regmap.h>
#include <syscon.h>
#include <asm/arch/clock.h>
#include <asm/io.h>
#include <syscon.h>
#include <linux/io.h>
#include "rk_ebc.h"
#ifdef CONFIG_IRQ
#include <irq-generic.h>
#endif

struct ebc_tcon_priv {
	struct udevice *dev;
	void __iomem *reg;
	u32 *regcache;
	u32 reg_len;
	void *grf;
	void *pmugrf;
	struct clk dclk;
};

#define msleep(a)		udelay((a) * 1000)
#define HIWORD_UPDATE(x, l, h)	(((x) << (l)) | (GENMASK(h, l) << 16))
#define UPDATE(x, h, l)		(((x) << (l)) & GENMASK((h), (l)))

#define REG_LOAD_GLOBAL_EN	0x1

/* ebc register define */
#define EBC_DSP_START		0x0000 //Frame statrt register
#define EBC_EPD_CTRL		0x0004 //EPD control register
#define EBC_DSP_CTRL		0x0008 //Display control register
#define EBC_DSP_HTIMING0	0x000c //H-Timing setting register0
#define EBC_DSP_HTIMING1	0x0010 //H-Timing setting register1
#define EBC_DSP_VTIMING0	0x0014 //V-Timing setting register0
#define EBC_DSP_VTIMING1	0x0018 //V-Timing setting register1
#define EBC_DSP_ACT_INFO	0x001c //ACTIVE width/height
#define EBC_WIN_CTRL		0x0020 //Window ctrl
#define EBC_WIN_MST0		0x0024 //Current win memory start
#define EBC_WIN_MST1		0x0028 //Next win memory start
#define EBC_WIN_VIR		0x002c //Window vir width/height
#define EBC_WIN_ACT		0x0030 //Window act width/height
#define EBC_WIN_DSP		0x0034 //Window dsp width/height
#define EBC_WIN_DSP_ST		0x0038 //Window display start point
#define EBC_INT_STATUS		0x003c //Interrupt register
#define EBC_VCOM0		0x0040 //VCOM setting register0
#define EBC_VCOM1		0x0044 //VCOM setting register1
#define EBC_VCOM2		0x0048 //VCOM setting register2
#define EBC_VCOM3		0x004c //VCOM setting register3
#define EBC_CONFIG_DONE		0x0050 //Config done register
#define EBC_VNUM		0x0054 //Line flag num
#define EBC_WIN_MST2		0x0058 //Framecount memory start
#define EBC_LUT_DATA_ADDR	0x1000 //lut data address

#define DSP_HTOTAL(x)		UPDATE(x, 27, 16)
#define DSP_HS_END(x)		UPDATE(x, 7, 0)
#define DSP_HACT_END(x)		UPDATE(x, 26, 16)
#define DSP_HACT_ST(x)		UPDATE(x, 7, 0)
#define DSP_VTOTAL(x)		UPDATE(x, 26, 16)
#define DSP_VS_END(x)		UPDATE(x, 7, 0)
#define DSP_VACT_END(x)		UPDATE(x, 26, 16)
#define DSP_VACT_ST(x)		UPDATE(x, 7, 0)
#define DSP_HEIGHT(x)		UPDATE(x, 26, 16)
#define DSP_WIDTH(x)		UPDATE(x, 11, 0)

#define WIN2_FIFO_ALMOST_FULL_LEVEL(x)	UPDATE(x, 27, 19)
#define WIN_EN(x)			UPDATE(x, 18, 18)
#define BURST_REG(x)			UPDATE(x, 12, 10)
#define WIN_FIFO_ALMOST_FULL_LEVEL(x)	UPDATE(x, 9, 2)
#define WIN_FMT(x)			UPDATE(x, 1, 0)

#define WIN_VIR_HEIGHT(x)		UPDATE(x, 31, 16)
#define WIN_VIR_WIDTH(x)		UPDATE(x, 15, 0)
#define WIN_ACT_HEIGHT(x)		UPDATE(x, 26, 16)
#define WIN_ACT_WIDTH(x)		UPDATE(x, 11, 0)
#define WIN_DSP_HEIGHT(x)		UPDATE(x, 26, 16)
#define WIN_DSP_WIDTH(x)		UPDATE(x, 11, 0)
#define WIN_DSP_YST(x)			UPDATE(x, 26, 16)
#define WIN_DSP_XST(x)			UPDATE(x, 11, 0)

#define DSP_OUT_LOW			BIT(31)
#define DSP_EINK_MODE(x)		UPDATE(x, 13, 13)
#define DSP_EINK_MODE_MASK		BIT(13)
#define DSP_SDCE_WIDTH(x)		UPDATE(x, 25, 16)
#define DSP_FRM_TOTAL(x)		UPDATE(x, 9, 2)
#define DSP_FRM_TOTAL_MASK		GENMASK(9, 2)
#define DSP_FRM_START			BIT(0)
#define DSP_FRM_START_MASK		BIT(0)
#define SW_BURST_CTRL			BIT(12)

#define EINK_MODE_SWAP(x)		UPDATE(x, 31, 31)
#define EINK_MODE_FRM_SEL(x)		UPDATE(x, 30, 30)
#define DSP_GD_END(x)			UPDATE(x, 26, 16)
#define DSP_GD_ST(x)			UPDATE(x, 15, 8)
#define DSP_THREE_WIN_MODE(x)		UPDATE(x, 7, 7)
#define THREE_WIN_MODE_MASK		BIT(7)
#define DSP_SDDW_MODE(x)		UPDATE(x, 6, 6)
#define EPD_AUO(x)			UPDATE(x, 5, 5)
#define EPD_PWR(x)			UPDATE(x, 4, 2)
#define EPD_GDRL(x)			UPDATE(x, 1, 1)
#define EPD_SDSHR(x)			UPDATE(x, 0, 0)

#define DSP_SWAP_MODE(x)		UPDATE(x, 31, 30)
#define DSP_SWAP_MODE_MASK		GENMASK(31, 30)
#define DSP_SDCLK_DIV(x)		UPDATE(x, 19, 16)
#define DSP_SDCLK_DIV_MASK		GENMASK(19, 16)
#define DSP_VCOM_MODE(x)		UPDATE(x, 27, 27)
#define DSP_VCOM_MODE_MASK		BIT(27)

#define DSP_UPDATE_MODE(x)	UPDATE(x, 29, 29)
#define DSP_DISPLAY_MODE(x)	UPDATE(x, 28, 28)
#define UPDATE_MODE_MASK	BIT(29)
#define DISPLAY_MODE_MASK	BIT(28)

#define DSP_FRM_INT_NUM(x)	UPDATE(x, 19, 12)
#define FRM_END_INT		BIT(0)
#define DSP_END_INT		BIT(1)
#define DSP_FRM_INT		BIT(2)
#define LINE_FLAG_INT		BIT(3)
#define FRM_END_INT_MASK	BIT(4)
#define DSP_END_INT_MASK	BIT(5)
#define DSP_FRM_INT_MASK	BIT(6)
#define LINE_FLAG_INT_MASK	BIT(7)
#define FRM_END_INT_CLR		BIT(8)
#define DSP_END_INT_CLR		BIT(9)
#define DSP_FRM_INT_CLR		BIT(10)
#define LINE_FLAG_INT_CLR	BIT(11)

#define PMU_BASE_ADDR		0xfdd90000
#define PMU_PWR_GATE_SFTCON	0xA0
#define PMU_PWR_DWN_ST		0x98
#define RGA_PD_OFF		BIT(5)
#define RGA_PD_STAT		BIT(5)
enum ebc_win_data_fmt {
	Y_DATA_4BPP = 0,
	Y_DATA_8BPP = 1,
	RGB888 = 2,
	RGB565 = 3,
};

#ifdef CONFIG_IRQ
#define IRQ_EBC			49
#endif
static volatile int last_frame_done = -1;
static inline void regs_dump(struct ebc_tcon_priv *tcon)
{
	int i;

	printf("dump registers:\n");
	for (i = 0; i <= EBC_WIN_MST2; i = i + 4) {
		if (!(i % 16))
			printf("\n 0x%p:\t", tcon->reg + i);
		printf("0x%x\t", readl(tcon->reg + i));
	}
	printf("\nlut data:\n");
	for (i = 0x1000; i <= 0x1100; i = i + 4) {
		if (!(i % 16))
			printf("\n 0x%p:\t", tcon->reg + i);
		printf("0x%x\t", readl(tcon->reg + i));
	}
	printf("\n");
}

static int ebc_power_domain(int on)
{
	u32 pd_reg;
	u32 pd_stat;
	int delay = 0;

	if (on) {
		pd_reg = RGA_PD_OFF << 16;
		pd_stat = RGA_PD_STAT;
	} else {
		pd_reg = RGA_PD_OFF | (RGA_PD_OFF << 16);
		pd_stat = ~((u32)RGA_PD_STAT);
	}

	/* enable rga pd for ebc tcon*/
	writel(pd_reg, PMU_BASE_ADDR + PMU_PWR_GATE_SFTCON);
	delay = 1000;
	do {
		udelay(1);
		delay--;
		if (delay == 0) {
			printf("Enable rga pd for ebc failed !\n");
			return -1;
		}
	} while (readl(PMU_BASE_ADDR + PMU_PWR_DWN_ST) & pd_stat);

	return 0;
}

static inline void tcon_write(struct ebc_tcon_priv *tcon, unsigned int reg,
			      unsigned int value)
{
	unsigned int *cache = tcon->regcache + (reg >> 2);

	writel(value, tcon->reg + reg);
	*cache = value;
}

static inline unsigned int tcon_read(struct ebc_tcon_priv *tcon,
				     unsigned int reg)
{
	return readl(tcon->reg + reg);
}

static inline void tcon_update_bits(struct ebc_tcon_priv *tcon,
				    unsigned int reg, unsigned int mask,
				    unsigned int val)
{
	unsigned int tmp;
	unsigned int *cache = tcon->regcache + (reg >> 2);

	tmp = *cache & ~mask;
	tmp |= val & mask;

	writel(tmp, tcon->reg + reg);
	*cache = tmp;
}

#ifdef CONFIG_IRQ
static void ebc_irq_handler(int irq, void *data)
{
	u32 intr_status;
	struct udevice *dev = data;
	struct ebc_tcon_priv *tcon = dev_get_priv(dev);

	intr_status = readl(tcon->reg + EBC_INT_STATUS);

	if (intr_status & DSP_END_INT) {
		tcon_update_bits(tcon, EBC_INT_STATUS,
				 DSP_END_INT_CLR, DSP_END_INT_CLR);
		last_frame_done = 1;
	}
}
#endif

static inline void tcon_cfg_done(struct ebc_tcon_priv *tcon)
{
	writel(REG_LOAD_GLOBAL_EN, tcon->reg + EBC_CONFIG_DONE);
}

static int ebc_tcon_enable(struct udevice *dev, struct ebc_panel *panel)
{
	int ret;
	struct ebc_tcon_priv *tcon = dev_get_priv(dev);
	u32 width, height, vir_width, vir_height;

	if (panel->rearrange) {
		width = panel->width * 2;
		height = panel->height / 2;
		vir_width = panel->vir_width * 2;
		vir_height = panel->vir_height / 2;
	} else {
		width = panel->width;
		height = panel->height;
		vir_width = panel->vir_width;
		vir_height = panel->vir_height;
	}

	/* panel timing and win info config */
	tcon_write(tcon, EBC_DSP_HTIMING0,
		   DSP_HTOTAL(panel->lsl + panel->lbl + panel->ldl +
			      panel->lel) | DSP_HS_END(panel->lsl));
	tcon_write(tcon, EBC_DSP_HTIMING1,
		   DSP_HACT_END(panel->lsl + panel->lbl + panel->ldl) |
		   DSP_HACT_ST(panel->lsl + panel->lbl - 1));
	tcon_write(tcon, EBC_DSP_VTIMING0,
		   DSP_VTOTAL(panel->fsl + panel->fbl + panel->fdl +
			      panel->fel) | DSP_VS_END(panel->fsl));
	tcon_write(tcon, EBC_DSP_VTIMING1,
		   DSP_VACT_END(panel->fsl + panel->fbl + panel->fdl) |
		   DSP_VACT_ST(panel->fsl + panel->fbl));
	tcon_write(tcon, EBC_DSP_ACT_INFO,
		   DSP_HEIGHT(height) |
		   DSP_WIDTH(width));
	tcon_write(tcon, EBC_WIN_VIR,
		   WIN_VIR_HEIGHT(vir_height) |
		   WIN_VIR_WIDTH(vir_width));
	tcon_write(tcon, EBC_WIN_ACT,
		   WIN_ACT_HEIGHT(height) |
		   WIN_ACT_WIDTH(width));
	tcon_write(tcon, EBC_WIN_DSP,
		   WIN_DSP_HEIGHT(height) |
		   WIN_DSP_WIDTH(width));
	tcon_write(tcon, EBC_WIN_DSP_ST,
		   WIN_DSP_YST(panel->fsl + panel->fbl) |
		   WIN_DSP_XST(panel->lsl + panel->lbl));

	/* win2 fifo is 512x128, win fifo is 256x128,
	 * we set fifo almost value (fifo_size - 16)
	 * burst_reg = 7 mean ahb burst is incr16
	 */
	tcon_write(tcon, EBC_WIN_CTRL,
		   WIN2_FIFO_ALMOST_FULL_LEVEL(496) | WIN_EN(1) |
		   BURST_REG(7) | WIN_FIFO_ALMOST_FULL_LEVEL(240) |
		   WIN_FMT(Y_DATA_4BPP));

	/*
	 * EBC_EPD_CTRL info:
	 * DSP_GD_ST: GCLK rising edge point(SCLK), which count from
	 *            the rasing edge of hsync(spec is wrong, count
	 *            from rasing edge of hsync, not falling edge of hsync)
	 * DSP_GD_END : GCLK falling edge point(SCLK), which count from
	 *              the rasing edge of hsync
	 * DSP_THREE_WIN_MODE: 0: lut mode or direct mode; 1: three win mode
	 * DSP_SDDW_MODE: 0: 8 bit data output; 1: 16 bit data output
	 * EPD_AUO: 0: EINK; 1:AUO
	 * EPD_GDRL: gate scanning direction: 1:button to top 0:top to button
	 * EPD_SDSHR: source scanning direction 1:right to left 0:left to right
	 */
	tcon_write(tcon, EBC_EPD_CTRL,
		   EINK_MODE_SWAP(1) |
		   DSP_GD_ST(panel->lsl + panel->gdck_sta) |
		   DSP_GD_END(panel->lsl + panel->gdck_sta + panel->lgonl) |
		   DSP_THREE_WIN_MODE(0) |
		   DSP_SDDW_MODE(!!panel->panel_16bit) |
		   EPD_AUO(0) |
		   EPD_GDRL(1) |
		   EPD_SDSHR(1));
	tcon_write(tcon, EBC_DSP_START,
		   DSP_SDCE_WIDTH(panel->ldl) | SW_BURST_CTRL);

	tcon_write(tcon, EBC_DSP_CTRL,
		   DSP_SWAP_MODE(panel->panel_16bit ? 2 : 3) |
		   DSP_VCOM_MODE(1) |
		   DSP_SDCLK_DIV(panel->panel_16bit ? 7 : 3));

	tcon_cfg_done(tcon);

	ret = clk_set_rate(&tcon->dclk, panel->sdck * ((panel->panel_16bit ? 7 : 3) + 1));
	if (ret < 0) {
		printf("%s: set clock rate failed, %d\n", __func__, ret);
		return ret;
	}

	return 0;
}

static int ebc_tcon_disable(struct udevice *dev)
{
	return 0;
}

static int ebc_tcon_dsp_mode_set(struct udevice *dev, int update_mode,
				 int display_mode, int three_win_mode,
				 int eink_mode)
{
	struct ebc_tcon_priv *tcon = dev_get_priv(dev);

	tcon_update_bits(tcon, EBC_DSP_CTRL,
			 UPDATE_MODE_MASK | DISPLAY_MODE_MASK,
			 DSP_UPDATE_MODE(!!update_mode) |
			 DSP_DISPLAY_MODE(!!display_mode));

	tcon_update_bits(tcon, EBC_EPD_CTRL, THREE_WIN_MODE_MASK,
			 DSP_THREE_WIN_MODE(!!three_win_mode));
	/* always set frm start bit 0 before real frame start */
	tcon_update_bits(tcon, EBC_DSP_START,
			 DSP_EINK_MODE_MASK | DSP_FRM_START_MASK,
			 DSP_EINK_MODE(!!eink_mode));
	tcon_cfg_done(tcon);

	return 0;
}

static int ebc_tcon_image_addr_set(struct udevice *dev, u32 pre_image_addr,
				   u32 cur_image_addr)
{
	struct ebc_tcon_priv *tcon = dev_get_priv(dev);

	tcon_write(tcon, EBC_WIN_MST0, pre_image_addr);
	tcon_write(tcon, EBC_WIN_MST1, cur_image_addr);
	tcon_cfg_done(tcon);

	return 0;
}

static int ebc_tcon_frame_addr_set(struct udevice *dev, u32 frame_addr)
{
	struct ebc_tcon_priv *tcon = dev_get_priv(dev);

	tcon_write(tcon, EBC_WIN_MST2, frame_addr);
	tcon_cfg_done(tcon);

	return 0;
}

static int ebc_tcon_lut_data_set(struct udevice *dev, unsigned int *lut_data,
				 int frame_count, int lut_32)
{
	int i, lut_size;
	struct ebc_tcon_priv *tcon = dev_get_priv(dev);

	if ((!lut_32 && frame_count > 256) || (lut_32 && frame_count > 64)) {
		dev_err(tcon->dev, "frame count over flow\n");
		return -1;
	}

	if (lut_32)
		lut_size = frame_count * 64;
	else
		lut_size = frame_count * 16;

	for (i = 0; i < lut_size; i++)
		tcon_write(tcon, EBC_LUT_DATA_ADDR + (i * 4), lut_data[i]);

	tcon_cfg_done(tcon);

	return 0;
}

static int wait_for_last_frame_complete(struct udevice *dev)
{
#ifndef CONFIG_IRQ
	u32 intr_status;
#endif
	struct ebc_tcon_priv *tcon = dev_get_priv(dev);

#ifdef CONFIG_IRQ
	while (1) {
		if ((last_frame_done == -1) || (last_frame_done == 1))
			break;
		msleep(1);
	}
#else
	/* wait for frame display end*/
	while (1) {
		/* first frame don't need to wait*/
		if (last_frame_done == -1)
			break;
		intr_status = readl(tcon->reg + EBC_INT_STATUS);
		if (intr_status & DSP_END_INT)
			break;
		msleep(1);
	}
#endif
	tcon_update_bits(tcon, EBC_INT_STATUS,
			 DSP_END_INT_CLR, DSP_END_INT_CLR);

	return 0;
}

static int ebc_tcon_frame_start(struct udevice *dev, int frame_total)
{
	struct ebc_tcon_priv *tcon = dev_get_priv(dev);

	tcon_write(tcon, EBC_INT_STATUS,
		   LINE_FLAG_INT_MASK | DSP_FRM_INT_MASK | FRM_END_INT_MASK);
	tcon_update_bits(tcon, EBC_DSP_START,
			 DSP_FRM_TOTAL_MASK, DSP_FRM_TOTAL(frame_total - 1));
	tcon_cfg_done(tcon);

	tcon_update_bits(tcon, EBC_DSP_START,
			 DSP_FRM_START_MASK, DSP_FRM_START);
	last_frame_done = 0;
	return 0;
}

static int rk_ebc_tcon_probe(struct udevice *dev)
{
	int ret;
	struct ebc_tcon_priv *priv = dev_get_priv(dev);

	/*Enable PD first*/
	ret = ebc_power_domain(1);
	if (ret) {
		printf("%s, enable pd failed\n", __func__);
		return -1;
	}

	priv->dev = dev;
	ret = clk_get_by_index(dev, 1, &priv->dclk);
	if (ret < 0) {
		printf("%s get clock fail! %d\n", __func__, ret);
		return -EINVAL;
	}

#ifdef CONFIG_IRQ
	irq_install_handler(IRQ_EBC, ebc_irq_handler, dev);
	irq_handler_enable(IRQ_EBC);
#endif
	return 0;
}

const struct rk_ebc_tcon_ops ebc_tcon_funcs = {
	.enable = ebc_tcon_enable,
	.disable = ebc_tcon_disable,
	.dsp_mode_set = ebc_tcon_dsp_mode_set,
	.image_addr_set = ebc_tcon_image_addr_set,
	.frame_addr_set = ebc_tcon_frame_addr_set,
	.lut_data_set = ebc_tcon_lut_data_set,
	.frame_start = ebc_tcon_frame_start,
	.wait_for_last_frame_complete = wait_for_last_frame_complete,
};

static int rk_ebc_tcon_ofdata_to_platdata(struct udevice *dev)
{
	fdt_size_t size;
	fdt_addr_t addr;
	struct ebc_tcon_priv *priv = dev_get_priv(dev);

	priv->grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
	if (priv->grf <= 0) {
		debug("%s: Get syscon grf failed (ret=%p)\n",
		      __func__, priv->grf);
		return  -ENXIO;
	}
	priv->pmugrf = syscon_get_first_range(ROCKCHIP_SYSCON_PMUGRF);
	if (priv->pmugrf <= 0) {
		debug("%s: Get syscon pmugrf failed (ret=%p)\n",
		      __func__, priv->grf);
		return  -ENXIO;
	}
	addr = dev_read_addr_size(dev, "reg", &size);
	if (addr == FDT_ADDR_T_NONE) {
		debug("%s: Get ebc_tcon address failed\n", __func__);
		return  -ENXIO;
	}

	priv->reg = ioremap(addr, size);
	priv->reg_len = size;
	priv->regcache = malloc(size);
	memset(priv->regcache, 0, size);
	return 0;
}

static const struct udevice_id ebc_tcon_ids[] = {
	{ .compatible = "rockchip,rk3568-ebc-tcon" },
	{ }
};

U_BOOT_DRIVER(rk_ebc_tcon) = {
	.name	= "rk_ebc_tcon",
	.id	= UCLASS_EBC,
	.of_match = ebc_tcon_ids,
	.ofdata_to_platdata = rk_ebc_tcon_ofdata_to_platdata,
	.probe	= rk_ebc_tcon_probe,
	.ops	= &ebc_tcon_funcs,
	.priv_auto_alloc_size   = sizeof(struct ebc_tcon_priv),
};

UCLASS_DRIVER(ebc_tcon) = {
	.id	= UCLASS_EBC,
	.name	= "ebc_tcon",
};

