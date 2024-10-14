/*****************************************************************************
**
**  Name:       vdp_api.h
**
**  Description:Interface to VDP Application Programming Interface
**
**  Copyright (c) 2004-2009, BROADCOM Corp., All Rights Reserved.
**  BROADCOM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef VDP_API_H
#define VDP_API_H
#include "sdp_api.h"
#include "avdt_api.h"

/*****************************************************************************
**  constants
*****************************************************************************/

/* AV Media Types */
#define VDP_MEDIA_TYPE_AUDIO         AVDT_MEDIA_AUDIO  /* 0x00 audio media type */
#define VDP_MEDIA_TYPE_VIDEO         AVDT_MEDIA_VIDEO  /* 0x01 video media type */
#define VDP_MEDIA_TYPE_MULTI         AVDT_MEDIA_MULTI  /* 0x02 multimedia media type */

/* AV Media Codec Type (Video Codec ID) */
#define VDP_MEDIA_CT_H263_P0         0x01    /* H.263 baseline */
#define VDP_MEDIA_CT_MPEG4           0x02    /* MPEG-4 Visual Simple Profile */
#define VDP_MEDIA_CT_H263_P3         0x03    /* H.263 profile 3 */
#define VDP_MEDIA_CT_H263_P8         0x04    /* H.263 profile 8 */
#define VDP_MEDIA_CT_VEND            0xFF    /* Non-VDP */

/* Company ID in BT assigned numbers */
#define VDP_BT_VENDOR_ID      15    /* Broadcom Corporation */
#define VDP_VENDOR_INFO_LEN   8     /* the length for vendor specific codec info
                                     * (does not include "Vendor Specific Value") */

/* vendor specific codec ID */
#define VDP_CODEC_ID_H264     1     /* Non-VDP codec ID - H.264 */
#define VDP_CODEC_ID_IMG      2     /* Non-VDP codec ID - images/slideshow */

#define VDP_SUCCESS           0     /* Success */
#define VDP_FAIL              0x0A  /* Failed */
#define VDP_BUSY              0x0B  /* VDP_FindService is already in progress */
#define VDP_INVALID_PARAMS    0x0C  /* bad parameters */
#define VDP_WRONG_CODEC       0x0D  /* wrong codec info */
#define VDP_BAD_CODEC_TYPE    0xC1  /* Media Codec Type is not valid  */
#define VDP_NS_CODEC_TYPE     0xC2  /* Media Codec Type is not supported */
#define VDP_INVALID_LEVEL     0xC3  /* Level is not valid or multiple values have been selected  */
#define VDP_NS_LEVEL          0xC4  /* Level is not supported  */
#define VDP_BAD_CP_TYPE       0xE0  /* The requested CP Type is not supported. */
#define VDP_BAD_CP_FORMAT     0xE1  /* The format of Content Protection Service Capability/Content Protection Scheme Dependent Data is not correct. */

typedef UINT8 tVDP_STATUS;


/*****************************************************************************
**  H.263 Constants
*****************************************************************************/
/* H.263 "basic" payload header size (RFC2429 4.1).
 * The actual header size is 2 + 1(if VRC) + PLEN */
#define VDP_H263_PL_HDR_LEN         2

/* the LOSC of H.263 baseline  Video media codec capabilitiy */
#define VDP_H263_INFO_LEN           3

/* for Codec Specific Information Element */
#define VDP_H263_IE_LEVEL_MASK      0xE0    /* b7-b5 level */
#define VDP_H263_IE_LEVEL10         0x80    /* b7: level 10  */
#define VDP_H263_IE_LEVEL20         0x40    /* b6: level 20  */
#define VDP_H263_IE_LEVEL30         0x20    /* b5: level 30  */


/* general H.263 payload header */
#define VDP_H263_HDR_P_MSK          0x0400
#define VDP_H263_HDR_V_MSK          0x0200
#define VDP_H263_PLEN_MSK           0x01F8
#define VDP_H263_PLEN_SHIFT         3
#define VDP_H263_PEBIT_MSK          0x0007

/*****************************************************************************
**  Type Definitions
*****************************************************************************/
/*****************************************************************************
**  MPEG-4 Constants
*****************************************************************************/

/* the LOSC of MPEG-4 media codec capabilitiy */
#define VDP_MPEG_INFO_LEN           3

/* for Codec Specific Information Element */
#define VDP_MPEG_IE_LEVEL_MASK      0xF0    /* b7-b4 level */
#define VDP_MPEG_IE_LEVEL0          0x80    /* b7: level 0  */
#define VDP_MPEG_IE_LEVEL1          0x40    /* b6: level 1  */
#define VDP_MPEG_IE_LEVEL2          0x20    /* b5: level 2  */
#define VDP_MPEG_IE_LEVEL3          0x10    /* b4: level 3  */

/*****************************************************************************
**  VENDOR Constants
*****************************************************************************/

/* the LOSC of Non-VDP  Video media codec capabilitiy
  (does not include Vendor specific value) */
#define VDP_VENDOR_INFO_LEN             8

/*****************************************************************************
**  These Constants are used by BTW
*****************************************************************************/
/* for Codec Specific Information Element */
#define VDT_IE_LEVEL_H263_10    0x80 /* H.263 Level 10. */
#define VDT_IE_LEVEL_H263_20    0x40 /* H.263 Level 20. */
#define VDT_IE_LEVEL_H263_30    0x20 /* H.263 Level 30. */
#define VDT_IE_LEVEL_MPEG4_0    0x80 /* MPEG-4 Level 0. */
#define VDT_IE_LEVEL_MPEG4_1    0x40 /* MPEG-4 Level 1. */
#define VDT_IE_LEVEL_MPEG4_2    0x20 /* MPEG-4 Level 2. */
#define VDT_IE_LEVEL_MPEG4_3    0x10 /* MPEG-4 Level 3. */

/*****************************************************************************
**  type definitions
*****************************************************************************/

/* This data type is used in VDP_FindService() to initialize the SDP database
 * to hold the result service search. */
typedef struct
{
    UINT32              db_len;  /* Length, in bytes, of the discovery database */
    UINT16              num_attr;/* The number of attributes in p_attrs */
    tSDP_DISCOVERY_DB  *p_db;    /* Pointer to the discovery database */
    UINT16             *p_attrs; /* The attributes filter. If NULL, VDP API sets the attribute filter
                                  * to be ATTR_ID_SERVICE_CLASS_ID_LIST, ATTR_ID_BT_PROFILE_DESC_LIST,
                                  * ATTR_ID_SERVICE_NAME and ATTR_ID_PROVIDER_NAME.
                                  * If not NULL, the input is taken as the filter. */
} tVDP_SDP_DB_PARAMS;

/* This data type is used in tVDP_FIND_CBACK to report the result of the SDP discovery process. */
typedef struct
{
    UINT16  service_len;    /* Length, in bytes, of the service name */
    UINT16  provider_len;   /* Length, in bytes, of the provider name */
    char   *p_service_name; /* Pointer the service name.  This character string may not be null terminated.
                             * Use the service_len parameter to safely copy this string */
    char   *p_provider_name;/* Pointer the provider name.  This character string may not be null terminated.
                             * Use the provider_len parameter to safely copy this string */
    UINT16  avdt_version;   /* AVDTP protocol version */
} tVDP_Service;

/* This is the callback to notify the result of the SDP discovery process. */
typedef void (tVDP_FIND_CBACK)(BOOLEAN found, tVDP_Service *p_service);


/*****************************************************************************
**  external function declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif
/******************************************************************************
**
** Function         VDP_AddRecord
**
** Description      This function is called by a server application to add
**                  SRC or SNK information to an SDP record.  Prior to
**                  calling this function the application must call
**                  SDP_CreateRecord() to create an SDP record.
**
**                  Input Parameters:
**                      service_uuid:  Indicates SRC or SNK.
**
**                      p_service_name:  Pointer to a null-terminated character
**                      string containing the service name.
**
**                      p_provider_name:  Pointer to a null-terminated character
**                      string containing the provider name.
**
**                      sdp_handle:  SDP handle returned by SDP_CreateRecord().
**
**                  Output Parameters:
**                      None.
**
** Returns          VDP_SUCCESS if function execution succeeded,
**                  VDP_INVALID_PARAMS if bad parameters were given.
**                  VDP_FAIL if function execution failed.
**
******************************************************************************/
VDP_API extern tVDP_STATUS VDP_AddRecord(UINT16 service_uuid, char *p_service_name, char *p_provider_name,
        UINT32 sdp_handle);

/******************************************************************************
**
** Function         VDP_FindService
**
** Description      This function is called by a client application to
**                  perform service discovery and retrieve SRC or SNK SDP
**                  record information from a server.  Information is
**                  returned for the first service record found on the
**                  server that matches the service UUID.  The callback
**                  function will be executed when service discovery is
**                  complete.  There can only be one outstanding call to
**                  VDP_FindService() at a time; the application must wait
**                  for the callback before it makes another call to
**                  the function.
**
**                  Input Parameters:
**                      service_uuid:  Indicates SRC or SNK.
**
**                      bd_addr:  BD address of the peer device.
**
**                      p_db:  Pointer to the information to initialize
**                             the discovery database.
**
**                      p_cback:  Pointer to the VDP_FindService()
**                      callback function.
**
**                  Output Parameters:
**                      None.
**
** Returns          VDP_SUCCESS if function execution succeeded,
**                  VDP_INVALID_PARAMS if bad parameters were given.
**                  VDP_BUSY if discovery is already in progress.
**                  VDP_FAIL if function execution failed.
**
******************************************************************************/
VDP_API extern tVDP_STATUS VDP_FindService(UINT16 service_uuid, BD_ADDR bd_addr,
        tVDP_SDP_DB_PARAMS *p_db, tVDP_FIND_CBACK *p_cback);

/******************************************************************************
**
** Function         VDP_SetTraceLevel
**
** Description      Sets the trace level for VDP. If 0xff is passed, the
**                  current trace level is returned.
**
**                  Input Parameters:
**                      new_level:  The level to set the VDP tracing to:
**                      0xff-returns the current setting.
**                      0-turns off tracing.
**                      >= 1-Errors.
**                      >= 2-Warnings.
**                      >= 3-APIs.
**                      >= 4-Events.
**                      >= 5-Debug.
**
** Returns          The new trace level or current trace level if
**                  the input parameter is 0xff.
**
******************************************************************************/
VDP_API extern UINT8 VDP_SetTraceLevel(UINT8 new_level);

/*******************************************************************************
**
** Function         VDP_Init
**
** Description      This function is called at stack startup to allocate the
**                  control block (if using dynamic memory), and initializes the
**                  control block and tracing level.
**
** Returns          void
**
*******************************************************************************/
VDP_API extern void VDP_Init(void);

/*******************************************************************************
**
** Function         VDP_SetCodecInfo
**
** Description      This function is called by a client application to
**                  set codec_info for
**                  H.263 and MPEG-4 codecs.
**
**                  Input Parameters:
**                      p_codec_info:  Pointer to set codec_info.
**
**                      media_type:  Type of media. Should be one of the AVDT_MEDIA_XXX
**                                   constants defined in avdt_api.h
**
**                      codec_type:  Codec type. Should be one of the VDT_MEDIA_CT_XXX
**                                   constants defined in vdp_api.h
**
**                      levels_mask: Bit mask of supported levels. Should be a combination
**                                   of VDT_IE_LEVEL_XXX bits. Bit VDT_IE_LEVEL_H263_10 or
**                                   bit VDT_IE_LEVEL_MPEG4_0 will be set if even isn't specified.
**
**                  Output Parameters:
**                      None.
**
** Returns          VDP_SUCCESS if function execution succeeded,
**                  VDP_INVALID_PARAMS if bad parameters are given.
**
*******************************************************************************/
VDP_API extern tVDP_STATUS VDP_SetCodecInfo(UINT8 *p_codec_info, UINT8 media_type,
        UINT8 codec_type, UINT8 levels_mask);

/*******************************************************************************
**
** Function         VDP_GetCodecInfo
**
** Description      This function is called by a client application to
**                  get codec_info for
**                  H.263 and MPEG-4 codecs.
**
**                  Input Parameters:
**                      p_codec_info:  Pointer get codec_info from.
**
**                  Output Parameters:
**                      p_media_type:  Where to place Type of media. Can be NULL if caller
**                                     doesn't want to get value. Will be be one of the
**                                     AVDT_MEDIA_XXX constants defined in avdt_api.h
**
**                      p_codec_type:  Where to place Codec type. Can be NULL if caller
**                                     doesn't want to get value. Will be one of the
**                                     VDT_MEDIA_CT_XXX constants defined in vdp_api.h
**
**                      p_levels_mask: Where to place Bit mask of supported levels. Can be
**                                     NULL if caller doesn't want to get value. Will be
**                                     a combination of VDT_IE_LEVEL_XXX bits.
**
** Returns          VDP_SUCCESS if function execution succeeded,
**                  VDP_INVALID_PARAMS if bad parameters are given.
**                  VDP_WRONG_CODEC if codec_info of p_cfg contains wrong data
**
*******************************************************************************/
VDP_API extern tVDP_STATUS VDP_GetCodecInfo(UINT8 *p_codec_info, UINT8 *p_media_type,
        UINT8 *p_codec_type, UINT8 *p_levels_mask);

/*******************************************************************************
**
** Function         VDP_BldH263MplHdr
**
** Description      This function is called by a client application to
**                  build the H.263 payload header.
**
**                  Input Parameters:
**                      p_data:  Pointer to set H.263 payload header.
**                      p:  P bit.
**
**                      v:  if TRUE, add 8 bit information for VRC
**
**                      plen:  len of extra picture header
**
**                      pebit: the number of bits that shall be ignored in
**                             the last byte of picture header.
**
**                  Output Parameters:
**                      p_data: contents in changed by VDP_H263_PL_HDR_LEN bytes
**                              if VDP_SUCCESS.
**
** UINT8          UINT8: size of the Media Payload header.
**
*******************************************************************************/
VDP_API extern UINT8 VDP_BldH263MplHdr(UINT8 *p_data, BOOLEAN p, BOOLEAN v,
                                       UINT8 plen, UINT8 pebit);

/*******************************************************************************
**
** Function         VDP_PrsH263MplHdr
**
** Description      This function is called by a client application to
**                  parse the H.263 payload header.
**
** NOTE             Currently, VC02 can not decode V=1.
**                  If V is 1, the application is advised to close the connection.
**
** Returns          The length of the H.263 payload header.
**
*******************************************************************************/
VDP_API extern UINT8 VDP_PrsH263MplHdr(UINT8 *p_data, BOOLEAN *p_p, BOOLEAN *p_v,
                                       UINT8 *p_plen, UINT8 *p_pebit);

/*******************************************************************************
**
** Function         VDP_SetVsInfo
**
** Description      This function is called by a client application to
**                  set codec_info for vendor specific codecs.
**
**                  Input Parameters:
**                      p_codec_info:  Pointer to set codec_info.
**
**                      media_type:  Type of media. Should be one of the AVDT_MEDIA_XXX
**                                   constants defined in avdt_api.h
**
**                      codec_id:    Vendor specific Codec ID. Should be one of the
**                                   VDP_CODEC_ID_XXX constants defined in vdp_api.h
**
**                      p_value:     Vendor specific codec value.
**
**                      value_len:   The length of p_value
**
**                  Output Parameters:
**                      p_codec_info:  the resulting codec_info.
**
** Returns          VDP_SUCCESS if function execution succeeded,
**                  VDP_INVALID_PARAMS if bad parameters are given.
**
*******************************************************************************/
VDP_API extern tVDP_STATUS VDP_SetVsInfo(UINT8 *p_codec_info, UINT8 media_type, UINT16 codec_id,
        UINT8 *p_value, UINT8 value_len);

/*******************************************************************************
**
** Function         VDP_GetVsInfo
**
** Description      This function is called by a client application to
**                  get codec_info for vendor specific codecs.
**
**                  Input Parameters:
**                      p_codec_info:  Pointer of codec_info.
**
**                  Output Parameters:
**                      p_media_type:  Where to place Type of media. Can be NULL if caller
**                                     doesn't want to get value. Will be be one of the
**                                     AVDT_MEDIA_XXX constants defined in avdt_api.h
**
**                      p_co_id:       Where to place the company ID.
**
**                      p_codec_id:    Where to place Vendor specific codec ID.
**
**                      p_value:       Where to place Vendor specific codec value.
**                                     Can be NULL.
**
**                      p_value_len:   Where to place the length of Vendor specific
**                                     codec value. Can be NULL.
**
** Returns          VDP_SUCCESS if function execution succeeded,
**                  VDP_INVALID_PARAMS if bad parameters are given.
**                  VDP_WRONG_CODEC if codec_info of p_cfg contains wrong data
**
*******************************************************************************/
VDP_API extern tVDP_STATUS VDP_GetVsInfo(UINT8 *p_codec_info, UINT8 *p_media_type, UINT32 *p_co_id,
        UINT16 *p_codec_id,  UINT8 *p_value, UINT8 *p_value_len);

#ifdef __cplusplus
}
#endif

#endif /* VDP_API_H */
