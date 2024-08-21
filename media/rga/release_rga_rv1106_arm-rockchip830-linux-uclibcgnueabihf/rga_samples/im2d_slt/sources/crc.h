/*
 * Copyright (C) 2024 Rockchip Electronics Co., Ltd.
 * Authors:
 *  Cerf Yu <cerf.yu@rock-chips.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _IM2D_SLT_CRC_H_
#define _IM2D_SLT_CRC_H_

#include <stdint.h>

#include "slt_config.h"

unsigned int crc_table[256];
unsigned int *golden_crc_table = NULL;

__attribute__((constructor)) static void init_crc_table(void)
{
	unsigned int c;
	unsigned int i, j;

	for (i = 0; i < 256; i++) {
		c = (unsigned int)i;
		for (j = 0; j < 8; j++) {
			if (c & 1)
				c = 0xedb88320L ^ (c >> 1);
			else
			    c = c >> 1;
		}
		crc_table[i] = c;
	}
}

static unsigned int crc32(unsigned int crc,unsigned char *buffer, unsigned int size)
{
	unsigned int i;
	for (i = 0; i < size; i++) {
		crc = crc_table[(crc ^ buffer[i]) & 0xff] ^ (crc >> 8);
	}
	return crc;
}

void save_crcdata_to_file(unsigned int crc_data, const char *prefix_name, int case_index)
{
    FILE* crc_file = NULL;
    char file_name[100];

    sprintf(file_name,"%s/%s_%s.txt",
            IM2D_SLT_DEFAULT_GOLDEN_PATH,
            IM2D_SLT_GENERATE_CRC_GOLDEN_PREFIX,
            prefix_name);
    if(case_index == 0) {
        crc_file = fopen(file_name, "wb+");
        if(crc_file == NULL){
            printf("%s,%d:openFile %s fail",__FUNCTION__,__LINE__,file_name);
            exit(0);
        }
    }else {
        crc_file = fopen(file_name, "a+");
        if(crc_file == NULL){
            printf("%s,%d:openFile %s fail",__FUNCTION__,__LINE__,file_name);
            exit(0);
        }
    }

    if(case_index % 8 == 0 && case_index != 0) {
        fprintf(crc_file, "\r\n0x%X,", crc_data);
    }else {
        fprintf(crc_file, "0x%X,", crc_data);
    }

    if (crc_file) {
        fclose(crc_file);
        crc_file = NULL;
    }

    /* golden */
    sprintf(file_name,"%s/%s_%s.bin",
            IM2D_SLT_DEFAULT_GOLDEN_PATH,
            IM2D_SLT_GENERATE_CRC_GOLDEN_PREFIX,
            prefix_name);
    if(case_index == 0) {
        crc_file = fopen(file_name, "wb+");
        if(crc_file == NULL){
            printf("%s,%d:openFile %s fail",__FUNCTION__,__LINE__,file_name);
            exit(0);
        }
    }else {
        crc_file = fopen(file_name, "a+");
        if(crc_file == NULL){
            printf("%s,%d:openFile %s fail",__FUNCTION__,__LINE__,file_name);
            exit(0);
        }
    }

    fwrite((void *)(&crc_data), sizeof(crc_data), 1, crc_file);
    if (crc_file) {
        fclose(crc_file);
        crc_file = NULL;
    }
}

unsigned int * read_crcdata_from_file(const char *prefix_name) {
    int size;
    FILE *golden_file = NULL;
    unsigned int *golden_table = NULL;
    char file_name[100];

    sprintf(file_name,"%s/%s_%s.bin",
            IM2D_SLT_DEFAULT_GOLDEN_PATH,
            IM2D_SLT_GENERATE_CRC_GOLDEN_PREFIX,
            prefix_name);
    golden_file = fopen(file_name,"rb");
    if (golden_file) {
        fseek(golden_file, 0, SEEK_END);
        size = ftell(golden_file);
        golden_table = (unsigned int *)malloc(size);
        if (golden_table == NULL) {
            printf("malloc golden_table fault!\n");
            fclose(golden_file);
            return NULL;
        }

        fseek (golden_file, 0, SEEK_SET);
        fread((void*)golden_table, size, 1, golden_file);
        fclose(golden_file);

        return golden_table;
    } else {
        printf("Could not open file : %s \n",file_name);
    }

    return NULL;
}

inline int crc_check(int index, unsigned int crc_data, unsigned int *golden_table) {
    return crc_data != golden_table[index] ? false : true;
}

#endif /* #ifndef _IM2D_SLT_CRC_H_ */
