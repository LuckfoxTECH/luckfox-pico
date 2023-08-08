/**
 * @file tuya_driver.h
 * @brief Common process - driver
 * @version 0.1
 * @date 2018-09-01
 *
 * @copyright Copyright 2018-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_DRV_H__
#define __TUYA_DRV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "tuya_cloud_types.h"
#include "tuya_hal_mutex.h"
#include "tuya_hal_semaphore.h"
#include "uni_log.h"

#define TUYA_DRV_ASSRET(__EXPRESS)                                                                 \
	if (!(__EXPRESS)) {                                                                            \
		PR_ERR("TUYA DRV ASSERT " #__EXPRESS "");                                                  \
		return OPRT_INVALID_PARM;                                                                  \
	}

typedef enum {
	TUYA_DRV_INT_RX_FLAG = 0x01,
	TUYA_DRV_INT_TX_FLAG = 0x02,
	TUYA_DRV_DMA_RX_FLAG = 0x04,
	TUYA_DRV_DMA_TX_FLAG = 0x08,
	TUYA_DRV_NONBLOCK_FLAG = 0x10,
} tuya_drv_flag_t;

typedef enum {
	TUYA_DRV_RESUME_CMD = 0x01,
	TUYA_DRV_SUSPEND_CMD,
	TUYA_DRV_SET_INT_CMD,
	TUYA_DRV_CLR_INT_CMD,
	TUYA_DRV_SET_ISR_CMD,
	TUYA_DRV_CONFIG_CMD,
	TUYA_DRV_ISR_MODE_CMD,
	TUYA_DRV_CUSTOM_CMD,
} tuya_drv_cmd_t;

typedef enum {
	TUYA_DRV_PIN = 0x01,
	TUYA_DRV_UART,
	TUYA_DRV_PWM,
	TUYA_DRV_TIMER,
	TUYA_DRV_ADC,
	TUYA_DRV_I2C,
	TUYA_DRV_RTC,
	TUYA_DRV_CUSTOM,
} tuya_drv_type_t;

typedef enum {
	TUYA_DRV_TX_EVENT = 0x01,
	TUYA_DRV_RX_EVENT,
	TUYA_DRV_DMARX_EVENT,
	TUYA_DRV_DMATX_EVENT
} tuya_drv_event_t;

typedef struct slist_head_driver {
	struct slist_head *next;
} SLIST_HEAD_DRIVER;

typedef struct {
	SLIST_HEAD_DRIVER list;
	uint8_t port;
	uint8_t type;
} tuya_drv_node_t;

/**
 * @brief driver find
 *
 * @param[in] type node type
 * @param[in] port node port
 *
 */
void *tuya_driver_find(uint8_t type, uint8_t port);

/**
 * @brief driver register
 *
 * @param[in] node refer to tuya_drv_node_t
 * @param[in] type node type
 * @param[in] port node port
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_driver_register(tuya_drv_node_t *node, uint8_t type, uint8_t port);

#ifdef __cplusplus
}
#endif

#endif
