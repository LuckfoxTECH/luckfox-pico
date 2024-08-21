// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2023 Rockchip Electronics Co. Ltd.
 */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/crypto.h>
#include <openssl/evp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "anti_copy.h"

static int anti_copy_read_cpuid(uint8_t *buf, int len)
{
	int fd = 0, ret = 0;
	int cpuid_off;

	fd = open("/sys/bus/nvmem/devices/rockchip-otp0/nvmem", O_RDONLY);
	if (fd < 0) {
		printf("error, open otp node fail!\n");
		return -1;
	}

	/* rk3588 is 7, rk3568 is 10, please check cpu_id or otp_id in dts. */
	cpuid_off = 7;
	ret = lseek(fd, cpuid_off, SEEK_SET);
	if (ret < 0) {
		printf("error, otp lseek fail!\n");
		goto out;
	}

	ret = read(fd, buf, len);

out:
	close(fd);
	return ret;
}

static int anti_copy_aes_crypto(uint8_t *indata, uint8_t *outdata, int len, uint8_t *key)
{
	int ret;
	int temp_len;
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

	EVP_CIPHER_CTX_init(ctx);

	ret = EVP_EncryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL);
	if (ret != 1)
		return -1;

	EVP_CIPHER_CTX_set_padding(ctx, 0 /* disable padding */);

	ret = EVP_EncryptUpdate(ctx, outdata, &temp_len, indata, len);
	if (ret != 1)
		return -1;

	ret = EVP_EncryptFinal_ex(ctx, outdata + temp_len, &temp_len);
	if (ret != 1)
		return -1;

	EVP_CIPHER_CTX_cleanup(ctx);

	EVP_CIPHER_CTX_free(ctx);

	return 0;
}

static int anti_copy_aes_crypto_dec(uint8_t *indata, uint8_t *outdata, int len, uint8_t *key)
{
	int ret;
	int temp_len;
	EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();

	EVP_CIPHER_CTX_init(ctx);

	ret = EVP_DecryptInit_ex(ctx, EVP_aes_256_ecb(), NULL, key, NULL);
	if (ret != 1)
		return -1;

	EVP_CIPHER_CTX_set_padding(ctx, 0 /* disable padding */);

	ret = EVP_DecryptUpdate(ctx, outdata, &temp_len, indata, len);
	if (ret != 1)
		return -1;

	ret = EVP_DecryptFinal_ex(ctx, outdata + temp_len, &temp_len);
	if (ret != 1)
		return -1;

	EVP_CIPHER_CTX_cleanup(ctx);

	EVP_CIPHER_CTX_free(ctx);

	return 0;
}

#define VENDOR_REQ_TAG		0x56524551
#define VENDOR_READ_IO		_IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO		_IOW('v', 0x02, unsigned int)
#define VENDOR_ENC_CPUID	21

struct rk_vendor_req {
	uint32_t tag;
	uint16_t id;
	uint16_t len;
	uint8_t data[0];
};

static int vendor_storage_write(uint16_t id, uint8_t *data, uint16_t len)
{
	int ret;
	uint8_t p_buf[2048];
	struct rk_vendor_req *req;

	/* make sure app has permission to access this node */
	int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
	if (sys_fd < 0) {
		printf("vendor_storage open fail!\n");
		return -1;
	}

	req = (struct rk_vendor_req *)p_buf;
	req->tag = VENDOR_REQ_TAG;
	req->id = id;
	req->len = len;
	memcpy(req->data, data, len);

	ret = ioctl(sys_fd, VENDOR_WRITE_IO, req);
	if (ret) {
		printf("vendor_storage write error!\n");
		return -1;
	}
	return 0;
}

static int vendor_storage_read(uint16_t id, uint8_t *data, uint16_t *len)
{
	int ret;
	uint8_t p_buf[2048];
	struct rk_vendor_req *req;

	/* make sure app has permission to access this node */
	int sys_fd = open("/dev/vendor_storage", O_RDWR, 0);
	if (sys_fd < 0) {
		printf("vendor_storage open fail!\n");
		return -1;
	}

	req = (struct rk_vendor_req *)p_buf;
	req->tag = VENDOR_REQ_TAG;
	req->id = id;
	req->len = 512;

	ret = ioctl(sys_fd, VENDOR_READ_IO, req);
	if (ret) {
		printf("vendor_storage read error!\n");
		return -1;
	}
	memcpy(data, req->data, req->len);
	*len = req->len;
	return 0;
}

int anti_copy_low_gen(void)
{
	int ret;
	uint8_t cpuid[16];
	uint8_t enc_cpuid[16];
	uint8_t aes_key[32];
	uint8_t aes_confuse_key[32] = {0x87, 0x58, 0x27, 0xd3, 0xdc, 0xe8, 0xd5, 0xbb,
				       0x42, 0xcd, 0x79, 0xf6, 0xeb, 0x8d, 0x25, 0xe0,
				       0xe0, 0x92, 0xdc, 0xb2, 0xd3, 0x77, 0xa2, 0x04,
				       0xc5, 0x48, 0x0e, 0x25, 0xbe, 0x7f, 0x6d, 0xd4};
	uint8_t aes_confuse_data[32] = {0x45, 0xa3, 0xbf, 0x81, 0xc6, 0x55, 0x03, 0xa0,
					0xfd, 0x52, 0xb8, 0x9a, 0x44, 0xb5, 0x2c, 0x1c,
					0xce, 0x5c, 0xf5, 0x14, 0x4f, 0x49, 0x42, 0x6a,
					0xfa, 0xba, 0x1d, 0xd0, 0x41, 0xe2, 0x83, 0x58};

	ret = anti_copy_read_cpuid(cpuid, sizeof(cpuid));
	if (ret != sizeof(cpuid)) {
		printf("read cpuid fail! ret=%d\n", ret);
		return -1;
	}
	dump_hex("cpuid", cpuid, sizeof(cpuid));

	ret = anti_copy_aes_crypto(aes_confuse_data, aes_key, sizeof(aes_confuse_data), aes_confuse_key);
	if (ret) {
		printf("aes crypto fail!\n");
		return -1;
	}
	dump_hex("aes_key", aes_key, sizeof(aes_key));

	ret = anti_copy_aes_crypto(cpuid, enc_cpuid, sizeof(cpuid), aes_key);
	if (ret) {
		printf("aes crypto fail!\n");
		return -1;
	}
	dump_hex("enc_cpuid", enc_cpuid, sizeof(enc_cpuid));

	ret = vendor_storage_write(VENDOR_ENC_CPUID, enc_cpuid, sizeof(enc_cpuid));
	if (ret) {
		printf("vendor storage fail!\n");
		return -1;
	}
	printf("generate enc_cpuid success!\n");
	return 0;
}

int anti_copy_low_verify(void)
{
	int ret;
	uint16_t read_len;
	uint8_t cpuid[16];
	uint8_t enc_cpuid[16];
	uint8_t dec_cpuid[16];
	uint8_t aes_key[32];
	uint8_t aes_confuse_key[32] = {0x87, 0x58, 0x27, 0xd3, 0xdc, 0xe8, 0xd5, 0xbb,
				       0x42, 0xcd, 0x79, 0xf6, 0xeb, 0x8d, 0x25, 0xe0,
				       0xe0, 0x92, 0xdc, 0xb2, 0xd3, 0x77, 0xa2, 0x04,
				       0xc5, 0x48, 0x0e, 0x25, 0xbe, 0x7f, 0x6d, 0xd4};
	uint8_t aes_confuse_data[32] = {0x45, 0xa3, 0xbf, 0x81, 0xc6, 0x55, 0x03, 0xa0,
					0xfd, 0x52, 0xb8, 0x9a, 0x44, 0xb5, 0x2c, 0x1c,
					0xce, 0x5c, 0xf5, 0x14, 0x4f, 0x49, 0x42, 0x6a,
					0xfa, 0xba, 0x1d, 0xd0, 0x41, 0xe2, 0x83, 0x58};

	ret = vendor_storage_read(VENDOR_ENC_CPUID, enc_cpuid, &read_len);
	if (ret) {
		printf("vendor storage read fail!\n");
		return -1;
	}
	if (read_len != sizeof(enc_cpuid)) {
		printf("vendor storage read lenght error!\n");
		return -1;
	}
	dump_hex("enc_cpuid", enc_cpuid, sizeof(enc_cpuid));

	ret = anti_copy_aes_crypto(aes_confuse_data, aes_key, sizeof(aes_confuse_data), aes_confuse_key);
	if (ret) {
		printf("aes crypto fail!\n");
		return -1;
	}
	dump_hex("aes_key", aes_key, sizeof(aes_key));

	ret = anti_copy_aes_crypto_dec(enc_cpuid, dec_cpuid, sizeof(enc_cpuid), aes_key);
	if (ret) {
		printf("aes crypto dec fail!\n");
		return -1;
	}
	dump_hex("dec_cpuid", dec_cpuid, sizeof(dec_cpuid));

	ret = anti_copy_read_cpuid(cpuid, sizeof(cpuid));
	if (ret != sizeof(cpuid)) {
		printf("read cpuid fail! ret=%d\n", ret);
		return -1;
	}
	dump_hex("cpuid", cpuid, sizeof(cpuid));

	if (memcmp(dec_cpuid, cpuid, sizeof(cpuid)) != 0) {
		printf("check cpuid fail!\n");
		return -1;
	}
	printf("verify success!\n");
	return 0;
}
