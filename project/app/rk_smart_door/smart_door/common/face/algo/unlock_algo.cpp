// Copyright 2022 Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <time.h>
#include <math.h>

#include <malloc.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/types.h>
#include <sys/time.h>

#include <rga/im2d.h>
#include <rga/im2d.hpp>
#include <rga/rga.h>

#include "face_unlock.hh"
#include "smart_unlock.hh"
#include "calib_data.h"
#include "message_mcu_v1.h"

#include "unlock_data_proc.h"
#include "unlock_algo.h"

//#define TIME_DEBUG

using namespace std;

#define MODEL_PATH           "/oem/usr/share/models/"
#define FACE_DATA_PATH       "/userdata/lib/"
#define DEPTH_CALIB_PATH     "/userdata/lib/"
#define DEPTH_INIT_DATA_PATH "/userdata/lib/"

#define ZIP_MODEL_MODE (1)

static int algo_width = 1080;
static int algo_height = 1280;

static int pic_width = 1080;
static int pic_height = 1280;

Smart_unlock *smart_unlock = NULL;

#if SC035_VGA_SENSOR
#define PRO_BINNING_MODE   (0)
#define IR_BINNING_MODE    (0)
#define DEPTH_BINNING_MODE (0)
#define RGA_TRANSPOSE      (1)
#define IR_ROTATE          (90)
#define PRO_ROTATE         (90)
#define IR_BLC             (32)
#define IR_SHIFT_BIT       (2)
#define SPECKLE_FACE_AE    (0)
#define EEPROM_UPDATE_REF  (1)

#elif SC132GS_SENSOR
#define PRO_BINNING_MODE   (0)
#define IR_BINNING_MODE    (0)
#define DEPTH_BINNING_MODE (1)
#define RGA_TRANSPOSE      (0)
#define IR_ROTATE          (90)
#define PRO_ROTATE         (90)
#define IR_BLC             (32)
#define IR_SHIFT_BIT       (2)
#define SPECKLE_FACE_AE    (0)
#define EEPROM_UPDATE_REF  (0)

#elif SC2355_SENSOR
#define FULL_SIZE_MODE     (0) //sync face_recognition.c

#if FULL_SIZE_MODE
#define PRO_BINNING_MODE   (1)
#define IR_BINNING_MODE    (1)
#define AE_BINNING_SIZE    (8)
#define IR_SHIFT_BIT       (0)
//for speckle ae mode
#define DEFAULT_EXP 10.0
#define MAX_EXP     12.0
#define MIN_EXP     2.0

#else
#define PRO_BINNING_MODE   (0)
#define IR_BINNING_MODE    (0)
#define AE_BINNING_SIZE    (4)
#define IR_SHIFT_BIT       (2)

//for speckle ae mode
#define DEFAULT_EXP 10.0
#define MAX_EXP     30.0
#define MIN_EXP     2.0

#endif

#define DEPTH_BINNING_MODE (0)
#define RGA_TRANSPOSE      (1)
#define IR_ROTATE          (270)
#define PRO_ROTATE         (90)
#define IR_BLC             (64)
#define SPECKLE_FACE_AE    (1)
#define EEPROM_UPDATE_REF  (1)

#else //SC2355_SENSOR end

#error "error sensor type"
#endif

#define ENABLE_JUDGE_FACE (0)
#define ENABLE_FAS_IR_0   (1)

#if SPECKLE_FACE_AE
static pthread_mutex_t mut;
static pthread_cond_t cond;
#endif

struct dma_buff_type {
	void *ptr;
	int fd;
	int size;
};

struct dma_buff_type dma_buf_input;
struct dma_buff_type detect_nn_dma;
struct dma_buff_type fas_ir_nn_dma;
struct dma_buff_type fas_depth_nn_dma;
struct dma_buff_type recog_nn_dma;
struct dma_buff_type head_pos_nn_dma;
struct dma_buff_type judge_face_nn_dma;
struct dma_buff_type fas_ir_0_nn_dma;
#if SPECKLE_FACE_AE
struct dma_buff_type dma_buf_rotate;
#endif

typedef unsigned long long __u64;
typedef unsigned int __u32;

struct dma_heap_allocation_data {
	__u64 len;
	__u32 fd;
	__u32 fd_flags;
	__u64 heap_flags;
};

struct dma_buf_sync {
	    __u64 flags;
};

struct rk_decom_param {
	__u32 mode;
	__u32 dst_max_size;
	__s32 src_fd;
	__s32 dst_fd;
	__u64 decom_data_len;
};

enum rk_decom_mod {
	RK_LZ4_MOD,
	RK_GZIP_MOD,
	RK_ZLIB_MOD,
	RK_DECOM_MOD_MAX,
};

#define CMA_HEAP_PATH   "/dev/rk_dma_heap/rk-dma-heap-cma"
#define DMA_HEAP_IOC_MAGIC      'R'
#define DMA_HEAP_IOCTL_ALLOC    _IOWR(DMA_HEAP_IOC_MAGIC, 0x0,\
		                      struct dma_heap_allocation_data)
#define DMA_BUF_SYNC_READ      (1 << 0)
#define DMA_BUF_SYNC_WRITE     (2 << 0)
#define DMA_BUF_SYNC_RW        (DMA_BUF_SYNC_READ | DMA_BUF_SYNC_WRITE)
#define DMA_BUF_SYNC_START     (0 << 2)
#define DMA_BUF_SYNC_END       (1 << 2)
#define DMA_BUF_BASE        'b'
#define DMA_BUF_IOCTL_SYNC  _IOW(DMA_BUF_BASE, 0, struct dma_buf_sync)

#define  RK_DECOM_MAGIC		'D'
#define  RK_DECOM_USER		_IOWR(RK_DECOM_MAGIC, 101, struct rk_decom_param)

int dma_buffer_malloc(int size, struct dma_buff_type *buff)
{
	int fd;
	int ret;
	void *ptr;
	struct dma_buf_sync sync = { 0 };
	int prot = PROT_READ | PROT_WRITE;

	struct dma_heap_allocation_data value;
	memset(&value, 0, sizeof(value));

	fd = open(CMA_HEAP_PATH, O_RDWR);
	if (fd < 0) {
		printf("open CMA_HEAP_PATH!\n");
		return fd;
	}

	value.fd = 0;
	value.len = size;
	value.fd_flags = O_CLOEXEC | O_RDWR;

	ret = ioctl(fd, DMA_HEAP_IOCTL_ALLOC, &value);
	if (ret < 0) {
		printf("DMA_HEAP_ALLOC_BUFFER failed\n");
		goto exit;
	}

	// printf("dmabuf fd = %d\n", value.fd);

	//if (fcntl(value.fd, F_GETFL) & O_RDWR)
	//prot |= PROT_WRITE;

	/* mmap contiguors buffer to user */
	ptr = mmap(NULL, size, prot, MAP_SHARED, value.fd, 0);
	if (ptr == MAP_FAILED) {
		printf("mmap failed: %s\n", strerror(errno));
		ret = -errno;
		goto exit;
	}

	buff->fd = value.fd;
	buff->ptr = ptr;
	buff->size = size;

	sync.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_RW;
	ret = ioctl(value.fd, DMA_BUF_IOCTL_SYNC, &sync);
	if (ret) {
		printf("sync start failed %d\n", errno);
		goto exit;
	}

	ret = 0;

exit:
	close(fd);
	return ret;
}

void dma_buffer_free(struct dma_buff_type *buff)
{
	if (buff->ptr && buff->fd) {
		munmap(buff->ptr, buff->size);
		close(buff->fd);
	} else {
		printf("%s error\n", __func__);
	}
}

static int rga_rotate(int src_fd, int srcWidth, int srcHeight,
		int dst_fd, int dstWidth, int dstHeight, int rotate_degree,
		int srcFormat, int dstFormat)
{
	int rotation = 0;

	if (rotate_degree == 90) {
		rotation = IM_HAL_TRANSFORM_ROT_90;
	} else if (rotate_degree == 180) {
		rotation = IM_HAL_TRANSFORM_ROT_180;
	} else if (rotate_degree == 270) {
		rotation = IM_HAL_TRANSFORM_ROT_270;
	}

	rga_buffer_t src, dst;
	src = wrapbuffer_handle(importbuffer_fd(src_fd, srcWidth,
				srcHeight, srcFormat), srcWidth, srcHeight, srcFormat);
	dst = wrapbuffer_handle(importbuffer_fd(dst_fd, dstWidth,
				dstHeight, dstFormat), dstWidth, dstHeight, dstFormat);

	imrotate(src, dst, rotation);

	releasebuffer_handle(src.handle);
	releasebuffer_handle(dst.handle);
	return 0;
}

static int rga_mirror(int src_fd, int srcWidth, int srcHeight,
		int dst_fd, int dstWidth, int dstHeight, int srcFormat, int dstFormat)
{
	rga_buffer_t src, dst;
	src = wrapbuffer_handle(importbuffer_fd(src_fd, srcWidth,
				srcHeight, srcFormat), srcWidth, srcHeight, srcFormat);
	dst = wrapbuffer_handle(importbuffer_fd(dst_fd, dstWidth,
				dstHeight, dstFormat), dstWidth, dstHeight, dstFormat);

	imflip(src, dst, IM_HAL_TRANSFORM_FLIP_H);

	releasebuffer_handle(src.handle);
	releasebuffer_handle(dst.handle);
	return 0;
}

static uint64_t rk_decom_buffer(uint32_t mode, int src_fd, uint32_t src_len,
				int dst_fd, uint32_t dst_max_size)
{
	int ret = -1;
	int fd_decom = -1;
	struct rk_decom_param decom_param;

	fd_decom = open("/dev/rk_decom", O_RDWR);
	if (fd_decom < 0) {
		printf("open rk_decom fail\n");
		return 0;
	}

	memset(&decom_param, 0x00, sizeof(decom_param));

	decom_param.mode         = mode;
	decom_param.src_fd       = src_fd;
	decom_param.dst_fd       = dst_fd;
	decom_param.dst_max_size = dst_max_size;

	ret =  ioctl(fd_decom, RK_DECOM_USER, &decom_param);
	if (ret)
		printf("rk_decom RK_DECOM_USER failed!\n");

	close(fd_decom);

	return ret ? 0 : decom_param.decom_data_len;
}

static int do_decom(const char *src_name, struct dma_buff_type *dst_dma, int *dst_len)
{
	int ret = 0;
	FILE *fp = NULL;
	const char *src_path = MODEL_PATH;
	char src_file[64];
	struct dma_buff_type src_dma;
	int src_len;

	if (!src_name || !dst_dma || !dst_len)
		return -1;

	memset(src_file, 0, sizeof(src_file));
	strcpy(src_file, src_path);
	strcat(src_file, src_name);
	if (!(fp = fopen(src_file, "rb"))) {
		printf("fopen %s failed!\n", src_file);
		ret = -1;
		goto exit;
	}

	fseek(fp, 0, SEEK_END);
	src_len = ftell(fp);
	rewind(fp);

	memset(&src_dma, 0, sizeof(struct dma_buff_type));
	ret = dma_buffer_malloc(src_len, &src_dma);
	if (ret) {
		printf("dma buffer malloc %d failed!\n", src_len);
		goto exit;
	}

	if (src_len != fread(src_dma.ptr, 1, src_len, fp)) {
		printf("fread %s failed!\n", src_file);
		ret = -1;
		goto exit;
	}

	fclose(fp);
	fp = NULL;

	*dst_len = rk_decom_buffer(RK_GZIP_MOD, src_dma.fd, src_dma.size,
				   dst_dma->fd, dst_dma->size);
	if (*dst_len == 0) {
		printf("%s decom failed!\n", src_file);
		ret = -1;
		goto exit;
	}

exit:
	if (fp) {
		fclose(fp);
		fp = NULL;
	}

	if (src_dma.ptr && src_dma.fd)
		dma_buffer_free(&src_dma);

	return ret;
}

static void decom_nn_deinit(nn_addr_t *nn)
{
	if (!nn)
		return;

	if (detect_nn_dma.ptr && detect_nn_dma.fd)
		dma_buffer_free(&detect_nn_dma);

	if (fas_ir_nn_dma.ptr && fas_ir_nn_dma.fd)
		dma_buffer_free(&fas_ir_nn_dma);

	if (fas_depth_nn_dma.ptr && fas_depth_nn_dma.fd)
		dma_buffer_free(&fas_depth_nn_dma);

	if (recog_nn_dma.ptr && recog_nn_dma.fd)
		dma_buffer_free(&recog_nn_dma);

	if (head_pos_nn_dma.ptr && head_pos_nn_dma.fd)
		dma_buffer_free(&head_pos_nn_dma);

#if ENABLE_JUDGE_FACE
	if (judge_face_nn_dma.ptr && judge_face_nn_dma.fd)
		dma_buffer_free(&judge_face_nn_dma);
#endif

#if ENABLE_FAS_IR_0
	if (fas_ir_0_nn_dma.ptr && fas_ir_0_nn_dma.fd)
		dma_buffer_free(&fas_ir_0_nn_dma);
#endif
}

static int decom_nn_init(nn_addr_t *nn)
{
	int ret = 0;
	const char *detect_nn_name = "detection.rknn.gz";
	const char *fas_ir_nn_name = "fas_ir.rknn.gz";
	const char *fas_depth_nn_name = "fas_depth.rknn.gz";
	const char *recog_nn_name = "recognition.rknn.gz";
	const char *head_pos_nn_name = "headpose.rknn.gz";
	const char *judge_face_nn_name = "judge_face.rknn.gz";
	const char *fas_ir_0_nn_name = "fas_ir_0.rknn.gz";
	// We do not know the real size. The reserved size is larger than it actually is.
	int detect_nn_max_len = 512 * 1024;
	int fas_ir_nn_max_len = 768 * 1024;
	int fas_depth_nn_max_len = 768 * 1024;
	int recog_nn_max_len = 3 * 512 * 1024;
	int head_pos_nn_max_len = 512 * 1024;
	int judge_face_nn_max_len = 512 * 1024;
	int fas_ir_0_nn_max_len = 512 * 1024;

	if (!nn)
		return -1;

	memset(&detect_nn_dma, 0, sizeof(struct dma_buff_type));
	ret = dma_buffer_malloc(detect_nn_max_len, &detect_nn_dma);
	if (ret) {
		printf("dma buffer malloc %d failed!\n", detect_nn_max_len);
		goto exit;
	}

	memset(&fas_ir_nn_dma, 0, sizeof(struct dma_buff_type));
	ret = dma_buffer_malloc(fas_ir_nn_max_len, &fas_ir_nn_dma);
	if (ret) {
		printf("dma buffer malloc %d failed!\n", fas_ir_nn_max_len);
		goto exit;
	}

	memset(&fas_depth_nn_dma, 0, sizeof(struct dma_buff_type));
	ret = dma_buffer_malloc(fas_depth_nn_max_len, &fas_depth_nn_dma);
	if (ret) {
		printf("dma buffer malloc %d failed!\n", fas_depth_nn_max_len);
		goto exit;
	}

	memset(&recog_nn_dma, 0, sizeof(struct dma_buff_type));
	ret = dma_buffer_malloc(recog_nn_max_len, &recog_nn_dma);
	if (ret) {
		printf("dma buffer malloc %d failed!\n", recog_nn_max_len);
		goto exit;
	}

	memset(&head_pos_nn_dma, 0, sizeof(struct dma_buff_type));
	ret = dma_buffer_malloc(head_pos_nn_max_len, &head_pos_nn_dma);
	if (ret) {
		printf("dma buffer malloc %d failed!\n", head_pos_nn_max_len);
		goto exit;
	}

	nn->detect_nn_addr = detect_nn_dma.ptr;
	nn->fas_ir_nn_addr = fas_ir_nn_dma.ptr;
	nn->fas_depth_nn_addr = fas_depth_nn_dma.ptr;
	nn->recog_nn_addr = recog_nn_dma.ptr;
	nn->headpose_nn_addr = head_pos_nn_dma.ptr;

	ret = do_decom(detect_nn_name, &detect_nn_dma, &nn->detect_nn_size);
	if (ret) {
		printf("decom %s failed!\n", detect_nn_name);
		goto exit;
	}

	ret = do_decom(fas_ir_nn_name, &fas_ir_nn_dma, &nn->fas_ir_nn_size);
	if (ret) {
		printf("decom %s failed!\n", fas_ir_nn_name);
		goto exit;
	}

	ret = do_decom(fas_depth_nn_name, &fas_depth_nn_dma, &nn->fas_depth_nn_size);
	if (ret) {
		printf("decom %s failed!\n", fas_depth_nn_name);
		goto exit;
	}

	ret = do_decom(recog_nn_name, &recog_nn_dma, &nn->recog_nn_size);
	if (ret) {
		printf("decom %s failed!\n", recog_nn_name);
		goto exit;
	}

	ret = do_decom(head_pos_nn_name, &head_pos_nn_dma,
			&nn->headpose_nn_size);
	if (ret) {
		printf("decom %s failed!\n", head_pos_nn_name);
		goto exit;
	}

#if ENABLE_JUDGE_FACE
	memset(&judge_face_nn_dma, 0, sizeof(struct dma_buff_type));
	ret = dma_buffer_malloc(judge_face_nn_max_len, &judge_face_nn_dma);
	if (ret) {
		printf("dma buffer malloc %d failed!\n", judge_face_nn_max_len);
		goto exit;
	}
	nn->judge_face_nn_addr = judge_face_nn_dma.ptr;

	ret = do_decom(judge_face_nn_name, &judge_face_nn_dma,
			&nn->judge_face_nn_size);
	if (ret) {
		printf("decom %s failed!\n", judge_face_nn_name);
		goto exit;
	}
#endif

#if ENABLE_FAS_IR_0
	memset(&fas_ir_0_nn_dma, 0, sizeof(struct dma_buff_type));
	ret = dma_buffer_malloc(fas_ir_0_nn_max_len, &fas_ir_0_nn_dma);
	if (ret) {
		printf("dma buffer malloc %d failed!\n", fas_ir_0_nn_max_len);
		goto exit;
	}
	nn->fas_ir_many_nn_addr[0] = fas_ir_0_nn_dma.ptr;

	ret = do_decom(fas_ir_0_nn_name, &fas_ir_0_nn_dma,
			&nn->fas_ir_many_nn_size[0]);
	if (ret) {
		printf("decom %s failed!\n", fas_ir_0_nn_name);
		goto exit;
	}
#endif

exit:
	if (ret)
		decom_nn_deinit(nn);

	return ret;
}

void unlock_algo_init(int width, int height)
{
	printf("%s w:%d h:%d\n", __func__, width, height);

#if SPECKLE_FACE_AE
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mut, NULL);
#endif

	int dp_w;
	int dp_h;
	nn_addr_t nn;

	pic_width  = width;
	pic_height = height;

#if RGA_TRANSPOSE
	algo_width = height;
	algo_height = width;
	dp_w = height;
	dp_h = width;
#else
	algo_width = width;
	algo_height = height;
	dp_w = width;
	dp_h = height;
#endif

#if IR_BINNING_MODE
	algo_width /= 2;
	algo_height /= 2;
#endif

#if PRO_BINNING_MODE
	dp_w /= 2;
	dp_h /= 2;
#endif

	printf("algo_width:%d algo_height:%d dp_w:%d dp_h:%d\n",
			algo_width, algo_height,
			dp_w, dp_h);

#if RGA_TRANSPOSE
	memset(&dma_buf_input, 0, sizeof(struct dma_buff_type));
	dma_buffer_malloc(pic_width * pic_height * 2, &dma_buf_input);

#if SPECKLE_FACE_AE
	memset(&dma_buf_rotate, 0, sizeof(struct dma_buff_type));
	dma_buffer_malloc(pic_width * pic_height * 2, &dma_buf_rotate);
#endif
#endif

	smart_unlock = new Smart_unlock();

#if DEPTH_BINNING_MODE
	smart_unlock->set_depth_ratio(2);
#endif

	/* 1. model path, permission R required
	 * 2. face data path, permission RW required
	 * 3. depth calib data path, permission RW required
	 * 4. depth init data path, permission RW required
	 */
	smart_unlock->set_path(MODEL_PATH, FACE_DATA_PATH,
			DEPTH_CALIB_PATH, DEPTH_INIT_DATA_PATH);

#if EEPROM_UPDATE_REF
	//update calib data
	eeprom_calib_data_check(DEPTH_CALIB_PATH, DEPTH_INIT_DATA_PATH);
#endif

#if ZIP_MODEL_MODE
	smart_unlock->unlock_param_init(algo_height, algo_width, algo_height,
			algo_width, dp_h, dp_w);

	if (decom_nn_init(&nn)) {
		printf("decom nn init failed!\n");
		return;
	}

#if SPECKLE_FACE_AE

#if RGA_TRANSPOSE
	int det_speckle_w = height / AE_BINNING_SIZE;
	int det_speckle_h = width / AE_BINNING_SIZE;
#else
	int det_speckle_w = width / AE_BINNING_SIZE;
	int det_speckle_h = height / AE_BINNING_SIZE;
#endif

	smart_unlock->unlock_face_speckle_ae_init(algo_height, algo_width, det_speckle_h, det_speckle_w, nn);

	pthread_cond_signal(&cond);
#else
	smart_unlock->unlock_face_ae_init(algo_height, algo_width,
			dp_h, dp_w, nn);
#endif

	smart_unlock->unlock_nn_init(nn);
#else
	smart_unlock->unlock_init(algo_height, algo_width,
			algo_height, algo_width, dp_h, dp_w);
#endif

	//smart_unlock->set_threshold(0.1, 0.1, 0.95);

#if ZIP_MODEL_MODE
	decom_nn_deinit(&nn);
#endif
	printf("%s end\n", __func__);
}

void unlock_algo_deinit()
{
	printf("%s\n", __func__);
	if (smart_unlock == NULL)
		return;

	smart_unlock->unlock_deinit();

	delete(smart_unlock);

	smart_unlock = NULL;

#if RGA_TRANSPOSE
	dma_buffer_free(&dma_buf_input);
#if SPECKLE_FACE_AE
	dma_buffer_free(&dma_buf_rotate);
#endif
#endif

#if SPECKLE_FACE_AE
	pthread_mutex_destroy(&mut);
	pthread_cond_destroy(&cond);
#endif
}

static int dump_algo_input(char *ir, char *pro)
{
	static int frame_cnt = 0;
	int dump_frame = 2;

	static FILE *fp = NULL;
	const char *tmp_file = "/tmp/input.raw";

	if (frame_cnt >= dump_frame)
		return 0;

	frame_cnt++;

	if (!fp) {
		fp = fopen(tmp_file, "w+");
		if (!fp) {
			printf("create %s failed.\n", tmp_file);
			return -1;
		}
	}

	int len = algo_width * algo_height * 2;
	fwrite(ir, len, 1, fp);
	fwrite(pro, len, 1, fp);

	if (frame_cnt >= dump_frame) {
		fclose(fp);
		printf("save %d frame raw data to %s success.\n",
				dump_frame, tmp_file);
	}

	return 0;
}

int dump_raw(void *input, int size, const char *name)
{
	FILE *fp = NULL;

	fp = fopen(name, "w+");
	if (!fp) {
		printf("create %s failed.\n", name);
		return -1;
	}

	fwrite(input, size, 1, fp);

	fclose(fp);
	printf("save raw data to %s success.\n", name);

	return 0;
}

static int import_algo_input(char *ir, char *pro)
{
	static int frame_cnt = 0;
	int dump_frame = 2;

	FILE *fp = NULL;
	const char *tmp_file = "/tmp/algo_input.raw";

	if (frame_cnt >= dump_frame)
		return -1;

	frame_cnt++;

	fp = fopen(tmp_file, "rb");
	if (!fp) {
		printf("open %s failed.\n", tmp_file);
		return -1;
	}

	int len = algo_width * algo_height * 2;
	fread(ir, len, 1, fp);
	fread(pro, len, 1, fp);

	fclose(fp);
	printf("read raw data to %s success.\n", tmp_file);

	return 0;
}

static int64_t getCurrentTimeUs()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}


face_use_e get_algo_state(int enroll_mode, int dir)
{
	face_use_e use_state;
	if (enroll_mode == MID_ENROLL_SINGLE)
		use_state = SAME_ID_REGISTER;
	else if (enroll_mode == MID_ENROLL) {
		switch (dir) {
			case FACE_DIRECTION_UP:
				use_state = REGISTER_DIR_UP;
				break;
			case FACE_DIRECTION_DOWN:
				use_state = REGISTER_DIR_DOWN;
				break;
#if (IR_ROTATE == 90)//use mirror
			case FACE_DIRECTION_LEFT:
				use_state = REGISTER_DIR_RIGHT;
				break;
			case FACE_DIRECTION_RIGHT:
				use_state = REGISTER_DIR_LEFT;
				break;
#else
			case FACE_DIRECTION_LEFT:
				use_state = REGISTER_DIR_LEFT;
				break;
			case FACE_DIRECTION_RIGHT:
				use_state = REGISTER_DIR_RIGHT;
				break;
#endif
			case FACE_DIRECTION_MIDDLE:
				use_state = REGISTER_DIR_MIDDLE;
				break;
			default:
				use_state = REGISTER;
				break;
		}
	} else {
		use_state = COMPARE;
	}
	return use_state;
}

int unlock_algo_process(
		short *raw_ir, short *raw_pro,
		int ir_fd, int pro_fd,
		int *face_id, uint8_t enroll_mode, uint8_t dir)
{
	printf("%s\n", __func__);

#ifdef TIME_DEBUG
	int64_t start_us = getCurrentTimeUs();
#endif

	if (smart_unlock == NULL)
		return 0;

#if 0
	int ret = import_algo_input((char *)raw_ir, (char *)raw_pro);
	if (ret)
		return ret;
	//dump_algo_input((char *)raw_ir, (char *)raw_pro);
#endif

	int w = pic_width;
	int h = pic_height;
	int binning_w = pic_width;
	int binning_h = pic_height;

#if RGA_TRANSPOSE
	binning_w = pic_height;
	binning_h = pic_width;
#endif

	detect_res_t res;

	face_use_e use_state = get_algo_state(enroll_mode, dir);

	uint8_t *resize_ir = NULL;
	short *resize_pro = NULL;

#if RGA_TRANSPOSE
	int srcFormat = RK_FORMAT_RGB_565;
	int dstFormat = RK_FORMAT_RGB_565;

	rga_rotate(ir_fd, w, h, dma_buf_input.fd, h, w, IR_ROTATE, srcFormat, dstFormat);
	rga_mirror(dma_buf_input.fd, h, w, ir_fd, h, w, srcFormat, dstFormat);
#endif

#if IR_BINNING_MODE
	resize_ir = (uint8_t *)malloc(sizeof(uint8_t) * binning_h/2 * binning_w/2);
	unlock_ir_binning(raw_ir, resize_ir, binning_w, binning_h, IR_BLC, IR_SHIFT_BIT);
#else
	resize_ir = (uint8_t *)malloc(sizeof(uint8_t) * h * w);
	unlock_raw16_to_raw8(raw_ir, resize_ir, w, h, IR_BLC);
#endif

#if RGA_TRANSPOSE
	rga_rotate(pro_fd, w, h, dma_buf_input.fd, h, w, PRO_ROTATE, srcFormat, dstFormat);
	rga_mirror(dma_buf_input.fd, h, w, pro_fd, h, w, srcFormat, dstFormat);
#endif

#if PRO_BINNING_MODE
	resize_pro = (short *)malloc(sizeof(short) * binning_h/2 * binning_w/2);
	unlock_pro_binning((unsigned short*)raw_pro,
			(unsigned short*)resize_pro, binning_w, binning_h);
	raw_pro = resize_pro;
#endif

	//dump_raw(resize_pro, h * w / 2, "/tmp/dump/pro.raw");
	//dump_raw(resize_ir, h * w / 2, "/tmp/dump/ir.raw");

	res = smart_unlock->face_unlock_flow(
			(char*)raw_ir, (char*)resize_ir, (char*)raw_pro, use_state);

	free(resize_ir);
	if (resize_pro)
		free(resize_pro);

	*face_id = res.face_id;

#ifdef TIME_DEBUG
	printf("%s Time = %.3fms\n", __func__,
			(getCurrentTimeUs() - start_us) / 1000.f);
#endif

	return (int)res.unlock_state;
}

int unlock_algo_yuv_process(
		char *raw_ir, char *raw_pro,
		int ir_fd, int pro_fd,
		int *face_id, uint8_t enroll_mode, uint8_t dir)
{
	printf("%s\n", __func__);

	if (smart_unlock == NULL)
		return 0;

	int w = algo_width;
	int h = algo_height;
	detect_res_t res;

	face_use_e use_state = get_algo_state(enroll_mode, dir);

#if RGA_TRANSPOSE
	int srcFormat = RK_FORMAT_YCbCr_420_SP;
	int dstFormat = RK_FORMAT_YCbCr_420_SP;
	rga_rotate(ir_fd, h, w, dma_buf_input.fd, w, h, IR_ROTATE, srcFormat, dstFormat);
	rga_mirror(dma_buf_input.fd, w, h, ir_fd, w, h, srcFormat, dstFormat);

	rga_rotate(pro_fd, h, w, dma_buf_input.fd, w, h, PRO_ROTATE, srcFormat, dstFormat);
	rga_mirror(dma_buf_input.fd, w, h, pro_fd, w, h, srcFormat, dstFormat);
#endif

	uint16_t * pro_16bit = (uint16_t *)malloc(sizeof(uint16_t) * h * w);
	for (int i = 0; i < h; i++){
		for (int j = 0; j < w; j++){
			int wh = i * w + j;
			pro_16bit[wh] = raw_pro[wh] << 2;
		}
	}

	res = smart_unlock->face_unlock_flow(
			(char*)raw_ir, (char*)raw_ir, (char*)pro_16bit, use_state);

	free(pro_16bit);

	*face_id = res.face_id;
	return (int)res.unlock_state;
}

void unlock_algo_clear_face(int id)
{
	printf("%s id:%d\n", __func__ , id);
	if (smart_unlock == NULL)
		return;

	smart_unlock->delete_face(id);
}

int read_test_val(const char *path)
{
	int fd_node;
	char str[10] = {0};
	int value = 0;

	fd_node = open(path, O_RDONLY);
	if (fd_node < 0) {
		return 0;
	}

	if (lseek(fd_node, 0, SEEK_SET) != -1) {
		read(fd_node, str, sizeof(str) - 1);
		value = atoi(str);
	}
	close(fd_node);

	return value;
}

#if SPECKLE_FACE_AE
static int face_xmin = 0;
static int face_ymin = 0;
static int face_xmax = 0;
static int face_ymax = 0;

int unlock_cal_pro_face_luma(uint8_t *src, int w, int h,
		int xmin, int ymin, int xmax, int ymax)
{
	int sum = 0;
	for (int i = ymin; i < ymax; i++)
		for (int j = xmin; j < xmax; j++)
			sum += src[i * w + j];

	sum /= (xmax - xmin) * (ymax - ymin);
	return sum;
}

int unlock_cal_raw_luma(uint16_t *src, int w, int h,
		int xmin, int ymin, int xmax, int ymax)
{
	uint32_t sum = 0;
	for (int i = ymin; i < ymax; i++)
		for (int j = xmin; j < xmax; j++)
			sum += src[i * w + j];

	sum /= (xmax - xmin) * (ymax - ymin);
	return sum;
}

void set_face_rect(int xmin, int ymin, int xmax, int ymax)
{
	int binning_size = AE_BINNING_SIZE;
	face_xmin = ymin * binning_size;
	face_ymin = xmin * binning_size;
	face_xmax = ymax * binning_size;
	face_ymax = xmax * binning_size;
	printf("set face rect:%d %d %d %d\n",
			face_xmin, face_ymin,
			face_xmax, face_ymax);
}

void get_face_rect(int *xmin, int *ymin, int *xmax, int *ymax)
{
	*xmin= face_xmin;
	*xmax= face_xmax;
	*ymin= face_ymin;
	*ymax= face_ymax;
	printf("get face rect:%d %d %d %d\n", *xmin, *ymin, *xmax, *ymax);
}

int unlock_cal_ir_pro_luma(unsigned short *flood,
		unsigned short *pro, int width, int height)
{
	int luma;
	int xmin, ymin, xmax, ymax;
	get_face_rect(&xmin, &ymin, &xmax, &ymax);
	luma = unlock_cal_raw_luma((unsigned short*)flood,
			width, height,
			xmin, ymin, xmax, ymax);

	printf("*** flood avg:%d\n", luma);

	luma = unlock_cal_raw_luma((unsigned short*)pro,
			width, height,
			xmin, ymin, xmax, ymax);

	printf("*** pro   avg:%d\n", luma);
	//dump_raw(pro_buff->buffer, width * height * 2, "/tmp/pro.raw");
	//dump_raw(flood_buff->buffer, width * height * 2, "/tmp/flood.raw");

	return 0;
}

float unlock_cal_target_exp(uint16_t *pro, int pro_fd, int width, int height)
{
	printf("%s w:%d h:%d\n", __func__, width, height);

	if (smart_unlock == NULL || smart_unlock->detect_init_flag == false) {
		printf("%s det init not ready\n", __func__);
		pthread_cond_wait(&cond, &mut);
		printf("%s wait finish\n", __func__);
		//return DEFAULT_EXP;
	}

	int binning_size = AE_BINNING_SIZE;
	int dst_w = width / binning_size;
	int dst_h = height / binning_size;

#if RGA_TRANSPOSE
	int srcFormat = RK_FORMAT_RGB_565;
	int dstFormat = RK_FORMAT_RGB_565;
	int w = width;
	int h = height;
	rga_rotate(pro_fd, w, h, dma_buf_input.fd, h, w, IR_ROTATE,
			srcFormat, dstFormat);
	rga_mirror(dma_buf_input.fd, h, w, dma_buf_rotate.fd,
			h, w, srcFormat, dstFormat);

	//update width height
	width = h;
	height = w;
	dst_w = width / binning_size;
	dst_h = height / binning_size;
	pro = (uint16_t*)dma_buf_rotate.ptr;
#endif

	uint8_t *pro_binning = (uint8_t*)malloc(dst_w * dst_h);

#ifdef TIME_DEBUG
	int64_t start_us = getCurrentTimeUs();
#endif

#if (AE_BINNING_SIZE == 8)
	unlock_pro_8x8_binning(pro, pro_binning, width, height);
#elif (AE_BINNING_SIZE == 4)
	unlock_pro_4x4_binning(pro, pro_binning, width, height);
#else
#error "not support"
#endif

#ifdef TIME_DEBUG
	printf("pro binning Time = %.3fms\n",
			(getCurrentTimeUs() - start_us) / 1000.f);
#endif

	//dump_raw(pro, width * height * 2, "/tmp/pro.raw");
	//dump_raw(pro_binning, dst_w * dst_h, "/tmp/pro_binning.raw");

	detect_res_t res = smart_unlock->face_unlock_speckle_det_face(
			(char*)pro_binning, dst_h, dst_w);

	int xmin = 0;
	int ymin = 0;
	int xmax = dst_w;
	int ymax = dst_h;
	int luma;
	if (res.unlock_state == RESULT_DETECT_FACE_OK) {
		xmin = res.best_box_rect.xmin;
		ymin = res.best_box_rect.ymin;
		xmax = res.best_box_rect.xmax;
		ymax = res.best_box_rect.ymax;
		printf("get speckle rect:%d %d %d %d\n", xmin, ymin, xmax, ymax);
	} else {
		xmin = dst_w / 3;
		xmax = dst_w / 3 * 2;
		ymin = dst_h / 2;
		ymax = dst_h;
		printf("speckle no face, use fix:%d %d %d %d\n",
				xmin, ymin, xmax, ymax);
	}
	set_face_rect(xmin, ymin, xmax, ymax);

#ifdef TIME_DEBUG
	start_us = getCurrentTimeUs();
#endif

	luma = unlock_cal_pro_face_luma(pro_binning, dst_w, dst_h, xmin,
			ymin, xmax, ymax);

#ifdef TIME_DEBUG
	printf("cal_pro_face_luma:%d Time = %.3fms\n", luma,
			(getCurrentTimeUs() - start_us) / 1000.f);
#endif

	free(pro_binning);

	int a_read = read_test_val("/tmp/a");
	int b_read = read_test_val("/tmp/b");
	int c_read = read_test_val("/tmp/c");

#if 0
	float a = 0.1063;
	float b = 22.615;

	if (a_read)
		a = a_read / 1000.0;
	if (b_read)
		b = b_read / 10.0;

	printf("target formula: a:%f b:%f\n", a, b);

	float target_exp = -a * luma + b;
#else
	float a = 0.0007;
	float b = -0.2939;
	float c = 32.996;

	if (a_read)
		a = a_read / 10000.0;
	if (b_read)
		b = b_read / -10000.0;
	if (c_read)
		c = c_read / 10000.0;

	//printf("target formula: a:%f b:%f c:%f\n", a, b, c);

	float target_exp = a * luma * luma + b * luma + c;
#endif

	if (target_exp > MAX_EXP)
		target_exp = MAX_EXP;
	if (target_exp < MIN_EXP)
		target_exp = MIN_EXP;

	return target_exp;
}
#endif //end SPECKLE_FACE_AE
