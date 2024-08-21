/****************************************************************************/
/*                                                                          */
/*  Name:       testercfg.h                                                 */
/*                                                                          */
/*  Function    this file contains definitions for HCI Tester configuration */
/*                                                                          */
/*                                                                          */
/*  Copyright (c) 1999-2002, Widcomm Inc., All Rights Reserved.             */
/*  Widcomm Bluetooth Core. Proprietary and confidential.                   */
/*                                                                          */
/****************************************************************************/

#ifndef HCI_TESTER_CFG_H
#define HCI_TESTER_CFG_H

#include <stdio.h>
#include <stdlib.h>
#include "bt_target.h"
#ifndef _MAX_PATH
#define _MAX_PATH                   16
#endif
/* Determine the number of UINT32's necessary for trace layers */
#define CFG_TRACE_ARRAY_BITS        32      /* Number of bits in each array element */
#define CFG_TRACE_LAYER_NUM_INDEX   (((UINT32)TRACE_LAYER_MAX_NUM / CFG_TRACE_ARRAY_BITS) + \
                                    (((UINT32)TRACE_LAYER_MAX_NUM % CFG_TRACE_ARRAY_BITS) ? 1 : 0))

typedef struct
{
#define TESTER_CFG_DEV_TYPE_COM 0
#define TESTER_CFG_DEV_TYPE_IP  1
#define TESTER_CFG_DEV_TYPE_USB 2
#define TESTER_CFG_DEV_TYPE_EMBEDDED 3


    UINT32  m_fg_color[TRACE_TYPE_MAX_NUM];
    UINT32  m_bg_color[TRACE_ORG_MAX_NUM];
    UINT32  m_scr_fg_color[MAX_SCRIPT_TYPE];
    UINT32  m_scr_bg_color[MAX_SCRIPT_TYPE];
    UINT32  m_custom_color[16];

    UINT32  m_nFontSize;
    char    m_strFontFace[20];

// Protocol Trace Level
    UINT8   m_lap_level;
    UINT8   m_telp_level;
    UINT8   m_tcs_level;
    UINT8   m_oapp_level;
    UINT8   m_obex_level;
    UINT8   m_l2cap_level;
    UINT8   m_btm_level;
    UINT8   m_hci_level;
    UINT8   m_sdp_level;
    UINT8   m_rfcomm_level;
    UINT8   m_spp_level;
    UINT8   m_gap_level;
    UINT8   m_amp_level;
    UINT8   m_mcap_level;
    UINT8   m_gatt_level;
    UINT8   m_smp_level;
    UINT8   m_nfc_level;
    UINT8   m_nci_level;
    UINT8   m_llcp_level;
    UINT8   m_rw_level;
    UINT8   m_ce_level;
    UINT8   m_snep_level;
    UINT8   m_ndef_level;

    char    curr_open_script_file_1[256];
    char    curr_open_script_file_2[256];

    char    curr_indiv_log_file_1[256];
    char    curr_indiv_log_file_2[256];

    BOOLEAN m_bIsLogging_Each;
    BOOLEAN m_bIsLogging_All;
    BOOLEAN m_bLogEach;
    BOOLEAN m_bLogAll;

#define APPEND_DEFAULT_SETTING      TRUE
    BOOLEAN m_append;

#define PREPEND_DEFAULT_SETTING     FALSE
    BOOLEAN m_prepend;

#define FILTER_FILE_LOGGING_DEFAULT_SETTING FALSE
    BOOLEAN m_filterfilelogging;

    char    m_filename[_MAX_PATH];
    char    m_sLogDir[_MAX_PATH];

//#if ((defined(BLUESTACK_TESTER)) || (defined(STACK_DLL)))
    /* Options for embedded target */
#define TEST_STATE_SIMULATION   0
#define TEST_STATE_DONGLE       1
#define TEST_STATE_EMBEDTARGET  2

    UINT8   m_test_sel1;            /* Selection of test mode for port 1*/
    UINT8   m_test_sel2;            /* Selection of test mode for port 2*/

//#endif


    UINT32  m_traceOrgMask;

    UINT32  m_traceStack_layer[CFG_TRACE_LAYER_NUM_INDEX];

    UINT32  m_traceHciTrans_layerTrans[CFG_TRACE_LAYER_NUM_INDEX];
    UINT32  m_traceHciTrans_type;

    UINT32  m_traceScrWrap_layer[CFG_TRACE_LAYER_NUM_INDEX];
    UINT32  m_traceScrWrap_type;

    UINT32  m_traceProtoDisp_layer[CFG_TRACE_LAYER_NUM_INDEX];
    UINT32  m_traceProtoDisp_type;

    UINT32  m_traceRPC_layer[CFG_TRACE_LAYER_NUM_INDEX];
    UINT32  m_traceRPC_type;

    UINT32  m_traceScrEngine_type;

    UINT32  m_traceUsrScr_type;

    UINT32  m_traceTester_type;

    UINT32  m_traceGKI_type;
    UINT32  m_traceAppl_type;

    UINT32  m_traceTestapp_layer[CFG_TRACE_LAYER_NUM_INDEX];
    UINT32  m_traceTestapp_type;

    BOOLEAN bLiteOrigProto;
    BOOLEAN bLiteOrigTrans;
    BOOLEAN bLiteOrigDisp;
    BOOLEAN bLiteOrigRPC;
    BOOLEAN bLiteOrigGKI;
    BOOLEAN bLiteOrigAppl;
    BOOLEAN bLiteOrigScrEng;
    BOOLEAN bLiteOrigScrWrpr;
    BOOLEAN bLiteOrigUserScr;


    BOOLEAN bLiteTransTX;
    BOOLEAN bLiteTransRX;

    BOOLEAN bLiteDispHCI;
    BOOLEAN bLiteDispL2C;
    BOOLEAN bLiteDispRFC;
    BOOLEAN bLiteDispSDP;
    BOOLEAN bLiteDispTCS;
    BOOLEAN bLiteDispOBEX;
    BOOLEAN bLiteDispBTM;
    BOOLEAN bLiteDispProfile;
    BOOLEAN bLiteDispErr;
    BOOLEAN bLiteDispTx;
    BOOLEAN bLiteDispRx;
    BOOLEAN bLiteDispGeneric;
    BOOLEAN bLiteDispAssert;
    BOOLEAN bLiteDispLM_LC;
    BOOLEAN bLiteDispAMP;
    BOOLEAN bLiteDispMCA;
    BOOLEAN bLiteDispGATT;
    BOOLEAN bLiteDispSMP;

    BOOLEAN bLiteDispNCI;
    BOOLEAN bLiteDispLLCP;
    BOOLEAN bLiteDispNDEF;
    BOOLEAN bLiteDispRW;
    BOOLEAN bLiteDispCE;
    BOOLEAN bLiteDispSNEP;
    BOOLEAN bLiteDispCHO;

    BOOLEAN bLiteProtoProfile;
    BOOLEAN bLiteProtoHCI;
    BOOLEAN bLiteProtoL2C;
    BOOLEAN bLiteProtoRFC;
    BOOLEAN bLiteProtoSDP;
    BOOLEAN bLiteProtoTCS;
    BOOLEAN bLiteProtoOBEX;
    BOOLEAN bLiteProtoBTM;
    BOOLEAN bLiteProtoLM_LC;
    BOOLEAN bLiteProtoAMP;
    BOOLEAN bLiteProtoMCA;
    BOOLEAN bLiteProtoGATT;
    BOOLEAN bLiteProtoSMP;
    BOOLEAN bLiteProtoNFC;
    BOOLEAN bLiteProtoLLCP;
    BOOLEAN bLiteProtoRW;
    BOOLEAN bLiteProtoCE;
    BOOLEAN bLiteProtoP2P;
    BOOLEAN bLiteProtoSNEP;
    BOOLEAN bLiteProtoCHO;
    BOOLEAN bLiteProtoNFA;
    BOOLEAN bLiteProtoHAL;

    UINT32  nLiteProtoProfileLevel;

    UINT32  nLiteProtoFilter;

} CTesterCfg;

#define TRACE_FILTER_HCI_DATA       0x00000008
#define TRACE_FILTER_HCI_DATA_HDR   0x00000004
#define TRACE_FILTER_HCI_PARAM      0x00000002
#define TRACE_FILTER_HCI_NA         0x00000001
#define TRACE_FILTER_AVD_DATA       0x00000080
#define TRACE_FILTER_AVD_DATA_HDR   0x00000040
#define TRACE_FILTER_AVD_PARAM      0x00000020
#define TRACE_FILTER_AVD_NA         0x00000010
#define TRACE_FILTER_RFC_DATA       0x00000100
#define TRACE_FILTER_OBX_DATA       0x00000200

/************************************************************************************************
** Layer MACROS handle array of UINT32 bits for more than 32 layers
*************************************************************************************************/
/* MACRO to set the trace LAYER bit mask in a bit stream */
#define TRACE_SET_LAYER(p, layer)  (((UINT32 *)(p))[(((UINT32)(layer)) / CFG_TRACE_ARRAY_BITS)] |=  \
                                    (1 << (((UINT32)(layer)) % CFG_TRACE_ARRAY_BITS)))


/* MACRO to set the trace LAYER bit mask in a bit stream */
#define TRACE_CLR_LAYER(p, layer)  (((UINT32 *)(p))[(((UINT32)(layer)) / CFG_TRACE_ARRAY_BITS)] &=  \
                                    ~(1 << (((UINT32)(layer)) % CFG_TRACE_ARRAY_BITS)))

/* MACRO to check the trace LAYER bit mask in a bit stream (Returns TRUE or FALSE) */
#define TRACE_IS_LAYER_SET(p, layer)    (( ((UINT32 *)(p))[(((UINT32)(layer)) / CFG_TRACE_ARRAY_BITS)] &   \
                                        (1 << (((UINT32)(layer)) % CFG_TRACE_ARRAY_BITS)) ) ? TRUE : FALSE)

/************************************************************************************************
** MACROS handle single UINT32 bit mask (orig, type, etc.)
*************************************************************************************************/
/* MACROs to set (clear) a bit in a 32-bit stream */
#define TRACE_SET_UINT32(p, value)    (*((UINT32 *)(p)) |=  ((UINT32)1 << ((UINT32)(value))))
#define TRACE_CLR_UINT32(p, value)    (*((UINT32 *)(p)) &= ~((UINT32)1 << ((UINT32)(value))))

/* MACRO to check the UINT32 bit mask (Returns TRUE if set, otherwise FALSE) */
#define TRACE_IS_UINT32_SET(mask, bit)    ((((UINT32)(mask)) & ((UINT32)1 << ((UINT32)(bit)))) ? TRUE : FALSE)                 \

#define TRACE_UINT32_SET_ALL    ((UINT32)0xffffffff)   /* set variable all */
#define TRACE_LITE_ORIG_ALL     ((UINT32)0x0000003f)   /* trace lite: orig variable all */

#define TRACE_FIRST_LAYER_INDEX     TRACE_LAYER_TRANS_MAX_NUM   /* Beginning of layer values */
#ifdef __cplusplus
extern "C"
{
#endif

extern CTesterCfg tester_cfg;

BOOLEAN PassedThroughFilter(UINT32 trace_set_mask);

#ifdef DYN_CONTROL

BOOL IsToBeDeliveredToClient(UINT32 trace_set_mask);

#endif


#ifdef __cplusplus
}
#endif


#endif
