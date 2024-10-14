/*
 * Copyright (C) 2022 Rockchip Electronics Co., Ltd.
 * Authors:
 *  Cerf Yu <cerf.yu@rock-chips.com>
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

#ifndef __DMA_ALLOC_H__
#define __DMA_ALLOC_H__

int dma_sync_device_to_cpu(int fd);
int dma_sync_cpu_to_device(int fd);

int dma_buf_alloc(int width, int height, int format, int *fd, void **va);
void dma_buf_free(int width, int height, int format, int *fd, void *va);

#endif
