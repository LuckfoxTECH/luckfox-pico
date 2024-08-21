// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>
#include <tee_api_defines.h>
#include "rktest_handle.h"


TEE_Result handle_storage(void)
{
	TEE_Result res = TEE_SUCCESS;
	TEE_ObjectHandle ob_handle;
	uint32_t flags = TEE_DATA_FLAG_ACCESS_READ |
			 TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_ACCESS_WRITE_META;
	uint8_t object_id_1[] = {0x01, 0x02, 0x03, 0x04};
	uint8_t w_data[] = "Read and Write test.";
	uint8_t r_data[32] = {'\0'};
	uint32_t count = 0;

	/*
	 * The storageID parameter of TEE_CreatePersistentObject can be:
	 * TEE_STORAGE_PRIVATE - Same as TEE_STORAGE_PRIVATE_REE. See below.
	 * TEE_STORAGE_PRIVATE_REE - Using the REE file system. Data will be
	 *preferentially stored in the 'security' partition if the
	 *	'security' partition is configured to be available. If the
	 *	'security' partition is not available, the data will be stored
	 *	in the Linux or Android file system.
	 * TEE_STORAGE_PRIVATE_RPMB -  Using the RPMB file system. Data will be
	 *	stored in the 'RPMB' partition in eMMC. You should make sure
	 *	first that the device supports eMMC.
	 */
	//Create
	res = TEE_CreatePersistentObject(TEE_STORAGE_PRIVATE, object_id_1,
					 sizeof(object_id_1), flags,
					 TEE_HANDLE_NULL, NULL, 0, &ob_handle);
	if (res != TEE_SUCCESS) {
		EMSG("CreatePersistentObject ERR: 0x%x.", res);
		return res;
	}
	IMSG("Test create OK.");

	//You can use the objectHandle now. Or close the object.
	//Write
	res = TEE_SeekObjectData(ob_handle, 0, TEE_DATA_SEEK_SET);
	if (res != TEE_SUCCESS) {
		EMSG("SeekObjectData ERR: 0x%x.", res);
		goto out;
	}

	res = TEE_WriteObjectData(ob_handle, w_data, sizeof(w_data));
	if (res != TEE_SUCCESS) {
		EMSG("WriteObjectData ERR: 0x%x.", res);
		goto out;
	}
	IMSG("Test write OK.");

	//Close and open
	TEE_CloseObject(ob_handle);

	res = TEE_OpenPersistentObject(TEE_STORAGE_PRIVATE, object_id_1,
				       sizeof(object_id_1), flags, &ob_handle);
	if (res != TEE_SUCCESS) {
		EMSG("OpenPersistentObject ERR: 0x%x.", res);
		goto out;
	}

	//Read
	res = TEE_ReadObjectData(ob_handle, r_data, sizeof(w_data), &count);
	if (res != TEE_SUCCESS) {
		EMSG("ReadObjectData ERR: 0x%x.", res);
		goto out;
	}
	IMSG("Test read OK.");

	//Compare the data
	if (TEE_MemCompare(r_data, w_data, sizeof(w_data))) {
		EMSG("Read data != write data. ERR!");
		goto out;
	}
	IMSG("Read data = write data.");

out:
	//Close and delete
	TEE_CloseAndDeletePersistentObject(ob_handle);
	IMSG("Delete.");
	return res;
}

