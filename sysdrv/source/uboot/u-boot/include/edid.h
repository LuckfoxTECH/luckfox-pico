/*
 * Copyright (c) 2012 The Chromium OS Authors.
 *
 * (C) Copyright 2010
 * Petr Stetiar <ynezz@true.cz>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 *
 * Contains stolen code from ddcprobe project which is:
 * Copyright (C) Nalin Dahyabhai <bigfun@pobox.com>
 * (C) Copyright 2008-2017 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef __EDID_H_
#define __EDID_H_

#include <div64.h>
#include <linux/types.h>
#include <drm_modes.h>
#include <i2c.h>

/* Size of the EDID data */
#define EDID_SIZE	128
#define EDID_EXT_SIZE	256
#define MODE_LEN	240

#define CEA_EXT	    0x02
#define VTB_EXT	    0x10
#define DI_EXT	    0x40
#define LS_EXT	    0x50
#define MI_EXT	    0x60
#define DISPLAYID_EXT 0x70

#define EDID_TIMING_ASPECT_SHIFT 6
#define EDID_TIMING_ASPECT_MASK  (0x3 << EDID_TIMING_ASPECT_SHIFT)

/* need to add 60 */
#define EDID_TIMING_VFREQ_SHIFT  0
#define EDID_TIMING_VFREQ_MASK   (0x3f << EDID_TIMING_VFREQ_SHIFT)

/* OUI of HDMI vendor specific data block */
#define HDMI_IEEE_OUI 0x000c03

/* drm mode 4k and 3d */
#define DRM_MODE_FLAG_420_MASK			(0x03 << 23)
#define  DRM_MODE_FLAG_420			BIT(23)
#define  DRM_MODE_FLAG_420_ONLY			BIT(24)

#define BITS_PER_BYTE         8
#define BITS_TO_LONGS(nr)     DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#define GET_BIT(_x, _pos) \
	(((_x) >> (_pos)) & 1)
#define GET_BITS(_x, _pos_msb, _pos_lsb) \
	(((_x) >> (_pos_lsb)) & ((1 << ((_pos_msb) - (_pos_lsb) + 1)) - 1))
#define DRM_MODE(t, c, hd, hss, hse, ht, vd, vss, vse, vt, vs, f) \
	.clock = (c), .type = (t),\
	.hdisplay = (hd), .hsync_start = (hss), .hsync_end = (hse), \
	.htotal = (ht), .vdisplay = (vd), \
	.vsync_start = (vss), .vsync_end = (vse), .vtotal = (vt), \
	.vscan = (vs), .flags = (f)

#define DDC_SEGMENT_ADDR 0x30
#define DDC_ADDR 0x50
#define HDMI_EDID_BLOCK_SIZE 128
#define SCDC_I2C_SLAVE_ADDRESS 0x54

/* Aspect ratios used in EDID info. */
enum edid_aspect {
	ASPECT_625 = 0,
	ASPECT_75,
	ASPECT_8,
	ASPECT_5625,
};

struct est_timings {
	u8 t1;
	u8 t2;
	u8 mfg_rsvd;
} __packed;

/* 00=16:10, 01=4:3, 10=5:4, 11=16:9 */
#define EDID_TIMING_ASPECT_SHIFT 6
#define EDID_TIMING_ASPECT_MASK  (0x3 << EDID_TIMING_ASPECT_SHIFT)

/* need to add 60 */
#define EDID_TIMING_VFREQ_SHIFT  0
#define EDID_TIMING_VFREQ_MASK   (0x3f << EDID_TIMING_VFREQ_SHIFT)

struct std_timing {
	u8 hsize; /* need to multiply by 8 then add 248 */
	u8 vfreq_aspect;
} __packed;

struct detailed_pixel_timing {
	u8 hactive_lo;
	u8 hblank_lo;
	u8 hactive_hblank_hi;
	u8 vactive_lo;
	u8 vblank_lo;
	u8 vactive_vblank_hi;
	u8 hsync_offset_lo;
	u8 hsync_pulse_width_lo;
	u8 vsync_offset_pulse_width_lo;
	u8 hsync_vsync_offset_pulse_width_hi;
	u8 width_mm_lo;
	u8 height_mm_lo;
	u8 width_height_mm_hi;
	u8 hborder;
	u8 vborder;
	u8 misc;
} __packed;

/* If it's not pixel timing, it'll be one of the below */
struct detailed_data_string {
	u8 str[13];
} __packed;

struct detailed_data_monitor_range {
	u8 min_vfreq;
	u8 max_vfreq;
	u8 min_hfreq_khz;
	u8 max_hfreq_khz;
	u8 pixel_clock_mhz; /* need to multiply by 10 */
	u8 flags;
	union {
		struct {
			u8 reserved;
			u8 hfreq_start_khz; /* need to multiply by 2 */
			u8 c; /* need to divide by 2 */
			__le16 m;
			u8 k;
			u8 j; /* need to divide by 2 */
		} __packed gtf2;
		struct {
			u8 version;
			u8 data1; /* high 6 bits: extra clock resolution */
			u8 data2; /* plus low 2 of above: max hactive */
			u8 supported_aspects;
			u8 flags; /* preferred aspect and blanking support */
			u8 supported_scalings;
			u8 preferred_refresh;
		} __packed cvt;
	} formula;
} __packed;

struct detailed_data_wpindex {
	u8 white_yx_lo; /* Lower 2 bits each */
	u8 white_x_hi;
	u8 white_y_hi;
	u8 gamma; /* need to divide by 100 then add 1 */
} __packed;

struct detailed_data_color_point {
	u8 windex1;
	u8 wpindex1[3];
	u8 windex2;
	u8 wpindex2[3];
} __packed;

struct cvt_timing {
	u8 code[3];
} __packed;

struct detailed_non_pixel {
	u8 pad1;
	u8 type; /* ff=serial, fe=string, fd=monitor range, fc=monitor name
		  * fb=color point data, fa=standard timing data,
		  * f9=undefined, f8=mfg. reserved
		  */
	u8 pad2;
	union {
		struct detailed_data_string str;
		struct detailed_data_monitor_range range;
		struct detailed_data_wpindex color;
		struct std_timing timings[6];
		struct cvt_timing cvt[4];
	} data;
} __packed;

#define EDID_DETAIL_EST_TIMINGS 0xf7
#define EDID_DETAIL_CVT_3BYTE 0xf8
#define EDID_DETAIL_COLOR_MGMT_DATA 0xf9
#define EDID_DETAIL_STD_MODES 0xfa
#define EDID_DETAIL_MONITOR_CPDATA 0xfb
#define EDID_DETAIL_MONITOR_NAME 0xfc
#define EDID_DETAIL_MONITOR_RANGE 0xfd
#define EDID_DETAIL_MONITOR_STRING 0xfe
#define EDID_DETAIL_MONITOR_SERIAL 0xff

struct detailed_timing {
	__le16 pixel_clock; /* need to multiply by 10 KHz */
	union {
		struct detailed_pixel_timing pixel_data;
		struct detailed_non_pixel other_data;
	} data;
} __packed;

/* Detailed timing information used in EDID v1.x */
struct edid_detailed_timing {
	unsigned char pixel_clock[2];
#define EDID_DETAILED_TIMING_PIXEL_CLOCK(_x) \
	(((((uint32_t)(_x).pixel_clock[1]) << 8) + \
	 (_x).pixel_clock[0]) * 10000)
	unsigned char horizontal_active;
	unsigned char horizontal_blanking;
	unsigned char horizontal_active_blanking_hi;
#define EDID_DETAILED_TIMING_HORIZONTAL_ACTIVE(_x) \
	((GET_BITS((_x).horizontal_active_blanking_hi, 7, 4) << 8) + \
	 (_x).horizontal_active)
#define EDID_DETAILED_TIMING_HORIZONTAL_BLANKING(_x) \
	((GET_BITS((_x).horizontal_active_blanking_hi, 3, 0) << 8) + \
	 (_x).horizontal_blanking)
	unsigned char vertical_active;
	unsigned char vertical_blanking;
	unsigned char vertical_active_blanking_hi;
#define EDID_DETAILED_TIMING_VERTICAL_ACTIVE(_x) \
	((GET_BITS((_x).vertical_active_blanking_hi, 7, 4) << 8) + \
	 (_x).vertical_active)
#define EDID_DETAILED_TIMING_VERTICAL_BLANKING(_x) \
	((GET_BITS((_x).vertical_active_blanking_hi, 3, 0) << 8) + \
	 (_x).vertical_blanking)
	unsigned char hsync_offset;
	unsigned char hsync_pulse_width;
	unsigned char vsync_offset_pulse_width;
	unsigned char hsync_vsync_offset_pulse_width_hi;
#define EDID_DETAILED_TIMING_HSYNC_OFFSET(_x) \
	((GET_BITS((_x).hsync_vsync_offset_pulse_width_hi, 7, 6) << 8) + \
	 (_x).hsync_offset)
#define EDID_DETAILED_TIMING_HSYNC_PULSE_WIDTH(_x) \
	((GET_BITS((_x).hsync_vsync_offset_pulse_width_hi, 5, 4) << 8) + \
	 (_x).hsync_pulse_width)
#define EDID_DETAILED_TIMING_VSYNC_OFFSET(_x) \
	((GET_BITS((_x).hsync_vsync_offset_pulse_width_hi, 3, 2) << 4) + \
	 GET_BITS((_x).vsync_offset_pulse_width, 7, 4))
#define EDID_DETAILED_TIMING_VSYNC_PULSE_WIDTH(_x) \
	((GET_BITS((_x).hsync_vsync_offset_pulse_width_hi, 1, 0) << 4) + \
	 GET_BITS((_x).vsync_offset_pulse_width, 3, 0))
	unsigned char himage_size;
	unsigned char vimage_size;
	unsigned char himage_vimage_size_hi;
#define EDID_DETAILED_TIMING_HIMAGE_SIZE(_x) \
	((GET_BITS((_x).himage_vimage_size_hi, 7, 4) << 8) + (_x).himage_size)
#define EDID_DETAILED_TIMING_VIMAGE_SIZE(_x) \
	((GET_BITS((_x).himage_vimage_size_hi, 3, 0) << 8) + (_x).vimage_size)
	unsigned char hborder;
	unsigned char vborder;
	unsigned char flags;
#define EDID_DETAILED_TIMING_FLAG_INTERLACED(_x) \
	GET_BIT((_x).flags, 7)
#define EDID_DETAILED_TIMING_FLAG_STEREO(_x) \
	GET_BITS((_x).flags, 6, 5)
#define EDID_DETAILED_TIMING_FLAG_DIGITAL_COMPOSITE(_x) \
	GET_BITS((_x).flags, 4, 3)
#define EDID_DETAILED_TIMING_FLAG_POLARITY(_x) \
	GET_BITS((_x).flags, 2, 1)
#define EDID_DETAILED_TIMING_FLAG_VSYNC_POLARITY(_x) \
	GET_BIT((_x).flags, 2)
#define EDID_DETAILED_TIMING_FLAG_HSYNC_POLARITY(_x) \
	GET_BIT((_x).flags, 1)
#define EDID_DETAILED_TIMING_FLAG_INTERLEAVED(_x) \
	GET_BIT((_x).flags, 0)
} __attribute__ ((__packed__));

enum edid_monitor_descriptor_types {
	EDID_MONITOR_DESCRIPTOR_SERIAL = 0xff,
	EDID_MONITOR_DESCRIPTOR_ASCII = 0xfe,
	EDID_MONITOR_DESCRIPTOR_RANGE = 0xfd,
	EDID_MONITOR_DESCRIPTOR_NAME = 0xfc,
};

struct edid_monitor_descriptor {
	uint16_t zero_flag_1;
	unsigned char zero_flag_2;
	unsigned char type;
	unsigned char zero_flag_3;
	union {
		char string[13];
		struct {
			unsigned char vertical_min;
			unsigned char vertical_max;
			unsigned char horizontal_min;
			unsigned char horizontal_max;
			unsigned char pixel_clock_max;
			unsigned char gtf_data[8];
		} range_data;
	} data;
} __attribute__ ((__packed__));

#define DRM_EDID_INPUT_SERRATION_VSYNC (1 << 0)
#define DRM_EDID_INPUT_SYNC_ON_GREEN   (1 << 1)
#define DRM_EDID_INPUT_COMPOSITE_SYNC  (1 << 2)
#define DRM_EDID_INPUT_SEPARATE_SYNCS  (1 << 3)
#define DRM_EDID_INPUT_BLANK_TO_BLACK  (1 << 4)
#define DRM_EDID_INPUT_VIDEO_LEVEL     (3 << 5)
#define DRM_EDID_INPUT_DIGITAL         (1 << 7)
#define DRM_EDID_DIGITAL_DEPTH_MASK    (7 << 4)
#define DRM_EDID_DIGITAL_DEPTH_UNDEF   (0 << 4)
#define DRM_EDID_DIGITAL_DEPTH_6       (1 << 4)
#define DRM_EDID_DIGITAL_DEPTH_8       (2 << 4)
#define DRM_EDID_DIGITAL_DEPTH_10      (3 << 4)
#define DRM_EDID_DIGITAL_DEPTH_12      (4 << 4)
#define DRM_EDID_DIGITAL_DEPTH_14      (5 << 4)
#define DRM_EDID_DIGITAL_DEPTH_16      (6 << 4)
#define DRM_EDID_DIGITAL_DEPTH_RSVD    (7 << 4)
#define DRM_EDID_DIGITAL_TYPE_UNDEF    (0)
#define DRM_EDID_DIGITAL_TYPE_DVI      (1)
#define DRM_EDID_DIGITAL_TYPE_HDMI_A   (2)
#define DRM_EDID_DIGITAL_TYPE_HDMI_B   (3)
#define DRM_EDID_DIGITAL_TYPE_MDDI     (4)
#define DRM_EDID_DIGITAL_TYPE_DP       (5)

#define DRM_EDID_FEATURE_DEFAULT_GTF      (1 << 0)
#define DRM_EDID_FEATURE_PREFERRED_TIMING (1 << 1)
#define DRM_EDID_FEATURE_STANDARD_COLOR   (1 << 2)
/* If analog */
/* 00=mono, 01=rgb, 10=non-rgb, 11=unknown */
#define DRM_EDID_FEATURE_DISPLAY_TYPE     (3 << 3)
/* If digital */
#define DRM_EDID_FEATURE_COLOR_MASK	  (3 << 3)
#define DRM_EDID_FEATURE_RGB		  (0 << 3)
#define DRM_EDID_FEATURE_RGB_YCRCB444	  (1 << 3)
#define DRM_EDID_FEATURE_RGB_YCRCB422	  (2 << 3)
/* both 4:4:4 and 4:2:2 */
#define DRM_EDID_FEATURE_RGB_YCRCB	  (3 << 3)

#define DRM_EDID_FEATURE_PM_ACTIVE_OFF    (1 << 5)
#define DRM_EDID_FEATURE_PM_SUSPEND       (1 << 6)
#define DRM_EDID_FEATURE_PM_STANDBY       (1 << 7)

#define DRM_EDID_HDMI_DC_48               (1 << 6)
#define DRM_EDID_HDMI_DC_36               (1 << 5)
#define DRM_EDID_HDMI_DC_30               (1 << 4)
#define DRM_EDID_HDMI_DC_Y444             (1 << 3)

/* YCBCR 420 deep color modes */
#define DRM_EDID_YCBCR420_DC_48		  (1 << 2)
#define DRM_EDID_YCBCR420_DC_36		  (1 << 1)
#define DRM_EDID_YCBCR420_DC_30		  (1 << 0)
#define DRM_EDID_YCBCR420_DC_MASK (DRM_EDID_YCBCR420_DC_48 | \
				    DRM_EDID_YCBCR420_DC_36 | \
				    DRM_EDID_YCBCR420_DC_30)

/* HDMI 2.1 additional fields */
#define DRM_EDID_MAX_FRL_RATE_MASK		0xf0
#define DRM_EDID_FAPA_START_LOCATION		BIT(0)
#define DRM_EDID_ALLM				BIT(1)
#define DRM_EDID_FVA				BIT(2)

/* Deep Color specific */
#define DRM_EDID_DC_30BIT_420			BIT(0)
#define DRM_EDID_DC_36BIT_420			BIT(1)
#define DRM_EDID_DC_48BIT_420			BIT(2)

/* VRR specific */
#define DRM_EDID_CNMVRR				BIT(3)
#define DRM_EDID_CINEMA_VRR			BIT(4)
#define DRM_EDID_MDELTA				BIT(5)
#define DRM_EDID_VRR_MAX_UPPER_MASK		0xc0
#define DRM_EDID_VRR_MAX_LOWER_MASK		0xff
#define DRM_EDID_VRR_MIN_MASK			0x3f

/* DSC specific */
#define DRM_EDID_DSC_10BPC			BIT(0)
#define DRM_EDID_DSC_12BPC			BIT(1)
#define DRM_EDID_DSC_16BPC			BIT(2)
#define DRM_EDID_DSC_ALL_BPP			BIT(3)
#define DRM_EDID_DSC_NATIVE_420			BIT(6)
#define DRM_EDID_DSC_1P2			BIT(7)
#define DRM_EDID_DSC_MAX_FRL_RATE_MASK		0xf0
#define DRM_EDID_DSC_MAX_SLICES			0xf
#define DRM_EDID_DSC_TOTAL_CHUNK_KBYTES		0x3f

struct edid1_info {
	unsigned char header[8];
	unsigned char manufacturer_name[2];
#define EDID1_INFO_MANUFACTURER_NAME_ZERO(_x) \
	GET_BIT(((_x).manufacturer_name[0]), 7)
#define EDID1_INFO_MANUFACTURER_NAME_CHAR1(_x) \
	GET_BITS(((_x).manufacturer_name[0]), 6, 2)
#define EDID1_INFO_MANUFACTURER_NAME_CHAR2(_x) \
	((GET_BITS(((_x).manufacturer_name[0]), 1, 0) << 3) + \
	 GET_BITS(((_x).manufacturer_name[1]), 7, 5))
#define EDID1_INFO_MANUFACTURER_NAME_CHAR3(_x) \
	GET_BITS(((_x).manufacturer_name[1]), 4, 0)
	unsigned char product_code[2];
#define EDID1_INFO_PRODUCT_CODE(_x) \
	(((uint16_t)(_x).product_code[1] << 8) + (_x).product_code[0])
	unsigned char serial_number[4];
#define EDID1_INFO_SERIAL_NUMBER(_x) \
	(((uint32_t)(_x).serial_number[3] << 24) + \
	 ((_x).serial_number[2] << 16) + ((_x).serial_number[1] << 8) + \
	 (_x).serial_number[0])
	unsigned char week;
	unsigned char year;
	unsigned char version;
	unsigned char revision;
	unsigned char video_input_definition;
#define EDID1_INFO_VIDEO_INPUT_DIGITAL(_x) \
	GET_BIT(((_x).video_input_definition), 7)
#define EDID1_INFO_VIDEO_INPUT_VOLTAGE_LEVEL(_x) \
	GET_BITS(((_x).video_input_definition), 6, 5)
#define EDID1_INFO_VIDEO_INPUT_BLANK_TO_BLACK(_x) \
	GET_BIT(((_x).video_input_definition), 4)
#define EDID1_INFO_VIDEO_INPUT_SEPARATE_SYNC(_x) \
	GET_BIT(((_x).video_input_definition), 3)
#define EDID1_INFO_VIDEO_INPUT_COMPOSITE_SYNC(_x) \
	GET_BIT(((_x).video_input_definition), 2)
#define EDID1_INFO_VIDEO_INPUT_SYNC_ON_GREEN(_x) \
	GET_BIT(((_x).video_input_definition), 1)
#define EDID1_INFO_VIDEO_INPUT_SERRATION_V(_x) \
	GET_BIT(((_x).video_input_definition), 0)
	unsigned char max_size_horizontal;
	unsigned char max_size_vertical;
	unsigned char gamma;
	unsigned char feature_support;
#define EDID1_INFO_FEATURE_STANDBY(_x) \
	GET_BIT(((_x).feature_support), 7)
#define EDID1_INFO_FEATURE_SUSPEND(_x) \
	GET_BIT(((_x).feature_support), 6)
#define EDID1_INFO_FEATURE_ACTIVE_OFF(_x) \
	GET_BIT(((_x).feature_support), 5)
#define EDID1_INFO_FEATURE_DISPLAY_TYPE(_x) \
	GET_BITS(((_x).feature_support), 4, 3)
#define EDID1_INFO_FEATURE_RGB(_x) \
	GET_BIT(((_x).feature_support), 2)
#define EDID1_INFO_FEATURE_PREFERRED_TIMING_MODE(_x) \
	GET_BIT(((_x).feature_support), 1)
#define EDID1_INFO_FEATURE_DEFAULT_GTF_SUPPORT(_x) \
	GET_BIT(((_x).feature_support), 0)
	unsigned char color_characteristics[10];
	unsigned char established_timings[3];
#define EDID1_INFO_ESTABLISHED_TIMING_720X400_70(_x) \
	GET_BIT(((_x).established_timings[0]), 7)
#define EDID1_INFO_ESTABLISHED_TIMING_720X400_88(_x) \
	GET_BIT(((_x).established_timings[0]), 6)
#define EDID1_INFO_ESTABLISHED_TIMING_640X480_60(_x) \
	GET_BIT(((_x).established_timings[0]), 5)
#define EDID1_INFO_ESTABLISHED_TIMING_640X480_67(_x) \
	GET_BIT(((_x).established_timings[0]), 4)
#define EDID1_INFO_ESTABLISHED_TIMING_640X480_72(_x) \
	GET_BIT(((_x).established_timings[0]), 3)
#define EDID1_INFO_ESTABLISHED_TIMING_640X480_75(_x) \
	GET_BIT(((_x).established_timings[0]), 2)
#define EDID1_INFO_ESTABLISHED_TIMING_800X600_56(_x) \
	GET_BIT(((_x).established_timings[0]), 1)
#define EDID1_INFO_ESTABLISHED_TIMING_800X600_60(_x) \
	GET_BIT(((_x).established_timings[0]), 0)
#define EDID1_INFO_ESTABLISHED_TIMING_800X600_72(_x) \
	GET_BIT(((_x).established_timings[1]), 7)
#define EDID1_INFO_ESTABLISHED_TIMING_800X600_75(_x) \
	GET_BIT(((_x).established_timings[1]), 6)
#define EDID1_INFO_ESTABLISHED_TIMING_832X624_75(_x) \
	GET_BIT(((_x).established_timings[1]), 5)
#define EDID1_INFO_ESTABLISHED_TIMING_1024X768_87I(_x) \
	GET_BIT(((_x).established_timings[1]), 4)
#define EDID1_INFO_ESTABLISHED_TIMING_1024X768_60(_x) \
	GET_BIT(((_x).established_timings[1]), 3)
#define EDID1_INFO_ESTABLISHED_TIMING_1024X768_70(_x) \
	GET_BIT(((_x).established_timings[1]), 2)
#define EDID1_INFO_ESTABLISHED_TIMING_1024X768_75(_x) \
	GET_BIT(((_x).established_timings[1]), 1)
#define EDID1_INFO_ESTABLISHED_TIMING_1280X1024_75(_x) \
	GET_BIT(((_x).established_timings[1]), 0)
#define EDID1_INFO_ESTABLISHED_TIMING_1152X870_75(_x) \
	GET_BIT(((_x).established_timings[2]), 7)
	struct {
		unsigned char xresolution;
		unsigned char aspect_vfreq;
	} __attribute__((__packed__)) standard_timings[8];
#define EDID1_INFO_STANDARD_TIMING_XRESOLUTION(_x, _i) \
	(((_x).standard_timings[_i]).xresolution)
#define EDID1_INFO_STANDARD_TIMING_ASPECT(_x, _i) \
	GET_BITS(((_x).standard_timings[_i].aspect_vfreq), 7, 6)
#define EDID1_INFO_STANDARD_TIMING_VFREQ(_x, _i) \
	GET_BITS(((_x).standard_timings[_i].aspect_vfreq), 5, 0)
	union {
		unsigned char timing[72];
		struct edid_monitor_descriptor descriptor[4];
	} monitor_details;
	unsigned char extension_flag;
	unsigned char checksum;
} __attribute__ ((__packed__));

enum edid_cea861_db_types {
	EDID_CEA861_DB_AUDIO = 0x01,
	EDID_CEA861_DB_VIDEO = 0x02,
	EDID_CEA861_DB_VENDOR = 0x03,
	EDID_CEA861_DB_SPEAKER = 0x04,
	EDID_CEA861_DB_USE_EXTENDED = 0x07,
};

#define EXT_VIDEO_CAPABILITY_BLOCK 0x00
#define EXT_VIDEO_DATA_BLOCK_420        0x0E
#define EXT_VIDEO_CAP_BLOCK_Y420CMDB 0x0F
#define EDID_BASIC_AUDIO        BIT(6)
#define EDID_CEA_YCRCB444       BIT(5)
#define EDID_CEA_YCRCB422       BIT(4)
#define EDID_CEA_VCDB_QS        BIT(6)

#define EXT_VIDEO_DATA_BLOCK_420 0x0E

struct edid_cea861_info {
	unsigned char extension_tag;
#define EDID_CEA861_EXTENSION_TAG	0x02
	unsigned char revision;
	unsigned char dtd_offset;
	unsigned char dtd_count;
#define EDID_CEA861_SUPPORTS_UNDERSCAN(_x) \
	GET_BIT(((_x).dtd_count), 7)
#define EDID_CEA861_SUPPORTS_BASIC_AUDIO(_x) \
	GET_BIT(((_x).dtd_count), 6)
#define EDID_CEA861_SUPPORTS_YUV444(_x) \
	GET_BIT(((_x).dtd_count), 5)
#define EDID_CEA861_SUPPORTS_YUV422(_x) \
	GET_BIT(((_x).dtd_count), 4)
#define EDID_CEA861_DTD_COUNT(_x) \
	GET_BITS(((_x).dtd_count), 3, 0)
	unsigned char data[124];
#define EDID_CEA861_DB_TYPE(_x, offset) \
	GET_BITS((_x).data[offset], 7, 5)
#define EDID_CEA861_DB_LEN(_x, offset) \
	GET_BITS((_x).data[offset], 4, 0)
} __attribute__ ((__packed__));

#define DATA_BLOCK_PRODUCT_ID 0x00
#define DATA_BLOCK_DISPLAY_PARAMETERS 0x01
#define DATA_BLOCK_COLOR_CHARACTERISTICS 0x02
#define DATA_BLOCK_TYPE_1_DETAILED_TIMING 0x03
#define DATA_BLOCK_TYPE_2_DETAILED_TIMING 0x04
#define DATA_BLOCK_TYPE_3_SHORT_TIMING 0x05
#define DATA_BLOCK_TYPE_4_DMT_TIMING 0x06
#define DATA_BLOCK_VESA_TIMING 0x07
#define DATA_BLOCK_CEA_TIMING 0x08
#define DATA_BLOCK_VIDEO_TIMING_RANGE 0x09
#define DATA_BLOCK_PRODUCT_SERIAL_NUMBER 0x0a
#define DATA_BLOCK_GP_ASCII_STRING 0x0b
#define DATA_BLOCK_DISPLAY_DEVICE_DATA 0x0c
#define DATA_BLOCK_INTERFACE_POWER_SEQUENCING 0x0d
#define DATA_BLOCK_TRANSFER_CHARACTERISTICS 0x0e
#define DATA_BLOCK_DISPLAY_INTERFACE 0x0f
#define DATA_BLOCK_STEREO_DISPLAY_INTERFACE 0x10
#define DATA_BLOCK_TILED_DISPLAY 0x12

struct displayid_hdr {
	u8 rev;
	u8 bytes;
	u8 prod_id;
	u8 ext_count;
} __packed;

struct displayid_block {
	u8 tag;
	u8 rev;
	u8 num_bytes;
} __packed;

struct displayid_detailed_timings_1 {
	u8 pixel_clock[3];
	u8 flags;
	u8 hactive[2];
	u8 hblank[2];
	u8 hsync[2];
	u8 hsw[2];
	u8 vactive[2];
	u8 vblank[2];
	u8 vsync[2];
	u8 vsw[2];
} __packed;

struct displayid_detailed_timing_block {
	struct displayid_block base;
	struct displayid_detailed_timings_1 timings[0];
};

/**
 * struct drm_scrambling: sink's scrambling support.
 */
struct drm_scrambling {
	/**
	 * @supported: scrambling supported for rates > 340 Mhz.
	 */
	bool supported;
	/**
	 * @low_rates: scrambling supported for rates <= 340 Mhz.
	 */
	bool low_rates;
};

/**
 * struct drm_scdc - Information about scdc capabilities of a HDMI 2.0 sink
 *
 * Provides SCDC register support and capabilities related information on a
 * HDMI 2.0 sink. In case of a HDMI 1.4 sink, all parameter must be 0.
 */

struct drm_scdc {
	/**
	 * @supported: status control & data channel present.
	 */
	bool supported;
	/**
	 * @read_request: sink is capable of generating scdc read request.
	 */
	bool read_request;
	/**
	 * @scrambling: sink's scrambling capabilities
	 */
	struct drm_scrambling scrambling;
};

/**
 * struct drm_hdmi_dsc_cap - DSC capabilities of HDMI sink
 *
 * Describes the DSC support provided by HDMI 2.1 sink.
 * The information is fetched fom additional HFVSDB blocks defined
 * for HDMI 2.1.
 */
struct drm_hdmi_dsc_cap {
	/** @v_1p2: flag for dsc1.2 version support by sink */
	bool v_1p2;

	/** @native_420: Does sink support DSC with 4:2:0 compression */
	bool native_420;

	/**
	 * @all_bpp: Does sink support all bpp with 4:4:4: or 4:2:2
	 * compressed formats
	 */
	bool all_bpp;

	/**
	 * @bpc_supported: compressed bpc supported by sink : 10, 12 or 16 bpc
	 */
	u8 bpc_supported;

	/** @max_slices: maximum number of Horizontal slices supported by */
	u8 max_slices;

	/** @clk_per_slice : max pixel clock in MHz supported per slice */
	int clk_per_slice;

	/** @max_lanes : dsc max lanes supported for Fixed rate Link training */
	u8 max_lanes;

	/** @max_frl_rate_per_lane : maximum frl rate with DSC per lane */
	u8 max_frl_rate_per_lane;

	/** @total_chunk_kbytes: max size of chunks in KBs supported per line*/
	u8 total_chunk_kbytes;
};

/**
 * struct drm_hdmi_info - runtime information about the connected HDMI sink
 *
 * Describes if a given display supports advanced HDMI 2.0 features.
 * This information is available in CEA-861-F extension blocks (like HF-VSDB).
 */
struct drm_hdmi_info {
	struct drm_scdc scdc;

	/**
	 * @y420_vdb_modes: bitmap of modes which can support ycbcr420
	 * output only (not normal RGB/YCBCR444/422 outputs). There are total
	 * 107 VICs defined by CEA-861-F spec, so the size is 128 bits to map
	 * upto 128 VICs;
	 */
	unsigned long y420_vdb_modes[BITS_TO_LONGS(128)];

	/**
	 * @y420_cmdb_modes: bitmap of modes which can support ycbcr420
	 * output also, along with normal HDMI outputs. There are total 107
	 * VICs defined by CEA-861-F spec, so the size is 128 bits to map upto
	 * 128 VICs;
	 */
	unsigned long y420_cmdb_modes[BITS_TO_LONGS(128)];

	/** @y420_cmdb_map: bitmap of SVD index, to extraxt vcb modes */
	u64 y420_cmdb_map;

	/** @y420_dc_modes: bitmap of deep color support index */
	u8 y420_dc_modes;

	/** @max_frl_rate_per_lane: support fixed rate link */
	u8 max_frl_rate_per_lane;

	/** @max_lanes: supported by sink */
	u8 max_lanes;

	/* @add_func: support hdmi2.1 function */
	u8 add_func;

	/** @dsc_cap: DSC capabilities of the sink */
	struct drm_hdmi_dsc_cap dsc_cap;
};

enum subpixel_order {
	subpixelunknown = 0,
	subpixelhorizontalrgb,
	subpixelhorizontalbgr,
	subpixelverticalrgb,
	subpixelverticalbgr,
	subpixelnone,
};

#define DRM_COLOR_FORMAT_RGB444         BIT(0)
#define DRM_COLOR_FORMAT_YCRCB444       BIT(1)
#define DRM_COLOR_FORMAT_YCRCB422       BIT(2)
#define DRM_COLOR_FORMAT_YCRCB420       BIT(3)

/*
 * Describes a given display (e.g. CRT or flat panel) and its limitations.
 */
struct drm_display_info {
	char name[32];

	/* Physical size */
	unsigned int width_mm;
	unsigned int height_mm;

	/* Clock limits FIXME: storage format */
	unsigned int min_vfreq, max_vfreq;
	unsigned int min_hfreq, max_hfreq;
	unsigned int pixel_clock;
	unsigned int bpc;

	enum subpixel_order subpixel_order;
	u32 color_formats;

	const u32 *bus_formats;
	unsigned int num_bus_formats;

	/**
	 * @max_tmds_clock: Maximum TMDS clock rate supported by the
	 * sink in kHz. 0 means undefined.
	 */
	int max_tmds_clock;

	/**
	 * @dvi_dual: Dual-link DVI sink?
	 */
	bool dvi_dual;

	/* Mask of supported hdmi deep color modes */
	u8 edid_hdmi_dc_modes;

	u8 cea_rev;

	/**
	 * @hdmi: advance features of a HDMI sink.
	 */
	struct drm_hdmi_info hdmi;
};

struct edid {
	u8 header[8];
	/* Vendor & product info */
	u8 mfg_id[2];
	u8 prod_code[2];
	u32 serial; /* FIXME: byte order */
	u8 mfg_week;
	u8 mfg_year;
	/* EDID version */
	u8 version;
	u8 revision;
	/* Display info: */
	u8 input;
	u8 width_cm;
	u8 height_cm;
	u8 gamma;
	u8 features;
	/* Color characteristics */
	u8 red_green_lo;
	u8 black_white_lo;
	u8 red_x;
	u8 red_y;
	u8 green_x;
	u8 green_y;
	u8 blue_x;
	u8 blue_y;
	u8 white_x;
	u8 white_y;
	/* Est. timings and mfg rsvd timings*/
	struct est_timings established_timings;
	/* Standard timings 1-8*/
	struct std_timing standard_timings[8];
	/* Detailing timings 1-4 */
	struct detailed_timing detailed_timings[4];
	/* Number of 128 byte ext. blocks */
	u8 extensions;
	/* Checksum */
	u8 checksum;
} __packed;

enum base_output_format {
	DRM_HDMI_OUTPUT_DEFAULT_RGB, /* default RGB */
	DRM_HDMI_OUTPUT_YCBCR444, /* YCBCR 444 */
	DRM_HDMI_OUTPUT_YCBCR422, /* YCBCR 422 */
	DRM_HDMI_OUTPUT_YCBCR420, /* YCBCR 420 */
	/* (YCbCr444 > YCbCr422 > YCbCr420 > RGB) */
	DRM_HDMI_OUTPUT_YCBCR_HQ,
	/* (YCbCr420 > YCbCr422 > YCbCr444 > RGB) */
	DRM_HDMI_OUTPUT_YCBCR_LQ,
	DRM_HDMI_OUTPUT_INVALID, /* Guess what ? */
};

enum  base_output_depth {
	AUTOMATIC = 0,
	DEPTH_24BIT = 8,
	DEPTH_30BIT = 10,
};

struct base_bcsh_info {
	unsigned short brightness;
	unsigned short contrast;
	unsigned short saturation;
	unsigned short hue;
};

struct base_overscan {
	unsigned int maxvalue;
	unsigned short leftscale;
	unsigned short rightscale;
	unsigned short topscale;
	unsigned short bottomscale;
};

struct base_drm_display_mode {
	int clock;		/* in kHz */
	int hdisplay;
	int hsync_start;
	int hsync_end;
	int htotal;
	int vdisplay;
	int vsync_start;
	int vsync_end;
	int vtotal;
	int vrefresh;
	int vscan;
	unsigned int flags;
	int picture_aspect_ratio;
};

struct base_screen_info {
	int type;
	struct base_drm_display_mode mode;	/* 52 bytes */
	enum base_output_format  format;	/* 4 bytes */
	enum base_output_depth depth;		/* 4 bytes */
	unsigned int feature;			/* 4 bytes */
};

struct base_disp_info {
	struct base_screen_info screen_list[5];
	struct base_overscan scan;		/* 12 bytes */
};

struct base2_cubic_lut_data {
	u16 size;
	u16 lred[4913];
	u16 lgreen[4913];
	u16 lblue[4913];
};

struct base2_screen_info {
	u32 type;
	u32 id;
	struct base_drm_display_mode resolution;
	enum base_output_format  format;
	enum base_output_depth depthc;
	u32 feature;
};

struct base2_gamma_lut_data {
	u16 size;
	u16 lred[1024];
	u16 lgreen[1024];
	u16 lblue[1024];
};

struct framebuffer_info {
	u32 framebuffer_width;
	u32 framebuffer_height;
	u32 fps;
};

struct csc_info {
	u16 hue;
	u16 saturation;
	u16 contrast;
	u16 brightness;
	u16 r_gain;
	u16 g_gain;
	u16 b_gain;
	u16 r_offset;
	u16 g_offset;
	u16 b_offset;
	u16 csc_enable;
};


#define ACM_GAIN_LUT_HY_LENGTH		(9*17)
#define ACM_GAIN_LUT_HY_TOTAL_LENGTH	(ACM_GAIN_LUT_HY_LENGTH * 3)
#define ACM_GAIN_LUT_HS_LENGTH		(13*17)
#define ACM_GAIN_LUT_HS_TOTAL_LENGTH (ACM_GAIN_LUT_HS_LENGTH * 3)
#define ACM_DELTA_LUT_H_LENGTH		65
#define ACM_DELTA_LUT_H_TOTAL_LENGTH	(ACM_DELTA_LUT_H_LENGTH * 3)

struct acm_data {
	s16 delta_lut_h[ACM_DELTA_LUT_H_TOTAL_LENGTH];
	s16 gain_lut_hy[ACM_GAIN_LUT_HY_TOTAL_LENGTH];
	s16 gain_lut_hs[ACM_GAIN_LUT_HS_TOTAL_LENGTH];
	u16 y_gain;
	u16 h_gain;
	u16 s_gain;
	u16 acm_enable;
};

struct base2_disp_info {
	char disp_head_flag[6];
	struct base2_screen_info screen_info[4];
	struct base_bcsh_info bcsh_info;
	struct base_overscan overscan_info;
	struct base2_gamma_lut_data gamma_lut_data;
	struct base2_cubic_lut_data cubic_lut_data;
	struct framebuffer_info framebuffer_info;
	u32 cacm_header;
	u32 reserved[243];
	u32 crc;
	/* baseparameter version 3.0 add */
	struct csc_info csc_info;
	struct acm_data acm_data;
	u8 resv2[10*1024]; /* */
	u32 crc2;
	/* baseparameter version 3.0 add */
};

struct base2_disp_header {
	u32 connector_type;
	u32 connector_id;
	u32 offset;
};

struct base2_info {
	char head_flag[4];
	u16 major_version;
	u16 minor_version;
	struct base2_disp_header disp_header[8];
	struct base2_disp_info disp_info[8];
};

/**
 * Print the EDID info.
 *
 * @param edid_info	The EDID info to be printed
 */
void edid_print_info(struct edid1_info *edid_info);

/**
 * Check the EDID info.
 *
 * @param info  The EDID info to be checked
 * @return 0 on valid, or -1 on invalid
 */
int edid_check_info(struct edid1_info *info);

/**
 * Check checksum of a 128 bytes EDID data block
 *
 * @param edid_block	EDID block data
 *
 * @return 0 on success, or a negative errno on error
 */
int edid_check_checksum(u8 *edid_block);

/**
 * Get the horizontal and vertical rate ranges of the monitor.
 *
 * @param edid	The EDID info
 * @param hmin	Returns the minimum horizontal rate
 * @param hmax	Returns the maximum horizontal rate
 * @param vmin	Returns the minimum vertical rate
 * @param vmax	Returns the maximum vertical rate
 * @return 0 on success, or -1 on error
 */
int edid_get_ranges(struct edid1_info *edid, unsigned int *hmin,
		    unsigned int *hmax, unsigned int *vmin,
		    unsigned int *vmax);

struct drm_display_mode;
struct display_timing;

struct hdmi_edid_data {
	struct drm_display_mode *preferred_mode;
	int modes;
	struct drm_display_mode *mode_buf;
	struct drm_display_info display_info;
};

struct ddc_adapter {
	int (*ddc_xfer)(struct ddc_adapter *adap, struct i2c_msg *msgs,
			int num);
	struct udevice *i2c_bus;
	struct dm_i2c_ops *ops;
};

/**
 * edid_get_timing() - Get basic digital display parameters
 *
 * @param buf		Buffer containing EDID data
 * @param buf_size	Size of buffer in bytes
 * @param timing	Place to put preferring timing information
 * @param panel_bits_per_colourp	Place to put the number of bits per
 *			colour supported by the panel. This will be set to
 *			-1 if not available
 * @return 0 if timings are OK, -ve on error
 */
int edid_get_timing(u8 *buf, int buf_size, struct display_timing *timing,
		    int *panel_bits_per_colourp);
int edid_get_drm_mode(u8 *buf, int buf_size, struct drm_display_mode *mode,
		      int *panel_bits_per_colourp);
int drm_add_edid_modes(struct hdmi_edid_data *data, u8 *edid);
bool drm_detect_hdmi_monitor(struct edid *edid);
bool drm_detect_monitor_audio(struct edid *edid);
int do_cea_modes(struct hdmi_edid_data *data, const u8 *db, u8 len);
int drm_do_get_edid(struct ddc_adapter *adap, u8 *edid);
enum hdmi_quantization_range
drm_default_rgb_quant_range(struct drm_display_mode *mode);
u8 drm_scdc_readb(struct ddc_adapter *adap, u8 offset,
		  u8 *value);
u8 drm_scdc_writeb(struct ddc_adapter *adap, u8 offset,
		   u8 value);
void drm_mode_sort(struct hdmi_edid_data *edid_data);
int drm_mode_prune_invalid(struct hdmi_edid_data *edid_data);
void drm_rk_filter_whitelist(struct hdmi_edid_data *edid_data);
void drm_rk_select_mode(struct hdmi_edid_data *edid_data,
			struct base_screen_info *screen_info);

#endif /* __EDID_H_ */
