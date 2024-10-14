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
#include <linux/v4l2-subdev.h>
#include <linux/videodev2.h>
#include "param/rkisp32-config.h"

#include "v4l2_isp_subdev.h"

#define FMT_NUM_PLANES	1

#define ISP20HW_SUBM "isp subdev:"
#define LOGE_CAMHW_SUBM(sub_modules, arg, ...) printf(sub_modules arg, ##__VA_ARGS__); printf("\n")
#define LOGD_CAMHW_SUBM(sub_modules, arg, ...) printf(sub_modules arg, ##__VA_ARGS__); printf("\n")

static enum v4l2_buf_type buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
static enum v4l2_memory mem_type = V4L2_MEMORY_MMAP;

//rkisp-isp-subdev

typedef unsigned long int uintptr_t;
int _v4l2_isp_init(struct v4l2_contex *ctx, const char *video_node,
		 int width, int height, int format, int buf_cnt)
{
	int video_fd = -1;
	struct v4l2_ctx_buffer *tmp_buffers = NULL;
	struct v4l2_buffer buf;
	struct v4l2_plane planes[FMT_NUM_PLANES];
	int ret;

	video_fd = open(video_node, O_RDWR /* required */ /*| O_NONBLOCK*/, 0);
	if (video_fd < 0) {
		printf("Cannot open %s: %d, %s\n", video_node, errno, strerror(errno));
		goto error;
	}

	struct v4l2_subdev_format isp_sink_fmt;

	memset(&isp_sink_fmt, 0, sizeof(isp_sink_fmt));
	isp_sink_fmt.pad = 0;
	isp_sink_fmt.which = V4L2_SUBDEV_FORMAT_ACTIVE;
	ret = ioctl(video_fd, VIDIOC_SUBDEV_G_FMT, &isp_sink_fmt);
	if (ret) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "get mIspCoreDev fmt failed !\n");
		return ret;
	}

#if 0
	printf("ispsubdev get fmt: pad: %d, which: %d, width: %d, "
			"height: %d, format: 0x%x, field: %d, color space: %d\n",
			isp_sink_fmt.pad,
			isp_sink_fmt.which,
			isp_sink_fmt.format.width,
			isp_sink_fmt.format.height,
			isp_sink_fmt.format.code,
			isp_sink_fmt.format.field,
			isp_sink_fmt.format.colorspace);
#endif

	isp_sink_fmt.format.width = width;
	isp_sink_fmt.format.height = height;
	isp_sink_fmt.format.code = 0x200a; //sensor fmt

	ret = ioctl(video_fd, VIDIOC_SUBDEV_S_FMT, &isp_sink_fmt);
	if (ret) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "set mIspCoreDev fmt failed !\n");
		return ret;
	}

	// set selection, isp do the crop
	struct v4l2_subdev_selection aSelection;
	memset(&aSelection, 0, sizeof(aSelection));

	aSelection.which = V4L2_SUBDEV_FORMAT_ACTIVE;
	aSelection.pad = 0;
	aSelection.flags = 0;
	aSelection.target = V4L2_SEL_TGT_CROP;
	aSelection.r.width = width;
	aSelection.r.height = height;
	aSelection.r.left = 0;
	aSelection.r.top = 0;


	ret = ioctl(video_fd, VIDIOC_SUBDEV_S_SELECTION, &aSelection);
	if (ret < 0) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "subdev(%s) VIDIOC_S_SELECTION failed", video_node);
		return ret;
	}

	LOGD_CAMHW_SUBM(ISP20HW_SUBM, "isp sink crop info: %dx%d@%d,%d !",
			aSelection.r.width, aSelection.r.height,
			aSelection.r.left, aSelection.r.top);

	// set isp rkisp-isp-subdev src crop
	aSelection.pad = 2;
	aSelection.target = V4L2_SEL_TGT_CROP;
	aSelection.r.left = 0;
	aSelection.r.top = 0;
	aSelection.r.width = width;
	aSelection.r.height = height;
	ret = ioctl(video_fd, VIDIOC_SUBDEV_S_SELECTION, &aSelection);

	if (ret < 0) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "subdev(%s) VIDIOC_S_SELECTION failed", video_node);
		return ret;
	}
	LOGD_CAMHW_SUBM(ISP20HW_SUBM, "isp src crop info: %dx%d@%d,%d !",
			aSelection.r.width, aSelection.r.height,
			aSelection.r.left, aSelection.r.top);

	// set isp rkisp-isp-subdev src pad fmt
	struct v4l2_subdev_format isp_src_fmt;

	isp_src_fmt.which = V4L2_SUBDEV_FORMAT_ACTIVE;
	isp_src_fmt.pad = 2;
	ret = ioctl(video_fd, VIDIOC_SUBDEV_G_FMT, &isp_src_fmt);
	if (ret) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "get mIspCoreDev src fmt failed !\n");
		return ret;
	}

	isp_src_fmt.format.width = width;
	isp_src_fmt.format.height = height;
	ret = ioctl(video_fd, VIDIOC_SUBDEV_S_FMT, &isp_src_fmt);
	if (ret) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "set mIspCoreDev src fmt failed !\n");
		return ret;
	}

	LOGD_CAMHW_SUBM(ISP20HW_SUBM, "isp src fmt info: fmt 0x%x, %dx%d !",
			isp_src_fmt.format.code, isp_src_fmt.format.width, isp_src_fmt.format.height);


	ctx->video_fd   = video_fd;

	return 0;

error:
	printf("%s %d\n", __func__, __LINE__);
	if (video_fd >= 0)
		close(video_fd);

	ctx->video_fd = -1;

	return -1;
}

void _v4l2_isp_deinit(struct v4l2_contex *ctx)
{
	if (!ctx)
		return;

	if (ctx->buffers) {
		free(ctx->buffers);
	}

	if (ctx->video_fd >= 0)
		close(ctx->video_fd);

	memset(ctx, 0x00, sizeof(*ctx));
}

int v4l2_isp_init(struct v4l2_contex *ctx, const char *video_node, int width, int height)
{
	printf("%s:%s\n", __func__, video_node);
	int res = 0;
	int ret = -1;
	void *buffer;
	size_t length;
	int buf_cnt = 1;
	int format  = V4L2_PIX_FMT_NV12;

	ret = _v4l2_isp_init(ctx, video_node, width, height, format, buf_cnt);
	if (ret) {
		printf("v4l2_isp_init error! ret:%d\n", ret);
	}

	return ret;
}

void v4l2_isp_deinit(struct v4l2_contex *ctx)
{
	printf("%s\n", __func__);
	_v4l2_isp_deinit(ctx);
}

void v4l2_isp_trigger(struct v4l2_contex *ctx, int sequence)
{
	int ret;

	struct isp2x_csi_trigger tg = {
		.sof_timestamp = 0,
		.frame_timestamp = 0,
		.frame_id = sequence,
		.times = 0,
		.mode = T_START_X1,
	};

	ret = ioctl(ctx->video_fd, RKISP_CMD_TRIGGER_READ_BACK, &tg);
	if (ret)
		printf("%s ret:%d\n", __func__, ret);
}
