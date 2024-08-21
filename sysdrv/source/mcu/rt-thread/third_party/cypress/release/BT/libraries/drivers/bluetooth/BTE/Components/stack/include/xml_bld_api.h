/*****************************************************************************
**
**  Name:       xml_bld_api.h
**
**  File:       XML Builder API
**
**
**  Copyright (c) 2000-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef XML_BLD_API_H
#define XML_BLD_API_H

#include "data_types.h"


/*****************************************************************************
**   Type Definitions
*****************************************************************************/

enum
{
    XML_BLD_SUCCESS,
    XML_BLD_ERROR
};
typedef UINT16 tXML_BLD_RESULT;


enum
{
    XML_ATTR_CONT = FALSE, /* not the end of attribute list. add quote(') */
    XML_ATTR_LAST = TRUE,  /* the last one of the attribute list. add quote('), greater_than(>), and line_feed(\n) */
    XML_ATTR_ETAG  = 2     /* the end of the element. add quote('), end_mark(/), greater_than(>), and line_feed(\n) */
};
typedef UINT8 tXML_ATTR_END;


/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif



/*****************************************************************************
**
**  Function:    XML_BufAddTag
**
**  Purpose:     Write a start or end tag and optional prefix.
**
**  Parameters:
**       UINT8       **pp_buf     reference to the storage to hold the XML object
**                                GOEP_WriteStore.
**       const UINT8*  prefix     tag prefix (namespace)
**       const UINT8*  tag        tag name
**       BOOLEAN       start_tag  TRUE = start tag, FALSE = end tag
**       BOOLEAN       has_attr   TRUE if the tag contains attributes
**
**  Returns:     XML_BLD_SUCCESS if success
**
*****************************************************************************/
tXML_BLD_RESULT XML_BufAddTag(UINT8    **pp_buf,
                              const UINT8 *prefix,
                              const UINT8 *tag,
                              BOOLEAN     start_tag,
                              BOOLEAN     has_attr);


/*****************************************************************************
**
**  Function:    XML_BufAddAttribute
**
**  Purpose:     Write an attribute and optional prefix.
**
**  Parameters:
**       UINT8       **pp_buf     reference to the storage to hold the XML object
**       const UINT8*  prefix     attribute prefix (namespace)
**       const UINT8*  attr_name  attribute name
**       const UINT8*  attr_value attribute value
**
**  Returns:     XML_BLD_SUCCESS if success
**
*****************************************************************************/
tXML_BLD_RESULT XML_BufAddAttribute(UINT8    **pp_buf,
                                    const UINT8 *prefix,
                                    const UINT8 *attr_name,
                                    const UINT8 *attr_value,
                                    tXML_ATTR_END     last_attr);

/*****************************************************************************
**
**  Function:    XML_BufAddCharData
**
**  Purpose:     Write the element content.
**
**  Parameters:
**       UINT8       **pp_buf     reference to the storage to hold the XML object
**       const UINT8*  content    element content
**
**  Returns:     XML_BLD_SUCCESS if success
**
*****************************************************************************/
tXML_BLD_RESULT XML_BufAddCharData(UINT8 **pp_buf, const UINT8 *charData);

#ifdef __cplusplus
}
#endif

#endif /* ifndef XML_BLD_API_H */
