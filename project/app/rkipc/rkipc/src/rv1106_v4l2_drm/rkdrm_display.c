/*
 * Copyright (C) 2020 Rockchip Electronics Co., Ltd.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL), available from the file
 * COPYING in the main directory of this source tree, or the
 * OpenIB.org BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "rkdrm_display.h"
#include <libdrm/drm_mode.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>
#include <xf86drmMode.h>

#define IS_SUPPORT_WINID_PROPERTY 0
#define INT_MAX 0xff
#define COLOR_KEY 0xFFFFFFFF
struct display *disp_test_;
static bool drm_plane_set_property(int fd, drmModePlane *plane, const char *prop_name,
                                   uint64_t prop_value) {
	drmModeObjectPropertiesPtr props;
	drmModePropertyPtr prop;
	int i, ret = -1;

	props = drmModeObjectGetProperties(fd, plane->plane_id, DRM_MODE_OBJECT_PLANE);
	if (!props)
		return false;

	for (i = 0; i < props->count_props; i++) {
		prop = drmModeGetProperty(fd, props->props[i]);
		if (prop && !strcmp(prop->name, prop_name)) {
			ret = drmModeObjectSetProperty(fd, plane->plane_id, DRM_MODE_OBJECT_PLANE,
			                               props->props[i], prop_value);
		}
		drmModeFreeProperty(prop);
	}

	drmModeFreeObjectProperties(props);
	return ret < 0 ? false : true;
}

/**
 * drm_fb_get_bpp_depth - get the bpp/depth values for format
 * @format: pixel format (DRM_FORMAT_*)
 * @depth: storage for the depth value
 * @bpp: storage for the bpp value
 *
 * This only supports RGB formats here for compat with code that doesn't use
 * pixel formats directly yet.
 */
void drm_fb_get_bpp_depth(uint32_t format, unsigned int *depth, int *bpp) {
	switch (format) {
	case DRM_FORMAT_C8:
	case DRM_FORMAT_RGB332:
	case DRM_FORMAT_BGR233:
		*depth = 8;
		*bpp = 8;
		break;
	case DRM_FORMAT_XRGB1555:
	case DRM_FORMAT_XBGR1555:
	case DRM_FORMAT_RGBX5551:
	case DRM_FORMAT_BGRX5551:
	case DRM_FORMAT_ARGB1555:
	case DRM_FORMAT_ABGR1555:
	case DRM_FORMAT_RGBA5551:
	case DRM_FORMAT_BGRA5551:
		*depth = 15;
		*bpp = 16;
		break;
	case DRM_FORMAT_RGB565:
	case DRM_FORMAT_BGR565:
		*depth = 16;
		*bpp = 16;
		break;
	case DRM_FORMAT_RGB888:
	case DRM_FORMAT_BGR888:
		*depth = 24;
		*bpp = 24;
		break;
	case DRM_FORMAT_XRGB8888:
	case DRM_FORMAT_XBGR8888:
	case DRM_FORMAT_RGBX8888:
	case DRM_FORMAT_BGRX8888:
		*depth = 24;
		*bpp = 32;
		break;
	case DRM_FORMAT_XRGB2101010:
	case DRM_FORMAT_XBGR2101010:
	case DRM_FORMAT_RGBX1010102:
	case DRM_FORMAT_BGRX1010102:
	case DRM_FORMAT_ARGB2101010:
	case DRM_FORMAT_ABGR2101010:
	case DRM_FORMAT_RGBA1010102:
	case DRM_FORMAT_BGRA1010102:
		*depth = 30;
		*bpp = 32;
		break;
	case DRM_FORMAT_ARGB8888:
	case DRM_FORMAT_ABGR8888:
	case DRM_FORMAT_RGBA8888:
	case DRM_FORMAT_BGRA8888:
		*depth = 32;
		*bpp = 32;
		break;
	default:
		printf("unsupported pixel format %d\n", format);
		*depth = 0;
		*bpp = 0;
		break;
	}
}

int drm_format_to_bpp(uint32_t format) {
	unsigned int depth;
	int bpp;

	switch (format) {
	case DRM_FORMAT_NV12:
	case DRM_FORMAT_NV21:
	case DRM_FORMAT_YUV420:
	case DRM_FORMAT_YUV422:
	case DRM_FORMAT_YUV444:
		return 8;
	default:
		drm_fb_get_bpp_depth(format, &depth, &bpp);
		return bpp;
	}
}

int drmGetBuffer(int fd, int width, int height, int format, struct drm_buf *buffer) {
	struct drm_mode_create_dumb alloc_arg;
	struct drm_mode_map_dumb mmap_arg;
	struct drm_mode_destroy_dumb destory_arg;
	uint32_t handles[4], pitches[4], offsets[4];
	int bpp, ret;
	void *map;

	if (fd < 0 || !width || !height) {
		printf("%s: invalid parameters\n", __func__);
		return -EINVAL;
	}

	bpp = drm_format_to_bpp(format);

	memset(&alloc_arg, 0, sizeof(alloc_arg));
	alloc_arg.bpp = bpp;
	alloc_arg.width = width;
	if (format == DRM_FORMAT_NV12)
		alloc_arg.height = height * 3 / 2;
	else
		alloc_arg.height = height * 4;

	ret = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &alloc_arg);
	if (ret) {
		printf("failed to create dumb buffer: %s\n", strerror(errno));
		return ret;
	}
	// HACK of gpu 64 bytes align?
	if (format == DRM_FORMAT_NV12)
		alloc_arg.pitch = width;
	else
		alloc_arg.pitch = width * 4;

	memset(&mmap_arg, 0, sizeof(mmap_arg));
	mmap_arg.handle = alloc_arg.handle;

	ret = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mmap_arg);
	if (ret) {
		printf("failed to create map dumb: %s\n", strerror(errno));
		ret = -EINVAL;
		goto destory_dumb;
	}

	map = mmap(0, alloc_arg.size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, mmap_arg.offset);
	if (map == MAP_FAILED) {
		printf("failed to mmap buffer: %s\n", strerror(errno));
		ret = -EINVAL;
		goto destory_dumb;
	}

	ret = drmPrimeHandleToFD(fd, alloc_arg.handle, 0, &buffer->dmabuf_fd);
	if (ret) {
		printf("failed to get dmabuf fd: %s\n", strerror(errno));
		munmap(map, alloc_arg.size);
		ret = -EINVAL;
		goto destory_dumb;
	}

	handles[0] = alloc_arg.handle;
	pitches[0] = alloc_arg.pitch;
	offsets[0] = 0;

	if (format == DRM_FORMAT_NV12) {
		handles[1] = alloc_arg.handle;
		pitches[1] = pitches[0];
		offsets[1] = pitches[0] * height;
	}

	ret = drmModeAddFB2(fd, width, height, format, handles, pitches, offsets,
	                    (uint32_t *)&buffer->fb_id, 0);
	if (ret)
		printf("failed to create fb_id %d\n", ret);

	buffer->handle = alloc_arg.handle;
	buffer->pitch = alloc_arg.pitch;
	buffer->size = alloc_arg.size;
	buffer->map = map;

destory_dumb:
	memset(&destory_arg, 0, sizeof(destory_arg));
	destory_arg.handle = alloc_arg.handle;
	drmIoctl(fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destory_arg);

	return ret;
}

int drmPutBuffer(int fd, struct drm_buf *buffer) {
	if (buffer) {
		drmModeRmFB(fd, buffer->fb_id);
		close(buffer->dmabuf_fd);
		return munmap(buffer->map, buffer->size);
	}

	return -EINVAL;
}

static int drmGetPlaneType(int fd, drmModePlanePtr p) {
	drmModeObjectPropertiesPtr props;
	drmModePropertyPtr prop;
	int i, type = -1;

	props = drmModeObjectGetProperties(fd, p->plane_id, DRM_MODE_OBJECT_PLANE);
	if (!props) {
		printf("failed to found props plane[%d] %s\n", p->plane_id, strerror(errno));
		return -errno;
	}

	for (i = 0; i < props->count_props; i++) {
		prop = drmModeGetProperty(fd, props->props[i]);
		if (!strcmp(prop->name, "type")) {
			type = props->prop_values[i];
			drmModeFreeProperty(prop);
			break;
		}
		drmModeFreeProperty(prop);
	}

	drmModeFreeObjectProperties(props);
	return type;
}

static int drmFillPlaneProp(int fd, struct drm_dev_plane *pp) {
	drmModeObjectPropertiesPtr props;
	drmModePropertyPtr prop;
	struct plane_prop *plane_prop = &pp->plane_prop;
	drmModePlanePtr p = pp->p;
	int i;

	props = drmModeObjectGetProperties(fd, p->plane_id, DRM_MODE_OBJECT_PLANE);
	if (!props) {
		printf("failed to found props plane[%d] %s\n", p->plane_id, strerror(errno));
		return -errno;
	}

	pp->zpos_max = INT_MAX;
	for (i = 0; i < props->count_props; i++) {
		prop = drmModeGetProperty(fd, props->props[i]);
		if (!strcmp(prop->name, "CRTC_ID"))
			plane_prop->crtc_id = prop->prop_id;
		else if (!strcmp(prop->name, "FB_ID"))
			plane_prop->fb_id = prop->prop_id;
		else if (!strcmp(prop->name, "SRC_X"))
			plane_prop->src_x = prop->prop_id;
		else if (!strcmp(prop->name, "SRC_Y"))
			plane_prop->src_y = prop->prop_id;
		else if (!strcmp(prop->name, "SRC_W"))
			plane_prop->src_w = prop->prop_id;
		else if (!strcmp(prop->name, "SRC_H"))
			plane_prop->src_h = prop->prop_id;
		else if (!strcmp(prop->name, "CRTC_X"))
			plane_prop->crtc_x = prop->prop_id;
		else if (!strcmp(prop->name, "CRTC_Y"))
			plane_prop->crtc_y = prop->prop_id;
		else if (!strcmp(prop->name, "CRTC_W"))
			plane_prop->crtc_w = prop->prop_id;
		else if (!strcmp(prop->name, "CRTC_H"))
			plane_prop->crtc_h = prop->prop_id;
		else if (!strcmp(prop->name, "ZPOS")) {
			plane_prop->zpos = prop->prop_id;
			pp->zpos_max = prop->values[1];
		} else if (!strcmp(prop->name, "ACTIVE")) {
			plane_prop->property_active = prop->prop_id;
		} else if (!strcmp(prop->name, "MODE_ID")) {
			plane_prop->property_mode_id = prop->prop_id;
		}

		drmModeFreeProperty(prop);
	}
	drmModeFreeObjectProperties(props);

	return 0;
}

static drmModePlanePtr drmGetPlaneByType(int fd, int crtc_index, int type) {
	drmModePlanePtr plane = NULL;
	drmModePlaneResPtr plane_res;
	int i;

	plane_res = drmModeGetPlaneResources(fd);
	for (i = 0; i < plane_res->count_planes; i++) {
		drmModePlanePtr p = drmModeGetPlane(fd, plane_res->planes[i]);

		if (!(p->possible_crtcs & (1 << crtc_index))) {
			drmModeFreePlane(p);
			continue;
		}
		if (drmGetPlaneType(fd, p) == type) {
			plane = p;
			break;
		}
		drmModeFreePlane(p);
	}

	drmModeFreePlaneResources(plane_res);

	return plane;
}

drmModeConnectorPtr drmFoundConn(int fd, drmModeResPtr res) {
	drmModeConnectorPtr connector = NULL;
	int i;

	for (i = 0; i < res->count_connectors; ++i) {
		drmModeConnectorPtr c;
		c = drmModeGetConnector(fd, res->connectors[i]);
		if (!c)
			continue;
		if (c->connector_type == DRM_MODE_CONNECTOR_HDMIA ||
		    c->connector_type == DRM_MODE_CONNECTOR_eDP ||
		    c->connector_type == DRM_MODE_CONNECTOR_LVDS) {
			connector = c;
			break;
		}
		drmModeFreeConnector(c);
	}

	return connector;
}

static drmModePropertyPtr drmFoundDPMS(int fd, drmModeConnectorPtr connector) {
	drmModePropertyPtr dpms_prop = NULL;
	drmModeObjectPropertiesPtr props;
	int i;

	props = drmModeObjectGetProperties(fd, connector->connector_id, DRM_MODE_OBJECT_CONNECTOR);
	if (!props) {
		printf("failed to found props connector[%d] %s\n", connector->connector_id,
		       strerror(errno));
		goto out;
	}
	for (i = 0; i < props->count_props; i++) {
		drmModePropertyPtr prop;
		prop = drmModeGetProperty(fd, props->props[i]);
		if (!strcmp(prop->name, "DPMS")) {
			dpms_prop = prop;
			goto out;
		}
		drmModeFreeProperty(prop);
	}

out:
	drmModeFreeObjectProperties(props);
	return dpms_prop;
}

static drmModeCrtcPtr drmFoundCrtc(int fd, drmModeResPtr res, drmModeConnector *conn,
                                   int *crtc_index) {
	int i;
	int crtc_id;
	drmModeEncoder *enc;
	drmModeCrtc *crtc;
	uint32_t crtcs_for_connector = 0;

	crtc_id = -1;
	for (i = 0; i < res->count_encoders; i++) {
		enc = drmModeGetEncoder(fd, res->encoders[i]);
		if (enc) {
			if (enc->encoder_id == conn->encoder_id) {
				crtc_id = enc->crtc_id;
				drmModeFreeEncoder(enc);
				break;
			}
			drmModeFreeEncoder(enc);
		}
	}

	/* If no active crtc was found, pick the first possible crtc */
	if (crtc_id == -1) {
		for (i = 0; i < conn->count_encoders; i++) {
			enc = drmModeGetEncoder(fd, conn->encoders[i]);
			crtcs_for_connector |= enc->possible_crtcs;
			drmModeFreeEncoder(enc);
		}

		if (crtcs_for_connector != 0)
			crtc_id = res->crtcs[ffs(crtcs_for_connector) - 1];
	}

	if (crtc_id == -1)
		return NULL;

	for (i = 0; i < res->count_crtcs; i++) {
		crtc = drmModeGetCrtc(fd, res->crtcs[i]);
		printf("name is %s\n", crtc->mode.name);
		if (crtc) {
			if (crtc_id == crtc->crtc_id) {
				if (crtc_index)
					*crtc_index = i;
				return crtc;
			}
			drmModeFreeCrtc(crtc);
		}
	}

	return NULL;
}

int drmInit(struct drm_dev *dev) {
	drmModeResPtr res;
	drmModeCrtcPtr crtc = NULL;
	drmModeConnectorPtr connector = NULL;
	drmModePlanePtr plane = NULL;
	int fd, ret;
	int crtc_index;

	fd = drmOpen("rockchip", NULL);
	if (fd < 0) {
		printf("failed to open rockchip drm: %s\n", strerror(errno));
		return fd;
	}
	dev->drm_fd = fd;

	ret = drmSetClientCap(fd, DRM_CLIENT_CAP_UNIVERSAL_PLANES, 1);
	if (ret) {
		printf("Failed to set atomic cap %s", strerror(errno));
		return ret;
	}

	ret = drmSetClientCap(fd, DRM_CLIENT_CAP_ATOMIC, 1);
	if (ret) {
		printf("Failed to set atomic cap %s", strerror(errno));
		return ret;
	}

	res = drmModeGetResources(fd);
	if (!res) {
		printf("Failed to get resources: %s\n", strerror(errno));
		return -ENODEV;
	}

	/* found connector */
	connector = drmFoundConn(fd, res);
	if (!connector) {
		printf("failed to find connector\n");
		ret = -ENODEV;
		goto err_conn;
	}
	printf("connector->connector_id is %d\n", connector->connector_id);
	/* found connector dpms prop */
	dev->dpms_prop = drmFoundDPMS(fd, connector);
	if (!dev->dpms_prop) {
		ret = -ENODEV;
		goto err_dpms;
	}

	/* found crtc that connect to connector */
	crtc = drmFoundCrtc(fd, res, connector, &crtc_index);
	if (!crtc) {
		printf("failed to find crtc connect to connector \n");
		ret = -ENODEV;
		goto err_crtc;
	}
	printf("crtc_index is %d\n", crtc_index);

	plane = drmGetPlaneByType(fd, crtc_index, DRM_PLANE_TYPE_PRIMARY);
	if (!plane) {
		printf("failed to found expected plane DRM_PLANE_TYPE_PRIMARY\n");
		ret = -ENODEV;
		goto err_plane_primary;
	}
	dev->plane_primary.p = plane;

	// plane = drmGetPlaneByType(fd, crtc_index, DRM_PLANE_TYPE_OVERLAY);
	// if (!plane) {
	// 	printf("failed to found expected plane DRM_PLANE_TYPE_OVERLAY\n");
	// 	ret = -ENODEV;
	// 	goto err_plane_overlay;
	// }
	// dev->plane_overlay.p = plane;

	drmModeFreeResources(res);

	drmFillPlaneProp(fd, &dev->plane_primary);
	// drmFillPlaneProp(fd, &dev->plane_overlay);
	dev->crtc_index = crtc_index;
	dev->crtc = crtc;
	dev->connector = connector;

	if (!drm_plane_set_property(fd, dev->plane_primary.p, "zpos", 0)) {
		printf("%s: plane_primary set ZPOS property failed!\n", __func__);
		goto err_plane_overlay;
	}
	// if (!drm_plane_set_property(fd, dev->plane_overlay.p, "zpos", 1)) {
	//     printf("%s: plane_overlay set ZPOS property failed!\n", __func__);
	//     goto err_plane_overlay;
	// }

	// printf("dev->connector->modes[0] name %s, %d,%d\n", dev->connector->modes[0].name,
	// dev->connector->modes[0].hdisplay, dev->connector->modes[0].vdisplay);
	// printf("dev->connector->modes[17] name %s, %d,%d\n", dev->connector->modes[17].name,
	// dev->connector->modes[17].hdisplay, dev->connector->modes[17].vdisplay);

#if 1
	// drmModeCreatePropertyBlob(fd, &dev->connector->modes[15], sizeof(dev->connector->modes[15]),
	// &dev->plane_primary.plane_prop.blob_id); printf("blob_id is %d\n",
	// dev->plane_primary.plane_prop.blob_id);
#else
	printf("drmModeSetCrtc crtc_id is %d\n", crtc->crtc_id);
	printf("drmModeSetCrtc connector_id is %d\n", res->connectors);
	ret = drmModeSetCrtc(fd, crtc->crtc_id, disp_test_->buf[0].fb_id, 0, 0, &res->connectors, 1,
	                     &dev->connector->modes[17]);
	printf("drmModeSetCrtc ret is %d\n", ret);
#endif

	return 0;

err_plane_overlay:
	drmModeFreePlane(dev->plane_primary.p);
err_plane_primary:
	drmModeFreeCrtc(crtc);
err_crtc:
	drmModeFreeProperty(dev->dpms_prop);
err_dpms:
	drmModeFreeConnector(connector);
err_conn:
	drmModeFreeResources(res);

	return ret;
}

int drmDeinit(struct drm_dev *dev) {
	if (dev->plane_primary.p)
		drmModeFreePlane(dev->plane_primary.p);
	if (dev->plane_overlay.p)
		drmModeFreePlane(dev->plane_overlay.p);
	if (dev->dpms_prop)
		drmModeFreeProperty(dev->dpms_prop);
	if (dev->crtc)
		drmModeFreeCrtc(dev->crtc);
	if (dev->connector)
		drmModeFreeConnector(dev->connector);
	if (dev->drm_fd > 0)
		drmClose(dev->drm_fd);

	return 0;
}

int drmCommit(struct drm_buf *buffer, int width, int height, int x_off, int y_off,
              struct drm_dev *dev, int plane_type) {
	drmModeAtomicReq *req;
	drmModeCrtcPtr crtc = dev->crtc;
	drmModePlanePtr plane;
	struct plane_prop *plane_prop;
	int plane_zpos;
	uint32_t flags = 0;
	int ret;

	if (dev->drm_fd < 0 || !buffer) {
		printf("%s: invalid parameters\n", __func__);
		return -EINVAL;
	}

	if (plane_type == DRM_PLANE_TYPE_PRIMARY) {
		plane = dev->plane_primary.p;
		plane_zpos = dev->plane_primary.zpos_max;
		plane_prop = &dev->plane_primary.plane_prop;
	} else {
		plane = dev->plane_overlay.p;
		plane_zpos = dev->plane_overlay.zpos_max;
		plane_prop = &dev->plane_overlay.plane_prop;
	}
	// printf("crtc->mode.hdisplay is %d, crtc->mode.vdisplay is %d\n", crtc->mode.hdisplay,
	// crtc->mode.vdisplay);
	width = width == 0 ? crtc->mode.hdisplay : width;
	height = height == 0 ? crtc->mode.vdisplay : height;

	req = drmModeAtomicAlloc();

#define DRM_ATOMIC_ADD_PROP(object_id, value)                                                      \
	ret = drmModeAtomicAddProperty(req, plane->plane_id, object_id, value);                        \
	if (ret < 0)                                                                                   \
		printf("Failed to add prop[%d] to [%d]", value, object_id);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_id, crtc->crtc_id);
	DRM_ATOMIC_ADD_PROP(plane_prop->fb_id, buffer->fb_id);
#if 0
	DRM_ATOMIC_ADD_PROP(plane_prop->src_x, 0);
	DRM_ATOMIC_ADD_PROP(plane_prop->src_y, 0);
	DRM_ATOMIC_ADD_PROP(plane_prop->src_w, width << 16);
	DRM_ATOMIC_ADD_PROP(plane_prop->src_h, height << 16);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_x, x_off);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_y, y_off);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_w, width);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_h, height);
#else
	DRM_ATOMIC_ADD_PROP(plane_prop->src_x, 0);
	DRM_ATOMIC_ADD_PROP(plane_prop->src_y, 0);
	DRM_ATOMIC_ADD_PROP(plane_prop->src_w, width << 16);
	DRM_ATOMIC_ADD_PROP(plane_prop->src_h, height << 16);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_x, 0);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_y, 0);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_w, width);
	DRM_ATOMIC_ADD_PROP(plane_prop->crtc_h, height);
#endif
	// DRM_ATOMIC_ADD_PROP(plane_prop->property_active, 1);
	// DRM_ATOMIC_ADD_PROP(plane_prop->property_mode_id, plane_prop->blob_id);
	// DRM_ATOMIC_ADD_PROP(plane_prop->zpos, plane_zpos);
	// flags |= DRM_MODE_ATOMIC_ALLOW_MODESET;
	ret = drmModeAtomicCommit(dev->drm_fd, req, flags, NULL);
	if (ret)
		printf("atomic: couldn't commit new state: %s\n", strerror(errno));

	drmModeAtomicFree(req);

	return ret;
}

int drm_display_init(struct display *disp) {
	int ret;
	disp_test_ = disp;
	if (drmInit(&disp->dev)) {
		fprintf(stderr, "drmInit: Failed\n");
		return -1;
	}
	for (int i = 0; i < disp->buf_cnt; i++) {
		ret = drmGetBuffer(disp->dev.drm_fd, disp->width, disp->height, disp->fmt, &disp->buf[i]);
		if (ret) {
			fprintf(stderr, "Alloc drm buffer failed, %d\n", i);
			return -1;
		}
	}

	return 0;
}
