#ifndef TYPES_H
#define TYPES_H

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

#define MPEG2_5 0
#define MPEG2   2
#define MPEG1   3

#define MODE_STEREO       0
#define MODE_MS_STEREO    1
#define MODE_DUAL_CHANNEL 2
#define MODE_MONO         3

#define LAYER_3 1
#define LAYER_2 2
#define LAYER_1 3
#define samp_per_frame  1152
#define samp_per_frame2 576
#define PI              3.14159265358979
#define HAN_SIZE        512
#define SBLIMIT         32

#define BUFFER_SIZE  2048//2048//1384   /* output buffer size in bytes 576 bytes */

#define FIFO_SIZE  128
typedef struct
{
    int si_len; /* number of bytes in side info */
    int fr_len; /* number of data bytes in frame */
    unsigned char side[40]; /* side info max len = (288 bits / 8) + 1 which gets cleared 最初40*/
} fifo_t;

#define main_size 16384   //main_size = si->resv_drain

typedef struct
{
    unsigned int i;   /* file buffer index */
    unsigned char *b; /* buffer pointer */
} bs_t;

typedef struct
{
    FILE *file;
    int  type;
    int  layr;
    int  mode;
    int  bitr;
    int  psyc;
    int  emph;
    int  padding;
    long samples_per_frame;
    long bits_per_frame;
    long bits_per_slot;
    long total_frames;
    int  bitrate_index;
    int  samplerate;
    int  samplerate_index;
    int crc;
    int ext;
    int mode_ext;
    int copyright;
    int original;
    int channels;
    int granules;
    int resv_limit;
    int cutoff;
} mpeg_t;

typedef struct
{
    char *in_buf;
    char *out_buf;
    mpeg_t mpeg;
} config_t;

typedef struct
{
    long          remainder;
    long          bytes_per_frame;
    long          lag;
    int           mean_bits;
    int           sideinfo_len;
} mp3_info;

#define HUFFBITS unsigned long int
#define HTN     34
#define MXOFF   250

struct huffcodetab
{
    unsigned int xlen;    /*max. x-index+                         */
    unsigned int ylen;    /*max. y-index+                         */
    unsigned int linbits; /*number of linbits                     */
    unsigned int linmax;  /*max number to be stored in linbits    */
    HUFFBITS *table;      /*pointer to array[xlen][ylen]          */
    unsigned char *hlen;  /*pointer to array[xlen][ylen]          */
};

extern struct huffcodetab ht[HTN];/* global memory block                */
/* array of all huffcodtable headers    */
/* 0..31 Huffman code table 0..31       */
/* 32,33 count1-tables                  */

/* Side information */
typedef struct
{
    unsigned part2_3_length;
    unsigned big_values;
    unsigned count1;
    unsigned global_gain;
    unsigned table_select[3];
    unsigned region0_count;
    unsigned region1_count;
    unsigned count1table_select;
    unsigned address1;
    unsigned address2;
    unsigned address3;
    int quantizerStepSize;
    int *scalefac_band_long;
    int cutoff;
} gr_info;

typedef struct
{
    int main_data_begin;
    struct
    {
        struct
        {
            gr_info tt;
        } ch[2];
    } gr[2];
    int resv_drain;
} L3_side_info_t;

typedef struct
{
    mp3_info info;
    config_t config;
    unsigned char header[4];

    fifo_t fifo[FIFO_SIZE];
    unsigned char main_[main_size];  /* main data buffer (length always less than this)实测68*/
    bs_t bs;
    int l3_enc[2][2][samp_per_frame2];
    long mdct_freq[2][2][samp_per_frame2];
    L3_side_info_t side_info;
    long l3_sb_sample[2][3][18][SBLIMIT];
    int main_data_begin;
    int wr;
    int rd;
    int by;
    int bi;
    int count;
    int bits;
    int off[2];
    int *scalefac_band_long;
    int frame_size;
} mp3_enc;

/* function prototypes */

/* bitstream.c */
void close_bit_stream(mp3_enc *mp3);
void open_bit_stream(mp3_enc *mp3);

unsigned int L3_format_bitstream(mp3_enc *mp3, unsigned char **ppOutBuf);
mp3_enc *Mp3EncodeVariableInit(int samplerate, int channel, int  Bitrate);
void Mp3EncodeDeinit(mp3_enc *mp3);
/* l3loop.c */

/* layer3.c */
long L3_compress(mp3_enc *mp3, int len, unsigned char **ppOutBuf);

/* coder.c */
void L3_window_filter_subband(mp3_enc *mp3, unsigned long **buffer, long s[SBLIMIT],
                              int k);
#if 1
void L3_iteration_loop(mp3_enc *mp3) ;
void L3_mdct_sub(mp3_enc *mp3);
#else
void L3_iteration_loop(long            mdct_freq_org[1][1][samp_per_frame2],
                       L3_side_info_t *side_info,
                       int             l3_enc[1][1][samp_per_frame2],
                       int             mean_bits);
void L3_mdct_sub(mp3_enc *mp3, long sb_sample[2][3][18][SBLIMIT],
                 long mdct_freq[2][2][samp_per_frame2]);

#endif
/* utils.c */
long mul(int x, int y);
long muls(int x, int y);
long mulr(int x, int y);
long mulsr(int x, int y);

#endif
