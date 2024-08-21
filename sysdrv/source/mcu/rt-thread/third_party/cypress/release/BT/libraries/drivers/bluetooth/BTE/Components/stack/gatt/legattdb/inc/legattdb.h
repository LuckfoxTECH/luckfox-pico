#ifndef _LEATTDB_H_
#define _LEATTDB_H_
/*
********************************************************************
* THIS INFORMATION IS PROPRIETARY TO
* BROADCOM CORP.
*-------------------------------------------------------------------
*
*           Copyright (c) 2011 Broadcom Corp.
*                      ALL RIGHTS RESERVED
*
********************************************************************

 ********************************************************************
 *    File Name: legattdb.h
 *
 *    Abstract: This module implements the GATT DB for LE.
 *
 *    Functions:
 *            --
 *
 *    $History:$
 *
 ********************************************************************
*/


#include "data_types.h"
#include "leatt.h"
#include "gatt_api.h"


// The purpose of these 2 macros is to abstract the multi-byte access.
// Some CPU architecture has difficulty to directly access un-aligned
// muti-byte entities. We need to extract 1 byte at a time for ARM7
// based CPU + ARM toolset. For CM3, we don't need to do the byte
// access. ARM7 + Green Hills Toolset also do not need the byte access.
//#define LEATTDB_GET_UINT16( a )   ( * ((UINT16 *)(a)) )
//#define LEATTDB_SET_UINT16( a,b ) ( * ((UINT16 *)(a)) = (b) )
#define LEATTDB_GET_UINT16( a )   ( ((UINT16)(*((UINT8*)(a)+1)) << 8) + *(UINT8*)(a) )
#define LEATTDB_SET_UINT16( a,b ) {*((UINT8*)(a)) = ((b) & 0x00ff);\
                                    *((UINT8*)((a)+1)) = ((b) >> 8);}


//
#define LEATTDB_LAST_HANDLE   0xffff

//
#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
//              public data type definition.
#ifdef _WIN32
#include <pshpack1.h>
#endif
// GHS syntax.
#pragma pack(1)

///////////////////////////////////////////////////////////////////////////
// CHARACTERISTIC_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION Properties
#define LEGATTDB_CHAR_CLT_CHAR_CONF_NOTI              (0x1<<0)
#define LEGATTDB_CHAR_CLT_CHAR_CONF_IND               (0x1<<1)

typedef PACKED struct
{
    UINT16 handle ; // This is the handle of the entry.
    UINT8  perm;    // This is the permission.
    UINT8  len;    // This is the length of the entry. It excludes the header.
} LEGATTDB_ENTRY_HDR;

typedef PACKED struct
{
    UINT16 handle ; // This is the handle of the entry.
    UINT8  perm;    // This is the permission.
    UINT8  len;     // This is the length of the entry. It excludes the header.
    UINT8  maxLen;  // This is the max space for writable attribute. This byte is not
    // counted in the length. It is used for attribute truncation.
} LEGATTDB_WRITABLE_ENTRY_HDR;

typedef PACKED struct
{
    UINT16 handle ; // This is the handle of the entry.
    UINT8  perm;    // This is the permission.
    UINT16  len;     // This is the length of the entry. It excludes the header.
} LEGATTDB_ENTRY_HDR16;

typedef PACKED struct
{
    UINT16 handle ; // This is the handle of the entry.
    UINT8  perm;    // This is the permission.
    UINT16  len;     // This is the length of the entry. It excludes the header.
    UINT16  maxLen;  // This is the max space for writable attribute. This byte is not
    // counted in the length. It is used for attribute truncation.
} LEGATTDB_WRITABLE_ENTRY_HDR16;

#ifdef _WIN32
#include <poppack.h>
#endif
// GHS syntax.
#pragma pack()


// This call back takes one parameter.
typedef int (*LEGATTDB_WRITE_CB)(void *);




// This is bit [2].
#define LEGATTDB_PERM_FLAG_WRITE_CMD    LEGATTDB_PERM_WRITE_CMD
// This is bit [6].
#define LEGATTDB_PERM_FLAG_AUTH         LEGATTDB_PERM_AUTH_WRITABLE
// Internal  means internal access, permission check is waived.
#define LEGATTDB_PERM_FLAG_INTERNAL     (0x1ul << 31)


//////////////////////////////////////////////////////////////////////////////
//                      public interface declaration.

//
// This function should be called to initialize the variable.
void legattdb_init(void);

// This function will set DB with different value
void legattdb_SetDb(const UINT8 *ptr, UINT16 size);

// this function will search the LE GATT database to find the
int legattdb_readBlob(
    UINT8 conn_id,
    UINT16 handle,
    int offset,
    UINT8  *outPdu,
    UINT16 *outPduLen);


int legattdb_readMultipleReq(
    UINT8 conn_id,
    UINT8 *attrHandleList,
    int    numAttrHandles,
    UINT8  *outPdu,
    UINT16    *outPduLen);


//
int legattdb_findInfo(
    UINT16 startHandle,     // input param. start handle.
    UINT16 endHandle,       // input param, end handle
    UINT8  *outPdu,
    UINT16    outPduLen
);

int legattdb_readByGroupTypeUUID128(
    UINT16 startHandle,// input param. start handle.
    UINT16 endHandle,  // input param. end handle.
    UINT8 *uuid,       //
    UINT8  *outPdu,
    UINT16    outPduLen
);

int legattdb_findByTypeValue(
    UINT16 startHandle, //
    UINT16 endHandle,   //
    UINT16 uuid,       // attribute uuid.
    UINT8 *attrValue,   // attribute value
    int attrValueLen,   // length of attribute value.
    UINT8 *outPdu,
    UINT16 outPduLen
);


int legattdb_readByType(
    UINT8 conn_id,
    UINT16 startHandle,
    UINT16 endHandle,
    int    uuidLen, // size of uuid.
    UINT8 *uuid, //  pointer to uuid.
    UINT8  *outPdu,
    UINT16    *outPduLen
);

int legattdb_readHandle(
    UINT8 conn_id,
    UINT16 handle,      // handle to read.
    UINT8  *outPdu,
    UINT16 *outPduLen,
    UINT32 permFlag     //
);


int legattdb_writeHandle(
    UINT8 conn_id,
    UINT16 handle,      // handle to write.
    UINT16 offset,      // offset to write.
    UINT8 *attrValue,   // attribute value to write.
    UINT16 attrValLen,     // length of attribute value.
    UINT32 permFlag     // This is permission flag.
);

extern int  legattdb_gattPendingAttrWriteSize;

int legattdb_prepareWriteReq(UINT8 conn_idx,
                             UINT16 attrHandle, UINT8 *p_data,  int pduLen);


int legattdb_writeCmd(UINT8 conn_idx, UINT16 handle, UINT8 *attrVale, int attrLen);


// This function will dump the current database.
void legattdb_dumpDb(void);

// if 1, at least one notification is turned on
// if 0, all notifications are turned off
UINT8 legattdb_checkNotificationDb(void);

//////////////////////////////////////////////////////////////////////////////
//  Here are the functions that supports the database access.
//
// This function will return a pointer to next entry.
LEGATTDB_ENTRY_HDR *legattdb_nextEntry(LEGATTDB_ENTRY_HDR *p);


// This function returns the attribute handle of this entry.
UINT16 legattdb_getHandle(LEGATTDB_ENTRY_HDR *p);

// This will return a attribute UUID in the entry. If the attribute
// uuid is not 2 bytes. It will return 0x0 which is invalid uuid.
UINT16 legattdb_getAttrUUID16(LEGATTDB_ENTRY_HDR *p);

// This function will fill the buf with the UUID. The return value will indicate
// the length of UUID, 2 bytes or 16 bytes.
int    legattdb_getAttrUUID(LEGATTDB_ENTRY_HDR *h, UINT8 *buf);

// This is a convenient function. It returns the attribute value in the
// form of UUID16. Caller need to be sure this is a UUID16. Or else it

UINT16 legattdb_getAttrValueUUID16(LEGATTDB_ENTRY_HDR *p);

// This function will return a pointer to the attribute value.
UINT8 *legattdb_getAttrValue(LEGATTDB_ENTRY_HDR *p);

// this function will return the length of the attribute.
int    legattdb_getAttrValueLen(LEGATTDB_ENTRY_HDR *p);

UINT8 legattdb_getExtAttrValue(UINT8 conn_idx,
                               LEGATTDB_ENTRY_HDR *dbPtr,
                               UINT8 *p_value,
                               UINT16 *p_val_len, UINT16 offfset);

// This function change the length of current attriute length.
void   legattdb_setAttrValueLen(LEGATTDB_ENTRY_HDR *p, int len);

// This function return the maximum writable attribute length. If the attribute
// is not writable, it will return 0.
int    legattdb_getMaxWritableAttrValueLen(LEGATTDB_ENTRY_HDR *p);

// This function will update the writable attribute.
int legattdb_writeAttrValue(
    UINT8 conn_id, LEGATTDB_ENTRY_HDR *p, UINT16 offset, UINT8 *val, int len, UINT32 permFlag);

// this will get the permission byte.
int    legattdb_getPerm(LEGATTDB_ENTRY_HDR *p);

// this function will return a pointer to the actual attribute,
// skip over the header.
UINT8 *legattdb_getAttrPtr(LEGATTDB_ENTRY_HDR *p);

// This function will compare the UUID of _1 and _2 . It return TRUE if
// they are the same. if the size of the two uuid are not the same, they
// will be converted to 128 bit uuid for comparison.
int legattdb_uuidIsTheSame(
    int uuidLen_1, UINT8 *uuidPtr_1, int uuidLen_2, UINT8 *uuidPtr_2);

UINT32 legattdb_checkPermission(UINT8 conn_idx, int permToCheck, UINT32 perm, LEGATTDB_ENTRY_HDR *p);

void legattdb_connDown(void);
void legattdb_connUp(void);

// This function register a callback.
int  legattdb_regWriteHandleCb(LEGATTDB_WRITE_CB cb);
// This function de-register a callback.
void legattdb_deRegWriteHandleCb(LEGATTDB_WRITE_CB cb);


int legattdb_dispatchWriteCb(void *p);

UINT32 legattdb_checkEncAuthRequirement(UINT8 conn_idx);

// This function finds descriptor with specified UUID which belongs to the specific characteristic
UINT16 legattdb_findCharacteristicDescriptor(UINT16 char_handle, UINT16 descriptor_uuid);

// Check if value for requested db entry is an externally defined
// tGATTS_EXT_ATTR *legattdb_checkExtAttribute(LEGATTDB_ENTRY_HDR *p);
BOOLEAN legattdb_checkExtAttribute(LEGATTDB_ENTRY_HDR *p);

#ifdef __cplusplus
}
#endif
#endif // end of #ifndef _LEATTDB_H_
