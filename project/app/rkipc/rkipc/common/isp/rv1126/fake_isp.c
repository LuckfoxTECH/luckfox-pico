// Copyright 2021 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <assert.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "fake_isp.h"

#define AIQ_WIDTH 2688
#define AIQ_HEIGHT 1520

#define DUMP_RAW 0

static int xioctl(int fh, int request, void *arg) {
	int r;
	do {
		r = ioctl(fh, request, arg);
	} while (-1 == r && EINTR == errno);
	return r;
}

rk_aiq_sys_ctx_t *aiq_fake_init(rk_aiq_working_mode_t mode, const char *iq_dir,
                                rk_aiq_raw_prop_t prop, void (*callback)(void *)) {
	rk_aiq_sys_ctx_t *ctx = NULL;
	printf("sensor_name is FakeCamera\n");
	ctx = rk_aiq_uapi_sysctl_init("FakeCamera", iq_dir, NULL, NULL);
	if (!ctx) {
		printf("%s: rk_aiq_uapi_sysctl_init fail!\n", __func__);
		return NULL;
	}
	rk_aiq_uapi_sysctl_prepareRkRaw(ctx, prop);
	if (rk_aiq_uapi_sysctl_prepare(ctx, AIQ_WIDTH, AIQ_HEIGHT, mode)) {
		printf("%s: rk_aiq_uapi_sysctl_prepare fail!\n", __func__);
		rk_aiq_uapi_sysctl_deinit(ctx);
		return NULL;
	}
	rk_aiq_uapi_sysctl_registRkRawCb(ctx, callback);
	if (rk_aiq_uapi_sysctl_start(ctx) < 0) {
		printf("%s: rk_aiq_uapi_sysctl_start fail!\n", __func__);
		rk_aiq_uapi_sysctl_deinit(ctx);
		return NULL;
	}

	ahdr_attrib_t hdr_attr;
	rk_aiq_user_api_ahdr_GetAttrib(ctx, &hdr_attr);
	hdr_attr.opMode = HDR_OpMode_Tool;
	for (int i = 0; i < CALIBDB_MAX_MODE_NUM; i++)
		hdr_attr.stTool.tmo.en[i].en = 0;
	rk_aiq_user_api_ahdr_SetAttrib(ctx, hdr_attr);

	return ctx;
}

void aiq_fake_exit(rk_aiq_sys_ctx_t *ctx) {
	if (ctx) {
		rk_aiq_uapi_sysctl_stop(ctx, false);
		rk_aiq_uapi_sysctl_deinit(ctx);
		ctx = NULL;
	}
}

bool parse_mcu_rkraws(const char *subdev, struct mcu_rkaiq_rkraw *mcu_rkraws) {
	struct rkisp_thunderboot_resmem resmem;
	struct rkisp_thunderboot_shmem _shmem;

	int mfd = open("/dev/mem", O_RDWR | O_SYNC);
	if (-1 == mfd) {
		printf("%s: open /dev/mem failed\n", __FUNCTION__);
		assert(0);
	}
	int isp_dev = open(subdev, O_RDWR | O_SYNC);
	if (-1 == isp_dev) {
		printf("%s: open %s\n", __FUNCTION__, subdev);
		assert(0);
	}
	if (0 > xioctl(isp_dev, RKISP_CMD_GET_SHARED_BUF, &resmem)) {
		printf("%s: get isp shared buf failed\n", __FUNCTION__);
		return false;
	}

	if ((0 == resmem.resmem_padr) || (0 == resmem.resmem_size)) {
		printf("%s: isp shared buf is incorrect\n", __FUNCTION__);
		return false;
	}
	printf("%s: resmem.resmem_padr 0x%x, size:0x%x\n", __FUNCTION__, resmem.resmem_padr,
	       resmem.resmem_size);
	uint8_t *mem_vir_addr =
	    (uint8_t *)mmap(NULL, resmem.resmem_size, PROT_WRITE, MAP_SHARED, mfd, resmem.resmem_padr);
	if (NULL == mem_vir_addr) {
		printf("%s: mmap failed\n", __FUNCTION__);
		return false;
	}
	printf("mem_vir_addr=0x%x\n", (unsigned int)mem_vir_addr);

	unsigned short head1;
	uint8_t *p = (uint8_t *)mem_vir_addr;
	p = p + sizeof(struct rkisp_fastboot_resmem_head);
	int icnt = 0;
	while (icnt < MAX_FAKE_FRAMES_NUM) {
		head1 = *((unsigned short *)p);
		printf(">>>>>>>>>>>>>>header=0x%x\n", head1);
		switch (head1) {
		case 0xff00:
			mcu_rkraws[icnt].frame_base = p;
			p = p + 2;
			break;
		case 0xff01: {
			struct _raw_data *raw_data_ptr = NULL;
			raw_data_ptr = (struct _raw_data *)p;
			mcu_rkraws[icnt].rawdata = *raw_data_ptr;
			printf("==0x%x, 0x%x, 0x%x, 0x%x,0x%x\n", raw_data_ptr->flag, raw_data_ptr->size,
			       raw_data_ptr->ddr_laddr, raw_data_ptr->ddr_haddr, raw_data_ptr->raw_size);
			_shmem.shm_start = mcu_rkraws[icnt].rawdata.ddr_laddr;
			_shmem.shm_size = mcu_rkraws[icnt].rawdata.raw_size;
			printf(">>>shm_size=%d\n", _shmem.shm_size);
			if (0 > xioctl(isp_dev, RKISP_CMD_GET_SHM_BUFFD, &_shmem)) {
				printf("%s: get shared buf fd failed\n", __FUNCTION__);
				return false;
			}
			printf(">>get buf fd=%d\n", _shmem.shm_fd);
#if DUMP_RAW // just for sc210iot
			{
				uint8_t buffer[128] = {0};
				char raw_path[64];
				uint8_t *raw_ptr = (uint8_t *)mem_vir_addr;
				FILE *file = NULL;
				sprintf(raw_path, "/tmp/test_mcu_raw_%d.raw", icnt);
				file = fopen(raw_path, "w");
				if (file) {
					*((uint16_t *)buffer) = 0x8080;
					*((uint16_t *)(buffer + 2)) = 128U;
					*((uint32_t *)(buffer + 4)) = 0x0171;
					*((uint16_t *)(buffer + 8)) = 1920;
					*((uint16_t *)(buffer + 10)) = 1080;
					*(buffer + 12) = 10;
					*(buffer + 13) = 0;
					*(buffer + 14) = 1;
					*(buffer + 15) = 0;
					*(buffer + 16) = 1;
					*((uint16_t *)(buffer + 17)) = 2560;
					*((uint16_t *)(buffer + 19)) = 2400;
					fwrite(buffer, sizeof(buffer), 1, file);
					printf(">>>shm offset=%x\n", (_shmem.shm_start - resmem.resmem_padr));
					raw_ptr = (uint8_t *)(raw_ptr + (_shmem.shm_start - resmem.resmem_padr));
					fwrite((uint8_t *)raw_ptr, 1, 2560 * 1080, file);
					fclose(file);
				}
			}
#endif
			mcu_rkraws[icnt].raw_buf_fd = _shmem.shm_fd;
			p = p + sizeof(struct _raw_data);
			break;
		}
		case 0xff02:
			mcu_rkraws[icnt].rawfmt = *((struct rkipc_raw_format *)p);
			p = p + sizeof(struct rkipc_raw_format);
			break;
		case 0xff03:
			mcu_rkraws[icnt].finfo = *((struct _frame_inf *)p);
			p = p + sizeof(struct _frame_inf);
			break;
		case 0X00ff:
			p = p + 2;
			icnt++;
			break;
		}
	}
	munmap(mem_vir_addr, resmem.resmem_size);
	close(isp_dev);
	close(mfd);
	return true;
}

bool free_rkisp_reserve_mem(const char *subdev) {

	int isp_dev = open(subdev, O_RDWR | O_SYNC);
	if (-1 == isp_dev) {
		printf("%s: open %s\n", __FUNCTION__, subdev);
		assert(0);
	}
	if (0 > xioctl(isp_dev, RKISP_CMD_FREE_SHARED_BUF, NULL)) {
		printf("%s: get isp shared buf failed\n", __FUNCTION__);
		return false;
	}

	close(isp_dev);
	return true;
}

void alloc_rkraws(uint8_t **rkraws) {
	for (int i = 0; i < MAX_FAKE_FRAMES_NUM; i++) {
		rkraws[i] = malloc(2048);
	}
}

void free_rkraws(uint8_t **rkraws) {
	for (int i = 0; i < MAX_FAKE_FRAMES_NUM; i++) {
		if (rkraws[i])
			free(rkraws[i]);
	}
}

void make_rkraws(struct mcu_rkaiq_rkraw *mcu_rkraws, uint8_t **rkraws) {
	struct _block_header {
		unsigned short block_id;
		unsigned int block_length;
	} __attribute__((packed));

	struct _st_addrinfo {
		uint32_t fd;
		uint32_t haddr;
		uint32_t laddr;
		uint32_t size;
	} __attribute__((packed));

	struct rkipc_raw_format {
		uint16_t tag;
		uint32_t size;
		uint16_t vesrion;
		char sensor[32];
		char scene[32];
		uint32_t frame_id;
		uint16_t width;
		uint16_t height;
		uint8_t bit_width;
		uint8_t bayer_fmt;
		uint8_t hdr_mode;
		uint8_t buf_type;
		uint8_t byte_order;
	} __attribute__((packed));

	struct _frame_info {
		uint16_t tag;
		uint32_t size;
		uint16_t vesrion;
		uint32_t frame_id;

		float normal_exp;
		float normal_gain;
		uint32_t normal_exp_reg;
		uint32_t normal_gain_reg;

		float hdr_exp_l;
		float hdr_gain_l;
		uint32_t hdr_exp_l_reg;
		uint32_t hdr_gain_l_reg;

		float hdr_exp_m;
		float hdr_gain_m;
		uint32_t hdr_gain_m_reg;
		uint32_t hdr_exp_m_reg;

		float hdr_exp_s;
		float hdr_gain_s;
		uint32_t hdr_exp_s_reg;
		uint32_t hdr_gain_s_reg;

		float awg_rgain;
		float awg_bgain;
	} __attribute__((packed));

	struct _block_header header;
	struct _st_addrinfo addrinfo;
	struct rkipc_raw_format format;
	struct _frame_info frame_info;
	uint16_t tag;
	for (int i = 0; i < MAX_FAKE_FRAMES_NUM; i++) {
		uint8_t *ptr = rkraws[i];
		tag = 0xff00;
		memcpy(ptr, &tag, 2);
		ptr += 2;

		format.frame_id = i;
		format.hdr_mode = 1;
		format.size = sizeof(format) - sizeof(header);
		format.tag = 0xff01;
		format.vesrion = mcu_rkraws[i].rawfmt.vesrion;
		memcpy(format.sensor, mcu_rkraws[i].rawfmt.sensor, 32);
		memcpy(format.scene, mcu_rkraws[i].rawfmt.scene, 32);
		format.frame_id = mcu_rkraws[i].rawfmt.frame_no;
		format.width = mcu_rkraws[i].rawfmt.width;
		format.height = mcu_rkraws[i].rawfmt.height;

		format.byte_order = 1;

		memcpy(ptr, &format, sizeof(format));
		ptr += sizeof(format);

		header.block_id = 0xff02;
		header.block_length = sizeof(addrinfo);
		memset(&addrinfo, 0, sizeof(addrinfo));
		addrinfo.fd = mcu_rkraws[i].raw_buf_fd;
		addrinfo.size = mcu_rkraws[i].rawdata.raw_size;
		memcpy(ptr, &header, sizeof(header));
		ptr += sizeof(header);
		memcpy(ptr, &addrinfo, sizeof(addrinfo));
		ptr += sizeof(addrinfo);

		frame_info.frame_id = i;
		// WARNING: although mcu_rkraws[i].finfo.normal_gain/exp are float, but they are actually
		// integer reg_value
		frame_info.normal_gain_reg = (uint32_t)(mcu_rkraws[i].finfo.normal_gain);
		frame_info.normal_exp_reg = (uint32_t)(mcu_rkraws[i].finfo.normal_exp);
		frame_info.size = sizeof(frame_info) - sizeof(header);
		frame_info.tag = 0xff06;
		memcpy(ptr, &frame_info, sizeof(frame_info));
		ptr += sizeof(frame_info);
		tag = 0x00ff;
		memcpy(ptr, &tag, 2);
	}
}
