#ifndef __AIC_BSP_EXPORT_H
#define __AIC_BSP_EXPORT_H

#define AIC_BLUETOOTH        0
#define AIC_WIFI             1
#define AIC_PWR_OFF          0
#define AIC_PWR_ON           1

struct aicbsp_feature_t {
	bool band_5g_support;
	uint32_t sdio_clock;
	uint8_t  sdio_phase;
	uint8_t irqf;
};

int aicbsp_set_subsys(int, int);
int aicbsp_get_feature(struct aicbsp_feature_t *feature);

#endif
