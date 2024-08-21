/*
 * Copyright 2023, Rockchip Electronics Co., Ltd
 * callen.cai, <callen.cai@rock-chips.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef ID_ATTESTATION_H_
#define ID_ATTESTATION_H_

#include <common.h>
#include "attestation_key.h"
#define   IDAT_DEBUG     0
#if IDAT_DEBUG
    #define MSG(...) printf(__VA_ARGS__)
#else
    #define MSG(...) do {} while (0)
#endif

#define PROP_LEN   32

typedef struct AttestationIds_ {
	u8 brand[PROP_LEN];
	u8 device[PROP_LEN];
	u8 product[PROP_LEN];
	u8 serial[PROP_LEN];
	u8 imei[PROP_LEN];
	u8 second_imei[PROP_LEN];
	u8 meid[PROP_LEN];
	u8 manufacturer[PROP_LEN];
	u8 model[PROP_LEN];
} AttestationIds;

void printAttestationIds(const AttestationIds *ids);

/*
 * write id attestation to secure storage.
 * @received_data: received data from usb
 * @len: the size of received_data
 * @return ATAP_RESULT_OK if ok, or ATAP_RESULT_ERROR_* on error
 */
atap_result write_id_attestation_to_secure_storage(u8 *received_data, uint32_t len);
#endif	//ID_ATTESTATION_H_
