/*
 * Copyright 2015 Google Inc.
 *
 * SPDX-License-Identifier: GPL 2.0+ BSD-3-Clause
 */

#include <common.h>
#include <compiler.h>
#include <misc.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <asm/unaligned.h>

static u16 LZ4_readLE16(const void *src)
{
	return get_unaligned_le16(src);
}
static void LZ4_copy4(void *dst, const void *src)
{
	put_unaligned(get_unaligned((const u32 *)src), (u32 *)dst);
}
static void LZ4_copy8(void *dst, const void *src)
{
	put_unaligned(get_unaligned((const u64 *)src), (u64 *)dst);
}

typedef  uint8_t BYTE;
typedef uint16_t U16;
typedef uint32_t U32;
typedef  int32_t S32;
typedef uint64_t U64;

#define FORCE_INLINE static inline __attribute__((always_inline))

/* Unaltered (except removing unrelated code) from github.com/Cyan4973/lz4. */
#include "lz4.c"	/* #include for inlining, do not link! */

bool lz4_is_valid_header(const unsigned char *h)
{
	const struct lz4_frame_header *hdr  = (const struct lz4_frame_header *)h;
	/* We assume there's always only a single, standard frame. */
	if (le32_to_cpu(hdr->magic) != LZ4F_MAGIC || hdr->version != 1)
		return false;        /* unknown format */
	if (hdr->reserved0 || hdr->reserved1 || hdr->reserved2)
		return false; /* reserved must be zero */
	if (!hdr->independent_blocks)
		return false; /* we can't support this yet */

	return true;
}

int ulz4fn(const void *src, size_t srcn, void *dst, size_t *dstn)
{
	const void *end = dst + *dstn;
	const void *in = src;
	void *out = dst;
	int has_block_checksum;
	int ret;
	*dstn = 0;

#if defined(CONFIG_MISC_DECOMPRESS) && !defined(CONFIG_SPL_BUILD)
	u64 len;

	ret = misc_decompress_process((ulong)dst, (ulong)src, (ulong)srcn,
				      DECOM_LZ4, false, &len, 0);
	if (!ret) {
		*dstn = len;
		return 0;
	}

	printf("hw ulz4fn failed(%d), fallback to soft ulz4fn\n", ret);
#endif
	{ /* With in-place decompression the header may become invalid later. */
		const struct lz4_frame_header *h = in;

		if (srcn < sizeof(*h) + sizeof(u64) + sizeof(u8))
			return -EINVAL;	/* input overrun */

		/* We assume there's always only a single, standard frame. */
		if (le32_to_cpu(h->magic) != LZ4F_MAGIC || h->version != 1)
			return -EPROTONOSUPPORT;	/* unknown format */
		if (h->reserved0 || h->reserved1 || h->reserved2)
			return -EINVAL;	/* reserved must be zero */
		if (!h->independent_blocks)
			return -EPROTONOSUPPORT; /* we can't support this yet */
		has_block_checksum = h->has_block_checksum;

		in += sizeof(*h);
		if (h->has_content_size)
			in += sizeof(u64);
		in += sizeof(u8);
	}

	while (1) {
		struct lz4_block_header b;

		b.raw = le32_to_cpu(*(u32 *)in);
		in += sizeof(struct lz4_block_header);

		if (in - src + b.size > srcn) {
			ret = -EINVAL;		/* input overrun */
			break;
		}

		if (!b.size) {
			ret = 0;	/* decompression successful */
			break;
		}

		if (b.not_compressed) {
			size_t size = min((ptrdiff_t)b.size, end - out);
			memcpy(out, in, size);
			out += size;
			if (size < b.size) {
				ret = -ENOBUFS;	/* output overrun */
				break;
			}
		} else {
			/* constant folding essential, do not touch params! */
			ret = LZ4_decompress_generic(in, out, b.size,
					end - out, endOnInputSize,
					full, 0, noDict, out, NULL, 0);
			if (ret < 0) {
				ret = -EPROTO;	/* decompression error */
				break;
			}
			out += ret;
		}

		in += b.size;
		if (has_block_checksum)
			in += sizeof(u32);
	}

	*dstn = out - dst;
	return ret;
}
