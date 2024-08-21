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

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup DVFS
 *  @{
 */

/** @defgroup DVFS_How_To_Use How To Use
 *  @{

The DVFS driver is used to set rate and volt of a clk:

- **To set a clk rate and a volt of a clk**:
    - Get a rk_dvfs_desc pointer and a dvfs_clk_req_id by dvfs_get_by_clk.
    - Set a rate value by dvfs_set_rate.
    - Or Set a dvfs table idex(indicate a rate) by dvfs_set_rate_by_idx.

- **Several devices may request setting different rates for a clk, must select the max rate**:
    - Get a req_clk_desc pointer and a req_id by clk_get_req_rate_id.
    - Request setting a rate value by clk_req_set_rate.

- **Several devices may request setting different volt for a regulator, must select the max rate**:
    - Get a req_regulator_desc pointer and a req_id by regulator_get_req_volt_id.
    - Request setting a volt value by regulator_req_set_voltage.

 @} */

#include <rthw.h>
#include <rtthread.h>
#include <drivers/pm.h>
#include "hal_base.h"
#ifdef HAL_CRU_MODULE_ENABLED
#include "drv_clock.h"
#endif
#include "drv_regulator.h"
#include "drv_dvfs.h"

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif

struct rk_req_clk_volt_ctrl
{
#ifdef RK_PM_USING_REQ_PWR
    struct req_pwr_desc *pwr_array;
#endif

#ifdef RK_PM_USING_REQ_CLK
    struct req_clk_desc  *clk_array;
#endif

#ifdef RK_PM_USING_DVFS
    uint32_t dvfs_cnt;
    struct rk_dvfs_desc  *dvfs_array;
#endif
};

#if defined(RK_PM_USING_REQ_PWR) || defined(RK_PM_USING_REQ_CLK) || \
    defined(RK_PM_USING_DVFS)

static struct rk_req_clk_volt_ctrl pm_req;

static rt_base_t dvfs_interrupt_disable(void)
{
    return rt_hw_interrupt_disable();
}

static void dvfs_interrupt_enable(rt_base_t level)
{
    return rt_hw_interrupt_enable(level);
}
#endif

#if defined(RT_USING_PM_REQ_PWR) || defined(RK_PM_USING_REQ_CLK)
static uint8_t req_val_get_max_val(uint32_t *array, uint8_t ttl_req, uint8_t req_bits)
{
    int i;
    uint32_t max_val = 0;
    uint8_t max_idx = REQ_ID_INV;

    RT_ASSERT(array);
    for (i = 0; i < ttl_req; i++)
    {
        if (max_val < array[i])
        {
            DVFS_INFO("* %s: idx-%d; max-%d, cmp-%d, req_bits-0x%x\n",
                      __func__, i, max_val, array[i], req_bits);
            if (req_bits & BIT(i))
            {
                max_idx = i;
                max_val = array[i];
            }
        }
    }

    return max_idx;
}

static uint32_t req_val_updata_val(struct req_val_ctrl *req_ctrl, uint8_t req_id, uint32_t val)
{
    uint32_t old_max_val, updata_val;
    uint8_t max_id = 0;

    RT_ASSERT(req_ctrl->req_vals);
    RT_ASSERT(req_id < req_ctrl->info.ttl_req);
    RT_ASSERT(req_ctrl->req_bits & BIT(req_id));

    old_max_val = req_ctrl->req_vals[req_ctrl->max_val_id];

    DVFS_INFO("** %s: req_id-%d, val-%d,%d, old max-%d %d\n", __func__,
              req_id, req_ctrl->req_vals[req_id],
              val, req_ctrl->max_val_id, old_max_val);

    if ((val ==  req_ctrl->req_vals[req_id]) || (val == old_max_val) ||
            (val < old_max_val && req_ctrl->max_val_id != req_id))
    {
        req_ctrl->req_vals[req_id] = val;
        updata_val = 0;
    }
    else if (val > old_max_val)
    {
        req_ctrl->req_vals[req_id] = val;
        req_ctrl->max_val_id = req_id;
        updata_val = val;
    }
    else
    {
        req_ctrl->req_vals[req_id] = val;
        max_id = req_val_get_max_val(req_ctrl->req_vals,
                                     req_ctrl->info.ttl_req, req_ctrl->req_bits);
        RT_ASSERT(max_id != REQ_ID_INV);
        req_ctrl->max_val_id = max_id;

        if (req_ctrl->req_vals[req_ctrl->max_val_id] != old_max_val)
            updata_val = req_ctrl->req_vals[req_ctrl->max_val_id];
        else
            updata_val = 0;
    }

    DVFS_INFO("** %s end: max_val_id-%d, max_val-%d, updata_val-%d\n", __func__,
              req_ctrl->max_val_id, req_ctrl->req_vals[req_ctrl->max_val_id],
              updata_val);

    return updata_val;
}

static uint32_t req_val_by_id(struct req_val_ctrl *req_ctrl, uint8_t id)
{
    RT_ASSERT((req_ctrl->req_bits | BIT(id)));

    if (id < req_ctrl->info.ttl_req)
        return req_ctrl->req_vals[id];
    else
        return 0;
}

static uint32_t req_val_max(struct req_val_ctrl *req_ctrl)
{
    return req_val_by_id(req_ctrl, req_ctrl->max_val_id);
}

static uint8_t req_val_get_id(struct req_val_ctrl *req_ctrl, uint32_t owner_id)
{
    int i;

#ifdef REQ_VAL_OWNE_EN
    for (i = 0; i < req_ctrl->info.ttl_req; i++)
    {
        if (req_ctrl->owner_id[i] == owner_id && (req_ctrl->req_bits & BIT(i)))
        {
            DVFS_INFO("has req by %x, err!\n", owner_id);
            return i;
        }
    }
#endif

    for (i = 0; i < req_ctrl->info.ttl_req; i++)
    {
        if (!(req_ctrl->req_bits & BIT(i)))
        {
            req_ctrl->req_bits |= BIT(i);
            req_ctrl->req_vals[i] = 0;
#ifdef REQ_VAL_OWNE_EN
            req_ctrl->owner_id[i] = owner_id;
#endif
            DVFS_INFO("%s:%d, req_bits-0x%x\n", __func__, i, req_ctrl->req_bits);
            return i;
        }
    }

    RT_ASSERT(i < req_ctrl->info.ttl_req);

    return REQ_ID_INV;
}
#endif

#ifdef RK_PM_USING_REQ_PWR
static struct req_pwr_desc  *regulator_get_req_desc(ePWR_ID pwrid)
{
    struct req_pwr_desc  *req_pwr = pm_req.pwr_array;

    RT_ASSERT(req_pwr);

    while (req_pwr->pwr_id)
    {
        if (req_pwr->pwr_id == pwrid)
        {
            if (!req_pwr->desc)
            {
                req_pwr->desc = regulator_get_desc_by_pwrid(pwrid);
                RT_ASSERT(req_pwr->desc);
            }
            return req_pwr;
        }
        req_pwr++;
    }
    return NULL;
}

static uint8_t  regulator_req_req_id(struct req_pwr_desc  *req_pwr,
                                     uint32_t owner_id)
{
    uint8_t ret;
    rt_base_t level;

    level = dvfs_interrupt_disable();

    ret = req_val_get_id(&req_pwr->req_ctrl, owner_id);

    dvfs_interrupt_enable(level);
    return ret;
}

/** @defgroup REGULATOR_REQ_Public_Functions Public Functions
 *  @{
 */

/**
 * This function is for initializing a base address of the req_pwr_desc array in system.
 * @param desc_array: a base address of the req_pwr_desc array.
 */
void regulator_req_desc_init(struct req_pwr_desc *desc_array)
{
    if (!desc_array)
        return;
    pm_req.pwr_array = desc_array;
}

/**
 * @brief This function is for getting all of
 * regulators info for request.
 */
void regulator_req_init(void)
{
    struct req_pwr_desc *req_pwr = pm_req.pwr_array;

    RT_ASSERT(req_pwr);

    while (req_pwr->pwr_id)
    {
        RT_ASSERT(req_pwr->req_ctrl.info.ttl_req);

        req_pwr->desc = regulator_get_desc_by_pwrid(req_pwr->pwr_id);
        RT_ASSERT(req_pwr->desc);

#ifdef REQ_VAL_OWNE_EN
        req_pwr->req_ctrl.owner_id =
            malloc(req_pwr->req_ctrl.info.ttl_req * sizeof(uint32_t));
        RT_ASSERT(req_pwr->req_ctrl.owner_id);
#endif
        req_pwr++;
    }
}

/**
 * @brief This function is getting a request description pointer and a request id.
 * @param pwrid: a id for indicating a PWR_DESC.
 * @param req_id: it is a index for setting volt in a array.
 * @return a request description pointer.
 */
struct req_pwr_desc  *regulator_get_req_volt_id(ePWR_ID pwrid, uint8_t *req_id)
{
    struct req_pwr_desc  *req_pwr;

    if (!req_id)
        return NULL;

    req_pwr = regulator_get_req_desc(pwrid);
    if (!req_pwr)
        return NULL;

    *req_id = regulator_req_req_id(req_pwr, 0);

    RT_ASSERT((*req_id) != REQ_ID_INV);

    return req_pwr;
}

/**
 * @brief This function is for setting volt.
 * @param req_pwr: a request description pointer.
 * @param req_id: indicate where the volt is saved.
 * @param volt: the volt value to be config.
 * @return RT_EOK if successful.
 */
rt_err_t regulator_req_set_voltage(struct req_pwr_desc *req_pwr, uint8_t req_id,
                                   uint32_t volt)
{
    struct req_val_ctrl *req_ctrl;
    uint32_t volt_new;
    rt_base_t level;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(req_pwr);
    req_ctrl = &req_pwr->req_ctrl;

    level = dvfs_interrupt_disable();

    volt_new = req_val_updata_val(req_ctrl, req_id, volt);

    if (volt_new)
        ret = regulator_set_voltage(req_pwr->desc, volt_new);

    dvfs_interrupt_enable(level);

    DVFS_INFO("**** %s req_id-%d, req volt-%d, updata-%d, active-%d\n",
              __func__, req_id, volt, volt_new,
              regulator_get_voltage(req_pwr->desc));
    return ret;
}

/**
 * @brief This function is for getting volt which has config into a pmic.
 * @param req_pwr: a request description pointer.
 * @return volt val.
 */
uint32_t regulator_req_get_voltage(struct req_pwr_desc *req_pwr)
{
    RT_ASSERT(req_pwr);
    return regulator_get_voltage(req_pwr->desc);
}

/**
 * @brief This function is for getting volt which is the max val in all of request.
 * @param req_pwr: a request description pointer.
 * @return volt val.
 */
uint32_t regulator_req_get_max_voltage(struct req_pwr_desc *req_pwr)
{
    RT_ASSERT(req_pwr);
    return req_val_max(&req_pwr->req_ctrl);
}

/**
 * @brief This function is for getting volt which is requested by the req_id.
 * @param req_pwr: a request description pointer.
 * @param req_id: indicate where the volt is saved.
 * @return volt val.
 */
uint32_t regulator_req_get_set_voltage(struct req_pwr_desc *req_pwr,
                                       uint8_t req_id)
{
    RT_ASSERT(req_pwr);
    return req_val_by_id(&req_pwr->req_ctrl, req_id);
}

/**
 * @brief This function is for releasing a request volt by the req_id.
 * @param req_pwr: a request description pointer.
 * @param req_id: indicate where the volt is saved.
 * @return RT_EOK if successful.
 */
rt_err_t regulator_req_voltage_release(struct req_pwr_desc *req_pwr, uint8_t req_id)
{
    RT_ASSERT(req_pwr);
    return regulator_req_set_voltage(req_pwr, req_id, 0);
}

/**
 * @brief This function is for releasing a regulator request by the req_id.
 * @param req_pwr: a request description pointer.
 * @param req_id: indicate where the volt is saved.
 * @return RT_EOK if successful.
 */
rt_err_t regulator_req_release(struct req_pwr_desc *req_pwr, uint8_t req_id)
{
    rt_err_t ret = RT_EOK;

    RT_ASSERT(req_pwr);
    if (!(req_pwr->req_ctrl.req_bits & BIT(req_id)))
        return ret;
    RT_ASSERT(req_pwr->req_ctrl.req_bits & BIT(req_id));
    ret = regulator_req_voltage_release(req_pwr, req_id);
    req_pwr->req_ctrl.req_bits &= ~BIT(req_id);
    return ret;
}
/** @} */

#endif

#ifdef RK_PM_USING_REQ_CLK
static struct req_clk_desc *clk_get_req_desc(eCLOCK_Name clk_id)
{
    int i;
    struct req_clk_desc  *req_clk = pm_req.clk_array;

    while (req_clk->clk_id)
    {
        if (req_clk->clk_id == clk_id)
        {
            return req_clk;
        }
        req_clk++;
    }
    return NULL;
}

static uint8_t clk_req_req_id(struct req_clk_desc  *req_clk,
                              uint32_t owner_id)
{
    uint8_t ret;
    rt_base_t level;

    level = dvfs_interrupt_disable();

    ret = req_val_get_id(&req_clk->req_ctrl, owner_id);

    dvfs_interrupt_enable(level);

    return ret;
}

/** @defgroup CLK_REQ_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief This function is for initializing a base address of the req_clk_desc array in system.
 * @param desc_array: a base address of the req_clk_desc array.
 */
void clk_req_desc_init(struct req_clk_desc *desc_array)
{
    if (!desc_array)
        return;
    pm_req.clk_array = desc_array;
}

/**
 * @brief This function is for getting all of
 * clks info for request.
 */
void clk_req_init(void)
{
    int i;
    struct req_clk_desc *req_clk = pm_req.clk_array;

    RT_ASSERT(req_clk);

    while (req_clk->clk_id)
    {
        RT_ASSERT(req_clk->clk_id);
#ifdef REQ_VAL_OWNE_EN
        req_clk->req_ctrl.owner_id =
            malloc(req_clk->req_ctrl.info.ttl_req * sizeof(uint32_t));
        RT_ASSERT(req_clk->req_ctrl.owner_id);
#endif
        req_clk++;
    }
}

/**
 * @brief This function is getting a request description pointer and a request id.
 * @param clk_id: a id for indicating a clk.
 * @param req_id: it is a index for setting volt in a array.
 * @return a request description pointer.
 */
struct req_clk_desc  *clk_get_req_rate_id(eCLOCK_Name clk_id, uint8_t *req_id)
{
    struct req_clk_desc  *req_clk;

    if (!req_id)
        return NULL;

    req_clk = clk_get_req_desc(clk_id);

    if (!req_clk)
        return NULL;

    *req_id = clk_req_req_id(req_clk, 0);

    RT_ASSERT((*req_id) != REQ_ID_INV);

    return req_clk;
}

/**
 * @brief This function is for setting clk rate.
 * @param req_clk: a request description pointer.
 * @param req_id: indicate where the rate saves.
 * @param rate: the rate value to be config.
 * @return RT_EOK if successful.
 */
rt_err_t clk_req_set_rate(struct req_clk_desc *req_clk, uint8_t req_id, uint32_t rate)
{
    struct req_val_ctrl *req_ctrl = &req_clk->req_ctrl;
    uint32_t new_freq;
    rt_base_t level;
    rt_err_t ret = RT_EOK;

    RT_ASSERT(req_clk);
    req_ctrl = &req_clk->req_ctrl;

    level = dvfs_interrupt_disable();

    new_freq = req_val_updata_val(req_ctrl, req_id, rate);
    if (new_freq)
    {
        ret = HAL_CRU_ClkSetFreq(req_clk->clk_id, new_freq);
        if (ret == HAL_OK)
            ret = RT_EOK;
        else
            ret = RT_ERROR;
    }

    dvfs_interrupt_enable(level);

    DVFS_INFO("**** %s: req_id-%d, req rate-%d, has set rate-(%d %d)\n",
              __func__, req_id, rate, new_freq, HAL_CRU_ClkGetFreq(req_clk->clk_id));
    return ret;
}

/**
 * @brief This function is for getting rate which has config into cru.
 * @param req_clk: a request description pointer.
 * @return clk rate.
 */
uint32_t clk_req_get_rate(struct req_clk_desc *req_clk)
{
    RT_ASSERT(req_clk);
    return HAL_CRU_ClkGetFreq(req_clk->clk_id);
}

/**
 * @brief This function is for getting rate which is the max val in all of request.
 * @param req_clk: a request description pointer.
 * @return clk rate.
 */
uint32_t clk_req_get_max_rate(struct req_clk_desc *req_clk)
{
    RT_ASSERT(req_clk);
    return req_val_max(&req_clk->req_ctrl);
}

/**
 * @brief This function is for getting rate which is requested by the req_id.
 * @param req_clk: a request description pointer.
 * @param req_id: indicate where the clk rate is saved.
 * @return clk rate.
 */
uint32_t clk_req_get_set_rate(struct req_clk_desc *req_clk, uint8_t req_id)
{
    RT_ASSERT(req_clk);
    return req_val_by_id(&req_clk->req_ctrl, req_id);
}

/**
 * @brief This function is for releasing a request rate by the req_id.
 * @param req_clk: a request description pointer.
 * @param req_id: indicate where the rate is saved.
 * @return RT_EOK if successful.
 */
rt_err_t clk_req_rate_release(struct req_clk_desc *req_clk, uint8_t req_id)
{
    RT_ASSERT(req_clk);
    return clk_req_set_rate(req_clk, req_id, 0);
}

/**
 * @brief This function is for releasing a regulator request by the req_id.
 * @param req_clk: a request description pointer.
 * @param req_id: indicate where the rate is saved.
 * @return RT_EOK if successful.
 */
rt_err_t clk_req_release(struct req_clk_desc *req_clk, uint8_t req_id)
{
    rt_err_t ret = HAL_OK;

    RT_ASSERT(req_clk);
    if (!(req_clk->req_ctrl.req_bits & BIT(req_id)))
        return ret;
    ret = clk_req_rate_release(req_clk, req_id);
    req_clk->req_ctrl.req_bits &= ~BIT(req_id);
    return ret;
}

/** @} */

#endif

#ifdef RK_PM_USING_DVFS
static uint8_t dvfs_tbl_get_idx_by_freq(struct rk_dvfs_desc *dvfs_desc,
                                        uint32_t rate)
{
    int i;

    RT_ASSERT(dvfs_desc);
    for (i = 0; i < dvfs_desc->tbl_cnt; i++)
    {
        if (dvfs_desc->table[i].freq == rate)
            return i;
    }
    return DVFS_TBL_IDX_INV;
}

static int dvfs_set_rate_volt(struct rk_dvfs_desc *dvfs_desc, uint32_t rate,
                              uint32_t old_freq, uint32_t volt)
{
    int ret = RT_EOK;

    RT_ASSERT(dvfs_desc);

    DVFS_INFO("****** %s target, new-%lu, %lu, old-%lu, %d\n",
              __func__, rate, volt, HAL_CRU_ClkGetFreq(dvfs_desc->clk_id),
              regulator_get_voltage(dvfs_desc->pwr_desc));

    if (rate == old_freq)
        goto _set_exit;

    if (rate > old_freq)
    {
        ret = regulator_set_voltage(dvfs_desc->pwr_desc, volt);
        if (ret)
        {
            DVFS_INFO("%s volt up error!\n", __func__);
            goto _set_exit;
        }
    }

    ret = HAL_CRU_ClkSetFreq(dvfs_desc->clk_id, rate);

    if (ret == HAL_OK)
        ret = RT_EOK;
    else
        ret = RT_ERROR;
    if (ret)
        goto _set_exit;

    if (rate < old_freq)
    {
        if (dvfs_desc->pwr_desc)
            ret = regulator_set_voltage(dvfs_desc->pwr_desc, volt);
        if (ret)
        {
            DVFS_INFO("%s volt down error!\n", __func__);
            goto _set_exit;
        }
    }
_set_exit:

    DVFS_INFO("****** %s end! %lu, %d, %d\n", __func__,
              HAL_CRU_ClkGetFreq(dvfs_desc->clk_id),
              regulator_get_voltage(dvfs_desc->pwr_desc), ret);

    return ret;
}

#ifdef RK_PM_USING_REQ_PWR
static int dvfs_set_rate_req_volt(struct rk_dvfs_desc *dvfs_desc, uint32_t rate,
                                  uint32_t old_freq, uint32_t volt)
{
    int ret = RT_EOK;

    RT_ASSERT(dvfs_desc);

    if (!dvfs_desc->req_pwr)
        return dvfs_set_rate_volt(dvfs_desc, rate, old_freq, volt);
    DVFS_INFO("****** %s target, new-%u, %u, active-%u, %u, has req volt-%u\n", __func__, rate, volt,
              HAL_CRU_ClkGetFreq(dvfs_desc->clk_id),
              regulator_req_get_voltage(dvfs_desc->req_pwr),
              regulator_req_get_set_voltage(dvfs_desc->req_pwr, dvfs_desc->pwr_req_id));
    if (rate > old_freq)
    {
        ret =  regulator_req_set_voltage(dvfs_desc->req_pwr,
                                         dvfs_desc->pwr_req_id, volt);
        if (ret)
            goto _set_exit;
    }

    if (rate != old_freq)
        ret = HAL_CRU_ClkSetFreq(dvfs_desc->clk_id, rate);

    if (rate <= old_freq)
    {
        ret =  regulator_req_set_voltage(dvfs_desc->req_pwr,
                                         dvfs_desc->pwr_req_id, volt);
        if (ret)
            goto _set_exit;
    }

_set_exit:

    DVFS_INFO("****** %s end, active-%u, %u, req volt-%lu\n", __func__,
              HAL_CRU_ClkGetFreq(dvfs_desc->clk_id),
              regulator_req_get_voltage(dvfs_desc->req_pwr),
              regulator_req_get_set_voltage(dvfs_desc->req_pwr, dvfs_desc->pwr_req_id));


    return ret;
}
#endif

static int dvfs_set_rate_chk_req_volt(struct rk_dvfs_desc *dvfs_desc,
                                      uint32_t rate, uint8_t tbl_idx)
{
    uint8_t old_idx = dvfs_desc->tbl_idx;
    uint32_t old_freq, volt;
    int ret = RT_ERROR;

    RT_ASSERT(dvfs_desc);

    old_freq = dvfs_desc->table[old_idx].freq;
    volt = dvfs_desc->table[tbl_idx].volt;

    DVFS_INFO("****** %s: new(%d %d), old(%d), idx(%d, %d)\n", __func__,
              rate, volt, old_freq, old_idx, tbl_idx);

#ifdef RK_PM_USING_REQ_PWR
    ret = dvfs_set_rate_req_volt(dvfs_desc, rate, old_freq, volt);
#else
    if (dvfs_desc->pwr_desc)
        ret = dvfs_set_rate_volt(dvfs_desc, rate, old_freq, volt);
#endif
    if (!ret)
        dvfs_desc->tbl_idx = tbl_idx;
    return ret;
}

#ifdef RK_PM_USING_REQ_CLK
static int dvfs_req_set_voltage(struct rk_dvfs_desc *dvfs_desc, uint8_t req_id, uint32_t volt)
{
    int ret = HAL_OK;

    RT_ASSERT(dvfs_desc);
#ifdef RK_PM_USING_REQ_PWR
    if (dvfs_desc->req_pwr)
    {
        ret =  regulator_req_set_voltage(dvfs_desc->req_pwr, req_id, volt);
    }
    else if (dvfs_desc->pwr_desc)
#endif
    {
        ret = regulator_set_voltage(dvfs_desc->pwr_desc, volt);
    }
    return ret;
}

static int dvfs_req_set_clk_rate(struct rk_dvfs_desc *dvfs_desc, uint8_t req_id, uint32_t rate)
{
    int ret = HAL_OK;

    RT_ASSERT(dvfs_desc);
    if (dvfs_desc->req_clk)
    {
        ret =  clk_req_set_rate(dvfs_desc->req_clk, req_id, rate);
    }
    else
    {
        ret = HAL_CRU_ClkSetFreq(dvfs_desc->clk_id, rate);
        if (ret == HAL_OK)
            ret = RT_EOK;
        else
            ret = RT_ERROR;
    }
    return ret;
}

static int dvfs_req_rate_chk_req_volt(struct rk_dvfs_desc *dvfs_desc,
                                      uint32_t rate, uint8_t tbl_idx,
                                      uint8_t dvfs_clk_req_id)
{
    struct req_clk_desc *req_clk;
    uint8_t req_clk_id, req_pwr_id;
    uint32_t active_freq, volt;
    int ret = HAL_OK;

    if (!dvfs_desc->req_clk)
        return dvfs_set_rate_chk_req_volt(dvfs_desc, rate, tbl_idx);

    req_clk = dvfs_desc->req_clk;
    volt = dvfs_desc->table[tbl_idx].volt;
    req_clk_id = dvfs_clk_req_id;
    req_pwr_id = dvfs_desc->pwr_req_id;
    active_freq = clk_req_get_max_rate(req_clk);
    if (!active_freq)
        active_freq = HAL_CRU_ClkGetFreq(dvfs_desc->clk_id);

    DVFS_INFO("****** %s start: (%d %d), active(%d)\n",
              __func__, rate, volt, active_freq);

    if (rate > active_freq)
        ret = dvfs_req_set_voltage(dvfs_desc, req_pwr_id, volt);
    RT_ASSERT(!ret);

    ret = dvfs_req_set_clk_rate(dvfs_desc, req_clk_id, rate);
    RT_ASSERT(!ret);

    if (rate <= active_freq)
        ret = dvfs_req_set_voltage(dvfs_desc, req_pwr_id, volt);
    RT_ASSERT(!ret);

    dvfs_desc->tbl_idx = tbl_idx;

    DVFS_INFO("****** %s end: active(%d, %d)\n",
              __func__, clk_req_get_max_rate(req_clk),
              dvfs_desc->req_pwr ? regulator_req_get_voltage(dvfs_desc->req_pwr) :
              regulator_get_voltage(dvfs_desc->pwr_desc));
    return ret;
}
#endif

static rt_err_t __dvfs_set_rate(struct rk_dvfs_desc *dvfs_desc,
                                uint32_t rate, uint8_t tbl_idx,
                                uint8_t dvfs_clk_req_id)
{
    rt_err_t ret = RT_EOK;
    rt_base_t level;

    if (!dvfs_desc)
        return RT_EINVAL;

    if (rate)
    {
        tbl_idx = dvfs_tbl_get_idx_by_freq(dvfs_desc, rate);
    }
    else
    {
        rate = dvfs_desc->table[tbl_idx].freq;
    }

    RT_ASSERT(tbl_idx < dvfs_desc->tbl_cnt);

    DVFS_INFO("******** %s: clk_id-0x%x, pwr_id-%d, %d %d new_tbl_idx-%d\n",
              __func__, dvfs_desc->clk_id, dvfs_desc->pwr_id,
              rate, dvfs_desc->table[tbl_idx].volt, tbl_idx);

    level = dvfs_interrupt_disable();

#ifdef RK_PM_USING_REQ_CLK
    ret = dvfs_req_rate_chk_req_volt(dvfs_desc, rate, tbl_idx, dvfs_clk_req_id);
#else
    ret = dvfs_set_rate_chk_req_volt(dvfs_desc, rate, tbl_idx);
#endif

    dvfs_interrupt_enable(level);

    return ret;
}

/** @defgroup DVFS_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief This function is for setting clk rate and setting volt according to its dvfs table.
 * @param dvfs_desc: a rk_dvfs_desc pointer.
 * @param rate: the rate value to be config.
 * @param dvfs_clk_req_id: indicate where the rate are saved.
 * @return RT_EOK if successful.
 */
rt_err_t dvfs_set_rate(struct rk_dvfs_desc *dvfs_desc, uint8_t dvfs_clk_req_id,
                       uint32_t rate)
{
    RT_ASSERT(dvfs_desc);
    return __dvfs_set_rate(dvfs_desc, rate, DVFS_TBL_IDX_INV, dvfs_clk_req_id);
}

/**
 * @brief This function is for setting clk rate and setting volt according to
 *  its dvfs table.
 * @param dvfs_desc: a rk_dvfs_desc pointer.
 * @param tbl_idx: a idx of a dvfs table,
 *  it is for getting a clk rate and a volt from the dvfs table.
 * @param dvfs_clk_req_id: indicate where the rate are saved.
 * @return RT_EOK if successful.
 */
rt_err_t dvfs_set_rate_by_idx(struct rk_dvfs_desc *dvfs_desc,
                              uint8_t tbl_idx, uint8_t dvfs_clk_req_id)
{
    RT_ASSERT(dvfs_desc);
    return __dvfs_set_rate(dvfs_desc, 0, tbl_idx, dvfs_clk_req_id);
}

/**
 * @brief This function is for getting rate which has config into cru.
 * @param dvfs_desc: a dvfs_desc pointer.
 * @return clk rate.
 */
uint32_t dvfs_req_get_rate(struct rk_dvfs_desc *dvfs_desc)
{
    RT_ASSERT(dvfs_desc);
    return HAL_CRU_ClkGetFreq(dvfs_desc->clk_id);
}

/**
 * @brief This function is for getting rate which is the max val in all of request.
 * @param dvfs_desc: a rk_dvfs_desc pointer.
 * @return clk rate.
 */
uint32_t dvfs_req_get_max_rate(struct rk_dvfs_desc *dvfs_desc)
{
    RT_ASSERT(dvfs_desc);
#ifdef RK_PM_USING_REQ_CLK
    if (dvfs_desc->req_clk)
        return clk_req_get_max_rate(dvfs_desc->req_clk);
    else
#endif
        return HAL_CRU_ClkGetFreq(dvfs_desc->clk_id);
}

/**
 * @brief This function is for getting rate which is requested by the dvfs_clk_req_id.
 * @param dvfs_desc: a rk_dvfs_desc pointer.
 * @param dvfs_clk_req_id: indicate where the clk rate is saved.
 * @return clk rate.
 */
uint32_t dvfs_req_get_set_rate(struct rk_dvfs_desc *dvfs_desc,
                               uint8_t dvfs_clk_req_id)
{
    RT_ASSERT(dvfs_desc);
#ifdef RK_PM_USING_REQ_CLK
    if (dvfs_desc->req_clk)
        return clk_req_get_set_rate(dvfs_desc->req_clk, dvfs_clk_req_id);
    else
#endif
        return HAL_CRU_ClkGetFreq(dvfs_desc->clk_id);
}

/**
 * @brief This function is for releasing a request rate by the dvfs_clk_req_id.
 * @param dvfs_desc: a rk_dvfs_desc pointer.
 * @param dvfs_clk_req_id: indicate where the rate is saved.
 */
void rk_dvfs_req_rate_release(struct rk_dvfs_desc *dvfs_desc, uint8_t dvfs_clk_req_id)
{
    RT_ASSERT(dvfs_desc);
#ifdef RK_PM_USING_REQ_CLK
    if (dvfs_desc ->req_clk)
        clk_req_rate_release(dvfs_desc ->req_clk, dvfs_clk_req_id);
#endif

#ifdef RK_PM_USING_REQ_PWR
    if (dvfs_desc ->req_pwr)
        regulator_req_voltage_release(dvfs_desc ->req_pwr,
                                      dvfs_desc->pwr_req_id);
#endif
    return;
}

/**
 * @brief This function is for releasing a regulator request by the dvfs_clk_req_id.
 * @param dvfs_desc: a dvfs_desc pointer.
 * @param dvfs_clk_req_id: indicate where the rate is saved.
 */
void rk_dvfs_req_release(struct rk_dvfs_desc *dvfs_desc,  uint8_t dvfs_clk_req_id)
{
    RT_ASSERT(dvfs_desc);
#ifdef RK_PM_USING_REQ_CLK
    if (dvfs_desc ->req_clk)
        clk_req_release(dvfs_desc ->req_clk, dvfs_clk_req_id);
#endif

#ifdef RK_PM_USING_REQ_PWR
    if (dvfs_desc ->req_pwr)
        regulator_req_release(dvfs_desc ->req_pwr, dvfs_desc->pwr_req_id);
#endif

    return;
}

/**
 * @brief This function is getting a rk_dvfs_desc pointer and a request dvfs id.
 * @param clk_id: a id for indicating a clk.
 * @param dvfs_clk_req_id: it is a index for setting clk in a array.
 * @return a rk_dvfs_desc pointer.
 */
struct rk_dvfs_desc *dvfs_get_by_clk(eCLOCK_Name clk_id, uint8_t  *dvfs_clk_req_id)
{
    struct rk_dvfs_desc *dvfs_desc = NULL;
    struct rk_dvfs_desc *desc = pm_req.dvfs_array ;

    while (desc->pwr_id)
    {
        if (desc->clk_id == clk_id)
        {
            dvfs_desc = desc;
            DVFS_INFO("%s: dvfs: pwr_id-%d, clk_id-0x%x\n",
                      __func__, dvfs_desc->pwr_id, dvfs_desc->clk_id);
            break;
        }
        desc++;
    }

    if (!dvfs_desc)
        return NULL;

#ifdef RK_PM_USING_REQ_CLK
    if (dvfs_desc->req_clk && dvfs_clk_req_id)
    {
        *dvfs_clk_req_id = clk_req_req_id(dvfs_desc->req_clk, 0);
        RT_ASSERT((*dvfs_clk_req_id) != REQ_ID_INV)

        DVFS_INFO("%s: dvfs-%d, pwr_id-%d, dvfs_clk_req_id-0x%x\n",
                  __func__, i, dvfs_desc->pwr_id, clk_req_id);
    }
#endif

    return dvfs_desc;
}

/**
 * @brief This function is for initializing a base address of
 *  the rk_dvfs_desc array in system.
 * @param dvfs_array: a base address of the rk_dvfs_desc array.
 */
void dvfs_desc_init(struct rk_dvfs_desc *dvfs_array)
{
    int i;

    if (!dvfs_array)
        return;

    pm_req.dvfs_array = dvfs_array;

    while (dvfs_array->pwr_id)
    {
        RT_ASSERT(dvfs_array->table);
        i = 0;
        while (dvfs_array->table[i].freq)
        {
            i++;
        };
        dvfs_array->tbl_cnt = i;

        dvfs_array++;
    };
}

#ifdef RT_USING_COMMON_TEST_PM
/**
 * @brief This function is for getting a base address of
 *  the rk_dvfs_desc array in system.
 * @return a rk_dvfs_desc pointer.
 */
struct rk_dvfs_desc *dvfs_get_desc(void)
{
    return pm_req.dvfs_array;
}

/**
 * @brief This function is for getting the number of
 *  rk_dvfs_desc array in system.
 * @return the number of rk_dvfs_desc.
 */
uint32_t dvfs_get_desc_cnt(void)
{
    return pm_req.dvfs_cnt;
}
#endif

/**
 * @brief This function is for initializing clk and regulator
 * for a dvfs node.
 */
void dvfs_init(void)
{
    struct rk_dvfs_desc *dvfs_desc = pm_req.dvfs_array;
    int i = 0;
    uint32_t rate, old_req, volt;

    if (!dvfs_desc)
        return;

    while (dvfs_desc->pwr_id)
    {
        dvfs_desc->pwr_desc = regulator_get_desc_by_pwrid(dvfs_desc->pwr_id);
        if (!dvfs_desc->pwr_desc)
            continue;
        old_req = HAL_CRU_ClkGetFreq(dvfs_desc->clk_id);

        if (!old_req)
        {
            dvfs_desc->tbl_idx = DVFS_TBL_IDX_INV;
            continue;
        }

        RT_ASSERT(dvfs_desc->tbl_idx < dvfs_desc->tbl_cnt);

        volt = dvfs_desc->table[dvfs_desc->tbl_idx].volt;
        rate = dvfs_desc->table[dvfs_desc->tbl_idx].freq;

        DVFS_INFO("%s: dvfs-%d clk_id-0x%x, pwr_id-%d  tbl_idx-%d\n",
                  __func__, i, dvfs_desc->clk_id, dvfs_desc->pwr_id,
                  dvfs_desc->tbl_idx);
        DVFS_INFO("%s: %lu %lu, init-%lu %lu\n", __func__,
                  rate, volt,  old_req, regulator_get_voltage(dvfs_desc->pwr_desc));

#ifdef RK_PM_USING_REQ_PWR
        dvfs_desc->req_pwr = regulator_get_req_volt_id(dvfs_desc->pwr_id, &dvfs_desc->pwr_req_id);
        RT_ASSERT(dvfs_desc->req_pwr);
        dvfs_set_rate_req_volt(dvfs_desc, rate, old_req, volt);
#else
        if (rate != old_req)
        {
            if (dvfs_set_rate_volt(dvfs_desc, rate, old_req, volt))
            {
                rt_kprintf("%s:int dvfs error!(%d))\n", __func__, i);
                while (1);
            }
        }
        else
        {
            regulator_set_voltage(dvfs_desc->pwr_desc, volt);
        }
#endif


#ifdef RK_PM_USING_REQ_CLK
        dvfs_desc->req_clk = clk_get_req_desc(dvfs_desc->clk_id);
#endif

        dvfs_desc++;
        i++;
    }
    pm_req.dvfs_cnt = i;
}

/** @} */

#endif

#ifndef RK_PM_USING_REQ_PWR
void regulator_req_init(void)
{
    return;
}

void regulator_req_desc_init(struct req_pwr_desc *desc_array)
{
    return;
}

struct req_pwr_desc  *regulator_get_req_volt_id(ePWR_ID pwrid,
        uint8_t *req_id)
{
    return NULL;
}

rt_err_t regulator_req_set_voltage(struct req_pwr_desc *req_pwr,
                                   uint8_t req_id, uint32_t volt)
{
    return RT_ERROR;
}

rt_err_t regulator_req_voltage_release(struct req_pwr_desc *req_pwr, uint8_t req_id)
{
    return RT_ERROR;
}

rt_err_t regulator_req_release(struct req_pwr_desc *req_pwr, uint8_t req_id)
{
    return RT_ERROR;
}
uint32_t regulator_req_get_voltage(struct req_pwr_desc *req_pwr)
{
    return 0;
}

uint32_t regulator_req_get_max_voltage(struct req_pwr_desc *req_pwr)
{
    return 0;
}

uint32_t regulator_req_get_set_voltage(struct req_pwr_desc *req_pwr,
                                       uint8_t req_id)
{
    return 0;
}
#endif

#ifndef RK_PM_USING_REQ_CLK
void clk_req_desc_init(struct req_clk_desc *desc_array)
{
    return;
}

void clk_req_init(void)
{
    return;
}
struct req_clk_desc  *clk_get_req_rate_id(eCLOCK_Name clk_id, uint8_t *req_id)
{
    return NULL;
}
rt_err_t clk_req_set_rate(struct req_clk_desc *req_clk, uint8_t req_id, uint32_t rate)
{
    return RT_ERROR;
}
rt_err_t clk_req_rate_release(struct req_clk_desc *req_clk, uint8_t req_id)
{
    return RT_ERROR;
}
rt_err_t clk_req_release(struct req_clk_desc *req_clk, uint8_t req_id)
{
    return RT_ERROR;
}

uint32_t clk_req_get_rate(struct req_clk_desc *req_clk)
{
    return 0;
}
uint32_t clk_req_get_max_rate(struct req_clk_desc *req_clk)
{
    return 0;
}

uint32_t clk_req_get_set_rate(struct req_clk_desc *req_clk, uint8_t req_id)
{
    return 0;
}
#endif

#ifndef RK_PM_USING_DVFS
rt_err_t dvfs_set_rate(struct rk_dvfs_desc *dvfs_desc, uint8_t dvfs_clk_req_id,
                       uint32_t rate)
{
    return RT_ERROR;
}

rt_err_t dvfs_set_rate_by_idx(struct rk_dvfs_desc *dvfs_desc,
                              uint8_t tbl_idx, uint8_t dvfs_clk_req_id)
{
    return RT_ERROR;
}

struct rk_dvfs_desc *dvfs_get_by_clk(eCLOCK_Name clk_id, uint8_t  *dvfs_clk_req_id)
{
    return NULL;
}

uint32_t dvfs_req_get_rate(struct rk_dvfs_desc *dvfs_desc)
{
    return 0;
}
uint32_t dvfs_req_get_max_rate(struct rk_dvfs_desc *dvfs_desc)
{
    return 0;
}

uint32_t dvfs_req_get_set_rate(struct rk_dvfs_desc *dvfs_desc, uint8_t dvfs_clk_req_id)
{
    return 0;
}

void rk_dvfs_req_rate_release(struct rk_dvfs_desc *dvfs_desc,
                              uint8_t dvfs_clk_req_id)
{
    return;
}

void rk_dvfs_req_release(struct rk_dvfs_desc *dvfs_desc,  uint8_t dvfs_clk_req_id)
{
    return;
}
void dvfs_desc_init(struct rk_dvfs_desc *dvfs_array)
{
    return;
}
void dvfs_init()
{
    return;
}
#endif

/** @} */  // DVFS

/** @} */  // RKBSP_Driver_Reference
