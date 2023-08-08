/**
 * @file tuya_i2c.h
 * @brief Common process - i2c
 * @version 0.1
 * @date 2018-09-01
 *
 * @copyright Copyright 2018-2021 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TUYA_I2C_H__
#define __TUYA_I2C_H__

#include "tuya_driver.h"

#define TUYA_I2C_WR 0x0000
#define TUYA_I2C_RD (1 << 0)
#define TUYA_I2C_ADDR_10BIT (1 << 2) // this is a ten bit chip address
#define TUYA_I2C_NO_START (1 << 4)
#define TUYA_I2C_IGNORE_NACK (1 << 5)
#define TUYA_I2C_NO_READ_ACK (1 << 6)  // when I2C reading, we do not ACK
#define TUYA_I2C_NO_WRITE_ACK (1 << 7) // when I2C writing, we do not ACK

#define TUYA_I2C_MASTER_CFG(__I2C, __ADDR)                                                         \
	(__I2C)->cfg.dev_addr = __ADDR;                                                                \
	(__I2C)->cfg.mode = TUYA_I2C_MODE_MASTER

#define TUYA_I2C_SW_CFG_INIT(__CFG, __SCL, __SDA, __DELAY)                                         \
	(__CFG)->scl_pin = __SCL;                                                                      \
	(__CFG)->sda_pin = __SDA;                                                                      \
	(__CFG)->delay_count = __DELAY;                                                                \
	(__CFG)->timeout = 100

typedef enum {
	TUYA_I2C0 = 0,
	TUYA_I2C1,
	TUYA_I2C2,
	TUYA_I2C3,
} tuya_i2c_port_t;

typedef enum {
	TUYA_I2C_MODE_MASTER = 0,
	TUYA_I2C_MODE_SLAVE,
} tuya_i2c_mode_t;

typedef enum {
	TUYA_I2C_TYPE_SW = 0,
	TUYA_I2C_TYPE_HD,
} tuya_i2c_type_t;

typedef struct {
	uint8_t sda_pin;
	uint8_t scl_pin;
	uint32_t delay_count;
	uint32_t timeout;
} tuya_i2c_sw_cfg_t;

typedef struct {
	uint8_t mode;
	uint16_t dev_addr;
} tuya_i2c_cfg_t;

typedef struct {
	uint16_t addr;
	uint16_t flags;
	uint16_t len;
	uint8_t *buf;
} tuya_i2c_msg_t;

typedef struct tuya_i2c tuya_i2c_t;

typedef struct {
	int (*init)(tuya_i2c_t *i2c, tuya_i2c_cfg_t *cfg);
	int (*xfer)(tuya_i2c_t *i2c, tuya_i2c_msg_t *msg, uint8_t num);
	int (*control)(tuya_i2c_t *i2c, uint8_t cmd, void *arg);
	int (*deinit)(tuya_i2c_t *i2c);
} tuya_i2c_ops_t;

struct tuya_i2c {
	tuya_drv_node_t node;
	tuya_i2c_cfg_t cfg;
	tuya_i2c_ops_t *ops;
};

/**
 * @brief i2c init
 *
 * @param[in] i2c refer to tuya_i2c_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2c_init(tuya_i2c_t *i2c);

/**
 * @brief i2c transfer
 *
 * @param[in] i2c refer to tuya_i2c_t
 * @param[in] msg refer to tuya_i2c_msg_t
 * @param[in] num i2c number
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2c_xfer(tuya_i2c_t *i2c, tuya_i2c_msg_t *msg, uint8_t num);

/**
 * @brief i2c control
 *
 * @param[in] i2c refer to tuya_i2c_t
 * @param[in] cmd control command
 * @param[in] arg control argument
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2c_control(tuya_i2c_t *i2c, uint8_t cmd, void *arg);

/**
 * @brief i2c deinit
 *
 * @param[in] i2c refer to tuya_i2c_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2c_deinit(tuya_i2c_t *i2c);

/**
 * @brief i2c master send
 *
 * @param[in] i2c refer to tuya_i2c_t
 * @param[in] addr message address
 * @param[in] flags read or write
 * @param[in] buf send buf
 * @param[in] count send length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2c_master_send(tuya_i2c_t *i2c, uint16_t addr, uint16_t flags, void *buf, uint16_t count);

/**
 * @brief i2c master receive
 *
 * @param[in] i2c refer to tuya_i2c_t
 * @param[in] addr message address
 * @param[in] flags read or write
 * @param[in] buf receive buf
 * @param[in] count receive length
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_i2c_master_recv(tuya_i2c_t *i2c, uint16_t addr, uint16_t flags, void *buf, uint16_t count);

/**
 * @brief i2c register
 *
 * @param[in] port refer to tuya_i2c_port_t
 * @param[in] cfg refer to tuya_i2c_sw_cfg_t
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
int tuya_sw_i2c_register(tuya_i2c_port_t port, tuya_i2c_sw_cfg_t *cfg);

#endif
