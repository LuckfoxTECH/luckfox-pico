// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include <tee_api_defines_extensions.h>
#include "rktest_handle.h"

TEE_Result handle_storage_speed(uint32_t param_types, TEE_Param params[4])
{
	TEE_Result res = TEE_SUCCESS;
	TEE_ObjectHandle ob_handle;
	uint32_t flags = TEE_DATA_FLAG_ACCESS_READ |
			 TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_WRITE_META;
	uint8_t object_id_1[] = {0x05, 0x06, 0x07, 0x08};
	uint32_t count = 0;
	void *storage_buf;
	uint32_t storage_size, storage_type;
	TEE_Time time1;
	TEE_Time time2;
	uint32_t use_time;
	void *buf = NULL;

	ASSERT_PARAM_TYPES(TEE_PARAM_TYPES(TEE_PARAM_TYPE_MEMREF_INOUT,
		TEE_PARAM_TYPE_VALUE_INPUT, TEE_PARAM_TYPE_NONE, TEE_PARAM_TYPE_NONE));

	storage_buf = params[0].memref.buffer;
	storage_size = params[0].memref.size;
	storage_type = params[1].value.a;

	IMSG("Test %s storage speed, size = %d.",
	    (storage_type == TEE_STORAGE_PRIVATE_RPMB) ? "RPMB" : "REE",
	    storage_size);

	buf = TEE_Malloc(storage_size, 0);
	if (!buf)
		return TEE_ERROR_OUT_OF_MEMORY;

	TEE_MemMove(buf, storage_buf, storage_size);

	//Create
	TEE_GetSystemTime(&time1);
	res = TEE_CreatePersistentObject(storage_type, object_id_1,
					 sizeof(object_id_1), flags,
					 TEE_HANDLE_NULL, NULL, 0, &ob_handle);
	if (res != TEE_SUCCESS) {
		EMSG("CreatePersistentObject ERR: 0x%x.", res);
		goto out;
	}
	TEE_GetSystemTime(&time2);
	use_time = (time2.seconds - time1.seconds) * 1000 + time2.millis - time1.millis;
	IMSG("Test create OK. use time %d ms.", use_time);

	//Seek
	res = TEE_SeekObjectData(ob_handle, 0, TEE_DATA_SEEK_SET);
	if (res != TEE_SUCCESS) {
		EMSG("SeekObjectData ERR: 0x%x.", res);
		goto out;
	}

	//Write
	TEE_GetSystemTime(&time1);
	res = TEE_WriteObjectData(ob_handle, buf, storage_size);
	if (res != TEE_SUCCESS) {
		EMSG("WriteObjectData ERR: 0x%x.", res);
		goto out;
	}
	TEE_GetSystemTime(&time2);
	use_time = (time2.seconds - time1.seconds) * 1000 + time2.millis - time1.millis;
	IMSG("Test write OK. use time %d ms.", use_time);

	//Seek
	res = TEE_SeekObjectData(ob_handle, 0, TEE_DATA_SEEK_SET);
	if (res != TEE_SUCCESS) {
		EMSG("SeekObjectData ERR: 0x%x.", res);
		goto out;
	}

	//Read
	TEE_GetSystemTime(&time1);
	res = TEE_ReadObjectData(ob_handle, buf, storage_size, &count);
	if (res != TEE_SUCCESS || count != storage_size) {
		EMSG("ReadObjectData ERR: 0x%x.", res);
		goto out;
	}
	TEE_GetSystemTime(&time2);
	use_time = (time2.seconds - time1.seconds) * 1000 + time2.millis - time1.millis;
	IMSG("Test read OK. use time %d ms.", use_time);

out:
	//Close and delete
	TEE_GetSystemTime(&time1);
	TEE_CloseAndDeletePersistentObject(ob_handle);
	TEE_GetSystemTime(&time2);
	use_time = (time2.seconds - time1.seconds) * 1000 + time2.millis - time1.millis;
	IMSG("Delete. use time %d ms.", use_time);
	if (buf)
		TEE_Free(buf);
	return res;
}
