/******************************************************************************
*
* $ Copyright Cypress Semiconductor $
*
*****************************************************************************/

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

/**
 * IMPORTANT
 * NOTE:
 *          Do NOT change the contents of this file. The content is common
 *          to both the codec interface as well as to the playback system.
 *          Any changes here can cause system crashes or poor playback.
 */

/*****************************************************************************
**  Constants
*****************************************************************************/

#define WICED_SBC_IE_SAMP_FREQ_MSK    0xF    /* b7-b4 sampling frequency */
#define WICED_SBC_IE_SAMP_FREQ_16     0x8    /* 16  kHz */
#define WICED_SBC_IE_SAMP_FREQ_32     0x4    /* 32  kHz */
#define WICED_SBC_IE_SAMP_FREQ_44     0x2    /* 44.1kHz */
#define WICED_SBC_IE_SAMP_FREQ_48     0x1    /* 48  kHz */

#define WICED_SBC_IE_CH_MD_MSK        0x0F    /* b3-b0 channel mode */
#define WICED_SBC_IE_CH_MD_MONO       0x8    /* mono */
#define WICED_SBC_IE_CH_MD_DUAL       0x4    /* dual stereo */
#define WICED_SBC_IE_CH_MD_STEREO     0x2    /* stereo */
#define WICED_SBC_IE_CH_MD_JOINT      0x1    /* joint stereo */

#define WICED_SBC_IE_BLOCKS_MSK       0xF    /* b7-b4 number of blocks */
#define WICED_SBC_IE_BLOCKS_4         0x8    /* 4 blocks */
#define WICED_SBC_IE_BLOCKS_8         0x4    /* 8 blocks */
#define WICED_SBC_IE_BLOCKS_12        0x2    /* 12blocks */
#define WICED_SBC_IE_BLOCKS_16        0x1    /* 16blocks */

#define WICED_SBC_IE_SUBBAND_MSK      0xC    /* b3-b2 number of subbands */
#define WICED_SBC_IE_SUBBAND_4        0x8    /* b3: 4 */
#define WICED_SBC_IE_SUBBAND_8        0x4    /* b2: 8 */

#define WICED_SBC_IE_ALLOC_MD_MSK     0x3    /* b1-b0 allocation mode */
#define WICED_SBC_IE_ALLOC_MD_S       0x2    /* b1: SNR */
#define WICED_SBC_IE_ALLOC_MD_L       0x1    /* b0: loundess */

#define WICED_SBC_IE_MIN_BITPOOL      2
#define WICED_SBC_IE_MAX_BITPOOL      250



/*****************************************************************************
**  Type Definitions
*****************************************************************************/

/* data type for the SBC Codec Information Element*/
typedef struct
{
    unsigned char   samp_freq;      /* Sampling frequency */
    unsigned char   ch_mode;        /* Channel mode */
    unsigned char   block_len;      /* Block length */
    unsigned char   num_subbands;   /* Number of subbands */
    unsigned char   alloc_mthd;     /* Allocation method */
    unsigned char   max_bitpool;    /* Maximum bitpool */
    unsigned char   min_bitpool;    /* Minimum bitpool */
} wiced_sbc_codec_params_t;


#ifdef __cplusplus
}
#endif
