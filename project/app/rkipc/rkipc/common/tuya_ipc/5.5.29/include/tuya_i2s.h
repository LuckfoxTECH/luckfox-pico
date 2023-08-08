/**
 * @file tuya_i2s.h
 * @brief Common process - i2s
 * @version 0.1
 * @date 2018-09-01
 *
 * @copyright Copyright 2018-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_I2S_H__
#define __TUYA_I2S_H__

#include "tuya_driver.h"

#define TUYA_I2S_CFG(__I2S, __MODE, __FREQ, __DATABIT, __CHANNEL, __STANDARD)                      \
	(__I2S)->cfg.mode = __MODE;                                                                    \
	(__I2S)->cfg.freq = __FREQ;                                                                    \
	(__I2S)->cfg.cahnnel = __DATABIT;                                                              \
	(__I2S)->cfg.standard = __CHANNEL;                                                             \
	(__I2S)->cfg.databits = __STANDARD

typedef enum {
	TUYA_I2S0 = 0,
	TUYA_I2S1,
} tuya_i2s_port_t;

typedef enum {
	TUYA_I2S_FREQ_8K = 8000,
	TUYA_I2S_FREQ_16K = 16000,
	TUYA_I2S_FREQ_22K = 22050,
	TUYA_I2S_FREQ_32K = 32000,
	TUYA_I2S_FREQ_44K = 44100,
	TUYA_I2S_FREQ_48K = 48000,
} tuya_i2s_freq_t;

typedef enum {
	TUYA_I2S_DATA_BIT8 = 8,
	TUYA_I2S_DATA_BIT16 = 16,
	TUYA_I2S_DATA_BIT24 = 24,
	TUYA_I2S_DATA_BIT32 = 32,
} tuya_i2c_databit_t;

typedef enum {
	TUYA_I2S_CHANNEL_MONO = 1,
	TUYA_I2S_CHANNEL_STEREO = 2,
} tuya_i2s_channel_t;

typedef enum {
	TUYA_I2S_STANDARD_PHILIPS = 1,
	TUYA_I2S_STANDARD_MSB,
	TUYA_I2S_STANDARD_LSB,
} tuya_i2s_standard_t;

typedef enum {
	TUYA_I2S_MODE_TX = 1,
	TUYA_I2S_MODE_RX,
	TUYA_I2S_MODE_TXRX,
} tuya_i2s_mode_t;

typedef enum {
	TUYA_I2S_START_CMD = 0x20,
	TUYA_I2S_STOP_CMD,
} tuya_i2s_cmd_t;

typedef struct {
	tuya_i2s_mode_t mode;
	tuya_i2s_freq_t freq;
	tuya_i2s_channel_t cahnnel;
	tuya_i2s_standard_t standard;
	tuya_i2c_databit_t databits;
} tuya_i2s_cfg_t;

typedef struct tuya_i2s tuya_i2s_t;

typedef struct {
	int (*init)(tuya_i2s_t *i2s, tuya_i2s_cfg_t *cfg);
	int (*write)(tuya_i2s_t *i2s, void *data, uint32_t length);
	int (*read)(tuya_i2s_t *i2s, void *data, uint32_t length);
	int (*control)(tuya_i2s_t *i2s, uint32_t cmd, void *arg);
	int (*deinit)(tuya_i2s_t *i2s);
} tuya_i2s_ops_t;

struct tuya_i2s {
	tuya_drv_node_t node;
	tuya_i2s_cfg_t cfg;
	tuya_i2s_ops_t *ops;
};

/**
 * @brief i2s init
 *
 * @param[in] i2s refer to tuya_i2s_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2s_init(tuya_i2s_t *i2s);

/**
 * @brief i2s start
 *
 * @param[in] i2s refer to tuya_i2s_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2s_start(tuya_i2s_t *i2s);

/**
 * @brief i2s stop
 *
 * @param[in] i2s refer to tuya_i2s_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2s_stop(tuya_i2s_t *i2s);

/**
 * @brief i2s write
 *
 * @param[in] i2s refer to tuya_i2s_t
 * @param[in] data buffer
 * @param[in] length buffer length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2s_write(tuya_i2s_t *i2s, void *data, uint32_t length);

/**
 * @brief i2s read
 *
 * @param[in] i2s refer to tuya_i2s_t
 * @param[out] data buffer
 * @param[out] length buffer length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2s_read(tuya_i2s_t *i2s, void *data, uint32_t length);

/**
 * @brief i2s control
 *
 * @param[in] i2s refer to tuya_i2s_t
 * @param[in] cmd control command
 * @param[in] arg command argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2s_control(tuya_i2s_t *i2s, UINT8_T cmd, void *arg);

/**
 * @brief i2s deinit
 *
 * @param[in] i2s refer to tuya_i2s_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2s_deinit(tuya_i2s_t *i2s);

#endif
