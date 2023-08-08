#ifndef _RK_AIQ_OFFLINE_RAW_H_
#define _RK_AIQ_OFFLINE_RAW_H_

#include <stdint.h>

#define START_TAG           0xFF00
#define FORMAT_TAG          0xFF01
#define NORMAL_RAW_TAG      0xFF02
#define HDR_S_RAW_TAG       0xFF03
#define HDR_M_RAW_TAG       0xFF04
#define HDR_L_RAW_TAG       0xFF05
#define STATS_TAG           0xFF06
#define ISP_REG_FMT_TAG     0xFF07
#define ISP_REG_TAG         0xFF08
#define ISPP_REG_FMT_TAG    0xFF09
#define ISPP_REG_TAG        0xFF0A
#define PLATFORM_TAG        0xFF0B
#define END_TAG             0x00FF

#define TAG_BYTE_LEN        (2)
#define BLOCK_BYTE_LEN      (4)

struct _block_header {
    unsigned short block_id;
    unsigned int block_length;
}__attribute__ ((packed));

struct _st_addrinfo
{
    unsigned int fd;
    unsigned int haddr;
    unsigned int laddr;
    unsigned int size;
}__attribute__ ((packed));

struct _raw_format
{
    unsigned short tag;
    unsigned int size;
    unsigned short vesrion;
    char sensor[32];
    char scene[32];
    uint32_t frame_id;
    unsigned short width;
    unsigned short height;
    unsigned char bit_width;
    unsigned char bayer_fmt;
    unsigned char hdr_mode;
    unsigned char buf_type;
    unsigned short line_length;
    unsigned short acive_line_length;
    unsigned char byte_order;
}__attribute__ ((packed));

struct _frame_info
{
    unsigned short tag;
    unsigned int size;
    unsigned short vesrion;
    uint32_t frame_id;

    float normal_exp;
    float normal_gain;
    unsigned int normal_exp_reg;
    unsigned int normal_gain_reg;

    float hdr_exp_l;
    float hdr_gain_l;
    unsigned int hdr_exp_l_reg;
    unsigned int hdr_gain_l_reg;

    float hdr_exp_m;
    float hdr_gain_m;
    unsigned int hdr_gain_m_reg;
    unsigned int hdr_exp_m_reg;

    float hdr_exp_s;
    float hdr_gain_s;
    unsigned int hdr_exp_s_reg;
    unsigned int hdr_gain_s_reg;

    float awg_rgain;
    float awg_bgain;
}__attribute__ ((packed));

#endif
