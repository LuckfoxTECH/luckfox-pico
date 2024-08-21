/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-2-1       Tony.xie     first implementation
 *
 */

#ifndef _DRV_DVFS_H_
#define _DRV_DVFS_H_

#ifdef RT_USING_PM_DVFS
#define RK_PM_USING_DVFS
#endif

#ifdef RT_USING_PM_REQ_PWR
#define RK_PM_USING_REQ_PWR
#endif

#ifdef RT_USING_PM_REQ_CLK
#define RK_PM_USING_REQ_CLK
#endif

#ifdef RT_USING_DVFS_DBG
#define DVFS_INFO(fmt, ...) rt_kprintf(fmt, ##__VA_ARGS__)
#else
#define DVFS_INFO(fmt, ...)
#endif

#ifdef RK_PM_USING_REQ_PWR
#ifndef HAL_PWR_MODULE_ENABLED
#error "The RK_PM_USING_REQ_PWR is enable, HAL_PWR_MODULE_ENABLED must be enable also!"
#endif
#endif

#ifdef RK_PM_USING_REQ_CLK
#ifndef HAL_CRU_MODULE_ENABLED
#error "The RK_PM_USING_REQ_CLK is enable, HAL_CRU_MODULE_ENABLED must be enable also!"
#endif
#endif

#ifdef RK_PM_USING_DVFS
#ifndef HAL_CRU_MODULE_ENABLED
#error "The RK_PM_USING_DVFS is enable, HAL_CRU_MODULE_ENABLED must be enable also!"
#endif
#endif

#define REQ_ID_MSK  (0xFF)
#define REQ_ID_INV  (0xFF)
#define REQ_ID_DIS  (0xFE)

#define DVFS_REQ_ID_PWR_SFT (8)
#define DVFS_REQ_ID_CLK_SFT (0)
#define DVFS_REQ_CLK_SET_ID(id) ((id) << DVFS_REQ_ID_CLK_SFT)
#define DVFS_REQ_PWR_SET_ID(id) ((id) << DVFS_REQ_ID_PWR_SFT)

#define DVFS_REQ_PWR_ID_MSK DVFS_REQ_PWR_SET_ID(REQ_ID_MSK)
#define DVFS_REQ_CLK_ID_MSK DVFS_REQ_CLK_SET_ID(REQ_ID_MSK)

#define DVFS_REQ_PWR_ID(id)  (( (id) >> DVFS_REQ_ID_PWR_SFT) & REQ_ID_MSK)
#define DVFS_REQ_CLK_ID(id)  ((id) & REQ_ID_MSK)

/* not support */
#define DVFS_ID_DIS (DVFS_REQ_CLK_SET_ID(REQ_ID_DIS) | DVFS_REQ_PWR_SET_ID(REQ_ID_DIS))

#define DVFS_TBL_IDX_INV (0xFF)

//#define REQ_VAL_OWNE_EN

struct dvfs_table
{
    uint32_t freq;
    uint32_t volt;
};

struct req_val_ctrl
{
    struct
    {
        uint8_t ttl_req: 3;
    } info;
    uint8_t max_val_id;
    uint8_t req_bits;
    uint32_t *req_vals;
#ifdef REQ_VAL_OWNE_EN
    uint32_t *owner_id;
#endif
};

struct req_pwr_desc
{
    uint8_t pwr_id;
    struct regulator_desc *desc;
    struct req_val_ctrl req_ctrl;
};

struct req_clk_desc
{
    eCLOCK_Name clk_id;
    struct req_val_ctrl req_ctrl;
};

struct rk_dvfs_desc
{
#ifdef HAL_CRU_MODULE_ENABLED
    eCLOCK_Name clk_id;
#endif
    uint8_t pwr_id;
    struct regulator_desc *pwr_desc;
    uint8_t tbl_idx;
    uint8_t tbl_cnt;
    const struct dvfs_table *table;

#ifdef RK_PM_USING_REQ_CLK
    struct req_clk_desc *req_clk;
#endif
#ifdef RK_PM_USING_REQ_PWR
    uint8_t pwr_req_id;
    struct req_pwr_desc *req_pwr;
#endif
};

void regulator_req_init(void);
void regulator_req_desc_init(struct req_pwr_desc *desc_arr);
struct req_pwr_desc  *regulator_get_req_volt_id(ePWR_ID pwrid, uint8_t *req_id);
rt_err_t regulator_req_set_voltage(struct req_pwr_desc *req_pwr, uint8_t req_id,
                                   uint32_t volt);
uint32_t regulator_req_get_voltage(struct req_pwr_desc *req_pwr);
uint32_t regulator_req_get_max_voltage(struct req_pwr_desc *req_pwr);
uint32_t regulator_req_get_set_voltage(struct req_pwr_desc *req_pwr, uint8_t req_id);
rt_err_t regulator_req_voltage_release(struct req_pwr_desc *req_pwr, uint8_t req_id);
rt_err_t regulator_req_release(struct req_pwr_desc *req_pwr, uint8_t req_id);

void clk_req_desc_init(struct req_clk_desc *desc_array);
void clk_req_init(void);

struct req_clk_desc  *clk_get_req_rate_id(eCLOCK_Name clk_id, uint8_t *req_id);
rt_err_t clk_req_set_rate(struct req_clk_desc *req_clk, uint8_t req_id, uint32_t rate);
uint32_t clk_req_get_rate(struct req_clk_desc *req_clk);
uint32_t clk_req_get_max_rate(struct req_clk_desc *req_clk);
uint32_t clk_req_get_set_rate(struct req_clk_desc *req_clk, uint8_t req_id);
rt_err_t clk_req_rate_release(struct req_clk_desc *req_clk, uint8_t req_id);
rt_err_t clk_req_release(struct req_clk_desc *req_clk, uint8_t req_id);

rt_err_t dvfs_set_rate(struct rk_dvfs_desc *dvfs_desc, uint8_t dvfs_clk_req_id, uint32_t rate);
rt_err_t dvfs_set_rate_by_idx(struct rk_dvfs_desc *dvfs_desc,
                              uint8_t tbl_idx, uint8_t dvfs_clk_req_id);
struct rk_dvfs_desc *dvfs_get_by_clk(eCLOCK_Name clk_id, uint8_t  *dvfs_clk_req_id);
uint32_t dvfs_req_get_rate(struct rk_dvfs_desc *dvfs_desc);
uint32_t dvfs_req_get_max_rate(struct rk_dvfs_desc *dvfs_desc);
uint32_t dvfs_req_get_set_rate(struct rk_dvfs_desc *dvfs_desc, uint8_t dvfs_clk_req_id);
void rk_dvfs_req_rate_release(struct rk_dvfs_desc *dvfs_desc,
                              uint8_t dvfs_clk_req_id);
void rk_dvfs_req_release(struct rk_dvfs_desc *dvfs_desc,  uint8_t dvfs_clk_req_id);
void dvfs_desc_init(struct rk_dvfs_desc *dvfs_array);
void dvfs_init(void);

#ifdef RT_USING_COMMON_TEST_PM
struct rk_dvfs_desc *dvfs_get_desc(void);
uint32_t dvfs_get_desc_cnt(void);
#endif

#endif
