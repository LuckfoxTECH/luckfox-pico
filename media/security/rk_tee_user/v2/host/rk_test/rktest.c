// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2021 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <compiler.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <rktest_ta.h>
#include <rktest.h>
#include <pta_secstor_ta_mgmt.h>
#include <sys/time.h>

#define STORAGE_UUID \
		{ 0x2d26d8a8, 0x5134, 0x4dd8, \
			{ 0xb3, 0x2f, 0xb3, 0x4b, 0xce, 0xeb, 0xc4, 0x71 } }
#define STORAGE_CMD_WRITE_OEM_NS_OTP		12
#define STORAGE_CMD_READ_OEM_NS_OTP		13

static void dump_hex(char *var_name __unused, const uint8_t *data __unused,
	      uint32_t len __unused)
{
	uint32_t i;

	printf("LINE:%d  %s:", __LINE__, var_name);
	for (i = 0; i < len; i++) {
		if ((i % 16) == 7)
			printf(" ");
		if ((i % 16) == 0)
			printf("\n");
		printf("0x%02x", data[i]);
	}
	printf("\n");
}

static TEEC_Result invoke_transfer_data(TEEC_Context *context,
					TEEC_Session *session,
					TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	TEEC_SharedMemory sm;
	uint8_t temref_input[50];
	const uint8_t transfer_inout[] = "Transfer data test.";

	memcpy(temref_input, transfer_inout, sizeof(transfer_inout));
	//Initialize the Shared Memory buffers
	sm.size = sizeof(temref_input);
	sm.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;

	res = TEEC_AllocateSharedMemory(context, &sm);
	if (res != TEEC_SUCCESS) {
		printf("AllocateSharedMemory ERR! res= 0x%x\n", res);
		return res;
	}

	memset(operation, 0, sizeof(TEEC_Operation));
	//Note: these parameters must correspond to operation.params[],
	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT,
						 TEEC_MEMREF_TEMP_INPUT,
						 TEEC_MEMREF_PARTIAL_OUTPUT, TEEC_NONE);
	operation->params[0].value.a = 66;
	//This buffer will be temporarily shared.
	operation->params[1].tmpref.size = sizeof(temref_input);
	operation->params[1].tmpref.buffer = (void *)temref_input;
	operation->params[2].memref.parent = &sm;
	operation->params[2].memref.offset = 0;
	operation->params[2].memref.size = sm.size;

	//Invoke to TA
	res = TEEC_InvokeCommand(session, RKTEST_TA_CMD_TRANSFER_DATA,
				 operation, error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! res= 0x%x\n", res);
		goto out;
	}

	//Check the result
	if (operation->params[0].value.a == 66 + 1 &&
	    operation->params[0].value.b == operation->params[0].value.a)
		printf("test value : Pass!\n");
	else
		printf("test value : Fail! (mismatch values)\n");

	//Check if the sm->buffer (params[2]) = TRANSFER_INOUT (params[1])
	if (memcmp(sm.buffer, transfer_inout, sizeof(transfer_inout)) == 0)
		printf("test buffer : Pass!\n");
	else
		printf("test buffer : Fail! (mismatch buffer)\n");

out:
	TEEC_ReleaseSharedMemory(&sm);
	return res;
};

static TEEC_Result invoke_storage(TEEC_Session *session,
				  TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_STORAGE,
				  operation, error_origin);
};

static TEEC_Result test_storage_speed(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin,
			uint32_t storage_type, uint32_t *storage_size, uint32_t times)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint8_t *temref_input = NULL;

	for (uint32_t i = 0; i < times; i++) {
		temref_input = malloc(storage_size[i]);
		memset(temref_input, 0xab, storage_size[i]);
		memset(operation, 0, sizeof(TEEC_Operation));

		//Note: these parameters must correspond to operation.params[],
		operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INOUT, TEEC_VALUE_INPUT,
			TEEC_NONE, TEEC_NONE);
		operation->params[0].tmpref.size = storage_size[i];
		operation->params[0].tmpref.buffer = (void *)temref_input;
		operation->params[1].value.a = storage_type;

		//Invoke to TA
		res = TEEC_InvokeCommand(session, RKTEST_TA_CMD_STORAGE_SPEED,
					operation, error_origin);

		if (temref_input)
			free(temref_input);

		if (res != TEEC_SUCCESS) {
			printf("InvokeCommand ERR! res= 0x%x\n", res);
			break;
		}
	}

	return res;
};

static TEEC_Result invoke_storage_speed(TEEC_Session *session,
			TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t rpmb_storage_size[] = {32, 1024, 4096, 30000};
	uint32_t ree_storage_size[] = {32, 1024, 4096, 30000, 60000};

	res = test_storage_speed(session, operation, error_origin, TEE_STORAGE_PRIVATE_RPMB,
		rpmb_storage_size, sizeof(rpmb_storage_size) / sizeof(rpmb_storage_size[0]));
	if (res != TEEC_SUCCESS)
		return res;

	res = test_storage_speed(session, operation, error_origin, TEE_STORAGE_PRIVATE_REE,
		ree_storage_size, sizeof(ree_storage_size) / sizeof(ree_storage_size[0]));
	return res;
}

static TEEC_Result invoke_property(TEEC_Session *session,
				   TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_PROPERTY,
				  operation, error_origin);
};

static TEEC_Result invoke_crypto_sha(TEEC_Session *session,
				     TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_CRYPTO_SHA,
				  operation, error_origin);
};

static TEEC_Result invoke_crypto_aes(TEEC_Session *session,
				     TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_CRYPTO_AES,
				  operation, error_origin);
};

static TEEC_Result invoke_crypto_rsa(TEEC_Session *session,
				     TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_CRYPTO_RSA,
				  operation, error_origin);
};

static TEEC_Result invoke_secstor_ta(TEEC_Session *session,
				     TEEC_Operation *operation, uint32_t *error_origin)
{
	FILE *f = NULL;
	void *buf = NULL;
	size_t s = 0;
	const char *ta_path_v2_and =
		"/vendor/lib/optee_armtz/1db57234-dacd-462d-9bb1-ae79de44e2a5.ta";
	const char *ta_path_v2_linux =
		"/lib/optee_armtz/1db57234-dacd-462d-9bb1-ae79de44e2a5.ta";
	TEEC_Result res = TEEC_ERROR_GENERIC;

	/* Read ta to the buf. */
	f = fopen(ta_path_v2_and, "rb");
	if (!f) {
		f = fopen(ta_path_v2_linux, "rb");
		if (!f) {
			printf("fopen(\"%s\") and (\"%s\") fail!", ta_path_v2_and, ta_path_v2_linux);
			goto out;
		}
	}
	if (fseek(f, 0, SEEK_END)) {
		printf("fseek fail!");
		goto out;
	}
	s = ftell(f);
	rewind(f);
	buf = malloc(s);
	if (!buf) {
		printf("malloc fail!");
		goto out;
	}
	if (fread(buf, 1, s, f) != s) {
		printf("fread fail!");
		goto out;
	}
	fclose(f);

	memset(operation, 0, sizeof(TEEC_Operation));
	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT, TEEC_NONE,
						 TEEC_NONE, TEEC_NONE);
	operation->params[0].tmpref.buffer = buf;
	operation->params[0].tmpref.size = s;

	/*
	 * The data will be sent to the built-in TA(secstor_ta_mgmt) for processing,
	 * such as verification, encryption, secure storage, etc.
	 */
	res = TEEC_InvokeCommand(session, PTA_SECSTOR_TA_MGMT_BOOTSTRAP, operation,
				 error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! res= 0x%x\n", res);
		goto out;
	}

out:
	printf("Installing TAs done\n");
	if (buf)
		free(buf);
	return res;
};

static TEEC_Result invoke_otp_read(TEEC_Session *session,
				   TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_OEM_OTP_READ,
				  operation, error_origin);
};

static TEEC_Result invoke_otp_write(TEEC_Session *session,
				    TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_OEM_OTP_WRITE,
				  operation, error_origin);
};

static TEEC_Result invoke_otp_size(TEEC_Session *session,
				    TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_OEM_OTP_SIZE,
				  operation, error_origin);
};

static TEEC_Result invoke_otp_ns_read(TEEC_Session *session,
				      TEEC_Operation *operation,
				      uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t offset = 0;
	uint8_t read_data[8];

	memset(read_data, 0, sizeof(read_data));
	memset(operation, 0, sizeof(TEEC_Operation));
	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						 TEEC_MEMREF_TEMP_OUTPUT,
						 TEEC_NONE, TEEC_NONE);
	operation->params[0].value.a = offset;
	operation->params[1].tmpref.size = sizeof(read_data);
	operation->params[1].tmpref.buffer = (void *)read_data;

	res = TEEC_InvokeCommand(session, STORAGE_CMD_READ_OEM_NS_OTP,
				 operation, error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! res= 0x%x\n", res);
		return res;
	}

	dump_hex("otp_ns_read data", read_data, sizeof(read_data));
	printf("test otp_ns_read : Pass!\n");
	return res;
};

static TEEC_Result invoke_otp_ns_write(TEEC_Session *session,
				       TEEC_Operation *operation,
				       uint32_t *error_origin)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t offset = 0;
	uint8_t write_data = 0x01;

	memset(operation, 0, sizeof(TEEC_Operation));
	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INPUT,
						 TEEC_MEMREF_TEMP_INPUT,
						 TEEC_NONE, TEEC_NONE);
	operation->params[0].value.a = offset;
	operation->params[1].tmpref.size = sizeof(write_data);
	operation->params[1].tmpref.buffer = (void *)&write_data;

	res = TEEC_InvokeCommand(session, STORAGE_CMD_WRITE_OEM_NS_OTP,
				 operation, error_origin);
	if (res != TEEC_SUCCESS) {
		printf("InvokeCommand ERR! res= 0x%x\n", res);
		return res;
	}

	printf("test otp_ns_write : Pass!\n");
	return res;
};

static TEEC_Result invoke_trng_read(TEEC_Session *session,
				    TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_TRNG_READ,
				  operation, error_origin);
}

static void *server_init(void *arg __unused)
{
	int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(2345);

	bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	listen(serv_sock, 10);

	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size = sizeof(clnt_addr);
	int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);

	char str[] = "I am data from server!";
	write(clnt_sock, str, sizeof(str));

	char buffer[64];
	read(clnt_sock, buffer, sizeof(buffer));
	printf("Message from TA: %s\n", buffer);

	read(clnt_sock, buffer, 1);

	close(clnt_sock);
	close(serv_sock);
	return NULL;
}

static TEEC_Result invoke_socket(TEEC_Session *session,
				 TEEC_Operation *operation, uint32_t *error_origin)
{
	pthread_t server;
	pthread_create(&server, 0, server_init, NULL);

	TEEC_Result res;
	res = TEEC_InvokeCommand(session, RKTEST_TA_CMD_SOCKET,
				 operation, error_origin);
	pthread_join(server, NULL);
	return res;
}

static TEEC_Result invoke_crypto_hw(TEEC_Context *context, TEEC_Session *session,
				     TEEC_Operation *operation, uint32_t *error_origin)
{
	TEEC_Result res;
	TEEC_SharedMemory sm;
	uint8_t data[128 * 1024];
	uint8_t check_data[128 * 1024];

	memset(data, 0xab, sizeof(data));
	memset(check_data, 0xab, sizeof(check_data));
	memset(operation, 0, sizeof(TEEC_Operation));

	operation->paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INOUT,
						 TEEC_NONE,
						 TEEC_NONE,
						 TEEC_NONE);

	sm.size = sizeof(data);
	sm.flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	res = TEEC_AllocateSharedMemory(context, &sm);
	if (res != TEEC_SUCCESS) {
		printf("AllocateSharedMemory ERR! TEEC res= 0x%x", res);
		return res;
	}
	memcpy(sm.buffer, data, sm.size);
	operation->params[0].memref.parent = &sm;
	operation->params[0].memref.offset = 0;
	operation->params[0].memref.size   = sm.size;

	struct timeval time;
	gettimeofday(&time, NULL);
	res = TEEC_InvokeCommand(session, RKTEST_TA_CMD_CRYPTO_HW,
				  operation, error_origin);
	struct timeval time2;
	gettimeofday(&time2, NULL);
	printf("invoke_crypto_hw use time: %ld us \n",
	       (time2.tv_sec - time.tv_sec) * 1000000 + (time2.tv_usec - time.tv_usec));

	if (res == TEEC_SUCCESS) {
		if (memcmp(data, check_data, sizeof(data)) == 0) {
			printf("hardware crypto success!\n");
		} else {
			printf("hardware crypto fail!\n");
		}
	}
	return res;
};

static TEEC_Result invoke_derive_key(TEEC_Session *session,
				 TEEC_Operation *operation, uint32_t *error_origin)
{
	return TEEC_InvokeCommand(session, RKTEST_TA_CMD_DERIVE_KEY,
				  operation, error_origin);
}

TEEC_Result rk_test(uint32_t invoke_command)
{
	TEEC_Result res = TEEC_SUCCESS;
	uint32_t error_origin = 0;
	TEEC_Context contex;
	TEEC_Session session;
	TEEC_Operation operation;
	const TEEC_UUID secstor_uuid = PTA_SECSTOR_TA_MGMT_UUID;
	const TEEC_UUID rktest_uuid = RKTEST_TA_UUID;
	const TEEC_UUID storage_uuid = STORAGE_UUID;
	const TEEC_UUID *uuid;

	switch (invoke_command) {
	case SECSTOR_TA:
		/*
		 * Call the built-in TA(secstor_ta_mgmt) to handle SECSTOR.
		 */
		uuid = &secstor_uuid;
		break;
	case OTP_NS_READ:
	case OTP_NS_WRITE:
		/*
		 * Call the built-in TA(uboot_storedata_otp.ta) to handle NS OTP.
		 */
		uuid = &storage_uuid;
		break;
	default:
		uuid = &rktest_uuid;
		break;
	}

	//[1] Connect to TEE
	res = TEEC_InitializeContext(NULL, &contex);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_InitializeContext failed with code 0x%x\n", res);
		return res;
	}

	//[2] Open session with TEE application
	res = TEEC_OpenSession(&contex, &session, uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &error_origin);
	if (res != TEEC_SUCCESS) {
		printf("TEEC_Opensession failed with code 0x%x origin 0x%x\n",
		       res, error_origin);
		goto out;
	}

	//[3] Perform operation initialization
	memset(&operation, 0, sizeof(TEEC_Operation));
	operation.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE,
						TEEC_NONE, TEEC_NONE);

	//[4] Start invoke command to the TEE application.
	switch (invoke_command) {
	case  TRANSFER_DATA:
		res = invoke_transfer_data(&contex, &session, &operation, &error_origin);
		break;
	case  STORAGE:
		res = invoke_storage(&session, &operation, &error_origin);
		break;
	case  STORAGE_SPEED:
		res = invoke_storage_speed(&session, &operation, &error_origin);
		break;
	case  PROPERTY:
		res = invoke_property(&session, &operation, &error_origin);
		break;
	case  CRYPTO_SHA:
		res = invoke_crypto_sha(&session, &operation, &error_origin);
		break;
	case  CRYPTO_AES:
		res = invoke_crypto_aes(&session, &operation, &error_origin);
		break;
	case  CRYPTO_RSA:
		res = invoke_crypto_rsa(&session, &operation, &error_origin);
		break;
	case  SECSTOR_TA:
		res = invoke_secstor_ta(&session, &operation, &error_origin);
		break;
	case  OTP_READ:
		res = invoke_otp_read(&session, &operation, &error_origin);
		break;
	case  OTP_WRITE:
		res = invoke_otp_write(&session, &operation, &error_origin);
		break;
	case  OTP_SIZE:
		res = invoke_otp_size(&session, &operation, &error_origin);
		break;
	case  OTP_NS_READ:
		res = invoke_otp_ns_read(&session, &operation, &error_origin);
		break;
	case  OTP_NS_WRITE:
		res = invoke_otp_ns_write(&session, &operation, &error_origin);
		break;
	case  TRNG_READ:
		res = invoke_trng_read(&session, &operation, &error_origin);
		break;
	case  SOCKET:
		res = invoke_socket(&session, &operation, &error_origin);
		break;
	case  CRYPTO_HW:
		res = invoke_crypto_hw(&contex, &session, &operation, &error_origin);
		break;
	case  DERIVE_KEY:
		res = invoke_derive_key(&session, &operation, &error_origin);
		break;
	default:
		printf("Doing nothing.\n");
		break;
	}
	if (res != TEEC_SUCCESS) {
		printf("Test ERR. res 0x%x origin 0x%x\n", res, error_origin);
		goto out1;
	}
	printf("Test OK.\n");
	//[5] Tidyup resources
out1:
	TEEC_CloseSession(&session);
out:
	TEEC_FinalizeContext(&contex);
	return res;
}

