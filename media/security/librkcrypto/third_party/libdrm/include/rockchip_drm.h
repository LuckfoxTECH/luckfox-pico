/*
 * Copyright (c) Fuzhou Rockchip Electronics Co.Ltd
 * Authors:
 *       Mark Yao <yzq@rock-chips.com>
 *
 * based on exynos_drm.h
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#ifndef _ROCKCHIP_DRM_H_
#define _ROCKCHIP_DRM_H_

#include <stdint.h>
#include "drm.h"

/**
 * User-desired buffer creation information structure.
 *
 * @size: user-desired memory allocation size.
 *	- this size value would be page-aligned internally.
 * @flags: user request for setting memory type or cache attributes.
 * @handle: returned a handle to created gem object.
 *	- this handle will be set by gem module of kernel side.
 */
struct drm_rockchip_gem_create {
	uint64_t size;
	uint32_t flags;
	uint32_t handle;
};

/**
 * A structure for getting buffer offset.
 *
 * @handle: a pointer to gem object created.
 * @pad: just padding to be 64-bit aligned.
 * @offset: relatived offset value of the memory region allocated.
 *	- this value should be set by user.
 */
struct drm_rockchip_gem_map_off {
	uint32_t handle;
	uint32_t pad;
	uint64_t offset;
};

struct drm_rockchip_rga_get_ver {
	__u32   major;
	__u32   minor;
};

struct drm_rockchip_rga_cmd {
	__u32   offset;
	__u32   data;
};

enum drm_rockchip_rga_buf_type {
	RGA_BUF_TYPE_USERPTR = 1 << 31,
	RGA_BUF_TYPE_GEMFD   = 1 << 30,
};

struct drm_rockchip_rga_userptr {
	unsigned long userptr;
	unsigned long size;
};

struct drm_rockchip_rga_set_cmdlist {
	__u64		cmd;
	__u64		cmd_buf;
	__u32		cmd_nr;
	__u32		cmd_buf_nr;
	__u64		user_data;
};

struct drm_rockchip_rga_exec {
	__u64		async;
};

#define DRM_ROCKCHIP_GEM_CREATE	0x00
#define DRM_ROCKCHIP_GEM_MAP_OFFSET	0x01
#define DRM_ROCKCHIP_RGA_GET_VER		0x20
#define DRM_ROCKCHIP_RGA_SET_CMDLIST		0x21
#define DRM_ROCKCHIP_RGA_EXEC			0x22

#define DRM_IOCTL_ROCKCHIP_RGA_GET_VER		DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_RGA_GET_VER, struct drm_rockchip_rga_get_ver)

#define DRM_IOCTL_ROCKCHIP_RGA_SET_CMDLIST	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_RGA_SET_CMDLIST, struct drm_rockchip_rga_set_cmdlist)

#define DRM_IOCTL_ROCKCHIP_RGA_EXEC		DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_RGA_EXEC, struct drm_rockchip_rga_exec)

#define DRM_IOCTL_ROCKCHIP_GEM_CREATE		DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_GEM_CREATE, struct drm_rockchip_gem_create)

#define DRM_IOCTL_ROCKCHIP_GEM_MAP_OFFSET	DRM_IOWR(DRM_COMMAND_BASE + \
		DRM_ROCKCHIP_GEM_MAP_OFFSET, struct drm_rockchip_gem_map_off)

#endif
