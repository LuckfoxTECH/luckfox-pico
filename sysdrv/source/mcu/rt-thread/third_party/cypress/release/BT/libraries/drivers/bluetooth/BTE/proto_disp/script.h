/****************************************************************************/
/*                                                                          */
/*  Name:       script.h                                                    */
/*                                                                          */
/*  Function:   this file contains script related definitions               */
/*                                                                          */
/*                                                                          */
/*  Copyright (c) 1999-2002, Widcomm Inc., All Rights Reserved.             */
/*  Widcomm Bluetooth Core. Proprietary and confidential.                   */
/*                                                                          */
/****************************************************************************/

#ifndef SCRIPT_H
#define SCRIPT_H

#include "bt_types.h"
#include "trace_api.h"

#ifdef __cplusplus
extern "C" {
#endif

extern UINT32   ScrProtocolTraceFlag;

/* Utility functions */
extern void scru_ascii_2_bdaddr(char *p_ascii, UINT8 *p_bd);
extern void scru_ascii_2_link_key(char *p_ascii, UINT8 *p_key);
extern void scru_ascii_2_dev_class(char *p_ascii, UINT8 *p_dev);
extern void scru_bdaddr_2_ascii(UINT8 *p_bd, char *p_ascii);
extern void scru_link_key_2_ascii(UINT8 *p_key, char *p_ascii);
extern void scru_dev_class_2_ascii(UINT8 *p_dev_class, char *p_ascii);
extern int scru_ascii_2_feature_mask(char *p_ascii, UINT8 *p_hex);
extern int scru_asciiw_2_hex(char *p_ascii, int len, UINT8 *p_hex);
extern int scru_read_tag_content(char *p_file_name, int len_hex, UINT8 *p_hex, UINT32 trace_layer);
extern void scru_read_hex_content(char *p_file_name, void *p, UINT16 buf_size, UINT8 pool_id, UINT32 trace_layer);
extern void scru_write_hex_content(char *p_file_name, void *p, UINT32 trace_layer);
extern int  scru_ascii_2_hex(char *p_ascii, int len, UINT8 *p_hex);
extern void scru_ascii_2_uint(char *p_ascii, int n_bytes, void *p);
extern void scru_hex_2_ascii(UINT8 *p_hex, int len, char *p_ascii);
extern char *scru_find_evt_descr(UINT32 event);
extern void scru_map_event(BT_HDR *p_msg);
extern BOOLEAN scru_hci_flow_control_event(BT_HDR *p_msg);
extern void scru_send_to_tester(BT_HDR *p_msg, UINT16 event);
extern UINT16 scru_translate_and_copy_string(char *p_src, char *p_dst);
extern UINT8 *scru_dump_hex(UINT8 *p, char *p_title, UINT32 len, UINT32 trace_layer, UINT32 trace_type);
extern void scru_hex_2_string(UINT8 *p_hex, int len, char *p_ascii);
extern void scru_send_to_target(BT_HDR *, UINT16);
extern void scr_process_peer_msg(BT_HDR *p_msg);
extern void scru_send_to_peer(BT_HDR *p_msg, UINT16 event);
extern void scru_send_to_nfc_peer(BT_HDR *p_msg, UINT16 event);

#ifdef __cplusplus
}
#endif
#endif

