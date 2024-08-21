/****************************************************************************
**
**  Name        HciExt.h
**
**  Function:   this file contains Host Controller Interface externals
**
**
**  Copyright (c) 1999-2012, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
*****************************************************************************/

#ifndef HCIEXT_H
#define HCIEXT_H


extern void  hcic_process_hci_command(BT_HDR *p_buf);
extern void  hcic_start_hci_command(void);


extern UINT16  hci_max_acl_len;
extern UINT8   hci_max_sco_len;
extern UINT16  hci_max_acl_buffs;
extern UINT16  hci_max_sco_buffs;

/*
** Functions provided by hcicsend.c
*/
extern void     hcic_send_cmd_complete(void *p_buf, UINT16 req_opcode, UINT8 status);
extern void     hcic_send_cmd_complete_param1(void *p_buf, UINT16 req_opcode, UINT8 status, UINT8 param);
extern void     hcic_send_cmd_complete_param2(void *p_buf, UINT16 req_opcode, UINT8 status, UINT16 param);
extern void     hcic_send_cmd_complete_param4(void *p_buf, UINT16 req_opcode, UINT8 status, UINT32 param);
extern void     hcic_send_cmd_complete_bd_addr(void *p_buf, UINT16 req_opcode, UINT8 status, BD_ADDR bd_addr);
extern void     hcic_send_cmd_complete_read_local_supported_codecs(void *p_buf, UINT16 req_opcode, UINT8 status);
extern void     hcic_send_cmd_complete_params(void *p_buf, UINT16 req_opcode, UINT8 status);
extern void     hcic_send_cmd_complete_get_WMS_trans_layer_config(void *p_buf, UINT16 req_opcode, UINT8 status);
extern void     hcic_send_cmd_complete_set_WMS_signaling(void *p_buf, UINT16 req_opcode, UINT8 status);
extern void     hcic_send_flush_occured(UINT16 handle);
extern void     hcic_send_role_change_event(ACLCB *p_acl, UINT8 status);
extern void     hcic_send_cmd_status(void *p_buf, UINT16 req_opcode, UINT8 status);
extern BOOLEAN  hcic_send_hw_err(void *p_buf, UINT8 error);
extern void     hcic_send_connection_complete(void *p_acl, UINT8 status, UINT8 link_type, UINT8 encr_mode);
extern BOOLEAN  hcic_send_connection_request(ACLCB *p_acl, UINT8 link_type);
extern void     hcic_send_disconnection_complete(UINT16 handle, UINT8 status);
extern void     hcic_send_auth_complete_event(ACLCB *p_acl, UINT8 status);
extern void     hcic_send_inquiry_result(BD_ADDR bd_addr, UINT8 page_scan_rep_mode,
        UINT8 page_scan_per_mode, UINT8 page_scan_mode,
        DEV_CLASS dev_class, UINT16 clock_offset);
extern void     hcic_send_inquiry_complete(void);
extern void     hcic_send_num_compl_pkts(UINT8 num, UINT16 *p_handle, UINT16 *p_num_packets);
extern void     hcic_send_mode_change(ACLCB *p_acl, UINT8 status, UINT8 mode, UINT16 interval);
extern void     hcic_send_rmt_name_req_complete(ACLCB *p_acl, UINT8 status);
extern void     hcic_send_encryption_change_event(ACLCB *p_acl, UINT8 status, BOOLEAN enable);
extern void     hcic_send_conn_link_key_change_complete(ACLCB *p_acl, UINT8 status);
extern void     hcic_send_master_link_key_complete(ACLCB *p_acl, UINT8 status);
extern void     hcic_send_rmt_features_complete(ACLCB *p_acl, UINT8 status);
extern void     hcic_send_rmt_ver_info_complete(ACLCB *p_acl, UINT8 status);
extern void     hcic_send_qos_setup_complete(ACLCB *p_acl, UINT8 status);
extern void     hcic_send_max_slots_change(ACLCB *p_acl);
extern void     hcic_send_read_clock_off_comp(ACLCB *p_acl, UINT8 status, UINT16 offset);
extern void     hcic_send_conn_pkt_type_change(void *p_cb, UINT8 conn_type, UINT8 status);
extern BOOLEAN  hcic_send_link_key_req(ACLCB *p_acl);
extern BOOLEAN  hcic_send_pin_code_req(ACLCB *p_acl);
extern void     hcic_send_link_key_notify(BD_ADDR bd_addr, LINK_KEY link_key, UINT8 key_type);
extern void     hcic_send_loopback_ccommand(BT_HDR *p_buf);
extern void     hcic_send_return_link_keys(UINT8 num_keys, BD_ADDR *p_bd_addr, LINK_KEY *p_link_key);
extern void     hcic_send_pagescan_mode_changed(BD_INFO *p_bd_info);
extern void     hcic_send_pagescan_rep_mode_changed(BD_INFO *p_bd_info);


#endif


