/**
 * aicwf_custom_utils.c
 *
 * host-ota function declarations
 *
 * Copyright (C) AICSemi 2018-2022
 */


#ifndef _AICWF_CUSTOM_UTILS_H_
#define _AICWF_CUSTOM_UTILS_H_

#include <linux/fs.h>
#include <linux/uaccess.h>
#include "rwnx_main.h"

#define IMAGE_INFO_SIZE         0x1000
#define UPGRADE_START_ADDR      0x08143000
#define UPGRADE_INFO_ADDR       (UPGRADE_START_ADDR)
#define UPGRADE_IMAGE_ADDR      (UPGRADE_START_ADDR + IMAGE_INFO_SIZE)

#define MCU_OTA_TIMEOUT         2000

enum OTA_STEP
{
    OTA_STEP_FLASH_ERASE,
    OTA_STEP_FR_PKG_WRITE,
    OTA_STEP_LT_PKG_WRITE,
    OTA_STEP_HEADER_WRITE,
};

enum OTA_STEP_STATUS
{
    OTA_STEP_FLASH_ERASE_OK,
    OTA_STEP_FLASH_ERASE_ERR,
    OTA_STEP_FR_PKG_WRITE_OK,
    OTA_STEP_FR_PKG_WRITE_ERR,
    OTA_STEP_LT_PKG_WRITE_OK,
    OTA_STEP_LT_PKG_WRITE_ERR,
    OTA_STEP_BIN_FILE_CRC_ERR,
    OTA_STEP_BIN_VERSION_ERR,
    OTA_STEP_HEADER_WRITE_OK,
    OTA_STEP_HEADER_WRITE_ERR,
};


int host_ota_test(const char * file_path);
void aicwf_mcu_ota_confirm_ready(void);

#endif

