/* SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#ifndef __OS_FWANALYSIS_FWANALYSIS_H__
#define __OS_FWANALYSIS_FWANALYSIS_H__
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #include define
*
*---------------------------------------------------------------------------------------------------------------------
*/


/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   #define / #typedef define
*
*---------------------------------------------------------------------------------------------------------------------
*/

typedef struct _FIRMWARE_INF
{
    //Firmware Info addr
    uint32_t Font12LogicAddress;     //char font 12
    uint32_t Font16LogicAddress;     //char font 16
    uint32_t IDBlockByteAddress;     //reserved
    uint32_t CodeLogicAddress;       // code
    uint32_t FontLogicAddress;       //char font 12/16
    uint32_t GBKLogicAddress;        //GBK
    uint32_t MenuLogicAddress;       //Ment
    uint32_t ImageLogicAddress;      //Image
    uint32_t LoadStartBase;
    uint32_t SystemDefaultParaAddress;  //defalut para
    uint16_t FirmwareYear;
    uint16_t FirmwareDate;
    uint16_t MasterVersion;
    uint16_t SlaveVersion;
    uint16_t SmallVersion;
} FIRMWARE_INF, *PFIRMWARE_INF;

/***********FIRMWARE_HEADER**********/
typedef struct _STRUCT_VERSION
{
    unsigned char major;
    unsigned char minor;
    unsigned short small;
} STRUCT_VERSION;

typedef struct
{
    unsigned short year;
    unsigned char month;
    unsigned char day;
} STRUCT_DATE;

typedef struct _FIRMWARE_HEADER
{
    unsigned char  magic[8]; //'RESC'
    unsigned char  chip[16];
    unsigned char  model[32];
    unsigned char  desc[16]; //description
    STRUCT_VERSION version;
    STRUCT_DATE release_date;
    unsigned int data_offset; //raw firmware offset
    unsigned int data_size;  //raw firmware size
    unsigned char  reserved1[4];
    unsigned char  digest_flag; //digest algorithm,default is 1(js_hash)
    unsigned char  reserved2[421];
} FIRMWARE_HEADER, *PFIRMWARE_HEADER; //head is 512 bytes


typedef struct _DATABASE_INFO
{
    uint32_t  capacity;           //(Sector为单位)  4Byte
    uint32_t  BlockSize;          //(Sector为单位)  2Byte
    uint32_t  PageSize;           //(Sector为单位)  1Byte

} DATABASE_INFO;

typedef struct fw_ab_slot_data_t
{
    /* Slot priority. Valid values range from 0 to AVB_AB_MAX_PRIORITY,
     * both inclusive with 1 being the lowest and AVB_AB_MAX_PRIORITY
     * being the highest. The special value 0 is used to indicate the
     * slot is unbootable.
     */
    uint8_t priority;

    /* Number of times left attempting to boot this slot ranging from 0
     * to AVB_AB_MAX_TRIES_REMAINING.
     */
    uint8_t tries_remaining;

    /* Non-zero if this slot has booted successfully, 0 otherwise. */
    uint8_t successful_boot;

    /* Reserved for future use. */
    uint8_t reserved[1];
} fw_ab_slot_data;


typedef struct fw_ab_data_t
{
    /* Magic number used for identification - see AVB_AB_MAGIC. */
    uint8_t magic[4];

    /* Version of on-disk struct - see AVB_AB_{MAJOR,MINOR}_VERSION. */
    uint8_t version_major;
    uint8_t version_minor;
    /* Padding to ensure |slots| field start eight bytes in. */
    uint8_t reserved1[2];

    /* Per-slot metadata. */
    fw_ab_slot_data slots[2];

    /* Reserved for future use. */
    uint8_t reserved2[12];

    /* CRC32 of all 28 bytes preceding this field. */
    rt_uint32_t jshash;
} fw_ab_data;

typedef struct user_ab_slot_data_t
{
    /* Slot priority. Valid values range from 0 to AVB_AB_MAX_PRIORITY,
     * both inclusive with 1 being the lowest and AVB_AB_MAX_PRIORITY
     * being the highest. The special value 0 is used to indicate the
     * slot is unbootable.
     */
    rt_uint8_t priority;

    /* Number of times left attempting to boot this slot ranging from 0
     * to AVB_AB_MAX_TRIES_REMAINING.
     */
    rt_uint8_t tries_remaining;

    /* Non-zero if this slot has booted successfully, 0 otherwise. */
    rt_uint8_t successful;

    /* Reserved for future use. */
    rt_uint8_t reserved[1];
} user_ab_slot_data;


typedef struct user_ab_data_t
{
    /* Magic number used for identification - see AVB_AB_MAGIC. */
    rt_uint8_t magic[4];

    /* Version of on-disk struct - see AVB_AB_{MAJOR,MINOR}_VERSION. */
    rt_uint8_t version_major;
    rt_uint8_t version_minor;
    /* Padding to ensure |slots| field start eight bytes in. */
    rt_uint8_t reserved1[2];

    /* Per-slot metadata. */
    user_ab_slot_data slots[2];

    /* Reserved for future use. */
    rt_uint8_t reserved2[12];

    /* CRC32 of all 28 bytes preceding this field. */
    rt_uint32_t jshash;
} user_ab_data;

/* Starting address of the A\B os boot data in memory. */
#define     OS_AB_DATA_PART_OFFSET             (1024 * 24)

/* Starting address of the A\B user data in memory. */
#define     USER_AB_DATA_OFFSET                (1024 * 32)


/* Magic for the A/B struct when serialized. */
#define AB_MAGIC "\0AB0"
#define AB_MAGIC_LEN 4

/* Versioning for the on-disk A/B metadata - keep in sync with avbtool. */
#define AB_MAJOR_VERSION 1
#define AB_MINOR_VERSION 0

/* Size of AvbABData struct. */
#define AB_DATA_SIZE 32

/* Maximum values for slot data */
#define AB_MAX_PRIORITY 15
#define AB_MAX_TRIES_REMAINING 3


#ifndef MIN
#define MIN(a, b) (a < b ? a : b)
#endif
#ifndef MAX
#define MAX(a, b) (a > b ? a : b)
#endif
/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   global variable declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern FIRMWARE_INF gstFwInf;
extern uint16_t  UpgradeTimes;
extern uint16_t  LoaderVer;
extern uint32_t  SysProgDiskCapacity;
extern uint32_t  SysProgRawDiskCapacity;
extern int32_t  FW1Valid, FW2Valid;
extern uint32_t FwSysOffset;
extern uint32_t IdbBlockOffset;

/*
*---------------------------------------------------------------------------------------------------------------------
*
*                                                   API Declare
*
*---------------------------------------------------------------------------------------------------------------------
*/
extern unsigned int jshash(unsigned int hash, char *str, unsigned int len);
extern void DumpData(char *buf, int bufSize, int radix);
extern rt_err_t fw_GetWifiMac(void *pWifiMac);
extern rt_err_t fw_GetBtMac(void *pBtMac);
extern rt_err_t fw_GetCustom(int snTypeId, void *pCustom, int len);
extern rt_err_t fw_WriteProductSn(void *pSn, int len);
extern rt_err_t fw_WriteBtMac(void *pBtMac, int len);
extern rt_err_t fw_WriteCustom(int snTypeId, void *pCustom, int len);
extern rt_err_t fw_CodePageDeInit(void);
extern rt_err_t FwResume(void);
extern rt_err_t FwSuspend(void);
extern rt_err_t DBResume(void);
extern rt_err_t DBSuspend(void);
extern rt_err_t fw_DBInit(void);
extern rt_err_t fw_CodePageLoad(void);
extern rt_err_t fw_CodePageUnLoad(void);
extern rt_err_t fw_GetProductSn(void *pSn);
extern void fw_GetDBInf(DATABASE_INFO *pstDataBaseInf);
extern rt_err_t fw_ReadDataBaseByByte(uint32_t Addr, uint8_t *pData, uint32_t length);
extern rt_err_t fw_ReadDataBaseBySector(uint32_t LBA, uint8_t *buf,  uint32_t SecCnt);
extern rt_err_t fw_WriteDataBaseBySector(uint32_t LBA, uint8_t *buf, uint32_t SecCnt);
extern void fw_Resource_Init(void);
extern void fw_Resource_DeInit(void);
extern rt_uint32_t fw_GetModle(void *modle, int fw_slot);
extern rt_uint32_t fw_GetChipType(void *chipType, int fw_slot);
extern rt_err_t fw_GetVersion(uint32_t *version, int fw_slot);
extern int fw_GetVersionStr(char *dest, int fw_slot);
extern int fw_CheckHash(rt_uint8_t fw_slot);

extern rt_bool_t fw_ab_data_verify(fw_ab_data *src, fw_ab_data *dest);
extern void fw_ab_data_init(fw_ab_data *data);
extern int fw_ab_data_write(const fw_ab_data *data);
extern rt_bool_t fw_slot_is_bootable(fw_ab_slot_data *slot);
extern int fw_ab_data_read(fw_ab_data *data);
extern int fw_slot_set_pending(uint32_t slot);
extern int fw_slot_set_active(uint32_t slot);
extern int fw_slot_get_current_running(uint32_t *cur_slot);
extern int fw_slot_reset_flag(uint32_t slot);
extern int fw_slot_change(uint32_t boot_slot);
extern fw_ab_data *fw_ab_data_get(void);
extern void fw_slot_info_dump(void);
#ifdef RT_SUPPORT_ROOT_AB
extern void user_ab_data_init(user_ab_data *data);
extern rt_bool_t user_ab_data_verify(user_ab_data *src, user_ab_data *dest);
extern int user_ab_data_write(const user_ab_data *data);
extern rt_bool_t user_slot_is_bootable(user_ab_slot_data *slot);
extern int user_ab_data_read(user_ab_data *data);
extern int user_slot_set_pending(uint32_t slot);
extern int user_slot_set_active(uint32_t slot);
extern int user_slot_get_current_running(uint32_t *cur_slot);
extern user_ab_data *user_ab_data_get(void);
#endif

#endif
