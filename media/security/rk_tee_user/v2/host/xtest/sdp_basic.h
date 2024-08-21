/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2017, Linaro Limited
 * All rights reserved.
 */

#ifndef XTEST_SDP_BASIC_H
#define XTEST_SDP_BASIC_H

#include "include/uapi/linux/ion.h"
#include "ta_sdp_basic.h"

#define DEFAULT_ION_HEAP_TYPE	ION_HEAP_TYPE_UNMAPPED

enum test_target_ta {
	TEST_NS_TO_TA,
	TEST_NS_TO_PTA,
	TEST_TA_TO_TA,
	TEST_TA_TO_PTA,
};

int allocate_ion_buffer(size_t size, int heap_id, int verbosity);
int sdp_basic_test(enum test_target_ta ta,
			  size_t size, size_t loop, int ion_heap,
			  int rnd_offset, int verbosity);

int sdp_out_of_bounds_memref_test(size_t size, int ion_heap, int verbosity);

#endif /* XTEST_SDP_BASIC_H */
