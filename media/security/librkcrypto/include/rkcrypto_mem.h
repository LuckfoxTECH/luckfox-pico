/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */

#ifndef __RKCRYPTO_MEM_H__
#define __RKCRYPTO_MEM_H__

typedef struct {
	void		*vaddr;
	int		dma_fd;
	size_t		size;
} rk_crypto_mem;

int rk_crypto_mem_init(void);
void rk_crypto_mem_deinit(void);
rk_crypto_mem *rk_crypto_mem_alloc(size_t size);
void rk_crypto_mem_free(rk_crypto_mem *memory);

#endif
