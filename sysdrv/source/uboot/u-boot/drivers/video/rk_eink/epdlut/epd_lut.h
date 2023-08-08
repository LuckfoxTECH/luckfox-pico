// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2020 Rockchip Electronics Co. Ltd.
 *
 * Author: Zorro Liu <zorro.liu@rock-chips.com>
 */

#ifndef EPD_LUT_H
#define EPD_LUT_H

#define LUT_SUCCESS	(0)
#define LUT_ERROR	(-1)

#define LUT_FROM_GPIO_SPI_FLASH		(0)
#define LUT_FROM_RK_SPI_FLASH		(1)
#define LUT_FROM_NAND_FLASH		(2)
#define LUT_FROM_WAVEFORM_FILE		(3)
#define LUT_FROM_MEM_RESERVED		(4)

enum epd_lut_type {
	WF_TYPE_RESET	= 1,
	WF_TYPE_GRAY16	= 2,
	WF_TYPE_GRAY4	= 3,
	WF_TYPE_GRAY2	= 4,
	WF_TYPE_AUTO	= 5,
	WF_TYPE_A2	= 6,
	WF_TYPE_GC16	= 7,
	WF_TYPE_GL16	= 8,
	WF_TYPE_GLR16	= 9,
	WF_TYPE_GLD16	= 10,
	WF_TYPE_GCC16	= 11,
	WF_TYPE_GRAY32	= 12,
	WF_TYPE_MAX	= 13,
};

enum waveform_type {
	RKF_WAVEFORM	= 1,
	PVI_WAVEFORM	= 2,
	OED_WAVEFORM	= 3,
};

enum pvi_wf_mode {
	PVI_WF_RESET	= 0,
	PVI_WF_DU	= 1,
	PVI_WF_DU4	= 2,
	PVI_WF_GC16	= 3,
	PVI_WF_GL16	= 4,
	PVI_WF_GLR16	= 5,
	PVI_WF_GLD16	= 6,
	PVI_WF_A2	= 7,
	PVI_WF_GCC16 = 8,
	PVI_WF_MAX,
};

enum oed_wf_mode {
	OED_WF_RESET	= 0,
	OED_WF_DU	= 1,
	OED_WF_GC16	= 2,
	OED_WF_GU16	= 3,
	OED_WF_A2	= 4,
	OED_WF_GL16	= 5,
	OED_WF_A2IN	= 6,
	OED_WF_A2OUT	= 7,
	OED_WF_MAX,
};

struct epd_lut_data {
	unsigned int frame_num;
	unsigned int *data;
};

struct epd_lut_ops {
	int (*lut_get)(struct epd_lut_data *lut, enum epd_lut_type type,
		       int temp);
};

/*
 * EPD LUT module export symbols
 */
int epd_lut_from_mem_init(void *waveform, struct epd_lut_ops *ops);
const char *epd_lut_get_wf_version(void);

/*
 * External functions
 */
int map_gray16_mode(void);
int map_auto_mode(void);

/*
 * PVI Waveform Interfaces
 */
int pvi_wf_input(void *waveform_file);
const char *pvi_wf_get_version(void);
int pvi_wf_get_lut(struct epd_lut_data *output, enum epd_lut_type lut_type,
		   int temperature);

/*
 * RKF Waveform Interfaces
 */
int rkf_wf_input(void *waveform_file);
const char *rkf_wf_get_version(void);
int rkf_wf_get_lut(struct epd_lut_data *output, enum epd_lut_type lut_type,
		   int temperature);
#endif

