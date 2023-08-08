/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "base_types.h"
#include "os_dep.h"
#include "sdio_delay_tuning.h"

INTN sdio_tuning_more_info = 0;

#define MAX_SDIO_DELAY_TUNING   100
#define MAX_SDIO_TUNING_DATA_PARTTEN    128
#define MAX_SDIO_TUNING_ALIGN_PARTTEN   512

#define SDIO_TUNING_INPUT       0
#define SDIO_TUNING_OUTPUT      1

#define SDIO_INPUT_DELAY_MSK            0x04 
#define SDIO_INPUT_DELAY_SFT            2
#define SDIO_INPUT_DELAY_LEVEL_MSK      0x03
#define SDIO_INPUT_DELAY_LEVEL_SFT      0
#define SDIO_OUTPUT_DELAY_MSK           0x40
#define SDIO_OUTPUT_DELAY_SFT           6
#define SDIO_OUTPUT_DELAY_LEVEL_MSK     0x30
#define SDIO_OUTPUT_DELAY_LEVEL_SFT     4

#define SDIO_DELAY_LEVEL_OFF    0
#define SDIO_DELAY_LEVEL_0      1
#define SDIO_DELAY_LEVEL_1      2
#define SDIO_DELAY_LEVEL_2      3
#define SDIO_DELAY_LEVEL_3      4
#define MAX_SDIO_DELAY_LEVEL    5

#define SDIO_DATA0_DELAY      0
#define SDIO_DATA1_DELAY      1
#define SDIO_DATA2_DELAY      2
#define SDIO_DATA3_DELAY      3

#define SDIO_TUNING_INFO    1
#define SDIO_TUNING_DBG     2

struct ssv6xxx_sdio_tuning_glue {
    CHAR8 pattern[MAX_SDIO_TUNING_DATA_PARTTEN+MAX_SDIO_TUNING_ALIGN_PARTTEN];
    CHAR8 buff[MAX_SDIO_TUNING_DATA_PARTTEN+MAX_SDIO_TUNING_ALIGN_PARTTEN];;
    
    //D0 input_delay, D0 output_delay
    CHAR8 one_bit_delay[MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL];
    UINT32 total_one_bit;
    UINT32 success_one_bit;
    
    //D0 output delay, D1 output delay, D2 output delay, D3 output delay
    CHAR8 data_out_delay[MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL];
    UINT32 total_data_out;
    UINT32 success_data_out;
    
    //D0 input delay, D1 input delay, D2 input delay, D3 input delay
    CHAR8 data_in_delay[MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL];
    UINT32 total_data_in;
    UINT32 success_data_in;
};

static INTN ssv6xxx_sdio_tuning_cmd52_read(OS_WRAPPER_SDIO_FUNC *func, UINT32 addr, UINT8 *value)
{
    INTN ret = 0;

    OS_WRAPPER_SDIO_HOST_LOCK(func);
    *value = OS_WRAPPER_SDIO_CMD52_RD(func, addr, &ret);
    OS_WRAPPER_SDIO_HOST_UNLOCK(func);

    return ret;
}

static INTN ssv6xxx_sdio_tuning_cmd52_write(OS_WRAPPER_SDIO_FUNC *func, UINT32 addr, UINT8 value)
{
    INTN ret = 0;

    OS_WRAPPER_SDIO_HOST_LOCK(func);
    OS_WRAPPER_SDIO_CMD52_WR(func, value, addr, &ret);
    OS_WRAPPER_SDIO_HOST_UNLOCK(func);

    return ret;
}

static INTN ssv6xxx_sdio_tuning_cmd53_read(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN align_size = 0;
    INTN ret = 0;

    align_size = OS_WRAPPER_SDIO_ALIGN_SIZE(func, MAX_SDIO_TUNING_DATA_PARTTEN);
    OS_WRAPPER_SDIO_HOST_LOCK(func);
    ret = OS_WRAPPER_SDIO_CMD53_RD(func, glue->buff, 0x10000, align_size);
    OS_WRAPPER_SDIO_HOST_UNLOCK(func);

    return ret;
}

static INTN ssv6xxx_sdio_tuning_cmd53_write(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN align_size = 0;
    INTN ret = 0;

    align_size = OS_WRAPPER_SDIO_ALIGN_SIZE(func, MAX_SDIO_TUNING_DATA_PARTTEN);
    OS_WRAPPER_SDIO_HOST_LOCK(func);
    ret = OS_WRAPPER_SDIO_CMD53_WR(func, 0x10000, glue->pattern, align_size);
    OS_WRAPPER_SDIO_HOST_UNLOCK(func);

    return ret;
}

static INTN ssv6xxx_sdio_tuning_read_register(OS_WRAPPER_SDIO_FUNC *func, UINT32 addr, UINT32 *buf)
{
    INTN ret = (-1);
    UINT8 data[4];
    
    OS_WRAPPER_SDIO_HOST_LOCK(func);

    // 4 bytes address
    data[0] = (addr >> ( 0 )) &0xff;
    data[1] = (addr >> ( 8 )) &0xff;
    data[2] = (addr >> ( 16 )) &0xff;
    data[3] = (addr >> ( 24 )) &0xff;

    //8 byte ( 4 bytes address , 4 bytes data )
    while(OS_WRAPPER_SDIO_CMD52_RD(func, 0xf, &ret)!=2);  // cmd52 check status
    ret = OS_WRAPPER_SDIO_CMD53_WR(func, 0x10020, data, 4);
    while(OS_WRAPPER_SDIO_CMD52_RD(func, 0xf, &ret)!=2);  // cmd52 check status
    if (ret) {
        dev_err(&func->dev, "sdio read reg write address failed (%d)\n", ret);
        goto io_err;
    }

    ret = OS_WRAPPER_SDIO_CMD53_RD(func, data, 0x10020, 4);
    if (ret) {
        dev_err(&func->dev, "sdio read reg from I/O failed (%d)\n",ret);
        goto io_err;
    }

    *buf = (data[0]&0xff);
    *buf = *buf | ((data[1]&0xff)<<( 8 ));
    *buf = *buf | ((data[2]&0xff)<<( 16 ));
    *buf = *buf | ((data[3]&0xff)<<( 24 ));

io_err:
    OS_WRAPPER_SDIO_HOST_UNLOCK(func);

    return ret;
    
}


static INTN ssv6xxx_sdio_tuning_write_register(OS_WRAPPER_SDIO_FUNC *func, UINT32 addr, UINT32 buf)
{
    INTN ret = 0;
    CHAR8 data[8];

    OS_WRAPPER_SDIO_HOST_LOCK(func);

    // 4 bytes address
    data[0] = (addr >> ( 0 )) &0xff;
    data[1] = (addr >> ( 8 )) &0xff;
    data[2] = (addr >> ( 16 )) &0xff;
    data[3] = (addr >> ( 24 )) &0xff;

    // 4 bytes data
    data[4] = (buf >> ( 0 )) &0xff;
    data[5] = (buf >> ( 8 )) &0xff;
    data[6] = (buf >> ( 16 )) &0xff;
    data[7] = (buf >> ( 24 )) &0xff;

    //8 byte ( 4 bytes address , 4 bytes data )
    ret = OS_WRAPPER_SDIO_CMD53_WR(func, 0x10020, data, 8);

    OS_WRAPPER_SDIO_HOST_UNLOCK(func);

    return ret;

}

static INTN ssv6xxx_sdio_tuning_cmd_partter1_verify(OS_WRAPPER_SDIO_FUNC *func)
{
    INTN ret = 0;
    UINT8 retval = 0;

    ret = ssv6xxx_sdio_tuning_cmd52_write(func, 0x64, 0xaa);
    if (ret != 0) {
        OS_WRAPPER_LOG("%s(): fail to write by cmd52\n", __FUNCTION__);
        return ret;
    }

    ret = ssv6xxx_sdio_tuning_cmd52_read(func, 0x64, &retval);
    if (ret != 0) {
        OS_WRAPPER_LOG("%s(): fail to read by cmd52\n", __FUNCTION__);
        return ret;
    }

    if (retval != 0xaa) {
        OS_WRAPPER_LOG("Fail to sdio tuning, write [0xaa], read[0x%02x]\n", retval);
        return -1;
    }

    return 0;
}

static INTN ssv6xxx_sdio_tuning_cmd_partter2_verify(OS_WRAPPER_SDIO_FUNC *func)
{
    INTN ret = 0;
    UINT8 retval = 0;

    ret = ssv6xxx_sdio_tuning_cmd52_write(func, 0x64, 0x55);
    if (ret != 0) {
        OS_WRAPPER_LOG("%s(): fail to write by cmd52\n", __FUNCTION__);
        return ret;
    }

    ret = ssv6xxx_sdio_tuning_cmd52_read(func, 0x64, &retval);
    if (ret != 0) {
        OS_WRAPPER_LOG("%s(): fail to read by cmd52\n", __FUNCTION__);
        return ret;
    }

    if (retval != 0x55) {
        OS_WRAPPER_LOG("Fail to sdio tuning, write [0x55], read[0x%02x]\n", retval);
        return -1;
    }

    return 0;
}

static INTN ssv6xxx_sdio_tuning_cmd(OS_WRAPPER_SDIO_FUNC *func)
{
    INTN i = 0;
    INTN ret = 0;

    OS_WRAPPER_LOG( "===============================================\n");
    OS_WRAPPER_LOG( "==           SDIO CMD TUNING START           ==\n");
    OS_WRAPPER_LOG( "===============================================\n");


    for (i = 0; i < MAX_SDIO_DELAY_TUNING; i++) {
        ret = ssv6xxx_sdio_tuning_cmd_partter1_verify(func);
        if (ret != 0) 
            break; 
        
        ret = ssv6xxx_sdio_tuning_cmd_partter2_verify(func);
        if (ret != 0) 
            break;
    }

    OS_WRAPPER_LOG( "===============================================\n");
    OS_WRAPPER_LOG( "==          SDIO CMD TUNING RESULT           ==\n");
    OS_WRAPPER_LOG( "===============================================\n");

    OS_WRAPPER_LOG( "SDIO Cmd tuning result = %s\n", ((ret == 0) ? "Success" : "Fail"));
    
    return ret;
}

static INTN ssv6xxx_sdio_tuning_set_data_default_delay(OS_WRAPPER_SDIO_FUNC *func, INTN data)
{
    INTN ret = 0;
    UINT32 addr = 0;
    UINT8 val = 0;

    switch (data) {
        case 0:
            addr = 0x5c;
            break;
        case 1:
            addr = 0x5b;
            break;
        case 2:
            addr = 0x5a;
            break;
        case 3:
            addr = 0x59;
            break;
        default:
            OS_WRAPPER_LOG("%s(): Cannot tuning the data[%d] delay\n", __FUNCTION__, data);
            return -1;
    }
    
    val = ((1 << SDIO_INPUT_DELAY_SFT) | (1 << SDIO_OUTPUT_DELAY_SFT));
    ret = ssv6xxx_sdio_tuning_cmd52_write(func, addr, val);
    if (ret != 0) {
        OS_WRAPPER_LOG("%s(): fail to write by cmd52\n", __FUNCTION__);
    }

    return ret;
}

static INTN ssv6xxx_sdio_tuning_set_data_delay(OS_WRAPPER_SDIO_FUNC *func, INTN data, INTN input_delay, INTN output_delay)
{
    INTN ret = 0;
    UINT32 addr = 0;
    UINT8 val = 0;

    switch (data) {
        case 0:
            addr = 0x5c;
            break;
        case 1:
            addr = 0x5b;
            break;
        case 2:
            addr = 0x5a;
            break;
        case 3:
            addr = 0x59;
            break;
        default:
            OS_WRAPPER_LOG("%s(): Cannot tuning the data[%d] delay\n", __FUNCTION__, data);
            return -1;
    }

    if (input_delay == SDIO_DELAY_LEVEL_OFF)
        val |= (1 << SDIO_INPUT_DELAY_SFT);
    else
        val |= ((input_delay-1) << SDIO_INPUT_DELAY_LEVEL_SFT);


    if (output_delay == SDIO_DELAY_LEVEL_OFF)
        val |= (1 << SDIO_OUTPUT_DELAY_SFT);
    else
        val |= ((output_delay-1) << SDIO_OUTPUT_DELAY_LEVEL_SFT);
    
    ret = ssv6xxx_sdio_tuning_cmd52_write(func, addr, val);
    if (ret != 0) {
        OS_WRAPPER_LOG("%s(): fail to write by cmd52\n", __FUNCTION__);
    }

    if (sdio_tuning_more_info >= SDIO_TUNING_DBG) {
        ssv6xxx_sdio_tuning_cmd52_read(func, addr, &val);
        OS_WRAPPER_LOG("Set sdio delay=> addr=%02x, delay=%02x\n", addr, val);
    }

    return ret;
}

static INTN ssv6xxx_sdio_tuning_data_partter_verification(struct ssv6xxx_sdio_tuning_glue *glue)
{
    INTN i = 0;
    INTN ret = 0;

    for (i = 0; i < MAX_SDIO_TUNING_DATA_PARTTEN; i++) {
        if (glue->pattern[i] != glue->buff[i]) {
           ret = -1;
           break;
        }
    }

    return ret;
}

static void ssv6xxx_sdio_tuning_data_partter_show(struct ssv6xxx_sdio_tuning_glue *glue)
{
    INTN i = 0;
    
    for (i = 0; i < MAX_SDIO_TUNING_DATA_PARTTEN; i++) {
        OS_WRAPPER_LOG("data[%d]=%02x, buff[%d]=%02x\n", i, glue->pattern[i], i, glue->buff[i]);
    }
}

static INTN ssv6xxx_sdio_tuning_onebit_verification(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN i = 0;
    INTN ret = 0;
    
    for (i = 0; i < MAX_SDIO_DELAY_TUNING; i++) {
        /* write */
        ret = ssv6xxx_sdio_tuning_cmd53_write(glue, func);
        if (ret != 0) {
            ret = -1;
            break;
        }

        /* read */
        ret = ssv6xxx_sdio_tuning_cmd53_read(glue, func);
        if (ret != 0) {
            ret = -2;
            break;
        }

        ret = ssv6xxx_sdio_tuning_data_partter_verification(glue);    
        if (ret != 0) {
            ret = -3;
            if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                ssv6xxx_sdio_tuning_data_partter_show(glue);

            break;
        }
    }

    return ret;
}

static void ssv6xxx_sdio_tuning_onebit_mode_show(struct ssv6xxx_sdio_tuning_glue *glue)
{
    INTN i, j, len = 0;
    CHAR8 buf[32];

    OS_WRAPPER_LOG( "===============================================\n");
    OS_WRAPPER_LOG( "==         SDIO ONE BIT MODE RESULT          ==\n");
    OS_WRAPPER_LOG( "===============================================\n");

    OS_WRAPPER_LOG( "1bit mode test: total %d, success %d\n", glue->total_one_bit, glue->success_one_bit);

    //if (!sdio_tuning_more_info)
        //return;

    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        if (i == 0) {
            len += sprintf(buf+len, "\\I ");
            for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                len += sprintf(buf+len, "%d ", j);
            }
            len += sprintf(buf+len, "\n");
            OS_WRAPPER_LOG( "%s", buf);
            len = 0;
            len += sprintf(buf+len, "O\\\n\n");
            OS_WRAPPER_LOG( "%s", buf);
            len = 0;
            len += sprintf(buf+len, "%d  ", i);
        } else {
            len += sprintf(buf+len, "%d  ", i);
        }
        for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
            if (glue->one_bit_delay[i][j] == 1) {
                /*
                OS_WRAPPER_LOG( "input delay[%d], output delay[%d], test result[%s]\n", 
                    i, j, "success");
                */
                len += sprintf(buf+len, "O ");
            } else {
                /*
                OS_WRAPPER_LOG( "input delay[%d], output delay[%d], test result[%s] reason[%d]\n", 
                    i, j, "fail", glue->one_bit_delay[i][j]);
                */
                len += sprintf(buf+len, "X ");
            }
        }
        len += sprintf(buf+len, "\n");
        OS_WRAPPER_LOG( "%s", buf);
        len = 0;
    }

}

static INTN ssv6xxx_sdio_tuning_onebit_mode(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN i = 0, j = 0;
    INTN ret = 0;
    BOOL find = false;

    OS_WRAPPER_LOG( "===============================================\n");
    OS_WRAPPER_LOG( "==          SDIO ONE BIT MODE START          ==\n");
    OS_WRAPPER_LOG( "===============================================\n");
    
    /* set one bit mode*/
    OS_WRAPPER_SDIO_SET_BUS_WIDTH(func, OS_WRAPPER_SDIO_BUS_WIDTH_1);

    /* set data path to sram */
    ssv6xxx_sdio_tuning_cmd52_write(func, 0x0c, 0x2);

    /* set data0 to no delay */
    ssv6xxx_sdio_tuning_set_data_default_delay(func, SDIO_DATA0_DELAY);
    
    /* verify 1 bit mode */
    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
            
            if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                OS_WRAPPER_LOG("1bit mode test[0x%02x][0x%02x]\n", i, j);

            glue->total_one_bit++;
            /* Set data0 output delay */
            ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA0_DELAY, i, j);
           
            ret = ssv6xxx_sdio_tuning_onebit_verification(glue, func); 
            if (ret == 0) {
                glue->one_bit_delay[i][j] = 1;
                glue->success_one_bit++;
                find = true;
            } else {
                glue->one_bit_delay[i][j] = ret;
            }
            
            if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                OS_WRAPPER_LOG("\n");
        }
    }

    /* set data path to packet */
    ssv6xxx_sdio_tuning_cmd52_write(func, 0x0c, 0x0);

    ssv6xxx_sdio_tuning_onebit_mode_show(glue);

    return ((find) ? 0 : -1);
}

static INTN ssv6xxx_sdio_tuning_set_one_bit_mode_delay(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN ret = -1;
    INTN i, j;

    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
            if (glue->one_bit_delay[i][j] == 1) {
                /* Set data0 output delay */
                OS_WRAPPER_LOG("Set 1bit mode delay, input_delay[%d], output_delay[%d]\n", i, j);
                ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA0_DELAY, i, j);
                return 0;
            }
        }
    }

    return ret;
}

static INTN ssv6xxx_sdio_tuning_output_verification(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN i = 0;
    INTN ret = 0;
    
    for (i = 0; i < MAX_SDIO_DELAY_TUNING; i++) {
        /* read */
        ret = ssv6xxx_sdio_tuning_cmd53_read(glue, func);
        if (ret != 0) {
            ret = -2;
            break;
        }

        ret = ssv6xxx_sdio_tuning_data_partter_verification(glue);    
        if (ret != 0) {
            ret = -3;
            if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                ssv6xxx_sdio_tuning_data_partter_show(glue);
            
            break;
        }
    }

    return ret;
}

static void ssv6xxx_sdio_tuning_output_data_show(struct ssv6xxx_sdio_tuning_glue *glue)
{
    INTN i, j, k, h, len = 0;
    CHAR8 buf[256] ;

    OS_WRAPPER_LOG( "===============================================\n");
    OS_WRAPPER_LOG( "==         SDIO DATA OUTPUT RESULT           ==\n");
    OS_WRAPPER_LOG( "===============================================\n");

    OS_WRAPPER_LOG( "data output test: total %d, success %d\n", glue->total_data_out, glue->success_data_out);
    
    //if (!sdio_tuning_more_info)
        //return;

    len += sprintf(buf+len, "      ");
    for (j = 0 ; j < MAX_SDIO_DELAY_LEVEL ; j++) {
        len += sprintf(buf+len, "    #1 = %d    ", j);
    }
    OS_WRAPPER_LOG( "%s", buf);
    len = 0;

    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
            len += sprintf(buf+len, "      ");
            if (k == 0) {
                for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                    len += sprintf(buf+len, "\\#3 ");
                    for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                        len += sprintf(buf+len, "%d ", h);
                    }
                }
                OS_WRAPPER_LOG( "%s\n", buf);
                len = 0;
                len += sprintf(buf+len, "      ");
                for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                    len += sprintf(buf+len, "#2\\           ");
                }
                len += sprintf(buf+len, "\n\n");
                OS_WRAPPER_LOG( "%s", buf);
                len = 0;
                len += sprintf(buf+len, "#0=%d  ", i);
                len += sprintf(buf+len, "%d   ", k);
            } else {
                len += sprintf(buf+len, "%d   ", k);
            }
            for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {

                    if (glue->data_out_delay[i][j][k][h] == 1) {
                        /*
                        OS_WRAPPER_LOG( "output delay[%d][%d][%d][%d], test result[%s]\n", 
                            i, j, k, h, "success");
                        */
                        len += sprintf(buf+len, "O ");
                    } else {
                        /*
                        OS_WRAPPER_LOG( "output delay[%d][%d][%d][%d], test result[%s] reason[%d]\n", 
                            i, j, k, h, "fail", glue->data_out_delay[i][j][k][h]);
                        */
                        len += sprintf(buf+len, "X ");
                    }
                }
                len += sprintf(buf+len, "    ");
            }
            len += sprintf(buf+len, "\n");
            OS_WRAPPER_LOG( "%s", buf);
            len = 0;
        }
        len += sprintf(buf+len, "\n");
        OS_WRAPPER_LOG( "%s", buf);
        len = 0;
    }

    OS_WRAPPER_LOG("\nSummary table:\n");
    //Per ASIC team's request:
    {
        CHAR8 data_out_delay_summary[MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL];
        for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
            for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
                    for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                        data_out_delay_summary[k][h] = 0;
                    }
                }
            }
        }
        for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
            for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
                    for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                        if (glue->data_out_delay[i][j][k][h] == 1) {
                            data_out_delay_summary[k][h] += glue->data_out_delay[i][j][k][h];
                        }
                    }
                }
            }
        }

        for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
            if (i == 0) {
                len += sprintf(buf+len, "\\3 ");
                for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                    len += sprintf(buf+len, "%2d ", j);
                }
                len += sprintf(buf+len, "\n");
                OS_WRAPPER_LOG( "%s", buf);
                len = 0;
                len += sprintf(buf+len, "2\\\n\n");
                OS_WRAPPER_LOG( "%s", buf);
                len = 0;
                len += sprintf(buf+len, "%d  ", i);
            } else {
                len += sprintf(buf+len, "%d  ", i);
            }
            for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                len += sprintf(buf+len, "%2d ", data_out_delay_summary[i][j]);
            }
            len += sprintf(buf+len, "\n");
            OS_WRAPPER_LOG( "%s", buf);
            len = 0;
        }
    }
}

static INTN ssv6xxx_sdio_tuning_output_data(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN ret = 0;
    INTN i, j, k, h;
    BOOL find = false;
    
    OS_WRAPPER_LOG( "===============================================\n");
    OS_WRAPPER_LOG( "==       SDIO DATA OUTPUT TUNING START       ==\n");
    OS_WRAPPER_LOG( "===============================================\n");
    
    /* set one bit mode*/
    OS_WRAPPER_SDIO_SET_BUS_WIDTH(func, OS_WRAPPER_SDIO_BUS_WIDTH_1);
    ret = ssv6xxx_sdio_tuning_set_one_bit_mode_delay(glue, func);
    if (ret != 0) {
        OS_WRAPPER_LOG("1bit mode has problem, don't do output data test.");
        return ret;
    }

    /* set data path to sram */
    ssv6xxx_sdio_tuning_cmd52_write(func, 0x0c, 0x2);
   
    /* write parttern to sram*/
    ret = ssv6xxx_sdio_tuning_cmd53_write(glue, func);
    
    /* set four bit mode*/
    OS_WRAPPER_SDIO_SET_BUS_WIDTH(func, OS_WRAPPER_SDIO_BUS_WIDTH_4);

    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
            for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
                for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
   
                    if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                        OS_WRAPPER_LOG("data output test[0x%02x][0x%02x][0x%02x][0x%02x]\n", i, j, k, h);
                    
                    glue->total_data_out++;
                    //set D0 output delay
                    ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA0_DELAY, SDIO_DELAY_LEVEL_OFF, i);
                    //set D1 output delay
                    ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA1_DELAY, SDIO_DELAY_LEVEL_OFF, j);
                    //set D2 output delay
                    ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA2_DELAY, SDIO_DELAY_LEVEL_OFF, k);
                    //set D3 output delay
                    ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA3_DELAY, SDIO_DELAY_LEVEL_OFF, h);

                    ret = ssv6xxx_sdio_tuning_output_verification(glue, func);
                    if (ret == 0) {
                        glue->data_out_delay[i][j][k][h] = 1;
                        glue->success_data_out++;
                        find = true;
                    } else {
                        glue->data_out_delay[i][j][k][h] = ret;
                    }
                    
                    if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                        OS_WRAPPER_LOG("\n");

                }
            }
        }
    }

    /* set data path to packet */
    ssv6xxx_sdio_tuning_cmd52_write(func, 0x0c, 0x0);

    ssv6xxx_sdio_tuning_output_data_show(glue);

    return ((find == true) ? 0 : -1);
}

static INTN ssv6xxx_sdio_tuning_in_verification(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN i = 0;
    INTN ret = 0;
    
    for (i = 0; i < MAX_SDIO_DELAY_TUNING; i++) {
        /* write */
        ret = ssv6xxx_sdio_tuning_cmd53_write(glue, func);
        if (ret != 0) {
            ret = -1;
            break;
        }
        
        /* read */
        ret = ssv6xxx_sdio_tuning_cmd53_read(glue, func);
        if (ret != 0) {
            ret = -2;
            break;
        }

        ret = ssv6xxx_sdio_tuning_data_partter_verification(glue);    
        if (ret != 0) {
            ret = -3;
            if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                ssv6xxx_sdio_tuning_data_partter_show(glue);
            
            break;
        }
    }

    return ret;
}

static void ssv6xxx_sdio_tuning_input_data_show(struct ssv6xxx_sdio_tuning_glue *glue,
    INTN d0_output_delay, INTN d1_output_delay, INTN d2_output_delay, INTN d3_output_delay)
{
    INTN i, j, k, h, len = 0;
    CHAR8 buf[256];


    OS_WRAPPER_LOG( "===============================================\n");
    OS_WRAPPER_LOG( "==         SDIO DATA INPUT RESULT            ==\n");
    OS_WRAPPER_LOG( "==        OUTPUT DELAY[%d][%d][%d][%d]           ==\n", 
        d0_output_delay, d1_output_delay, d2_output_delay, d3_output_delay);
    OS_WRAPPER_LOG( "===============================================\n");

    OS_WRAPPER_LOG( "data input test: total %d, success %d\n", glue->total_data_in, glue->success_data_in);
    
    //if (!sdio_tuning_more_info)
        //return;

    len += sprintf(buf+len, "      ");
    for (j = 0 ; j < MAX_SDIO_DELAY_LEVEL ; j++) {
        len += sprintf(buf+len, "    #1 = %d    ", j);
    }
    OS_WRAPPER_LOG( "%s", buf);
    len = 0;

    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
            len += sprintf(buf+len, "      ");
            if (k == 0) {
                for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                    len += sprintf(buf+len, "\\#3 ");
                    for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                        len += sprintf(buf+len, "%d ", h);
                    }
                }
                OS_WRAPPER_LOG( "%s\n", buf);
                len = 0;
                len += sprintf(buf+len, "      ");
                for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                    len += sprintf(buf+len, "#2\\           ");
                }
                len += sprintf(buf+len, "\n\n");
                OS_WRAPPER_LOG( "%s", buf);
                len = 0;
                len += sprintf(buf+len, "#0=%d  ", i);
                len += sprintf(buf+len, "%d   ", k);
            } else {
                len += sprintf(buf+len, "%d   ", k);
            }
            for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {

                    if (glue->data_in_delay[i][j][k][h] == 1) {
                        /*
                        OS_WRAPPER_LOG( "output delay[%d][%d][%d][%d], test result[%s]\n", 
                            i, j, k, h, "success");
                        */
                        len += sprintf(buf+len, "O ");
                    } else {
                        /*
                        OS_WRAPPER_LOG( "output delay[%d][%d][%d][%d], test result[%s] reason[%d]\n", 
                            i, j, k, h, "fail", glue->data_out_delay[i][j][k][h]);
                        */
                        len += sprintf(buf+len, "X ");
                    }
                }
                len += sprintf(buf+len, "    ");
            }
            len += sprintf(buf+len, "\n");
            OS_WRAPPER_LOG( "%s", buf);
            len = 0;
        }
        len += sprintf(buf+len, "\n");
        OS_WRAPPER_LOG( "%s", buf);
        len = 0;
    }

    OS_WRAPPER_LOG("\nSummary table:\n");
    //Per ASIC team's request:
    {
        CHAR8 data_in_delay_summary[MAX_SDIO_DELAY_LEVEL][MAX_SDIO_DELAY_LEVEL];
        for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
            for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
                    for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                        data_in_delay_summary[k][h] = 0;
                    }
                }
            }
        }
        for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
            for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
                    for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                        if (glue->data_in_delay[i][j][k][h] == 1) {
                            data_in_delay_summary[k][h] += glue->data_in_delay[i][j][k][h];
                        }
                    }
                }
            }
        }

        for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
            if (i == 0) {
                len += sprintf(buf+len, "\\3 ");
                for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                    len += sprintf(buf+len, "%2d ", j);
                }
                len += sprintf(buf+len, "\n");
                OS_WRAPPER_LOG( "%s", buf);
                len = 0;
                len += sprintf(buf+len, "2\\\n\n");
                OS_WRAPPER_LOG( "%s", buf);
                len = 0;
                len += sprintf(buf+len, "%d  ", i);
            } else {
                len += sprintf(buf+len, "%d  ", i);
            }
            for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
                len += sprintf(buf+len, "%2d ", data_in_delay_summary[i][j]);
            }
            len += sprintf(buf+len, "\n");
            OS_WRAPPER_LOG( "%s", buf);
            len = 0;
        }
    }
}

static INTN _ssv6xxx_sdio_tuning_input_data(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func, 
    INTN d0_output_delay, INTN d1_output_delay, INTN d2_output_delay, INTN d3_output_delay)
{
    INTN ret = 0;
    INTN i, j, k, h;
    BOOL find = false;

    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
            for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
                for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                    
                    if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                        OS_WRAPPER_LOG("data input test[0x%02x][0x%02x][0x%02x][0x%02x]\n", i, j, k, h);
                    
                    glue->total_data_in++;
                    //set D0 input delay
                    ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA0_DELAY, i, d0_output_delay);
                    //set D1 input delay
                    ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA1_DELAY, j, d1_output_delay);
                    //set D2 input delay
                    ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA2_DELAY, k, d2_output_delay);
                    //set D3 input delay
                    ssv6xxx_sdio_tuning_set_data_delay(func, SDIO_DATA3_DELAY, h, d3_output_delay);

                    ret = ssv6xxx_sdio_tuning_in_verification(glue, func);
                    if (ret == 0) {
                        glue->data_in_delay[i][j][k][h] = 1;
                        glue->success_data_in++;
                        find = true;
                    } else {
                        glue->data_in_delay[i][j][k][h] = ret;
                    }
                    
                    if (sdio_tuning_more_info >= SDIO_TUNING_DBG)
                        OS_WRAPPER_LOG("\n");

                }
            }
        }
    }
    
    ssv6xxx_sdio_tuning_input_data_show(glue, d0_output_delay, d1_output_delay, d2_output_delay, d3_output_delay);

    return ((find == true) ? 0 : -1);
}

static void ssv6xxx_sdio_tuning_input_data_init(struct ssv6xxx_sdio_tuning_glue *glue)
{
    INTN i, j, k, h;
    
    glue->total_data_in = 0;
    glue->success_data_in = 0;

    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
            for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
                for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                    glue->data_in_delay[i][j][k][h] = 0;
                }
            }
        }
    }
}

static INTN ssv6xxx_sdio_tuning_input_data(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN i, j, k, h;
    
    OS_WRAPPER_LOG( "==============================================\n");
    OS_WRAPPER_LOG( "==       SDIO DATA INPUT TUNING START       ==\n");
    OS_WRAPPER_LOG( "==============================================\n");

    /* set data path to sram */
    ssv6xxx_sdio_tuning_cmd52_write(func, 0x0c, 0x2);
   
    /* set four bit mode*/
    OS_WRAPPER_SDIO_SET_BUS_WIDTH(func, OS_WRAPPER_SDIO_BUS_WIDTH_4);
   
    // tunning data input from data output table
    for (i = 0; i < MAX_SDIO_DELAY_LEVEL; i++) {
        for (j = 0; j < MAX_SDIO_DELAY_LEVEL; j++) {
            for (k = 0; k < MAX_SDIO_DELAY_LEVEL; k++) {
                for (h = 0; h < MAX_SDIO_DELAY_LEVEL; h++) {
                    
                    if (glue->data_out_delay[i][j][k][h] == 1) {
                        ssv6xxx_sdio_tuning_input_data_init(glue);
                       _ssv6xxx_sdio_tuning_input_data(glue, func, i, j, k, h);
                    }
                }
            }
        }
    }
 
    /* set data path to packet */
    ssv6xxx_sdio_tuning_cmd52_write(func, 0x0c, 0x0);

    return 0;
}

static void ssv6xxx_sdio_tuning_set_bus(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    UINT32 regval = 0;

    /* set one bit mode*/
    OS_WRAPPER_SDIO_SET_BUS_WIDTH(func, OS_WRAPPER_SDIO_BUS_WIDTH_1);
    ssv6xxx_sdio_tuning_set_one_bit_mode_delay(glue, func);

    /* Set Bus to 120M */
    ssv6xxx_sdio_tuning_write_register(func, 0xccb0b004, 0xa51a8800);
    ssv6xxx_sdio_tuning_read_register(func, 0xccb0b004, &regval);
    
    ssv6xxx_sdio_tuning_write_register(func, 0xc0000054, 0x481100cc);
    ssv6xxx_sdio_tuning_read_register(func, 0xc0000054, &regval);
}

static INTN ssv6xxx_sdio_tuning_data(struct ssv6xxx_sdio_tuning_glue *glue, OS_WRAPPER_SDIO_FUNC *func)
{
    INTN ret = 0;

    OS_WRAPPER_LOG( "===============================================\n");
    OS_WRAPPER_LOG( "==          SDIO DATA TUNING START           ==\n");
    OS_WRAPPER_LOG( "===============================================\n");

    ret = ssv6xxx_sdio_tuning_onebit_mode(glue, func);
    if (ret) {
        OS_WRAPPER_LOG("Fail to sdio tuning one bit mode\n");
        return ret;
    }
    OS_WRAPPER_LOG("\n\n\n");
   
    ssv6xxx_sdio_tuning_set_bus(glue, func);
    
    ret = ssv6xxx_sdio_tuning_output_data(glue, func);
    if (ret) {
        OS_WRAPPER_LOG("Fail to sdio tuning output data\n");
        return ret;
    }
    OS_WRAPPER_LOG("\n\n\n");
    
    ret = ssv6xxx_sdio_tuning_input_data(glue, func);
    if (ret) {
        OS_WRAPPER_LOG("Fail to sdio tuning input data\n");
        return ret;
    }
    OS_WRAPPER_LOG("\n\n\n");

    return ret;
}

void init_sdio_tuning(struct ssv6xxx_sdio_tuning_glue *glue)
{
    INTN i = 0;
    INTN index = 0;
    CHAR8 pattern[] = {0xff, 0x0f, 0xff, 0x00, 0xff, 0xcc, 0xc3, 0xcc,
                      0xc3, 0x3c, 0xcc, 0xff, 0xfe, 0xff, 0xfe, 0xef,
                      0xff, 0xdf, 0xff, 0xdd, 0xff, 0xfb, 0xff, 0xfb, 
                      0xbf, 0xff, 0x7f, 0xff, 0x77, 0xf7, 0xbd, 0xef,
                      0xff, 0xf0, 0xff, 0xf0, 0x0f, 0xfc, 0xcc, 0x3c,
                      0xcc, 0x33, 0xcc, 0xcf, 0xff, 0xef, 0xff, 0xee,
                      0xff, 0xfd, 0xff, 0xfd, 0xdf, 0xff, 0xbf, 0xff, 
                      0xbb, 0xff, 0xf7, 0xff, 0xf7, 0x7f, 0x7b, 0xde};
    
    for (i = 0; i < MAX_SDIO_TUNING_DATA_PARTTEN; i++) {
        index = i % (sizeof(pattern)/sizeof(CHAR8));
        glue->pattern[i] = pattern[index];
    }
}

INTN ssv6xxx_sdio_tuning_start(OS_WRAPPER_SDIO_FUNC *func, const OS_WRAPPER_SDIO_DEVICE_ID *id)
{
    INTN ret = 0;
    struct ssv6xxx_sdio_tuning_glue     *glue;

    OS_WRAPPER_LOG( "=======================================\n");
    OS_WRAPPER_LOG( "==       RUN SDIO DELAY TUNING       ==\n");
    OS_WRAPPER_LOG( "=======================================\n");
    
    /* We are only able to handle the wlan function */
    if (func->num != 0x01)
        return -ENODEV;

    glue = OS_WRAPPER_MEM_ALLOC(sizeof(struct ssv6xxx_sdio_tuning_glue), GFP_KERNEL);
    if (!glue) {
        OS_WRAPPER_LOG("Cannot alloc glue\n");
        return -ENOMEM;
    }
  
    /* init sdio tuning */
    init_sdio_tuning(glue);
    /* set sdio to high clk */
    OS_WRAPPER_SDIO_SET_CLK(func, HIGH_SPEED_SDIO_CLOCK);
    //OS_WRAPPER_SDIO_SET_CLK(func, LOW_SPEED_SDIO_CLOCK);
    OS_WRAPPER_SDIO_SET_PRIV_DRV_DATA(func, glue);

    OS_WRAPPER_LOG("\n\n\n");
    ret = ssv6xxx_sdio_tuning_cmd(func);
    if (ret < 0) 
        goto out;

    OS_WRAPPER_LOG("\n\n\n");
    ret = ssv6xxx_sdio_tuning_data(glue, func);

out:
    return 0;

}

void ssv6xxx_sdio_tuning_cleanup(OS_WRAPPER_SDIO_FUNC *func)
{
    struct ssv6xxx_sdio_tuning_glue *glue = sdio_get_drvdata(func);

    OS_WRAPPER_LOG("%s..........\n", __func__);
    if (glue)
        OS_WRAPPER_MEM_FREE(glue);
}

