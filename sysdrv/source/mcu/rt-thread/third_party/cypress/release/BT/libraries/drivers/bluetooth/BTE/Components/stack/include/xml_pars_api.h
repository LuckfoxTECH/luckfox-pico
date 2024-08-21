/*****************************************************************************
**
**  Name:       xml_pars_api.h
**
**  File:       XML Parser
**
**  Copyright (c) 2000-2004, WIDCOMM Inc., All Rights Reserved.
**  WIDCOMM Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/
#ifndef XML_PARS_API_H
#define XML_PARS_API_H

#include "data_types.h"

#define XML_STACK_SIZE      7

#ifndef XML_UI_ENTRY_MAX_NAME_LEN
#define XML_UI_ENTRY_MAX_NAME_LEN 256
#endif

/*****************************************************************************
**   Type Definitions
*****************************************************************************/

/* Definition of XML events */
enum
{
    XML_XMLDECL,
    XML_TAG,         /* 1 For STag (Start Tag) and EmptyElemTag */
    XML_TAG_END,     /* 2 When STag or EmptyElemTag ends */
    XML_ATTRIBUTE,   /* 3 */
    XML_CHARDATA,    /* 4 */
    XML_ETAG,        /* 5 For End Tag */
    XML_PARTIAL,     /* 6 notify XML user that the parse does not end properly */
    XML_COPY,        /* 7 notify XML user to copy till end of a token */
    XML_QUERY,       /* 8 query if the XML object has ended */
    XML_TOP          /* 9 */
};
typedef UINT16 tXML_EVENT;
#define XML_LAST_XE     XML_ETAG

enum
{
    XML_SUCCESS,
    XML_WARNING,
    XML_OBJ_ST_EMPTY,
    XML_NO_MEM,         /* no p_last_bfr, and the tXML_MUL_STATE is not in init */
    XML_NO_PROP,        /* run out of tXML_PROP */
    XML_ERR,            /* generic error */
    XML_NO_END          /* to be continued */
};
typedef UINT16 tXML_RESULT;

/* Definitions of different event data */
typedef struct
{
    UINT8   *p_version;
    UINT8   *p_enc_name;
    BOOLEAN *p_standalone;    /* NULL if no standalone decl was found */
} tXML_DECL;



/* for parser that expand multi-sessions */
/* tag stack */
typedef struct
{
    UINT8         stack[XML_STACK_SIZE];
    UINT8         top;
    tXML_EVENT    event; /* last XML event */
} tXML_STACK;

typedef struct
{
    UINT8   *p;
    UINT16  len;
} tXML_STR;

typedef struct
{
    tXML_STACK  stack;
    tXML_STR    prefix;
    tXML_STR    name;
    UINT8       *p_last_stm;
} tXML_MTAG;

typedef struct
{
    tXML_STACK  stack;
    BOOLEAN     end;        /* true if end of EmptyElemTag */
    /* false if end of STag        */
} tXML_MTAG_END;



typedef struct
{
    tXML_STACK  stack;
    tXML_STR    prefix;
    tXML_STR    name;
    tXML_STR    value;
} tXML_MATTR;


/* There may be several XML_CHARDATA events dispatched for one CHARDATA
** string in the XML document. In each XML_CHARDATA only a segment of the
** total string is provided. For the last segement the field last is true.
*/
typedef struct
{
    tXML_STACK  stack;
    tXML_STR    value;
    BOOLEAN     last;
} tXML_MCH_DATA;


typedef struct
{
    tXML_STACK  stack;
    tXML_STR    prefix;
    tXML_STR    name;
} tXML_METAG;

typedef struct
{
    tXML_STACK  stack;
    UINT8       *p_begin;
    tXML_STR    last;
} tXML_MCOPY;

typedef struct
{
    tXML_STACK  stack;
    tXML_RESULT parse;
    UINT8       *p_keep;
} tXML_MPART;

typedef union
{
    tXML_STACK      stack;
    tXML_DECL       decl;
    tXML_MTAG       tag;
    tXML_MTAG_END   empty_elem;
    tXML_MATTR      attr;
    tXML_MCH_DATA   ch_data;
    tXML_METAG      etag;
    tXML_MCOPY      copy;
    tXML_MPART      part;
} tXML_MEVT_DATA;

typedef struct
{
    UINT8       *p_data;            /* Pointer to property data. */
    UINT16      len;                /* Length of data. */
    UINT8       prefix;             /* Property prefix. */
    UINT8       name;               /* Property name. */
    UINT8       level;              /* 0 as top level */
} tXML_PROP;

/* Definitions of different error actions.
** At present there is only one.
*/
enum
{
    XML_NEXT_TAG /* tries to find beginning of next tag */
};
typedef UINT16 tXML_ERR_ACTION;



enum
{
    XML_STS_INIT,
    XML_STS_1STM,   /* got the first < */
    XML_STS_1TAG,   /* got the first tag element */
    XML_STS_DONE
};
typedef UINT8   tXML_STATUS;

typedef struct
{
    UINT8       *p_hdr;  /* the beginning of GKI buffer */
    UINT8       *p_begin;/* the beginning of XML data */
    UINT8       *p_end;  /* the end of XML data */
} tXML_OS;


/*****************************************************************************
**
** Callback         tXML_CBACK
**
** Description
**     Called repeatedly from the XML parser for each XML construct parsed.
**     The parsed XML construct is reported as an XML event in the event
**     in-parameter. The parser provides event data specific for each event
**     in the callback through the p_event_data param which should be casted
**     to the proper type by the user implemented tXML_CBACK function.
**     The p_event_data should be casted to a pointer of a type with the same
**     name as the  event but prefixed with t. For example when the event is
**     XML_TAG, p_event_data should be casted to tXML_TAG*. Alternatively,
**     p_event_data can be casted to a tEVENT_DATA* (pointer to union) type
**     for all events. The memory behind the p_event_data is provided by the
**     parser and is only valid during the callback, i.e. the memory is not
**     valid after the callback. No pointer inside the event structures may
**     be NULL unless explicitly stated so above for some specific event.
**     The p_usr_data parameter is the same p_usr_data pointer the
**     API user passed in the XML_Parse. It is only declared as a mean for
**     the API user to pass any user data to its callback function.
**
**
** Parameters
**     event (in) :        the XML event or XML contruct just parsed,
**     p_event_data (in) : data, i.e. names values etc, found in the XML
**                         construct. The data is specific for the XML
**                         construct and should be casted to the
**                         proper type as descibed above. May not be NULL.
**     p_usr_data (in) :   Same p_usr_data as passed in XML_Parse.
**
** Returns
**     TRUE :  if parsing should continue
**     FALSE : if parsing should stop
**
*****************************************************************************/

typedef BOOLEAN(*tXML_CBACK)(tXML_EVENT event, void *p_event_data,
                             void *p_usr_data);

typedef struct
{

    UINT8       p[512];
    UINT16     len;
} tXML_STR_PARTIAL;

typedef struct
{
    tXML_STR_PARTIAL       last_bfr;
    UINT16          used_last_bfr;
    tXML_MEVT_DATA event_data;
}
tXML_PARTIAL;

/* tXML_STATE stores state during the XML_Parse call not between
** XML_Parse calls. It is used in most static functions in this file
** and passed in as first parameter to these.
*/
typedef struct
{
    /* input from XML_Parse caller */
    tXML_OS        xml_os;
    UINT8          *p_cur;
    tXML_CBACK     cback;
    void           *p_usr_data;
    tXML_MEVT_DATA event_data;

    /* handling of data buffers for prefix, name and value */
    UINT8          *p_data_bfr;
    UINT8          *p_last_stm; /* last '<' */
    UINT8          *p_copy;
    tXML_STR       last_bfr;
    tXML_STR       value;
    tXML_STR       prefix;
    tXML_STR       name;

    /* token and result handling */
    INT16          curr_res;
    tXML_RESULT    pars_res;
    UINT8          next_token;
    BOOLEAN        skip_next_nl;

    UINT16          used_last_bfr;   /* the actual data size in p_last_bfr */
    tXML_STATUS    status;

    /* structure used to store partial data */
    tXML_PARTIAL  partial_st;
} tXML_MUL_STATE;

/*****************************************************************************
**  External Function Declarations
*****************************************************************************/
#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
**
** Function         XML_Parse
**
** Description
**     Parses an XML message/document in utf8. The message is read from
**     an object store which must have been initialised and an object
**     (file) must have been opened so the parser can start directly with
**     reading the first byte using the GOEP_Read function and passing
**     the h_obj_store in-param.
**     The parser will call the xml_cback callback for each basic
**     XML-construct parsed. If the parser encounters an error it will
**     try to act according to the err_action in-param and report any
**     errors through the err_handle. The parser is non-validating.
**
** Parameters
**     h_obj_store (in) : a handle to an opened object (file) in
**                        the Object Store. The object must be
**                        ready for reading.
**     xml_cback (in) :   the user provided function to call back
**                        for each XML-construct (XML event) parsed.
**     err_action (in) :  the action the parser will try if it finds
**                        an error.
**     err_handle (in) :  a handle to some error recording object to
**                        which the parser may report errors. This is
**                        a place holder to connect any type of error
**                        recording object.
**     p_usr_data (in) :  any user data that will be passed on
**                        unchanged to the callback function.
**
** Returns
**     0 if the parsing was successfull without errors
**     non-zero error or warning code otherwise
**
*****************************************************************************/

tXML_RESULT XML_Parse(tXML_OS *p_os, tXML_CBACK xml_cback,
                      tXML_ERR_ACTION err_action, UINT32 err_handle,
                      void *p_usr_data);

/*****************************************************************************
**
** Function         XML_MulParse
**
** Description      Initialize XML parser state machine.
**
** Parameters
**      p_st (in)        :   xml state machine.
**      p_os (in)        :   xml data storage
**
*****************************************************************************/
tXML_RESULT XML_MulParse(tXML_MUL_STATE *p_st, tXML_OS *p_os);

/*****************************************************************************
**
** Function         XML_InitPars
**
** Description      Initialize XML parser state machine.
**
** Parameters
**      p_st (in)        :   xml state machine.
**      xml_cback (in)   :   the user provided function to call back
**      p_usr_data       :   user defined data.
**
*****************************************************************************/
void XML_InitPars(tXML_MUL_STATE *p_st, tXML_CBACK xml_cback, void *p_usr_data);

#ifdef __cplusplus
}
#endif

#endif /* ifndef XML_PARS_API_H */
