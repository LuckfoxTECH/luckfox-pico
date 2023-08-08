/*
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <asm/unaligned.h>
#include <boot_rkimg.h>
#include <config.h>
#include <common.h>
#include <errno.h>
#include <linux/libfdt.h>
#include <fdtdec.h>
#include <fdt_support.h>
#include <linux/hdmi.h>
#include <linux/list.h>
#include <linux/compat.h>
#include <linux/media-bus-format.h>
#include <malloc.h>
#include <video.h>
#include <video_rockchip.h>
#include <video_bridge.h>
#include <dm/device.h>
#include <dm/uclass-internal.h>
#include <asm/arch-rockchip/resource_img.h>

#include "bmp_helper.h"
#include "rockchip_display.h"
#include "rockchip_crtc.h"
#include "rockchip_connector.h"
#include "rockchip_bridge.h"
#include "rockchip_phy.h"
#include "rockchip_panel.h"
#include <dm.h>
#include <dm/of_access.h>
#include <dm/ofnode.h>
#include <asm/io.h>

#define DRIVER_VERSION	"v1.0.1"

/***********************************************************************
 *  Rockchip UBOOT DRM driver version
 *
 *  v1.0.0	: add basic version for rockchip drm driver(hjc)
 *  v1.0.1	: add much dsi update(hjc)
 *
 **********************************************************************/

#define RK_BLK_SIZE 512
#define BMP_PROCESSED_FLAG 8399

DECLARE_GLOBAL_DATA_PTR;
static LIST_HEAD(rockchip_display_list);
static LIST_HEAD(logo_cache_list);

static unsigned long memory_start;
static unsigned long cubic_lut_memory_start;
static unsigned long memory_end;
static struct base2_info base_parameter;
static uint32_t crc32_table[256];

/*
 * the phy types are used by different connectors in public.
 * The current version only has inno hdmi phy for hdmi and tve.
 */
enum public_use_phy {
	NONE,
	INNO_HDMI_PHY
};

/* save public phy data */
struct public_phy_data {
	const struct rockchip_phy *phy_drv;
	int phy_node;
	int public_phy_type;
	bool phy_init;
};

void rockchip_display_make_crc32_table(void)
{
	uint32_t c;
	int n, k;
	unsigned long poly;		/* polynomial exclusive-or pattern */
	/* terms of polynomial defining this crc (except x^32): */
	static const char p[] = {0, 1, 2, 4, 5, 7, 8, 10, 11, 12, 16, 22, 23, 26};

	/* make exclusive-or pattern from polynomial (0xedb88320L) */
	poly = 0L;
	for (n = 0; n < sizeof(p) / sizeof(char); n++)
		poly |= 1L << (31 - p[n]);

	for (n = 0; n < 256; n++) {
		c = (unsigned long)n;
		for (k = 0; k < 8; k++)
		c = c & 1 ? poly ^ (c >> 1) : c >> 1;
		crc32_table[n] = cpu_to_le32(c);
	}
}

uint32_t rockchip_display_crc32c_cal(unsigned char *data, int length)
{
	int i;
	uint32_t crc;
	crc = 0xFFFFFFFF;

	for (i = 0; i < length; i++) {
		crc = crc32_table[(crc ^ *data) & 0xff] ^ (crc >> 8);
		data++;
	}

	return crc ^ 0xffffffff;
}

int rockchip_get_baseparameter(void)
{
	struct blk_desc *dev_desc;
	disk_partition_t part_info;
	int block_num = 2048;
	char baseparameter_buf[block_num * RK_BLK_SIZE] __aligned(ARCH_DMA_MINALIGN);
	int ret = 0;

	dev_desc = rockchip_get_bootdev();
	if (!dev_desc) {
		printf("%s: Could not find device\n", __func__);
		return -ENOENT;
	}

	if (part_get_info_by_name(dev_desc, "baseparameter", &part_info) < 0) {
		printf("Could not find baseparameter partition\n");
		return -ENOENT;
	}

	ret = blk_dread(dev_desc, part_info.start, block_num, (void *)baseparameter_buf);
	if (ret < 0) {
		printf("read baseparameter failed\n");
		return ret;
	}

	memcpy(&base_parameter, baseparameter_buf, sizeof(base_parameter));
	if (strncasecmp(base_parameter.head_flag, "BASP", 4)) {
		printf("warning: bad baseparameter\n");
		memset(&base_parameter, 0, sizeof(base_parameter));
	}
	rockchip_display_make_crc32_table();

	return ret;
}

struct base2_disp_info *rockchip_get_disp_info(int type, int id)
{
	struct base2_disp_info *disp_info;
	struct base2_disp_header *disp_header;
	int i = 0, offset = -1;
	u32 crc_val;
	u32 base2_length;
	void *base_parameter_addr = (void *)&base_parameter;

	for (i = 0; i < 8; i++) {
		disp_header = &base_parameter.disp_header[i];
		if (disp_header->connector_type == type &&
		    disp_header->connector_id == id) {
			printf("disp info %d, type:%d, id:%d\n", i, type, id);
			offset = disp_header->offset;
			break;
		}
	}

	if (offset < 0)
		return NULL;
	disp_info = base_parameter_addr + offset;
	if (disp_info->screen_info[0].type != type ||
	    disp_info->screen_info[0].id != id) {
		printf("base2_disp_info couldn't be found, screen_info type[%d] or id[%d] mismatched\n",
		       disp_info->screen_info[0].type,
		       disp_info->screen_info[0].id);
		return NULL;
	}

	if (strncasecmp(disp_info->disp_head_flag, "DISP", 4))
		return NULL;

	if (base_parameter.major_version == 3 && base_parameter.minor_version == 0) {
		crc_val = rockchip_display_crc32c_cal((unsigned char *)disp_info,
						      sizeof(struct base2_disp_info) - 4);
		if (crc_val != disp_info->crc2) {
			printf("error: connector type[%d], id[%d] disp info crc2 check error\n",
			       type, id);
			return NULL;
		}
	} else {
		base2_length = sizeof(struct base2_disp_info) - sizeof(struct csc_info) -
			       sizeof(struct acm_data) - 10 * 1024 - 4;
		crc_val = rockchip_display_crc32c_cal((unsigned char *)disp_info, base2_length - 4);
		if (crc_val != disp_info->crc) {
			printf("error: connector type[%d], id[%d] disp info crc check error\n",
			       type, id);
			return NULL;
		}
	}

	return disp_info;
}

/* check which kind of public phy does connector use */
static int check_public_use_phy(struct rockchip_connector *conn)
{
	int ret = NONE;
#ifdef CONFIG_ROCKCHIP_INNO_HDMI_PHY

	if (!strncmp(dev_read_name(conn->dev), "tve", 3) ||
	    !strncmp(dev_read_name(conn->dev), "hdmi", 4))
		ret = INNO_HDMI_PHY;
#endif

	return ret;
}

/*
 * get public phy driver and initialize it.
 * The current version only has inno hdmi phy for hdmi and tve.
 */
static int get_public_phy(struct rockchip_connector *conn,
			  struct public_phy_data *data)
{
	struct rockchip_phy *phy;
	struct udevice *dev;
	int ret = 0;

	switch (data->public_phy_type) {
	case INNO_HDMI_PHY:
#if defined(CONFIG_ROCKCHIP_RK3328)
		ret = uclass_get_device_by_name(UCLASS_PHY,
						"hdmiphy@ff430000", &dev);
#elif defined(CONFIG_ROCKCHIP_RK322X)
		ret = uclass_get_device_by_name(UCLASS_PHY,
						"hdmi-phy@12030000", &dev);
#else
		ret = -EINVAL;
#endif
		if (ret) {
			printf("Warn: can't find phy driver\n");
			return 0;
		}

		phy = (struct rockchip_phy *)dev_get_driver_data(dev);
		if (!phy) {
			printf("failed to get phy driver\n");
			return 0;
		}

		ret = rockchip_phy_init(phy);
		if (ret) {
			printf("failed to init phy driver\n");
			return ret;
		}
		conn->phy = phy;

		debug("inno hdmi phy init success, save it\n");
		data->phy_drv = conn->phy;
		data->phy_init = true;
		return 0;
	default:
		return -EINVAL;
	}
}

static void init_display_buffer(ulong base)
{
	memory_start = base + DRM_ROCKCHIP_FB_SIZE;
	memory_end = memory_start;
	cubic_lut_memory_start = memory_start + MEMORY_POOL_SIZE;
}

void *get_display_buffer(int size)
{
	unsigned long roundup_memory = roundup(memory_end, PAGE_SIZE);
	void *buf;

	if (roundup_memory + size > memory_start + MEMORY_POOL_SIZE) {
		printf("failed to alloc %dbyte memory to display\n", size);
		return NULL;
	}
	buf = (void *)roundup_memory;

	memory_end = roundup_memory + size;

	return buf;
}

static unsigned long get_display_size(void)
{
	return memory_end - memory_start;
}

static unsigned long get_single_cubic_lut_size(void)
{
	ulong cubic_lut_size;
	int cubic_lut_step = CONFIG_ROCKCHIP_CUBIC_LUT_SIZE;

	/* This is depend on IC designed */
	cubic_lut_size = (cubic_lut_step * cubic_lut_step * cubic_lut_step + 1) / 2 * 16;
	cubic_lut_size = roundup(cubic_lut_size, PAGE_SIZE);

	return cubic_lut_size;
}

static unsigned long get_cubic_lut_offset(int crtc_id)
{
	return crtc_id * get_single_cubic_lut_size();
}

unsigned long get_cubic_lut_buffer(int crtc_id)
{
	return cubic_lut_memory_start + crtc_id * get_single_cubic_lut_size();
}

static unsigned long get_cubic_memory_size(void)
{
	/* Max support 4 cubic lut */
	return get_single_cubic_lut_size() * 4;
}

bool can_direct_logo(int bpp)
{
	return bpp == 16 || bpp == 32;
}

static int connector_phy_init(struct rockchip_connector *conn,
			      struct public_phy_data *data)
{
	int type;

	/* does this connector use public phy with others */
	type = check_public_use_phy(conn);
	if (type == INNO_HDMI_PHY) {
		/* there is no public phy was initialized */
		if (!data->phy_init) {
			debug("start get public phy\n");
			data->public_phy_type = type;
			if (get_public_phy(conn, data)) {
				printf("can't find correct public phy type\n");
				free(data);
				return -EINVAL;
			}
			return 0;
		}

		/* if this phy has been initialized, get it directly */
		conn->phy = (struct rockchip_phy *)data->phy_drv;
		return 0;
	}

	return 0;
}

int drm_mode_vrefresh(const struct drm_display_mode *mode)
{
	int refresh = 0;
	unsigned int calc_val;

	if (mode->vrefresh > 0) {
		refresh = mode->vrefresh;
	} else if (mode->htotal > 0 && mode->vtotal > 0) {
		int vtotal;

		vtotal = mode->vtotal;
		/* work out vrefresh the value will be x1000 */
		calc_val = (mode->clock * 1000);
		calc_val /= mode->htotal;
		refresh = (calc_val + vtotal / 2) / vtotal;

		if (mode->flags & DRM_MODE_FLAG_INTERLACE)
			refresh *= 2;
		if (mode->flags & DRM_MODE_FLAG_DBLSCAN)
			refresh /= 2;
		if (mode->vscan > 1)
			refresh /= mode->vscan;
	}
	return refresh;
}

int rockchip_ofnode_get_display_mode(ofnode node, struct drm_display_mode *mode)
{
	int hactive, vactive, pixelclock;
	int hfront_porch, hback_porch, hsync_len;
	int vfront_porch, vback_porch, vsync_len;
	int val, flags = 0;

#define FDT_GET_INT(val, name) \
	val = ofnode_read_s32_default(node, name, -1); \
	if (val < 0) { \
		printf("Can't get %s\n", name); \
		return -ENXIO; \
	}

#define FDT_GET_INT_DEFAULT(val, name, default) \
	val = ofnode_read_s32_default(node, name, default);

	FDT_GET_INT(hactive, "hactive");
	FDT_GET_INT(vactive, "vactive");
	FDT_GET_INT(pixelclock, "clock-frequency");
	FDT_GET_INT(hsync_len, "hsync-len");
	FDT_GET_INT(hfront_porch, "hfront-porch");
	FDT_GET_INT(hback_porch, "hback-porch");
	FDT_GET_INT(vsync_len, "vsync-len");
	FDT_GET_INT(vfront_porch, "vfront-porch");
	FDT_GET_INT(vback_porch, "vback-porch");
	FDT_GET_INT(val, "hsync-active");
	flags |= val ? DRM_MODE_FLAG_PHSYNC : DRM_MODE_FLAG_NHSYNC;
	FDT_GET_INT(val, "vsync-active");
	flags |= val ? DRM_MODE_FLAG_PVSYNC : DRM_MODE_FLAG_NVSYNC;
	FDT_GET_INT(val, "pixelclk-active");
	flags |= val ? DRM_MODE_FLAG_PPIXDATA : 0;

	FDT_GET_INT_DEFAULT(val, "screen-rotate", 0);
	if (val == DRM_MODE_FLAG_XMIRROR) {
		flags |= DRM_MODE_FLAG_XMIRROR;
	} else if (val == DRM_MODE_FLAG_YMIRROR) {
		flags |= DRM_MODE_FLAG_YMIRROR;
	} else if (val == DRM_MODE_FLAG_XYMIRROR) {
		flags |= DRM_MODE_FLAG_XMIRROR;
		flags |= DRM_MODE_FLAG_YMIRROR;
	}
	mode->hdisplay = hactive;
	mode->hsync_start = mode->hdisplay + hfront_porch;
	mode->hsync_end = mode->hsync_start + hsync_len;
	mode->htotal = mode->hsync_end + hback_porch;

	mode->vdisplay = vactive;
	mode->vsync_start = mode->vdisplay + vfront_porch;
	mode->vsync_end = mode->vsync_start + vsync_len;
	mode->vtotal = mode->vsync_end + vback_porch;

	mode->clock = pixelclock / 1000;
	mode->flags = flags;
	mode->vrefresh = drm_mode_vrefresh(mode);

	return 0;
}

static int display_get_force_timing_from_dts(ofnode node, struct drm_display_mode *mode)
{
	int ret = 0;

	ret = rockchip_ofnode_get_display_mode(node, mode);

	if (ret) {
		mode->clock = 74250;
		mode->flags = 0x5;
		mode->hdisplay = 1280;
		mode->hsync_start = 1390;
		mode->hsync_end = 1430;
		mode->htotal = 1650;
		mode->hskew = 0;
		mode->vdisplay = 720;
		mode->vsync_start = 725;
		mode->vsync_end = 730;
		mode->vtotal = 750;
		mode->vrefresh = 60;
		mode->picture_aspect_ratio = HDMI_PICTURE_ASPECT_16_9;
		mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	}

	printf("route node %s force_timing, use %dx%dp%d as default mode\n",
	       ret ? "undefine" : "define", mode->hdisplay, mode->vdisplay,
	       mode->vscan);

	return 0;
}

static int display_get_timing_from_dts(struct rockchip_panel *panel,
				       struct drm_display_mode *mode)
{
	struct ofnode_phandle_args args;
	ofnode dt, timing;
	int ret;

	dt = dev_read_subnode(panel->dev, "display-timings");
	if (ofnode_valid(dt)) {
		ret = ofnode_parse_phandle_with_args(dt, "native-mode", NULL,
						     0, 0, &args);
		if (ret)
			return ret;

		timing = args.node;
	} else {
		timing = dev_read_subnode(panel->dev, "panel-timing");
	}

	if (!ofnode_valid(timing)) {
		printf("failed to get display timings from DT\n");
		return -ENXIO;
	}

	rockchip_ofnode_get_display_mode(timing, mode);

	return 0;
}

/**
 * drm_mode_max_resolution_filter - mark modes out of vop max resolution
 * @edid_data: structure store mode list
 * @max_output: vop max output resolution
 */
void drm_mode_max_resolution_filter(struct hdmi_edid_data *edid_data,
				    struct vop_rect *max_output)
{
	int i;

	for (i = 0; i < edid_data->modes; i++) {
		if (edid_data->mode_buf[i].hdisplay > max_output->width ||
		    edid_data->mode_buf[i].vdisplay > max_output->height)
			edid_data->mode_buf[i].invalid = true;
	}
}

/**
 * drm_mode_set_crtcinfo - set CRTC modesetting timing parameters
 * @p: mode
 * @adjust_flags: a combination of adjustment flags
 *
 * Setup the CRTC modesetting timing parameters for @p, adjusting if necessary.
 *
 * - The CRTC_INTERLACE_HALVE_V flag can be used to halve vertical timings of
 *   interlaced modes.
 * - The CRTC_STEREO_DOUBLE flag can be used to compute the timings for
 *   buffers containing two eyes (only adjust the timings when needed, eg. for
 *   "frame packing" or "side by side full").
 * - The CRTC_NO_DBLSCAN and CRTC_NO_VSCAN flags request that adjustment *not*
 *   be performed for doublescan and vscan > 1 modes respectively.
 */
void drm_mode_set_crtcinfo(struct drm_display_mode *p, int adjust_flags)
{
	if ((p == NULL) || ((p->type & DRM_MODE_TYPE_CRTC_C) == DRM_MODE_TYPE_BUILTIN))
		return;

	if (p->flags & DRM_MODE_FLAG_DBLCLK)
		p->crtc_clock = 2 * p->clock;
	else
		p->crtc_clock = p->clock;
	p->crtc_hdisplay = p->hdisplay;
	p->crtc_hsync_start = p->hsync_start;
	p->crtc_hsync_end = p->hsync_end;
	p->crtc_htotal = p->htotal;
	p->crtc_hskew = p->hskew;
	p->crtc_vdisplay = p->vdisplay;
	p->crtc_vsync_start = p->vsync_start;
	p->crtc_vsync_end = p->vsync_end;
	p->crtc_vtotal = p->vtotal;

	if (p->flags & DRM_MODE_FLAG_INTERLACE) {
		if (adjust_flags & CRTC_INTERLACE_HALVE_V) {
			p->crtc_vdisplay /= 2;
			p->crtc_vsync_start /= 2;
			p->crtc_vsync_end /= 2;
			p->crtc_vtotal /= 2;
		}
	}

	if (!(adjust_flags & CRTC_NO_DBLSCAN)) {
		if (p->flags & DRM_MODE_FLAG_DBLSCAN) {
			p->crtc_vdisplay *= 2;
			p->crtc_vsync_start *= 2;
			p->crtc_vsync_end *= 2;
			p->crtc_vtotal *= 2;
		}
	}

	if (!(adjust_flags & CRTC_NO_VSCAN)) {
		if (p->vscan > 1) {
			p->crtc_vdisplay *= p->vscan;
			p->crtc_vsync_start *= p->vscan;
			p->crtc_vsync_end *= p->vscan;
			p->crtc_vtotal *= p->vscan;
		}
	}

	if (adjust_flags & CRTC_STEREO_DOUBLE) {
		unsigned int layout = p->flags & DRM_MODE_FLAG_3D_MASK;

		switch (layout) {
		case DRM_MODE_FLAG_3D_FRAME_PACKING:
			p->crtc_clock *= 2;
			p->crtc_vdisplay += p->crtc_vtotal;
			p->crtc_vsync_start += p->crtc_vtotal;
			p->crtc_vsync_end += p->crtc_vtotal;
			p->crtc_vtotal += p->crtc_vtotal;
			break;
		}
	}

	p->crtc_vblank_start = min(p->crtc_vsync_start, p->crtc_vdisplay);
	p->crtc_vblank_end = max(p->crtc_vsync_end, p->crtc_vtotal);
	p->crtc_hblank_start = min(p->crtc_hsync_start, p->crtc_hdisplay);
	p->crtc_hblank_end = max(p->crtc_hsync_end, p->crtc_htotal);
}

/**
 * drm_mode_is_420_only - if a given videomode can be only supported in YCBCR420
 * output format
 *
 * @connector: drm connector under action.
 * @mode: video mode to be tested.
 *
 * Returns:
 * true if the mode can be supported in YCBCR420 format
 * false if not.
 */
bool drm_mode_is_420_only(const struct drm_display_info *display,
			  struct drm_display_mode *mode)
{
	u8 vic = drm_match_cea_mode(mode);

	return test_bit(vic, display->hdmi.y420_vdb_modes);
}

/**
 * drm_mode_is_420_also - if a given videomode can be supported in YCBCR420
 * output format also (along with RGB/YCBCR444/422)
 *
 * @display: display under action.
 * @mode: video mode to be tested.
 *
 * Returns:
 * true if the mode can be support YCBCR420 format
 * false if not.
 */
bool drm_mode_is_420_also(const struct drm_display_info *display,
			  struct drm_display_mode *mode)
{
	u8 vic = drm_match_cea_mode(mode);

	return test_bit(vic, display->hdmi.y420_cmdb_modes);
}

/**
 * drm_mode_is_420 - if a given videomode can be supported in YCBCR420
 * output format
 *
 * @display: display under action.
 * @mode: video mode to be tested.
 *
 * Returns:
 * true if the mode can be supported in YCBCR420 format
 * false if not.
 */
bool drm_mode_is_420(const struct drm_display_info *display,
		     struct drm_display_mode *mode)
{
	return drm_mode_is_420_only(display, mode) ||
		drm_mode_is_420_also(display, mode);
}

static int display_get_timing(struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	const struct drm_display_mode *m;
	struct rockchip_panel *panel = conn_state->connector->panel;

	if (panel->funcs->get_mode)
		return panel->funcs->get_mode(panel, mode);

	if (dev_of_valid(panel->dev) &&
	    !display_get_timing_from_dts(panel, mode)) {
		printf("Using display timing dts\n");
		return 0;
	}

	if (panel->data) {
		m = (const struct drm_display_mode *)panel->data;
		memcpy(mode, m, sizeof(*m));
		printf("Using display timing from compatible panel driver\n");
		return 0;
	}

	return -ENODEV;
}

static int display_pre_init(void)
{
	struct display_state *state;
	int ret = 0;

	list_for_each_entry(state, &rockchip_display_list, head) {
		struct connector_state *conn_state = &state->conn_state;
		struct crtc_state *crtc_state = &state->crtc_state;
		struct rockchip_crtc *crtc = crtc_state->crtc;

		ret = rockchip_connector_pre_init(state);
		if (ret)
			printf("pre init conn error\n");

		crtc->vps[crtc_state->crtc_id].output_type = conn_state->type;
	}
	return ret;
}

static int display_use_force_mode(struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;

	conn_state->bpc = 8;
	memcpy(mode, &state->force_mode, sizeof(struct drm_display_mode));
	conn_state->bus_format = state->force_bus_format;

	return 0;
}

static int display_get_edid_mode(struct display_state *state)
{
	int ret = 0;
	struct connector_state *conn_state = &state->conn_state;
	struct drm_display_mode *mode = &conn_state->mode;
	int bpc;

	ret = edid_get_drm_mode(conn_state->edid, sizeof(conn_state->edid), mode, &bpc);
	if (!ret) {
		conn_state->bpc = bpc;
		edid_print_info((void *)&conn_state->edid);
	} else {
		conn_state->bpc = 8;
		mode->clock = 74250;
		mode->flags = 0x5;
		mode->hdisplay = 1280;
		mode->hsync_start = 1390;
		mode->hsync_end = 1430;
		mode->htotal = 1650;
		mode->hskew = 0;
		mode->vdisplay = 720;
		mode->vsync_start = 725;
		mode->vsync_end = 730;
		mode->vtotal = 750;
		mode->vrefresh = 60;
		mode->picture_aspect_ratio = HDMI_PICTURE_ASPECT_16_9;
		mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;

		printf("error: %s get mode from edid failed, use 720p60 as default mode\n",
		       state->conn_state.connector->dev->name);
	}

	return ret;
}

static int display_init(struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct rockchip_connector *conn = conn_state->connector;
	struct crtc_state *crtc_state = &state->crtc_state;
	struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;
	struct drm_display_mode *mode = &conn_state->mode;
	const char *compatible;
	int ret = 0;
	static bool __print_once = false;

	if (!__print_once) {
		__print_once = true;
		printf("Rockchip UBOOT DRM driver version: %s\n", DRIVER_VERSION);
	}

	if (state->is_init)
		return 0;

	if (!crtc_funcs) {
		printf("failed to find crtc functions\n");
		return -ENXIO;
	}

	if (crtc_state->crtc->active && !crtc_state->ports_node &&
	    memcmp(&crtc_state->crtc->active_mode, &conn_state->mode,
		   sizeof(struct drm_display_mode))) {
		printf("%s has been used for output type: %d, mode: %dx%dp%d\n",
			crtc_state->dev->name,
			crtc_state->crtc->active_mode.type,
			crtc_state->crtc->active_mode.hdisplay,
			crtc_state->crtc->active_mode.vdisplay,
			crtc_state->crtc->active_mode.vrefresh);
		return -ENODEV;
	}

	if (crtc_funcs->preinit) {
		ret = crtc_funcs->preinit(state);
		if (ret)
			return ret;
	}

	ret = rockchip_connector_init(state);
	if (ret)
		goto deinit;

	/*
	 * support hotplug, but not connect;
	 */
#ifdef CONFIG_DRM_ROCKCHIP_TVE
	if (crtc->hdmi_hpd && conn_state->type == DRM_MODE_CONNECTOR_TV) {
		printf("hdmi plugin ,skip tve\n");
		goto deinit;
	}
#elif defined(CONFIG_DRM_ROCKCHIP_RK1000)
	if (crtc->hdmi_hpd && conn_state->type == DRM_MODE_CONNECTOR_LVDS) {
		printf("hdmi plugin ,skip tve\n");
		goto deinit;
	}
#endif

	ret = rockchip_connector_detect(state);
#if defined(CONFIG_DRM_ROCKCHIP_TVE) || defined(CONFIG_DRM_ROCKCHIP_RK1000)
	if (conn_state->type == DRM_MODE_CONNECTOR_HDMIA)
		crtc->hdmi_hpd = ret;
#endif
	if (!ret && !state->force_output)
		goto deinit;

	if (conn->panel) {
		ret = display_get_timing(state);
		if (!ret)
			conn_state->bpc = conn->panel->bpc;
#if defined(CONFIG_I2C_EDID)
		if (ret < 0 && conn->funcs->get_edid) {
			rockchip_panel_prepare(conn->panel);
			ret = conn->funcs->get_edid(conn, state);
			if (!ret)
				display_get_edid_mode(state);
		}
#endif
	} else if (conn->bridge) {
		ret = video_bridge_read_edid(conn->bridge->dev,
					     conn_state->edid, EDID_SIZE);
		if (ret > 0) {
#if defined(CONFIG_I2C_EDID)
			display_get_edid_mode(state);
#endif
		} else {
			ret = video_bridge_get_timing(conn->bridge->dev);
		}
	} else if (conn->funcs->get_timing) {
		ret = conn->funcs->get_timing(conn, state);
	} else if (conn->funcs->get_edid) {
		ret = conn->funcs->get_edid(conn, state);
#if defined(CONFIG_I2C_EDID)
		if (!ret)
			display_get_edid_mode(state);
#endif
	}

	if (!ret && conn_state->secondary) {
		struct rockchip_connector *connector = conn_state->secondary;

		if (connector->panel) {
			if (connector->panel->funcs->get_mode) {
				struct drm_display_mode *_mode = drm_mode_create();

				ret = connector->panel->funcs->get_mode(connector->panel, _mode);
				if (!ret && !drm_mode_equal(_mode, mode))
					ret = -EINVAL;

				drm_mode_destroy(_mode);
			}
		}
	}

	if (ret && !state->force_output)
		goto deinit;
	if (state->force_output)
		display_use_force_mode(state);

	/* rk356x series drive mipi pixdata on posedge */
	compatible = dev_read_string(conn->dev, "compatible");
	if (!strcmp(compatible, "rockchip,rk3568-mipi-dsi"))
		conn_state->mode.flags |= DRM_MODE_FLAG_PPIXDATA;

	printf("%s: %s detailed mode clock %u kHz, flags[%x]\n"
	       "    H: %04d %04d %04d %04d\n"
	       "    V: %04d %04d %04d %04d\n"
	       "bus_format: %x\n",
	       conn->dev->name,
	       state->force_output ? "use force output" : "",
	       mode->clock, mode->flags,
	       mode->hdisplay, mode->hsync_start,
	       mode->hsync_end, mode->htotal,
	       mode->vdisplay, mode->vsync_start,
	       mode->vsync_end, mode->vtotal,
	       conn_state->bus_format);

	drm_mode_set_crtcinfo(mode, CRTC_INTERLACE_HALVE_V);

	if (conn_state->secondary) {
		mode->crtc_clock *= 2;
		mode->crtc_hdisplay *= 2;
		mode->crtc_hsync_start *= 2;
		mode->crtc_hsync_end *= 2;
		mode->crtc_htotal *= 2;
	}

	if (conn->bridge)
		rockchip_bridge_mode_set(conn->bridge, &conn_state->mode);

	if (crtc_funcs->init) {
		ret = crtc_funcs->init(state);
		if (ret)
			goto deinit;
	}
	state->is_init = 1;

	crtc_state->crtc->active = true;
	memcpy(&crtc_state->crtc->active_mode,
	       &conn_state->mode, sizeof(struct drm_display_mode));

	return 0;

deinit:
	rockchip_connector_deinit(state);
	return ret;
}

int display_send_mcu_cmd(struct display_state *state, u32 type, u32 val)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;
	int ret;

	if (!state->is_init)
		return -EINVAL;

	if (crtc_funcs->send_mcu_cmd) {
		ret = crtc_funcs->send_mcu_cmd(state, type, val);
		if (ret)
			return ret;
	}

	return 0;
}

static int display_set_plane(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;
	int ret;

	if (!state->is_init)
		return -EINVAL;

	if (crtc_funcs->set_plane) {
		ret = crtc_funcs->set_plane(state);
		if (ret)
			return ret;
	}

	return 0;
}

static int display_enable(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;

	if (!state->is_init)
		return -EINVAL;

	if (state->is_enable)
		return 0;

	if (crtc_funcs->prepare)
		crtc_funcs->prepare(state);

	rockchip_connector_pre_enable(state);

	if (crtc_funcs->enable)
		crtc_funcs->enable(state);

	rockchip_connector_enable(state);

	state->is_enable = true;

	return 0;
}

static int display_disable(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;

	if (!state->is_init)
		return 0;

	if (!state->is_enable)
		return 0;

	rockchip_connector_disable(state);

	if (crtc_funcs->disable)
		crtc_funcs->disable(state);

	rockchip_connector_post_disable(state);

	state->is_enable = 0;
	state->is_init = 0;

	return 0;
}

static int display_rect_calc_scale(int src, int dst)
{
	int scale = 0;

	if (WARN_ON(src < 0 || dst < 0))
		return -EINVAL;

	if (dst == 0)
		return 0;

	src <<= 16;

	if (src > (dst << 16))
		return DIV_ROUND_UP(src, dst);
	else
		scale = src / dst;

	return scale;
}

int display_rect_calc_hscale(struct display_rect *src, struct display_rect *dst,
			     int min_hscale, int max_hscale)
{
	int hscale = display_rect_calc_scale(src->w, dst->w);

	if (hscale < 0 || dst->w == 0)
		return hscale;

	if (hscale < min_hscale || hscale > max_hscale)
		return -ERANGE;

	return hscale;
}

int display_rect_calc_vscale(struct display_rect *src, struct display_rect *dst,
			     int min_vscale, int max_vscale)
{
	int vscale = display_rect_calc_scale(src->h, dst->h);

	if (vscale < 0 || dst->h == 0)
		return vscale;

	if (vscale < min_vscale || vscale > max_vscale)
		return -ERANGE;

	return vscale;
}

static int display_check(struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct rockchip_connector *conn = conn_state->connector;
	const struct rockchip_connector_funcs *conn_funcs = conn->funcs;
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;
	int ret;

	if (!state->is_init)
		return 0;

	if (conn_funcs->check) {
		ret = conn_funcs->check(conn, state);
		if (ret)
			goto check_fail;
	}

	if (crtc_funcs->check) {
		ret = crtc_funcs->check(state);
		if (ret)
			goto check_fail;
	}

	if (crtc_funcs->plane_check) {
		ret = crtc_funcs->plane_check(state);
		if (ret)
			goto check_fail;
	}

	return 0;

check_fail:
	state->is_init = false;
	return ret;
}

static int display_mode_valid(struct display_state *state)
{
	struct connector_state *conn_state = &state->conn_state;
	struct rockchip_connector *conn = conn_state->connector;
	const struct rockchip_connector_funcs *conn_funcs = conn->funcs;
	struct crtc_state *crtc_state = &state->crtc_state;
	const struct rockchip_crtc *crtc = crtc_state->crtc;
	const struct rockchip_crtc_funcs *crtc_funcs = crtc->funcs;
	int ret;

	if (!state->is_init)
		return 0;

	if (conn_funcs->mode_valid) {
		ret = conn_funcs->mode_valid(conn, state);
		if (ret)
			goto invalid_mode;
	}

	if (crtc_funcs->mode_valid) {
		ret = crtc_funcs->mode_valid(state);
		if (ret)
			goto invalid_mode;
	}

	return 0;

invalid_mode:
	state->is_init = false;
	return ret;
}

static int display_logo(struct display_state *state)
{
	struct crtc_state *crtc_state = &state->crtc_state;
	struct connector_state *conn_state = &state->conn_state;
	struct logo_info *logo = &state->logo;
	int hdisplay, vdisplay, ret;

	ret = display_init(state);
	if (!state->is_init || ret)
		return -ENODEV;

	switch (logo->bpp) {
	case 16:
		crtc_state->format = ROCKCHIP_FMT_RGB565;
		break;
	case 24:
		crtc_state->format = ROCKCHIP_FMT_RGB888;
		break;
	case 32:
		crtc_state->format = ROCKCHIP_FMT_ARGB8888;
		break;
	default:
		printf("can't support bmp bits[%d]\n", logo->bpp);
		return -EINVAL;
	}
	hdisplay = conn_state->mode.crtc_hdisplay;
	vdisplay = conn_state->mode.vdisplay;
	crtc_state->src_rect.w = logo->width;
	crtc_state->src_rect.h = logo->height;
	crtc_state->src_rect.x = 0;
	crtc_state->src_rect.y = 0;
	crtc_state->ymirror = logo->ymirror;
	crtc_state->rb_swap = 0;

	crtc_state->dma_addr = (u32)(unsigned long)logo->mem + logo->offset;
	crtc_state->xvir = ALIGN(crtc_state->src_rect.w * logo->bpp, 32) >> 5;

	if (state->logo_mode == ROCKCHIP_DISPLAY_FULLSCREEN) {
		crtc_state->crtc_rect.x = 0;
		crtc_state->crtc_rect.y = 0;
		crtc_state->crtc_rect.w = hdisplay;
		crtc_state->crtc_rect.h = vdisplay;
	} else {
		if (crtc_state->src_rect.w >= hdisplay) {
			crtc_state->crtc_rect.x = 0;
			crtc_state->crtc_rect.w = hdisplay;
		} else {
			crtc_state->crtc_rect.x = (hdisplay - crtc_state->src_rect.w) / 2;
			crtc_state->crtc_rect.w = crtc_state->src_rect.w;
		}

		if (crtc_state->src_rect.h >= vdisplay) {
			crtc_state->crtc_rect.y = 0;
			crtc_state->crtc_rect.h = vdisplay;
		} else {
			crtc_state->crtc_rect.y = (vdisplay - crtc_state->src_rect.h) / 2;
			crtc_state->crtc_rect.h = crtc_state->src_rect.h;
		}
	}

	display_mode_valid(state);
	display_check(state);
	display_set_plane(state);
	display_enable(state);

	return 0;
}

static int get_crtc_id(ofnode connect, bool is_ports_node)
{
	struct device_node *port_node;
	struct device_node *remote;
	int phandle;
	int val;

	if (is_ports_node) {
		port_node = of_get_parent(connect.np);
		if (!port_node)
			goto err;

		val = ofnode_read_u32_default(np_to_ofnode(port_node), "reg", -1);
		if (val < 0)
			goto err;
	} else {
		phandle = ofnode_read_u32_default(connect, "remote-endpoint", -1);
		if (phandle < 0)
			goto err;

		remote = of_find_node_by_phandle(phandle);
		if (!remote)
			goto err;

		val = ofnode_read_u32_default(np_to_ofnode(remote), "reg", -1);
		if (val < 0)
			goto err;
	}

	return val;
err:
	printf("Can't get crtc id, default set to id = 0\n");
	return 0;
}

static int get_crtc_mcu_mode(struct crtc_state *crtc_state)
{
	ofnode mcu_node;
	int total_pixel, cs_pst, cs_pend, rw_pst, rw_pend;

	mcu_node = dev_read_subnode(crtc_state->dev, "mcu-timing");
	if (!ofnode_valid(mcu_node))
		return -ENODEV;

#define FDT_GET_MCU_INT(val, name) \
	do { \
		val = ofnode_read_s32_default(mcu_node, name, -1); \
		if (val < 0) { \
			printf("Can't get %s\n", name); \
			return -ENXIO; \
		} \
	} while (0)

	FDT_GET_MCU_INT(total_pixel, "mcu-pix-total");
	FDT_GET_MCU_INT(cs_pst, "mcu-cs-pst");
	FDT_GET_MCU_INT(cs_pend, "mcu-cs-pend");
	FDT_GET_MCU_INT(rw_pst, "mcu-rw-pst");
	FDT_GET_MCU_INT(rw_pend, "mcu-rw-pend");

	crtc_state->mcu_timing.mcu_pix_total = total_pixel;
	crtc_state->mcu_timing.mcu_cs_pst = cs_pst;
	crtc_state->mcu_timing.mcu_cs_pend = cs_pend;
	crtc_state->mcu_timing.mcu_rw_pst = rw_pst;
	crtc_state->mcu_timing.mcu_rw_pend = rw_pend;

	return 0;
}

struct rockchip_logo_cache *find_or_alloc_logo_cache(const char *bmp)
{
	struct rockchip_logo_cache *tmp, *logo_cache = NULL;

	list_for_each_entry(tmp, &logo_cache_list, head) {
		if (!strcmp(tmp->name, bmp)) {
			logo_cache = tmp;
			break;
		}
	}

	if (!logo_cache) {
		logo_cache = malloc(sizeof(*logo_cache));
		if (!logo_cache) {
			printf("failed to alloc memory for logo cache\n");
			return NULL;
		}
		memset(logo_cache, 0, sizeof(*logo_cache));
		strcpy(logo_cache->name, bmp);
		INIT_LIST_HEAD(&logo_cache->head);
		list_add_tail(&logo_cache->head, &logo_cache_list);
	}

	return logo_cache;
}

/* Note: used only for rkfb kernel driver */
static int load_kernel_bmp_logo(struct logo_info *logo, const char *bmp_name)
{
#ifdef CONFIG_ROCKCHIP_RESOURCE_IMAGE
	void *dst = NULL;
	int len, size;
	struct bmp_header *header;

	if (!logo || !bmp_name)
		return -EINVAL;

	header = malloc(RK_BLK_SIZE);
	if (!header)
		return -ENOMEM;

	len = rockchip_read_resource_file(header, bmp_name, 0, RK_BLK_SIZE);
	if (len != RK_BLK_SIZE) {
		free(header);
		return -EINVAL;
	}
	size = get_unaligned_le32(&header->file_size);
	dst = (void *)(memory_start + MEMORY_POOL_SIZE / 2);
	len = rockchip_read_resource_file(dst, bmp_name, 0, size);
	if (len != size) {
		printf("failed to load bmp %s\n", bmp_name);
		free(header);
		return -ENOENT;
	}

	logo->mem = dst;
#endif

	return 0;
}

static int load_bmp_logo(struct logo_info *logo, const char *bmp_name)
{
#ifdef CONFIG_ROCKCHIP_RESOURCE_IMAGE
	struct rockchip_logo_cache *logo_cache;
	struct bmp_header *header;
	void *dst = NULL, *pdst;
	int size, len;
	int ret = 0;
	int reserved = 0;
	int dst_size;

	if (!logo || !bmp_name)
		return -EINVAL;
	logo_cache = find_or_alloc_logo_cache(bmp_name);
	if (!logo_cache)
		return -ENOMEM;

	if (logo_cache->logo.mem) {
		memcpy(logo, &logo_cache->logo, sizeof(*logo));
		return 0;
	}

	header = malloc(RK_BLK_SIZE);
	if (!header)
		return -ENOMEM;

	len = rockchip_read_resource_file(header, bmp_name, 0, RK_BLK_SIZE);
	if (len != RK_BLK_SIZE) {
		ret = -EINVAL;
		goto free_header;
	}

	logo->bpp = get_unaligned_le16(&header->bit_count);
	logo->width = get_unaligned_le32(&header->width);
	logo->height = get_unaligned_le32(&header->height);
	dst_size = logo->width * logo->height * logo->bpp >> 3;
	reserved = get_unaligned_le32(&header->reserved);
	if (logo->height < 0)
	    logo->height = -logo->height;
	size = get_unaligned_le32(&header->file_size);
	if (!can_direct_logo(logo->bpp)) {
		if (size > MEMORY_POOL_SIZE) {
			printf("failed to use boot buf as temp bmp buffer\n");
			ret = -ENOMEM;
			goto free_header;
		}
		pdst = get_display_buffer(size);

	} else {
		pdst = get_display_buffer(size);
		dst = pdst;
	}

	len = rockchip_read_resource_file(pdst, bmp_name, 0, size);
	if (len != size) {
		printf("failed to load bmp %s\n", bmp_name);
		ret = -ENOENT;
		goto free_header;
	}

	if (!can_direct_logo(logo->bpp)) {
		/*
		 * TODO: force use 16bpp if bpp less than 16;
		 */
		logo->bpp = (logo->bpp <= 16) ? 16 : logo->bpp;
		dst_size = logo->width * logo->height * logo->bpp >> 3;
		dst = get_display_buffer(dst_size);
		if (!dst) {
			ret = -ENOMEM;
			goto free_header;
		}
		if (bmpdecoder(pdst, dst, logo->bpp)) {
			printf("failed to decode bmp %s\n", bmp_name);
			ret = -EINVAL;
			goto free_header;
		}

		logo->offset = 0;
		logo->ymirror = 0;
	} else {
		logo->offset = get_unaligned_le32(&header->data_offset);
		if (reserved == BMP_PROCESSED_FLAG)
			logo->ymirror = 0;
		else
			logo->ymirror = 1;
	}
	logo->mem = dst;

	memcpy(&logo_cache->logo, logo, sizeof(*logo));

	flush_dcache_range((ulong)dst, ALIGN((ulong)dst + dst_size, CONFIG_SYS_CACHELINE_SIZE));

free_header:

	free(header);

	return ret;
#else
	return -EINVAL;
#endif
}

void rockchip_show_fbbase(ulong fbbase)
{
	struct display_state *s;

	list_for_each_entry(s, &rockchip_display_list, head) {
		s->logo.mode = ROCKCHIP_DISPLAY_FULLSCREEN;
		s->logo.mem = (char *)fbbase;
		s->logo.width = DRM_ROCKCHIP_FB_WIDTH;
		s->logo.height = DRM_ROCKCHIP_FB_HEIGHT;
		s->logo.bpp = 32;
		s->logo.ymirror = 0;

		display_logo(s);
	}
}

int rockchip_show_bmp(const char *bmp)
{
	struct display_state *s;
	int ret = 0;

	if (!bmp) {
		list_for_each_entry(s, &rockchip_display_list, head)
			display_disable(s);
		return -ENOENT;
	}

	list_for_each_entry(s, &rockchip_display_list, head) {
		s->logo.mode = s->charge_logo_mode;
		if (load_bmp_logo(&s->logo, bmp))
			continue;
		ret = display_logo(s);
	}

	return ret;
}

int rockchip_show_logo(void)
{
	struct display_state *s;
	int ret = 0;

	list_for_each_entry(s, &rockchip_display_list, head) {
		s->logo.mode = s->logo_mode;
		if (load_bmp_logo(&s->logo, s->ulogo_name))
			printf("failed to display uboot logo\n");
		else
			ret = display_logo(s);

		/* Load kernel bmp in rockchip_display_fixup() later */
	}

	return ret;
}

enum {
	PORT_DIR_IN,
	PORT_DIR_OUT,
};

static const struct device_node *rockchip_of_graph_get_port_by_id(ofnode node, int id)
{
	ofnode ports, port;
	u32 reg;

	ports = ofnode_find_subnode(node, "ports");
	if (!ofnode_valid(ports))
		return NULL;

	ofnode_for_each_subnode(port, ports) {
		if (ofnode_read_u32(port, "reg", &reg))
			continue;

		if (reg == id)
			break;
	}

	if (reg == id)
		return ofnode_to_np(port);

	return NULL;
}

static const struct device_node *rockchip_of_graph_get_port_parent(ofnode port)
{
	ofnode parent;
	int is_ports_node;

	parent = ofnode_get_parent(port);
	is_ports_node = strstr(ofnode_to_np(parent)->full_name, "ports") ? 1 : 0;
	if (is_ports_node)
		parent = ofnode_get_parent(parent);

	return ofnode_to_np(parent);
}

static const struct device_node *rockchip_of_graph_get_remote_node(ofnode node, int port,
								   int endpoint)
{
	const struct device_node *port_node;
	ofnode ep;
	u32 reg;
	uint phandle;

	port_node = rockchip_of_graph_get_port_by_id(node, port);
	if (!port_node)
		return NULL;

	ofnode_for_each_subnode(ep, np_to_ofnode(port_node)) {
		if (ofnode_read_u32(ep, "reg", &reg))
			break;
		if (reg == endpoint)
			break;
	}

	if (!ofnode_valid(ep))
		return NULL;

	if (ofnode_read_u32(ep, "remote-endpoint", &phandle))
		return NULL;

	ep = ofnode_get_by_phandle(phandle);
	if (!ofnode_valid(ep))
		return NULL;

	return ofnode_to_np(ep);
}

static int rockchip_of_find_panel(struct udevice *dev, struct rockchip_panel **panel)
{
	const struct device_node *ep_node, *panel_node;
	ofnode panel_ofnode, port;
	struct udevice *panel_dev;
	int ret = 0;

	*panel = NULL;
	panel_ofnode = dev_read_subnode(dev, "panel");
	if (ofnode_valid(panel_ofnode) && ofnode_is_available(panel_ofnode)) {
		ret = uclass_get_device_by_ofnode(UCLASS_PANEL, panel_ofnode,
						  &panel_dev);
		if (!ret)
			goto found;
	}

	ep_node = rockchip_of_graph_get_remote_node(dev->node, PORT_DIR_OUT, 0);
	if (!ep_node)
		return -ENODEV;

	port = ofnode_get_parent(np_to_ofnode(ep_node));
	if (!ofnode_valid(port))
		return -ENODEV;

	panel_node = rockchip_of_graph_get_port_parent(port);
	if (!panel_node)
		return -ENODEV;

	ret = uclass_get_device_by_ofnode(UCLASS_PANEL, np_to_ofnode(panel_node), &panel_dev);
	if (!ret)
		goto found;

	return -ENODEV;

found:
	*panel = (struct rockchip_panel *)dev_get_driver_data(panel_dev);
	return 0;
}

static int rockchip_of_find_bridge(struct udevice *dev, struct rockchip_bridge **bridge)
{
	const struct device_node *ep_node, *bridge_node;
	ofnode port;
	struct udevice *bridge_dev;
	int ret = 0;

	ep_node = rockchip_of_graph_get_remote_node(dev->node, PORT_DIR_OUT, 0);
	if (!ep_node)
		return -ENODEV;

	port = ofnode_get_parent(np_to_ofnode(ep_node));
	if (!ofnode_valid(port))
		return -ENODEV;

	bridge_node = rockchip_of_graph_get_port_parent(port);
	if (!bridge_node)
		return -ENODEV;

	ret = uclass_get_device_by_ofnode(UCLASS_VIDEO_BRIDGE, np_to_ofnode(bridge_node),
					  &bridge_dev);
	if (!ret)
		goto found;

	return -ENODEV;

found:
	*bridge = (struct rockchip_bridge *)dev_get_driver_data(bridge_dev);
	return 0;
}

static int rockchip_of_find_panel_or_bridge(struct udevice *dev, struct rockchip_panel **panel,
					    struct rockchip_bridge **bridge)
{
	int ret = 0;
	*panel = NULL;
	*bridge = NULL;

	if (panel) {
		ret  = rockchip_of_find_panel(dev, panel);
		if (!ret)
			return 0;
	}

	if (ret) {
		ret = rockchip_of_find_bridge(dev, bridge);
		if (!ret)
			ret = rockchip_of_find_panel_or_bridge((*bridge)->dev, panel,
							       &(*bridge)->next_bridge);
	}

	return ret;
}

static struct rockchip_phy *rockchip_of_find_phy(struct udevice *dev)
{
	struct udevice *phy_dev;
	int ret;

	ret = uclass_get_device_by_phandle(UCLASS_PHY, dev, "phys", &phy_dev);
	if (ret)
		return NULL;

	return (struct rockchip_phy *)dev_get_driver_data(phy_dev);
}

static struct udevice *rockchip_of_find_connector_device(ofnode endpoint)
{
	ofnode ep, port, ports, conn;
	uint phandle;
	struct udevice *dev;
	int ret;

	if (ofnode_read_u32(endpoint, "remote-endpoint", &phandle))
		return NULL;

	ep = ofnode_get_by_phandle(phandle);
	if (!ofnode_valid(ep) || !ofnode_is_available(ep))
		return NULL;

	port = ofnode_get_parent(ep);
	if (!ofnode_valid(port))
		return NULL;

	ports = ofnode_get_parent(port);
	if (!ofnode_valid(ports))
		return NULL;

	conn = ofnode_get_parent(ports);
	if (!ofnode_valid(conn) || !ofnode_is_available(conn))
		return NULL;

	ret = uclass_get_device_by_ofnode(UCLASS_DISPLAY, conn, &dev);
	if (ret)
		return NULL;

	return dev;
}

static struct rockchip_connector *rockchip_of_get_connector(ofnode endpoint)
{
	struct rockchip_connector *conn;
	struct udevice *dev;
	int ret;

	dev = rockchip_of_find_connector_device(endpoint);
	if (!dev) {
		printf("Warn: can't find connect driver\n");
		return NULL;
	}

	conn = get_rockchip_connector_by_device(dev);
	if (!conn)
		return NULL;
	ret = rockchip_of_find_panel_or_bridge(dev, &conn->panel, &conn->bridge);
	if (ret)
		debug("Warn: no find panel or bridge\n");

	conn->phy = rockchip_of_find_phy(dev);

	return conn;
}

static struct rockchip_connector *rockchip_get_split_connector(struct rockchip_connector *conn)
{
	char *conn_name;
	struct device_node *split_node;
	struct udevice *split_dev;
	struct rockchip_connector *split_conn;
	bool split_mode;
	int ret;

	split_mode = ofnode_read_bool(conn->dev->node, "split-mode");
	if (!split_mode)
		return NULL;

	switch (conn->type) {
	case DRM_MODE_CONNECTOR_DisplayPort:
		conn_name = "dp";
		break;
	case DRM_MODE_CONNECTOR_eDP:
		conn_name = "edp";
		break;
	case DRM_MODE_CONNECTOR_HDMIA:
		conn_name = "hdmi";
		break;
	default:
		return NULL;
	}

	split_node = of_alias_get_dev(conn_name, !conn->id);
	if (!split_node || !of_device_is_available(split_node))
		return NULL;

	ret = uclass_get_device_by_ofnode(UCLASS_DISPLAY, np_to_ofnode(split_node), &split_dev);
	if (ret)
		return NULL;

	split_conn = get_rockchip_connector_by_device(split_dev);
	if (!split_conn)
		return NULL;
	ret = rockchip_of_find_panel_or_bridge(split_dev, &split_conn->panel, &split_conn->bridge);
	if (ret)
		debug("Warn: no find panel or bridge\n");

	split_conn->phy = rockchip_of_find_phy(split_dev);

	return split_conn;
}

static bool rockchip_get_display_path_status(ofnode endpoint)
{
	ofnode ep;
	uint phandle;

	if (ofnode_read_u32(endpoint, "remote-endpoint", &phandle))
		return false;

	ep = ofnode_get_by_phandle(phandle);
	if (!ofnode_valid(ep) || !ofnode_is_available(ep))
		return false;

	return true;
}

#if defined(CONFIG_ROCKCHIP_RK3568)
static int rockchip_display_fixup_dts(void *blob)
{
	ofnode route_node, route_subnode, conn_ep, conn_port;
	const struct device_node *route_sub_devnode;
	const struct device_node *ep_node, *conn_ep_dev_node;
	u32 phandle;
	int conn_ep_offset;
	const char *route_sub_path, *path;

	/* Don't go further if new variant after
	 * reading PMUGRF_SOC_CON15
	 */
	if ((readl(0xfdc20100) & GENMASK(15, 14)))
		return 0;

	route_node = ofnode_path("/display-subsystem/route");
	if (!ofnode_valid(route_node))
		return -EINVAL;

	ofnode_for_each_subnode(route_subnode, route_node) {
		if (!ofnode_is_available(route_subnode))
			continue;

		route_sub_devnode = ofnode_to_np(route_subnode);
		route_sub_path = route_sub_devnode->full_name;
		if (!strstr(ofnode_get_name(route_subnode), "dsi") &&
		    !strstr(ofnode_get_name(route_subnode), "edp"))
			return 0;

		phandle = ofnode_read_u32_default(route_subnode, "connect", -1);
		if (phandle < 0) {
			printf("Warn: can't find connect node's handle\n");
			continue;
		}

		ep_node = of_find_node_by_phandle(phandle);
		if (!ofnode_valid(np_to_ofnode(ep_node))) {
			printf("Warn: can't find endpoint node from phandle\n");
			continue;
		}

		ofnode_read_u32(np_to_ofnode(ep_node), "remote-endpoint", &phandle);
		conn_ep = ofnode_get_by_phandle(phandle);
		if (!ofnode_valid(conn_ep) || !ofnode_is_available(conn_ep))
			return -ENODEV;

		conn_port = ofnode_get_parent(conn_ep);
		if (!ofnode_valid(conn_port))
			return -ENODEV;

		ofnode_for_each_subnode(conn_ep, conn_port) {
			conn_ep_dev_node = ofnode_to_np(conn_ep);
			path = conn_ep_dev_node->full_name;
			ofnode_read_u32(conn_ep, "remote-endpoint", &phandle);
			conn_ep_offset = fdt_path_offset(blob, path);

			if (!ofnode_is_available(conn_ep) &&
			    strstr(ofnode_get_name(conn_ep), "endpoint@0")) {
				do_fixup_by_path_u32(blob, route_sub_path,
						     "connect", phandle, 1);
				fdt_status_okay(blob, conn_ep_offset);

			} else if (ofnode_is_available(conn_ep) &&
				   strstr(ofnode_get_name(conn_ep), "endpoint@1")) {
				fdt_status_disabled(blob, conn_ep_offset);
			}
		}
	}

	return 0;
}
#endif

static int rockchip_display_probe(struct udevice *dev)
{
	struct video_priv *uc_priv = dev_get_uclass_priv(dev);
	struct video_uc_platdata *plat = dev_get_uclass_platdata(dev);
	const void *blob = gd->fdt_blob;
	int phandle;
	struct udevice *crtc_dev;
	struct rockchip_crtc *crtc;
	struct rockchip_connector *conn, *split_conn;
	struct display_state *s;
	const char *name;
	int ret;
	ofnode node, route_node, timing_node;
	struct device_node *port_node, *vop_node, *ep_node, *port_parent_node;
	struct public_phy_data *data;
	bool is_ports_node = false;

#if defined(CONFIG_ROCKCHIP_RK3568)
	rockchip_display_fixup_dts((void *)blob);
#endif
	/* Before relocation we don't need to do anything */
	if (!(gd->flags & GD_FLG_RELOC))
		return 0;

	data = malloc(sizeof(struct public_phy_data));
	if (!data) {
		printf("failed to alloc phy data\n");
		return -ENOMEM;
	}
	data->phy_init = false;

	init_display_buffer(plat->base);

	route_node = dev_read_subnode(dev, "route");
	if (!ofnode_valid(route_node))
		return -ENODEV;

	ofnode_for_each_subnode(node, route_node) {
		if (!ofnode_is_available(node))
			continue;
		phandle = ofnode_read_u32_default(node, "connect", -1);
		if (phandle < 0) {
			printf("Warn: can't find connect node's handle\n");
			continue;
		}
		ep_node = of_find_node_by_phandle(phandle);
		if (!ofnode_valid(np_to_ofnode(ep_node))) {
			printf("Warn: can't find endpoint node from phandle\n");
			continue;
		}
		port_node = of_get_parent(ep_node);
		if (!ofnode_valid(np_to_ofnode(port_node))) {
			printf("Warn: can't find port node from phandle\n");
			continue;
		}

		port_parent_node = of_get_parent(port_node);
		if (!ofnode_valid(np_to_ofnode(port_parent_node))) {
			printf("Warn: can't find port parent node from phandle\n");
			continue;
		}

		is_ports_node = strstr(port_parent_node->full_name, "ports") ? 1 : 0;
		if (is_ports_node) {
			vop_node = of_get_parent(port_parent_node);
			if (!ofnode_valid(np_to_ofnode(vop_node))) {
				printf("Warn: can't find crtc node from phandle\n");
				continue;
			}
		} else {
			vop_node = port_parent_node;
		}

		ret = uclass_get_device_by_ofnode(UCLASS_VIDEO_CRTC,
						  np_to_ofnode(vop_node),
						  &crtc_dev);
		if (ret) {
			printf("Warn: can't find crtc driver %d\n", ret);
			continue;
		}
		crtc = (struct rockchip_crtc *)dev_get_driver_data(crtc_dev);

		conn = rockchip_of_get_connector(np_to_ofnode(ep_node));
		if (!conn) {
			printf("Warn: can't get connect driver\n");
			continue;
		}
		split_conn = rockchip_get_split_connector(conn);

		s = malloc(sizeof(*s));
		if (!s)
			continue;

		memset(s, 0, sizeof(*s));

		INIT_LIST_HEAD(&s->head);
		ret = ofnode_read_string_index(node, "logo,uboot", 0, &name);
		if (!ret)
			memcpy(s->ulogo_name, name, strlen(name));
		ret = ofnode_read_string_index(node, "logo,kernel", 0, &name);
		if (!ret)
			memcpy(s->klogo_name, name, strlen(name));
		ret = ofnode_read_string_index(node, "logo,mode", 0, &name);
		if (!strcmp(name, "fullscreen"))
			s->logo_mode = ROCKCHIP_DISPLAY_FULLSCREEN;
		else
			s->logo_mode = ROCKCHIP_DISPLAY_CENTER;
		ret = ofnode_read_string_index(node, "charge_logo,mode", 0, &name);
		if (!strcmp(name, "fullscreen"))
			s->charge_logo_mode = ROCKCHIP_DISPLAY_FULLSCREEN;
		else
			s->charge_logo_mode = ROCKCHIP_DISPLAY_CENTER;

		s->force_output = ofnode_read_bool(node, "force-output");

		if (s->force_output) {
			timing_node = ofnode_find_subnode(node, "force_timing");
			ret = display_get_force_timing_from_dts(timing_node, &s->force_mode);
			if (ofnode_read_u32(node, "force-bus-format", &s->force_bus_format))
				s->force_bus_format = MEDIA_BUS_FMT_RGB888_1X24;
		}

		s->blob = blob;
		s->conn_state.connector = conn;
		s->conn_state.secondary = NULL;
		s->conn_state.type = conn->type;
		if (split_conn) {
			s->conn_state.secondary = split_conn;
			s->conn_state.output_flags |= ROCKCHIP_OUTPUT_DUAL_CHANNEL_LEFT_RIGHT_MODE;
			s->conn_state.output_flags |= conn->id ? ROCKCHIP_OUTPUT_DATA_SWAP : 0;
		}
		s->conn_state.overscan.left_margin = 100;
		s->conn_state.overscan.right_margin = 100;
		s->conn_state.overscan.top_margin = 100;
		s->conn_state.overscan.bottom_margin = 100;
		s->crtc_state.node = np_to_ofnode(vop_node);
		s->crtc_state.dev = crtc_dev;
		s->crtc_state.crtc = crtc;
		s->crtc_state.crtc_id = get_crtc_id(np_to_ofnode(ep_node), is_ports_node);
		s->node = node;

		if (is_ports_node) { /* only vop2 will get into here */
			ofnode vp_node = np_to_ofnode(port_node);
			static bool get_plane_mask_from_dts;

			s->crtc_state.ports_node = port_parent_node;
			if (!get_plane_mask_from_dts) {
				ofnode vp_sub_node;
				int vp_id = 0;
				bool vp_enable = false;

				ofnode_for_each_subnode(vp_node, np_to_ofnode(port_parent_node)) {
					int cursor_plane = -1;

					vp_id = ofnode_read_u32_default(vp_node, "reg", 0);

					s->crtc_state.crtc->vps[vp_id].xmirror_en =
						ofnode_read_bool(vp_node, "xmirror-enable");

					ret = ofnode_read_u32_default(vp_node, "rockchip,plane-mask", 0);

					cursor_plane = ofnode_read_u32_default(vp_node, "cursor-win-id", -1);
					s->crtc_state.crtc->vps[vp_id].cursor_plane = cursor_plane;
					if (ret) {
						s->crtc_state.crtc->vps[vp_id].plane_mask = ret;
						s->crtc_state.crtc->assign_plane |= true;
						s->crtc_state.crtc->vps[vp_id].primary_plane_id =
							ofnode_read_u32_default(vp_node, "rockchip,primary-plane", U8_MAX);
						printf("get vp%d plane mask:0x%x, primary id:%d, cursor_plane:%d, from dts\n",
						       vp_id,
						       s->crtc_state.crtc->vps[vp_id].plane_mask,
						       s->crtc_state.crtc->vps[vp_id].primary_plane_id == U8_MAX ? -1 :
						       s->crtc_state.crtc->vps[vp_id].primary_plane_id,
						       cursor_plane);
					}

					/* To check current vp status */
					vp_enable = false;
					ofnode_for_each_subnode(vp_sub_node, vp_node)
						vp_enable |= rockchip_get_display_path_status(vp_sub_node);
					s->crtc_state.crtc->vps[vp_id].enable = vp_enable;
				}
				get_plane_mask_from_dts = true;
			}
		}

		get_crtc_mcu_mode(&s->crtc_state);

		ret = ofnode_read_u32_default(s->crtc_state.node,
					      "rockchip,dual-channel-swap", 0);
		s->crtc_state.dual_channel_swap = ret;

		if (connector_phy_init(conn, data)) {
			printf("Warn: Failed to init phy drivers\n");
			free(s);
			continue;
		}
		list_add_tail(&s->head, &rockchip_display_list);
	}

	if (list_empty(&rockchip_display_list)) {
		debug("Failed to found available display route\n");
		return -ENODEV;
	}
	rockchip_get_baseparameter();
	display_pre_init();

	uc_priv->xsize = DRM_ROCKCHIP_FB_WIDTH;
	uc_priv->ysize = DRM_ROCKCHIP_FB_HEIGHT;
	uc_priv->bpix = VIDEO_BPP32;

	#ifdef CONFIG_DRM_ROCKCHIP_VIDEO_FRAMEBUFFER
	rockchip_show_fbbase(plat->base);
	video_set_flush_dcache(dev, true);
	#endif

	return 0;
}

void rockchip_display_fixup(void *blob)
{
	const struct rockchip_connector_funcs *conn_funcs;
	const struct rockchip_crtc_funcs *crtc_funcs;
	struct rockchip_connector *conn;
	const struct rockchip_crtc *crtc;
	struct display_state *s;
	int offset;
	int ret;
	const struct device_node *np;
	const char *path;
	const char *cacm_header;

	if (fdt_node_offset_by_compatible(blob, 0, "rockchip,drm-logo") >= 0) {
		list_for_each_entry(s, &rockchip_display_list, head) {
			ret = load_bmp_logo(&s->logo, s->klogo_name);
			if (ret < 0) {
				s->is_klogo_valid = false;
				printf("VP%d fail to load kernel logo\n", s->crtc_state.crtc_id);
			} else {
				s->is_klogo_valid = true;
			}
		}

		if (!get_display_size())
			return;

		offset = fdt_update_reserved_memory(blob, "rockchip,drm-logo",
						    (u64)memory_start,
						    (u64)get_display_size());
		if (offset < 0)
			printf("failed to reserve drm-loader-logo memory\n");

		offset = fdt_update_reserved_memory(blob, "rockchip,drm-cubic-lut",
						    (u64)cubic_lut_memory_start,
						    (u64)get_cubic_memory_size());
		if (offset < 0)
			printf("failed to reserve drm-cubic-lut memory\n");
	} else {
		printf("can't found rockchip,drm-logo, use rockchip,fb-logo\n");
		/* Compatible with rkfb display, only need reserve memory */
		offset = fdt_update_reserved_memory(blob, "rockchip,fb-logo",
						    (u64)memory_start,
						    MEMORY_POOL_SIZE);
		if (offset < 0)
			printf("failed to reserve fb-loader-logo memory\n");
		else
			list_for_each_entry(s, &rockchip_display_list, head)
				load_kernel_bmp_logo(&s->logo, s->klogo_name);
		return;
	}

	list_for_each_entry(s, &rockchip_display_list, head) {
		if (!s->is_init || !s->is_klogo_valid)
			continue;

		conn = s->conn_state.connector;
		if (!conn)
			continue;
		conn_funcs = conn->funcs;
		if (!conn_funcs) {
			printf("failed to get exist connector\n");
			continue;
		}

		if (s->conn_state.secondary) {
			s->conn_state.mode.clock *= 2;
			s->conn_state.mode.hdisplay *= 2;
		}

		crtc = s->crtc_state.crtc;
		if (!crtc)
			continue;

		crtc_funcs = crtc->funcs;
		if (!crtc_funcs) {
			printf("failed to get exist crtc\n");
			continue;
		}

		if (crtc_funcs->fixup_dts)
			crtc_funcs->fixup_dts(s, blob);

		np = ofnode_to_np(s->node);
		path = np->full_name;
		fdt_increase_size(blob, 0x400);
#define FDT_SET_U32(name, val) \
		do_fixup_by_path_u32(blob, path, name, val, 1);

		offset = s->logo.offset + (u32)(unsigned long)s->logo.mem
			 - memory_start;
		FDT_SET_U32("logo,offset", offset);
		FDT_SET_U32("logo,width", s->logo.width);
		FDT_SET_U32("logo,height", s->logo.height);
		FDT_SET_U32("logo,bpp", s->logo.bpp);
		FDT_SET_U32("logo,ymirror", s->logo.ymirror);
		FDT_SET_U32("video,clock", s->conn_state.mode.clock);
		FDT_SET_U32("video,hdisplay", s->conn_state.mode.hdisplay);
		FDT_SET_U32("video,vdisplay", s->conn_state.mode.vdisplay);
		FDT_SET_U32("video,crtc_hsync_end", s->conn_state.mode.crtc_hsync_end);
		FDT_SET_U32("video,crtc_vsync_end", s->conn_state.mode.crtc_vsync_end);
		FDT_SET_U32("video,vrefresh",
			    drm_mode_vrefresh(&s->conn_state.mode));
		FDT_SET_U32("video,flags", s->conn_state.mode.flags);
		FDT_SET_U32("video,aspect_ratio", s->conn_state.mode.picture_aspect_ratio);
		FDT_SET_U32("overscan,left_margin", s->conn_state.overscan.left_margin);
		FDT_SET_U32("overscan,right_margin", s->conn_state.overscan.right_margin);
		FDT_SET_U32("overscan,top_margin", s->conn_state.overscan.top_margin);
		FDT_SET_U32("overscan,bottom_margin", s->conn_state.overscan.bottom_margin);

		if (s->conn_state.disp_info) {
			cacm_header = (const char*)&s->conn_state.disp_info->cacm_header;

			FDT_SET_U32("bcsh,brightness", s->conn_state.disp_info->bcsh_info.brightness);
			FDT_SET_U32("bcsh,contrast", s->conn_state.disp_info->bcsh_info.contrast);
			FDT_SET_U32("bcsh,saturation", s->conn_state.disp_info->bcsh_info.saturation);
			FDT_SET_U32("bcsh,hue", s->conn_state.disp_info->bcsh_info.hue);

			if (!strncasecmp(cacm_header, "CACM", 4)) {
				FDT_SET_U32("post_csc,hue",
					    s->conn_state.disp_info->csc_info.hue);
				FDT_SET_U32("post_csc,saturation",
					    s->conn_state.disp_info->csc_info.saturation);
				FDT_SET_U32("post_csc,contrast",
					    s->conn_state.disp_info->csc_info.contrast);
				FDT_SET_U32("post_csc,brightness",
					    s->conn_state.disp_info->csc_info.brightness);
				FDT_SET_U32("post_csc,r_gain",
					    s->conn_state.disp_info->csc_info.r_gain);
				FDT_SET_U32("post_csc,g_gain",
					    s->conn_state.disp_info->csc_info.g_gain);
				FDT_SET_U32("post_csc,b_gain",
					    s->conn_state.disp_info->csc_info.b_gain);
				FDT_SET_U32("post_csc,r_offset",
					    s->conn_state.disp_info->csc_info.r_offset);
				FDT_SET_U32("post_csc,g_offset",
					    s->conn_state.disp_info->csc_info.g_offset);
				FDT_SET_U32("post_csc,b_offset",
					    s->conn_state.disp_info->csc_info.b_offset);
				FDT_SET_U32("post_csc,csc_enable",
					    s->conn_state.disp_info->csc_info.csc_enable);
			}
		}

		if (s->conn_state.disp_info->cubic_lut_data.size &&
		    CONFIG_ROCKCHIP_CUBIC_LUT_SIZE)
			FDT_SET_U32("cubic_lut,offset", get_cubic_lut_offset(s->crtc_state.crtc_id));

#undef FDT_SET_U32
	}
}

int rockchip_display_bind(struct udevice *dev)
{
	struct video_uc_platdata *plat = dev_get_uclass_platdata(dev);

	plat->size = DRM_ROCKCHIP_FB_SIZE + MEMORY_POOL_SIZE;

	return 0;
}

static const struct udevice_id rockchip_display_ids[] = {
	{ .compatible = "rockchip,display-subsystem" },
	{ }
};

U_BOOT_DRIVER(rockchip_display) = {
	.name	= "rockchip_display",
	.id	= UCLASS_VIDEO,
	.of_match = rockchip_display_ids,
	.bind	= rockchip_display_bind,
	.probe	= rockchip_display_probe,
};

static int do_rockchip_logo_show(cmd_tbl_t *cmdtp, int flag, int argc,
			char *const argv[])
{
	if (argc != 1)
		return CMD_RET_USAGE;

	rockchip_show_logo();

	return 0;
}

static int do_rockchip_show_bmp(cmd_tbl_t *cmdtp, int flag, int argc,
				char *const argv[])
{
	if (argc != 2)
		return CMD_RET_USAGE;

	rockchip_show_bmp(argv[1]);

	return 0;
}

U_BOOT_CMD(
	rockchip_show_logo, 1, 1, do_rockchip_logo_show,
	"load and display log from resource partition",
	NULL
);

U_BOOT_CMD(
	rockchip_show_bmp, 2, 1, do_rockchip_show_bmp,
	"load and display bmp from resource partition",
	"    <bmp_name>"
);
