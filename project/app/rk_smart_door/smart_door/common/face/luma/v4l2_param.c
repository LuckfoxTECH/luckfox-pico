#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <pthread.h>
#include <linux/videodev2.h>

#include "param/rkisp32-config.h"

#include "v4l2_param.h"

#if SC035_VGA_SENSOR
#define PARAM_LOAD_MODE (1)
#else
#define PARAM_LOAD_MODE (0)
#endif

#define FMT_NUM_PLANES	1
typedef unsigned long int uintptr_t;

static enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_META_OUTPUT;
static enum v4l2_memory mem_type = V4L2_MEMORY_MMAP;

int isp_param_print(void *userptr)
{
	struct isp32_isp_params_cfg *isp_cfg =
		(struct isp32_isp_params_cfg *)userptr;
	printf("bit enable:\n");
	for (int i = 0; i < 64; i++) {
		if (isp_cfg->module_en_update & (1 << i))
			printf("[%d]", i);
	}
	printf("\n");

	printf("frame_id:%d\n", isp_cfg->frame_id);

	return 0;
}

int isp_param_load_multi(void *userptr, int frame_id)
{
	struct isp32_isp_params_cfg *isp_cfg =
		(struct isp32_isp_params_cfg *)userptr;

	memset(isp_cfg, 0, sizeof(struct isp32_isp_params_cfg));

	FILE *fp = NULL;
	char name[128];
	snprintf(name, 128, "/userdata/lib/isp_param_f%d.bin", frame_id);
	fp = fopen(name, "rb");
	if (!fp) {
		printf("open %s failed.\n", name);
		return -1;
	}

	int len = sizeof(struct isp32_isp_params_cfg);

	fread((char *)isp_cfg, len, 1, fp);
	fclose(fp);

	printf("isp param:%s len:%d load success.\n", name, len);

	return 0;
}

int isp_param_load(void *userptr, int frame_id)
{
	struct isp32_isp_params_cfg *isp_cfg =
		(struct isp32_isp_params_cfg *)userptr;

	memset(isp_cfg, 0, sizeof(struct isp32_isp_params_cfg));

	FILE *fp = NULL;
	const char *name = "/userdata/lib/isp_param.bin";
	fp = fopen(name, "rb");
	if (!fp) {
		printf("open %s failed.\n", name);
		return -1;
	}

	int len = sizeof(struct isp32_isp_params_cfg);

	fread((char *)isp_cfg, len, 1, fp);
	fclose(fp);

	isp_cfg->frame_id = frame_id;
	printf("isp param:%s len:%d load success.\n", name, len);

	return 0;
}

int isp_param_fill(void *userptr, int width, int height, int blc)
{
	struct isp32_isp_params_cfg *isp_cfg =
		(struct isp32_isp_params_cfg *)userptr;

	memset(isp_cfg, 0, sizeof(struct isp32_isp_params_cfg));

	//blc
	isp_cfg->module_ens |= ISP3X_MODULE_BLS;
	isp_cfg->module_en_update |= ISP3X_MODULE_BLS;
	isp_cfg->module_cfg_update |= ISP3X_MODULE_BLS;

	isp_cfg->others.bls_cfg.enable_auto = 0;
	isp_cfg->others.bls_cfg.en_windows  = 0;

	isp_cfg->others.bls_cfg.bls_window1.h_offs = 0;
	isp_cfg->others.bls_cfg.bls_window1.v_offs = 0;
	isp_cfg->others.bls_cfg.bls_window1.h_size = 0;
	isp_cfg->others.bls_cfg.bls_window1.v_size = 0;

	isp_cfg->others.bls_cfg.bls_window2.h_offs = 0;
	isp_cfg->others.bls_cfg.bls_window2.v_offs = 0;
	isp_cfg->others.bls_cfg.bls_window2.h_size = 0;
	isp_cfg->others.bls_cfg.bls_window2.v_size = 0;

	isp_cfg->others.bls_cfg.bls_samples = 0;

	// blc0
	isp_cfg->others.bls_cfg.fixed_val.r  = blc << 2;//12bit
	isp_cfg->others.bls_cfg.fixed_val.gr = blc << 2;//12bit
	isp_cfg->others.bls_cfg.fixed_val.gb = blc << 2;//12bit
	isp_cfg->others.bls_cfg.fixed_val.b  = blc << 2;//12bit


	isp_cfg->others.bls_cfg.bls1_val.r  = 0;
	isp_cfg->others.bls_cfg.bls1_val.gr = 0;
	isp_cfg->others.bls_cfg.bls1_val.gb = 0;
	isp_cfg->others.bls_cfg.bls1_val.b  = 0;

	// TODO bls1 params
	isp_cfg->others.bls_cfg.bls1_en = 0;

	// blc_ob
	isp_cfg->others.bls_cfg.isp_ob_offset   = 0;
	isp_cfg->others.bls_cfg.isp_ob_predgain = 0;
	isp_cfg->others.bls_cfg.isp_ob_max      = 0xfffff;

	//AE
	isp_cfg->module_ens |= 1LL << RK_ISP2X_RAWAE1_ID;
	isp_cfg->module_en_update |= 1LL << RK_ISP2X_RAWAE1_ID;
	isp_cfg->module_cfg_update |= 1LL << RK_ISP2X_RAWAE1_ID;

	isp_cfg->meas.rawae1.rawae_sel = 1;
	isp_cfg->meas.rawae1.wnd_num = 2;
	isp_cfg->meas.rawae1.win.h_offs = 0;
	isp_cfg->meas.rawae1.win.v_offs = 0;
	isp_cfg->meas.rawae1.win.h_size = width; //set according to raw_info
	isp_cfg->meas.rawae1.win.v_size = height; // set according to raw_info

	isp_cfg->meas.rawae1.subwin_en[0] = 0;
	isp_cfg->meas.rawae1.subwin_en[1] = 0;
	isp_cfg->meas.rawae1.subwin_en[2] = 0;
	isp_cfg->meas.rawae1.subwin_en[3] = 0;

	return 0;
}

int isp_param_fill_AE(void *userptr, int width, int height, int blc)
{
	struct isp32_isp_params_cfg *isp_cfg =
		(struct isp32_isp_params_cfg *)userptr;

	//AE
	isp_cfg->module_ens |= 1LL << RK_ISP2X_RAWAE1_ID;
	isp_cfg->module_en_update |= 1LL << RK_ISP2X_RAWAE1_ID;
	isp_cfg->module_cfg_update |= 1LL << RK_ISP2X_RAWAE1_ID;

	isp_cfg->meas.rawae1.rawae_sel = 1;
	isp_cfg->meas.rawae1.wnd_num = 2;
	isp_cfg->meas.rawae1.win.h_offs = 0;
	isp_cfg->meas.rawae1.win.v_offs = 0;
	isp_cfg->meas.rawae1.win.h_size = width; //set according to raw_info
	isp_cfg->meas.rawae1.win.v_size = height; // set according to raw_info

	isp_cfg->meas.rawae1.subwin_en[0] = 0;
	isp_cfg->meas.rawae1.subwin_en[1] = 0;
	isp_cfg->meas.rawae1.subwin_en[2] = 0;
	isp_cfg->meas.rawae1.subwin_en[3] = 0;

	return 0;
}

int _v4l2_param_init(struct v4l2_contex *ctx, const char *video_node,
		 int width, int height, int format, int buf_cnt, int blc)
{
	int video_fd = -1;
	struct v4l2_ctx_buffer *tmp_buffers = NULL;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];

	video_fd = open(video_node, O_RDWR | O_CLOEXEC, 0);
	if (video_fd < 0) {
		printf("Cannot open %s: %d, %s\n", video_node, errno, strerror(errno));
		goto error;
	}

	struct v4l2_capability cap;
	//memset(ctx, 0x00, sizeof(*ctx));
	/* get */
	if (ioctl(video_fd, VIDIOC_QUERYCAP, &cap) == -1) {
		printf("VIDIOC_QUERYCAP fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)
		buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	else if (cap.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)
		buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	else if (cap.capabilities & V4L2_CAP_VIDEO_OUTPUT)
		buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT;
	else if (cap.capabilities & V4L2_CAP_VIDEO_OUTPUT_MPLANE)
		buf_type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	else if (cap.capabilities & V4L2_CAP_META_CAPTURE)
		buf_type = V4L2_BUF_TYPE_META_CAPTURE;
	else if (cap.capabilities & V4L2_CAP_META_OUTPUT)
		buf_type = V4L2_BUF_TYPE_META_OUTPUT;
	else {
		printf("@%s: unsupported buffer type.", __FUNCTION__);
		return -1;
	}

#if 0
	printf("------------------------------\n");
	printf("driver:       '%s'\n", cap.driver);
	printf("card:         '%s'\n", cap.card);
	printf("bus_info:     '%s'\n", cap.bus_info);
	printf("version:      %x\n", cap.version);
	printf("capabilities: %x\n", cap.capabilities);
	printf("device caps:  %x\n", cap.device_caps);
	printf("buffer type   %d\n", buf_type);
	printf("------------------------------\n");
#endif

	/* request v4l2_ctx_buffer */
	struct v4l2_requestbuffers req;

	memset(&req, 0, sizeof(req));
	req.count  = buf_cnt;
	req.type   = buf_type;
	req.memory = mem_type;

	if (ioctl(video_fd, VIDIOC_REQBUFS, &req) == -1) {
		printf("VIDIOC_REQBUFS fail: %d, %s\n", errno, strerror(errno));
		goto error;
	}

	/* alloc buffer */
	tmp_buffers = (struct v4l2_ctx_buffer *)calloc(req.count,
			sizeof(struct v4l2_ctx_buffer));
	if (!tmp_buffers) {
		printf("alloc v4l2_ctx_buffer error\n");
		goto error;
	}

	for (int numBufs = 0; numBufs < req.count; numBufs++) {
		memset(&buf, 0, sizeof(buf));
		buf.type   = buf_type;
		buf.memory = mem_type;
		buf.index  = numBufs;

		// query ir_buffer
		if (ioctl(video_fd, VIDIOC_QUERYBUF, &buf) == -1) {
			printf("VIDIOC_QUERYBUF fail\n");
			goto error;
		}

		void *pointer = MAP_FAILED;
		pointer = mmap (0, buf.length,
				PROT_READ | PROT_WRITE, MAP_SHARED, video_fd, buf.m.offset);
		if (pointer == MAP_FAILED) {
			printf("userptr == MAP_FAILED\n");
			goto error;
		}

		buf.m.userptr = (uintptr_t)pointer;

		// export buf dma fd
		struct v4l2_exportbuffer expbuf;
		memset(&expbuf, 0, sizeof(expbuf));
		expbuf.type = buf_type;
		expbuf.index = numBufs;
		expbuf.flags = O_CLOEXEC;

		if (ioctl(video_fd, VIDIOC_EXPBUF, &expbuf) < 0) {
			printf("get dma buf failed\n");
			goto error;
		}

		tmp_buffers[numBufs].export_fd = expbuf.fd;
		tmp_buffers[numBufs].userptr = buf.m.userptr;
		tmp_buffers[numBufs].length = buf.length;

#if PARAM_LOAD_MODE
		isp_param_load(pointer, numBufs);

		isp_param_fill_AE(pointer, width, height, blc);

		//isp_param_print(pointer);
#else
		isp_param_fill(pointer, width, height, blc);
#endif

		// qbuffer
		if (ioctl(video_fd, VIDIOC_QBUF, &buf) == -1) {
			printf("VIDIOC_QBUF fail\n");
			goto error;
		}
	}

	ctx->video_fd   = video_fd;
	ctx->planes     = FMT_NUM_PLANES;
	ctx->buffers    = tmp_buffers;
	ctx->buffer_cnt = buf_cnt;

	return 0;

error:
	if (tmp_buffers)
		free(tmp_buffers);

	if (video_fd >= 0)
		close(video_fd);

	ctx->video_fd   = -1;
	ctx->buffers    = NULL;
	ctx->buffer_cnt = 0;

	return -1;
}

void _v4l2_param_deinit(struct v4l2_contex *ctx)
{
	if (!ctx || !ctx->buffers)
		return;

	for (int i = 0; i < ctx->buffer_cnt; i++) {
		munmap((void*)ctx->buffers[i].userptr, ctx->buffers[i].length);
		if (ctx->buffers[i].export_fd)
			close(ctx->buffers[i].export_fd);
	}

	if (ctx->buffers) {
		free(ctx->buffers);
	}

	if (ctx->video_fd >= 0)
		close(ctx->video_fd);

	memset(ctx, 0x00, sizeof(*ctx));
}

int v4l2_param_stream_on(struct v4l2_contex *ctx)
{
	return ioctl(ctx->video_fd, VIDIOC_STREAMON, &buf_type);
}

void v4l2_param_stream_off(struct v4l2_contex *ctx)
{
	ioctl(ctx->video_fd, VIDIOC_STREAMOFF, &buf_type);
}

int v4l2_param_dequeue(struct v4l2_contex *ctx)
{
	int ret;
	struct v4l2_buffer buf;

	if (!ctx)
		return -1;

	memset(&buf, 0, sizeof(buf));

	buf.type     = buf_type;
	buf.memory   = mem_type;

	ret = ioctl(ctx->video_fd, VIDIOC_DQBUF, &buf);
	if (ret)
		return -1;

	printf("%s index:%d userptr:%d lenght:%d\n",
			__func__, buf.index, buf.m.userptr, buf.length);

	return 0;
}

int v4l2_param_enqueue(struct v4l2_contex *ctx, int fd)
{
	int ret;
	struct v4l2_buffer buf;

	if (!ctx || !ctx->buffers)
		return -1;

	memset(&buf, 0, sizeof(buf));

	buf.type     = buf_type;
	buf.memory   = mem_type;
	buf.index    = 0;
	buf.length   = ctx->buffers[buf.index].length;
	buf.bytesused = buf.length;

	ret = ioctl(ctx->video_fd, VIDIOC_QBUF, &buf);

	printf("%s index:%d userptr:%d lenght:%d\n",
			__func__, buf.index, buf.m.userptr, buf.length);

	return ret;
}

int v4l2_param_init(struct v4l2_contex *ctx,
		const char *video_node, int width, int height)
{
	printf("%s:%s\n", __func__, video_node);

	int ret = -1;
	int buf_cnt = 2;
	int format  = V4L2_PIX_FMT_Y10;//V4L2_PIX_FMT_SRGGB10;
#if SC2355_SENSOR
	int blc = 64;
#else
	int blc = 32;
#endif

	ret = _v4l2_param_init(ctx, video_node,
			width, height, format, buf_cnt, blc);
	if (ret) {
		printf("_v4l2_param_init error! ret:%d\n", ret);
		return ret;
	}

	ret = v4l2_param_stream_on(ctx);
	if (ret) {
		printf("v4l2_param_stream_on error!\n");
	}

	return ret;
}

void v4l2_param_deinit(struct v4l2_contex *ctx)
{
	v4l2_param_stream_off(ctx);

	_v4l2_param_deinit(ctx);

	printf("%s, exit.\n", __func__);
}
