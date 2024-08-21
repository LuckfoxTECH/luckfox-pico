// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "anti_copy.h"

void dump_hex(const char *var_name, const uint8_t *data, uint32_t len)
{
	printf("%s:\n", var_name);
	for (uint32_t i = 0; i < len; i++) {
		if(i != 0 && i % 16 == 0)
			printf("\n");
		printf("%4x", data[i]);
	}
	printf("\n");
}

int anti_copy(uint32_t invoke_command)
{
	int res = 0;

	switch (invoke_command) {
	case LOW_GEN:
		res = anti_copy_low_gen();
		break;
	case LOW_VERIFY:
		res = anti_copy_low_verify();
		break;
	case MID_GEN:
		res = anti_copy_mid_gen();
		break;
	case MID_VERIFY:
		res = anti_copy_mid_verify();
		break;
	case HIGH_GEN:
		res = anti_copy_high_gen();
		break;
	case HIGH_VERIFY:
		res = anti_copy_high_verify();
		break;
	default:
		printf("Doing nothing.\n");
		break;
	}
	return res;
}
