#ifndef _LEATT_H_
#define _LEATT_H_
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
 *    File Name: leatt.h
 *
 *    Abstract: This module implements the GATT for LE.
 *
 *    Functions:
 *            --
 *
 *    $History:$
 *
 ********************************************************************
*/

// This is for WIN32 platform.
#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

#define LEATT_ATT_MTU 23


//////////////////////////////////////////////////////////////////////////////
//      constant or data type definition from the ATT/GATT specification.
//
#ifdef _WIN32
#include <pshpack1.h>
#endif
// GHS syntax.
#pragma pack(1)

#define LEATT_HANDLE_RESERVED                                0x0000



#define LEATT_OPCODE_ERR_RSP                                 0x1
#define LEATT_OPCODE_EXCHANGE_MTU_REQ                        0x2
#define LEATT_OPCODE_EXCHANGE_MTU_RSP                        0x3
#define LEATT_OPCODE_FIND_INFO_REQ                           0x4
#define LEATT_OPCODE_FIND_INFO_RSP                           0x5
#define LEATT_OPCODE_FIND_BY_TYPE_VALUE_REQ                  0x6
#define LEATT_OPCODE_FIND_BY_TYPE_VALUE_RSP                  0x7
#define LEATT_OPCODE_READ_BY_TYPE_REQ                        0x8
#define LEATT_OPCODE_READ_BY_TYPE_RSP                        0x9
#define LEATT_OPCODE_READ_REQ                                0xa
#define LEATT_OPCODE_READ_RSP                                0xb
#define LEATT_OPCODE_READ_BLOB_REQ                           0xc
#define LEATT_OPCODE_READ_BLOB_RSP                           0xd
#define LEATT_OPCODE_READ_MULTI_REQ                          0xe
#define LEATT_OPCODE_READ_MULTI_RSP                          0xf
#define LEATT_OPCODE_READ_BY_GROUP_TYPE_REQ                  0x10
#define LEATT_OPCODE_READ_BY_GROUP_TYPE_RSP                  0x11
#define LEATT_OPCODE_WRITE_REQ                               0x12
#define LEATT_OPCODE_WRITE_RSP                               0x13

// 0x14 and 0x15 are not defined.
//

#define LEATT_OPCODE_PREPARE_WRITE_REQ                       0x16
#define LEATT_OPCODE_PREPARE_WRITE_RSP                       0x17
#define LEATT_OPCODE_EXECUTE_WRITE_REQ                       0x18
#define LEATT_OPCODE_EXECUTE_WRITE_RSP                       0x19
// 0x1a is not defined.
#define LEATT_OPCODE_HANDLE_VALUE_NOTIFICATION               0x1b
#define LEATT_OPCODE_HANDLE_VALUE_INDICATION                 0x1d
#define LEATT_OPCODE_HANDLE_VALUE_CONF                       0x1e


// Vol 3, Part F, 3.3.1. Attribute Pdu Format, bit 5-0 Method.
#define LEATT_OPCODE_MASK                                    0x3f
#define LEATT_OPCODE_AUTH_SIGN_FLAG                          0x80
#define LEATT_OPCODE_COMMAND_FLAG                            0x40


typedef PACKED struct
{
    UINT8 attrCode; // attribute code.
} LEATT_PDU_HDR;



#define LEATT_ERR_CODE_NO_ERROR                            GATT_SUCCESS
#define LEATT_ERR_CODE_INVALID_HANDLE                      GATT_INVALID_HANDLE
#define LEATT_ERR_CODE_READ_NOT_PERMITTED                  GATT_READ_NOT_PERMIT
#define LEATT_ERR_CODE_WRITE_NOT_PERMITTED                 GATT_WRITE_NOT_PERMIT
#define LEATT_ERR_CODE_INVALID_PDU                         GATT_INVALID_PDU
#define LEATT_ERR_CODE_INSUFFICIENT_AUTHENTICATION         GATT_INSUF_AUTHENTICATION
#define LEATT_ERR_CODE_REQ_NOT_SUPPORTED                   GATT_REQ_NOT_SUPPORTED
#define LEATT_ERR_CODE_INVALID_OFFSET                      GATT_INVALID_OFFSET
#define LEATT_ERR_CODE_INSUFFICIENT_AUTHORIZATION          GATT_INSUF_AUTHORIZATION
#define LEATT_ERR_CODE_PREPARE_QUEUE_FULL                  GATT_PREPARE_Q_FULL
#define LEATT_ERR_CODE_ATTRIBUTE_NOT_FOUND                 GATT_NOT_FOUND
#define LEATT_ERR_CODE_ATTRIBUTE_NOT_LONG                  GATT_NOT_LONG
#define LEATT_ERR_CODE_INSUFFICIENT_ENC_KEY_SIZE           GATT_INSUF_KEY_SIZE
#define LEATT_ERR_CODE_INVALID_ATTR_VALUE_LENGTH           GATT_INVALID_ATTR_LEN
#define LEATT_ERR_CODE_UNLIKELY_ERROR                      GATT_ERR_UNLIKELY
#define LEATT_ERR_CODE_INSUFFICIENT_ENCRYPTION             GATT_INSUF_ENCRYPTION
#define LEATT_ERR_CODE_UNSUPPORTED_GROUP_TYPE              GATT_UNSUPPORT_GRP_TYPE
#define LEATT_ERR_CODE_INSUFFICIENT_RESOURCES              GATT_INSUF_RESOURCE
// 0x12 - 0x7F are reserved.
// 0x80 - 0xff are application error code defined by higher layer specification.
#define LEATT_ERR_CODE_NOT_EXT_ATTR                         0xff

#define LEATT_ERR_CODE_RESERVED_SKIP_AFTER_HOOK            (0x1ul <<31)


#define LEATT_CLIENT_CONFIG_NOTIFICATION                        0x01
#define LEATT_CLIENT_CONFIG_INDICATION                          0x02




typedef PACKED struct
{
    UINT8   attrCode;   // 0x1 = error response
    UINT8   reqOpcode;  // opcode that generate this error.
    UINT16  attrHandleInError;
    UINT8   errCode;    // error code.

} LEATT_PDU_ERR_RSP;

typedef PACKED struct
{
    UINT8   attrCode;   // 0x2 = mtu exchange request
    UINT16  mtu;
} LEATT_PDU_MTU_EXCHANGE_REQ;


typedef PACKED struct
{
    UINT8   attrCode;   // 0x3 = mtu exchange response
    UINT16  mtu;
} LEATT_PDU_MTU_EXCHANGE_RSP;


typedef PACKED struct
{
    UINT8   attrCode;   // 0x4 = find information request
    UINT16  startHandle;
    UINT16  endHandle;
} LEATT_PDU_FIND_INFO_REQ;


#define LEATT_INFO_RSP_FORMAT_UUID16        0x1
#define LEATT_INFO_RSP_FORMAT_UUID128       0x2

typedef PACKED struct
{
    UINT8   attrCode;   // 0x5 = find information response
    UINT8   format;
    // the data follows the header.
} LEATT_PDU_FIND_INFO_RSP_HDR;

typedef PACKED struct
{
    UINT8   attrCode;   // 0x6 = find by type value request
    UINT16  startHandle;
    UINT16  endHandle;
    UINT16  attrType;
    // attribute value follows the hdr
} LEATT_PDU_FIND_BY_TYPE_VALUE_REQ_HDR;

typedef PACKED struct
{
    UINT8   attrCode;   // 0x7 = find by type value response
    // a list of 1 or more handle informations.
} LEATT_PDU_FIND_BY_TYPE_VALUE_RSP_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0x8 = read by type request
    UINT16  startHandle;
    UINT16  endHandle;
    // 2 or 16 byte UUID.
} LEATT_PDU_READ_BY_TYPE_REQ_HDR;

typedef PACKED struct
{
    UINT8   attrCode;   // 0x9 = read by type response
    UINT8   length;
    // attribute data list.
} LEATT_PDU_READ_BY_TYPE_RSP_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0xa = read request
    UINT16  attrHandle;
} LEATT_PDU_READ_REQ_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0xb = read response
    // attribute value
} LEATT_PDU_READ_RSP_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0xc = read blob request
    UINT16  attrHandle;
    UINT16  valueOffset; // offset of the first byte.
} LEATT_PDU_READ_BLOB_REQ;


typedef PACKED struct
{
    UINT8   attrCode;   // 0xd = read blob response
    // part attribute value
} LEATT_PDU_READ_BLOB_RSP_HDR;

typedef PACKED struct
{
    UINT8   attrCode;   // 0xe = read multi request
    // set of 2 or more handles.
} LEATT_PDU_READ_MULTI_REQ_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0xf = read multi response
    // set of 2 or more values.
} LEATT_PDU_READ_MULTI_RSP_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0x10 = read by group type request
    UINT16  startHandle;
    UINT16  endHandle;
    // 2 or 16 bytes of UUID, attribute group type
} LEATT_PDU_READ_BY_GROUP_TYPE_REQ_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0x11 = read by group type response.
    UINT8   length;     // size of earch attribute data
    // a list of attribute data.
} LEATT_PDU_READ_BY_GROUP_TYPE_RSP_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0x12 = Write Req.
    UINT16  attrHandle; // handle to write to.
    // Attribute Value
} LEATT_PDU_WRITE_HDR;


typedef PACKED struct
{
    UINT8   attrCode;   // 0x13 = Write Rsp.
} LEATT_PDU_WRITE_RSP_HDR;


typedef PACKED struct
{
    UINT8   attrCode;     // 0x16 = Prepare write.
    UINT16  attrHandle;   //
    UINT16  valOffset;    //
    // here follows the value.
} LEATT_PDU_PREPARE_WRITE_REQ_HDR;


typedef PACKED struct
{
    UINT8   attrCode;     // 0x17 = prepare write rsp
    UINT16  attrHandle;   //
    UINT16  valOffset;    //
    // here follows the value.
} LEATT_PDU_PREPARE_WRITE_RSP_HDR;



#define LEATT_PDU_EXECUTE_WRITE_REQ_CANCEL       0x0
#define LEATT_PDU_EXECUTE_WRITE_REQ_IMMEDIATE    0x1
typedef PACKED struct
{
    UINT8   attrCode;     // 0x18 = execute write.
    UINT8   flag;         //
} LEATT_PDU_EXECUTE_WRITE_REQ_HDR;


typedef PACKED struct
{
    UINT8   attrCode;     // 0x19 = execute write rsp.
} LEATT_PDU_EXECUTE_WRITE_RSP_HDR;


typedef PACKED struct
{
    UINT8   attrCode;     // 0x1b = Handle Value Notification
    UINT16  handle;
    // here starts the value.
} LEATT_PDU_NOTIFICATION_HDR;



typedef PACKED struct
{
    UINT8   attrCode;     // 0x1b = Handle Value Notification
    UINT16  handle;
    // here starts the value.
} LEATT_PDU_INDICATION_HDR;




typedef PACKED struct
{
    UINT8 len;
    UINT8 pdu[LEATT_ATT_MTU];
} LEATT_INTERNAL_PDU;


typedef PACKED struct
{
    void *msgPtr;// this is the msg ptr.
    int   msgLen; // this is the length of the msg.
    void *cb ; // this is the notification callback.
} LEATT_INDICATION_MSG;

typedef void (*LEATT_TRIPLE_PARAM_CB)(int, int, UINT8 *);
typedef void (*LEATT_NO_PARAM_CB)(void);

// Vol 3, Part F, 3.4.5.4
#define LEATT_AUTH_SIGNATURE_LENGTH                          12

// Vol 3. Part F, 2.4.5
#define LEATT_AUTH_SIGNATURE_COUNTER_SIZE                    4


#ifdef _WIN32
#include <poppack.h>
#endif
// GHS syntax.
#pragma pack()


//////////////////////////////////////////////////////////////////////////////
//                  Start of function declaration.
//


// called at start up.
void leatt_init(void);

// This will send an ATT Error response.
void leatt_sendErrResponse(int errCode, int reqOpcode, int handleInError);

// This will send an ATT Write Response.
void leatt_sendWriteRsp(void);

// This will send a notification msg.
void leatt_sendNotification(LEATT_PDU_NOTIFICATION_HDR *hdr, int attrLen);

// This will send a indication msg.
void leatt_sendIndication(LEATT_PDU_INDICATION_HDR *hdr, int attrLen, LEATT_NO_PARAM_CB cb);

// This will send a notification msg.
void leatt_sendHandleValueConf(void);



// This will register callback
void leatt_regFindmeRspCb(LEATT_TRIPLE_PARAM_CB cb);
//Client Functions
//
// This will register callback
void leatt_regHandleValueConfCb(LEATT_NO_PARAM_CB cb);

// This will register callback
void leatt_regReadRspCb(LEATT_TRIPLE_PARAM_CB cb);

// This will register callback
void leatt_regReadByTypeRspCb(LEATT_TRIPLE_PARAM_CB cb);

// This will register callback
void leatt_regReadByGroupTypeRspCb(LEATT_TRIPLE_PARAM_CB cb);

// This will register callback
void leatt_regWriteRspCb(LEATT_NO_PARAM_CB cb);

// This will register callback
void leatt_regNotificationCb(LEATT_TRIPLE_PARAM_CB cb);

// This will register callback
void leatt_regIndicationCb(LEATT_TRIPLE_PARAM_CB cb);

// This will register transaction timeout callback.
void leatt_regTransactionTimeoutCb(LEATT_NO_PARAM_CB  cb);


// This will send a write request
void leatt_sendWriteReq(LEATT_PDU_WRITE_HDR *msg, int len);

// This will send a write command
void leatt_sendWriteCmd(LEATT_PDU_WRITE_HDR *msg, int len);

// This will send a read request
void leatt_sendReadReq(LEATT_PDU_READ_REQ_HDR *msg);

// This will send a read by type Request
void leatt_sendReadByTypeReq(LEATT_PDU_READ_BY_TYPE_REQ_HDR *msg, int len);

// This will send a read by group type Request
void leatt_sendReadByGroupTypeReq(LEATT_PDU_READ_BY_GROUP_TYPE_REQ_HDR *msg, int len);




// This is a convenient function to map 16 bits UUID to 128 bits.
void leatt_mapUUID16ToUUID128(UINT16 uuid, UINT8 *uuid128Holder);

// This function will only update the 2 bytes of UUID.
void leatt_updateUUID16OfUUID128(UINT16 uuid, UINT8 *uuid128Holder);

void leatt_connDown(void);


#endif // end of #ifndef _LEATT_H_
