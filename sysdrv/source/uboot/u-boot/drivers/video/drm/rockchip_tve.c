/*
 * SPDX-License-Identifier:	GPL-2.0+
 * (C) Copyright 2008-2015 Fuzhou Rockchip Electronics Co., Ltd
 */
#include <common.h>
#include <malloc.h>
#include <fdtdec.h>
#include <fdt_support.h>
#include <asm/io.h>
#include <linux/media-bus-format.h>
#include <asm/arch-rockchip/clock.h>
#include <dm/device.h>
#include <dm/read.h>
#include <dm/uclass-internal.h>
#include <linux/fb.h>
#include <edid.h>
#include <syscon.h>
#include <boot_rkimg.h>
#include <mapmem.h>
#include <misc.h>

#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_phy.h"
#include "rockchip_tve.h"

#define RK322X_VDAC_STANDARD		0x15

static const struct drm_display_mode tve_modes[] = {
	/* 0 - 720x576i@50Hz */
	{ DRM_MODE(DRM_MODE_TYPE_DRIVER, 13500, 720, 753,
		   816, 864, 576, 580, 586, 625, 0,
		   DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC |
		   DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
		   .vrefresh = 50, },
	/* 1 - 720x480i@60Hz */
	{ DRM_MODE(DRM_MODE_TYPE_DRIVER, 13500, 720, 753,
		   815, 858, 480, 483, 486, 525, 0,
		   DRM_MODE_FLAG_PHSYNC | DRM_MODE_FLAG_PVSYNC |
		   DRM_MODE_FLAG_INTERLACE | DRM_MODE_FLAG_DBLCLK),
		   .vrefresh = 60, },
};

struct env_config {
	u32 offset;
	u32 value;
};

static struct env_config ntsc_bt656_config[] = {
	{ BT656_DECODER_CROP, 0x00000000 },
	{ BT656_DECODER_SIZE, 0x01e002d0 },
	{ BT656_DECODER_HTOTAL_HS_END, 0x035a003e },
	{ BT656_DECODER_VACT_ST_HACT_ST, 0x00160069 },
	{ BT656_DECODER_VTOTAL_VS_END, 0x020d0003 },
	{ BT656_DECODER_VS_ST_END_F1, 0x01060109 },
	{ BT656_DECODER_DBG_REG, 0x024002d0 },
	{ BT656_DECODER_CTRL, 0x00000009 },
};

static struct env_config ntsc_tve_config[] = {
	{ TVE_MODE_CTRL, 0x000af906 },
	{ TVE_HOR_TIMING1, 0x00c07a81 },
	{ TVE_HOR_TIMING2, 0x169810fc },
	{ TVE_HOR_TIMING3, 0x96b40000 },
	{ TVE_SUB_CAR_FRQ, 0x21f07bd7 },
	{ TVE_IMAGE_POSITION, 0x001500d6 },
	{ TVE_ROUTING, 0x10088880 },
	{ TVE_SYNC_ADJUST, 0x00000000 },
	{ TVE_STATUS, 0x00000000 },
	{ TVE_CTRL, 0x00000000 },
	{ TVE_INTR_STATUS, 0x00000000 },
	{ TVE_INTR_EN, 0x00000000 },
	{ TVE_INTR_CLR, 0x00000000 },
	{ TVE_COLOR_BUSRT_SAT, 0x0052543c },
	{ TVE_CHROMA_BANDWIDTH, 0x00000002 },
	{ TVE_BRIGHTNESS_CONTRAST, 0x00008300 },
	{ TVE_CLAMP, 0x00000000 },
};

static struct env_config pal_bt656_config[] = {
	{ BT656_DECODER_CROP, 0x00000000 },
	{ BT656_DECODER_SIZE, 0x024002d0 },
	{ BT656_DECODER_HTOTAL_HS_END, 0x0360003f },
	{ BT656_DECODER_VACT_ST_HACT_ST, 0x0016006f },
	{ BT656_DECODER_VTOTAL_VS_END, 0x02710003 },
	{ BT656_DECODER_VS_ST_END_F1, 0x0138013b },
	{ BT656_DECODER_DBG_REG, 0x024002d0 },
	{ BT656_DECODER_CTRL, 0x00000009 },
};

static struct env_config pal_tve_config[] = {
	{ TVE_MODE_CTRL, 0x010ab906 },
	{ TVE_HOR_TIMING1, 0x00c28381 },
	{ TVE_HOR_TIMING2, 0x267d111d },
	{ TVE_HOR_TIMING3, 0x66c00880 },
	{ TVE_SUB_CAR_FRQ, 0x2a098acb },
	{ TVE_IMAGE_POSITION, 0x001500f6 },
	{ TVE_ROUTING, 0x10008882 },
	{ TVE_SYNC_ADJUST, 0x00000000 },
	{ TVE_STATUS, 0x000000b0 },
	{ TVE_CTRL, 0x00000000 },
	{ TVE_INTR_STATUS, 0x00000000 },
	{ TVE_INTR_EN, 0x00000000 },
	{ TVE_INTR_CLR, 0x00000000 },
	{ TVE_COLOR_BUSRT_SAT, 0x00356245 },
	{ TVE_CHROMA_BANDWIDTH, 0x00000022 },
	{ TVE_BRIGHTNESS_CONTRAST, 0x0000aa00 },
	{ TVE_CLAMP, 0x00000000 },
};

#define BT656_ENV_CONFIG_SIZE		(sizeof(ntsc_bt656_config) / sizeof(struct env_config))
#define TVE_ENV_CONFIG_SIZE		(sizeof(ntsc_tve_config) / sizeof(struct env_config))

#define tve_writel(offset, v)		writel(v, tve->reg_base  + offset)
#define tve_readl(offset)		readl(tve->reg_base + offset)

#define tve_dac_writel(offset, v)	writel(v, tve->vdac_base + offset)
#define tve_dac_readl(offset)		readl(tve->vdac_base + offset)

#define tve_grf_writel(offset, v)	writel(v, tve->grf + offset)
#define tve_grf_readl(offset, v)	readl(tve->grf + offset)

struct rockchip_tve_data {
	int	input_format;
	int	soc_type;
};

struct rockchip_tve {
	struct	rockchip_connector connector;
	struct	udevice *dev;
	void	*reg_base;
	void	*vdac_base;
	int	soc_type;
	int	input_format;
	int 	tv_format;
	int	test_mode;
	int	saturation;
	int	brightcontrast;
	int	adjtiming;
	int	lumafilter0;
	int	lumafilter1;
	int	lumafilter2;
	int	lumafilter3;
	int	lumafilter4;
	int	lumafilter5;
	int	lumafilter6;
	int	lumafilter7;
	int	daclevel;
	int	dac1level;
	int	preferred_mode;
	int	upsample_mode;
	u8	vdac_out_current;
	void	*grf;
};

static void tve_write_block(struct rockchip_tve *tve, struct env_config *config, int len)
{
	int i;

	for (i = 0; i < len; i++)
		tve_writel(config[i].offset, config[i].value);
}

static void tve_set_mode(struct rockchip_tve *tve)
{
	struct env_config *bt656_cfg, *tve_cfg;
	int mode = tve->tv_format;

	if (tve->soc_type == SOC_RK3528) {
		tve_writel(TVE_LUMA_FILTER1, tve->lumafilter0);
		tve_writel(TVE_LUMA_FILTER2, tve->lumafilter1);
		tve_writel(TVE_LUMA_FILTER3, tve->lumafilter2);
		tve_writel(TVE_LUMA_FILTER4, tve->lumafilter3);
		tve_writel(TVE_LUMA_FILTER5, tve->lumafilter4);
		tve_writel(TVE_LUMA_FILTER6, tve->lumafilter5);
		tve_writel(TVE_LUMA_FILTER7, tve->lumafilter6);
		tve_writel(TVE_LUMA_FILTER8, tve->lumafilter7);
	} else {
		if (tve->input_format == INPUT_FORMAT_RGB)
			tve_writel(TV_CTRL, v_CVBS_MODE(mode) | v_CLK_UPSTREAM_EN(2) |
				v_TIMING_EN(2) | v_LUMA_FILTER_GAIN(0) |
				v_LUMA_FILTER_UPSAMPLE(1) | v_CSC_PATH(0));
		else
			tve_writel(TV_CTRL, v_CVBS_MODE(mode) | v_CLK_UPSTREAM_EN(2) |
				v_TIMING_EN(2) | v_LUMA_FILTER_GAIN(0) |
				v_LUMA_FILTER_UPSAMPLE(1) | v_CSC_PATH(3));

		tve_writel(TV_LUMA_FILTER0, tve->lumafilter0);
		tve_writel(TV_LUMA_FILTER1, tve->lumafilter1);
		tve_writel(TV_LUMA_FILTER2, tve->lumafilter2);
	}

	if (mode == TVOUT_CVBS_NTSC) {
		TVEDBG("tve set ntsc mode\n");

		if (tve->soc_type == SOC_RK3528) {
			bt656_cfg = ntsc_bt656_config;
			tve_cfg = ntsc_tve_config;

			tve_write_block(tve, bt656_cfg, BT656_ENV_CONFIG_SIZE);
			tve_write_block(tve, tve_cfg, TVE_ENV_CONFIG_SIZE);
		} else {
			tve_writel(TV_ROUTING, v_DAC_SENSE_EN(0) | v_Y_IRE_7_5(1) |
				   v_Y_AGC_PULSE_ON(0) | v_Y_VIDEO_ON(1) |
				   v_YPP_MODE(1) | v_Y_SYNC_ON(1) | v_PIC_MODE(mode));
			tve_writel(TV_BW_CTRL, v_CHROMA_BW(BP_FILTER_NTSC) |
				   v_COLOR_DIFF_BW(COLOR_DIFF_FILTER_BW_1_3));
			tve_writel(TV_SATURATION, 0x0042543C);
			if (tve->test_mode)
				tve_writel(TV_BRIGHTNESS_CONTRAST, 0x00008300);
			else
				tve_writel(TV_BRIGHTNESS_CONTRAST, 0x00007900);

			tve_writel(TV_FREQ_SC,	0x21F07BD7);
			tve_writel(TV_SYNC_TIMING, 0x00C07a81);
			tve_writel(TV_ADJ_TIMING, 0x96B40000 | 0x70);
			tve_writel(TV_ACT_ST,	0x001500D6);
			tve_writel(TV_ACT_TIMING, 0x069800FC | (1 << 12) | (1 << 28));
		}
	} else if (mode == TVOUT_CVBS_PAL) {
		TVEDBG("tve set pal mode\n");

		if (tve->soc_type == SOC_RK3528) {
			bt656_cfg = pal_bt656_config;
			tve_cfg = pal_tve_config;

			tve_write_block(tve, bt656_cfg, BT656_ENV_CONFIG_SIZE);
			tve_write_block(tve, tve_cfg, TVE_ENV_CONFIG_SIZE);
		} else {
			tve_writel(TV_ROUTING, v_DAC_SENSE_EN(0) | v_Y_IRE_7_5(0) |
				   v_Y_AGC_PULSE_ON(0) | v_Y_VIDEO_ON(1) |
				   v_YPP_MODE(1) | v_Y_SYNC_ON(1) | v_PIC_MODE(mode));
			tve_writel(TV_BW_CTRL, v_CHROMA_BW(BP_FILTER_PAL) |
				   v_COLOR_DIFF_BW(COLOR_DIFF_FILTER_BW_1_3));

			tve_writel(TV_SATURATION, tve->saturation);
			tve_writel(TV_BRIGHTNESS_CONTRAST, tve->brightcontrast);

			tve_writel(TV_FREQ_SC,	0x2A098ACB);
			tve_writel(TV_SYNC_TIMING, 0x00C28381);
			tve_writel(TV_ADJ_TIMING, (0xc << 28) | 0x06c00800 | 0x80);
			tve_writel(TV_ACT_ST,	0x001500F6);
			tve_writel(TV_ACT_TIMING, 0x0694011D | (1 << 12) | (2 << 28));

			tve_writel(TV_ADJ_TIMING, tve->adjtiming);
			tve_writel(TV_ACT_TIMING, 0x0694011D | (1 << 12) | (2 << 28));
		}
	}

	if (tve->soc_type == SOC_RK3528) {
		u32 upsample_mode = 0;
		u32 mask = 0;
		u32 val = 0;
		bool upsample_en;

		upsample_en = tve->upsample_mode ? 1 : 0;
		if (upsample_en)
			upsample_mode = tve->upsample_mode - 1;
		mask = m_TVE_DCLK_POL | m_TVE_DCLK_EN | m_DCLK_UPSAMPLE_2X4X |
		       m_DCLK_UPSAMPLE_EN | m_TVE_MODE | m_TVE_EN;
		val = v_TVE_DCLK_POL(0) | v_TVE_DCLK_EN(1) | v_DCLK_UPSAMPLE_2X4X(upsample_mode) |
		      v_DCLK_UPSAMPLE_EN(upsample_en) | v_TVE_MODE(tve->tv_format) | v_TVE_EN(1);

		tve_grf_writel(RK3528_VO_GRF_CVBS_CON, (mask << 16) | val);
	}
}

static void dac_init(struct rockchip_tve *tve)
{
	tve_dac_writel(VDAC_VDAC1, v_CUR_REG(tve->dac1level) |
				   m_DR_PWR_DOWN | m_BG_PWR_DOWN);
	tve_dac_writel(VDAC_VDAC2, v_CUR_CTR(tve->daclevel));
	tve_dac_writel(VDAC_VDAC3, v_CAB_EN(0));
}

static void dac_enable(struct rockchip_tve *tve, bool enable)
{
	u32 mask = 0;
	u32 val = 0;
	u32 grfreg = 0;
	u32 offset = 0;

	if (enable) {
		TVEDBG("tve enable\n");

		if (tve->soc_type == SOC_RK3036) {
			mask = m_VBG_EN | m_DAC_EN | m_DAC_GAIN;
			val = m_VBG_EN | m_DAC_EN | v_DAC_GAIN(tve->daclevel);
			grfreg = RK3036_GRF_SOC_CON3;
		} else if (tve->soc_type == SOC_RK312X) {
			mask = m_VBG_EN | m_DAC_EN | m_DAC_GAIN;
			val = m_VBG_EN | m_DAC_EN | v_DAC_GAIN(tve->daclevel);
			grfreg = RK312X_GRF_TVE_CON;
		} else if (tve->soc_type == SOC_RK322X || tve->soc_type == SOC_RK3328) {
			val = v_CUR_REG(tve->dac1level) | v_DR_PWR_DOWN(0) | v_BG_PWR_DOWN(0);
		} else if (tve->soc_type == SOC_RK3528) {
			/*
			 * Reset the vdac
			 */
			tve_dac_writel(VDAC_CLK_RST, v_ANALOG_RST(0) | v_DIGITAL_RST(0));
			mdelay(20);
			tve_dac_writel(VDAC_CLK_RST, v_ANALOG_RST(1) | v_DIGITAL_RST(1));

			tve_dac_writel(VDAC_CURRENT_CTRL, v_OUT_CURRENT(tve->vdac_out_current));

			val = v_REF_VOLTAGE(7) | v_DAC_PWN(1) | v_BIAS_PWN(1);
			offset = VDAC_PWM_REF_CTRL;
		}
	} else {
		TVEDBG("tve disable\n");

		if (tve->soc_type == SOC_RK312X) {
			mask = m_VBG_EN | m_DAC_EN;
			grfreg = RK312X_GRF_TVE_CON;
		} else if (tve->soc_type == SOC_RK3036) {
			mask = m_VBG_EN | m_DAC_EN;
			grfreg = RK3036_GRF_SOC_CON3;
		} else if (tve->soc_type == SOC_RK322X || tve->soc_type == SOC_RK3328) {
			val = v_CUR_REG(tve->dac1level) | m_DR_PWR_DOWN | m_BG_PWR_DOWN;
			offset = VDAC_VDAC1;
		} else if (tve->soc_type == SOC_RK3528) {
			val = v_DAC_PWN(0) | v_BIAS_PWN(0);
			offset = VDAC_PWM_REF_CTRL;
		}
	}

	if (grfreg)
		tve_grf_writel(grfreg, (mask << 16) | val);
	else if (tve->vdac_base)
		tve_dac_writel(offset, val);
}

static u8 rk_get_vdac_value(void)
{
	u8 value = 0;
#ifdef CONFIG_ROCKCHIP_EFUSE
#if defined(CONFIG_ROCKCHIP_RK322X)
	struct udevice *dev;
	u32 regs[2] = {0};
	u8 fuses[1];
	ofnode node;
	int ret;

	ret = uclass_get_device_by_driver(UCLASS_MISC, DM_GET_DRIVER(rockchip_efuse), &dev);
	if (ret) {
		printf("%s: no misc-device found\n", __func__);
		return -EINVAL;
	}

	node = dev_read_subnode(dev, "tve_dac");
	if (!ofnode_valid(node))
		return -EINVAL;

	ret = ofnode_read_u32_array(node, "reg", regs, 2);
	if (ret) {
		printf("Cannot get efuse reg\n");
		return -EINVAL;
	}

	ret = misc_read(dev, regs[0], &fuses, regs[1]);
	if (ret) {
		printf("%s: misc_read failed\n", __func__);
		return 0;
	}

	value = fuses[0];
	value = (value >> 3) & 0x1f;
#endif
#endif /* CONFIG_RK_EFUSE */
	if (value > 0)
		value += 5;
	TVEDBG("%s value = 0x%x\n", __func__, value);

	return value;
}

#if defined(CONFIG_ROCKCHIP_EFUSE) || defined(CONFIG_ROCKCHIP_OTP)
static int tve_read_otp_by_name(char *name, u8 *val, u8 default_val)
{
	struct udevice *dev;
	ofnode node;
	u32 regs[2] = {0};
	int ret = -EINVAL;

	*val = default_val;
	if (IS_ENABLED(CONFIG_ROCKCHIP_EFUSE))
		ret = uclass_get_device_by_driver(UCLASS_MISC,
						  DM_GET_DRIVER(rockchip_efuse),
						  &dev);
	else
		ret = uclass_get_device_by_driver(UCLASS_MISC,
						  DM_GET_DRIVER(rockchip_otp),
						  &dev);
	if (!ret) {
		node = dev_read_subnode(dev, name);
		if (ofnode_valid(node)) {
			if (!ofnode_read_u32_array(node, "reg", regs, 2)) {
				ret = misc_read(dev, regs[0], val, 1);
				if (!ret)
					return 0;
			}
		}
	}

	printf("tve read %s from otp failed, use default\n", name);

	return ret;
}
#endif

static int tve_parse_dt(struct rockchip_tve *tve)
{
	/*
	 * Read vdac output current from OTP if exists, and the default
	 * current val is 0xd2.
	 */
	u8 out_current = 0xd2;
#if defined(CONFIG_ROCKCHIP_EFUSE) || defined(CONFIG_ROCKCHIP_OTP)
	u8 version = 0;
	int ret = 0;

	ret = tve_read_otp_by_name("vdac-out-current", &out_current, out_current);
	if (!ret) {
		if (out_current) {
			/*
			 * If test version is 0x0, the value of vdac out current
			 * needs to be reduced by one.
			 */
			ret = tve_read_otp_by_name("test-version", &version, version);
			if (!ret) {
				if (version == 0x0)
					out_current -= 1;
			}
		} else {
			/*
			 * If the current value read from OTP is 0, set it to default.
			 */
			out_current = 0xd2;
		}

	}
#endif
	tve->vdac_out_current = out_current;

	tve->preferred_mode = dev_read_u32_default(tve->dev, "rockchip,tvemode", -1);
	if (tve->preferred_mode < 0) {
		tve->preferred_mode = 0;
	} else if (tve->preferred_mode > 1) {
		printf("tve mode value invalid\n");
		return -EINVAL;
	}

	tve->lumafilter0 = dev_read_u32_default(tve->dev, "rockchip,lumafilter0", 0);
	if (tve->lumafilter0 == 0) {
		printf("tve get lumafilter0 err\n");
		return -EINVAL;
	}

	tve->lumafilter1 = dev_read_u32_default(tve->dev, "rockchip,lumafilter1", 0);
	if (tve->lumafilter1 == 0) {
		printf("tve get lumafilter1 err\n");
		return -EINVAL;
	}

	tve->lumafilter2 = dev_read_u32_default(tve->dev, "rockchip,lumafilter2", 0);
	if (tve->lumafilter2 == 0) {
		printf("tve get lumafilter2 err\n");
		return -EINVAL;
	}

	tve->lumafilter3 = dev_read_u32_default(tve->dev, "rockchip,lumafilter3", 0);
	if (tve->lumafilter3 == 0) {
		printf("tve get lumafilter3 err\n");
		return -EINVAL;
	}

	tve->lumafilter4 = dev_read_u32_default(tve->dev, "rockchip,lumafilter4", 0);
	if (tve->lumafilter4 == 0) {
		printf("tve get lumafilter4 err\n");
		return -EINVAL;
	}

	tve->lumafilter5 = dev_read_u32_default(tve->dev, "rockchip,lumafilter5", 0);
	if (tve->lumafilter5 == 0) {
		printf("tve get lumafilter5 err\n");
		return -EINVAL;
	}

	tve->lumafilter6 = dev_read_u32_default(tve->dev, "rockchip,lumafilter6", 0);
	if (tve->lumafilter6 == 0) {
		printf("tve get lumafilter6 err\n");
		return -EINVAL;
	}

	tve->lumafilter7 = dev_read_u32_default(tve->dev, "rockchip,lumafilter7", 0);
	if (tve->lumafilter7 == 0) {
		printf("tve get lumafilter7 err\n");
		return -EINVAL;
	}

	tve->upsample_mode = dev_read_u32_default(tve->dev, "rockchip,tve-upsample", -1);
	if (tve->upsample_mode < 0 || tve->upsample_mode > DCLK_UPSAMPLEx4) {
		printf("tve get upsample_mode err\n");
		return -EINVAL;
	}

	TVEDBG("tve->preferred_mode = 0x%x\n", tve->preferred_mode);
	TVEDBG("tve->lumafilter0 = 0x%x\n", tve->lumafilter0);
	TVEDBG("tve->lumafilter1 = 0x%x\n", tve->lumafilter1);
	TVEDBG("tve->lumafilter2 = 0x%x\n", tve->lumafilter2);
	TVEDBG("tve->lumafilter3 = 0x%x\n", tve->lumafilter3);
	TVEDBG("tve->lumafilter4 = 0x%x\n", tve->lumafilter4);
	TVEDBG("tve->lumafilter5 = 0x%x\n", tve->lumafilter5);
	TVEDBG("tve->lumafilter6 = 0x%x\n", tve->lumafilter6);
	TVEDBG("tve->lumafilter7 = 0x%x\n", tve->lumafilter7);
	TVEDBG("tve->upsample_mode = 0x%x\n", tve->upsample_mode);

	return 0;
}

static int tve_parse_dt_legacy(struct rockchip_tve *tve)
{
	int dac_value, getvdac;

	if (tve->soc_type == SOC_RK312X)
		tve->test_mode = dev_read_u32_default(tve->dev, "test_mode", 0);

	tve->preferred_mode = dev_read_u32_default(tve->dev, "rockchip,tvemode", -1);
	if (tve->preferred_mode < 0) {
		tve->preferred_mode = 0;
	} else if (tve->preferred_mode > 1) {
		printf("tve mode value invalid\n");
		return -EINVAL;
	}

	tve->saturation = dev_read_u32_default(tve->dev, "rockchip,saturation", 0);
	if (tve->saturation == 0) {
		printf("tve get saturation err\n");
		return -EINVAL;
	}

	tve->brightcontrast = dev_read_u32_default(tve->dev, "rockchip,brightcontrast", 0);
	if (tve->brightcontrast == 0) {
		printf("tve get brightcontrast err\n");
		return -EINVAL;
	}

	tve->adjtiming = dev_read_u32_default(tve->dev, "rockchip,adjtiming", 0);
	if (tve->adjtiming == 0) {
		printf("tve get adjtiming err\n");
		return -EINVAL;
	}

	tve->lumafilter0 = dev_read_u32_default(tve->dev, "rockchip,lumafilter0", 0);
	if (tve->lumafilter0 == 0) {
		printf("tve get lumafilter0 err\n");
		return -EINVAL;
	}

	tve->lumafilter1 = dev_read_u32_default(tve->dev, "rockchip,lumafilter1", 0);
	if (tve->lumafilter1 == 0) {
		printf("tve get lumafilter1 err\n");
		return -EINVAL;
	}

	tve->lumafilter2 = dev_read_u32_default(tve->dev, "rockchip,lumafilter2", 0);
	if (tve->lumafilter2 == 0) {
		printf("tve get lumafilter2 err\n");
		return -EINVAL;
	}

	dac_value = dev_read_u32_default(tve->dev, "rockchip,daclevel", 0);
	if (dac_value == 0) {
		printf("tve get dac_value err\n");
		return -EINVAL;
	}

	tve->daclevel = dac_value;
	if (tve->soc_type == SOC_RK322X || tve->soc_type == SOC_RK3328) {
		getvdac = rk_get_vdac_value();
		if (getvdac > 0) {
			tve->daclevel = dac_value + getvdac - RK322X_VDAC_STANDARD;
			if (tve->daclevel > 0x3f || tve->daclevel < 0) {
				printf("rk322x daclevel error!\n");
				tve->daclevel = dac_value;
			}
		} else if (getvdac < 0) {
			printf("get rk322x daclevel error\n");
			return -EINVAL;
		}
	}

	if (tve->soc_type == SOC_RK322X || tve->soc_type == SOC_RK3328) {
		tve->dac1level = dev_read_u32_default(tve->dev, "rockchip,dac1level", 0);
		if (tve->dac1level == 0) {
			printf("rk322x dac1level error!\n");
			return -EINVAL;
		}
	}

	TVEDBG("tve->test_mode = 0x%x\n", tve->test_mode);
	TVEDBG("tve->saturation = 0x%x\n", tve->saturation);
	TVEDBG("tve->brightcontrast = 0x%x\n", tve->brightcontrast);
	TVEDBG("tve->adjtiming = 0x%x\n", tve->adjtiming);
	TVEDBG("tve->lumafilter0 = 0x%x\n", tve->lumafilter0);
	TVEDBG("tve->lumafilter1 = 0x%x\n", tve->lumafilter1);
	TVEDBG("tve->lumafilter2 = 0x%x\n", tve->lumafilter2);
	TVEDBG("tve->daclevel = 0x%x\n", tve->daclevel);

	return 0;
}

static int rockchip_drm_tve_init(struct rockchip_connector *conn, struct display_state *state)
{
	struct rockchip_tve *tve = dev_get_priv(conn->dev);
	struct connector_state *conn_state = &state->conn_state;
	fdt_addr_t addr;
	int ret;

	conn_state->output_mode = ROCKCHIP_OUT_MODE_P888;
	conn_state->bus_format = MEDIA_BUS_FMT_YUV8_1X24;
	if (tve->soc_type == SOC_RK3528)
		conn_state->output_if |= VOP_OUTPUT_IF_BT656;
	conn_state->color_encoding = DRM_COLOR_YCBCR_BT601;
	conn_state->color_range = DRM_COLOR_YCBCR_LIMITED_RANGE;

	conn_state->disp_info = rockchip_get_disp_info(conn_state->type, 0);

	if (tve->soc_type == SOC_RK3528)
		ret = tve_parse_dt(tve);
	else
		ret = tve_parse_dt_legacy(tve);
	if (ret) {
		printf("tve parse dts error\n");
		return -EINVAL;
	}

	addr = dev_read_addr_index(conn->dev, 0);
	if (addr == FDT_ADDR_T_NONE) {
		printf("failed to get tve reg_base\n");
		return -EINVAL;
	}
	tve->reg_base = (void *)addr;

	if (tve->soc_type == SOC_RK322X || tve->soc_type == SOC_RK3328 ||
	    tve->soc_type == SOC_RK3528) {
		addr = dev_read_addr_index(conn->dev, 1);
		if (addr == FDT_ADDR_T_NONE) {
			printf("failed to get tve vdac_base\n");
			return -EINVAL;
		}
		tve->vdac_base = (void *)addr;
	}

	tve->grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);

	if (tve->soc_type == SOC_RK322X || tve->soc_type == SOC_RK3328)
		dac_init(tve);

	return 0;
}

static int rockchip_drm_tve_enable(struct rockchip_connector *conn, struct display_state *state)
{
	struct rockchip_tve *tve = dev_get_priv(conn->dev);
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;

#ifdef CONFIG_ROCKCHIP_INNO_HDMI_PHY
	/* set inno hdmi phy clk. */
	if (tve->soc_type != SOC_RK3528)
		rockchip_phy_set_pll(conn->phy, 27000000);
#endif
	if (mode->vdisplay == 576)
		tve->tv_format = TVOUT_CVBS_PAL;
	else
		tve->tv_format = TVOUT_CVBS_NTSC;

	tve_set_mode(tve);
	mdelay(1000);
	dac_enable(tve, true);

	return 0;
}

static void rockchip_drm_tve_deinit(struct rockchip_connector *conn, struct display_state *state)
{
	struct rockchip_tve *tve = dev_get_priv(conn->dev);

	dac_enable(tve, false);
}

static int rockchip_drm_tve_prepare(struct rockchip_connector *conn, struct display_state *state)
{
	return 0;
}

static int rockchip_drm_tve_disable(struct rockchip_connector *conn, struct display_state *state)
{
	struct rockchip_tve *tve = dev_get_priv(conn->dev);

	dac_enable(tve, false);

	return 0;
}

static int rockchip_drm_tve_detect(struct rockchip_connector *conn, struct display_state *state)
{
	return 1;
}

static void tve_select_output(struct rockchip_tve *tve, struct connector_state *conn_state,
			      struct drm_display_mode *mode)
{
	int ret, i, screen_size;
	struct base_screen_info *screen_info = NULL;
	struct base2_screen_info *screen_info2 = NULL;
	struct base_disp_info base_parameter;
	struct base2_disp_info *base2_parameter = conn_state->disp_info;
	struct drm_display_mode modes[2];
	const struct base_overscan *scan;
	struct overscan *overscan = &conn_state->overscan;
	char baseparameter_buf[8 * RK_BLK_SIZE] __aligned(ARCH_DMA_MINALIGN);
	struct blk_desc *dev_desc;
	disk_partition_t part_info;
	int max_scan = 100;
	int min_scan = 50;
	int offset = 0;
	bool found = false;

	for (i = 0; i < 2; i++) {
		modes[i] = tve_modes[i];
		if (i == tve->preferred_mode)
			modes[i].type |= DRM_MODE_TYPE_PREFERRED;
	}
	*mode = modes[tve->preferred_mode];

	if (!base2_parameter) {
		dev_desc = rockchip_get_bootdev();
		if (!dev_desc) {
			printf("%s: Could not find device\n", __func__);
			goto null_basep;
		}

		ret = part_get_info_by_name(dev_desc, "baseparameter",
					    &part_info);
		if (ret < 0) {
			printf("Could not find baseparameter partition\n");
			goto null_basep;
		}

read_aux:
		ret = blk_dread(dev_desc, part_info.start + offset, 1,
				(void *)baseparameter_buf);
		if (ret < 0) {
			printf("read baseparameter failed\n");
			goto null_basep;
		}

		memcpy(&base_parameter, baseparameter_buf,
		       sizeof(base_parameter));
		scan = &base_parameter.scan;

		screen_size = sizeof(base_parameter.screen_list) /
			sizeof(base_parameter.screen_list[0]);

		for (i = 0; i < screen_size; i++) {
			if (base_parameter.screen_list[i].type ==
			    DRM_MODE_CONNECTOR_TV) {
				found = true;
				screen_info = &base_parameter.screen_list[i];
				break;
			}
		}

		if (!found && !offset) {
			printf("cvbs info isn't saved in main block\n");
			offset += 16;
			goto read_aux;
		}
	} else {
		scan = &base2_parameter->overscan_info;
		screen_size = sizeof(base2_parameter->screen_info) /
			sizeof(base2_parameter->screen_info[0]);

		for (i = 0; i < screen_size; i++) {
			if (base2_parameter->screen_info[i].type ==
			    DRM_MODE_CONNECTOR_TV) {
				screen_info2 =
					&base2_parameter->screen_info[i];
				break;
			}
		}
		screen_info = malloc(sizeof(*screen_info));

		screen_info->type = screen_info2->type;
		screen_info->mode = screen_info2->resolution;
		screen_info->format = screen_info2->format;
		screen_info->depth = screen_info2->depthc;
		screen_info->feature = screen_info2->feature;
	}

	if (scan->leftscale < min_scan && scan->leftscale > 0)
		overscan->left_margin = min_scan;
	else if (scan->leftscale < max_scan && scan->leftscale > 0)
		overscan->left_margin = scan->leftscale;

	if (scan->rightscale < min_scan && scan->rightscale > 0)
		overscan->right_margin = min_scan;
	else if (scan->rightscale < max_scan && scan->rightscale > 0)
		overscan->right_margin = scan->rightscale;

	if (scan->topscale < min_scan && scan->topscale > 0)
		overscan->top_margin = min_scan;
	else if (scan->topscale < max_scan && scan->topscale > 0)
		overscan->top_margin = scan->topscale;

	if (scan->bottomscale < min_scan && scan->bottomscale > 0)
		overscan->bottom_margin = min_scan;
	else if (scan->bottomscale < max_scan && scan->bottomscale > 0)
		overscan->bottom_margin = scan->bottomscale;

null_basep:

	if (screen_info)
		printf("cvbs base_parameter.mode:%dx%d\n",
		       screen_info->mode.hdisplay,
		       screen_info->mode.vdisplay);

	if (screen_info &&
	    (screen_info->mode.hdisplay == 720 &&
	     screen_info->mode.vdisplay == 576))
		*mode = modes[0];
	else if (screen_info &&
		 (screen_info->mode.hdisplay == 720 &&
		  screen_info->mode.vdisplay == 480))
		*mode = modes[1];
}

static int rockchip_drm_tve_get_timing(struct rockchip_connector *conn, struct display_state *state)
{
	struct rockchip_tve *tve = dev_get_priv(conn->dev);
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;

	tve_select_output(tve, conn_state, mode);

	return 0;
}

const struct rockchip_connector_funcs rockchip_drm_tve_funcs = {
	.init = rockchip_drm_tve_init,
	.deinit = rockchip_drm_tve_deinit,
	.prepare = rockchip_drm_tve_prepare,
	.enable = rockchip_drm_tve_enable,
	.disable = rockchip_drm_tve_disable,
	.get_timing = rockchip_drm_tve_get_timing,
	.detect = rockchip_drm_tve_detect,
};

static int rockchip_drm_tve_probe(struct udevice *dev)
{
	struct rockchip_tve *tve = dev_get_priv(dev);
	const struct rockchip_tve_data *data;

	tve->dev = dev;
	data = (const struct rockchip_tve_data *)dev_get_driver_data(dev);
	tve->soc_type = data->soc_type;
	tve->input_format = data->input_format;

	rockchip_connector_bind(&tve->connector, dev, 0, &rockchip_drm_tve_funcs, NULL, DRM_MODE_CONNECTOR_TV);

	return 0;
}

static const struct rockchip_tve_data rk3036_tve = {
	.soc_type = SOC_RK3036,
	.input_format = INPUT_FORMAT_RGB,
};

static const struct rockchip_tve_data rk312x_tve = {
	.soc_type = SOC_RK312X,
	.input_format = INPUT_FORMAT_RGB,
};

static const struct rockchip_tve_data rk322x_tve = {
	.soc_type = SOC_RK322X,
	.input_format = INPUT_FORMAT_YUV,
};

static const struct rockchip_tve_data rk3328_tve = {
	.soc_type = SOC_RK3328,
	.input_format = INPUT_FORMAT_YUV,
};

static const struct rockchip_tve_data rk3528_tve = {
	.soc_type = SOC_RK3528,
	.input_format = INPUT_FORMAT_YUV,
};

static const struct udevice_id rockchip_drm_tve_ids[] = {
	{ .compatible = "rockchip,rk3036-tve", .data = (ulong)&rk3036_tve },
	{ .compatible = "rockchip,rk312x-tve", .data = (ulong)&rk312x_tve },
	{ .compatible = "rockchip,rk322x-tve", .data = (ulong)&rk322x_tve },
	{ .compatible = "rockchip,rk3328-tve", .data = (ulong)&rk3328_tve },
	{ .compatible = "rockchip,rk3528-tve", .data = (ulong)&rk3528_tve },
};

U_BOOT_DRIVER(rockchip_drm_tve) = {
	.name = "rockchip_drm_tve",
	.id = UCLASS_DISPLAY,
	.of_match = rockchip_drm_tve_ids,
	.probe	= rockchip_drm_tve_probe,
	.priv_auto_alloc_size = sizeof(struct rockchip_tve),
};
