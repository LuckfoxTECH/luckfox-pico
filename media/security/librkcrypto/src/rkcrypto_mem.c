/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <xf86drm.h>
#include <rockchip_drm.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include "rkcrypto_mem.h"
#include "rkcrypto_trace.h"
#include "rk_list.h"
#include "dma-heap.h"

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

#define DRM_MODULE_NAME "rockchip"
#define DRM_CARD_PATH "/dev/dri/card0"
#define DMA_HEAP_PATH "/dev/rk_dma_heap/rk-dma-heap-cma"

#define IS_DMA_INVALID()	(dma_node_fd < 0)

struct mem_pool_node {
	rk_crypto_mem		mem;
	uint32_t		handle;
	uint32_t		flags;
	struct list_head	list;
};

struct mem_ops {
	int (*init)(void);
	void (*deinit)(int dma_node_fd);
	struct mem_pool_node *(*alloc_node)(int dma_node_fd, uint32_t size);
	void (*free_node)(int dma_node_fd, struct mem_pool_node *node);
};

static int dma_node_fd = -1;
static struct list_head mem_pool_list;
pthread_mutex_t dma_mutex = PTHREAD_MUTEX_INITIALIZER;
static int mem_init_cnt;
struct mem_ops *cur_mem_pos;

static int crypto_init_drm(void)
{
	int fd;

	fd = open(DRM_CARD_PATH, O_RDWR);
	if (fd < 0)
		D_TRACE("failed to open drm !\n");

	return fd;
}

static void crypto_deinit_drm(int dma_node_fd)
{
	if (dma_node_fd >= 0)
		close(dma_node_fd);
}

static struct mem_pool_node *crypto_alloc_node_drm(int dma_node_fd, uint32_t size)
{
	int ret = -1;
	size_t min_size;
	struct mem_pool_node *node = NULL;
	struct drm_rockchip_gem_create req = {
		.size = size,
		.flags = 1,
	};
	struct drm_rockchip_gem_map_off map_req;

	/* cma must alloc at least two page */
	min_size = 2 * getpagesize();
	req.size  = size < min_size ? min_size : size;

	node = malloc(sizeof(*node));
	if (!node)
		return NULL;

	memset(node, 0x00, sizeof(*node));
	memset(&map_req, 0x00, sizeof(map_req));

	ret = drmIoctl(dma_node_fd, DRM_IOCTL_ROCKCHIP_GEM_CREATE, &req);
	if (ret) {
		free(node);
		return NULL;
	}

	ret = drmPrimeHandleToFD(dma_node_fd, req.handle, 0, &node->mem.dma_fd);
	if (ret) {
		E_TRACE("failed to get dma dma_node_fd.\n");
		goto error;
	}

	map_req.handle = req.handle;
	ret = drmIoctl(dma_node_fd, DRM_IOCTL_ROCKCHIP_GEM_MAP_OFFSET, &map_req);
	if (ret) {
		E_TRACE("failed to ioctl gem map offset.");
		goto error;
	}

	D_TRACE("handle = %u, dma_fd = %d, alloc_size = %u, real_size = %u\n",
		req.handle, node->mem.dma_fd, size, req.size);

#ifdef __ANDROID__
	node->mem.vaddr = mmap64(0, req.size, PROT_READ | PROT_WRITE, MAP_SHARED,
				 dma_node_fd, map_req.offset);
#else
	node->mem.vaddr = mmap(0, req.size, PROT_READ | PROT_WRITE, MAP_SHARED,
			       dma_node_fd, map_req.offset);
#endif
	if (node->mem.vaddr == MAP_FAILED) {
		E_TRACE("failed to mmap buffer. offset = %"PRIu64", reason: %s\n",
			map_req.offset, strerror(errno));
		ret = -1;
		goto error;
	}

	node->handle   = req.handle;
	node->flags    = req.flags;
	node->mem.size = size;

	return node;
error:
	drmIoctl(dma_node_fd, DRM_IOCTL_GEM_CLOSE, &req);

	if (node)
		free(node);

	return NULL;
}

static void crypto_free_node_drm(int dma_node_fd, struct mem_pool_node *node)
{
	struct drm_gem_close req;
	size_t min_size;
	min_size = 2 * getpagesize();

	if (!node || node->mem.size == 0)
		return;

	memset(&req, 0x00, sizeof(req));

	req.handle = node->handle;

	if (node->mem.vaddr)
		munmap(node->mem.vaddr, node->mem.size < min_size ? min_size : node->mem.size);

	if (node->mem.dma_fd >= 0)
		close(node->mem.dma_fd);

	drmIoctl(dma_node_fd, DRM_IOCTL_GEM_CLOSE, &req);

	free(node);
}

static int crypto_init_dma_heap(void)
{
	int fd;

	fd = open(DMA_HEAP_PATH, O_RDWR);
	if (fd < 0)
		D_TRACE("failed to open cma heap !\n");

	return fd;
}

static void crypto_deinit_dma_heap(int dma_node_fd)
{
	if (dma_node_fd >= 0)
		close(dma_node_fd);
}

static struct mem_pool_node *crypto_alloc_node_dma_heap(int dma_node_fd, uint32_t size)
{
	int ret = -1;
	size_t min_size;
	struct mem_pool_node *node = NULL;
	struct dma_heap_allocation_data req = {
		.len = size,
		.fd_flags = O_CLOEXEC | O_RDWR,
	};

	/* cma must alloc at least two page */
	min_size = 2 * getpagesize();
	req.len  = size < min_size ? min_size : size;

	node = malloc(sizeof(*node));
	if (!node)
		return NULL;

	memset(node, 0x00, sizeof(*node));

	req.fd = 0;
	ret = ioctl(dma_node_fd, DMA_HEAP_IOCTL_ALLOC, &req);
	if (ret < 0) {
		E_TRACE("DMA_HEAP_ALLOC_BUFFER failed\n");
		free(node);
		return NULL;
	}

	node->mem.dma_fd = req.fd;

	D_TRACE("dma_fd = %d, alloc_size = %u, real_size = %u\n",
		node->mem.dma_fd, size, req.len);

#ifdef __ANDROID__
	node->mem.vaddr = mmap64(0, req.len, PROT_READ | PROT_WRITE, MAP_SHARED, req.fd, 0);
#else
	node->mem.vaddr = mmap(0, req.len, PROT_READ | PROT_WRITE, MAP_SHARED, req.fd, 0);
#endif
	if (node->mem.vaddr == MAP_FAILED) {
		E_TRACE("failed to mmap buffer. fd = %"PRIu64", reason: %s\n",
			req.fd, strerror(errno));
		ret = -1;
		goto error;
	}

	node->flags    = req.fd_flags;
	node->mem.size = size;

	return node;
error:
	close(req.fd);

	if (node)
		free(node);

	return NULL;
}

static void crypto_free_node_dma_heap(int dma_node_fd, struct mem_pool_node *node)
{
	size_t min_size;

	min_size = 2 * getpagesize();

	if (!node || node->mem.size == 0)
		return;

	if (node->mem.vaddr)
		munmap(node->mem.vaddr, node->mem.size < min_size ? min_size : node->mem.size);

	if (node->mem.dma_fd >= 0)
		close(node->mem.dma_fd);

	free(node);
}

struct mem_ops rk_mem_ops_tbl[] = {
	{
		.init       = crypto_init_dma_heap,
		.deinit     = crypto_deinit_dma_heap,
		.alloc_node = crypto_alloc_node_dma_heap,
		.free_node  = crypto_free_node_dma_heap,
	},
	{
		.init       = crypto_init_drm,
		.deinit     = crypto_deinit_drm,
		.alloc_node = crypto_alloc_node_drm,
		.free_node  = crypto_free_node_drm,
	},
};

int rk_crypto_mem_init(void)
{
	int ret = -1;
	uint32_t i;

	if (mem_init_cnt > 0) {
		ret = 0;
		return ret;
	}

	pthread_mutex_lock(&dma_mutex);

	INIT_LIST_HEAD(&mem_pool_list);

	for (i = 0; i < ARRAY_SIZE(rk_mem_ops_tbl); i++) {
		dma_node_fd = rk_mem_ops_tbl[i].init();
		if (dma_node_fd >= 0)
			break;
	}

	if (dma_node_fd < 0) {
		ret = -1;
		goto exit;
	}

	cur_mem_pos = &rk_mem_ops_tbl[i];

	mem_init_cnt++;

	ret = 0;
exit:
	pthread_mutex_unlock(&dma_mutex);

	return ret;
}

void rk_crypto_mem_deinit(void)
{
	/* free list */
	struct mem_pool_node *node;
	struct list_head *pos = NULL, *n = NULL;

	pthread_mutex_lock(&dma_mutex);

	mem_init_cnt--;
	if (mem_init_cnt > 0)
		goto exit;

	if (IS_DMA_INVALID())
		goto exit;

	list_for_each_safe(pos, n, &mem_pool_list) {
		node = list_entry(pos, struct mem_pool_node, list);
		list_del(pos);

		if (cur_mem_pos)
			cur_mem_pos->free_node(dma_node_fd, node);
	}

	if (cur_mem_pos)
		cur_mem_pos->deinit(dma_node_fd);
exit:
	pthread_mutex_unlock(&dma_mutex);
}

rk_crypto_mem *rk_crypto_mem_alloc(size_t size)
{
	struct mem_pool_node *node;

	pthread_mutex_lock(&dma_mutex);

	if (IS_DMA_INVALID() || !cur_mem_pos)
		goto error;

	node = cur_mem_pos->alloc_node(dma_node_fd, size);
	if (!node)
		goto error;

	list_add_tail(&node->list, &mem_pool_list);

	pthread_mutex_unlock(&dma_mutex);

	return &node->mem;
error:
	pthread_mutex_unlock(&dma_mutex);

	return NULL;
}

void rk_crypto_mem_free(rk_crypto_mem *memory)
{
	struct mem_pool_node *node;
	struct list_head *pos = NULL, *n = NULL;

	pthread_mutex_lock(&dma_mutex);

	if (IS_DMA_INVALID() || !cur_mem_pos)
		goto exit;

	if (!memory)
		goto exit;

	list_for_each_safe(pos, n, &mem_pool_list) {
		node = list_entry(pos, struct mem_pool_node, list);

		if (&node->mem == memory) {
			list_del(pos);
			cur_mem_pos->free_node(dma_node_fd, node);
			goto exit;
		}
	}

exit:
	pthread_mutex_unlock(&dma_mutex);
}
