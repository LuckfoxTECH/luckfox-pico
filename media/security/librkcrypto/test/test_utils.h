/*
 * Copyright (c) 2022 Rockchip Electronics Co. Ltd.
 */
#ifndef _TEST_UTILS_H_
#define _TEST_UTILS_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

bool is_no_multi_blocksize(uint32_t mode);

void test_get_rng(uint8_t *trn, uint32_t len);
void test_dump_hex(char *var_name, const uint8_t *data, uint32_t len);
const char *test_algo_name(uint32_t algo);
const char *test_mode_name(uint32_t mode);
const char *test_op_name(uint32_t operation);

#endif

