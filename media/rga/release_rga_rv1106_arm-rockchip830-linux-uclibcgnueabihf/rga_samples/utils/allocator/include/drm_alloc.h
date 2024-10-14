/*
 * Copyright (C) 2022  Rockchip Electronics Co., Ltd.
 * Authors:
 *     Randall zhuo <randall.zhuo@rock-chips.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __RGA_SAMPLES_ALLOCATOR_DRM_ALLOC_H__
#define __RGA_SAMPLES_ALLOCATOR_DRM_ALLOC_H__

/* memory type definitions. */
enum drm_rockchip_gem_mem_type
{
    /* Physically Continuous memory and used as default. */
    RGA_UTILS_ROCKCHIP_BO_CONTIG = 1 << 0,
    /* cachable mapping. */
    RGA_UTILS_ROCKCHIP_BO_CACHABLE = 1 << 1,
    /* write-combine mapping. */
    RGA_UTILS_ROCKCHIP_BO_WC = 1 << 2,
    RGA_UTILS_ROCKCHIP_BO_SECURE = 1 << 3,
    RGA_UTILS_ROCKCHIP_BO_MASK = RGA_UTILS_ROCKCHIP_BO_CONTIG | RGA_UTILS_ROCKCHIP_BO_CACHABLE |
                                 RGA_UTILS_ROCKCHIP_BO_WC | RGA_UTILS_ROCKCHIP_BO_SECURE
};

void* drm_buf_alloc(int TexWidth, int TexHeight,int bpp, int *fd, int *handle, size_t *actual_size, int flags = 0);
int drm_buf_destroy(int buf_fd, int handle, void *drm_buf, size_t size);
uint32_t drm_buf_get_phy(int handle);

#endif /* #ifndef __RGA_SAMPLES_ALLOCATOR_DRM_ALLOC_H__ */
