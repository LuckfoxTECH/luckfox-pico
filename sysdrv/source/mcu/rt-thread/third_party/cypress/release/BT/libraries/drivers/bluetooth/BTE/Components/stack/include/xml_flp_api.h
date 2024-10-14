/******************************************************************************
 **
 **  Name:          xml_flp.h
 **           $Id: xml_flp.h,v 1.2 2004/11/15 14:07:47 rlenden Exp
 **
 **  Description:   This module contains xml parser of obex folder listing
 **
 **  Copyright (c) 2004-2005, Broadcom Corporation, All Rights Reserved.
 **  WIDCOMM Bluetooth Core. Proprietary and confidential.
 ******************************************************************************/

#ifndef XML_FLP_H
#define XML_FLP_H

#include "bt_target.h"
#include "gki.h"
#include "xml_pars_api.h"

/**********************************************************************************/
#ifndef XML_OBX_FOLDER
#define XML_OBX_FOLDER             0
#endif
#ifndef XML_OBX_FILE
#define XML_OBX_FILE               1
#endif

#define XML_PERM_READ_B               0x01
#define XML_PERM_WRITE_B              0x02
#define XML_PERM_DELETE_B             0x04

typedef UINT8    XML_PERM_MASK ;

#ifndef XML_OBX_FILE_MAX
#define XML_OBX_FILE_MAX           100
#endif

#ifndef XML_FOLDER_CARRY_OVER_LEN
#define XML_FOLDER_CARRY_OVER_LEN 512 /* number of bytes we can store in case we did not yet find the > */
#endif


#ifndef XML_PARENT_FOLDER
#define XML_PARENT_FOLDER     ".."  /* <parent-folder/> is represented as .. */
#endif


typedef struct
{

    UINT8           *data;
    UINT32          size;           /* size of file or folder in bytes */
    UINT16          len;
    UINT8           type;
    XML_PERM_MASK   user_perm;
} tXML_FOLDER_ENTRY;

/**********************************************************************************/

typedef enum
{
    XML_FOLDER_OK, /* parsing is ok, operation is ok */
    XML_FOLDER_PENDING, /* parsing is ok but not enough data */
    XML_FOLDER_END_LIST, /* found </folder-listing> */
    XML_FOLDER_OUT_FULL, /* output buffer full  /folder-listing not reached! data is dumped */
    XML_FOLDER_ERROR,     /* some parsing error occured */
    XML_FOLDER_NO_RES,    /* ran out of resources (memory) */
    XML_FOLDER_DST_NO_RES /* ran out of destination data buffer */
} txml_folder_res;
typedef UINT8    tXML_FOLDER_RES;


typedef struct
{
    tXML_FOLDER_ENTRY      *p_entry;
    tXML_PROP              *p_prop;

    tXML_PROP              *offset_prop;    /* current filling property */
    UINT16                  prop_num;       /* number of properties left to be filled in */

    INT16                   current_entry;
    INT16                   max_name_len;    /* maximum length of name length of entry
                                                XML parser limits to 196 bytes i think. */
    UINT16                  max_entry;
    BOOLEAN                 ended;
    UINT16                  prop_index;
    UINT16                  max_num_prop;
    UINT8                   obj;           /* the XML object - (tFTC_XML_OBJ + 1)*/
} tXML_FOLDER_STATE;

typedef struct
{
    tXML_MUL_STATE          xml;
    tXML_FOLDER_STATE       xml_user_data;
} tXML_FOLDER_PARSER;

/* only this value is of significance, if not ok frame is dropped by parser */
#define XML_FOLDER_ENTRY_OK    0

typedef UINT8 tXML_FOLDER_STATUS;


typedef struct
{
    UINT8           *data;
    UINT16           len;
    BOOLEAN          final;     /* If TRUE, entry is last of the series */
    tXML_FOLDER_STATUS  status;    /* Fields are valid when status is BTA_FTC_OK */
} tXML_FOLDER_LIST; /* clone of tBTA_FTC_LIST */


/**************************************************************************************
** Function         XML_FolderInit
**
** Description      Initialize xml parser state machine.
**
** Parameters       p_xml_state: address of parser structure, allocate an additional space
**                                of size XML_FOLDER_CARRY_OVER_LEN right after p_xml_state
**                                to hold carry over data.
**                  p_entry    : points start of output directory entry. caller needs do free this memory
**                  max_entry  : max is 16 bit integer value which is the maximum number of folder entries.

**
** Returns          void
**************************************************************************************/
FTP_API void XML_FolderInit(tXML_FOLDER_PARSER  *p_xml_state,
                            tXML_FOLDER_ENTRY    *p_entry,
                            const UINT16             max_entry);


/**************************************************************************************
** Function         XML_FolderParse
**
** Description      This function is called to parse the xml data received from OBEX
**                  into folder entries associated with properties value. It can also be
**                  used as clean up function to delete left over data from previous parse.
**                  This clean up function is typically used when application runs out of
**                  resource and decides to discard extra xml data received.
**
** Parameters       p_xml_state: pointer to a xml parser initialized by XML_FolderInit().
**                  xml_data: valid pointer to OBEX list data in xml format.
**                  xml_len: length of the  package, must be non-zero value.
**                  dst_data: valid pointer to the buffer for holding converted folder entry name.
**                            When dst_data is NULL, clean up all remaining data in the parser.
**                  dst_len: length of the dst_data buffer, its carry out value is the number
**                            of bytes of available buffer remains.
**                  num_entries: current number of entries, in the end it is the total number of entries
**
** Returns          tXML_FOLDER_RES (see xml_flp.h)
**                  XML_PENDING: parsing not completed
**                  XML_END_LIST: found /folder-listing but no final flag detected
**                  XML_FOLDER_OUT_FULL: reached max_entry -> do not call parser anymore!!! dump data
**                  XML_FOLDER_DST_NO_RES : run out of dst buffer resource while
**                                          some xml data still remains.

**************************************************************************************/
FTP_API extern tXML_FOLDER_RES XML_FolderParse(tXML_FOLDER_PARSER   *p_xml_state,
        UINT8 *xml_data, UINT16 xml_len,
        UINT8 *dst_data, UINT16 *dst_len,
        UINT16               *num_entries);


#endif
