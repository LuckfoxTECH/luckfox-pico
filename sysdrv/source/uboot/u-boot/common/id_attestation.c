/*
 * Copyright 2023, Rockchip Electronics Co., Ltd
 * callen, <callen.cai@rock-chips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <malloc.h>
#include <keymaster.h>
#include "id_attestation.h"


#define ID_ATTESTATION_FILE "attestation_ids"
/* Maximum file name size.*/
#define STORAGE_ID_LENGTH_MAX  64

void printAttestationIds(const AttestationIds *ids)
{
	printf("AttestationIds:\n");
	printf("  brand:             %s\n", ids->brand);
	printf("  device:            %s\n", ids->device);
	printf("  product:           %s\n", ids->product);
	printf("  serial:            %s\n", ids->serial);
	printf("  imei:              %s\n", ids->imei);
	printf("  second_imei:       %s\n", ids->second_imei);
	printf("  meid:              %s\n", ids->meid);
	printf("  manufacturer:      %s\n", ids->manufacturer);
	printf("  model:             %s\n", ids->model);
}
uint32_t write_to_keymaster(u8 *filename, uint32_t filename_size,
			    u8 *data, uint32_t data_size);

/* read id attestation digest len */
uint32_t read_id_attestation_digest(const char *ids_digest_file, uint32_t *ids_digest_len)
{
	int len = sizeof(AttestationIds);
	u8 ids_digest[len];

	TEEC_Result ret = read_from_keymaster((u8 *)ids_digest_file, strlen(ids_digest_file),
					      (u8 *)ids_digest, len);
	if (ret != TEEC_SUCCESS)
		*ids_digest_len = 0;
	else
		*ids_digest_len = len;
	MSG("%s file:%s ,digest_len=%d,ret=%x\n", __func__, ids_digest_file, *ids_digest_len, ret);
	return ret;
}

uint32_t write_id_attestation(const char *ids_file, AttestationIds *ids, uint32_t ids_len)
{
	TEEC_Result ret = write_to_keymaster((u8 *)ids_file, strlen(ids_file),
					     (u8 *)ids, ids_len);
	MSG("%s ids_file=%s ret=%0x\n", __func__, ids_file, ret);
	return ret;
}

atap_result write_id_attestation_to_secure_storage(u8* received_data, uint32_t len)
{
	AttestationIds ids;
	u32 ids_len;
	AttestationIds ids_read;
	char ids_file[STORAGE_ID_LENGTH_MAX] = { 0 };

	ids_len = (received_data[5] << 8) | received_data[4];
	printf("%s size=%d\n", __func__, ids_len);
	if (ids_len != sizeof(AttestationIds)) {
		printf("%s AttestationIds size is %zu)\n", __func__, sizeof(AttestationIds));
		return ATAP_RESULT_ERROR_INVALID_HEAD;
	}
	memcpy(&ids, received_data + 8, len);
#if DEBUG
	printAttestationIds(&ids);
#endif
	/* now you have got the whole AttestationIds data....*/
	memcpy(ids_file, ID_ATTESTATION_FILE, sizeof(ID_ATTESTATION_FILE));
	TEEC_Result ret = read_from_keymaster((u8 *)ids_file,
					      strlen(ids_file),
					      (u8*)&ids_read,
					      sizeof(AttestationIds));
	MSG("read id attestation   ret=%0x\n", ret);
	if (ret == TEEC_SUCCESS) {
		printf("id attestation already exsit,you cannot update it!");
#if DEBUG
		printAttestationIds(&ids_read);
#endif
		ret = ATAP_RESULT_ERROR_ALREADY_EXSIT;
		return ret;
	}
	ret = write_id_attestation(ids_file, &ids, ids_len);
	printf("write id attestation : ret=%d\n", ret);
	return ret;
}
