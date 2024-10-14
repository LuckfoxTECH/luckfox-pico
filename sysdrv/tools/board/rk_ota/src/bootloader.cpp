/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include "bootloader.h"
#include <stdio.h>
#include "stdlib.h"
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/reboot.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "md5sum.h"
#include "mtdutils.h"

// ------------------------------------
// for misc partitions on block devices
// ------------------------------------
static void wait_for_device(const char* fn) {
    int tries = 0;
    int ret;
    struct stat buf;
    do {
        ++tries;
        ret = stat(fn, &buf);
        if (ret) {
            printf("stat %s try %d: %s\n", fn, tries, strerror(errno));
            sleep(1);
        }
    } while (ret && tries < 10);
    if (ret) {
        printf("failed to stat %s\n", fn);
    }
}

static unsigned int iavb_crc32_tab[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f,
    0xe963a535, 0x9e6495a3, 0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91, 0x1db71064, 0x6ab020f2,
    0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9,
    0xfa0f3d63, 0x8d080df5, 0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b, 0x35b5a8fa, 0x42b2986c,
    0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423,
    0xcfba9599, 0xb8bda50f, 0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d, 0x76dc4190, 0x01db7106,
    0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d,
    0x91646c97, 0xe6635c01, 0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457, 0x65b0d9c6, 0x12b7e950,
    0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7,
    0xa4d1c46d, 0xd3d6f4fb, 0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9, 0x5005713c, 0x270241aa,
    0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81,
    0xb7bd5c3b, 0xc0ba6cad, 0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683, 0xe3630b12, 0x94643b84,
    0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb,
    0x196c3671, 0x6e6b06e7, 0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5, 0xd6d6a3e8, 0xa1d1937e,
    0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55,
    0x316e8eef, 0x4669be79, 0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f, 0xc5ba3bbe, 0xb2bd0b28,
    0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f,
    0x72076785, 0x05005713, 0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21, 0x86d3d2d4, 0xf1d4e242,
    0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69,
    0x616bffd3, 0x166ccf45, 0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db, 0xaed16a4a, 0xd9d65adc,
    0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693,
    0x54de5729, 0x23d967bf, 0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

/* Converts a 32-bit unsigned integer from host to big-endian byte order. */
static unsigned int avb_htobe32(unsigned int in) {
    union {
        unsigned int word;
        unsigned char bytes[4];
    } ret;
    ret.bytes[0] = (in >> 24) & 0xff;
    ret.bytes[1] = (in >> 16) & 0xff;
    ret.bytes[2] = (in >> 8) & 0xff;
    ret.bytes[3] = in & 0xff;
    return ret.word;
}

/*
 *  A function that calculates the CRC-32 based on the table above is
 *  given below for documentation purposes. An equivalent implementation
 *  of this function that's actually used in the kernel can be found
 *  in sys/libkern.h, where it can be inlined.
 */
static unsigned int iavb_crc32(unsigned int crc_in, const unsigned char* buf, int size) {
    const unsigned char* p = buf;
    unsigned int crc;

    crc = crc_in ^ ~0U;
    while (size--)
    crc = iavb_crc32_tab[(crc ^ *p++) & 0xFF] ^ (crc >> 8);
    return crc ^ ~0U;
}

static unsigned int avb_crc32(const unsigned char* buf, size_t size) {
      return iavb_crc32(0, buf, size);
}

static void AvbABData_update_crc(struct AvbABData *data)
{
    data->crc32 = avb_htobe32(avb_crc32((const unsigned char*)data, sizeof(struct AvbABData) - sizeof(unsigned int)));
}

//判断是MTD还是block 设备
static bool isMtdDevice() {
    char param[2048];
    int fd, ret;
    char *s = NULL;
    fd = open("/proc/cmdline", O_RDONLY);
    ret = read(fd, (char*)param, 2048);
    close(fd);
#if 0
    s = strstr(param,"storagemedia");
    if(s == NULL){
        LOGI("no found storagemedia in cmdline, default is not MTD.\n");
        return false;
    }else{
        s = strstr(s, "=");
        if (s == NULL) {
            LOGI("no found storagemedia in cmdline, default is not MTD.\n");
            return false;
        }

        s++;
        while (*s == ' ') {
            s++;
        }

        if (strncmp(s, "mtd", 3) == 0 ) {
            if (strstr(param,"nand")) {
                LOGI("Now is MTD.\n");
                return true;
            } else {
                LOGI("Now is MTD block, not nand device.\n");
                return false;
            }
        } else if (strncmp(s, "sd", 2) == 0) {
            LOGI("Now is SD.\n");
            if ( !access(MTD_PATH, F_OK) ) {
                fd = open(MTD_PATH, O_RDONLY);
                ret = read(fd, (char*)param, 2048);
                close(fd);

                s = strstr(param,"mtd");
                if(s == NULL){
                    LOGI("no found mtd.\n");
                    return false;
                }
                LOGI("Now is MTD.\n");
                return true;
            }
        }
    }
    LOGI("Current device is not MTD.\n");
    return false;
#else
    s = strstr(param,"nand");
    if (s) {
        LOGI("Now is MTD.\n");
        return true;
    } else {
        LOGI("No found nand in cmdline, default is not MTD.\n");
        return false;
    }
#endif
}

static int writeMisc_mtd(char *buf, int offset, int size) {
    size_t page_size = 0;
    size_t total_size = 0;
    mtd_scan_partitions();
    const MtdPartition *part = mtd_find_partition_by_name(MISC_PARTITION_NAME_MTD);
    if (part == NULL || mtd_partition_info(part, &total_size, NULL, &page_size)) {
        LOGE("Can't find %s\n", MISC_PARTITION_NAME_MTD);
        return -1;
    }

    MtdReadContext *read = mtd_read_partition(part);
    if (read == NULL) {
        LOGE("Can't open %s\n(%s)\n", MISC_PARTITION_NAME_MTD, strerror(errno));
        return -1;
    }

    size_t write_size = total_size; //读取20个页，24k（原write_size = page_size * 10）
    char data[write_size];
    ssize_t r = mtd_read_data(read, data, write_size);
    if (r != write_size) LOGE("Can't read %s\n(%s)\n", MISC_PARTITION_NAME_MTD, strerror(errno));
    mtd_read_close(read);
    if (r != write_size) return -1;

    memcpy(&data[offset], buf, size);

    MtdWriteContext *write = mtd_write_partition(part);
    if (write == NULL) {
        LOGE("Can't open %s\n(%s)\n", MISC_PARTITION_NAME_MTD, strerror(errno));
        return -1;
    }

    if (mtd_write_data(write, data, write_size) != write_size) {
        LOGE("Can't write %s\n(%s)\n", MISC_PARTITION_NAME_MTD, strerror(errno));
        mtd_write_close(write);
        return -1;
    }
    if (mtd_write_close(write)) {
        LOGE("Can't finish %s\n(%s)\n", MISC_PARTITION_NAME_MTD, strerror(errno));
        return -1;
    }

    return 0;
}

static int readMisc_mtd(char *buf, int offset, int size) {
    size_t page_size;
    size_t total_size = 0;
    memset(buf, 0, size);
    mtd_scan_partitions();
    const MtdPartition *part = mtd_find_partition_by_name(MISC_PARTITION_NAME_MTD);
    if (part == NULL || mtd_partition_info(part, &total_size, NULL, &page_size)) {
        LOGE("Can't find %s\n", MISC_PARTITION_NAME_MTD);
        return -1;
    }

    MtdReadContext *read = mtd_read_partition(part);
    if (read == NULL) {
        LOGE("Can't open %s\n(%s)\n", MISC_PARTITION_NAME_MTD, strerror(errno));
        return -1;
    }
    size_t write_size = total_size; //读取20个页，24k（原write_size = page_size * 10）
    char data[write_size];
    ssize_t r = mtd_read_data(read, data, write_size);
    if (r != write_size) LOGE("Can't read %s\n(%s)\n", MISC_PARTITION_NAME_MTD, strerror(errno));
    mtd_read_close(read);
    if (r != write_size) return -1;

    memcpy(buf, &data[offset], size);
    return 0;
}

static int writeMisc_block(char *buf, int offset, int size) {
    FILE* f = fopen(MISC_PARTITION_NAME_BLOCK, "wb");
    if (f == NULL) {
        LOGE("Can't open %s\n(%s)\n", MISC_PARTITION_NAME_BLOCK, strerror(errno));
        return -1;
    }
    fseek(f, offset, SEEK_SET);
    int count = fwrite(buf, sizeof(char), size, f);
    if (count != size) {
        LOGE("Failed writing %s\n(%s)\n", MISC_PARTITION_NAME_BLOCK, strerror(errno));
        return -1;
    }
    if (fclose(f) != 0) {
        LOGE("Failed closing %s\n(%s)\n", MISC_PARTITION_NAME_BLOCK, strerror(errno));
        return -1;
    }
    return 0;
}

static int readMisc_block(char *buf, int offset, int size) {
    wait_for_device(MISC_PARTITION_NAME_BLOCK);
    FILE* f = fopen(MISC_PARTITION_NAME_BLOCK, "rb");
    if (f == NULL) {
        LOGE("Can't open %s\n(%s)\n", MISC_PARTITION_NAME_BLOCK, strerror(errno));
        return -1;
    }

    fseek(f, offset, SEEK_SET);

    int count = fread(buf, sizeof(char), size, f);
    if (count != size) {
        LOGE("Failed reading %s\n(%s)\n", MISC_PARTITION_NAME_BLOCK, strerror(errno));
        return -1;
    }
    if (fclose(f) != 0) {
        LOGE("Failed closing %s\n(%s)\n", MISC_PARTITION_NAME_BLOCK, strerror(errno));
        return -1;
    }
    return 0;
}

static int writeMisc(char *buf, int offset, int size) {
    if (isMtdDevice()) {
        return writeMisc_mtd(buf, offset, size);
    } else {
        return writeMisc_block(buf, offset, size);
    }
}

static int readMisc(char *buf, int offset, int size) {
    if (isMtdDevice()) {
        return readMisc_mtd(buf, offset, size);
    } else {
        return readMisc_block(buf, offset, size);
    }
}

/*
 * 读misc 偏移4K处格式化命令
 */
static int readCmdMisc(char *cmdline, int size) {
    return readMisc(cmdline, MISC_OFFSET_CMDLINE, size);
}

/*
 * 从misc 偏移2k处读取引导信息
 */
static int readABMisc(struct AvbABData* info) {
   return readMisc((char *)info, MISC_OFFSET, sizeof(struct AvbABData));
}

/*
 * 往misc 偏移2k处写入引导信息
 */
static int writeABMisc(struct AvbABData *info) {
    return writeMisc((char *)info, MISC_OFFSET, sizeof(struct AvbABData));
}

/*
 * 往misc 偏移16k位置处读取recovery信息
 */
int get_bootloader_message(struct bootloader_message *out) {
    return readMisc((char *)out, BOOTLOADER_MESSAGE_OFFSET_IN_MISC, sizeof(struct bootloader_message));
}

/*
 * 往misc 偏移16k位置写入recovery信息
 */
int set_bootloader_message(const struct bootloader_message *in){
    return writeMisc((char*)in, BOOTLOADER_MESSAGE_OFFSET_IN_MISC, sizeof(struct bootloader_message));
}

/*
 * 写入mtd设备
 */
static int mtd_write(char *src_path, long long size, char *dest_path) {
    LOGI("mtd_write src=%s dest=%s.\n", src_path, dest_path);

    struct stat sb;
    char mtd_write_erase_cmd[256] = {0};
    stat(dest_path, &sb);

    if ((sb.st_mode & S_IFMT) == S_IFCHR) {
        memset(mtd_write_erase_cmd, 0, sizeof(mtd_write_erase_cmd)/sizeof(mtd_write_erase_cmd[0]));
        sprintf(mtd_write_erase_cmd, "flash_erase %s 0 0", dest_path);
        system(mtd_write_erase_cmd);

        // e.g. nandwrite -p /dev/block/by-name/system_b /mnt/sdcard/rk_update/system.img
        memset(mtd_write_erase_cmd, 0, sizeof(mtd_write_erase_cmd)/sizeof(mtd_write_erase_cmd[0]));
        sprintf(mtd_write_erase_cmd, "nandwrite -p %s %s", dest_path, src_path);
        system(mtd_write_erase_cmd);
    } else {
        LOGE("flash_erase: can't erase MTD \"%s\"\n", dest_path);
        return -1;
    }

    return 0;
}

/*
 * 写入mmcblock设备
 */
static int block_write(char *src_path, long long size, char *dest_path) {
    LOGI("block_write src=%s dest=%s.\n", src_path, dest_path);

    int fd_src, fd_dest;
    int src_step, dest_step;
    long long src_remain, dest_remain;
    long long read_count, write_count;
    char data_buf[BLOCK_WRITE_LEN] = {0};

    dest_remain = src_remain = size;
    dest_step = src_step = BLOCK_WRITE_LEN;

    fd_src = open(src_path, O_RDONLY);
    if (fd_src < 0) {
        LOGE("Can't open %s\n", src_path);
        return -1;
    }

    if (lseek64(fd_src, 0, SEEK_SET) == -1) {
        close(fd_src);
        LOGE("lseek64 failed (%s:%d).\n", __func__, __LINE__);
        return -1;
    }

    fd_dest = open(dest_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd_dest < 0) {
        close(fd_src);
        LOGE("Can't open %s\n", dest_path);
        return -1;
    }
    if ( lseek64(fd_dest, 0, SEEK_SET) == -1 ) {
        LOGE("(%s:%d) lseek64 failed(%s).\n", __func__, __LINE__, strerror(errno));
        close(fd_src);
        close(fd_dest);
        return -1;
    }
    while (src_remain > 0 && dest_remain > 0) {
        memset(data_buf, 0, BLOCK_WRITE_LEN);
        read_count = src_remain>src_step?src_step:src_remain;

        if (read(fd_src, data_buf, read_count) != read_count) {
            close(fd_dest);
            close(fd_src);
            LOGE("Read failed(%s):(%s:%d)\n", strerror(errno), __func__, __LINE__);
            return -1;
        }

        src_remain -= read_count;
        write_count = dest_remain>dest_step?dest_step:dest_remain;

        if (write(fd_dest, data_buf, write_count) != write_count) {
            close(fd_dest);
            close(fd_src);
            LOGE("(%s:%d) write failed(%s).\n", __func__, __LINE__, strerror(errno));
            return -1;
        }
        dest_remain -= write_count;
    }

    fsync(fd_dest);
    close(fd_dest);
    close(fd_src);
    return 0;
}

static int comparefile(const char *dest_path, const char *source_path, long long dest_offset, long long source_offset, long long checkSize){
    unsigned char md5sum_source[16];
    unsigned char md5sum_dest[16];
    if (isMtdDevice()) {
        checkdata_mtd(dest_path, md5sum_dest, dest_offset, checkSize);
    } else {
        checkdata(dest_path, md5sum_dest, dest_offset, checkSize);
    }
    checkdata(source_path, md5sum_source, source_offset, checkSize);
    for(int i = 0; i < 16; i++){
        if(md5sum_dest[i] != md5sum_source[i]){
            LOGE("MD5Check is error of %s", dest_path);
            return -1;
        }
    }
    return 0;
}

int avb_safe_memcmp(const void* s1, const void* s2, size_t n) {
  const unsigned char* us1 = (const unsigned char *)s1;
  const unsigned char* us2 = (const unsigned char *)s2;
  int result = 0;

  if (0 == n) {
    return 0;
  }

  /*
   * Code snippet without data-dependent branch due to Nate Lawson
   * (nate@root.org) of Root Labs.
   */
  while (n--) {
    result |= *us1++ ^ *us2++;
  }

  return result != 0;
}

static bool slot_is_bootable(AvbABSlotData *slot)
{
	return slot->priority > 0 &&
		(slot->successful_boot || (slot->tries_remaining > 0));
}

static int get_lastboot(AvbABData *ab_data)
{
	return ab_data->last_boot;
}

int get_current_slot(AvbABData *ab_data)
{
	static int last_slot_index = -1;
	int slot_index_to_boot;
    int ret;

    if (slot_is_bootable(&ab_data->slots[0]) &&
	    slot_is_bootable(&ab_data->slots[1])) {
        if (ab_data->slots[1].priority > ab_data->slots[0].priority)
            slot_index_to_boot = 1;
		else
			slot_index_to_boot = 0;
	} else if (slot_is_bootable(&ab_data->slots[0])) {
		slot_index_to_boot = 0;
	} else if (slot_is_bootable(&ab_data->slots[1])) {
		slot_index_to_boot = 1;
	} else {
		printf("No bootable slots found, use lastboot.\n");
		if (get_lastboot(ab_data) == 0) {
            ret = 0;
            goto out;
        } else if (get_lastboot(ab_data) == 1) {
            ret = 1;
            goto out;
        } else {
			return -1;
		}
	}

	if (slot_index_to_boot == 0)
        ret = 0;
    else if (slot_index_to_boot == 1)
        ret = 1;

    if (last_slot_index != slot_index_to_boot) {
		last_slot_index = slot_index_to_boot;
        printf("A/B-slot: %c, successful: %d, tries-remain: %d\n",
               (ret == 0) ? 'A' : 'B',
               ab_data->slots[slot_index_to_boot].successful_boot,
               ab_data->slots[slot_index_to_boot].tries_remaining);
    }

out:
	return ret;
}

/*
 * 设置当前分区为可启动分区
 */
int setSlotSucceed() {
    struct AvbABData info;
    if (readABMisc(&info) == -1) {
        return -1;
    }
    int now_slot = get_current_slot(&info);
    if (now_slot == -1) {
        return -1;
    }

    for (size_t i = 0; i < 4; i++)
    {
        printf("info.mafic is %x\n",info.magic[i]);
    }
    if (avb_safe_memcmp(info.magic, AVB_AB_MAGIC, AVB_AB_MAGIC_LEN) != 0) {
        printf("Magic is incorrect.\n");
        return false;
    }
    if (info.slots[now_slot].priority != AVB_AB_MAX_PRIORITY) {
        /* Something could be wrong, correct it because this slot is bootable */
        printf("Warning: current slot priorty is %d != %d, Correct it!\n",
               info.slots[now_slot].priority, AVB_AB_MAX_PRIORITY);
        info.slots[now_slot].priority = AVB_AB_MAX_PRIORITY;
    }
    #ifdef SUCCESSFUL_BOOT
    info.slots[now_slot].tries_remaining = 0;
    info.slots[now_slot].successful_boot = 1;
    #endif
    #ifdef RETRY_BOOT
    info.slots[now_slot].tries_remaining = AVB_AB_MAX_TRIES_REMAINING;
    /* Clear suc boot flag anyway. Because it's chance that an older slot
     * used succ mode without this patch applied, while the other one
     * (new OTA slot) uses tries mode. We need to make sure not to confuse
     * u-boot, otherwise this slot will be unbootable.
     */
    info.slots[now_slot].successful_boot = 0;
    #endif
    info.last_boot = now_slot;

    AvbABData_update_crc(&info);

    if (writeABMisc(&info) != 0) {
        return -1;
    }
    return 0;
}

/*
 * 设置升级分区, 即另外一个分区优先级最高
 */
int setSlotActivity() {
    struct AvbABData info;
    if (readABMisc(&info) == -1) {
        return -1;
    }
    int now_slot = get_current_slot(&info);
    if (now_slot == -1) {
        return -1;
    }

    info.slots[now_slot].priority = AVB_AB_MAX_PRIORITY - 1;
    info.slots[1 - now_slot].priority = AVB_AB_MAX_PRIORITY;
    info.slots[1 - now_slot].tries_remaining = AVB_AB_MAX_TRIES_REMAINING;
    /* When switch to another slot, assume it is not suc boot, try it out! */
    info.slots[1 - now_slot].successful_boot = 0;

    AvbABData_update_crc(&info);

    if (writeABMisc(&info) != 0) {
        return -1;
    }
    return 0;
}

static int flash_write_partition(char *src_path, char *dest_path) {
    int ret = 0;
    struct stat image_buf;

    ret = stat(src_path, &image_buf);
    if (ret) {
        LOGE("stat %s failed.(%s)\n", src_path, strerror(errno));
        return ret;
    }

    if (!isMtdDevice()) {
        //block
        ret = block_write(src_path, image_buf.st_size, dest_path);
        if (ret) {
            LOGE("block_write failed.\n");
            return ret;
        }
    } else {
        //mtd
        ret = mtd_write(src_path, image_buf.st_size, dest_path);
        if (ret) {
            LOGE("mtd_write failed.\n");
            return ret;
        }
    }

    ret = comparefile(dest_path, src_path, 0, 0, image_buf.st_size);
    if (!ret) {
        LOGI("check %s ok.\n", dest_path);
    } else {
        LOGE("check %s failed.\n", dest_path);
        return ret;
    }

    LOGI("Write %s into %s successfully.\n", src_path, dest_path);
    if (remove(src_path)) {
        LOGE("remove %s error.(%s)\n", src_path, strerror(errno));
    }

    return ret;
}

/*
 * 只支持ENV方案，升级boot和system分区
 */
static int flash_write(char *image_dir, char *partition, char *extra_partition) {
    int slot;
    char src_path[64];
    char dest_uboot_path[64] = {0};
    char dest_boot_path[64] = {0};
    char dest_rootfs_path[64] = {0};
    char dest_extra_path[64] = {0};
    struct AvbABData info;

    if (readABMisc(&info) == -1)
        return -1;

    slot = get_current_slot(&info);
    if (slot == -1)
        return -1;

    if (!strcmp(partition, "all")) {
        sprintf(dest_uboot_path, "/dev/block/by-name/%s_%c", AB_UBOOT_NAME, (slot == 0) ? 'b' : 'a');
        sprintf(dest_boot_path, "/dev/block/by-name/%s_%c", AB_BOOT_NAME, (slot == 0) ? 'b' : 'a');
        sprintf(dest_rootfs_path, "/dev/block/by-name/%s_%c", AB_ROOTFS_NAME, (slot == 0) ? 'b' : 'a');
    } else if (!strcmp(partition, AB_UBOOT_NAME)) {
        sprintf(dest_uboot_path, "/dev/block/by-name/%s_%c", AB_UBOOT_NAME, (slot == 0) ? 'b' : 'a');
    } else if (!strcmp(partition, AB_BOOT_NAME)) {
        sprintf(dest_boot_path, "/dev/block/by-name/%s_%c", AB_BOOT_NAME, (slot == 0) ? 'b' : 'a');
    } else if (!strcmp(partition, AB_ROOTFS_NAME)) {
        sprintf(dest_rootfs_path, "/dev/block/by-name/%s_%c", AB_ROOTFS_NAME, (slot == 0) ? 'b' : 'a');
    } else {
        LOGE("Invalid parameter setting: --partition=%s.\n", partition);
        return -1;
    }

    if (dest_uboot_path[0]) {
        memset(src_path, 0, sizeof(src_path));
        sprintf(src_path, "%s/%s.img", image_dir, AB_UBOOT_NAME);
        if (!access(src_path, F_OK) && flash_write_partition(src_path, dest_uboot_path)) {
            LOGE("Write %s failed.\n", dest_uboot_path);
            return -1;
        }
    }
    if (dest_boot_path[0]) {
        memset(src_path, 0, sizeof(src_path));
        sprintf(src_path, "%s/%s.img", image_dir, AB_BOOT_NAME);
        if (!access(src_path, F_OK) && flash_write_partition(src_path, dest_boot_path)) {
            LOGE("Write %s failed.\n", dest_boot_path);
            return -1;
        }
    }
    if (dest_rootfs_path[0]) {
        memset(src_path, 0, sizeof(src_path));
        sprintf(src_path, "%s/%s.img", image_dir, AB_ROOTFS_NAME);
        if (!access(src_path, F_OK) && flash_write_partition(src_path, dest_rootfs_path)) {
            LOGE("Write %s failed.\n", dest_rootfs_path);
            return -1;
        }
    }
    if (extra_partition) {
        sprintf(dest_extra_path, "/dev/block/by-name/%s_%c", extra_partition, (slot == 0) ? 'b' : 'a');
        memset(src_path, 0, sizeof(src_path));
        sprintf(src_path, "%s/%s.img", image_dir, extra_partition);
        if (!access(src_path, F_OK)) {
            if (flash_write_partition(src_path, dest_extra_path)) {
                LOGE("Write %s failed.\n", dest_extra_path);
                return -1;
            }
        } else {
            LOGE("No found %s, skip.\n", src_path);
        }
    }

    return 0;
}

/*
 * AB系统OTA升级，同时更新MISC
 */
int miscUpdate(char *tar_path, char *save_dir, char *update_partition, char *extra_partition) {
    char savedir[64];
    char partition[16];
    char unpack_tar_cmd[128];
    struct AvbABData info;

    if (tar_path == NULL) {
        LOGE("'--tar_path=' must be set.\n");
        return -1;
    }

    if (save_dir == NULL) {
        snprintf(savedir, 64, "%s", UPDATE_TAR_DIRECTRY);
    } else {
        snprintf(savedir, 64, "%s", save_dir);
    }

    if (update_partition == NULL)
        sprintf(partition, "all");
    else
        sprintf(partition, "%s", update_partition);

    if (access(savedir, F_OK))
        mkdir(savedir, 0755);

    sprintf(unpack_tar_cmd, "tar -xf %s -C %s", tar_path, savedir);
    if (system(unpack_tar_cmd)) {
        LOGE("Unpack %s failed.\n", tar_path);
        return -1;
    }

    LOGI("tar path = %s\n", tar_path);
    LOGI("save path = %s\n", savedir);

    if (flash_write(savedir, partition, extra_partition)) {
        LOGE("Write flash error, exit upgrading.\n");
        return -1;
    }

    if (setSlotActivity()) {
        LOGE("Set slot activity failed.\n");
        return -1;
    }

    return 0;
}

/*
 * 显示MISC 分区的所有内容，方便调试
 */
void miscDisplay() {
    // 显示A/B 分区的misc 信息
    LOGI("===============Linux A/B=============\n");
    struct AvbABData info_ab;
    readABMisc(&info_ab);
    LOGI("magic : %s.\n", info_ab.magic);
    LOGI("version_major = %d.\n", info_ab.version_major);
    LOGI("version_minor = %d.\n", info_ab.version_minor);
    for (int i = 0; i < 2; i++) {
        LOGI("slot.[%d]->priority = %d.\n",        i, info_ab.slots[i].priority);
        LOGI("slot.[%d]->successful_boot = %d.\n", i, info_ab.slots[i].successful_boot);
        LOGI("slot.[%d]->tries_remaining = %d.\n", i, info_ab.slots[i].tries_remaining);
    }

    LOGI("last_boot : %d.\n", info_ab.last_boot);
    LOGI("calculate crc32: %x.\n", avb_htobe32(avb_crc32((const unsigned char*)&info_ab, sizeof(struct AvbABData) - sizeof(unsigned int))));
    LOGI("local crc32: %x.\n", info_ab.crc32);
    LOGI("sizeof(struct AvbABData) = %ld\n", sizeof(struct AvbABData));

    // // 显示格式化信息
    // LOGI("==============wipe================\n");
    // // 显示recovery 的misc 信息
    // char cmdline[20];
    // readCmdMisc(cmdline, sizeof(cmdline));
    // LOGI("cmdline = %s.\n", cmdline);
    // LOGI("==============recovery============\n");

    // bootloader_message boot;
    // get_bootloader_message(&boot);
    // LOGI("bootloader.command = %s.\n", boot.command);
    // LOGI("bootloader.recovery = %s.\n", boot.recovery);
    // LOGI("bootloader.status = %s.\n", boot.status);
    // LOGI("bootloader.needupdate = %x.\n", *((int *)boot.needupdate));
    // LOGI("bootloader.systemFlag = %s.\n", boot.systemFlag);
}
