// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright (c) 2016, Linaro Limited
 * All rights reserved.
 */

#include <err.h>
#include <fcntl.h>
#include <pta_invoke_tests.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <tee_client_api.h>
#include <tee_client_api_extensions.h>
#include <unistd.h>

#include "crypto_common.h"
#include "sdp_basic.h"
#include "xtest_helpers.h"
#include "xtest_test.h"

#include "include/uapi/linux/ion_old.h"

/*
 * SDP basic test setup overview.
 *
 * - A dedicated trusted application (SDP basic TA) supports 3 commands:
 *   - 'inject' data from a nonsecure buffer into a secure buffer
 *   - 'transform' data inside a secure buffer (bitwise invert + unsigned incr)
 *   - 'dump' data from a secure buffer into a nonsecure buffer

 * - This test client application (CA) invokes the TA for these 3 operations,
 *   inject random value, trasforming them then dump them.
 *
 * To do so, CA allocates a 'SDP secure buffer' and invoke the TA for these 3
 * operations (inject then transform then dump) over the allocate buffer.
 *
 * The secure buffer is currently allocation through ION support adn
 * registered to OP-TEE and as shared memory.
 *
 * To enhance test coverage against buffer alignement usecase, the CA invokes
 * the TA with a variable offset inside the buffer. As CA injects random data
 * into the buffer, the CA uses one of the random bytes to set the value of the
 * offset in the accessed secure buffer.
 *
 * For debugging support, the CA may map (in nonsecure world) the secure
 * buffer to read its content. As this is unsafe on a hardened platform, this
 * operation is default disable. When enable, error only print out a warning
 * trace but does not actually fail the test. This also give an easy way to
 * check that some HW complains on access violation when nonsecure accesses
 * secure data.
 */

struct tee_ctx {
	TEEC_Context ctx;
	TEEC_Session sess;
};

/*
 * Old ION API to allocate and export a buffer
 */
static int allocate_ion_buffer_old_api(size_t size, int heap_type_id, int ion)
{
	struct ion0_allocation_data alloc_data = { };
	struct ion0_handle_data hdl_data = { };
	struct ion0_fd_data fd_data = { };
	int fd = -1;

	alloc_data.len = size;
	alloc_data.align = 0;
	alloc_data.flags = 0;
	alloc_data.heap_id_mask = 1 << heap_type_id;
	if (ioctl(ion, ION0_IOC_ALLOC, &alloc_data) == -1) {
		fprintf(stderr, "Error: old ION allocate API failed\n");
		return fd;
	}

	fd_data.handle = alloc_data.handle;
	if (ioctl(ion, ION0_IOC_SHARE, &fd_data) != -1)
		fd = fd_data.fd;
	else
		fprintf(stderr, "Error: old ION share API failed\n");

	hdl_data.handle = alloc_data.handle;
	(void)ioctl(ion, ION0_IOC_FREE, &hdl_data);

	return fd;
}

int allocate_ion_buffer(size_t size, int heap_type_id, int verbosity)
{
	struct ion_heap_query query_data = { };
	struct ion_heap_data heap_data[32] = { };
	struct ion_allocation_data alloc_data = { };
	int ion = 0;
	int fd = -1;
	unsigned int idx = 0;

	ion = open("/dev/ion", O_RDWR);
	if (ion < 0) {
		fprintf(stderr, "Error: failed to open /dev/ion\n");
		verbose("Seems no ION heap is available.\n");
		verbose("To test ION allocation you can enable\n");
		verbose("CONFIG_ION and CONFIG_ION_DUMMY in your\n");
		verbose("linux kernel configuration.\n");
		return fd;
	}

	if (heap_type_id < 0)
		heap_type_id = DEFAULT_ION_HEAP_TYPE;

	if (ioctl(ion, ION_IOC_HEAP_QUERY, &query_data) < 0) {
		fprintf(stderr, "Error: failed to query the number of heaps\n");
		goto out;
	}

	query_data.heaps = (__u64)(unsigned long)&heap_data;
	if (ioctl(ion, ION_IOC_HEAP_QUERY, &query_data) < 0) {
		fprintf(stderr, "Info: can't query heaps data, try old API\n");
		fd = allocate_ion_buffer_old_api(size, heap_type_id, ion);
		goto out;
	}

	for (idx = 0; idx < query_data.cnt; idx++)
		if (heap_data[idx].type == (unsigned int)heap_type_id)
			break;
	if (idx == query_data.cnt) {
		fprintf(stderr, "Error: target heap type %d not found\n",
				heap_type_id);
		goto out;
	}

	verbose("Allocate in ION heap '%s' (type=%u, id=%u)\n",
		heap_data[idx].name, heap_data[idx].type,
		heap_data[idx].heap_id);

	alloc_data.len = size;
	alloc_data.flags = 0;
	alloc_data.heap_id_mask = 1 << heap_data[idx].heap_id;
	if (ioctl(ion, ION_IOC_ALLOC, &alloc_data) < 0) {
		fprintf(stderr, "Error: failed to allocate in target heap\n");
		goto out;
	}

	fd = alloc_data.fd;
out:
	close(ion);
	return fd;
}

static void finalize_tee_ctx(struct tee_ctx *ctx)
{
	if (!ctx)
		return;

	TEEC_CloseSession(&ctx->sess);
	TEEC_FinalizeContext(&ctx->ctx);
}

static int create_tee_ctx(struct tee_ctx *ctx, enum test_target_ta target_ta)
{
	TEEC_Result teerc = TEEC_ERROR_GENERIC;
	const TEEC_UUID *uuid = NULL;
	uint32_t err_origin = 0;

	switch (target_ta) {
	case TEST_NS_TO_TA:
	case TEST_TA_TO_TA:
	case TEST_TA_TO_PTA:
		uuid = &sdp_basic_ta_uuid;
		break;
	case TEST_NS_TO_PTA:
		uuid = &pta_invoke_tests_ta_uuid;
		break;
	default:
		return -1;
	}

	teerc = TEEC_InitializeContext(NULL, &ctx->ctx);
	if (teerc != TEEC_SUCCESS)
		return -1;

	teerc = TEEC_OpenSession(&ctx->ctx, &ctx->sess, uuid,
			       TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
	if (teerc != TEEC_SUCCESS) {
		fprintf(stderr, "Error: open session to target test %s failed %x %d\n",
			(target_ta == TEST_NS_TO_PTA) ? "pTA" : "TA",
			teerc, err_origin);

		TEEC_FinalizeContext(&ctx->ctx);
	}
	return (teerc == TEEC_SUCCESS) ? 0 : -1;
}

static int tee_register_buffer(struct tee_ctx *ctx, void **shm_ref, int fd)
{
	TEEC_Result teerc = TEEC_ERROR_GENERIC;
	TEEC_SharedMemory *shm = malloc(sizeof(*shm));

	if (!shm)
		return 1;

	shm->flags = TEEC_MEM_INPUT | TEEC_MEM_OUTPUT;
	teerc = TEEC_RegisterSharedMemoryFileDescriptor(&ctx->ctx, shm, fd);
	if (teerc != TEEC_SUCCESS) {
		fprintf(stderr, "Error: TEEC_RegisterMemoryFileDescriptor() failed %x\n",
			teerc);
		return 1;
	}

	*shm_ref = shm;
	return 0;
}

static void tee_deregister_buffer(struct tee_ctx *ctx, void *shm_ref)
{
	(void)ctx;

	if (!shm_ref)
		return;

	TEEC_ReleaseSharedMemory((TEEC_SharedMemory *)shm_ref);
	free(shm_ref);
}

static int inject_sdp_data(struct tee_ctx *ctx,
		    void *in, size_t offset, size_t len, void *shm_ref, int ind)
{
	TEEC_SharedMemory *shm = (TEEC_SharedMemory *)shm_ref;
	TEEC_Result teerc = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t err_origin = 0;
	unsigned int cmd = 0;

	switch (ind) {
	case TEST_NS_TO_TA:
		cmd = TA_SDP_BASIC_CMD_INJECT;
		break;
	case TEST_TA_TO_TA:
		cmd = TA_SDP_BASIC_CMD_INVOKE_INJECT;
		break;
	case TEST_TA_TO_PTA:
		cmd = TA_SDP_BASIC_CMD_PTA_INJECT;
		break;
	case TEST_NS_TO_PTA:
		cmd = PTA_INVOKE_TESTS_CMD_COPY_NSEC_TO_SEC;
		break;
	default:
		return -1;
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_INPUT,
					 TEEC_MEMREF_PARTIAL_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].tmpref.buffer = in;
	op.params[0].tmpref.size = len;

	op.params[1].memref.parent = shm;
	op.params[1].memref.size = len;
	op.params[1].memref.offset = offset;

	teerc = TEEC_InvokeCommand(&ctx->sess, cmd, &op, &err_origin);
	if (teerc != TEEC_SUCCESS)
		fprintf(stderr, "Error: invoke SDP test TA (inject) failed %x %d\n",
			teerc, err_origin);

	return (teerc == TEEC_SUCCESS) ? 0 : -1;
}

static int transform_sdp_data(struct tee_ctx *ctx,
			size_t offset, size_t len, void *shm_ref, int ind)
{
	TEEC_SharedMemory *shm = (TEEC_SharedMemory *)shm_ref;
	TEEC_Result teerc = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t err_origin = 0;
	unsigned int cmd = 0;

	switch (ind) {
	case TEST_NS_TO_TA:
		cmd = TA_SDP_BASIC_CMD_TRANSFORM;
		break;
	case TEST_TA_TO_TA:
		cmd = TA_SDP_BASIC_CMD_INVOKE_TRANSFORM;
		break;
	case TEST_TA_TO_PTA:
		cmd = TA_SDP_BASIC_CMD_PTA_TRANSFORM;
		break;
	case TEST_NS_TO_PTA:
		cmd = PTA_INVOKE_TESTS_CMD_READ_MODIFY_SEC;
		break;
	default:
		return -1;
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INOUT,
					 TEEC_NONE, TEEC_NONE, TEEC_NONE);
	op.params[0].memref.parent = shm;
	op.params[0].memref.size = len;
	op.params[0].memref.offset = offset;

	teerc = TEEC_InvokeCommand(&ctx->sess, cmd, &op, &err_origin);
	if (teerc != TEEC_SUCCESS)
		fprintf(stderr, "Error: invoke SDP test TA (transform) failed %x %d\n",
			teerc, err_origin);

	return (teerc == TEEC_SUCCESS) ? 0 : -1;
}

static int dump_sdp_data(struct tee_ctx *ctx,
		  void *out, size_t offset, size_t len, void *shm_ref, int ind)
{
	TEEC_SharedMemory *shm = (TEEC_SharedMemory *)shm_ref;
	TEEC_Result teerc = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t err_origin = 0;
	unsigned int cmd = 0;

	switch (ind) {
	case TEST_NS_TO_TA:
		cmd = TA_SDP_BASIC_CMD_DUMP;
		break;
	case TEST_TA_TO_TA:
		cmd = TA_SDP_BASIC_CMD_INVOKE_DUMP;
		break;
	case TEST_TA_TO_PTA:
		cmd = TA_SDP_BASIC_CMD_PTA_DUMP;
		break;
	case TEST_NS_TO_PTA:
		cmd = PTA_INVOKE_TESTS_CMD_COPY_SEC_TO_NSEC;
		break;
	default:
		return -1;
	}

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_MEMREF_TEMP_OUTPUT,
					 TEEC_NONE, TEEC_NONE);
	op.params[0].memref.parent = shm;
	op.params[0].memref.size = len;
	op.params[0].memref.offset = offset;

	op.params[1].tmpref.buffer = out;
	op.params[1].tmpref.size = len;

	teerc = TEEC_InvokeCommand(&ctx->sess, cmd, &op, &err_origin);
	if (teerc != TEEC_SUCCESS)
		fprintf(stderr, "Error: invoke SDP test TA (dump) failed %x %d\n",
			teerc, err_origin);

	return (teerc == TEEC_SUCCESS) ? 0 : -1;
}

static int check_sdp_dumped(struct tee_ctx *ctx, void *ref, size_t len,
								void *out)
{
	char *bref = (char *)ref;
	char *data = (char *)out;
	int err = 0;

	(void)ctx;

	while(len--)
		if (*data++ != (unsigned char)(~(*bref++) + 1))
			err++;

	return err;
}

/*
 * Consider 32kByte + 1 of random data is sufficient for an accurate test
 * whatever the test buffer size is. Random buffer is read as a ring buffer.
 */
#define RANDOM_BUFFER_SIZE	(32 * 1024 + 1)
static int get_random_bytes(char *out, size_t len)
{
	static char *rand_buf = NULL;
	static size_t rand_idx = 0;
	int rc = 0;

	if (!rand_buf) {
		const char rand_dev[] = "/dev/urandom";
		int fd = 0;

		rand_buf = malloc(RANDOM_BUFFER_SIZE);
		if (!rand_buf) {
			fprintf(stderr, "failed to random buffer memory (%d bytes)\n",
				RANDOM_BUFFER_SIZE);
			return -1;
		}

		fd = open(rand_dev, O_RDONLY);
		if (fd < 0) {
			fprintf(stderr, "failed to open %s\n", rand_dev);
			return -1;
		}

		rc = read(fd, rand_buf, RANDOM_BUFFER_SIZE);
		if (rc != RANDOM_BUFFER_SIZE) {
			fprintf(stderr, "failed to read %d bytes from %s\n",
				RANDOM_BUFFER_SIZE, rand_dev);
			return -1;
		}
		close(fd);
	}

	while (len) {
		size_t t_len = (RANDOM_BUFFER_SIZE < len) ? RANDOM_BUFFER_SIZE : len;

		if ((rand_idx + t_len) > RANDOM_BUFFER_SIZE) {
			int sz_end = RANDOM_BUFFER_SIZE - rand_idx;
			int sz_beg = t_len - sz_end;

			memcpy(out, rand_buf + rand_idx, sz_end);
			memcpy(out + sz_end, rand_buf , sz_beg);
			rand_idx = sz_beg;
		} else {
			memcpy(out, rand_buf + rand_idx, t_len);
			rand_idx += t_len;
		}
		len -= t_len;
	}
	return 0;
}


int sdp_basic_test(enum test_target_ta ta, size_t size, size_t loop,
		   int ion_heap, int rnd_offset, int verbosity)
{
	struct tee_ctx *ctx = NULL;
	unsigned char *test_buf = NULL;
	unsigned char *ref_buf = NULL;
	void *shm_ref = NULL;
	unsigned int err = 1;
	int fd = -1;
	size_t sdp_size = size;
	size_t offset = 0;
	size_t loop_cnt = 0;

	if (!loop) {
		fprintf(stderr, "Error: null loop value\n");
		return 1;
	}

	/* reduce size to enable offset tests (max offset is 255 bytes) */
	if (rnd_offset)
		size -= 255;

	test_buf = malloc(size);
	ref_buf = malloc(size);
	if (!test_buf || !ref_buf) {
		verbose("failed to allocate memory\n");
		goto bail1;
	}

	fd = allocate_ion_buffer(sdp_size, ion_heap, verbosity);
	if (fd < 0) {
		verbose("Failed to allocate SDP buffer (%zu bytes) in ION heap %d: %d\n",
				sdp_size, ion_heap, fd);
		goto bail1;
	}

	/* register secure buffer to TEE */
	ctx = malloc(sizeof(*ctx));
	if (!ctx)
		goto bail1;
	if (create_tee_ctx(ctx, ta))
		goto bail1;
	if (tee_register_buffer(ctx, &shm_ref, fd))
		goto bail2;

	/* release registered fd: tee should still hold refcount on resource */
	close(fd);
	fd = -1;

	/* invoke trusted application with secure buffer as memref parameter */
	for (loop_cnt = loop; loop_cnt; loop_cnt--) {
		/* get an buffer of random-like values */
		if (get_random_bytes((char *)ref_buf, size))
			goto bail2;
		memcpy(test_buf, ref_buf, size);
		/* random offset [0 255] */
		offset = (unsigned int)*ref_buf;

		/* TA writes into SDP buffer */
		if (inject_sdp_data(ctx, test_buf, offset, size, shm_ref, ta))
			goto bail2;

		/* TA reads/writes into SDP buffer */
		if (transform_sdp_data(ctx, offset, size, shm_ref, ta))
			goto bail2;

		/* TA reads into SDP buffer */
		if (dump_sdp_data(ctx, test_buf, offset, size, shm_ref, ta))
			goto bail2;

		/* check dumped data are the expected ones */
		if (check_sdp_dumped(ctx, ref_buf, size, test_buf)) {
			fprintf(stderr, "check SDP data: %d errors\n", err);
			goto bail2;
		}
	}

	err = 0;
bail2:
	if (fd >= 0)
		close(fd);
	if (shm_ref)
		tee_deregister_buffer(ctx, shm_ref);
	finalize_tee_ctx(ctx);
bail1:
	free(ctx);
	free(ref_buf);
	free(test_buf);
	return err;
}

static int invoke_out_of_bounds(struct tee_ctx *ctx,
				TEEC_SharedMemory *in, TEEC_SharedMemory *out,
				size_t offset, size_t size,
				bool valid_ref, int verbosity)
{
	TEEC_Result teerc = TEEC_ERROR_GENERIC;
	TEEC_Operation op = TEEC_OPERATION_INITIALIZER;
	uint32_t orig = 0;

	op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_PARTIAL_INPUT,
					 TEEC_MEMREF_PARTIAL_OUTPUT,
					 TEEC_NONE, TEEC_NONE);

	op.params[0].memref.parent = in;
	op.params[0].memref.offset = 0;
	op.params[0].memref.size = size;

	op.params[1].memref.parent = out;
	op.params[1].memref.offset = offset;
	op.params[1].memref.size = size;

	teerc = TEEC_InvokeCommand(&ctx->sess, TA_SDP_BASIC_CMD_INJECT,
				   &op, &orig);

	/*
	 * Invocation with invalid references should be nicely rejected by
	 * the TEE.
	 * Invocation with valid references should reach the TA, whatever
	 * result is.
	 */
	if ((valid_ref && orig != TEEC_ORIGIN_TRUSTED_APP) ||
	    (!valid_ref && ((orig == TEEC_ORIGIN_TRUSTED_APP) ||
			    (teerc != TEEC_ERROR_GENERIC &&
			     teerc != TEEC_ERROR_BAD_PARAMETERS))))
		goto error;

	verbose("Out of bounds memref test successful:\n");
	verbose("Shm size 0x%zx, offset 0x%zx/size 0x%zx: %s/0x%x from %s\n",
		out->size, offset, size,
		Do_ADBG_GetEnumName(teerc, ADBG_EnumTable_TEEC_Result), teerc,
		Do_ADBG_GetEnumName(orig, ADBG_EnumTable_TEEC_ErrorOrigin));
	return 0;

error:
	fprintf(stderr, "Out of bounds memref test FAILURE:\n");
	fprintf(stderr,
		"Shm size 0x%zx, offset 0x%zx/size 0x%zx: %s/0x%x from %s\n",
		out->size, offset, size,
		Do_ADBG_GetEnumName(teerc, ADBG_EnumTable_TEEC_Result),	teerc,
		Do_ADBG_GetEnumName(orig, ADBG_EnumTable_TEEC_ErrorOrigin));
	return 1;
}

int sdp_out_of_bounds_memref_test(size_t size, int ion_heap, int verbosity)
{
	struct tee_ctx ctx = { };
	int err = 0;
	int fd = -1;
	TEEC_Result teerc = TEEC_ERROR_GENERIC;
	TEEC_SharedMemory in = { };
	TEEC_SharedMemory *out = NULL;

	if (create_tee_ctx(&ctx, TEST_NS_TO_TA))
		return -1;

	fd = allocate_ion_buffer(size, ion_heap, verbosity);
	if (fd < 0) {
		verbose("SDP alloc failed (%zu bytes) in ION heap %d: %d\n",
			size, ion_heap, fd);
		err = 1;
		goto bail;
	}
	if (tee_register_buffer(&ctx, (void **)&out, fd)) {
		err = 1;
		goto bail;
	}

	/*
	 * The ION driver will decide how much SDP memory is being allocated.
	 * Rely on this size to test out of bounds reference cases.
	 */
	size = out->size;

	in.size = size;
	in.flags = TEEC_MEM_INPUT;
	teerc = TEEC_AllocateSharedMemory(&ctx.ctx, &in);
	if (teerc) {
		verbose("failed to allocate memory\n");
		goto bail;
	}

	if (verbosity) {
		/* Valid case: reference inside allocated buffer: last byte */
		err += invoke_out_of_bounds(&ctx, &in, out, size - 1, 1,
					    true, verbosity);
	}

	/* Reference overflows allocated buffer by 1 byte */
	err += invoke_out_of_bounds(&ctx, &in, out, size - 1, 2,
				    false, verbosity);

	/* Reference oveflows allocated buffer by more than 4kB byte */
	err += invoke_out_of_bounds(&ctx, &in, out, size - 1, 5000,
				    false, verbosity);

	/* Offset exceeds allocated buffer size value by 1 byte */
	err += invoke_out_of_bounds(&ctx, &in, out, size, 1,
				    false, verbosity);

	/* Offset exceeds allocated size value by 4kByte */
	err += invoke_out_of_bounds(&ctx, &in, out, size, 4096,
				    false, verbosity);

	/* Offset + size overflows offset value */
	err += invoke_out_of_bounds(&ctx, &in, out, 2, ~0,
				    false, verbosity);

	TEEC_ReleaseSharedMemory(&in);
bail:
	tee_deregister_buffer(&ctx, out);
	if (fd >= 0)
		close(fd);
	finalize_tee_ctx(&ctx);

	return err;
}

#define _TO_STR(x) #x
#define TO_STR(x) _TO_STR(x)

static void usage(const char *progname, size_t size, int loop, int ion_heap)
{
	fprintf(stderr, "Usage: %s [OPTION]\n", progname);
	fprintf(stderr,
		"Testing basic accesses to secure buffer (SDP) on OP-TEE.\n"
		"Allocates a secure buffer and invoke a TA to access it.\n"
		"TA is used to init/transform/dump the secure buffer.\n"
		"CA check dumped content.\n\n");

	fprintf(stderr, "Options:\n");
	fprintf(stderr, " -h|--help      Print this help and exit\n");
	fprintf(stderr, " -v             Be verbose\n");
	fprintf(stderr, " -s SIZE        SDP buffer byte size [%zu]\n", size);
	fprintf(stderr, " -n LOOP        Test loop iterations [%u]\n", loop);
	fprintf(stderr, " --ion-heap ID  Target ION heap ID [%d]\n", ion_heap);
	fprintf(stderr, " --no-offset    No random offset [0 255] in buffer\n");
}

#define NEXT_ARG(i) \
	do { \
		if (++i == argc) { \
			fprintf(stderr, "%s: %s: missing argument\n", \
				argv[0], argv[i-1]); \
			return 1; \
		} \
	} while (0);

#define CHECK_RESULT(_res, _exp, _action) \
	if ((_res) == (_exp)) { \
		verbose("Test passed\n"); \
	} else { \
		verbose("Test failed!\n"); \
		_action; \
	}

int sdp_basic_runner_cmd_parser(int argc, char *argv[])
{
	size_t test_size = 5000;
	size_t test_loop = 1000;
	int ion_heap = DEFAULT_ION_HEAP_TYPE;
	int rnd_offset = 1;
	int verbosity = 1;
	int err = 0;
	int i = 0;

	/* Parse command line */
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
			usage(argv[0], test_size, test_loop, ion_heap);
			return 0;
		}
	}
	for (i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-v")) {
			verbosity++;
		} else if (!strcmp(argv[i], "-s")) {
			NEXT_ARG(i);
			test_size = atoi(argv[i]);
		} else if (!strcmp(argv[i], "-n")) {
			NEXT_ARG(i);
			test_loop = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--ion-heap")) {
			NEXT_ARG(i);
			ion_heap = atoi(argv[i]);
		} else if (!strcmp(argv[i], "--no-offset")) {
			rnd_offset = 0;
		} else {
			fprintf(stderr, "%s: invalid argument: %s\n",
				argv[0], argv[i]);
			usage(argv[0], test_size, test_loop, ion_heap);
			return 1;
		}
	}

	verbose("\nSecure Data Path basic access: "
		"NS invokes SDP TA\n");
	err = sdp_basic_test(TEST_NS_TO_TA, test_size, test_loop, ion_heap,
			     rnd_offset, verbosity);
	CHECK_RESULT(err, 0, return 1);

	verbose("\nSecure Data Path basic access: "
		"SDP TA invokes SDP TA\n");
	err = sdp_basic_test(TEST_TA_TO_TA, test_size, test_loop, ion_heap,
			     rnd_offset, verbosity);
	CHECK_RESULT(err, 0, return 1);

	verbose("\nSecure Data Path basic access: "
		"SDP TA invokes SDP pTA\n");
	err = sdp_basic_test(TEST_TA_TO_PTA, test_size, test_loop, ion_heap,
			     rnd_offset, verbosity);
	CHECK_RESULT(err, 0, return 1);

	verbose("\nSecure Data Path basic access: "
		"NS invokes SDP pTA (shall fail)\n");
	err = sdp_basic_test(TEST_NS_TO_PTA, test_size, test_loop, ion_heap,
			     rnd_offset, verbosity);
	CHECK_RESULT(err, 1, return 1);

	verbose("\nSecure Data Path basic access: "
		"Invoke TA with out of bounds buffer references\n");
	err = sdp_out_of_bounds_memref_test(test_size, ion_heap, verbosity);
	CHECK_RESULT(err, 0, return 1);

	return 0;
}
