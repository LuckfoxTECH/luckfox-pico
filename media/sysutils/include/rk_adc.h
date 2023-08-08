/*
 * Copyright (c) 2022 rockchip
 *
 */
#ifndef __RK_ADC_H__
#define __RK_ADC_H__

#include <stdint.h>

int rk_adc_get_devnum(const char *name);
int rk_adc_get_value(uint32_t dev_num, uint32_t chn_num);

#endif //__RK_ADC_H__