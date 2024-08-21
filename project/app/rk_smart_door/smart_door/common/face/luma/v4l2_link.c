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

#include "mediactl/mediactl.h"

#include "v4l2subdev.h"
#include "v4l2_link.h"

typedef enum {
	XCAM_RETURN_NO_ERROR        = 0,
	XCAM_RETURN_BYPASS          = 1,

	/* errors */
	XCAM_RETURN_ERROR_FAILED    = -1,
	XCAM_RETURN_ERROR_PARAM     = -2,
	XCAM_RETURN_ERROR_MEM       = -3,
	XCAM_RETURN_ERROR_FILE      = -4,
	XCAM_RETURN_ERROR_ANALYZER  = -5,
	XCAM_RETURN_ERROR_ISP       = -6,
	XCAM_RETURN_ERROR_SENSOR    = -7,
	XCAM_RETURN_ERROR_THREAD    = -8,
	XCAM_RETURN_ERROR_IOCTL     = -9,
	XCAM_RETURN_ERROR_ORDER     = -10,
	XCAM_RETURN_ERROR_TIMEOUT   = -20,
	XCAM_RETURN_ERROR_OUTOFRANGE = -21,
	XCAM_RETURN_ERROR_UNKNOWN   = -255,
} XCamReturn;

#define ISP20HW_SUBM "link:"
#define LOGE_CAMHW_SUBM(sub_modules, ...) printf(sub_modules, ##__VA_ARGS__)

#define GET_ENTITY_NAME(name, path) \
	do { \
		entity = rk_media_get_entity_by_name(device, name, \
				strlen(name));\
		if(entity) {\
			entity_name = rk_media_entity_get_devname(entity);\
			if(entity_name) {\
				strncpy(isp_info->path, entity_name, \
						sizeof(isp_info->path));\
				/*printf("%s node:%s\n", name, entity_name);*/\
			}\
		}\
	} while(0);

int v4l2_media_link_rawrd(const char *media_dev_path, bool enable)
{
	printf("%s enable:%d\n", __func__, enable);

	struct media_device *device = NULL;
	struct media_entity *entity = NULL;
	struct media_pad *src_pad_s = NULL;
	struct media_pad* src_pad_m = NULL;
	struct media_pad* src_pad_l = NULL;
	struct media_pad* sink_pad = NULL;

	device = rk_media_device_new(media_dev_path);
	if (!device)
		return XCAM_RETURN_ERROR_FAILED;

	/* Enumerate entities, pads and links. */
	rk_media_device_enumerate (device);
	entity = rk_media_get_entity_by_name(device, "rkisp-isp-subdev",
			strlen("rkisp-isp-subdev"));
	if(entity) {
		sink_pad = (struct media_pad *)rk_media_entity_get_pad(entity, 0);
		if (!sink_pad) {
			LOGE_CAMHW_SUBM(ISP20HW_SUBM, "get sink pad failed!\n");
			goto FAIL;
		}
	}
	entity = rk_media_get_entity_by_name(device, "rkisp_rawrd2_s",
			strlen("rkisp_rawrd2_s"));
	if(entity) {
		src_pad_s = (struct media_pad *)rk_media_entity_get_pad(entity, 0);
		if (!src_pad_s) {
			LOGE_CAMHW_SUBM(ISP20HW_SUBM, "get source pad s failed!\n");
			goto FAIL;
		}
	}

	if (src_pad_s && sink_pad) {
		if (enable)
			rk_media_setup_link(device, src_pad_s, sink_pad, MEDIA_LNK_FL_ENABLED);
		else
			rk_media_setup_link(device, src_pad_s, sink_pad, 0);
	}

	rk_media_device_unref (device);
	return XCAM_RETURN_NO_ERROR;

FAIL:
	rk_media_device_unref (device);
	return XCAM_RETURN_ERROR_FAILED;
}

static int v4l2_media_copy_pad(struct media_entity *src_entity, struct media_entity *dst_entity, int pad)
{
	enum v4l2_subdev_format_whence which = V4L2_SUBDEV_FORMAT_ACTIVE;
	struct v4l2_mbus_framefmt format;
	struct v4l2_rect rect;
	int ret;

	ret = rk_v4l2_subdev_get_format(src_entity, &format, pad, which);
	if (ret != 0) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "v4l2_subdev_get_format pad[%d] error\n", pad);
		return XCAM_RETURN_ERROR_FAILED;
	}

	ret = rk_v4l2_subdev_get_selection(src_entity, &rect, pad,
				V4L2_SEL_TGT_CROP,
				which);
	if (ret != 0) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "v4l2_subdev_get_selection pad[%d] error\n", pad);
		return XCAM_RETURN_ERROR_FAILED;
	}

	ret = rk_v4l2_subdev_set_format(dst_entity, &format, pad, which);
	if (ret != 0) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "v4l2_subdev_set_format pad[%d] error\n", pad);
		return XCAM_RETURN_ERROR_FAILED;
	}

	ret = rk_v4l2_subdev_set_selection(dst_entity, &rect, pad, V4L2_SEL_TGT_CROP, which);
	if (ret != 0) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "v4l2_subdev_set_format pad[%d] error\n", pad);
		return XCAM_RETURN_ERROR_FAILED;
	}
}

int v4l2_media_copy_isp_entity(const char *src_media_dev, const char *dst_media_dev)
{
	struct media_device *src_device = NULL, *dst_device = NULL;
	struct media_entity *src_entity = NULL, *dst_entity = NULL;
	int ret;

	src_device = rk_media_device_new(src_media_dev);
	if (!src_device)
		return XCAM_RETURN_ERROR_FAILED;

	dst_device = rk_media_device_new(dst_media_dev);
	if (!dst_device) {
		rk_media_device_unref(src_device);
		return XCAM_RETURN_ERROR_FAILED;
	}

	/* Enumerate entities, pads and links. */
	rk_media_device_enumerate(src_device);
	src_entity = rk_media_get_entity_by_name(src_device, "rkisp-isp-subdev",
			strlen("rkisp-isp-subdev"));
	if (!src_entity) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "rkisp-isp-subdev not found in %s!\n", src_media_dev);
		goto FAIL;
	}

	/* Enumerate entities, pads and links. */
	rk_media_device_enumerate(dst_device);
	dst_entity = rk_media_get_entity_by_name(dst_device, "rkisp-isp-subdev",
			strlen("rkisp-isp-subdev"));
	if (!dst_entity) {
		LOGE_CAMHW_SUBM(ISP20HW_SUBM, "rkisp-isp-subdev not found in %s!\n", dst_media_dev);
		goto FAIL;
	}

	ret = v4l2_media_copy_pad(src_entity, dst_entity, 0);
	if (ret != 0)
		goto FAIL;

	ret = v4l2_media_copy_pad(src_entity, dst_entity, 2);
	if (ret != 0)
		goto FAIL;

	return 0;
FAIL:
	rk_media_device_unref(src_device);
	rk_media_device_unref(dst_device);
	return XCAM_RETURN_ERROR_FAILED;
}

int v4l2_media_get_isp_info(const char *media_dev_path,
		struct media_path_s *isp_info)
{
	struct media_device *device = NULL;
	struct media_entity *entity = NULL;
	const char *entity_name = NULL;

	device = rk_media_device_new(media_dev_path);
	if (!device)
		return XCAM_RETURN_ERROR_FAILED;

	/* Enumerate entities, pads and links. */
	rk_media_device_enumerate (device);

	GET_ENTITY_NAME("rkisp-isp-subdev", isp_dev_path);
	GET_ENTITY_NAME("rkisp_mainpath", main_path);
	GET_ENTITY_NAME("rkisp_rawrd2_s", rawrd2_s_path);
	GET_ENTITY_NAME("rkisp-statistics", stats_path);
	GET_ENTITY_NAME("rkisp-input-params", input_params_path);

	rk_media_device_unref (device);
	return XCAM_RETURN_NO_ERROR;

FAIL:
	rk_media_device_unref (device);
	return XCAM_RETURN_ERROR_FAILED;
}

int v4l2_media_get_entity_node(const char *media_dev_path,
		const char *name, char *node)
{
	struct media_device *device = NULL;
	struct media_entity *entity = NULL;
	const char *entity_name = NULL;

	device = rk_media_device_new(media_dev_path);
	if (!device)
		return XCAM_RETURN_ERROR_FAILED;

	/* Enumerate entities, pads and links. */
	rk_media_device_enumerate(device);

	entity = rk_media_get_entity_by_name(device, name,
			strlen(name));
	if(entity) {
		entity_name = rk_media_entity_get_devname(entity);
		if(entity_name) {
			strncpy(node, entity_name, strlen(entity_name));
			printf("%s node:%s\n", name, entity_name);
		}
	}

	rk_media_device_unref(device);
	return XCAM_RETURN_NO_ERROR;

FAIL:
	rk_media_device_unref(device);
	return XCAM_RETURN_ERROR_FAILED;
}
