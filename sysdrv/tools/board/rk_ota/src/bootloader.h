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

#ifndef _RECOVERY_BOOTLOADER_H
#define _RECOVERY_BOOTLOADER_H

#include "common.h"

static const int BOOTLOADER_MESSAGE_OFFSET_IN_MISC = 16 * 1024;
#define MISC_OFFSET 2048    //A,B 结构体在偏移2K 的位置
#define MISC_OFFSET_CMDLINE 6144    //擦除命令，在偏移4K 的位置，配合挂载脚本使用
#define MISC_OFFSET_CUSTOM (10 * 1024) //CUSTOM, 8K (10K), length 1K

#define MISC_PAGES_AB 2         // A,B 结构体存在 2 PAGE内
#define MISC_COMMAND_PAGE_AB 1  // A,B 结构体存在 1 PAGE处，即2k

#define AB_SLOT_NUM  2

/* Magic for the A/B struct when serialized. */
#define AVB_AB_MAGIC "\0AB0"
#define AVB_AB_MAGIC_LEN 4

/* Versioning for the on-disk A/B metadata - keep in sync with avbtool. */
#define AVB_AB_MAJOR_VERSION 1
#define AVB_AB_MINOR_VERSION 0

/* Size of AvbABData struct. */
#define AVB_AB_DATA_SIZE 32

/* Maximum values for slot data */
#define AVB_AB_MAX_PRIORITY 15
#define AVB_AB_MAX_TRIES_REMAINING 7

#define MTD_PATH "/proc/mtd"
#define CMD_WIPE_USERDATA "cmd_wipe_userdata"

#define BLOCK_WRITE_LEN (16 * 1024)
#define AB_UBOOT_NAME "uboot"
#define AB_BOOT_NAME "boot"
#define AB_ROOTFS_NAME "system"
#define UPDATE_TAR_PATH "/mnt/sdcard/update_ota.tar"
#define UPDATE_TAR_DIRECTRY "/mnt/sdcard/rk_update"

/* Bootloader Message
 *
 * This structure describes the content of a block in flash
 * that is used for recovery and the bootloader to talk to
 * each other.
 *
 * The command field is updated by linux when it wants to
 * reboot into recovery or to update radio or bootloader firmware.
 * It is also updated by the bootloader when firmware update
 * is complete (to boot into recovery for any final cleanup)
 *
 * The status field is written by the bootloader after the
 * completion of an "update-radio" or "update-hboot" command.
 *
 * The recovery field is only written by linux and used
 * for the system to send a message to recovery or the
 * other way around.
 *
 * The systemFlag field is used for the system to send a message to recovery.
 */
struct bootloader_message {
    char command[32];
    char status[32];
    char recovery[768];
    char needupdate[4];
    char systemFlag[252];
};

/* Read and write the bootloader command from the "misc" partition.
 * These return zero on success.
 */
int get_bootloader_message(struct bootloader_message *out);
int set_bootloader_message(const struct bootloader_message *in);

typedef struct AvbABSlotData {
    /* Slot priority. Valid values range from 0 to AVB_AB_MAX_PRIORITY,
     * both inclusive with 1 being the lowest and AVB_AB_MAX_PRIORITY
     * being the highest. The special value 0 is used to indicate the
     * slot is unbootable.
     */
    unsigned char priority;//0,14,15

    /* Number of times left attempting to boot this slot ranging from 0
     * to AVB_AB_MAX_TRIES_REMAINING.
     */
    unsigned char tries_remaining;//7--,成功启动，设为0

    /* Non-zero if this slot has booted successfully, 0 otherwise. */
    unsigned char successful_boot;//0,1

    /* Reserved for future use. */
    unsigned char reserved[1];
}AvbABSlotData;

/* Struct used for recording A/B metadata.
 *
 * When serialized, data is stored in network byte-order.
 */
typedef struct AvbABData {
    /* Magic number used for identification - see AVB_AB_MAGIC. */
    unsigned char magic[AVB_AB_MAGIC_LEN];

    /* Version of on-disk struct - see AVB_AB_{MAJOR,MINOR}_VERSION. */
    unsigned char version_major; //AVB_AB_MAJOR_VERSION
    unsigned char version_minor; //AVB_AB_MINOR_VERSION

    /* Padding to ensure |slots| field start eight bytes in. */
    unsigned char reserved1[2];

    /* Per-slot metadata. */
    AvbABSlotData slots[2];

    /* Reserved for future use. */
    unsigned char last_boot;//默认a，上一次成功启动slot的标志位，0-->a，1-->b
    unsigned char reserved2[11];

    /* CRC32 of all 28 bytes preceding this field. */
    unsigned int crc32;
}AvbABData;

int setSlotActivity();
int setSlotSucceed();
int readMisc(struct AvbABData *info);
void display(struct AvbABData info);
void miscDisplay() ;
int miscUpdate(char *tar_path, char *save_dir, char *update_partition, char *extra_partition);
int wipe_userdata(int auto_reboot);
int writeCustomMiscCmdline(void);
int readCustomMiscCmdline(void);
int cleanCustomMiscCmdline(void);

//bool wipe_userdata(bool auto_reboot);

#endif
