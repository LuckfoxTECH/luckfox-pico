// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <tee_internal_api.h>
#include "rktest_handle.h"
#include "../rk_public_api/rk_oem_otp_api.h"


TEE_Result handle_otp_read(void)
{
	TEE_Result res = TEE_SUCCESS;
	/*
	 * RK356x platform require the address and length of OTP must be
	 * an integral multiple of 2 integer(half word).
	 */
	uint32_t read_len = 2;
	uint32_t read_offset = 0;
	uint8_t *read_data;

	//The memory used to invoke internal TA MUST BE secure memory, instead of CA memory.
	read_data = TEE_Malloc(read_len, 0);
	if (!read_data) {
		EMSG("Malloc context memory FAILED!");
		return TEE_ERROR_OUT_OF_MEMORY;
	}
	res = rk_otp_read(read_offset, read_data, read_len);
	if (res != TEE_SUCCESS)
		EMSG("rk_otp_read failed with code 0x%x", res);
	else
		IMSG("rk_otp_read succes with data: 0x%x, 0x%x", *read_data,
		     *(read_data + 1));

	TEE_Free(read_data);
	return res;
}

TEE_Result handle_otp_write(void)
{
	TEE_Result res = TEE_SUCCESS;
	/*
	 * RK356x platform require the address and length of OTP must be
	 * an integral multiple of 2 integer(half word).
	 */
	uint32_t write_len = 2;
	uint8_t write_data[2] = {0xaa, 0xaa};
	uint32_t write_offset = 0;

	res = rk_otp_write(write_offset, write_data, write_len);
	if (res != TEE_SUCCESS) {
		EMSG("rk_otp_write failed with code 0x%x", res);
		return res;
	}
	IMSG("rk_otp_write succes with data: 0x%x, 0x%x", write_data[0], write_data[1]);
	return res;
}

TEE_Result handle_otp_size(void)
{
	TEE_Result res = TEE_SUCCESS;
	uint32_t otp_size = 0;

	res = rk_otp_size(&otp_size);
	if (res != TEE_SUCCESS) {
		EMSG("rk_otp_size failed with code 0x%x", res);
		return res;
	}
	IMSG("The oem otp size is %d byte.", otp_size);
	return res;
}
