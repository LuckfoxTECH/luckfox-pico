/******************************************************************************
**
** File:         trace_api.h
**
** Description:  Contains API for BTE Test Tool trace related functions.
**
**
**  Copyright (c) 2001-2014, Broadom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/


#ifndef TRACE_API_H
#define TRACE_API_H

#include "bt_target.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Trace API Function External Declarations */
BT_API extern void DispRFCOMMFrame(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispL2CCmd(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispSdp(BT_HDR *p_msg, BOOLEAN is_rcv, BOOLEAN is_segment);
BT_API extern void DispSdpFullList(UINT8 *p, UINT16 list_len, BOOLEAN is_rcv);
BT_API extern void DispHciEvt(BT_HDR *p_buf);
BT_API extern void DispHciAclData(BT_HDR *p_buf, BOOLEAN is_rcvd);
BT_API extern void DispHciScoData(BT_HDR *p_buf, BOOLEAN is_rcvd);
BT_API extern void DispHciCmd(BT_HDR *p_buf);
BT_API extern void DispBnep(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispAvdtMsg(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispAvct(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispMca(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispObxMsg(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispLMDiagEvent(BT_HDR *p_buf);
BT_API extern void DispHidFrame(BT_HDR *p_buf, BOOLEAN is_recv, BOOLEAN is_control);
BT_API extern void DispRawFrame(UINT8 *p, UINT16 len, BOOLEAN is_rcv);
BT_API extern void DispSlipPacket(UINT8 *p, UINT16 len, BOOLEAN is_rcv, BOOLEAN oof_flow_ctrl);
BT_API extern void DispNci(UINT8 *p, UINT16 len, BOOLEAN is_recv);
BT_API extern void DispHcp(UINT8 *p, UINT16 len, BOOLEAN is_recv, BOOLEAN is_first_seg);
BT_API extern void DispNDEFRecord(UINT8 *pRec, INT8 *pDescr);
BT_API extern void DispNDEFMsg(UINT8 *pMsg, UINT32 MsgLen, BOOLEAN is_recv);
BT_API extern void DispSmpMsg(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispAttMsg(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void DispLLCP(BT_HDR *p_buf, BOOLEAN is_rx);
BT_API extern void DispSNEP(UINT8 local_sap, UINT8 remote_sap, UINT8 *p_data, UINT16 length, BOOLEAN is_rx);
BT_API extern void DispCHO(UINT8 *pMsg, UINT32 MsgLen, BOOLEAN is_rx);
BT_API extern void DispT3TagMessage(BT_HDR *p_msg, BOOLEAN is_rx);
BT_API extern void DispRWT4Tags(BT_HDR *p_buf, BOOLEAN is_rx);
BT_API extern void DispCET4Tags(BT_HDR *p_buf, BOOLEAN is_rx);
BT_API extern void DispRWI93Tag(BT_HDR *p_buf, BOOLEAN is_rx, UINT8 command_to_respond);

BT_API extern void RPC_DispRFCOMMFrame(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void RPC_DispL2CCmd(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void RPC_DispSdp(BT_HDR *p_msg, BOOLEAN is_rcv, BOOLEAN is_segment);
BT_API extern void RPC_DispSdpFullList(UINT8 *p, UINT16 list_len, BOOLEAN is_rcv);
BT_API extern void RPC_DispTcsMsg(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void RPC_DispHciEvt(BT_HDR *p_buf);
BT_API extern void RPC_DispHciAclData(BT_HDR *p_buf, BOOLEAN is_rcvd);
BT_API extern void RPC_DispHciScoData(BT_HDR *p_buf, BOOLEAN is_rcvd);
BT_API extern void RPC_DispHciCmd(BT_HDR *p_buf);
BT_API extern void RPC_DispLMDiagEvent(BT_HDR *p_buf);
BT_API extern void RPC_DispBnep(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void RPC_DispAvdtMsg(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void RPC_DispAvct(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void RPC_DispMca(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void RPC_DispObxMsg(BT_HDR *p_buf, BOOLEAN is_recv);
BT_API extern void RPC_DispLMDiagEvent(BT_HDR *p_buf);
BT_API extern void RPC_DispHidFrame(BT_HDR *p_buf, BOOLEAN is_recv, BOOLEAN is_control);
BT_API extern void RPC_DispSmpMsg(BT_HDR *p_msg, BOOLEAN is_rcv);
BT_API extern void RPC_DispAttMsg(BT_HDR *p_msg, BOOLEAN is_rcv);
BT_API extern void RPC_DispNci(UINT8 *p, UINT16 len, BOOLEAN is_recv);
BT_API extern void RPC_DispHcp(UINT8 *p, UINT16 len, BOOLEAN is_recv, BOOLEAN is_first_seg);
BT_API extern void RPC_DispNDEFRecord(UINT8 *pRec, INT8 *pDescr);
BT_API extern void RPC_DispNDEFMsg(UINT8 *pMsg, UINT32 MsgLen, BOOLEAN is_recv);
BT_API extern void RPC_DispLLCP(BT_HDR *p_buf, BOOLEAN is_rx);
BT_API extern void RPC_DispSNEP(UINT8 local_sap, UINT8 remote_sap, UINT8 *p_data, UINT16 length, BOOLEAN is_rx);
BT_API extern void RPC_DispCHO(UINT8 *pMsg, UINT32 MsgLen, BOOLEAN is_rx);
BT_API extern void RPC_DispT3TagMessage(BT_HDR *p_msg, BOOLEAN is_rx);
BT_API extern void RPC_DispRWT4Tags(BT_HDR *p_buf, BOOLEAN is_rx);
BT_API extern void RPC_DispCET4Tags(BT_HDR *p_buf, BOOLEAN is_rx);
BT_API extern void RPC_DispRWI93Tag(BT_HDR *p_buf, BOOLEAN is_rx, UINT8 command_to_respond);

//EXPORT_API extern void LogMsg (UINT32 trace_set_mask, const char *fmt_str, ...);
#ifndef     LogMsg
#define  LogMsg(mask, fmt, args...)  do{ rt_kprintf(fmt,##args);} while(0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* TRACE_API_H */
