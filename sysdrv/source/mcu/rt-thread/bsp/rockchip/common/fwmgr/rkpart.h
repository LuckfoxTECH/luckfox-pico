/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef _RK_PARTITION_
#define _RK_PARTITION_

//typedef enum
//{
//    PART_VENDOR     = 1 << 0,
//    PART_IDBLOCK    = 1 << 1,
//    PART_MISC       = 1 << 2,
//    PART_FW1        = 1 << 3,
//    PART_FW2        = 1 << 4,
//    PART_DATA       = 1 << 5,
//    PART_FONT1      = 1 << 6,
//    PART_FONT2      = 1 << 7,
//    PART_CHAR       = 1 << 8,
//    PART_MENU       = 1 << 9,
//    PART_UI         = 1 << 10,
//    PART_USER1      = 1 << 30,
//    PART_USER2      = 1 << 31
//} ENUM_PARTITION_TYPE;

typedef struct
{
    uint16_t  year;
    uint8_t   month;
    uint8_t   day;
    uint8_t   hour;
    uint8_t   min;
    uint8_t   sec;
    uint8_t   reserve;
} STRUCT_DATETIME, *PSTRUCT_DATETIME;

typedef struct
{
    uint32_t  uiFwTag; //"RKFP"
    STRUCT_DATETIME dtReleaseDataTime;
    uint32_t  uiFwVer;
    uint32_t  uiSize;//size of sturct,unit of uint8_t
    uint32_t  uiPartEntryOffset;//unit of sector
    uint32_t  uiBackupPartEntryOffset;
    uint32_t  uiPartEntrySize;//unit of uint8_t
    uint32_t  uiPartEntryCount;
    uint32_t  uiFwSize;//unit of uint8_t
    uint8_t   reserved[464];
    uint32_t  uiPartEntryCrc;
    uint32_t  uiHeaderCrc;
} STRUCT_FW_HEADER, *PSTRUCT_FW_HEADER;

//typedef struct
//{
//    uint8_t   szName[32];
//    ENUM_PARTITION_TYPE emPartType;
//    uint32_t  uiPartOffset;//unit of sector
//    uint32_t  uiPartSize;//unit of sector
//    uint32_t  uiDataLength;//unit of uint8_t
//    uint32_t  uiPartProperty;
//    uint8_t   reserved[76];
//} STRUCT_PART_ENTRY, *PSTRUCT_PART_ENTRY;

//typedef struct
//{
//    STRUCT_FW_HEADER hdr;     //0.5KB
//    STRUCT_PART_ENTRY part[12]; //1.5KB
//} STRUCT_PART_INFO, *PSTRUCT_PART_INFO;

typedef enum
{
    DEV_SN          = 1,
    DEV_WLAN_MAC    = 2,
    DEV_LAN_MAC     = 3,
    DEV_BT_MAC      = 4,
    DEV_IMEI        = 5,

    /* below is for user custom. */
    /*
    * Note: The custom sn tyoe ID must be same with RKDevInfoWriteTool.
    */
    DEV_OTA         = 9,    /* add for ota use */
    DEV_AUTH        = 10,
    DEV_ACTIVE      = 11,
    DEV_VENDOR_KEY  = 12,

} dev_sn_type_t;

extern uint32_t firmware_addr1;
extern uint32_t firmware_addr2;
extern uint32_t fw_part_size1;
extern uint32_t fw_part_size2;

extern uint32_t vendor_start_addr;
extern uint32_t vendor_end_addr;

extern uint32_t idb_start_addr;
extern uint32_t idb_part_size;

extern uint32_t data_start_addr;
extern uint32_t data_part_size;

extern uint32_t user_start_addr1;
extern uint32_t user_start_addr2;
extern uint32_t user_part_size;

extern uint32_t font1_start_addr;
extern uint32_t font2_start_addr;
extern uint32_t char_start_addr;
extern uint32_t menu_start_addr;
extern uint32_t ui_start_addr;

//extern STRUCT_PART_INFO g_partition;
//extern int32_t rk_partition_init(void);
//extern STRUCT_PART_ENTRY *get_part_entry(STRUCT_PART_INFO *part, ENUM_PARTITION_TYPE emPartType);
//extern int change_part_name(int emPartType, char* new_name);
extern int get_device_sn(dev_sn_type_t emDevSnType, char *strBuf, int len);
extern int write_device_sn(dev_sn_type_t emDevSnType, char *strBuf, int len);

#endif
