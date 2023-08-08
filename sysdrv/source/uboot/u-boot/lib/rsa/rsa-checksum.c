/*
 * Copyright (c) 2013, Andreas Oetken.
 *
 * SPDX-License-Identifier:    GPL-2.0+
 */

#ifndef USE_HOSTCC
#include <common.h>
#include <crypto.h>
#include <fdtdec.h>
#include <asm/byteorder.h>
#include <linux/errno.h>
#include <asm/unaligned.h>
#include <hash.h>
#else
#include "fdt_host.h"
#endif
#include <u-boot/rsa.h>

int rsa_hash_calculate(const char *name,
		       const struct image_region region[],
		       int region_count, uint8_t *checksum)
{
	struct hash_algo *algo;
	int ret = 0;
	void *ctx;
	uint32_t i;
	i = 0;

	ret = hash_progressive_lookup_algo(name, &algo);
	if (ret)
		return ret;

	ret = algo->hash_init(algo, &ctx);
	if (ret)
		return ret;

	for (i = 0; i < region_count - 1; i++) {
		ret = algo->hash_update(algo, ctx, region[i].data,
					region[i].size, 0);
		if (ret)
			return ret;
	}

	ret = algo->hash_update(algo, ctx, region[i].data, region[i].size, 1);
	if (ret)
		return ret;
	ret = algo->hash_finish(algo, ctx, checksum, algo->digest_size);
	if (ret)
		return ret;

	return 0;
}

#if !defined(USE_HOSTCC)
#if CONFIG_IS_ENABLED(FIT_HW_CRYPTO)
int hw_rsa_hash_calculate(const char *name,
			  const struct image_region region[],
			  int region_count, uint8_t *checksum)

{
	struct udevice *dev;
	sha_context ctx;

	if (!name)
		return -EINVAL;

	if (!strcmp(name, "sha1"))
		ctx.algo = CRYPTO_SHA1;
	else if (!strcmp(name, "sha256"))
		ctx.algo = CRYPTO_SHA256;
	else
		return -EPERM;

	dev = crypto_get_device(ctx.algo);
	if (!dev) {
		printf("No crypto device for expected capability\n");
		return -ENODEV;
	}

	return crypto_sha_regions_csum(dev, &ctx, region,
				       region_count, checksum);
}
#endif
#endif

int hash_calculate(const char *name,
		   const struct image_region region[],
		   int region_count, uint8_t *checksum)
{
#if defined(USE_HOSTCC)
	return rsa_hash_calculate(name, region, region_count, checksum);
#else
#if !CONFIG_IS_ENABLED(FIT_HW_CRYPTO)
	return rsa_hash_calculate(name, region, region_count, checksum);
#else
	return hw_rsa_hash_calculate(name, region, region_count, checksum);
#endif
#endif
}
