/*
 * Copyright (c) 2022 rockchip
 *
 */
#include <stdio.h>
#include "rk_adc.h"

#if defined(RV1126_RV1109)
#define __RK_ADC_DEVNUM_NAME__   "ff5e0000.saradc"
#elif defined(RV1106_RV1103)
#define __RK_ADC_DEVNUM_NAME__   "ff3c0000.saradc"
#elif defined(RK3588)
#define __RK_ADC_DEVNUM_NAME__   "fec10000.saradc"
#else
#error	"Please check the platform for adc device name"
#endif

int main(int argc, char *argv[])
{
	int devnum = rk_adc_get_devnum(__RK_ADC_DEVNUM_NAME__);
    printf("rk_adc_get_devnum %d\n", devnum);
    printf("rk_adc_get_value %d, %d, %d, %d, %d\n",
        rk_adc_get_value(devnum, 0),
        rk_adc_get_value(devnum, 1),
        rk_adc_get_value(devnum, 2),
        rk_adc_get_value(devnum, 3),
        rk_adc_get_value(devnum, 4));
    return 0;
}

