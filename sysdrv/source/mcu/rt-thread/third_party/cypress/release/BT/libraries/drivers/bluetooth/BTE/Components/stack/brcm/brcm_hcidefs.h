/*****************************************************************************
**
**  Name        brcm_hcidefs.h
**
**  Function    This file contains Broadcom Specific Host Controller Interface
**              definitions.
**
**  Copyright (c) 1999-2014, Broadcom Corp., All Rights Reserved.
**  Broadcom Bluetooth Core. Proprietary and confidential.
**
******************************************************************************/

#ifndef BRCM_INCLUDE_H
#define BRCM_INCLUDE_H

/* Vendor specific commands for BRCM chipset */
#define HCI_BRCM_UPDATE_BAUD_RATE_ENCODED_LENGTH        0x02
#define HCI_BRCM_UPDATE_BAUD_RATE_UNENCODED_LENGTH      0x06
#define HCI_BRCM_WRITE_SLEEP_MODE_LENGTH                12
#define HCI_BRCM_ENABLE_H4IBSS_LENGTH                   7
#define HCI_BRCM_CUSTOMER_EXT               (0x0000 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_SET_LOCAL_DEV_ADDR         (0x0001 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_FM_OPCODE                  (0x0015 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_FMTX_OPCODE                (0x0082 | HCI_GRP_VENDOR_SPECIFIC)  /* FMTX VSC opcode */
#define HCI_BRCM_UPDATE_BAUDRATE_CMD        (0x0018 | HCI_GRP_VENDOR_SPECIFIC)    /* set baudrate of BCM2035 */
#define HCI_BRCM_WRITE_SCO_PCM_INT_PARAM    (0x001C | HCI_GRP_VENDOR_SPECIFIC)    /* set SCO interface param */
#define HCI_BRCM_READ_SCO_PCM_INT_PARAM     (0x001D | HCI_GRP_VENDOR_SPECIFIC)    /* read SCO interface param */
#define HCI_BRCM_SET_TX_POWER               (0x0026 | HCI_GRP_VENDOR_SPECIFIC)    /* set TX POWER*/
#define HCI_BRCM_WRITE_SLEEP_MODE           (0x0027 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_READ_SLEEP_MODE            (0x0028 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_H4IBSS_CMD                 (0x0029 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_DOWNLOAD_MINI_DRV          (0x002E | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_READ_USER_DEFINED_NVRAM    (0x0033 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_ENABLE_RADIO               (0x0034 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_READ_DIAGNOSTIC_VALUE      (0x0035 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_GET_HID_DEVICE_LIST        (0x0036 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_ADD_HID_DEVICE             (0x0037 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_WRITE_HID_DEVICE_NVRAM     (0x0038 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_DELETE_HID_DEVICE          (0x0039 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_ENABLE_USB_HID_EMULATION   (0x003B | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_READ_COEXISTENCE_MODE      (0x0040 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_WRITE_COEXISTENCE_MODE     (0x0041 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_WRITE_RAM                  (0x004C | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_LAUNCH_RAM                 (0x004E | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_BTW_STARTUP                (0x0053 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_SET_ACL_PRIORITY           (0x0057 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_SET_SEC_MODE               (0x0096 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_ENABLE_H4IBSS              (0x00D4 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_BLE_META_VSC               (0x00E9 | HCI_GRP_VENDOR_SPECIFIC)
#if (defined BRCM_ADV_PCF_LEGACY && BRCM_ADV_PCF_LEGACY == TRUE)
#define HCI_BRCM_BLE_PCF_VSC                HCI_BRCM_BLE_META_VSC
#else
#define HCI_BRCM_BLE_PCF_VSC                (0x0138 | HCI_GRP_VENDOR_SPECIFIC)
#endif
#define HCI_BRCM_FLASH_FW_UPGRADE           (0x0122 | HCI_GRP_VENDOR_SPECIFIC)

#define HCI_BRCM_SUPER_PEEK_POKE            (0x000A | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_ARM_MEM_PEEK                    0x04
#define HCI_ARM_MEM_POKE                    0x05
#define HCI_BRCM_SUPER_PEEK_POKE_LENGTH     9

#define HCI_BRCM_WRITE_I2SPCM_INTF_PARAM    (0x006D | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_READ_CONTROLLER_FEATURES   (0x006E | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_FEATURE_NFC_MASK           0x10
#define HCI_BRCM_FEATURE_NFC_OFF            0

#define HCI_BRCM_READ_VERBOSE_CFG_VER_INFO  (0x0079 | HCI_GRP_VENDOR_SPECIFIC)

/* Dual Stack */
#define HCI_BRCM_PAUSE_TRANSPORT            (0x007A | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_TRANSPORT_RESUME           (0x007B | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_TRANSPORT_ERROR_EVT        0x0C

#define HCI_BRCM_TX_POWER_OPCODE            (0x007D | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_ENABLE_WBS                 (0x007E | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_UIPC_OVER_HCI              (0x008B | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_READ_AUDIO_ROUTE_INFO      (0x00A2 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_UIPC_OVER_HCI_EVT          0x1A
#define HCI_BRCM_ENCAPSULATED_HCI           (0x00A3 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_ENABLE_WBS_MODIFIED        (0x0102 | HCI_GRP_VENDOR_SPECIFIC)

/* VSE subcode - VSE format FF [len] [subcode] [vse params...] */
#define HCI_BRCM_ENCAPSULATED_HCI_EVT       0x1E

/* PCM2 Setup */
#define HCI_BRCM_PCM2_SETUP                 (0x00AE | HCI_GRP_VENDOR_SPECIFIC)

/* BRR */
#define HCI_BRCM_SET_BRR                    (0x00AA | HCI_GRP_VENDOR_SPECIFIC)

/* 3DTV */
#define HCI_BRCM_3D_CTRL                    (0x00B7 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_3D_OFFSET_DELAY            (0x00D6 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_3D_ENABLE_VSYNC_DETECT     (0x00E3 | HCI_GRP_VENDOR_SPECIFIC)

/* GPS */
#define HCI_BRCM_GPS_DATA                   (0x0089 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_GPS_SENSOR_OPCODE          (0x008F | HCI_GRP_VENDOR_SPECIFIC)  /* GPS sensor VSC opcode */

/* MIP: Multicast Individual Polling */
#define HCI_BRCM_INIT_MIP                   (0x00DC | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_MIP_OPEN_CMD               (0x00DD | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_MIP_CLOSE_CMD              (0x00DE | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_MIP_ENC_KEY_CMD            (0x00DF | HCI_GRP_VENDOR_SPECIFIC)

/* Set ACL Priority Extended (with direction param) */
#define HCI_BRCM_SET_ACL_PRIORITY_EXT       (0x011A | HCI_GRP_VENDOR_SPECIFIC)

/* Vendor Specific Opcode for TBFC */
#define HCI_BRCM_TBFC_SUSPEND               (0x00B5 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_TBFC_RESUME                (0x00B6 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_TBFC_CONNECT               (0x00D0 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_TBFC_SET_PARAM             (0x00C2 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_TBFC_READ_PARAM            (0x00C3 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_READ_REMOTE_BPCS_FEATURE   (0x00F5 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_TBFC_WRITE_SCAN_ENABLE     (0x00FA | HCI_GRP_VENDOR_SPECIFIC)

/* Brcm LE Meta command subcode */
#define BTM_BLE_META_ENABLE_CS_FEAT     0x01
#define BTM_BLE_META_IRK_ENABLE         0x01
#define BTM_BLE_META_ADD_IRK_ENTRY      0x02
#define BTM_BLE_META_REMOVE_IRK_ENTRY   0x03
#define BTM_BLE_META_CLEAR_IRK_LIST     0x04
#if (defined BRCM_ADV_PCF_LEGACY && BRCM_ADV_PCF_LEGACY == TRUE)
#define BTM_BLE_META_PF_ENABLE          0x01
#define BTM_BLE_META_PF_FEAT_SEL        0x05
#define BTM_BLE_META_PF_ADDR            0x06
#define BTM_BLE_META_PF_UUID            0x07
#define BTM_BLE_META_PF_SOL_UUID        0x08
#define BTM_BLE_META_PF_LOCAL_NAME      0x09
#define BTM_BLE_META_PF_MANU_DATA       0x0A
#else
#define BTM_BLE_META_PF_ENABLE          0x00
#define BTM_BLE_META_PF_FEAT_SEL        0x01
#define BTM_BLE_META_PF_ADDR            0x02
#define BTM_BLE_META_PF_UUID            0x03
#define BTM_BLE_META_PF_SOL_UUID        0x04
#define BTM_BLE_META_PF_LOCAL_NAME      0x05
#define BTM_BLE_META_PF_MANU_DATA       0x06
#define BTM_BLE_META_PF_SRVC_DATA       0x07
#endif

#define BTM_BLE_META_RSSI_MONITOR       0x0B
#define BTM_BLE_META_TRACK_SENSOR       0x0C
#define BTM_BLE_META_READ_IRK_ENTRY     0x0d
#define BTM_BLE_META_ADD_WL_ATTR        0x0e

/* LE Get Vendor Capabilities Command OCF */
#define HCI_BLE_VENDOR_CAP_OCF     (0x0153 | HCI_GRP_VENDOR_SPECIFIC)

/* Multi adv OCF */
#define HCI_BLE_MULTI_ADV_OCF     (0x0154 | HCI_GRP_VENDOR_SPECIFIC)

/* subcode for multi adv feature */
#define BTM_BLE_MULTI_ADV_SET_PARAM                     0x01
#define BTM_BLE_MULTI_ADV_WRITE_ADV_DATA                0x02
#define BTM_BLE_MULTI_ADV_WRITE_SCAN_RSP_DATA           0x03
#define BTM_BLE_MULTI_ADV_SET_RANDOM_ADDR               0x04
#define BTM_BLE_MULTI_ADV_ENB                           0x05


/* BLE meta vsc header: 1 bytes of sub_code, 1 byte of PCF action */
#define BTM_BLE_META_HDR_LENGTH     2
#define BTM_BLE_PF_FEAT_SEL_LEN     12
#define BTM_BLE_META_ADDR_LEN       7
#define BTM_BLE_META_UUID_LEN       40
#define BTM_BLE_META_ADD_IRK_LEN        24
#define BTM_BLE_META_REMOVE_IRK_LEN     8
#define BTM_BLE_META_CLEAR_IRK_LEN      1
#define BTM_BLE_META_READ_IRK_LEN       2
#define BTM_BLE_META_ADD_WL_ATTR_LEN    9
#define BTM_BLE_META_CS_ENABLE_LEN      5
#define BTM_BLE_IRK_ENABLE_LEN          2
#if (defined BRCM_ADV_PCF_LEGACY && BRCM_ADV_PCF_LEGACY == TRUE)
#define BTM_BLE_PCF_ENABLE_LEN      5
#else
#define BTM_BLE_PCF_ENABLE_LEN      2
#endif

/* ConnectionLess Broadcast Stream VSC */
#define HCI_BRCM_SET_CLB_STREAM             (0x0111 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_RECEIVE_CLB_STREAM         (0x0112 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_WRITE_CLB_STREAM_DATA      (0x0113 | HCI_GRP_VENDOR_SPECIFIC)
#define HCI_BRCM_CLB_STREAM_FLUSH           (0x0114 | HCI_GRP_VENDOR_SPECIFIC)

#define HCI_BRCM_I15DOT4_COMMAND            (0x0177 | HCI_GRP_VENDOR_SPECIFIC)

/* MIP-related definitions */
#define BRCM_MIP_ROLE_MASTER                0x00
#define BRCM_MIP_ROLE_SLAVE                 0x01

#define BRCM_MIP_MODE_A2DP                  0x00
#define BRCM_MIP_MODE_3DG                   0x01
#define BRCM_MIP_MODE_CMBO                  0x02    /* A2DP + 3DG */

/* Parameter information for HCI_BRCM_SET_ACL_PRIORITY */
#define HCI_BRCM_ACL_PRIORITY_PARAM_SIZE    3
#define HCI_BRCM_ACL_PRIORITY_LOW           0x00
#define HCI_BRCM_ACL_PRIORITY_HIGH          0xFF

#define HCI_BRCM_PAUSE_TRANSPORT_LENGTH     6

/* Parameter information for HCI_BRCM_SET_ACL_PRIORITY_EXT */
#define HCI_BRCM_ACL_PRIORITY_EXT_PARAM_SIZE 4

/* Parameter information for HCI_BRCM_SCO_PCM_INT_PARAM */
#define HCI_BRCM_SCO_PCM_PARAM_SIZE         5
#define HCI_BRCM_SCO_ROUTE_PCM              0
#define HCI_BRCM_SCO_ROUTE_HCI              1

/* Parameter information for HCI_BRCM_WRITE_I2SPCM_INTF_PARAM */
#define HCI_BRCM_I2SPCM_PARAM_SIZE          4
#define HCI_BRCM_I2SPCM_I2S_DISABLE         0
#define HCI_BRCM_I2SPCM_I2S_ENABLE          1
#define HCI_BRCM_I2SPCM_IS_SLAVE            0
#define HCI_BRCM_I2SPCM_IS_MASTER           1
#define HCI_BRCM_I2SPCM_IS_DEFAULT_ROLE     2
#define HCI_BRCM_I2SPCM_SAMPLE_8K           0
#define HCI_BRCM_I2SPCM_SAMPLE_16K          1
#define HCI_BRCM_I2SPCM_SAMPLE_4K           2
#define HCI_BRCM_I2SPCM_SAMPLE_DEFAULT      3
#define HCI_BRCM_I2SPCM_CLOCK_128K          0
#define HCI_BRCM_I2SPCM_CLOCK_256K          1
#define HCI_BRCM_I2SPCM_CLOCK_512K          2
#define HCI_BRCM_I2SPCM_CLOCK_1024K         3
#define HCI_BRCM_I2SPCM_CLOCK_2048K         4
#define HCI_BRCM_I2SPCM_CLOCK_DEFAULT       5

#define HCI_BRCM_H4IBSS_EVT             0xEF  /* Vendor specific events for H4IBSS */


/* Broadcom Vendor Specific Event sub-codes */
#define HCI_BRCM_VSE_SUBCODE_AUTOMATIC_PAIRING_REQUEST              0x01
#define HCI_BRCM_VSE_SUBCODE_RX_COMPLETE                            0x02
#define HCI_BRCM_VSE_SUBCODE_LINK_QUALITY_REPORT                    0x03
#define HCI_BRCM_VSE_SUBCODE_IOP_TEST_RX_1_PACKET_REPORT            0x04
#define HCI_BRCM_VSE_SUBCODE_IOP_TEST_RX_N_PACKETS_REPORT_SUMMARY   0x05
#define HCI_BRCM_VSE_SUBCODE_MODULE_XRAM_TEST_REPORT_SUMMARY        0x06
#define HCI_BRCM_VSE_SUBCODE_CONNECTIONLESS_RX_TEST_STATISTICS      0x07
#define HCI_BRCM_VSE_SUBCODE_FM_INTERRUPT                           0x08
#define HCI_BRCM_VSE_SUBCODE_TCA_DEBUG_REPORT                       0x0B
#define HCI_BRCM_VSE_SUBCODE_RAM_ROM_CLOCK_TEST_STATUS              0x0C
#define HCI_BRCM_VSE_SUBCODE_DEBUG_OUTPUT_STRING                    0x0D
#define HCI_BRCM_VSE_SUBCODE_PCM_DATA_DUMP                          0x0E
#define HCI_BRCM_VSE_SUBCODE_PROTOCOL_MESSAGE_DUMP                  0x0F
#define HCI_BRCM_VSE_SUBCODE_GPS_DATA                               0x10
#define HCI_BRCM_VSE_SUBCODE_RESERVED_11                            0x11
#define HCI_BRCM_VSE_SUBCODE_GPS_SENSOR_EVENT                       0x12
#define HCI_BRCM_VSE_SUBCODE_GPS_SENSOR_SETUP_EVENT                 0x13
#define HCI_BRCM_VSE_SUBCODE_MEIF_RX_EVENT                          0x14
#define HCI_BRCM_VSE_SUBCODE_UNDETECTED_CRC_ERROR_EVENT             0x15
#define HCI_BRCM_VSE_SUBCODE_BFC_DISCONNECT_EVENT                   0x16
#define HCI_BRCM_VSE_SUBCODE_CUSTOMER_SPECIFIC_CS_ENERGY_DETECTED_EVENTS 0x17
#define HCI_BRCM_VSE_SUBCODE_BFC_POLLING_INFO_EVENT                 0x18
#define HCI_BRCM_VSE_SUBCODE_EEPROM_CHIP_ERASE_BY_MINIDRIVER_STATUS_REPORT 0xCE

/* MIP related Vendor Specific Event */
#define HCI_BRCM_VSE_SUBCODE_MIP_MODE_CHANGE            0x27
#define HCI_BRCM_VSE_SUBCODE_MIP_DISCONNECT             0x28
#define HCI_BRCM_VSE_SUBCODE_MIP_EIR_CMPLT              0x29
#define HCI_BRCM_VSE_SUBCODE_MIP_ENC_KEY                0x2A
#define HCI_BRCM_VSE_SUBCODE_MIP_DATA_RECEIVED          0x2E

/* Vendor Specific Event for TBFC */
#define HCI_BRCM_VSE_SUBCODE_TBFC_SUSPEND_CMPL          0x16
#define HCI_BRCM_VSE_SUBCODE_TBFC_RESUME_CMPL           0x18
#define HCI_BRCM_VSE_SUBCODE_TBFC_SUSPEND_REQ           0x2A
#define HCI_BRCM_VSE_SUBCODE_TBFC_RESUME_REQ            0x2B
#define HCI_BRCM_VSE_SUBCODE_TBFC_NO_DIST               0x2C
#define HCI_BRCM_VSE_SUBCODE_TBFC_SUSPENDED             0x39
#define HCI_BRCM_VSE_SUBCODE_TBFC_SUSPENDING            0x42

/* Vendor Specific Event for 3D */
#define HCI_BRCM_VSE_SUBCODE_3D_MODE_CHANGE             0x21    /* 3D Mode Change */
#define HCI_BRCM_VSE_SUBCODE_3D_FRAME_PERIOD            0x22    /* 3D Frame Period */
#define HCI_BRCM_VSE_SUBCODE_3D_CLOCK_SNAPSHOT          0x23    /* 3D Clock SnapShot */
#define HCI_BRCM_VSE_SUBCODE_3D_MULTICAST_LOCK_CPLT     0x24    /* 3D Slave Lock Complete */
#define HCI_BRCM_VSE_SUBCODE_3D_MULTICAST_SYNC_TO       0x25    /* 3D Slave Sync Timeout */
#define HCI_BRCM_VSE_SUBCODE_3D_EIR_HANDSHAKE_CPLT      0x29    /* EIR Handshake (Connect) */
#define HCI_BRCM_VSE_SUBCODE_3D_MULTICAST_RX_DATA       0x30    /* Multicast Rx Data */

/* VSE Parameters */
/*  ConnectionLess Broadcast Stream Tx Packet Status */
#define HCI_CLB_STREAM_TX_PKT_STS_NULL          0x00
#define HCI_CLB_STREAM_TX_PKT_STS_OK            0x01
#define HCI_CLB_STREAM_TX_PKT_STS_DISCARD       0x02
#define HCI_CLB_STREAM_TX_PKT_STS_FLUSH         0x03

#define HCI_BRCM_VSE_SUBCODE_CLB_STREAM_TX_EVT          0x4C    /* CLB Stream Tx Cfm */
#define HCI_BRCM_VSE_SUBCODE_CLB_MULTI_TRAIN_RX_EVT     0x4D    /* CLB Multiple Train Rx */

/* multi adv VSE subcode */
#define HCI_VSE_SUBCODE_BLE_MULTI_ADV_ST_CHG       0x55         /* multi adv instance state change */

#define HCI_BRCM_VSE_SUBCODE_FW_CORE_DUMP               0xF7    /* FW Core Dump Data */

/* AMP VSE events
*/
#define AMP_VSE_CHANSPEC_CHAN_MASK      0x00ff

#define AMP_VSE_CHANSPEC_CTL_SB_MASK    0x0300
#define AMP_VSE_CHANSPEC_CTL_SB_LOWER   0x0100
#define AMP_VSE_CHANSPEC_CTL_SB_UPPER   0x0200
#define AMP_VSE_CHANSPEC_CTL_SB_NONE    0x0300

#define AMP_VSE_CHANSPEC_BW_MASK        0x0C00
#define AMP_VSE_CHANSPEC_BW_10          0x0400
#define AMP_VSE_CHANSPEC_BW_20          0x0800
#define AMP_VSE_CHANSPEC_BW_40          0x0C00

#define AMP_VSE_CHANSPEC_BAND_MASK      0xf000
#define AMP_VSE_CHANSPEC_BAND_5G        0x1000
#define AMP_VSE_CHANSPEC_BAND_2G        0x2000

/* PCM2 related */
#define HCI_BRCM_CUSTOMER_EXT_ARC_LEN   4
#define LOC_AUDIO_ROUTING_CONTROL       0x88
#define LOC_ARC_CHANNEL_ID              0xF3

#define ARC_INTERFACE_PCMI2S            0x01
#define ARC_INTERFACE_FM_I2S            0x02
#define ARC_INTERFACE_SLB               0x03

#define ARC_MODE_NOT_USED               0x00
#define ARC_MODE_BT_AUDIO               0x01
#define ARC_MODE_FMRX                   0x02
#define ARC_MODE_FMTX                   0x03
#define ARC_MODE_SHARED_BT_FMRX         0x04
#define ARC_MODE_SHARED_BT_FMTX         0x05

#define BRCM_PCM2_SETUP_READ_SIZE       0x01
#define BRCM_PCM2_SETUP_WRITE_SIZE      0x1A

#define BTM_READ_BPCS_FEAT_LENGTH       2

#define BTM_SET_LOCAL_DEV_ADDR_LENGTH   6

#define BTM_SET_TX_POWER_LENGTH         3
#endif

