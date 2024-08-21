#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "calib_data.h"

#define EEPROM_UPDATE_PATH "/tmp/update/calib_data.bin"
#define CALIB_IMG_EEPROM   "/sys/bus/i2c/devices/3-0050/eeprom"

enum cmp_calib_result_type {
	CALIB_NEED_UPDATE,
	CALIB_NO_NEED_UPDATE,
};

void eeprom_calib_data_update(void)
{
	struct timeval time1, time2;
	float use_time;
	gettimeofday(&time1, NULL);
	if (access(EEPROM_UPDATE_PATH, F_OK) != 0)
		return;

	printf("%s exist, upate eeprom\n", EEPROM_UPDATE_PATH);
	char str[128];
	snprintf(str, sizeof(str), "cat %s > %s",
			EEPROM_UPDATE_PATH, CALIB_IMG_EEPROM);
	printf("##### start write eeprom, wait about 130 seconds #####\n");
	system(str);
	printf("update eeprom finish\n");

	snprintf(str, sizeof(str), "rm %s", EEPROM_UPDATE_PATH);
	system(str);

	gettimeofday(&time2, NULL);
	use_time = (time2.tv_sec - time1.tv_sec) * 1000 +
		(float)(time2.tv_usec - time1.tv_usec) / 1000;
	printf("write eeprom finish, time:%f ms\n", use_time);
}

enum cmp_calib_result_type cmp_calibdata(const char *calib_data_path)
{
	FILE *fp1 = NULL;
	FILE *fp2 = NULL;
	unsigned int version1, version2;
	unsigned int crc1, crc2;
	unsigned int items_number;
	size_t offset;
	unsigned char magic[16];
	enum cmp_calib_result_type result;
	int i;

	printf("check calibdata:%s\n", calib_data_path);

	/* open both files */
	fp1 = fopen(calib_data_path, "rb");
	if (fp1 == NULL) {
		printf("No calib data in userdata!\n");
		result = CALIB_NEED_UPDATE;
		goto out;
	}

	/* check claib data magic end */
	memset(magic, 0, sizeof(magic));
	offset = strlen(PREISP_CALIB_MAGIC_END);
	fseek(fp1, -(offset + 1), SEEK_END);
	fread(magic,offset,1,fp1);

	if (strncmp((char *)magic, PREISP_CALIB_MAGIC_END,
				strlen(PREISP_CALIB_MAGIC_END))) {
		printf("calib data magic end error, read:%s\n", magic);
		result = CALIB_NEED_UPDATE;
		goto out;
	}

	/* check claib data magic */
	offset = offsetof(struct calib_head, version);
	fseek(fp1, 0, SEEK_SET);
	fread(magic,offset,1,fp1);

	if (strncmp((char *)magic, PREISP_CALIB_MAGIC,
				strlen(PREISP_CALIB_MAGIC))) {
		printf("calib data magic error\n");
		result = CALIB_NEED_UPDATE;
		goto out;
	}

	fp2 = fopen(CALIB_IMG_EEPROM,"rb");
	if (fp2 == NULL) {
		printf("EEPROM not exsit.Do you have gt24 driver in kernel?\n");
		result = CALIB_NO_NEED_UPDATE;
		goto out;
	}

	/* check eeprom magic */
	offset = offsetof(struct calib_head, version);
	fseek(fp2, 0, SEEK_SET);
	fread(magic,offset,1,fp2);

	if (strncmp((char *)magic, PREISP_CALIB_MAGIC,
				strlen(PREISP_CALIB_MAGIC))) {
		printf("eeprom magic error\n");
		result = CALIB_NO_NEED_UPDATE;
		goto out;
	}

	/* compare version */
	offset = offsetof(struct calib_head, version);
	fseek(fp1, offset, SEEK_SET);
	fread(&version1,sizeof(unsigned int),1,fp1);

	fseek(fp2, offset, SEEK_SET);
	fread(&version2,sizeof(unsigned int),1,fp2);

	if(version1 != version2){
		printf("Version unmatch\n");
		result = CALIB_NEED_UPDATE;
		goto out;
	}

	/* read item num */
	offset = offsetof(struct calib_head, items_number);
	fseek(fp1, offset, SEEK_SET);
	fread(&items_number,sizeof(unsigned int),1,fp1);

	/* compare each item's crc */
	for(i=0; i<items_number; i++){
		offset = offsetof(struct calib_head, item) +
			i*sizeof(struct calib_item)
			+ offsetof(struct calib_item, crc32);

		fseek(fp1, offset, SEEK_SET);
		fread(&crc1,sizeof(unsigned int),1,fp1);

		fseek(fp2, offset, SEEK_SET);
		fread(&crc2,sizeof(unsigned int),1,fp2);

		if(crc1 != crc2){
			printf("crc unmatch\n");
			result = CALIB_NEED_UPDATE;
			goto out;
		}
	}

	result = CALIB_NO_NEED_UPDATE;

out:
	if(fp1)
		fclose(fp1);
	if(fp2)
		fclose(fp2);

	return result;
}

int check_eeprom_magic(void)
{
	int ret = -1;
	FILE *fp = NULL;
	size_t offset;
	unsigned char magic[16];

	printf("check eeprom:%s\n", CALIB_IMG_EEPROM);

	fp = fopen(CALIB_IMG_EEPROM,"rb");
	if (fp == NULL) {
		printf("EEPROM not exsit.Do you have gt24 driver in kernel?\n");
		goto out;
	}

	/* check eeprom magic */
	offset = offsetof(struct calib_head, version);
	fseek(fp, 0, SEEK_SET);
	fread(magic,offset,1,fp);

	if (strncmp((char *)magic, PREISP_CALIB_MAGIC,
				strlen(PREISP_CALIB_MAGIC))) {
		printf("eeprom magic error\n");
		goto out;
	}

	ret = 0;

out:
	if(fp)
		fclose(fp);

	return ret;
}

void eeprom_calib_data_check(const char*calib_path, const char *dp_init_path)
{
	struct timeval time1, time2;
	float use_time;

	enum cmp_calib_result_type result;
	char str[128];
	snprintf(str, sizeof(str), "%scalib_data.bin", calib_path);

	eeprom_calib_data_update();

	gettimeofday(&time1, NULL);

	result = cmp_calibdata(str);
	if (result == CALIB_NO_NEED_UPDATE) {
		gettimeofday(&time2, NULL);
		use_time = (time2.tv_sec - time1.tv_sec) * 1000 +
			(float)(time2.tv_usec - time1.tv_usec) / 1000;
		printf("calib no need update, time:%f\n", use_time);
		return;
	}

	if (check_eeprom_magic() != 0)
		return;

	printf("calib data corrupt or mismatch, update from eeprom...\n");

	snprintf(str, sizeof(str), "rm %sref*.bin", dp_init_path);
	printf("%s\n", str);
	system(str);

	snprintf(str, sizeof(str), "rm %smap*.bin", dp_init_path);
	printf("%s\n", str);
	system(str);

	snprintf(str, sizeof(str), "cat %s > %scalib_data.bin",
			CALIB_IMG_EEPROM, calib_path);
	printf("%s\n", str);
	system(str);

	snprintf(str, sizeof(str), "echo %s >> %scalib_data.bin",
			PREISP_CALIB_MAGIC_END, calib_path);
	printf("%s\n", str);
	system(str);

	gettimeofday(&time2, NULL);
	use_time = (time2.tv_sec - time1.tv_sec) * 1000 +
		(float)(time2.tv_usec - time1.tv_usec) / 1000;

	printf("update eeprom calib data finish, time:%f ms\n", use_time);
}

