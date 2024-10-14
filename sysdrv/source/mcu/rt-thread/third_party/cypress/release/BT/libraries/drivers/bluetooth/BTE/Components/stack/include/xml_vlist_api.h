/******************************************************************************
 **
 **  Name:          xml_vlist_api.h
 **
 **  Description:   This module contains xml parser of obex vcard listing
 **
 **  Copyright (c) 2004-2005, Broadcom Corporation, All Rights Reserved.
 **  WIDCOMM Bluetooth Core. Proprietary and confidential.
 ******************************************************************************/

#ifndef XML_VLIST_API_H
#define XML_VLIST_API_H

#include "bt_target.h"
#include "gki.h"
#include "xml_pars_api.h"

/**********************************************************************************/
#ifndef XML_VLIST_CARRY_OVER_LEN
#define XML_VLIST_CARRY_OVER_LEN 512 /* number of bytes we can store in case we did not yet find the > */
#endif

typedef struct
{
    UINT16          handle_len;
    UINT8           *handle;
    UINT16          name_len;
    UINT8           *name;
} tXML_VLIST_ENTRY;

/**********************************************************************************/

typedef enum
{
    XML_VLIST_OK, /* parsing is ok, operation is ok */
    XML_VLIST_PENDING, /* parsing is ok but not enough data */
    XML_VLIST_END_LIST, /* found </vCard-listing> */
    XML_VLIST_OUT_FULL, /* output buffer full  /vCard-listing not reached! data is dumped */
    XML_VLIST_ERROR,     /* some parsing error occured */
    XML_VLIST_NO_RES,    /* ran out of resources (memory) */
    XML_VLIST_DST_NO_RES /* ran out of destination data buffer */
} txml_vlist_res;
typedef UINT8    tXML_VLIST_RES;


typedef struct
{
    tXML_VLIST_ENTRY      *p_entry;
    tXML_PROP              *p_prop;

    tXML_PROP              *offset_prop;    /* current filling property */
    UINT16                  prop_num;       /* number of properties left to be filled in */

    INT16                   current_entry;
    INT16                   max_name_len;    /* maximum length of name length of entry
                                                XML parser limits to 64 bytes i think. */
    UINT16                  max_entry;
    BOOLEAN                 ended;
    UINT16                  prop_index;
    UINT16                  max_num_prop;
    UINT8                   obj;           /* the XML object */
} tXML_VLIST_STATE;

typedef struct
{
    tXML_MUL_STATE          xml;
    tXML_VLIST_STATE       xml_user_data;
} tXML_VLIST_PARSER;

/* only this value is of significance, if not ok frame is dropped by parser */
#define XML_VLIST_ENTRY_OK    0

typedef UINT8 tXML_VLIST_STATUS;


typedef struct
{
    UINT8           *data;
    UINT16           len;
    BOOLEAN          final;     /* If TRUE, entry is last of the series */
    tXML_VLIST_STATUS  status;    /* Fields are valid when status is BTA_FTC_OK */
} tXML_VLIST_LIST; /* clone of tBTA_FTC_LIST */


/**************************************************************************************
** Function         XML_VlistInit
**
** Description      Initialize xml parser state machine.
**
** Parameters       p_xml_state: address of parser structure, allocate an additional space
**                                of size XML_VLIST_CARRY_OVER_LEN right after p_xml_state
**                                to hold carry over data.
**                  p_entry    : points start of output vlist entry. caller needs do free this memory
**                  max_entry  : max is 16 bit integer value which is the maximum number of vlist entries.

**
** Returns          void
**************************************************************************************/
BT_API void XML_VlistInit(tXML_VLIST_PARSER  *p_xml_state,
                          tXML_VLIST_ENTRY    *p_entry,
                          const UINT16             max_entry);


/**************************************************************************************
** Function         XML_VlistParse
**
** Description      This function is called to parse the xml data received from OBEX
**                  into vlist entries associated with properties value. It can also be
**                  used as clean up function to delete left over data from previous parse.
**                  This clean up function is typically used when application runs out of
**                  resource and decides to discard extra xml data received.
**
** Parameters       p_xml_state: pointer to a xml parser initialized by XML_VlistInit().
**                  xml_data: valid pointer to OBEX list data in xml format.
**                  xml_len: length of the  package, must be non-zero value.
**                  dst_data: valid pointer to the buffer for holding converted vlist entry name and handle.
**                            When dst_data is NULL, clean up all remaining data in the parser.
**                  dst_len: length of the dst_data buffer, its carry out value is the number
**                            of bytes of available buffer remains.
**                  num_entries: current number of entries, in the end it is the total number of entries
**
** Returns          tXML_VLIST_RES (see xml_vlist_api.h)
**                  XML_PENDING: parsing not completed
**                  XML_END_LIST: found /vCard-listing but no final flag detected
**                  XML_VLIST_OUT_FULL: reached max_entry -> do not call parser anymore!!! dump data
**                  XML_VLIST_DST_NO_RES : run out of dst buffer resource while
**                                          some xml data still remains.

**************************************************************************************/
BT_API extern tXML_VLIST_RES XML_VlistParse(tXML_VLIST_PARSER   *p_xml_state,
        UINT8 *xml_data, UINT16 xml_len,
        UINT8 *dst_data, UINT16 *dst_len,
        UINT16               *num_entries);


#endif
