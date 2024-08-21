/*
 * Copyright 2023, Rockchip Electronics Co., Ltd
 * hisping lin, <hisping.lin@rock-chips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */
#include <stdlib.h>
#include <boot_rkimg.h>
#include <command.h>
#include <common.h>
#include <mmc.h>
#include <optee_include/OpteeClientLoadTa.h>

int is_uuid_equal(TEEC_UUID uuid1, TEEC_UUID uuid2)
{
	bool a, b, c;

	a = (uuid1.timeLow == uuid2.timeLow);
	b = (uuid1.timeMid == uuid2.timeMid);
	c = (uuid1.timeHiAndVersion == uuid2.timeHiAndVersion);
	if ((a & b & c) == 0) {
		return 0;
	} else {
		if (memcmp(uuid1.clockSeqAndNode,
			   uuid2.clockSeqAndNode, 8) == 0) {
			return 1;
		} else {
			return 0;
		}
	}
}

void tee_uuid_from_octets(TEEC_UUID *d, const uint8_t *s)
{
	d->timeLow = (s[0] << 24) | (s[1] << 16) | (s[2] << 8) | s[3];
	d->timeMid = (s[4] << 8) | s[5];
	d->timeHiAndVersion = (s[6] << 8) | s[7];
	memcpy(d->clockSeqAndNode, s + 8, sizeof(d->clockSeqAndNode));
}

static struct blk_desc *dev_desc;
static disk_partition_t part_info;
int search_ta(void *uuid_octets, void *ta, size_t *ta_size)
{
	char fname[255];
	char *format;
	unsigned long ret = 0;
	TEEC_UUID uuid;
	TEEC_UUID ta_uuid;
	uint8_t *userta;
	struct userta_header *header;
	struct userta_item *item;
	int ta_ver;
	int res;

	if (!uuid_octets || !ta_size) {
		printf("TEEC: wrong parameter to search_ta\n");
		return -1;
	}

	if (!dev_desc) {
		dev_desc = rockchip_get_bootdev();
		if (!dev_desc) {
			printf("TEEC: %s: Could not find device\n", __func__);
			return -1;
		}

		if (part_get_info_by_name(dev_desc,
					  "userta", &part_info) < 0) {
			dev_desc = NULL;
			printf("TEEC: Could not find userta partition\n");
			return -1;
		}
	}

#ifdef CONFIG_OPTEE_V1
	memcpy(&uuid, uuid_octets, sizeof(TEEC_UUID));
	ta_ver = 1;
	format = "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x.ta";
#endif

#ifdef CONFIG_OPTEE_V2
	tee_uuid_from_octets(&uuid, uuid_octets);
	ta_ver = 2;
	format = "%08x-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x.ta";
#endif

	snprintf(fname, 255,
			format,
			uuid.timeLow,
			uuid.timeMid,
			uuid.timeHiAndVersion,
			uuid.clockSeqAndNode[0],
			uuid.clockSeqAndNode[1],
			uuid.clockSeqAndNode[2],
			uuid.clockSeqAndNode[3],
			uuid.clockSeqAndNode[4],
			uuid.clockSeqAndNode[5],
			uuid.clockSeqAndNode[6],
			uuid.clockSeqAndNode[7]);

	printf("Attempt to load %s \n", fname);

    userta = (uint8_t *)memalign(CONFIG_SYS_CACHELINE_SIZE, part_info.size * part_info.blksz);
	if (!userta) {
		printf("TEEC: Malloc failed!\n");
		res = -1;
		goto exit;
	}

	ret = blk_dread(dev_desc, part_info.start, part_info.size, userta);
	if (ret != part_info.size) {
		printf("TEEC: blk_dread fail\n");
		res = -1;
		goto exit;
	}

	header = (struct userta_header *)userta;
	if (header->magic != 0x524B5441 || header->img_ver != 1) {
		printf("TEEC: userta_header format error! \n");
		res = -1;
		goto exit;
	}

	item = (struct userta_item *)(header + 1);

	for (int i = 0; i < header->ta_num; i++) {
		tee_uuid_from_octets(&ta_uuid, item->ta_uuid);
			snprintf(fname, 255,
			format,
			ta_uuid.timeLow,
			ta_uuid.timeMid,
			ta_uuid.timeHiAndVersion,
			ta_uuid.clockSeqAndNode[0],
			ta_uuid.clockSeqAndNode[1],
			ta_uuid.clockSeqAndNode[2],
			ta_uuid.clockSeqAndNode[3],
			ta_uuid.clockSeqAndNode[4],
			ta_uuid.clockSeqAndNode[5],
			ta_uuid.clockSeqAndNode[6],
			ta_uuid.clockSeqAndNode[7]);
		debug("search TA %s \n", fname);
		debug("item->ta_offset=0x%x item->ta_len=0x%x *ta_size=%zu\n",
				item->ta_offset, item->ta_len, *ta_size);

		if (is_uuid_equal(ta_uuid, uuid) && item->ta_ver == ta_ver) {
			if (item->ta_len <= *ta_size && ta)
				memcpy(ta, userta + item->ta_offset, item->ta_len);
			*ta_size = item->ta_len;
			res = TA_BINARY_FOUND;
			goto exit;
		} else {
			item++;
		}
	}
	res = TA_BINARY_NOT_FOUND;

exit:
	if (userta)
		free(userta);
	return res;
}
