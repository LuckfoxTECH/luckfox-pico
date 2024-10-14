/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2019-07-31     Xing Zheng        first version
 * 2022-12-07     Jun Zeng          second version
 */

#ifndef __DRV_CODECS_H__
#define __DRV_CODECS_H__

#ifdef RT_USING_CODEC_AW8896
extern const struct codec_desc codec_aw8896;
#endif

#ifdef RT_USING_CODEC_ES7243
extern const struct codec_desc codec_es7243;
#endif

#ifdef RT_USING_CODEC_ES8156
extern const struct codec_desc codec_es8156;
#endif

#ifdef RT_USING_CODEC_ES8311
extern const struct codec_desc codec_es8311;
#endif

#ifdef RT_USING_CODEC_ES8388
extern const struct codec_desc codec_es8388;
#endif

#ifdef RT_USING_CODEC_TDA7803
#define MAX_DEVICE_NUM                     4
extern const struct codec_desc codec_tda7803;
#endif

/**
  * @brief  Codec Interface Type
  */
typedef enum
{
    IF_TYPE_I2C = 0,
    IF_TYPE_SPI,
} eInterfaceType;

/**
  * @brief  Codec Device information definition
  */
struct codec_desc
{
    eInterfaceType if_type;
    char name[RT_NAME_MAX];
    char i2c_bus[RT_NAME_MAX];
#ifdef RT_USING_CODEC_TDA7803
    uint8_t i2c_addr[MAX_DEVICE_NUM];
    uint8_t device_num;
#else
    uint8_t i2c_addr;
#endif
};

#endif /* __DRV_CODECS_H__ */
