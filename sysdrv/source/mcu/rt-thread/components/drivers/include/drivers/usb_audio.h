/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    usb_audio.h
  * @version V0.1
  * @brief   usb audio driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-09-17     Liangfeng Wu    first implementation
  *
  ******************************************************************************
  */
#ifndef _USB_AUDIO_H
#define _USB_AUDIO_H

/* bInterfaceProtocol values to denote the version of the standard used */
#define UAC_VERSION_1           0x00
#define UAC_VERSION_2           0x20

/* A.2 Audio Interface Subclass Codes */
#define USB_SUBCLASS_AUDIOCONTROL   0x01
#define USB_SUBCLASS_AUDIOSTREAMING 0x02
#define USB_SUBCLASS_MIDISTREAMING  0x03

/* A.5 Audio Class-Specific AC Interface Descriptor Subtypes */
#define UAC_HEADER              0x01
#define UAC_INPUT_TERMINAL      0x02
#define UAC_OUTPUT_TERMINAL     0x03
#define UAC_MIXER_UNIT          0x04
#define UAC_SELECTOR_UNIT       0x05
#define UAC_FEATURE_UNIT        0x06
#define UAC1_PROCESSING_UNIT    0x07
#define UAC1_EXTENSION_UNIT     0x08

/* A.6 Audio Class-Specific AS Interface Descriptor Subtypes */
#define UAC_AS_GENERAL          0x01
#define UAC_FORMAT_TYPE         0x02
#define UAC_FORMAT_SPECIFIC     0x03

/* A.8 Audio Class-Specific Endpoint Descriptor Subtypes */
#define UAC_EP_GENERAL          0x01

/* A.9 Audio Class-Specific Request Codes */
#define UAC_SET_            0x00
#define UAC_GET_            0x80

#define UAC__CUR            0x1
#define UAC__MIN            0x2
#define UAC__MAX            0x3
#define UAC__RES            0x4
#define UAC__MEM            0x5

#define UAC_SET_CUR         (UAC_SET_ | UAC__CUR)
#define UAC_GET_CUR         (UAC_GET_ | UAC__CUR)
#define UAC_SET_MIN         (UAC_SET_ | UAC__MIN)
#define UAC_GET_MIN         (UAC_GET_ | UAC__MIN)
#define UAC_SET_MAX         (UAC_SET_ | UAC__MAX)
#define UAC_GET_MAX         (UAC_GET_ | UAC__MAX)
#define UAC_SET_RES         (UAC_SET_ | UAC__RES)
#define UAC_GET_RES         (UAC_GET_ | UAC__RES)
#define UAC_SET_MEM         (UAC_SET_ | UAC__MEM)
#define UAC_GET_MEM         (UAC_GET_ | UAC__MEM)

#define UAC_GET_STAT            0xff

/* A.10.1 Terminal Control Selectors */
#define UAC_TERM_COPY_PROTECT       0x01

/* A.10.2 Feature Unit Control Selectors */
#define UAC_FU_MUTE                 0x01
#define UAC_FU_VOLUME               0x02
#define UAC_FU_BASS                 0x03
#define UAC_FU_MID                  0x04
#define UAC_FU_TREBLE               0x05
#define UAC_FU_GRAPHIC_EQUALIZER    0x06
#define UAC_FU_AUTOMATIC_GAIN       0x07
#define UAC_FU_DELAY                0x08
#define UAC_FU_BASS_BOOST           0x09
#define UAC_FU_LOUDNESS             0x0a

#define UAC_CONTROL_BIT(CS) (1 << ((CS) - 1))

/* Terminals - 2.1 USB Terminal Types */
#define UAC_TERMINAL_UNDEFINED      0x100
#define UAC_TERMINAL_STREAMING      0x101
#define UAC_TERMINAL_VENDOR_SPEC    0x1FF

#define UAC_EP_CS_ATTR_SAMPLE_RATE      0x01
#define UAC_EP_CS_ATTR_PITCH_CONTROL    0x02
#define UAC_EP_CS_ATTR_FILL_MAX         0x80

#pragma pack(1)

/* Terminal Control Selectors */
/* 4.3.2  Class-Specific AC Interface Descriptor */
struct uac1_ac_header_descriptor
{
    rt_uint8_t  bLength;            /* 8 + n */
    rt_uint8_t  bDescriptorType;    /* USB_DT_CS_INTERFACE */
    rt_uint8_t  bDescriptorSubtype; /* UAC_MS_HEADER */
    rt_uint16_t bcdADC;             /* 0x0100 */
    rt_uint16_t wTotalLength;       /* includes Unit and Terminal desc. */
    rt_uint8_t  bInCollection;      /* n */
    rt_uint8_t  baInterfaceNr[];    /* [n] */
};

#define UAC_DT_AC_HEADER_SIZE(n)    (8 + (n))

/* As above, but more useful for defining your own descriptors: */
#define DECLARE_UAC_AC_HEADER_DESCRIPTOR(n)     \
struct uac1_ac_header_descriptor_##n {          \
    rt_uint8_t  bLength;                        \
    rt_uint8_t  bDescriptorType;                \
    rt_uint8_t  bDescriptorSubtype;             \
    rt_uint16_t bcdADC;                         \
    rt_uint16_t wTotalLength;                   \
    rt_uint8_t  bInCollection;                  \
    rt_uint8_t  baInterfaceNr[n];               \
} __attribute__ ((packed))

/* 4.3.2.1 Input Terminal Descriptor */
struct uac_input_terminal_descriptor
{
    rt_uint8_t  bLength;            /* in bytes: 12 */
    rt_uint8_t  bDescriptorType;    /* CS_INTERFACE descriptor type */
    rt_uint8_t  bDescriptorSubtype; /* INPUT_TERMINAL descriptor subtype */
    rt_uint8_t  bTerminalID;        /* Constant uniquely terminal ID */
    rt_uint16_t wTerminalType;      /* USB Audio Terminal Types */
    rt_uint8_t  bAssocTerminal;     /* ID of the Output Terminal associated */
    rt_uint8_t  bNrChannels;        /* Number of logical output channels */
    rt_uint16_t wChannelConfig;
    rt_uint8_t  iChannelNames;
    rt_uint8_t  iTerminal;
};

#define UAC_DT_INPUT_TERMINAL_SIZE          12

/* Terminals - 2.2 Input Terminal Types */
#define UAC_INPUT_TERMINAL_UNDEFINED                0x200
#define UAC_INPUT_TERMINAL_MICROPHONE               0x201
#define UAC_INPUT_TERMINAL_DESKTOP_MICROPHONE       0x202
#define UAC_INPUT_TERMINAL_PERSONAL_MICROPHONE      0x203
#define UAC_INPUT_TERMINAL_OMNI_DIR_MICROPHONE      0x204
#define UAC_INPUT_TERMINAL_MICROPHONE_ARRAY         0x205
#define UAC_INPUT_TERMINAL_PROC_MICROPHONE_ARRAY    0x206

/* 4.3.2.2 Output Terminal Descriptor */
struct uac1_output_terminal_descriptor
{
    rt_uint8_t  bLength;            /* in bytes: 9 */
    rt_uint8_t  bDescriptorType;    /* CS_INTERFACE descriptor type */
    rt_uint8_t  bDescriptorSubtype; /* OUTPUT_TERMINAL descriptor subtype */
    rt_uint8_t  bTerminalID;        /* Constant uniquely terminal ID */
    rt_uint16_t wTerminalType;      /* USB Audio Terminal Types */
    rt_uint8_t  bAssocTerminal;     /* ID of the Input Terminal associated */
    rt_uint8_t  bSourceID;          /* ID of the connected Unit or Terminal*/
    rt_uint8_t  iTerminal;
};

#define UAC_DT_OUTPUT_TERMINAL_SIZE         9

/* Terminals - 2.3 Output Terminal Types */
#define UAC_OUTPUT_TERMINAL_UNDEFINED                   0x300
#define UAC_OUTPUT_TERMINAL_SPEAKER                     0x301
#define UAC_OUTPUT_TERMINAL_HEADPHONES                  0x302
#define UAC_OUTPUT_TERMINAL_HEAD_MOUNTED_DISPLAY_AUDIO  0x303
#define UAC_OUTPUT_TERMINAL_DESKTOP_SPEAKER             0x304
#define UAC_OUTPUT_TERMINAL_ROOM_SPEAKER                0x305
#define UAC_OUTPUT_TERMINAL_COMMUNICATION_SPEAKER       0x306
#define UAC_OUTPUT_TERMINAL_LOW_FREQ_EFFECTS_SPEAKER    0x307

/* Set bControlSize = 2 as default setting */
#define UAC_DT_FEATURE_UNIT_SIZE(ch)        (7 + ((ch) + 1) * 2)

/* As above, but more useful for defining your own descriptors: */
#define DECLARE_UAC_FEATURE_UNIT_DESCRIPTOR(ch)     \
struct uac_feature_unit_descriptor_##ch {           \
    rt_uint8_t  bLength;                            \
    rt_uint8_t  bDescriptorType;                    \
    rt_uint8_t  bDescriptorSubtype;                 \
    rt_uint8_t  bUnitID;                            \
    rt_uint8_t  bSourceID;                          \
    rt_uint8_t  bControlSize;                       \
    rt_uint16_t bmaControls[ch + 1];                \
    rt_uint8_t  iFeature;                           \
} __attribute__ ((packed))

/* 4.5.2 Class-Specific AS Interface Descriptor */
struct uac1_as_header_descriptor
{
    rt_uint8_t  bLength;            /* in bytes: 7 */
    rt_uint8_t  bDescriptorType;    /* USB_DT_CS_INTERFACE */
    rt_uint8_t  bDescriptorSubtype; /* AS_GENERAL */
    rt_uint8_t  bTerminalLink;      /* Terminal ID of connected Terminal */
    rt_uint8_t  bDelay;             /* Delay introduced by the data path */
    rt_uint16_t wFormatTag;         /* The Audio Data Format */
};

#define UAC_DT_AS_HEADER_SIZE       7

/* Formats - A.1.1 Audio Data Format Type I Codes */
#define UAC_FORMAT_TYPE_I_UNDEFINED     0x0
#define UAC_FORMAT_TYPE_I_PCM           0x1
#define UAC_FORMAT_TYPE_I_PCM8          0x2
#define UAC_FORMAT_TYPE_I_IEEE_FLOAT    0x3
#define UAC_FORMAT_TYPE_I_ALAW          0x4
#define UAC_FORMAT_TYPE_I_MULAW         0x5

#define DECLARE_UAC_FORMAT_TYPE_I_DISCRETE_DESC(n)      \
struct uac_format_type_i_discrete_descriptor_##n {      \
    rt_uint8_t  bLength;                                \
    rt_uint8_t  bDescriptorType;                        \
    rt_uint8_t  bDescriptorSubtype;                     \
    rt_uint8_t  bFormatType;                            \
    rt_uint8_t  bNrChannels;                            \
    rt_uint8_t  bSubframeSize;                          \
    rt_uint8_t  bBitResolution;                         \
    rt_uint8_t  bSamFreqType;                           \
    rt_uint8_t  tSamFreq[n][3];                         \
} __attribute__ ((packed))

#define UAC_FORMAT_TYPE_I_DISCRETE_DESC_SIZE(n) (8 + (n * 3))

/* Formats - A.2 Format Type Codes */
#define UAC_FORMAT_TYPE_UNDEFINED   0x0
#define UAC_FORMAT_TYPE_I           0x1
#define UAC_FORMAT_TYPE_II          0x2
#define UAC_FORMAT_TYPE_III         0x3
#define UAC_EXT_FORMAT_TYPE_I       0x81
#define UAC_EXT_FORMAT_TYPE_II      0x82
#define UAC_EXT_FORMAT_TYPE_III     0x83

struct uac_iso_endpoint_descriptor
{
    rt_uint8_t  bLength;            /* in bytes: 7 */
    rt_uint8_t  bDescriptorType;    /* USB_DT_CS_ENDPOINT */
    rt_uint8_t  bDescriptorSubtype; /* EP_GENERAL */
    rt_uint8_t  bmAttributes;
    rt_uint8_t  bLockDelayUnits;
    rt_uint16_t wLockDelay;
};

#define UAC_ISO_ENDPOINT_DESC_SIZE  7

#pragma pack()

#endif /* _USB_AUDIO_H */
