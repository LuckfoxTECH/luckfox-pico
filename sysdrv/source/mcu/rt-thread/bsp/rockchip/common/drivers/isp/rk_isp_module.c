/*
 *  Rockchip driver for RT RK ISP 1.0
 * (Based on Intel driver for sofiaxxx)
 *
 * Copyright (C) 2015 Intel Mobile Communications GmbH
 * Copyright (C) 2016 Fuzhou Rockchip Electronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-7-23      ISP_TEAM     first implementation
 *
 */
#include "rk_isp_isp.h"
#include "rk_isp.h"
#include "isp.h"
#include "vcm.h"

#if defined(RT_USING_ISP)

/* For Debugging only!!! */

#define RKISP_MODULE_DEFAULT_VBLANKING_TIME                    2000

#define RKISP_DPRINT(level, ...) \
    do { \
        if (level == RKISP_ERROR) \
            rt_kprintf(__VA_ARGS__); \
        else if(ISP_DEBUG) \
            rt_kprintf(__VA_ARGS__); \
    } while (0)

/* Set this flag to enable ISP Register debug
#define RKISP_DEBUG_REG*/
/* Set this flag to dump the parameters
#define RKISP_DEBUG_PARAM*/
/* Set this flag to trace the capture params
#define LOG_CAPTURE_PARAMS*/
/* Set this flag to trace the isr execution time
#define LOG_ISR_EXE_TIME*/
/* Set this flag to exclude everything except
measurements
#define RKISP_DEBUG_DISABLE_BLOCKS*/

#ifdef LOG_CAPTURE_PARAMS
static struct rkisp_last_capture_config g_last_capture_config;
#endif

#ifdef LOG_ISR_EXE_TIME
static unsigned int g_longest_isr_time;
#endif

static inline bool rt_do_set_rkisp_isr_other_config(
    struct rt_rkisp_isp_dev *isp_dev,
    int *time_left);

static inline bool rt_do_set_rkisp_isr_meas_config(
    struct rt_rkisp_isp_dev *isp_dev,
    int *time_left);

/* Functions for Debugging */
static void rkisp_param_dump(const void *config, unsigned int module);

static void rkisp_reg_dump(const struct rt_rkisp_isp_dev *isp_dev,
                           unsigned int module, int level);

#ifdef LOG_CAPTURE_PARAMS
static void rkisp_reg_dump_capture(const struct rt_rkisp_isp_dev *isp_dev);
#endif
static inline bool rt_rkisp_isp_other_cfg(
    struct rt_rkisp_isp_dev *isp_dev,
    int *time_left);
static inline bool rt_rkisp_isr_meas_config(
    struct rt_rkisp_isp_dev *isp_dev,
    int *time_left);

static int rt_rkisp_module_enable(struct rt_rkisp_isp_dev *isp_dev,
                                  bool flag, int *value, unsigned int module)
{
    unsigned int curr_id, new_id;
    unsigned int *updates, *curr_ens, *new_ens, *actives;
    unsigned long lock_flags = 0;

    if (module >= RK_ISP_MODULE_MAX)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    if (module >= RK_ISP_MEAS_ID)
    {
        curr_id = isp_dev->meas_cfgs.log[module].curr_id;
        new_id = isp_dev->meas_cfgs.log[module].new_id;
        updates = &isp_dev->meas_cfgs.module_updates;
        actives = &isp_dev->meas_cfgs.module_actives;
        curr_ens = &isp_dev->meas_cfgs.cfgs[curr_id].module_ens;
        new_ens = &isp_dev->meas_cfgs.cfgs[new_id].module_ens;
    }
    else
    {
        curr_id = isp_dev->other_cfgs.log[module].curr_id;
        new_id = isp_dev->other_cfgs.log[module].new_id;
        updates = &isp_dev->other_cfgs.module_updates;
        actives = &isp_dev->other_cfgs.module_actives;
        curr_ens = &isp_dev->other_cfgs.cfgs[curr_id].module_ens;
        new_ens = &isp_dev->other_cfgs.cfgs[new_id].module_ens;
    }

    if (flag == _GET_)
    {
        *value = RKISP_MODULE_IS_EN(
                     *curr_ens, (1 << module));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(*actives, (1 << module)) && *value)
        goto end;

    if ((RKISP_MODULE_IS_EN(*curr_ens, (1 << module)) != *value) ||
            (RKISP_MODULE_IS_UPDATE(*updates, (1 << module))))
    {
        if (*value)
            RKISP_MODULE_EN(*new_ens, (1 << module));
        else
            RKISP_MODULE_DIS(*new_ens, (1 << module));
        RKISP_MODULE_UPDATE(*updates, (1 << module));
    }

end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif
    return 0;
}

/* ISP DPCC */
static int rt_rkisp_dpcc_param(struct rt_rkisp_isp_dev *isp_dev,
                               bool flag, struct rk_isp_dpcc_config *arg)
{
    unsigned long lock_flags = 0;
    unsigned int i;
    struct rk_isp_dpcc_methods_config *method;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_dpcc_config *curr_cfg, *new_cfg;
    int retval = RT_EOK;

    if (arg == NULL)
    {
        RKISP_DPRINT(RKISP_ERROR, "arg is NULL: %s\n", __FUNCTION__);

        return -RT_EINVAL;
    }

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif


    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_DPCC_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].dpcc_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].dpcc_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_DPCC))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_DPCC);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_DPCC) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->mode > RKISP_DPCC_MODE_MAX ||
            arg->output_mode > RKISP_DPCC_OUTPUTMODE_MAX ||
            arg->set_use > RKISP_DPCC_SETUSE_MAX ||
            arg->ro_limits & RKISP_DPCC_RO_LIMIT_RESERVED ||
            arg->rnd_offs & RKISP_DPCC_RND_OFFS_RESERVED)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    method = &arg->methods[0];
    for (i = 0; i < RK_ISP_DPCC_METHODS_MAX; i++)
    {
        if ((method->method & RKISP_DPCC_METHODS_SET_RESERVED) ||
                (method->line_thresh & RKISP_DPCC_LINE_THRESH_RESERVED) ||
                (method->line_mad_fac & RKISP_DPCC_LINE_MAD_FAC_RESERVED) ||
                (method->pg_fac & RKISP_DPCC_PG_FAC_RESERVED) ||
                (method->rnd_thresh & RKISP_DPCC_RND_THRESH_RESERVED) ||
                (method->rg_fac & RKISP_DPCC_RG_FAC_RESERVED))
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible param in function: %s\n", __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }
        method++;
    }


    rt_memcpy(new_cfg, arg, sizeof(struct rk_isp_dpcc_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_DPCC);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif
    return retval;
}

static void rt_rkisp_dpcc_config(
    const struct rt_rkisp_isp_dev *isp_dev
)
{
    unsigned int i;
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_DPCC_ID].new_id;
    const struct rk_isp_dpcc_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].dpcc_config;

    WRITE_RKISP_REG(pconfig->mode, RK_ISP_DPCC_MODE);
    WRITE_RKISP_REG(pconfig->output_mode, RK_ISP_DPCC_OUTPUT_MODE);
    WRITE_RKISP_REG(pconfig->set_use, RK_ISP_DPCC_SET_USE);

    WRITE_RKISP_REG(pconfig->methods[0].method,
                    RK_ISP_DPCC_METHODS_SET_1);
    WRITE_RKISP_REG(pconfig->methods[1].method,
                    RK_ISP_DPCC_METHODS_SET_2);
    WRITE_RKISP_REG(pconfig->methods[2].method,
                    RK_ISP_DPCC_METHODS_SET_3);
    for (i = 0; i < RK_ISP_DPCC_METHODS_MAX; i++)
    {
        WRITE_RKISP_REG(pconfig->methods[i].line_thresh,
                        RK_ISP_DPCC_LINE_THRESH_1 + 0x14 * i);
        WRITE_RKISP_REG(pconfig->methods[i].line_mad_fac,
                        RK_ISP_DPCC_LINE_MAD_FAC_1 + 0x14 * i);
        WRITE_RKISP_REG(pconfig->methods[i].pg_fac,
                        RK_ISP_DPCC_PG_FAC_1 + 0x14 * i);
        WRITE_RKISP_REG(pconfig->methods[i].rnd_thresh,
                        RK_ISP_DPCC_RND_THRESH_1 + 0x14 * i);
        WRITE_RKISP_REG(pconfig->methods[i].rg_fac,
                        RK_ISP_DPCC_RG_FAC_1 + 0x14 * i);
    }

    WRITE_RKISP_REG(pconfig->rnd_offs, RK_ISP_DPCC_RND_OFFS);
    WRITE_RKISP_REG(pconfig->ro_limits, RK_ISP_DPCC_RO_LIMITS);
    return;
}

static void rt_rkisp_dpcc_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_OR(RKISP_DPCC_ENA, RK_ISP_DPCC_MODE);
}

static void rt_rkisp_dpcc_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG(RKISP_DPCC_DIS, RK_ISP_DPCC_MODE);
}

/* ISP black level substraction interface function */
static int rt_rkisp_bls_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_bls_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_bls_config *curr_cfg, *new_cfg;
    int retval = RT_EOK;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_BLS_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].bls_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].bls_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_BLS))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_BLS);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_BLS) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->bls_window1.h_offs > RKISP_BLS_START_H_MAX ||
            arg->bls_window1.h_size > RKISP_BLS_STOP_H_MAX ||
            arg->bls_window1.v_offs > RKISP_BLS_START_V_MAX ||
            arg->bls_window1.v_size > RKISP_BLS_STOP_V_MAX ||
            arg->bls_window2.h_offs > RKISP_BLS_START_H_MAX ||
            arg->bls_window2.h_size > RKISP_BLS_STOP_H_MAX ||
            arg->bls_window2.v_offs > RKISP_BLS_START_V_MAX ||
            arg->bls_window2.v_size > RKISP_BLS_STOP_V_MAX ||
            arg->bls_samples > RKISP_BLS_SAMPLES_MAX ||
            arg->fixed_val.r > RKISP_BLS_FIX_SUB_MAX ||
            arg->fixed_val.gr > RKISP_BLS_FIX_SUB_MAX ||
            arg->fixed_val.gb > RKISP_BLS_FIX_SUB_MAX ||
            arg->fixed_val.b > RKISP_BLS_FIX_SUB_MAX ||
            arg->fixed_val.r < (short) RKISP_BLS_FIX_SUB_MIN ||
            arg->fixed_val.gr < (short) RKISP_BLS_FIX_SUB_MIN ||
            arg->fixed_val.gb < (short) RKISP_BLS_FIX_SUB_MIN ||
            arg->fixed_val.b < (short) RKISP_BLS_FIX_SUB_MIN)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg, arg, sizeof(struct rk_isp_bls_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_BLS);
end:

#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_bls_get_meas(const struct rt_rkisp_isp_dev *isp_dev,
                                  struct rk_isp_stat_buffer *pbuf)
{
    const struct rk_isp_device *rt_isp_dev =
        rt_container_of(isp_dev, struct rk_isp_device, isp_dev);
    enum rk_isp_pix_fmt in_pix_fmt;

    in_pix_fmt = rt_isp_dev->config.isp_config.input->pix_fmt;
    if (RKISP_PIX_FMT_BAYER_PAT_IS_BGGR(in_pix_fmt))
    {
        pbuf->params.ae.bls_val.meas_b =
            READ_RKISP_REG(RK_ISP_BLS_A_MEASURED);
        pbuf->params.ae.bls_val.meas_gb =
            READ_RKISP_REG(RK_ISP_BLS_B_MEASURED);
        pbuf->params.ae.bls_val.meas_gr =
            READ_RKISP_REG(RK_ISP_BLS_C_MEASURED);
        pbuf->params.ae.bls_val.meas_r =
            READ_RKISP_REG(RK_ISP_BLS_D_MEASURED);
    }
    else if (RKISP_PIX_FMT_BAYER_PAT_IS_GBRG(in_pix_fmt))
    {
        pbuf->params.ae.bls_val.meas_gb =
            READ_RKISP_REG(RK_ISP_BLS_A_MEASURED);
        pbuf->params.ae.bls_val.meas_b =
            READ_RKISP_REG(RK_ISP_BLS_B_MEASURED);
        pbuf->params.ae.bls_val.meas_r =
            READ_RKISP_REG(RK_ISP_BLS_C_MEASURED);
        pbuf->params.ae.bls_val.meas_gr =
            READ_RKISP_REG(RK_ISP_BLS_D_MEASURED);
    }
    else if (RKISP_PIX_FMT_BAYER_PAT_IS_GRBG(in_pix_fmt))
    {
        pbuf->params.ae.bls_val.meas_gr =
            READ_RKISP_REG(RK_ISP_BLS_A_MEASURED);
        pbuf->params.ae.bls_val.meas_r =
            READ_RKISP_REG(RK_ISP_BLS_B_MEASURED);
        pbuf->params.ae.bls_val.meas_b =
            READ_RKISP_REG(RK_ISP_BLS_C_MEASURED);
        pbuf->params.ae.bls_val.meas_gb =
            READ_RKISP_REG(RK_ISP_BLS_D_MEASURED);
    }
    else if (RKISP_PIX_FMT_BAYER_PAT_IS_RGGB(in_pix_fmt))
    {
        pbuf->params.ae.bls_val.meas_r =
            READ_RKISP_REG(RK_ISP_BLS_A_MEASURED);
        pbuf->params.ae.bls_val.meas_gr =
            READ_RKISP_REG(RK_ISP_BLS_B_MEASURED);
        pbuf->params.ae.bls_val.meas_gb =
            READ_RKISP_REG(RK_ISP_BLS_C_MEASURED);
        pbuf->params.ae.bls_val.meas_b =
            READ_RKISP_REG(RK_ISP_BLS_D_MEASURED);
    }
}

static void rt_rkisp_bls_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_BLS_ID].new_id;
    const struct rk_isp_bls_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].bls_config;
    unsigned int new_control = 0;
    const struct rk_isp_device *rt_isp_dev =
        rt_container_of(isp_dev, struct rk_isp_device, isp_dev);
    enum rk_isp_pix_fmt in_pix_fmt;

    in_pix_fmt = rt_isp_dev->config.isp_config.input->pix_fmt;

    /* fixed subtraction values */
    if (pconfig->enable_auto == false)
    {
        const struct rk_isp_bls_fixed_val *pval =
                &pconfig->fixed_val;

        if (RKISP_PIX_FMT_BAYER_PAT_IS_BGGR(in_pix_fmt))
        {
            WRITE_RKISP_REG(pval->r, RK_ISP_BLS_D_FIXED);
            WRITE_RKISP_REG(pval->gr, RK_ISP_BLS_C_FIXED);
            WRITE_RKISP_REG(pval->gb, RK_ISP_BLS_B_FIXED);
            WRITE_RKISP_REG(pval->b, RK_ISP_BLS_A_FIXED);
        }
        else if (RKISP_PIX_FMT_BAYER_PAT_IS_GBRG(in_pix_fmt))
        {
            WRITE_RKISP_REG(pval->r, RK_ISP_BLS_C_FIXED);
            WRITE_RKISP_REG(pval->gr, RK_ISP_BLS_D_FIXED);
            WRITE_RKISP_REG(pval->gb, RK_ISP_BLS_A_FIXED);
            WRITE_RKISP_REG(pval->b, RK_ISP_BLS_B_FIXED);
        }
        else if (RKISP_PIX_FMT_BAYER_PAT_IS_GRBG(in_pix_fmt))
        {
            WRITE_RKISP_REG(pval->r, RK_ISP_BLS_B_FIXED);
            WRITE_RKISP_REG(pval->gr, RK_ISP_BLS_A_FIXED);
            WRITE_RKISP_REG(pval->gb, RK_ISP_BLS_D_FIXED);
            WRITE_RKISP_REG(pval->b, RK_ISP_BLS_C_FIXED);
        }
        else if (RKISP_PIX_FMT_BAYER_PAT_IS_RGGB(in_pix_fmt))
        {
            WRITE_RKISP_REG(pval->r, RK_ISP_BLS_A_FIXED);
            WRITE_RKISP_REG(pval->gr, RK_ISP_BLS_B_FIXED);
            WRITE_RKISP_REG(pval->gb, RK_ISP_BLS_C_FIXED);
            WRITE_RKISP_REG(pval->b, RK_ISP_BLS_D_FIXED);
        }

        new_control = RKISP_BLS_MODE_FIXED;
        WRITE_RKISP_REG(new_control, RK_ISP_BLS_CTRL);
    }
    else
    {
        if (pconfig->en_windows & 2)
        {
            WRITE_RKISP_REG(pconfig->bls_window2.h_offs,
                            RK_ISP_BLS_H2_START);
            WRITE_RKISP_REG(pconfig->bls_window2.h_size,
                            RK_ISP_BLS_H2_STOP);
            WRITE_RKISP_REG(pconfig->bls_window2.v_offs,
                            RK_ISP_BLS_V2_START);
            WRITE_RKISP_REG(pconfig->bls_window2.v_size,
                            RK_ISP_BLS_V2_STOP);
            new_control |= RKISP_BLS_WINDOW_2;
        }

        if (pconfig->en_windows & 1)
        {
            WRITE_RKISP_REG(pconfig->bls_window1.h_offs,
                            RK_ISP_BLS_H1_START);
            WRITE_RKISP_REG(pconfig->bls_window1.h_size,
                            RK_ISP_BLS_H1_STOP);
            WRITE_RKISP_REG(pconfig->bls_window1.v_offs,
                            RK_ISP_BLS_V1_START);
            WRITE_RKISP_REG(pconfig->bls_window1.v_size,
                            RK_ISP_BLS_V1_STOP);
            new_control |= RKISP_BLS_WINDOW_1;
        }

        WRITE_RKISP_REG(pconfig->bls_samples, RK_ISP_BLS_SAMPLES);

        new_control |= RKISP_BLS_MODE_MEASURED;

        WRITE_RKISP_REG(new_control, RK_ISP_BLS_CTRL);
    }

}

static void rt_rkisp_bls_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_OR(RKISP_BLS_ENA, RK_ISP_BLS_CTRL);
}

static void rt_rkisp_bls_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG(RKISP_BLS_DIS, RK_ISP_BLS_CTRL);
}


/* ISP LS correction interface function */
static int rt_rkisp_lsc_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_lsc_config *arg)
{
    int i;
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_lsc_config *curr_cfg, *new_cfg;
    int retval = RT_EOK;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_LSC_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].lsc_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].lsc_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_LSC))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_LSC);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_LSC) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    for (i = 0; i < RK_ISP_LSC_SIZE_TBL_SIZE; i++)
    {
        if ((*(arg->x_size_tbl + i) & RKISP_LSC_SECT_SIZE_RESERVED) ||
                (*(arg->y_size_tbl + i) & RKISP_LSC_SECT_SIZE_RESERVED))
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible sect size x 0x%x y 0x%x in function: %s\n",
                         *(arg->x_size_tbl + i),
                         *(arg->y_size_tbl + i), __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }
    }

    for (i = 0; i < RK_ISP_LSC_GRAD_TBL_SIZE; i++)
    {
        if ((*(arg->x_grad_tbl + i) & RKISP_LSC_GRAD_RESERVED) ||
                (*(arg->y_grad_tbl + i) & RKISP_LSC_GRAD_RESERVED))
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible grad x 0x%x y 0x%xin function: %s\n",
                         *(arg->x_grad_tbl + i),
                         *(arg->y_grad_tbl + i), __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }
    }

    for (i = 0; i < RK_ISP_LSC_DATA_TBL_SIZE; i++)
    {
        if ((*(arg->r_data_tbl + i) & RKISP_LSC_SAMPLE_RESERVED) ||
                (*(arg->gr_data_tbl + i) & RKISP_LSC_SAMPLE_RESERVED) ||
                (*(arg->gb_data_tbl + i) & RKISP_LSC_SAMPLE_RESERVED) ||
                (*(arg->b_data_tbl + i) & RKISP_LSC_SAMPLE_RESERVED))
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible sample r 0x%x gr 0x%x gb 0x%x b 0x%x "
                         "in function: %s\n",
                         *(arg->r_data_tbl + i),
                         *(arg->gr_data_tbl + i),
                         *(arg->gb_data_tbl + i),
                         *(arg->b_data_tbl + i), __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_lsc_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_LSC);

end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static bool rt_rkisp_lsc_correct_matrix_config(struct rt_rkisp_isp_dev *isp_dev)
{
    int i, n;
    unsigned int isp_lsc_status, sram_addr, isp_lsc_table_sel;
    unsigned int data;
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_LSC_ID].new_id;
    const struct rk_isp_lsc_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].lsc_config;

    isp_lsc_status = READ_RKISP_REG(RK_ISP_LSC_STATUS);
    sram_addr = (isp_lsc_status & 0x2U) ? 0U : 153U; /* ( 17 * 18 ) >> 1 */

    WRITE_RKISP_REG(sram_addr, RK_ISP_LSC_R_TABLE_ADDR);
    WRITE_RKISP_REG(sram_addr, RK_ISP_LSC_GR_TABLE_ADDR);
    WRITE_RKISP_REG(sram_addr, RK_ISP_LSC_GB_TABLE_ADDR);
    WRITE_RKISP_REG(sram_addr, RK_ISP_LSC_B_TABLE_ADDR);

    /* program data tables (table size is 9 * 17 = 153) */
    for (n = 0; n < ((RKISP_LSC_SECTORS_MAX + 1) * (RKISP_LSC_SECTORS_MAX + 1)); n += RKISP_LSC_SECTORS_MAX + 1)
    {
        /* 17 sectors with 2 values in one DWORD = 9 DWORDs (8 steps + 1 outside loop) */
        for (i = 0; i < (RKISP_LSC_SECTORS_MAX); i += 2)
        {
            data = RKISP_LSC_TABLE_DATA(pconfig->r_data_tbl[n + i],
                                        pconfig->r_data_tbl[n + i + 1]);
            WRITE_RKISP_REG(data, RK_ISP_LSC_R_TABLE_DATA);

            data = RKISP_LSC_TABLE_DATA(pconfig->gr_data_tbl[n + i],
                                        pconfig->gr_data_tbl[n + i + 1]);
            WRITE_RKISP_REG(data, RK_ISP_LSC_GR_TABLE_DATA);

            data = RKISP_LSC_TABLE_DATA(pconfig->gb_data_tbl[n + i],
                                        pconfig->gb_data_tbl[n + i + 1]);
            WRITE_RKISP_REG(data, RK_ISP_LSC_GB_TABLE_DATA);

            data = RKISP_LSC_TABLE_DATA(pconfig->b_data_tbl[n + i],
                                        pconfig->b_data_tbl[n + i + 1]);
            WRITE_RKISP_REG(data, RK_ISP_LSC_B_TABLE_DATA);
        }

        data = RKISP_LSC_TABLE_DATA(
                   pconfig->r_data_tbl[n + RKISP_LSC_SECTORS_MAX],
                   /*isp_dev->lsc_config.r_data_tbl[n + i]*/0);
        WRITE_RKISP_REG(data, RK_ISP_LSC_R_TABLE_DATA);

        data = RKISP_LSC_TABLE_DATA(
                   pconfig->gr_data_tbl[n + RKISP_LSC_SECTORS_MAX],
                   /*isp_dev->lsc_config.gr_data_tbl[n + i]*/0);
        WRITE_RKISP_REG(data, RK_ISP_LSC_GR_TABLE_DATA);

        data = RKISP_LSC_TABLE_DATA(
                   pconfig->gb_data_tbl[n + RKISP_LSC_SECTORS_MAX],
                   /*isp_dev->lsc_config.gr_data_tbl[n + i]*/0);
        WRITE_RKISP_REG(data, RK_ISP_LSC_GB_TABLE_DATA);

        data = RKISP_LSC_TABLE_DATA(
                   pconfig->b_data_tbl[n + RKISP_LSC_SECTORS_MAX],
                   /*isp_dev->lsc_config.b_data_tbl[n + i]*/0);
        WRITE_RKISP_REG(data, RK_ISP_LSC_B_TABLE_DATA);
    }

    isp_lsc_table_sel = (isp_lsc_status & 0x2U) ? 0U : 1U;
    WRITE_RKISP_REG(isp_lsc_table_sel, RK_ISP_LSC_TABLE_SEL);
    return true;
}

static bool rt_rkisp_lsc_config(struct rt_rkisp_isp_dev *isp_dev)
{
    int i;
    unsigned int data;
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_LSC_ID].new_id;
    const struct rk_isp_lsc_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].lsc_config;

    if (pconfig->config_width != isp_dev->input_width ||
            pconfig->config_height != isp_dev->input_height)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "LSC config: lsc_w %d lsc_h %d act_w %d act_h %d\n",
                     pconfig->config_width,
                     pconfig->config_height,
                     isp_dev->input_width,
                     isp_dev->input_height);
        return false;
    }
    else
        RKISP_DPRINT(RKISP_DEBUG,
                     "LSC config: lsc_w %d lsc_h %d\n",
                     pconfig->config_width,
                     pconfig->config_height);

    /*To config must be off */
    WRITE_RKISP_REG(0, RK_ISP_LSC_CTRL);

    rt_rkisp_lsc_correct_matrix_config(isp_dev);

    for (i = 0; i < 4; i++)
    {
        /* program x size tables */
        data = RKISP_LSC_SECT_SIZE(
                   pconfig->x_size_tbl[i * 2],
                   pconfig->x_size_tbl[i * 2 + 1]);
        WRITE_RKISP_REG(data, RK_ISP_LSC_XSIZE_01 + i * 4);

        /* program x grad tables */
        data = RKISP_LSC_SECT_SIZE(
                   pconfig->x_grad_tbl[i * 2],
                   pconfig->x_grad_tbl[i * 2 + 1]);
        WRITE_RKISP_REG(data, RK_ISP_LSC_XGRAD_01 + i * 4);

        /* program y size tables */
        data = RKISP_LSC_SECT_SIZE(
                   pconfig->y_size_tbl[i * 2],
                   pconfig->y_size_tbl[i * 2 + 1]);
        WRITE_RKISP_REG(data, RK_ISP_LSC_YSIZE_01 + i * 4);

        /* program y grad tables */
        data = RKISP_LSC_SECT_SIZE(
                   pconfig->y_grad_tbl[i * 2],
                   pconfig->y_grad_tbl[i * 2 + 1]);
        WRITE_RKISP_REG(data, RK_ISP_LSC_YGRAD_01 + i * 4);
    }

    isp_dev->active_lsc_width = pconfig->config_width;
    isp_dev->active_lsc_height = pconfig->config_height;
    WRITE_RKISP_REG(1, RK_ISP_LSC_CTRL);

    return true;
}

static void rt_rkisp_lsc_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG(0, RK_ISP_LSC_CTRL);
}

/*ISP Filtering function*/
static int rt_rkisp_flt_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_flt_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_flt_config *curr_cfg, *new_cfg;
    int retval = RT_EOK;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_FLT_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].flt_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].flt_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_FLT))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_FLT);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_FLT) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    /* Parameter check */
    if (arg->mode > RKISP_FLT_MODE_MAX ||
            arg->grn_stage1 > RKISP_FLT_GREEN_STAGE1_MAX ||
            arg->chr_v_mode > RKISP_FLT_CHROMA_MODE_MAX ||
            arg->chr_h_mode > RKISP_FLT_CHROMA_MODE_MAX ||
            arg->thresh_sh0 & RKISP_FLT_THREAD_RESERVED ||
            arg->thresh_sh1 & RKISP_FLT_THREAD_RESERVED ||
            arg->thresh_bl0 & RKISP_FLT_THREAD_RESERVED ||
            arg->thresh_bl1 & RKISP_FLT_THREAD_RESERVED ||
            arg->fac_bl0 & RKISP_FLT_FAC_RESERVED ||
            arg->fac_bl1 & RKISP_FLT_FAC_RESERVED ||
            arg->fac_sh0 & RKISP_FLT_FAC_RESERVED ||
            arg->fac_sh1 & RKISP_FLT_FAC_RESERVED ||
            arg->fac_mid & RKISP_FLT_FAC_RESERVED ||
            arg->lum_weight & RKISP_FLT_LUM_WEIGHT_RESERVED)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_flt_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_FLT);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_flt_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_FLT_ID].new_id;
    const struct rk_isp_flt_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].flt_config;

    WRITE_RKISP_REG(pconfig->thresh_bl0,
                    RK_ISP_FILT_THRESH_BL0);
    WRITE_RKISP_REG(pconfig->thresh_bl1,
                    RK_ISP_FILT_THRESH_BL1);
    WRITE_RKISP_REG(pconfig->thresh_sh0,
                    RK_ISP_FILT_THRESH_SH0);
    WRITE_RKISP_REG(pconfig->thresh_sh1,
                    RK_ISP_FILT_THRESH_SH1);
    WRITE_RKISP_REG(pconfig->fac_bl0,
                    RK_ISP_FILT_FAC_BL0);
    WRITE_RKISP_REG(pconfig->fac_bl1,
                    RK_ISP_FILT_FAC_BL1);
    WRITE_RKISP_REG(pconfig->fac_mid,
                    RK_ISP_FILT_FAC_MID);
    WRITE_RKISP_REG(pconfig->fac_sh0,
                    RK_ISP_FILT_FAC_SH0);
    WRITE_RKISP_REG(pconfig->fac_sh1,
                    RK_ISP_FILT_FAC_SH1);
    WRITE_RKISP_REG(pconfig->lum_weight,
                    RK_ISP_FILT_LUM_WEIGHT);

    WRITE_RKISP_REG(RKISP_FLT_MODE(pconfig->mode) |
                    RKISP_FLT_CHROMA_V_MODE(pconfig->chr_v_mode) |
                    RKISP_FLT_CHROMA_H_MODE(pconfig->chr_h_mode) |
                    RKISP_FLT_GREEN_STAGE1(pconfig->grn_stage1), RK_ISP_FILT_MODE);
}

static void rt_rkisp_flt_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_OR(RKISP_FLT_ENA, RK_ISP_FILT_MODE);
}

static void rt_rkisp_flt_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG(RKISP_FLT_DIS, RK_ISP_FILT_MODE);
}

/* ISP demosaic interface function */
static int rt_rkisp_bdm_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_bdm_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_bdm_config *curr_cfg, *new_cfg;
    int retval = RT_EOK;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_BDM_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].bdm_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].bdm_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_BDM))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_BDM);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_BDM) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    /* Parameter Check */
    if (arg->demosaic_th > RK_ISP_BDM_MAX_TH)
    {
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_bdm_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_BDM);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_bdm_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_BDM_ID].new_id;
    const struct rk_isp_bdm_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].bdm_config;

    /*set demosaic threshold */
    WRITE_RKISP_REG(pconfig->demosaic_th, RK_ISP_DEMOSAIC);
}

static void rt_rkisp_bdm_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_AND(~(RKISP_BDM_BYPASS_EN(1)), RK_ISP_DEMOSAIC);
}

static void rt_rkisp_bdm_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG(0, RK_ISP_DEMOSAIC);
}

/* ISP GAMMA correction interface function */
static int rt_rkisp_sdg_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_sdg_config *arg)
{
    unsigned long lock_flags = 0;
    unsigned int i;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_sdg_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_SDG_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].sdg_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].sdg_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_SDG))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_SDG);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_SDG) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->xa_pnts.gamma_dx0 & RKISP_DEGAMMA_X_RESERVED ||
            arg->xa_pnts.gamma_dx1 & RKISP_DEGAMMA_X_RESERVED)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    for (i = 0; i < RK_ISP_DEGAMMA_CURVE_SIZE; i++)
    {
        if ((arg->curve_b.gamma_y[i] & RKISP_DEGAMMA_Y_RESERVED) ||
                (arg->curve_r.gamma_y[i] & RKISP_DEGAMMA_Y_RESERVED) ||
                (arg->curve_g.gamma_y[i] & RKISP_DEGAMMA_Y_RESERVED))
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible param in function: %s\n", __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_sdg_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_SDG);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_sdg_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_SDG_ID].new_id;
    const struct rk_isp_sdg_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].sdg_config;
    unsigned int i;

    WRITE_RKISP_REG(pconfig->xa_pnts.gamma_dx0, RK_ISP_GAMMA_DX_LO);
    WRITE_RKISP_REG(pconfig->xa_pnts.gamma_dx1, RK_ISP_GAMMA_DX_HI);

    for (i = 0; i < RK_ISP_DEGAMMA_CURVE_SIZE; i++)
    {
        WRITE_RKISP_REG(pconfig->curve_r.gamma_y[i], RK_ISP_GAMMA_R_Y0 + i * 4);
        WRITE_RKISP_REG(pconfig->curve_g.gamma_y[i], RK_ISP_GAMMA_G_Y0 + i * 4);
        WRITE_RKISP_REG(pconfig->curve_b.gamma_y[i], RK_ISP_GAMMA_B_Y0 + i * 4);
    }
}

static void rt_rkisp_sdg_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_GAMMA_IN_ENA, RK_ISP_CTRL);
}

static void rt_rkisp_sdg_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_AND(~RK_ISP_CTRL_ISP_GAMMA_IN_ENA, RK_ISP_CTRL);
}


/* ISP GAMMA correction interface function */
static int rt_rkisp_goc_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_goc_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_goc_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_GOC_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].goc_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].goc_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_GOC))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_GOC);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_GOC) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->mode > RKISP_GOC_MODE_MAX)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param 0x%x in  function: %s\n",
                     arg->mode, __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_goc_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_GOC);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif
    return retval;
}

static void rt_rkisp_goc_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    int i;
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_GOC_ID].new_id;
    const struct rk_isp_goc_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].goc_config;

    WRITE_RKISP_REG_AND(~RK_ISP_CTRL_ISP_GAMMA_OUT_ENA, RK_ISP_CTRL);

    WRITE_RKISP_REG(pconfig->mode, RK_ISP_GAMMA_OUT_MODE);
    for (i = 0; i < RK_ISP_GAMMA_OUT_MAX_SAMPLES / 2; i++)
        WRITE_RKISP_REG(pconfig->gamma_y[i],
                        RK_ISP_GAMMA_OUT_Y_0 + i * 4);
}

static void rt_rkisp_goc_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_GAMMA_OUT_ENA, RK_ISP_CTRL);
}

static void rt_rkisp_goc_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_AND(~RK_ISP_CTRL_ISP_GAMMA_OUT_ENA, RK_ISP_CTRL);
}


/* wdr interface function */
static int rt_rkisp_wdr_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_wdr_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_wdr_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_WDR_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].wdr_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].wdr_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_WDR))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_WDR);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_WDR) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->mode > RKISP_WDR_MODE_MAX)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param 0x%x in  function: %s\n",
                     arg->mode, __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_wdr_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_WDR);
end:

#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif
    return retval;
}

static void rt_rkisp_wdr_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    int i;
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_WDR_ID].new_id;
    const struct rk_isp_wdr_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].wdr_config;

    for (i = 0; i < RK_ISP_WDR_SIZE; i++)
    {
        if (i <= 39)
            WRITE_RKISP_REG(pconfig->c_wdr[i],
                            RK_ISP_WDR_CTRL + i * 4);
        else
            WRITE_RKISP_REG(pconfig->c_wdr[i],
                            RK_ISP_WDR_CTRL0 + (i - 40) * 4);
    }
}

static void rt_rkisp_wdr_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_OR(0x030cf1, RK_ISP_WDR_CTRL0);
}

static void rt_rkisp_wdr_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_AND(0x030cf0, RK_ISP_WDR_CTRL0);
}

/* ISP Cross Talk */
static int rt_rkisp_ctk_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_ctk_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_ctk_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_CTK_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].ctk_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].ctk_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_CTK))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_CTK);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_CTK) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    /* Perform parameter check */
    if (arg->coeff0 & RKISP_CTK_COEFF_RESERVED ||
            arg->coeff1 & RKISP_CTK_COEFF_RESERVED ||
            arg->coeff2 & RKISP_CTK_COEFF_RESERVED ||
            arg->coeff3 & RKISP_CTK_COEFF_RESERVED ||
            arg->coeff4 & RKISP_CTK_COEFF_RESERVED ||
            arg->coeff5 & RKISP_CTK_COEFF_RESERVED ||
            arg->coeff6 & RKISP_CTK_COEFF_RESERVED ||
            arg->coeff7 & RKISP_CTK_COEFF_RESERVED ||
            arg->coeff8 & RKISP_CTK_COEFF_RESERVED ||
            arg->ct_offset_r & RKISP_XTALK_OFFSET_RESERVED ||
            arg->ct_offset_g & RKISP_XTALK_OFFSET_RESERVED ||
            arg->ct_offset_b & RKISP_XTALK_OFFSET_RESERVED)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_ctk_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_CTK);
end:

#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_ctk_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_CTK_ID].new_id;
    const struct rk_isp_ctk_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].ctk_config;

    WRITE_RKISP_REG(pconfig->coeff0, RK_ISP_CT_COEFF_0);
    WRITE_RKISP_REG(pconfig->coeff1, RK_ISP_CT_COEFF_1);
    WRITE_RKISP_REG(pconfig->coeff2, RK_ISP_CT_COEFF_2);
    WRITE_RKISP_REG(pconfig->coeff3, RK_ISP_CT_COEFF_3);
    WRITE_RKISP_REG(pconfig->coeff4, RK_ISP_CT_COEFF_4);
    WRITE_RKISP_REG(pconfig->coeff5, RK_ISP_CT_COEFF_5);
    WRITE_RKISP_REG(pconfig->coeff6, RK_ISP_CT_COEFF_6);
    WRITE_RKISP_REG(pconfig->coeff7, RK_ISP_CT_COEFF_7);
    WRITE_RKISP_REG(pconfig->coeff8, RK_ISP_CT_COEFF_8);
    WRITE_RKISP_REG(pconfig->ct_offset_r, RK_ISP_CT_OFFSET_R);
    WRITE_RKISP_REG(pconfig->ct_offset_g, RK_ISP_CT_OFFSET_G);
    WRITE_RKISP_REG(pconfig->ct_offset_b, RK_ISP_CT_OFFSET_B);
}

static void rt_rkisp_ctk_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    /* Write back the default values. */
    WRITE_RKISP_REG(0x80, RK_ISP_CT_COEFF_0);
    WRITE_RKISP_REG(0, RK_ISP_CT_COEFF_1);
    WRITE_RKISP_REG(0, RK_ISP_CT_COEFF_2);
    WRITE_RKISP_REG(0, RK_ISP_CT_COEFF_3);
    WRITE_RKISP_REG(0x80, RK_ISP_CT_COEFF_4);
    WRITE_RKISP_REG(0, RK_ISP_CT_COEFF_5);
    WRITE_RKISP_REG(0, RK_ISP_CT_COEFF_6);
    WRITE_RKISP_REG(0, RK_ISP_CT_COEFF_7);
    WRITE_RKISP_REG(0x80, RK_ISP_CT_COEFF_8);

    WRITE_RKISP_REG(0, RK_ISP_CT_OFFSET_R);
    WRITE_RKISP_REG(0, RK_ISP_CT_OFFSET_G);
    WRITE_RKISP_REG(0, RK_ISP_CT_OFFSET_B);
}

/* Auto White Balance */
/* ISP White Balance Mode */
static int rt_rkisp_awb_meas_param(struct rt_rkisp_isp_dev *isp_dev,
                                   bool flag, struct rk_isp_awb_meas_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_awb_meas_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->meas_cfgs.log[RK_ISP_AWB_ID];
    curr_cfg = &(isp_dev->meas_cfgs.cfgs[cfg_log->curr_id].awb_meas_config);
    new_cfg = &(isp_dev->meas_cfgs.cfgs[cfg_log->new_id].awb_meas_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->meas_cfgs.module_actives,
                                 RK_ISP_MODULE_AWB))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_AWB);

    if (RKISP_MODULE_IS_EN(
                isp_dev->meas_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_AWB) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->awb_mode > RK_ISP_AWB_MODE_YCBCR ||
            arg->awb_wnd.h_offs > RKISP_AWB_WINDOW_OFFSET_MAX ||
            arg->awb_wnd.v_offs > RKISP_AWB_WINDOW_OFFSET_MAX ||
            arg->awb_wnd.h_size > RKISP_AWB_WINDOW_MAX_SIZE ||
            arg->awb_wnd.v_size > RKISP_AWB_WINDOW_MAX_SIZE ||
            arg->max_y > RKISP_AWB_THRES_MAX_YC ||
            arg->min_y > RKISP_AWB_THRES_MAX_YC ||
            arg->max_csum > RKISP_AWB_THRES_MAX_YC ||
            arg->min_c > RKISP_AWB_THRES_MAX_YC ||
            arg->frames > RK_ISP_AWB_MAX_FRAMES ||
            arg->awb_ref_cr > RKISP_AWB_CBCR_MAX_REF ||
            arg->awb_ref_cb > RKISP_AWB_CBCR_MAX_REF)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_awb_meas_config));
    RKISP_MODULE_UPDATE(
        isp_dev->meas_cfgs.module_updates,
        RK_ISP_MODULE_AWB);
end:

#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_awb_meas_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->meas_cfgs.log[RK_ISP_AWB_ID].new_id;
    const struct rk_isp_awb_meas_config *pconfig =
            &isp_dev->meas_cfgs.cfgs[new_id].awb_meas_config;
    unsigned int awb_prob = 0;

    /*based on the mode,configure the awb module */
    if (pconfig->awb_mode == RK_ISP_AWB_MODE_RGB)
    {
        awb_prob = RKISP_AWB_MODE_RGB_EN;
    }
    else
    {
        if (pconfig->enable_ymax_cmp)
            awb_prob = RKISP_AWB_YMAX_CMP_EN;

        /*Reference Cb and Cr */
        WRITE_RKISP_REG(RKISP_AWB_REF_CR_SET(pconfig->awb_ref_cr) |
                        pconfig->awb_ref_cb, RK_ISP_AWB_REF);
        /* Yc Threshold */
        WRITE_RKISP_REG(RKISP_AWB_MAX_Y_SET(pconfig->max_y) |
                        RKISP_AWB_MIN_Y_SET(pconfig->min_y) |
                        RKISP_AWB_MAX_CS_SET(pconfig->max_csum) |
                        pconfig->min_c, RK_ISP_AWB_THRESH);
    }

    /* Common Configuration */
    WRITE_RKISP_REG(awb_prob, RK_ISP_AWB_PROP);
    /*window offset */
    WRITE_RKISP_REG(pconfig->awb_wnd.v_offs,
                    RK_ISP_AWB_WND_V_OFFS);
    WRITE_RKISP_REG(pconfig->awb_wnd.h_offs,
                    RK_ISP_AWB_WND_H_OFFS);
    /*AWB window size */
    WRITE_RKISP_REG(pconfig->awb_wnd.v_size, RK_ISP_AWB_WND_V_SIZE);
    WRITE_RKISP_REG(pconfig->awb_wnd.h_size, RK_ISP_AWB_WND_H_SIZE);
    /*Number of frames */
    WRITE_RKISP_REG(pconfig->frames, RK_ISP_AWB_FRAMES);
}

static void rt_rkisp_awb_meas_en(struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->meas_cfgs.log[RK_ISP_AWB_ID].new_id;
    const struct rk_isp_awb_meas_config *pconfig =
            &isp_dev->meas_cfgs.cfgs[new_id].awb_meas_config;
    unsigned int reg_val = READ_RKISP_REG(RK_ISP_AWB_PROP);

    /* switch off */
    reg_val &= 0xFFFFFFFC;

    if (pconfig->awb_mode == RK_ISP_AWB_MODE_RGB)
        reg_val |= RKISP_AWB_MODE_RGB_EN;
    else
        reg_val |= RKISP_AWB_MODE_YCBCR_EN;

    WRITE_RKISP_REG(reg_val, RK_ISP_AWB_PROP);

    isp_dev->active_meas |= RK_ISP_AWB_DONE;

    /* Measurements require AWB block be active. */
    WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_AWB_ENA, RK_ISP_CTRL);
}

static void rt_rkisp_awb_meas_end(struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int reg_val = READ_RKISP_REG(RK_ISP_AWB_PROP);

    /* switch off */
    reg_val &= 0xFFFFFFFC;

    WRITE_RKISP_REG(reg_val, RK_ISP_AWB_PROP);

    isp_dev->active_meas &= ~RK_ISP_AWB_DONE;

    WRITE_RKISP_REG_AND(~RK_ISP_CTRL_ISP_AWB_ENA,
                        RK_ISP_CTRL);
}

static void rt_rkisp_get_awb_meas(struct rt_rkisp_isp_dev *isp_dev,
                                  struct rk_isp_stat_buffer *pbuf)
{
    /* Protect against concurrent access from ISR? */
    unsigned int reg_val;
    unsigned int curr_id =
        isp_dev->meas_cfgs.log[RK_ISP_AWB_ID].curr_id;
    const struct rk_isp_awb_meas_config *pconfig =
            &isp_dev->meas_cfgs.cfgs[curr_id].awb_meas_config;

    pbuf->meas_type |= RK_ISP_STAT_AWB;
    reg_val = READ_RKISP_REG(RK_ISP_AWB_WHITE_CNT);
    pbuf->params.awb.awb_mean[0].cnt =
        RKISP_AWB_GET_PIXEL_CNT(reg_val);
    reg_val = READ_RKISP_REG(RK_ISP_AWB_MEAN);

    if (pconfig->awb_mode == RK_ISP_AWB_MODE_RGB)
    {
        pbuf->params.awb.awb_mean[0].mean_r =
            RKISP_AWB_GET_MEAN_R(reg_val);
        pbuf->params.awb.awb_mean[0].mean_b =
            RKISP_AWB_GET_MEAN_B(reg_val);
        pbuf->params.awb.awb_mean[0].mean_g =
            RKISP_AWB_GET_MEAN_G(reg_val);
    }
    else
    {
        pbuf->params.awb.awb_mean[0].mean_cr =
            (unsigned char) RKISP_AWB_GET_MEAN_CR(reg_val);
        pbuf->params.awb.awb_mean[0].mean_cb =
            (unsigned char) RKISP_AWB_GET_MEAN_CB(reg_val);
        pbuf->params.awb.awb_mean[0].mean_y =
            (unsigned char) RKISP_AWB_GET_MEAN_Y(reg_val);
    }
}

static int rt_rkisp_awb_gain_param(struct rt_rkisp_isp_dev *isp_dev,
                                   bool flag, struct rk_isp_awb_gain_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_awb_gain_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_AWB_GAIN_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].awb_gain_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].awb_gain_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_AWB_GAIN))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_AWB_GAIN);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_AWB_GAIN) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->gain_red > RKISP_AWB_GAINS_MAX_VAL ||
            arg->gain_green_r > RKISP_AWB_GAINS_MAX_VAL ||
            arg->gain_green_b > RKISP_AWB_GAINS_MAX_VAL ||
            arg->gain_blue > RKISP_AWB_GAINS_MAX_VAL)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_awb_gain_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_AWB_GAIN);
end:

#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_awb_gain_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_AWB_GAIN_ID].new_id;
    unsigned int new_ens =
        isp_dev->other_cfgs.cfgs[new_id].module_ens;
    const struct rk_isp_awb_gain_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].awb_gain_config;

    if (RKISP_MODULE_IS_EN(new_ens, RK_ISP_MODULE_AWB_GAIN))
    {
        WRITE_RKISP_REG(RKISP_AWB_GAIN_R_SET(pconfig->gain_green_r) |
                        pconfig->gain_green_b, RK_ISP_AWB_GAIN_G);
        WRITE_RKISP_REG(RKISP_AWB_GAIN_R_SET(pconfig->gain_red) |
                        pconfig->gain_blue, RK_ISP_AWB_GAIN_RB);
    }
    else
    {
        WRITE_RKISP_REG(0x01000100, RK_ISP_AWB_GAIN_G);
        WRITE_RKISP_REG(0x01000100, RK_ISP_AWB_GAIN_RB);
    }
}

#ifdef LOG_CAPTURE_PARAMS
static void rt_rkisp_awb_gain_config_read(const struct rt_rkisp_isp_dev *isp_dev,
        struct rk_isp_awb_gain_config *pconfig)
{
    unsigned int reg = READ_RKISP_REG(RK_ISP_AWB_GAIN_G);

    pconfig->gain_green_r = RKISP_AWB_GAIN_R_READ(reg);
    pconfig->gain_green_b = RKISP_AWB_GAIN_B_READ(reg);
    reg = READ_RKISP_REG(RK_ISP_AWB_GAIN_RB);
    pconfig->gain_red = RKISP_AWB_GAIN_R_READ(reg);
    pconfig->gain_blue = RKISP_AWB_GAIN_B_READ(reg);
}
#endif

/* Auto Exposure */
/*****************************************************************************/
static int rt_rkisp_aec_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_aec_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_aec_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->meas_cfgs.log[RK_ISP_AEC_ID];
    curr_cfg = &(isp_dev->meas_cfgs.cfgs[cfg_log->curr_id].aec_config);
    new_cfg = &(isp_dev->meas_cfgs.cfgs[cfg_log->new_id].aec_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->meas_cfgs.module_actives,
                                 RK_ISP_MODULE_AEC))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_AEC);

    if (RKISP_MODULE_IS_EN(
                isp_dev->meas_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_AEC) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_aec_config));
    RKISP_MODULE_UPDATE(
        isp_dev->meas_cfgs.module_updates,
        RK_ISP_MODULE_AEC);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_aec_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->meas_cfgs.log[RK_ISP_AEC_ID].new_id;
    const struct rk_isp_aec_config *pconfig =
            &isp_dev->meas_cfgs.cfgs[new_id].aec_config;
    unsigned int block_hsize, block_vsize;

    WRITE_RKISP_REG(RKISP_EXP_CTRL_AUTOSTOP(pconfig->autostop) |
                    RKISP_EXP_CTRL_MEASMODE(pconfig->mode),
                    RK_ISP_EXP_CTRL);

    WRITE_RKISP_REG(pconfig->meas_window.h_offs, RK_ISP_EXP_H_OFFSET);
    WRITE_RKISP_REG(pconfig->meas_window.v_offs, RK_ISP_EXP_V_OFFSET);

    block_hsize = pconfig->meas_window.h_size /
                  RKISP_EXP_COLUMN_NUM - 1;
    block_vsize = pconfig->meas_window.v_size /
                  RKISP_EXP_ROW_NUM - 1;

    WRITE_RKISP_REG(RKISP_EXP_HSIZE(block_hsize), RK_ISP_EXP_H_SIZE);
    WRITE_RKISP_REG(RKISP_EXP_VSIZE(block_vsize), RK_ISP_EXP_V_SIZE);
}

static void rt_rkisp_aec_en(struct rt_rkisp_isp_dev *isp_dev)
{
    isp_dev->active_meas |= RK_ISP_EXP_END;

    WRITE_RKISP_REG_OR(RKISP_EXP_ENA, RK_ISP_EXP_CTRL);
}

static void rt_rkisp_aec_end(struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG(RKISP_EXP_DIS, RK_ISP_EXP_CTRL);

    isp_dev->active_meas &= ~RK_ISP_EXP_END;
}

static void rt_rkisp_get_aec_meas(struct rt_rkisp_isp_dev *isp_dev,
                                  struct rk_isp_stat_buffer *pbuf)
{
    unsigned int i;

    pbuf->meas_type |= RK_ISP_STAT_AUTOEXP; /*Set the measurement type */
    for (i = 0; i < RK_ISP_AE_MEAN_MAX; i++)
    {
        pbuf->params.ae.exp_mean[i] =
            (unsigned char) READ_RKISP_REG(RK_ISP_EXP_MEAN_00 + i * 4);
    }
}

/* CPROC */
/*****************************************************************************/
static int rt_rkisp_cproc_param(struct rt_rkisp_isp_dev *isp_dev,
                                bool flag, struct rk_isp_cproc_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_cproc_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_CPROC_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].cproc_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].cproc_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_CPROC))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_CPROC);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_CPROC) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->c_out_range & RKISP_CPROC_CTRL_RESERVED ||
            arg->y_out_range & RKISP_CPROC_CTRL_RESERVED ||
            arg->y_in_range & RKISP_CPROC_CTRL_RESERVED ||
            arg->contrast & RKISP_CPROC_CONTRAST_RESERVED ||
            arg->brightness & RKISP_CPROC_BRIGHTNESS_RESERVED ||
            arg->sat & RKISP_CPROC_SATURATION_RESERVED ||
            arg->hue & RKISP_CPROC_HUE_RESERVED)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_cproc_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_CPROC);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif
    return retval;
}


static void rt_rkisp_cproc_config(const struct rt_rkisp_isp_dev *isp_dev,
                                  enum rk_isp_pix_fmt_quantization quantization)
{
    unsigned int new_id_cproc =
        isp_dev->other_cfgs.log[RK_ISP_CPROC_ID].new_id;
    const struct rk_isp_cproc_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id_cproc].cproc_config;
    unsigned int curr_id =
        isp_dev->other_cfgs.log[RK_ISP_IE_ID].curr_id;
    const struct rk_isp_ie_config *ie_pconfig =
            &isp_dev->other_cfgs.cfgs[curr_id].ie_config;

    WRITE_RKISP_REG(pconfig->contrast, RK_C_PROC_CONTRAST);
    WRITE_RKISP_REG(pconfig->hue, RK_C_PROC_HUE);
    WRITE_RKISP_REG(pconfig->sat, RK_C_PROC_SATURATION);
    WRITE_RKISP_REG(pconfig->brightness, RK_C_PROC_BRIGHTNESS);

    if ((quantization != RK_ISP_QUANTIZATION_FULL_RANGE) ||
            (ie_pconfig->effect != RKISP_COLORFX_NONE))
    {
        WRITE_RKISP_REG_AND(
            ~(RK_C_PROC_YOUT_FULL |
              RK_C_PROC_YIN_FULL |
              RK_C_PROC_COUT_FULL),
            RK_C_PROC_CTRL);
    }
    else
    {
        WRITE_RKISP_REG_OR(
            (RK_C_PROC_YOUT_FULL |
             RK_C_PROC_YIN_FULL |
             RK_C_PROC_COUT_FULL),
            RK_C_PROC_CTRL);
    }
}

#ifdef LOG_CAPTURE_PARAMS
static void rt_rkisp_cproc_config_read(const struct rt_rkisp_isp_dev *isp_dev,
                                       struct rk_isp_cproc_config *pconfig)
{
    unsigned int reg;

    pconfig->contrast = READ_RKISP_REG(RK_C_PROC_CONTRAST);
    pconfig->hue = READ_RKISP_REG(RK_C_PROC_HUE);
    pconfig->sat = READ_RKISP_REG(RK_C_PROC_SATURATION);
    pconfig->brightness = READ_RKISP_REG(RK_C_PROC_BRIGHTNESS);
    reg = READ_RKISP_REG(RK_C_PROC_CTRL);
    pconfig->y_out_range = (reg >> 1) & 1;
    pconfig->y_in_range = (reg >> 2) & 1;
    pconfig->c_out_range = (reg >> 3) & 1;
}
#endif

static void rt_rkisp_cproc_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_OR(RKISP_CPROC_EN, RK_C_PROC_CTRL);
}

static void rt_rkisp_cproc_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_AND(~RKISP_CPROC_EN, RK_C_PROC_CTRL);
}

/* HISTOGRAM CALCULATION */
/*****************************************************************************/
static int rt_rkisp_hst_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_hst_config *arg)
{
    unsigned long lock_flags = 0;
    unsigned int i;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_hst_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->meas_cfgs.log[RK_ISP_HST_ID];
    curr_cfg = &(isp_dev->meas_cfgs.cfgs[cfg_log->curr_id].hst_config);
    new_cfg = &(isp_dev->meas_cfgs.cfgs[cfg_log->new_id].hst_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->meas_cfgs.module_actives,
                                 RK_ISP_MODULE_HST))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (RKISP_MODULE_IS_EN(
                isp_dev->meas_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_HST) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->mode > RK_ISP_HISTOGRAM_MODE_Y_HISTOGRAM ||
            arg->histogram_predivider > RKISP_MAX_HIST_PREDIVIDER ||
            arg->meas_window.v_offs & RKISP_HIST_WINDOW_OFFSET_RESERVED ||
            arg->meas_window.h_offs & RKISP_HIST_WINDOW_OFFSET_RESERVED ||
            (arg->meas_window.v_size / (RKISP_HIST_ROW_NUM - 1)) &
            RKISP_HIST_WINDOW_SIZE_RESERVED ||
            (arg->meas_window.h_size / (RKISP_HIST_COLUMN_NUM - 1)) &
            RKISP_HIST_WINDOW_SIZE_RESERVED)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s line: %d \n", __FUNCTION__, __LINE__);
        retval = -RT_EINVAL;
        goto end;
    }

    for (i = 0; i < RK_ISP_HISTOGRAM_WEIGHT_GRIDS_SIZE; i++)
    {
        if (arg->hist_weight[i] & RKISP_HIST_WEIGHT_RESERVED)
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible param in function: %s line: %d \n", __FUNCTION__, __LINE__);
            retval = -RT_EINVAL;
            goto end;
        }
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_hst_config));
    RKISP_MODULE_UPDATE(
        isp_dev->meas_cfgs.module_updates,
        RK_ISP_MODULE_HST);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_hst_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->meas_cfgs.log[RK_ISP_HST_ID].new_id;
    const struct rk_isp_hst_config *pconfig =
            &isp_dev->meas_cfgs.cfgs[new_id].hst_config;
    unsigned int block_hsize, block_vsize;

    WRITE_RKISP_REG(RKISP_HIST_PREDIV_SET(pconfig->histogram_predivider),
                    RK_ISP_HIST_PROP);
    WRITE_RKISP_REG(pconfig->meas_window.h_offs, RK_ISP_HIST_H_OFFS);
    WRITE_RKISP_REG(pconfig->meas_window.v_offs, RK_ISP_HIST_V_OFFS);

    block_hsize = pconfig->meas_window.h_size /
                  RKISP_HIST_COLUMN_NUM - 1;
    block_vsize = pconfig->meas_window.v_size /
                  RKISP_HIST_ROW_NUM - 1;

    WRITE_RKISP_REG(block_hsize, RK_ISP_HIST_H_SIZE);
    WRITE_RKISP_REG(block_vsize, RK_ISP_HIST_V_SIZE);

    WRITE_RKISP_REG(RKISP_HIST_WEIGHT_SET(pconfig->hist_weight[0],
                                          pconfig->hist_weight[1], pconfig->hist_weight[2],
                                          pconfig->hist_weight[3]), RK_ISP_HIST_WEIGHT_00TO30);
    WRITE_RKISP_REG(RKISP_HIST_WEIGHT_SET(pconfig->hist_weight[4],
                                          pconfig->hist_weight[5], pconfig->hist_weight[6],
                                          pconfig->hist_weight[7]), RK_ISP_HIST_WEIGHT_40TO21);
    WRITE_RKISP_REG(RKISP_HIST_WEIGHT_SET(pconfig->hist_weight[8],
                                          pconfig->hist_weight[9], pconfig->hist_weight[10],
                                          pconfig->hist_weight[11]), RK_ISP_HIST_WEIGHT_31TO12);
    WRITE_RKISP_REG(RKISP_HIST_WEIGHT_SET(pconfig->hist_weight[12],
                                          pconfig->hist_weight[13], pconfig->hist_weight[14],
                                          pconfig->hist_weight[15]), RK_ISP_HIST_WEIGHT_22TO03);
    WRITE_RKISP_REG(RKISP_HIST_WEIGHT_SET(pconfig->hist_weight[16],
                                          pconfig->hist_weight[17], pconfig->hist_weight[18],
                                          pconfig->hist_weight[19]), RK_ISP_HIST_WEIGHT_13TO43);
    WRITE_RKISP_REG(RKISP_HIST_WEIGHT_SET(pconfig->hist_weight[20],
                                          pconfig->hist_weight[21], pconfig->hist_weight[22],
                                          pconfig->hist_weight[23]), RK_ISP_HIST_WEIGHT_04TO34);
    WRITE_RKISP_REG(RKISP_HIST_WEIGHT_SET(pconfig->hist_weight[24],
                                          0, 0, 0), RK_ISP_HIST_WEIGHT_44);
}

static void rt_rkisp_hst_en(struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->meas_cfgs.log[RK_ISP_HST_ID].new_id;
    const struct rk_isp_hst_config *pconfig =
            &isp_dev->meas_cfgs.cfgs[new_id].hst_config;

    isp_dev->active_meas |= RK_ISP_HIST_MEASURE_RDY;

    WRITE_RKISP_REG_OR(pconfig->mode,
                       RK_ISP_HIST_PROP);
}

static void rt_rkisp_hst_end(struct rt_rkisp_isp_dev *isp_dev)
{
    /*Disable measurement */
    WRITE_RKISP_REG(RK_ISP_HISTOGRAM_MODE_DISABLE,
                    RK_ISP_HIST_PROP);

    isp_dev->active_meas &= ~RK_ISP_HIST_MEASURE_RDY;
}

static void rt_rkisp_get_hst_meas(const struct rt_rkisp_isp_dev *isp_dev,
                                  struct rk_isp_stat_buffer *pbuf)
{
    int i;

    pbuf->meas_type |= RK_ISP_STAT_HIST;
    for (i = 0; i < RK_ISP_HIST_BIN_N_MAX; i++)
    {
        pbuf->params.hist.hist_bins[i] =
            READ_RKISP_REG(RK_ISP_HIST_BIN_0 + (i * 4));
    }
}

/* AUTO FOCUS */
/*****************************************************************************/
static int rt_rkisp_afc_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_afc_config *arg)
{
    unsigned long lock_flags = 0;
    int i;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_afc_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->meas_cfgs.log[RK_ISP_AFC_ID];
    curr_cfg = &(isp_dev->meas_cfgs.cfgs[cfg_log->curr_id].afc_config);
    new_cfg = &(isp_dev->meas_cfgs.cfgs[cfg_log->new_id].afc_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->meas_cfgs.module_actives,
                                 RK_ISP_MODULE_AFC))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_AFC);

    if (RKISP_MODULE_IS_EN(
                isp_dev->meas_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_AFC) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    if (arg->num_afm_win > RK_ISP_AFM_MAX_WINDOWS ||
            arg->thres & RKISP_AFC_THRES_RESERVED ||
            arg->var_shift & RKISP_AFC_VAR_SHIFT_RESERVED)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    for (i = 0; i < arg->num_afm_win; i++)
    {
        if (arg->afm_win[i].h_offs & RKISP_AFC_WINDOW_X_RESERVED ||
                arg->afm_win[i].h_offs < RKISP_AFC_WINDOW_X_MIN ||
                arg->afm_win[i].v_offs & RKISP_AFC_WINDOW_Y_RESERVED ||
                arg->afm_win[i].v_offs < RKISP_AFC_WINDOW_Y_MIN ||
                arg->afm_win[i].h_size & RKISP_AFC_WINDOW_X_RESERVED ||
                arg->afm_win[i].v_size & RKISP_AFC_WINDOW_Y_RESERVED)
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible param in function: %s\n",
                         __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }

    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_afc_config));
    RKISP_MODULE_UPDATE(
        isp_dev->meas_cfgs.module_updates,
        RK_ISP_MODULE_AFC);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_afc_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->meas_cfgs.log[RK_ISP_AFC_ID].new_id;
    const struct rk_isp_afc_config *pconfig =
            &isp_dev->meas_cfgs.cfgs[new_id].afc_config;
    int num_of_win = pconfig->num_afm_win, i;

    /* Switch off to configure. Enabled during normal flow in frame isr. */
    WRITE_RKISP_REG(0, RK_ISP_AFM_CTRL);

    for (i = 0; i < num_of_win; i++)
    {
        WRITE_RKISP_REG(
            RKISP_AFC_WINDOW_X(pconfig->afm_win[i].h_offs) |
            RKISP_AFC_WINDOW_Y(pconfig->afm_win[i].v_offs),
            RK_ISP_AFM_LT_A + i * 8);
        WRITE_RKISP_REG(
            RKISP_AFC_WINDOW_X(pconfig->afm_win[i].h_size +
                               pconfig->afm_win[i].h_offs) |
            RKISP_AFC_WINDOW_Y(pconfig->afm_win[i].v_size +
                               pconfig->afm_win[i].v_offs),
            RK_ISP_AFM_RB_A + i * 8);
    }

    WRITE_RKISP_REG(pconfig->thres, RK_ISP_AFM_THRES);
    WRITE_RKISP_REG(pconfig->var_shift, RK_ISP_AFM_VAR_SHIFT);

}

static void rt_rkisp_afc_en(struct rt_rkisp_isp_dev *isp_dev)
{
    isp_dev->active_meas |= RK_ISP_AFM_FIN;
    WRITE_RKISP_REG(RKISP_AFC_ENA, RK_ISP_AFM_CTRL);
}

static void rt_rkisp_afc_end(struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG(RKISP_AFC_DIS, RK_ISP_AFM_CTRL);
    isp_dev->active_meas &= ~RK_ISP_AFM_FIN;
}

static void rt_rkisp_get_afc_meas(struct rt_rkisp_isp_dev *isp_dev,
                                  struct rk_isp_stat_buffer *pbuf)
{
    pbuf->meas_type |= RK_ISP_STAT_AFM_FIN;

    pbuf->params.af.window[0].sum =
        READ_RKISP_REG(RK_ISP_AFM_SUM_A);
    pbuf->params.af.window[0].lum =
        READ_RKISP_REG(RK_ISP_AFM_LUM_A);
    pbuf->params.af.window[1].sum =
        READ_RKISP_REG(RK_ISP_AFM_SUM_B);
    pbuf->params.af.window[1].lum =
        READ_RKISP_REG(RK_ISP_AFM_LUM_B);
    pbuf->params.af.window[2].sum =
        READ_RKISP_REG(RK_ISP_AFM_SUM_C);
    pbuf->params.af.window[2].lum =
        READ_RKISP_REG(RK_ISP_AFM_LUM_C);
}

/* IMAGE EFFECT */
/*****************************************************************************/
static int rt_rkisp_ie_param(struct rt_rkisp_isp_dev *isp_dev,
                             bool flag, struct rk_isp_ie_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_ie_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_IE_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].ie_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].ie_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_IE))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_IE);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_IE) &&
            rt_memcmp(arg,
                      curr_cfg,
                      sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "same param in function: %s, enable: 0x%x, curr_id: 0x%x, effect: %d\n", __FUNCTION__,
                     RKISP_MODULE_IS_EN(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens, RK_ISP_MODULE_IE),
                     cfg_log->curr_id,
                     arg->effect);
        goto end;
    }

    if (arg->effect != RKISP_COLORFX_NONE &&
            arg->effect != RKISP_COLORFX_BW &&
            arg->effect != RKISP_COLORFX_SEPIA &&
            arg->effect != RKISP_COLORFX_NEGATIVE &&
            arg->effect != RKISP_COLORFX_EMBOSS &&
            arg->effect != RKISP_COLORFX_SKETCH &&
            arg->effect != RKISP_COLORFX_AQUA &&
            arg->effect != RKISP_COLORFX_SET_CBCR)
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_ie_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_IE);
end:
#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif
    return retval;
}

static void rt_rkisp_ie_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_IE_ID].new_id;
    const struct rk_isp_ie_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].ie_config;

    switch (pconfig->effect)
    {
    case RKISP_COLORFX_SET_CBCR:
        WRITE_RKISP_REG(pconfig->eff_tint, RK_IMG_EFF_TINT);
        break;
    /*Color selection is similiar to water color(AQUA):
        grayscale + selected color w threshold*/
    case RKISP_COLORFX_AQUA:
        WRITE_RKISP_REG(pconfig->color_sel, RK_IMG_EFF_COLOR_SEL);
        break;
    case RKISP_COLORFX_EMBOSS:
        WRITE_RKISP_REG(pconfig->eff_mat_1, RK_IMG_EFF_MAT_1);
        WRITE_RKISP_REG(pconfig->eff_mat_2, RK_IMG_EFF_MAT_2);
        WRITE_RKISP_REG(pconfig->eff_mat_3, RK_IMG_EFF_MAT_3);
        break;
    case RKISP_COLORFX_SKETCH:
        WRITE_RKISP_REG(pconfig->eff_mat_3, RK_IMG_EFF_MAT_3);
        WRITE_RKISP_REG(pconfig->eff_mat_4, RK_IMG_EFF_MAT_4);
        WRITE_RKISP_REG(pconfig->eff_mat_5, RK_IMG_EFF_MAT_5);
        break;
    default:
        break;
    }
}

static void rt_rkisp_ie_en(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_IE_ID].new_id;
    const struct rk_isp_ie_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].ie_config;
    enum rk_isp_image_effect effect;

    switch (pconfig->effect)
    {
    case RKISP_COLORFX_SEPIA:
    case RKISP_COLORFX_SET_CBCR:
        effect = RK_ISP_IE_SEPIA;
        break;
    case RKISP_COLORFX_BW:
        effect = RK_ISP_IE_BW;
        break;
    case RKISP_COLORFX_NEGATIVE:
        effect = RK_ISP_IE_NEGATIVE;
        break;
    case RKISP_COLORFX_EMBOSS:
        effect = RK_ISP_IE_EMBOSS;
        break;
    case RKISP_COLORFX_SKETCH:
        effect = RK_ISP_IE_SKETCH;
        break;
    case RKISP_COLORFX_AQUA:
        effect = RK_ISP_IE_C_SEL;
        break;
    case RKISP_COLORFX_NONE:
    default:
        effect = RK_ISP_IE_NONE;
        break;
    }

    if (effect < RK_ISP_IE_NONE)
    {
        WRITE_RKISP_REG_OR(RK_ICCL_IE_CLK, RK_ICCL);
        WRITE_RKISP_REG(RK_IMG_EFF_CTRL_ENABLE |
                        effect << 1, RK_IMG_EFF_CTRL);
        WRITE_RKISP_REG_OR(RK_IMG_EFF_CTRL_CFG_UPD, RK_IMG_EFF_CTRL);
    }
    else if (effect == RK_ISP_IE_NONE)
    {
        WRITE_RKISP_REG_AND(~RK_IMG_EFF_CTRL_ENABLE, RK_IMG_EFF_CTRL);
        WRITE_RKISP_REG_AND(~RK_ICCL_IE_CLK, RK_ICCL);
    }
}

static void rt_rkisp_ie_end(const struct rt_rkisp_isp_dev *isp_dev)
{
    /* Disable measurement */
    WRITE_RKISP_REG_AND(~RK_IMG_EFF_CTRL_ENABLE, RK_IMG_EFF_CTRL);
    WRITE_RKISP_REG_AND(~RK_ICCL_IE_CLK, RK_ICCL);
}

/*ISP De-noise Pre-Filter(DPF) function*/
static int rt_rkisp_dpf_param(struct rt_rkisp_isp_dev *isp_dev,
                              bool flag, struct rk_isp_dpf_config *arg)
{
    unsigned long lock_flags = 0;
    unsigned int i;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_dpf_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_DPF_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].dpf_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].dpf_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_DPF))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_DPF);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_DPF) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    /* Parameter check */
    if ((arg->gain.mode >= RK_ISP_DPF_GAIN_USAGE_MAX) ||
            (arg->gain.mode < RK_ISP_DPF_GAIN_USAGE_DISABLED) ||
            (arg->gain.nf_b_gain & RKISP_DPF_NF_GAIN_RESERVED) ||
            (arg->gain.nf_r_gain & RKISP_DPF_NF_GAIN_RESERVED) ||
            (arg->gain.nf_gr_gain & RKISP_DPF_NF_GAIN_RESERVED) ||
            (arg->gain.nf_gb_gain & RKISP_DPF_NF_GAIN_RESERVED))
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible DPF GAIN param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    for (i = 0; i < RK_ISP_DPF_MAX_SPATIAL_COEFFS; i++)
    {
        if ((arg->g_flt.spatial_coeff[i] > RKISP_DPF_SPATIAL_COEFF_MAX))
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible DPF G Spatial param in function: %s\n", __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }

        if ((arg->rb_flt.spatial_coeff[i] > RKISP_DPF_SPATIAL_COEFF_MAX))
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible DPF RB Spatial param in function: %s\n", __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }
    }

    if ((arg->rb_flt.fltsize != RK_ISP_DPF_RB_FILTERSIZE_9x9) &&
            (arg->rb_flt.fltsize != RK_ISP_DPF_RB_FILTERSIZE_13x9))
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible DPF RB filter size param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    for (i = 0; i < RK_ISP_DPF_MAX_NLF_COEFFS; i++)
    {
        if (arg->nll.coeff[i] > RKISP_DPF_NLL_COEFF_N_MAX)
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "incompatible DPF NLL coeff param in function: %s\n", __FUNCTION__);
            retval = -RT_EINVAL;
            goto end;
        }
    }

    if ((arg->nll.scale_mode != RK_ISP_NLL_SCALE_LINEAR) &&
            (arg->nll.scale_mode != RK_ISP_NLL_SCALE_LOGARITHMIC))
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "incompatible DPF NLL scale mode param in function: %s\n", __FUNCTION__);
        retval = -RT_EINVAL;
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_dpf_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_DPF);
end:

#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static int rt_rkisp_dpf_strength_param(struct rt_rkisp_isp_dev *isp_dev,
                                       bool flag, struct rk_isp_dpf_strength_config *arg)
{
    unsigned long lock_flags = 0;
    struct rk_isp_cfgs_log *cfg_log;
    struct rk_isp_dpf_strength_config *curr_cfg, *new_cfg;
    int retval = 0;

    if (arg == NULL)
        return -RT_EINVAL;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    cfg_log = &isp_dev->other_cfgs.log[RK_ISP_DPF_STRENGTH_ID];
    curr_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->curr_id].dpf_strength_config);
    new_cfg = &(isp_dev->other_cfgs.cfgs[cfg_log->new_id].dpf_strength_config);

    if (flag == _GET_)
    {
        rt_memcpy(arg, curr_cfg, sizeof(*arg));
        goto end;
    }

    if (RKISP_MODULE_IS_UNACTIVE(isp_dev->other_cfgs.module_actives,
                                 RK_ISP_MODULE_DPF_STRENGTH))
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "module is unactive in function: %s\n", __FUNCTION__);
        goto end;
    }

    rkisp_param_dump(arg, RK_ISP_MODULE_DPF_STRENGTH);

    if (RKISP_MODULE_IS_EN(
                isp_dev->other_cfgs.cfgs[cfg_log->curr_id].module_ens,
                RK_ISP_MODULE_DPF_STRENGTH) &&
            rt_memcmp(arg, curr_cfg, sizeof(*arg)) == 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "same param in function: %s\n", __FUNCTION__);
        goto end;
    }

    rt_memcpy(new_cfg,
              arg,
              sizeof(struct rk_isp_dpf_strength_config));
    RKISP_MODULE_UPDATE(
        isp_dev->other_cfgs.module_updates,
        RK_ISP_MODULE_DPF_STRENGTH);
end:

#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

    return retval;
}

static void rt_rkisp_dpf_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_DPF_ID].new_id;
    const struct rk_isp_dpf_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].dpf_config;
    unsigned int isp_dpf_mode;
    unsigned int i;
    unsigned int spatial_coeff;

    isp_dpf_mode =
        READ_RKISP_REG(RK_ISP_DPF_MODE) & RKISP_DPF_MODE_EN;

    switch (pconfig->gain.mode)
    {
    case RK_ISP_DPF_GAIN_USAGE_DISABLED:
        break;
    case RK_ISP_DPF_GAIN_USAGE_NF_GAINS:
        isp_dpf_mode |= RKISP_DPF_MODE_USE_NF_GAIN |
                        RKISP_DPF_MODE_AWB_GAIN_COMP;
        break;
    case RK_ISP_DPF_GAIN_USAGE_LSC_GAINS:
        isp_dpf_mode |= RKISP_DPF_MODE_LSC_GAIN_COMP;
        break;
    case RK_ISP_DPF_GAIN_USAGE_NF_LSC_GAINS:
        isp_dpf_mode |= RKISP_DPF_MODE_USE_NF_GAIN |
                        RKISP_DPF_MODE_AWB_GAIN_COMP |
                        RKISP_DPF_MODE_LSC_GAIN_COMP;
        break;
    case RK_ISP_DPF_GAIN_USAGE_AWB_GAINS:
        isp_dpf_mode |= RKISP_DPF_MODE_AWB_GAIN_COMP;
        break;
    case RK_ISP_DPF_GAIN_USAGE_AWB_LSC_GAINS:
        isp_dpf_mode |= RKISP_DPF_MODE_LSC_GAIN_COMP |
                        RKISP_DPF_MODE_AWB_GAIN_COMP;
        break;
    default:
        break;
    }

    isp_dpf_mode |= RKISP_DPF_MODE_NLL_SEGMENTATION(pconfig->nll.scale_mode);
    isp_dpf_mode |= RKISP_DPF_MODE_RB_FLTSIZE(pconfig->rb_flt.fltsize);

    isp_dpf_mode |= (pconfig->rb_flt.r_enable) ?
                    RKISP_DPF_MODE_R_FLT_EN : RKISP_DPF_MODE_R_FLT_DIS;
    isp_dpf_mode |= (pconfig->rb_flt.b_enable) ?
                    RKISP_DPF_MODE_B_FLT_EN : RKISP_DPF_MODE_B_FLT_DIS;
    isp_dpf_mode |= (pconfig->g_flt.gb_enable) ?
                    RKISP_DPF_MODE_GB_FLT_EN : RKISP_DPF_MODE_GB_FLT_DIS;
    isp_dpf_mode |= (pconfig->g_flt.gr_enable) ?
                    RKISP_DPF_MODE_GR_FLT_EN : RKISP_DPF_MODE_GR_FLT_DIS;

    WRITE_RKISP_REG(isp_dpf_mode, RK_ISP_DPF_MODE);
    WRITE_RKISP_REG(pconfig->gain.nf_b_gain, RK_ISP_DPF_NF_GAIN_B);
    WRITE_RKISP_REG(pconfig->gain.nf_r_gain, RK_ISP_DPF_NF_GAIN_R);
    WRITE_RKISP_REG(pconfig->gain.nf_gb_gain, RK_ISP_DPF_NF_GAIN_GB);
    WRITE_RKISP_REG(pconfig->gain.nf_gr_gain, RK_ISP_DPF_NF_GAIN_GR);

    for (i = 0; i < RK_ISP_DPF_MAX_NLF_COEFFS; i++)
    {
        WRITE_RKISP_REG(pconfig->nll.coeff[i],
                        RK_ISP_DPF_NULL_COEFF_0 + i * 4);
    }

    spatial_coeff = pconfig->g_flt.spatial_coeff[0] |
                    ((unsigned int)pconfig->g_flt.spatial_coeff[1] << 8) |
                    ((unsigned int)pconfig->g_flt.spatial_coeff[2] << 16) |
                    ((unsigned int)pconfig->g_flt.spatial_coeff[3] << 24) ;
    WRITE_RKISP_REG(spatial_coeff, RK_ISP_DPF_S_WEIGHT_G_1_4);
    spatial_coeff = pconfig->g_flt.spatial_coeff[4] |
                    ((unsigned int)pconfig->g_flt.spatial_coeff[5] << 8);
    WRITE_RKISP_REG(spatial_coeff, RK_ISP_DPF_S_WEIGHT_G_5_6);
    spatial_coeff = pconfig->rb_flt.spatial_coeff[0] |
                    ((unsigned int)pconfig->rb_flt.spatial_coeff[1] << 8) |
                    ((unsigned int)pconfig->rb_flt.spatial_coeff[2] << 16) |
                    ((unsigned int)pconfig->rb_flt.spatial_coeff[3] << 24) ;
    WRITE_RKISP_REG(spatial_coeff, RK_ISP_DPF_S_WEIGHT_RB_1_4);
    spatial_coeff = pconfig->rb_flt.spatial_coeff[4] |
                    ((unsigned int)pconfig->rb_flt.spatial_coeff[5] << 8);
    WRITE_RKISP_REG(spatial_coeff, RK_ISP_DPF_S_WEIGHT_RB_5_6);
}

static void rt_rkisp_dpf_strength_config(const struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int new_id =
        isp_dev->other_cfgs.log[RK_ISP_DPF_STRENGTH_ID].new_id;
    const struct rk_isp_dpf_strength_config *pconfig =
            &isp_dev->other_cfgs.cfgs[new_id].dpf_strength_config;

    WRITE_RKISP_REG(pconfig->b, RK_ISP_DPF_STRENGTH_B);
    WRITE_RKISP_REG(pconfig->g, RK_ISP_DPF_STRENGTH_G);
    WRITE_RKISP_REG(pconfig->r, RK_ISP_DPF_STRENGTH_R);
}

static void rt_rkisp_dpf_en(struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_OR(RKISP_DPF_MODE_EN,
                       RK_ISP_DPF_MODE);
}

static void rt_rkisp_dpf_end(struct rt_rkisp_isp_dev *isp_dev)
{
    WRITE_RKISP_REG_AND(~RKISP_DPF_MODE_EN,
                        RK_ISP_DPF_MODE);
}

static int rt_rkisp_last_capture_config(struct rk_isp_last_capture_config *arg)
{
#ifdef LOG_CAPTURE_PARAMS
    if (arg == NULL)
        return -RT_EINVAL;

    rt_memcpy(arg, &g_last_capture_config, sizeof(*arg));

    return 0;
#else
    return -RT_ERROR;
#endif
}

static void rt_rkisp_csm_config(const struct rt_rkisp_isp_dev *isp_dev,
                                enum rk_isp_pix_fmt_quantization quantization)
{
    unsigned int curr_id_ie =
        isp_dev->other_cfgs.log[RK_ISP_IE_ID].curr_id;
    const struct rk_isp_ie_config *ie_pconfig =
            &isp_dev->other_cfgs.cfgs[curr_id_ie].ie_config;
    unsigned int curr_id_cproc =
        isp_dev->other_cfgs.log[RK_ISP_CPROC_ID].curr_id;
    unsigned int curr_cproc_en =
        isp_dev->other_cfgs.cfgs[curr_id_cproc].module_ens;

    if ((quantization != RK_ISP_QUANTIZATION_FULL_RANGE) ||
            ((ie_pconfig->effect != RKISP_COLORFX_NONE) &&
             RKISP_MODULE_IS_EN(
                 curr_cproc_en,
                 RK_ISP_MODULE_CPROC)))
    {
        /* Limit range conversion */
        WRITE_RKISP_REG(0x02100021, RK_ISP_CC_COEFF_0);
        WRITE_RKISP_REG(0x04000040, RK_ISP_CC_COEFF_1);
        WRITE_RKISP_REG(0x000d000d, RK_ISP_CC_COEFF_2);
        WRITE_RKISP_REG(0x01ed01ed, RK_ISP_CC_COEFF_3);
        WRITE_RKISP_REG(0x01db01db, RK_ISP_CC_COEFF_4);
        WRITE_RKISP_REG(0x00380038, RK_ISP_CC_COEFF_5);
        WRITE_RKISP_REG(0x00380038, RK_ISP_CC_COEFF_6);
        WRITE_RKISP_REG(0x01d101d1, RK_ISP_CC_COEFF_7);
        WRITE_RKISP_REG(0x01f701f7, RK_ISP_CC_COEFF_8);
        WRITE_RKISP_REG_AND(~RK_ISP_CTRL_ISP_CSM_Y_FULL_ENA,
                            RK_ISP_CTRL);
        WRITE_RKISP_REG_AND(~RK_ISP_CTRL_ISP_CSM_C_FULL_ENA,
                            RK_ISP_CTRL);
    }
    else
    {
        WRITE_RKISP_REG(0x02100026, RK_ISP_CC_COEFF_0);
        WRITE_RKISP_REG(0x0400004b, RK_ISP_CC_COEFF_1);
        WRITE_RKISP_REG(0x000d000f, RK_ISP_CC_COEFF_2);
        WRITE_RKISP_REG(0x01ed01ea, RK_ISP_CC_COEFF_3);
        WRITE_RKISP_REG(0x01db01d6, RK_ISP_CC_COEFF_4);
        WRITE_RKISP_REG(0x00380040, RK_ISP_CC_COEFF_5);
        WRITE_RKISP_REG(0x00380040, RK_ISP_CC_COEFF_6);
        WRITE_RKISP_REG(0x01d101ca, RK_ISP_CC_COEFF_7);
        WRITE_RKISP_REG(0x01f701f6, RK_ISP_CC_COEFF_8);
        WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CSM_Y_FULL_ENA,
                           RK_ISP_CTRL);
        WRITE_RKISP_REG_OR(RK_ISP_CTRL_ISP_CSM_C_FULL_ENA,
                           RK_ISP_CTRL);
    }
}

int rt_do_rkisp_get_mod(struct rt_rkisp_isp_dev *isp_dev, int *value, unsigned int mod_id)
{
    if (mod_id < 0 || mod_id > RK_ISP_MODULE_MAX)
        return -RT_EINVAL;
    return rt_rkisp_module_enable(
               isp_dev,
               _GET_,
               value,
               mod_id);

}

int rt_do_rkisp_set_mod(struct rt_rkisp_isp_dev *isp_dev, int *value, unsigned int mod_id)
{

    if (mod_id < 0 || mod_id > RK_ISP_MODULE_MAX)
        return -RT_EINVAL;
    return rt_rkisp_module_enable(
               isp_dev,
               _SET_,
               value,
               mod_id);
}

int rt_do_cmd_ctrl_rkisp_module(struct rt_rkisp_isp_dev *isp, unsigned int cmd, void *arg)
{

    switch (cmd)
    {
    case RK_ISP_CMD_G_DPCC:
        return rt_rkisp_dpcc_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_DPCC:
        return rt_rkisp_dpcc_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_BLS:
        return rt_rkisp_bls_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_BLS:
        return rt_rkisp_bls_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_SDG:
        return rt_rkisp_sdg_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_SDG:
        return rt_rkisp_sdg_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_LSC:
        return rt_rkisp_lsc_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_LSC:
        return rt_rkisp_lsc_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_AWB_MEAS:
        return rt_rkisp_awb_meas_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_AWB_MEAS:
        return rt_rkisp_awb_meas_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_AWB_GAIN:
        return rt_rkisp_awb_gain_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_AWB_GAIN:
        return rt_rkisp_awb_gain_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_FLT:
        return rt_rkisp_flt_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_FLT:
        return rt_rkisp_flt_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_BDM:
        return rt_rkisp_bdm_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_BDM:
        return rt_rkisp_bdm_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_CTK:
        return rt_rkisp_ctk_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_CTK:
        return rt_rkisp_ctk_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_GOC:
        return rt_rkisp_goc_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_GOC:
        return rt_rkisp_goc_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_WDR:
        return rt_rkisp_wdr_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_WDR:
        return rt_rkisp_wdr_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_HST:
        return rt_rkisp_hst_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_HST:
        return rt_rkisp_hst_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_AEC:
        return rt_rkisp_aec_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_AEC:
        return rt_rkisp_aec_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_CPROC:
        return rt_rkisp_cproc_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_CPROC:
        return rt_rkisp_cproc_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_AFC:
        return rt_rkisp_afc_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_AFC:
        return rt_rkisp_afc_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_IE:
        return rt_rkisp_ie_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_IE:
        return rt_rkisp_ie_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_DPF:
        return rt_rkisp_dpf_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_DPF:
        return rt_rkisp_dpf_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_DPF_STRENGTH:
        return rt_rkisp_dpf_strength_param(isp, _GET_, arg);
    case RK_ISP_CMD_S_DPF_STRENGTH:
        return rt_rkisp_dpf_strength_param(isp, _SET_, arg);
    case RK_ISP_CMD_G_LAST_CONFIG:
        return rt_rkisp_last_capture_config(arg);
    default:
        return -RT_EINVAL;
    }
}

static int rt_do_rkisp_reset(struct rt_rkisp_isp_dev *isp_dev)
{

    rt_memset(&isp_dev->other_cfgs, 0, sizeof(struct rk_isp_other_cfg));
    rt_memset(&isp_dev->meas_cfgs, 0, sizeof(struct rk_isp_meas_cfg));
    isp_dev->other_cfgs.module_updates = 0;
    isp_dev->meas_cfgs.module_updates = 0;

    isp_dev->active_lsc_width = 0;
    isp_dev->active_lsc_height = 0;

    isp_dev->active_meas = 0;
    isp_dev->frame_id = 0;
    isp_dev->rk_ism_cropping = false;

    isp_dev->meas_send_alone = RKISP_MEAS_SEND_ALONE;
    isp_dev->awb_meas_ready = false;
    isp_dev->afm_meas_ready = false;
    isp_dev->aec_meas_ready = false;
    isp_dev->hst_meas_ready = false;
    return 0;
}

int rt_do_rkisp_open(struct rt_rkisp_isp_dev *isp_dev)
{
    ISP_DBG("isp_module_open\n");

    rt_do_rkisp_reset(isp_dev);

    return 0;
}

void rt_do_dump_rkisp_reg(struct rt_rkisp_isp_dev *isp_dev, int level, int active_flag)
{
    if (active_flag & RK_ISP_MODULE_DPCC)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_DPCC, level);

    if (active_flag & RK_ISP_MODULE_LSC)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_LSC, level);

    if (active_flag & RK_ISP_MODULE_BLS)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_BLS, level);

    if (active_flag & RK_ISP_MODULE_SDG)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_SDG, level);

    if (active_flag & RK_ISP_MODULE_GOC)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_GOC, level);

    if (active_flag & RK_ISP_MODULE_BDM)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_BDM, level);

    if (active_flag & RK_ISP_MODULE_FLT)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_FLT, level);

    if (active_flag & RK_ISP_MODULE_AWB || active_flag & RK_ISP_MODULE_AWB_GAIN)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_AWB, level);

    if (active_flag & RK_ISP_MODULE_AEC)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_AEC, level);

    if (active_flag & RK_ISP_MODULE_CTK)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_CTK, level);

    if (active_flag & RK_ISP_MODULE_CPROC)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_CPROC, level);

    if (active_flag & RK_ISP_MODULE_AFC)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_AFC, level);

    if (active_flag & RK_ISP_MODULE_HST)
        rkisp_reg_dump(isp_dev, RK_ISP_MODULE_HST, level);
}

static void rt_do_set_rkisp_modules_actives(struct rt_rkisp_isp_dev *isp_dev,
        enum rk_isp_pix_fmt in_pix_fmt)
{
    isp_dev->other_cfgs.module_actives = 0;
    isp_dev->meas_cfgs.module_actives = 0;
    if (RKISP_PIX_FMT_IS_RAW_BAYER(in_pix_fmt))
    {
        /* unlimited */
    }
    else if (RKISP_PIX_FMT_IS_Y_ONLY(in_pix_fmt))
    {
        RKISP_MODULE_UNACTIVE(
            isp_dev->other_cfgs.module_actives,
            RK_ISP_MODULE_LSC |
            RK_ISP_MODULE_AWB_GAIN |
            RK_ISP_MODULE_BDM |
            RK_ISP_MODULE_CTK);

        RKISP_MODULE_UNACTIVE(
            isp_dev->meas_cfgs.module_actives,
            RK_ISP_MODULE_AWB);
    }
    else
    {
        RKISP_MODULE_UNACTIVE(
            isp_dev->other_cfgs.module_actives,
            RK_ISP_MODULE_DPCC |
            RK_ISP_MODULE_BLS |
            RK_ISP_MODULE_SDG |
            RK_ISP_MODULE_LSC |
            RK_ISP_MODULE_AWB_GAIN |
            RK_ISP_MODULE_FLT |
            RK_ISP_MODULE_BDM |
            RK_ISP_MODULE_CTK |
            RK_ISP_MODULE_GOC |
            RK_ISP_MODULE_CPROC |
            RK_ISP_MODULE_IE |
            RK_ISP_MODULE_WDR |
            RK_ISP_MODULE_DPF |
            RK_ISP_MODULE_DPF_STRENGTH);

        RKISP_MODULE_UNACTIVE(
            isp_dev->meas_cfgs.module_actives,
            RK_ISP_MODULE_HST |
            RK_ISP_MODULE_AFC |
            RK_ISP_MODULE_AWB |
            RK_ISP_MODULE_AEC);
    }
}

/* Not called when the camera active, thus not isr protection. */
void rt_do_configure_rkisp(
    struct rt_rkisp_isp_dev *isp_dev,
    enum rk_isp_pix_fmt in_pix_fmt,
    enum rk_isp_pix_fmt_quantization quantization)
{
    unsigned int time_left = 3000;
    unsigned int i, curr_id;

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    isp_dev->quantization = quantization;

    rt_do_set_rkisp_modules_actives(isp_dev, in_pix_fmt);

    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_THRESHBLO,
                    RK_ISP_FILT_THRESH_BL0);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_THRESHBL1,
                    RK_ISP_FILT_THRESH_BL1);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_THRESHSH0,
                    RK_ISP_FILT_THRESH_SH0);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_THRESHSH1,
                    RK_ISP_FILT_THRESH_SH1);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_FACBL0,
                    RK_ISP_FILT_FAC_BL0);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_FACBL1,
                    RK_ISP_FILT_FAC_BL1);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_FACMID,
                    RK_ISP_FILT_FAC_MID);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_FACSH0,
                    RK_ISP_FILT_FAC_SH0);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_FACSH1,
                    RK_ISP_FILT_FAC_SH1);
    WRITE_RKISP_REG(RKISP_FLT_INIT_PARAM_LUMWEIGHT,
                    RK_ISP_FILT_LUM_WEIGHT);

    WRITE_RKISP_REG(RKISP_FLT_MODE(RKISP_FLT_INIT_PARAM_MODE) |
                    RKISP_FLT_CHROMA_V_MODE(RKISP_FLT_INIT_PARAM_CHRVMODE) |
                    RKISP_FLT_CHROMA_H_MODE(RKISP_FLT_INIT_PARAM_CHRHMODE) |
                    RKISP_FLT_GREEN_STAGE1(RKISP_FLT_INIT_PARAM_GRNSTAGE), RK_ISP_FILT_MODE);
    WRITE_RKISP_REG_OR(RKISP_FLT_ENA, RK_ISP_FILT_MODE);

    /*
     * Must config isp, Hardware may has been reseted.
     */
    for (i = 0; i < RK_ISP_MEAS_ID; i++)
    {
        if (RKISP_MODULE_IS_UNACTIVE(
                    isp_dev->other_cfgs.module_actives,
                    (1 << i)))
            continue;

        if (RKISP_MODULE_IS_UPDATE(
                    isp_dev->other_cfgs.module_updates,
                    (1 << i)))
            continue;

        curr_id = isp_dev->other_cfgs.log[i].curr_id;
        if (RKISP_MODULE_IS_EN(
                    isp_dev->other_cfgs.cfgs[curr_id].module_ens,
                    (1 << i)))
        {
            isp_dev->other_cfgs.log[i].new_id = curr_id;
            RKISP_MODULE_UPDATE(
                isp_dev->other_cfgs.module_updates,
                (1 << i));

            if (i == RK_ISP_DPF_ID)
            {
                isp_dev->other_cfgs.log[RK_ISP_DPF_STRENGTH_ID].new_id = curr_id;
                RKISP_MODULE_UPDATE(
                    isp_dev->other_cfgs.module_updates,
                    (1 << RK_ISP_DPF_STRENGTH_ID));
            }
        }
    }

    for (i = RK_ISP_MEAS_ID; i < RK_ISP_MODULE_MAX; i++)
    {
        if (RKISP_MODULE_IS_UNACTIVE(
                    isp_dev->meas_cfgs.module_actives,
                    (1 << i)))
            continue;

        if (RKISP_MODULE_IS_UPDATE(
                    isp_dev->meas_cfgs.module_updates,
                    (1 << i)))
            continue;

        curr_id = isp_dev->meas_cfgs.log[i].curr_id;
        if (RKISP_MODULE_IS_EN(
                    isp_dev->meas_cfgs.cfgs[curr_id].module_ens,
                    (1 << i)))
        {
            isp_dev->meas_cfgs.log[i].new_id = curr_id;
            RKISP_MODULE_UPDATE(
                isp_dev->meas_cfgs.module_updates,
                (1 << i));
        }
    }

    rt_do_set_rkisp_isr_other_config(isp_dev, &time_left);
    rt_rkisp_csm_config(isp_dev, quantization);
    rt_do_set_rkisp_isr_meas_config(isp_dev, &time_left);


#ifdef RKISP_DEBUG_REG
    rkisp_dump_reg(isp_dev, RKISP_DEBUG);
#endif

#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif
}

void rt_do_rkisp_frame_in(
    struct rt_rkisp_isp_dev *isp_dev,
    unsigned int *fi_t)
{
    unsigned int write_id;
    /* Called in an interrupt context. */
    isp_dev->fi_t = *fi_t;

}

void rt_do_rkisp_v_start(struct rt_rkisp_isp_dev *isp_dev, unsigned int *timestamp)
{
    unsigned int write_id;
    /* Called in an interrupt context. */
    isp_dev->frame_id++;
    isp_dev->vs_t = *timestamp;
}

void rt_do_rkisp_frame_id_reset(
    struct rt_rkisp_isp_dev *isp_dev)
{
    unsigned int i;

    isp_dev->frame_id = 0;
    for (i = 0; i < RK_ISP_MEAS_ID; i++)
    {
        rt_memset(isp_dev->other_cfgs.log[i].s_frame_id,
                  0x00,
                  sizeof(isp_dev->other_cfgs.log[i].s_frame_id));
    }
    for (i = RK_ISP_MEAS_ID; i < RK_ISP_MODULE_MAX; i++)
    {
        rt_memset(isp_dev->meas_cfgs.log[i].s_frame_id,
                  0x00,
                  sizeof(isp_dev->meas_cfgs.log[i].s_frame_id));
    }
}

/* Not called when the camera active, thus not isr protection. */
void rt_do_disable_rkisp(struct rt_rkisp_isp_dev *isp_dev)
{
    RKISP_DPRINT(RKISP_DEBUG, "%s\n", __FUNCTION__);

#if defined(RT_USING_ISP_MUTEX)
    rt_base_t level;
    level = rt_mutex_take(isp_dev->isp_mutex, RT_WAITING_FOREVER);
#endif

    rt_rkisp_dpcc_end(isp_dev);
    rt_rkisp_lsc_end(isp_dev);
    rt_rkisp_bls_end(isp_dev);
    rt_rkisp_sdg_end(isp_dev);
    rt_rkisp_goc_end(isp_dev);
    rt_rkisp_bdm_end(isp_dev);
    rt_rkisp_flt_end(isp_dev);
    rt_rkisp_awb_meas_end(isp_dev);
    rt_rkisp_aec_end(isp_dev);
    rt_rkisp_ctk_end(isp_dev);
    rt_rkisp_cproc_end(isp_dev);
    rt_rkisp_hst_end(isp_dev);
    rt_rkisp_afc_end(isp_dev);
    rt_rkisp_ie_end(isp_dev);
    rt_rkisp_dpf_end(isp_dev);
    /*
    Isp isn't active, isp interrupt isn't enabled, spin_lock is enough;
    */

    rt_memset(&isp_dev->other_cfgs, 0, sizeof(struct rk_isp_other_cfg));
    rt_memset(&isp_dev->meas_cfgs, 0, sizeof(struct rk_isp_meas_cfg));
    isp_dev->other_cfgs.module_updates = 0;
    isp_dev->meas_cfgs.module_updates = 0;


#if defined(RT_USING_ISP_MUTEX)
    rt_mutex_release(isp_dev->isp_mutex);
#endif

}

static void rt_do_rkisp_convert_hdrae_data(
    unsigned int *msg,
    struct rk_isp_hdrae_result *hdrae_res)
{
    int i, j;
    unsigned int *hist;
    unsigned short *y_meas;

    if (msg[1] != (RK_ISP_GAMMA_OUT_OFFSET * 4))
    {
        RKISP_DPRINT(RKISP_ERROR,
                     "msg size error 0x%x\n", msg[1]);
        return;
    }

    hdrae_res->mesg_id = msg[0];
    hdrae_res->mesg_size = msg[1];

    for (j = 0; j < RK_ISP_HDRAE_MAXFRAMES; j++)
    {
        if (j == 0)
        {
            hist = &msg[RK_ISP_HDRAE_1_FRAME_OFFSET];
            y_meas = (unsigned short *)&msg[RK_ISP_HDRAE_1_FRAME_OFFSET +
                                                                        RK_ISP_HDRAE_HIST_BIN_NUM];
        }
        else if (j == 1)
        {
            hist = &msg[RK_ISP_HDRAE_2_FRAME_OFFSET];
            y_meas = (unsigned short *)&msg[RK_ISP_HDRAE_2_FRAME_OFFSET +
                                                                        RK_ISP_HDRAE_HIST_BIN_NUM];
        }
        else
        {
            hist = &msg[RK_ISP_HDRAE_3_FRAME_OFFSET];
            y_meas = (unsigned short *)&msg[RK_ISP_HDRAE_3_FRAME_OFFSET +
                                                                        RK_ISP_HDRAE_HIST_BIN_NUM];
        }

        for (i = 0; i < RK_ISP_HDRAE_HIST_BIN_NUM; i++)
            hdrae_res->oneframe[j].hist_meas.hist_bin[i] = hist[i];

        for (i = 0; i < RK_ISP_HDRAE_MAXGRIDITEMS; i++)
            hdrae_res->oneframe[j].mean_meas.y_meas[i] = y_meas[i];
    }

    hdrae_res->OEMeasRes.OE_Pixel =
        msg[RK_ISP_HDRAE_NEW_DATA_START];
    hdrae_res->OEMeasRes.SumHistPixel =
        msg[RK_ISP_HDRAE_NEW_DATA_START + 1];
    hdrae_res->OEMeasRes.SframeMaxLuma =
        msg[RK_ISP_HDRAE_NEW_DATA_START + 2];

    hdrae_res->DRIndexRes.fNormalIndex =
        msg[RK_ISP_HDRAE_NEW_DATA_START + 3];
    hdrae_res->DRIndexRes.fLongIndex =
        msg[RK_ISP_HDRAE_NEW_DATA_START + 4];

    /* the expose val that rk1608 pass back */
    rt_memcpy(&hdrae_res->sensor,
              (struct sensor_metadata_s *)&msg[RK_ISP_HDRAE_EXP_DATA_OFFSET],
              RK_ISP_HDRAE_EXP_DATA_NUM);
    hdrae_res->lgmean = msg[RK_ISP_HDRAE_EXP_LGMEAN_OFFSET];

    RKISP_DPRINT(RKISP_DEBUG, "sensor time: [%d, %d, %d], gain: [%d, %d, %d]\n",
                 hdrae_res->sensor.exp_time_l,
                 hdrae_res->sensor.exp_time,
                 hdrae_res->sensor.exp_time_s,
                 hdrae_res->sensor.gain_l,
                 hdrae_res->sensor.gain,
                 hdrae_res->sensor.gain_s);
}

static void rt_do_send_rkisp_measurement(
    struct rt_rkisp_isp_dev *isp_dev, unsigned int send_meas)
{
    unsigned int active_meas = send_meas;
    unsigned int meas_type_temp = 0;
    struct rk_isp_stat_buffer *stat_buf;
    struct rk_isp_device *rt_isp_dev =
        rt_container_of(isp_dev, struct rk_isp_device, isp_dev);
    long ret;

    struct rk_isp_metadata *isp_metadata = &rt_isp_dev->isp_buf_metadata[rt_isp_dev->buf_index].isp_meta;
    stat_buf = &isp_metadata->meas_stat;
    isp_metadata->frame_id = isp_dev->frame_id;

    if (active_meas & RK_ISP_AWB_DONE)
    {
        rt_memcpy(&stat_buf->params.awb,
                  &isp_dev->meas_stats.stat.params.awb,
                  sizeof(struct rk_isp_awb_stat));
        meas_type_temp |= RK_ISP_STAT_AWB;
        isp_dev->awb_meas_ready = false;
    }

    if (active_meas & RK_ISP_AFM_FIN)
    {
        rt_memcpy(&stat_buf->params.af,
                  &isp_dev->meas_stats.stat.params.af,
                  sizeof(struct rk_isp_af_stat));
        meas_type_temp |= RK_ISP_STAT_AFM_FIN;
        isp_dev->afm_meas_ready = false;
    }

    if (active_meas & RK_ISP_EXP_END)
    {
        rt_memcpy(&stat_buf->params.ae,
                  &isp_dev->meas_stats.stat.params.ae,
                  sizeof(struct rk_isp_ae_stat));

        rt_do_sync_rkisp_sensor_mode_data(rt_isp_dev,
                                          isp_dev->frame_id,
                                          &stat_buf->sensor_mode);

        meas_type_temp |= RK_ISP_STAT_AUTOEXP;
        isp_dev->aec_meas_ready = false;
    }

    if (active_meas & RK_ISP_HIST_MEASURE_RDY)
    {
        rt_memcpy(&stat_buf->params.hist,
                  &isp_dev->meas_stats.stat.params.hist,
                  sizeof(struct rk_isp_hist_stat));
        meas_type_temp |= RK_ISP_STAT_HIST;
        isp_dev->hst_meas_ready = false;
    }

    stat_buf->meas_type |= meas_type_temp;
    stat_buf->vs_t = isp_dev->vs_t;
    stat_buf->fi_t = isp_dev->fi_t;

#if defined(RT_USING_VCM)
    struct rk_vcm_dev *vcm;
    vcm = (struct rk_vcm_dev *)rt_device_find("vcm-0");
    if (vcm == NULL)
    {
        rt_kprintf("get vcm device handle failed\n");
        return;
    }
    vcm->ops->control(vcm, RK_GET_VCM_MOVE_RES, &stat_buf->vcm_move);
#endif
    ISP_DBG("%s : [%x %x %x %x]\n", __FUNCTION__, stat_buf->vs_t, stat_buf->fi_t,
            stat_buf->vcm_move.start_move_tv, stat_buf->vcm_move.end_move_tv);

    ISP_DBG(
        "Measurement done(%d, %d) buf_index %d\n",
        isp_dev->frame_id,
        stat_buf->meas_type,
        rt_isp_dev->buf_index);

}

int rt_do_rkisp_vb_metadata(
    struct rt_rkisp_isp_dev *isp_dev)
{
    struct rk_isp_device *rk_isp_dev =
        rt_container_of(isp_dev, struct rk_isp_device, isp_dev);
    struct rk_isp_metadata *isp_metadata = (struct rk_isp_metadata *)
                                           &rk_isp_dev->isp_buf_metadata[rk_isp_dev->buf_index].isp_meta;
#if 0
    struct rk_isp_other_cfg *other_cfg =
            &isp_metadata->other_cfg;
    struct rk_isp_meas_cfg *meas_cfg =
            &isp_metadata->meas_cfg;
#endif
    struct rk_isp_stat_buffer *stat_new =
            &isp_metadata->meas_stat;
    unsigned int i, j, match_id;
    unsigned long int lock_flags;

    rt_do_sync_rkisp_sensor_mode_data(rk_isp_dev,
                                      isp_dev->frame_id,
                                      &isp_dev->meas_stats.stat.sensor_mode);

#if 0
    other_cfg->module_ens = 0;
    for (i = 0; i < RK_ISP_MEAS_ID; i++)
    {
        match_id = 0xff;
        for (j = 0; j < 3; j++)
        {
            if (isp_dev->frame_id >=
                    isp_dev->other_cfgs.log[i].s_frame_id[j])
            {
                if (match_id == 0xff)
                    match_id = j;
                else if (isp_dev->other_cfgs.log[i].s_frame_id[match_id] <
                         isp_dev->other_cfgs.log[i].s_frame_id[j])
                    match_id = j;
            }
        }

        if (match_id == 0xff)
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "%s: FrameID:%d isp other config haven't found! s_frame_id:%d, %d,%d\n",
                         __FUNCTION__,
                         readout_work->frame_id,
                         isp_dev->other_cfgs.log[i].s_frame_id[0],
                         isp_dev->other_cfgs.log[i].s_frame_id[1],
                         isp_dev->other_cfgs.log[i].s_frame_id[2]);
            match_id = isp_dev->other_cfgs.log[i].curr_id;
        }

        other_cfg->module_ens |=
            (isp_dev->other_cfgs.cfgs[match_id].module_ens &
             (1 << i));

        switch (i)
        {
        case RK_ISP_DPCC_ID:
            rt_memcpy(&other_cfg->dpcc_config,
                      &isp_dev->other_cfgs.cfgs[match_id].dpcc_config,
                      sizeof(other_cfg->dpcc_config));
            break;
        case RK_ISP_BLS_ID:
            rt_memcpy(&other_cfg->bls_config,
                      &isp_dev->other_cfgs.cfgs[match_id].bls_config,
                      sizeof(other_cfg->bls_config));
            break;
        case RK_ISP_SDG_ID:
            rt_memcpy(&other_cfg->sdg_config,
                      &isp_dev->other_cfgs.cfgs[match_id].sdg_config,
                      sizeof(other_cfg->sdg_config));
            break;
        case RK_ISP_LSC_ID:
            rt_memcpy(&other_cfg->lsc_config,
                      &isp_dev->other_cfgs.cfgs[match_id].lsc_config,
                      sizeof(other_cfg->lsc_config));
            break;
        case RK_ISP_AWB_GAIN_ID:
            if (RKISP_MODULE_IS_EN(other_cfg->module_ens,
                                   RK_ISP_MODULE_AWB_GAIN))
            {
                rt_memcpy(&other_cfg->awb_gain_config,
                          &isp_dev->other_cfgs.cfgs[match_id].awb_gain_config,
                          sizeof(other_cfg->awb_gain_config));
            }
            else
            {
                unsigned int reg = READ_RKISP_REG(RK_ISP_AWB_GAIN_RB);
                other_cfg->awb_gain_config.gain_red =
                    (unsigned short)RKISP_AWB_GAIN_R_READ(reg);
                other_cfg->awb_gain_config.gain_blue =
                    (unsigned short)RKISP_AWB_GAIN_B_READ(reg);
                reg = READ_RKISP_REG(RK_ISP_AWB_GAIN_RB);
                other_cfg->awb_gain_config.gain_green_r =
                    (unsigned short)RKISP_AWB_GAIN_R_READ(reg);
                other_cfg->awb_gain_config.gain_green_b =
                    (unsigned short)RKISP_AWB_GAIN_B_READ(reg);
            }
            break;
        case RK_ISP_FLT_ID:
            rt_memcpy(&other_cfg->flt_config,
                      &isp_dev->other_cfgs.cfgs[match_id].flt_config,
                      sizeof(other_cfg->flt_config));
            break;
        case RK_ISP_BDM_ID:
            rt_memcpy(&other_cfg->bdm_config,
                      &isp_dev->other_cfgs.cfgs[match_id].bdm_config,
                      sizeof(other_cfg->bdm_config));
            break;
        case RK_ISP_CTK_ID:
            if (RKISP_MODULE_IS_EN(other_cfg->module_ens,
                                   RK_ISP_MODULE_CTK))
            {
                rt_memcpy(&other_cfg->ctk_config,
                          &isp_dev->other_cfgs.cfgs[match_id].ctk_config,
                          sizeof(other_cfg->ctk_config));
            }
            else
            {
                other_cfg->ctk_config.coeff0 = READ_RKISP_REG(RK_ISP_CT_COEFF_0);
                other_cfg->ctk_config.coeff1 = READ_RKISP_REG(RK_ISP_CT_COEFF_1);
                other_cfg->ctk_config.coeff2 = READ_RKISP_REG(RK_ISP_CT_COEFF_2);
                other_cfg->ctk_config.coeff3 = READ_RKISP_REG(RK_ISP_CT_COEFF_3);
                other_cfg->ctk_config.coeff4 = READ_RKISP_REG(RK_ISP_CT_COEFF_4);
                other_cfg->ctk_config.coeff5 = READ_RKISP_REG(RK_ISP_CT_COEFF_5);
                other_cfg->ctk_config.coeff6 = READ_RKISP_REG(RK_ISP_CT_COEFF_6);
                other_cfg->ctk_config.coeff7 = READ_RKISP_REG(RK_ISP_CT_COEFF_7);
                other_cfg->ctk_config.coeff8 = READ_RKISP_REG(RK_ISP_CT_COEFF_8);
            }
            break;
        case RK_ISP_GOC_ID:
            rt_memcpy(&other_cfg->goc_config,
                      &isp_dev->other_cfgs.cfgs[match_id].goc_config,
                      sizeof(other_cfg->goc_config));
            break;
        case RK_ISP_CPROC_ID:
            rt_memcpy(&other_cfg->cproc_config,
                      &isp_dev->other_cfgs.cfgs[match_id].cproc_config,
                      sizeof(other_cfg->cproc_config));
            break;
        case RK_ISP_IE_ID:
            rt_memcpy(&other_cfg->ie_config,
                      &isp_dev->other_cfgs.cfgs[match_id].ie_config,
                      sizeof(other_cfg->ie_config));
            break;
        case RK_ISP_WDR_ID:
            rt_memcpy(&other_cfg->wdr_config,
                      &isp_dev->other_cfgs.cfgs[match_id].wdr_config,
                      sizeof(other_cfg->wdr_config));
            break;
        case RK_ISP_DPF_ID:
            rt_memcpy(&other_cfg->dpf_config,
                      &isp_dev->other_cfgs.cfgs[match_id].dpf_config,
                      sizeof(other_cfg->dpf_config));
            break;
        case RK_ISP_DPF_STRENGTH_ID:
            rt_memcpy(&other_cfg->dpf_strength_config,
                      &isp_dev->other_cfgs.cfgs[match_id].dpf_strength_config,
                      sizeof(other_cfg->dpf_strength_config));
            break;
        default:
            break;
        }
    }

    meas_cfg->module_ens = 0;
    for (i = RK_ISP_MEAS_ID; i < RK_ISP_MODULE_MAX; i++)
    {
        match_id = 0xff;
        for (j = 0; j < 3; j++)
        {
            if (isp_dev->frame_id >=
                    isp_dev->meas_cfgs.log[i].s_frame_id[j])
            {
                if (match_id == 0xff)
                    match_id = j;
                else if (isp_dev->meas_cfgs.log[i].s_frame_id[match_id] <
                         isp_dev->meas_cfgs.log[i].s_frame_id[j])
                    match_id = j;
            }
        }

        if (match_id == 0xff)
        {
            RKISP_DPRINT(RKISP_ERROR,
                         "%s: FrameID:%d isp meas config haven't found! s_frame_id:%d, %d,%d\n",
                         __FUNCTION__,
                         readout_work->frame_id,
                         isp_dev->meas_cfgs.log[i].s_frame_id[0],
                         isp_dev->meas_cfgs.log[i].s_frame_id[1],
                         isp_dev->meas_cfgs.log[i].s_frame_id[2]);
            match_id = isp_dev->meas_cfgs.log[i].curr_id;
        }
        switch (i)
        {
        case RK_ISP_AFC_ID:
            rt_memcpy(&meas_cfg->afc_config,
                      &isp_dev->meas_cfgs.cfgs[match_id].afc_config,
                      sizeof(meas_cfg->afc_config));
            break;
        case RK_ISP_AEC_ID:
            rt_memcpy(&meas_cfg->aec_config,
                      &isp_dev->meas_cfgs.cfgs[match_id].aec_config,
                      sizeof(meas_cfg->aec_config));
            break;
        case RK_ISP_AWB_ID:
            rt_memcpy(&meas_cfg->awb_meas_config,
                      &isp_dev->meas_cfgs.cfgs[match_id].awb_meas_config,
                      sizeof(meas_cfg->awb_meas_config));
            break;
        case RK_ISP_HST_ID:
            rt_memcpy(&meas_cfg->hst_config,
                      &isp_dev->meas_cfgs.cfgs[match_id].hst_config,
                      sizeof(meas_cfg->hst_config));
            break;
        default:
            break;
        }

        meas_cfg->module_ens |=
            (isp_dev->meas_cfgs.cfgs[match_id].module_ens &
             (1 << i));
    }
#endif

    if (isp_dev->meas_stats.g_frame_id >=
            isp_dev->frame_id)
    {
        rt_memcpy(stat_new,
                  &isp_dev->meas_stats.stat,
                  sizeof(*stat_new));
    }

    rt_do_set_rkisp_metadata(rk_isp_dev);
    return 0;
}

int rt_do_rkisp_meas_queue_work(struct rt_rkisp_isp_dev *isp_dev,
                                unsigned int send_meas, bool force_send)
{
    rt_do_send_rkisp_measurement(isp_dev, send_meas);

    return 0;
}

void rt_do_get_meas_data_buf(struct rt_rkisp_isp_dev *isp_dev, void *arg)
{
    struct rk_isp_device *rk_isp_dev =
        rt_container_of(isp_dev, struct rk_isp_device, isp_dev);
    struct rk_isp_stat_buffer *stat_buf;
    struct rk_buffer_metadata_s *isp_metadata[2] ;
    int *data = (int *)arg;
    isp_metadata[0] = &rk_isp_dev->isp_buf_metadata[rk_isp_dev->buf_index];
    isp_metadata[1] = &rk_isp_dev->isp_buf_metadata[rk_isp_dev->buf_index ^ 1];

    if (isp_metadata[0]->frame_id > isp_metadata[1]->frame_id)
    {
        *data = (int)&isp_metadata[0]->isp_meta.meas_stat;
    }
    else
    {
        *data = (int)&isp_metadata[1]->isp_meta.meas_stat;
    }
}

static inline bool rt_do_set_rkisp_isr_other_config(
    struct rt_rkisp_isp_dev *isp_dev,
    int *time_left)
{
    unsigned int new_id;
    unsigned int i, j;
    unsigned int *ens;
    unsigned int *actives = &isp_dev->other_cfgs.module_actives;

    for (i = 0; i < RK_ISP_MEAS_ID; i++)
    {
        if (RKISP_MODULE_IS_UNACTIVE(*actives, (1 << i)))
            continue;

        if (RKISP_MODULE_IS_UPDATE(
                    isp_dev->other_cfgs.module_updates,
                    (1 << i)))
        {
            new_id = isp_dev->other_cfgs.log[i].new_id;
            ens = &isp_dev->other_cfgs.cfgs[new_id].module_ens;

            switch (i)
            {
            case RK_ISP_DPCC_ID:
                /*update dpc config */
                rt_rkisp_dpcc_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_dpcc_en(isp_dev);
                else
                    rt_rkisp_dpcc_end(isp_dev);
                *time_left -= RKISP_MODULE_DPCC_PROC_TIME;
                break;
            case RK_ISP_BLS_ID:
                /*update bls config */
                rt_rkisp_bls_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_bls_en(isp_dev);
                else
                    rt_rkisp_bls_end(isp_dev);
                *time_left -= RKISP_MODULE_BLS_PROC_TIME;
                break;
            case RK_ISP_SDG_ID:
                /*update sdg config */
                rt_rkisp_sdg_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_sdg_en(isp_dev);
                else
                    rt_rkisp_sdg_end(isp_dev);
                *time_left -= RKISP_MODULE_SDG_PROC_TIME;
                break;
            case RK_ISP_LSC_ID:
            {
                bool res = true;
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                {
                    if (!rt_rkisp_lsc_config(isp_dev))
                        res = false;
                }
                else
                    rt_rkisp_lsc_end(isp_dev);
                *time_left -= RKISP_MODULE_LSC_PROC_TIME;
                break;
            }
            case RK_ISP_AWB_GAIN_ID:
                /*update awb gains */
                rt_rkisp_awb_gain_config(isp_dev);
                *time_left -= RKISP_MODULE_AWB_GAIN_PROC_TIME;
                break;
            case RK_ISP_BDM_ID:
                /*update bdm config */
                rt_rkisp_bdm_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_bdm_en(isp_dev);
                else
                    rt_rkisp_bdm_end(isp_dev);
                *time_left -= RKISP_MODULE_BDM_PROC_TIME;
                break;
            case RK_ISP_FLT_ID:
                /*update filter config */
                rt_rkisp_flt_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_flt_en(isp_dev);
                else
                    rt_rkisp_flt_end(isp_dev);
                *time_left -= RKISP_MODULE_FLT_PROC_TIME;
                break;
            case RK_ISP_CTK_ID:
                /*update ctk config */
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_ctk_en(isp_dev);
                else
                    rt_rkisp_ctk_end(isp_dev);
                *time_left -= RKISP_MODULE_CTK_PROC_TIME;
                break;
            case RK_ISP_GOC_ID:
                /*update goc config */
                rt_rkisp_goc_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_goc_en(isp_dev);
                else
                    rt_rkisp_goc_end(isp_dev);
                *time_left -= RKISP_MODULE_GOC_PROC_TIME;
                break;
            case RK_ISP_CPROC_ID:
                /*update cprc config */
                rt_rkisp_cproc_config(
                    isp_dev,
                    isp_dev->quantization);
                rt_rkisp_csm_config(isp_dev,
                                    isp_dev->quantization);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                {
                    rt_rkisp_cproc_en(isp_dev);
                }
                else
                {
                    rt_rkisp_cproc_end(isp_dev);
                }
                *time_left -= RKISP_MODULE_CPROC_PROC_TIME;
                break;
            case RK_ISP_IE_ID:
                /*update ie config */
                rt_rkisp_ie_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_ie_en(isp_dev);
                else
                    rt_rkisp_ie_end(isp_dev);
                *time_left -= RKISP_MODULE_IE_PROC_TIME;
                break;
            case RK_ISP_DPF_ID:
                /*update dpf  config */
                rt_rkisp_dpf_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_dpf_en(isp_dev);
                else
                    rt_rkisp_dpf_end(isp_dev);
                *time_left -= RKISP_MODULE_DPF_TIME;
                break;
            case RK_ISP_DPF_STRENGTH_ID:
                /*update dpf strength config */
                rt_rkisp_dpf_strength_config(isp_dev);
                *time_left -= RKISP_MODULE_DPF_STRENGTH_TIME;
                break;
            case RK_ISP_WDR_ID:
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                {
                    rt_rkisp_wdr_config(isp_dev);
                    rt_rkisp_wdr_en(isp_dev);
                }
                else
                {
                    rt_rkisp_wdr_end(isp_dev);
                }
                *time_left -= RKISP_MODULE_WDR_TIME;
                break;
            default:
                break;
            }

            isp_dev->other_cfgs.log[i].s_frame_id[new_id] =
                isp_dev->frame_id;
            isp_dev->other_cfgs.log[i].curr_id = new_id;
            new_id = 0;
            for (j = 0; j < 2; j++)
            {
                if (isp_dev->other_cfgs.log[i].s_frame_id[new_id] >
                        isp_dev->other_cfgs.log[i].s_frame_id[j])
                    new_id = j;
            }
            isp_dev->other_cfgs.log[i].new_id = new_id;
            RKISP_MODULE_CLR_UPDATE(
                isp_dev->other_cfgs.module_updates,
                (1 << i));
        }
    }

    if (*time_left <= 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "%s: time_left: %dus\n",
                     __FUNCTION__,
                     *time_left);
        return -1;
    }

    return 0;
}

static inline bool rt_do_set_rkisp_isr_meas_config(
    struct rt_rkisp_isp_dev *isp_dev,
    int *time_left)
{
    unsigned int new_id;
    unsigned int i, j;
    unsigned int *ens;
    unsigned int *actives = &isp_dev->meas_cfgs.module_actives;

    for (i = RK_ISP_MEAS_ID; i < RK_ISP_MODULE_MAX; i++)
    {
        if (RKISP_MODULE_IS_UNACTIVE(*actives, (1 << i)))
            continue;

        if (RKISP_MODULE_IS_UPDATE(
                    isp_dev->meas_cfgs.module_updates,
                    (1 << i)))
        {

            new_id = isp_dev->meas_cfgs.log[i].new_id;
            ens = &isp_dev->meas_cfgs.cfgs[new_id].module_ens;

            switch (i)
            {
            case RK_ISP_AWB_ID:
                /*update dpc config */
                rt_rkisp_awb_meas_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_awb_meas_en(isp_dev);
                else
                    rt_rkisp_awb_meas_end(isp_dev);
                *time_left -= RKISP_MODULE_AWB_PROC_TIME;
                break;
            case RK_ISP_AEC_ID:
                /*update aec config */
                rt_rkisp_aec_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_aec_en(isp_dev);
                else
                    rt_rkisp_aec_end(isp_dev);
                *time_left -= RKISP_MODULE_AEC_PROC_TIME;
                break;
            case RK_ISP_AFC_ID:
                /*update afc config */
                rt_rkisp_afc_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_afc_en(isp_dev);
                else
                    rt_rkisp_afc_end(isp_dev);
                *time_left -= RKISP_MODULE_AFC_PROC_TIME;
                break;
            case RK_ISP_HST_ID:
                /*update hst config */
                rt_rkisp_hst_config(isp_dev);
                if (RKISP_MODULE_IS_EN(*ens, (1 << i)))
                    rt_rkisp_hst_en(isp_dev);
                else
                    rt_rkisp_hst_end(isp_dev);
                *time_left -= RKISP_MODULE_HST_PROC_TIME;
                break;
            default:
                break;
            }

            isp_dev->meas_cfgs.log[i].s_frame_id[new_id] =
                isp_dev->frame_id;
            isp_dev->meas_cfgs.log[i].curr_id = new_id;
            new_id = 0;
            for (j = 0; j < 2; j++)
            {
                if (isp_dev->meas_cfgs.log[i].s_frame_id[new_id] >
                        isp_dev->meas_cfgs.log[i].s_frame_id[j])
                    new_id = j;
            }
            isp_dev->meas_cfgs.log[i].new_id = new_id;
            RKISP_MODULE_CLR_UPDATE(
                isp_dev->meas_cfgs.module_updates,
                (1 << i));
        }
    }

    if (*time_left <= 0)
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "%s: time_left: %dus\n",
                     __FUNCTION__,
                     *time_left);
        return -1;
    }

    return 0;
}


int rt_do_set_rkisp_isp_module_interrupt(struct rt_rkisp_isp_dev *isp_dev, unsigned int isp_mis)
{
    const struct rk_isp_device *rt_isp_dev =
        rt_container_of(isp_dev, struct rk_isp_device, isp_dev);
    unsigned int isp_mis_tmp = 0, isp_module_mis = 0;
    int time_left = isp_dev->v_blanking_us;
    unsigned int active_meas = 0;
    int ret = 0;

    if (isp_mis & (RK_ISP_DATA_LOSS | RK_ISP_PIC_SIZE_ERROR))
        return 0;

    isp_module_mis = isp_mis & (RK_ISP_AWB_DONE | RK_ISP_AFM_FIN |
                                RK_ISP_EXP_END | RK_ISP_HIST_MEASURE_RDY);
    ISP_DBG("isp MODULE status is %d time_left is %d\n", isp_module_mis, time_left);

    if (isp_module_mis)
    {
        WRITE_RKISP_REG(isp_module_mis, RK_ISP_ICR);

        isp_mis_tmp = READ_RKISP_REG(RK_ISP_MIS);
        if (isp_mis_tmp & isp_module_mis)
            rt_kprintf("isp icr 3A info err: 0x%x\n", isp_mis_tmp);
    }

    if (isp_module_mis & RK_ISP_AWB_DONE)
    {
        isp_dev->awb_meas_ready = true;
        rt_rkisp_get_awb_meas(isp_dev, &isp_dev->meas_stats.stat);
    }

    if (isp_module_mis & RK_ISP_AFM_FIN)
    {
        isp_dev->afm_meas_ready = true;
        rt_rkisp_get_afc_meas(isp_dev, &isp_dev->meas_stats.stat);
    }

    if (isp_module_mis & RK_ISP_EXP_END)
    {
        isp_dev->aec_meas_ready = true;
        rt_rkisp_get_aec_meas(isp_dev, &isp_dev->meas_stats.stat);
        rt_rkisp_bls_get_meas(isp_dev, &isp_dev->meas_stats.stat);
        isp_dev->meas_stats.g_frame_id = isp_dev->frame_id;
    }

    if (isp_module_mis & RK_ISP_HIST_MEASURE_RDY)
    {
        isp_dev->hst_meas_ready = true;
        rt_rkisp_get_hst_meas(isp_dev, &isp_dev->meas_stats.stat);
    }

    if (isp_mis & RK_ISP_FRAME)
    {
        if (isp_dev->awb_meas_ready)
            active_meas |= RK_ISP_AWB_DONE;

        if (isp_dev->afm_meas_ready)
            active_meas |= RK_ISP_AFM_FIN;

        if (isp_dev->aec_meas_ready)
            active_meas |= RK_ISP_EXP_END;

        if (isp_dev->hst_meas_ready)
            active_meas |= RK_ISP_HIST_MEASURE_RDY;

        rt_do_rkisp_meas_queue_work(isp_dev, active_meas, false);

        if (!rt_do_set_rkisp_isr_other_config(isp_dev, &time_left))
            ret = rt_do_set_rkisp_isr_meas_config(isp_dev, &time_left);

#ifdef RKISP_DEBUG_REG
        rkisp_dump_reg(isp_dev, RKISP_DEBUG);
#endif
    }

    ISP_DBG("inter leave status is %d\n", READ_RKISP_REG(RK_ISP_MIS));
    return 0;
}

static void rkisp_param_dump(const void *config, unsigned int module)
{
#ifdef RKISP_DEBUG_PARAM
    switch (module)
    {
    case RK_ISP_MODULE_AWB_GAIN:
    {
        struct rk_isp_awb_gain_config *pconfig =
            (struct rk_isp_awb_gain_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: AWB Gain Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, "g_g: %d\n",
                     pconfig->gain_green_r);
        RKISP_DPRINT(RKISP_DEBUG, "g_b: %d\n",
                     pconfig->gain_green_b);
        RKISP_DPRINT(RKISP_DEBUG, "r: %d\n",
                     pconfig->gain_red);
        RKISP_DPRINT(RKISP_DEBUG, "b: %d\n",
                     pconfig->gain_blue);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: AWB Gain Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;
    case RK_ISP_MODULE_DPCC:
    {
    }
    break;

    case RK_ISP_MODULE_BLS:
    {
        struct rk_isp_bls_config *pconfig =
            (struct rk_isp_bls_config *)config;
        struct rk_isp_bls_fixed_val *pval = &pconfig->fixed_val;

        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: BLS Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, " enable_auto: %d\n",
                     pconfig->enable_auto);
        RKISP_DPRINT(RKISP_DEBUG, " en_windows: %d\n",
                     pconfig->en_windows);
        RKISP_DPRINT(RKISP_DEBUG,
                     " bls_window1.h_offs: %d\n",
                     pconfig->bls_window1.h_offs);
        RKISP_DPRINT(RKISP_DEBUG,
                     " bls_window1.v_offs: %d\n",
                     pconfig->bls_window1.v_offs);
        RKISP_DPRINT(RKISP_DEBUG,
                     " bls_window1.h_size: %d\n",
                     pconfig->bls_window1.h_size);
        RKISP_DPRINT(RKISP_DEBUG,
                     " bls_window1.v_size: %d\n",
                     pconfig->bls_window1.v_size);
        RKISP_DPRINT(RKISP_DEBUG,
                     " bls_window2.h_offs: %d\n",
                     pconfig->bls_window2.h_offs);
        RKISP_DPRINT(RKISP_DEBUG,
                     " bls_window2.v_offs: %d\n",
                     pconfig->bls_window2.v_offs);
        RKISP_DPRINT(RKISP_DEBUG,
                     " bls_window2.h_size: %d\n",
                     pconfig->bls_window2.h_size);
        RKISP_DPRINT(RKISP_DEBUG,
                     " bls_window2.v_size: %d\n",
                     pconfig->bls_window2.v_size);
        RKISP_DPRINT(RKISP_DEBUG, " bls_samples: %d\n",
                     pconfig->bls_samples);
        RKISP_DPRINT(RKISP_DEBUG, " fixed_A: %d\n",
                     pval->fixed_a);
        RKISP_DPRINT(RKISP_DEBUG, " fixed_B: %d\n",
                     pval->fixed_b);
        RKISP_DPRINT(RKISP_DEBUG, " fixed_C: %d\n",
                     pval->fixed_c);
        RKISP_DPRINT(RKISP_DEBUG, " fixed_D: %d\n",
                     pval->fixed_d);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: BLS Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;
    case RK_ISP_MODULE_LSC:
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### LSC Parameters - BEGIN ####\n");
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### LSC Parameters - END ####\n");
    }
    break;
    case RK_ISP_MODULE_FLT:
    {
        struct rk_isp_flt_config *pconfig =
            (struct rk_isp_flt_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: FLT Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_mask_sharp0: %d\n",
                     pconfig->flt_mask_sharp0);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_mask_sharp1: %d\n",
                     pconfig->flt_mask_sharp1);
        RKISP_DPRINT(RKISP_DEBUG, " flt_mask_diag: %d\n",
                     pconfig->flt_mask_diag);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_mask_blur_max: %d\n",
                     pconfig->flt_mask_blur_max);
        RKISP_DPRINT(RKISP_DEBUG, " flt_mask_blur: %d\n",
                     pconfig->flt_mask_blur);
        RKISP_DPRINT(RKISP_DEBUG, " flt_mask_lin: %d\n",
                     pconfig->flt_mask_lin);
        RKISP_DPRINT(RKISP_DEBUG, " flt_mask_orth: %d\n",
                     pconfig->flt_mask_orth);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_mask_v_diag: %d\n",
                     pconfig->flt_mask_v_diag);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_mask_h_diag: %d\n",
                     pconfig->flt_mask_h_diag);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_lum_weight: %d\n",
                     pconfig->flt_lum_weight);
        RKISP_DPRINT(RKISP_DEBUG, " flt_blur_th0: %d\n",
                     pconfig->flt_blur_th0);
        RKISP_DPRINT(RKISP_DEBUG, " flt_blur_th1: %d\n",
                     pconfig->flt_blur_th1);
        RKISP_DPRINT(RKISP_DEBUG, " flt_sharp0_th: %d\n",
                     pconfig->flt_sharp0_th);
        RKISP_DPRINT(RKISP_DEBUG, " flt_sharp1_th: %d\n",
                     pconfig->flt_sharp1_th);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_chrom_h_mode: %d\n",
                     pconfig->flt_chrom_h_mode);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_chrom_v_mode: %d\n",
                     pconfig->flt_chrom_v_mode);
        RKISP_DPRINT(RKISP_DEBUG,
                     " flt_diag_sharp_mode: %d\n",
                     pconfig->flt_diag_sharp_mode);
        RKISP_DPRINT(RKISP_DEBUG, " flt_mode: %d\n",
                     pconfig->flt_mode);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: FLT Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;

    case RK_ISP_MODULE_BDM:
    {
        struct rk_isp_bdm_config *pconfig =
            (struct rk_isp_bdm_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: BDM Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, " demosaic_th: %d\n",
                     pconfig->demosaic_th);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: BDM Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;

    case RK_ISP_MODULE_SDG:
    {
        struct rk_isp_sdg_config *pconfig =
            (struct rk_isp_sdg_config *)config;
        unsigned int i;

        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: SDG Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG,
                     " RED -Curve parameters\n");
        for (i = 0; i < RKISP_DEGAMMA_CURVE_SIZE; i++)
        {
            RKISP_DPRINT(RKISP_DEBUG, " gamma_y[%d]: %d\n",
                         pconfig->curve_r.gamma_y[i]);
        }
        RKISP_DPRINT(RKISP_DEBUG,
                     " GREEN -Curve parameters\n");
        for (i = 0; i < RKISP_DEGAMMA_CURVE_SIZE; i++)
        {
            RKISP_DPRINT(RKISP_DEBUG, " gamma_y[%d]: %d\n",
                         pconfig->curve_g.gamma_y[i]);
        }
        RKISP_DPRINT(RKISP_DEBUG,
                     " BLUE -Curve parameters\n");
        for (i = 0; i < RKISP_DEGAMMA_CURVE_SIZE; i++)
        {
            RKISP_DPRINT(RKISP_DEBUG, " gamma_y[%d]: %d\n",
                         pconfig->curve_b.gamma_y[i]);
        }
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: SDG Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;

    case RK_ISP_MODULE_GOC:
    {
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: GOC Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: GOC Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;

    case RK_ISP_MODULE_CTK:
    {
        struct rk_isp_ctk_config *pconfig =
            (struct rk_isp_ctk_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: CTK Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, " coeff0: %d\n",
                     pconfig->coeff0);
        RKISP_DPRINT(RKISP_DEBUG, " coeff1: %d\n",
                     pconfig->coeff1);
        RKISP_DPRINT(RKISP_DEBUG, " coeff2: %d\n",
                     pconfig->coeff2);
        RKISP_DPRINT(RKISP_DEBUG, " coeff3: %d\n",
                     pconfig->coeff3);
        RKISP_DPRINT(RKISP_DEBUG, " coeff4: %d\n",
                     pconfig->coeff4);
        RKISP_DPRINT(RKISP_DEBUG, " coeff5: %d\n",
                     pconfig->coeff5);
        RKISP_DPRINT(RKISP_DEBUG, " coeff6: %d\n",
                     pconfig->coeff6);
        RKISP_DPRINT(RKISP_DEBUG, " coeff7: %d\n",
                     pconfig->coeff7);
        RKISP_DPRINT(RKISP_DEBUG, " coeff8: %d\n",
                     pconfig->coeff8);
        RKISP_DPRINT(RKISP_DEBUG, " ct_offset_r: %d\n",
                     pconfig->ct_offset_r);
        RKISP_DPRINT(RKISP_DEBUG, " ct_offset_g: %d\n",
                     pconfig->ct_offset_g);
        RKISP_DPRINT(RKISP_DEBUG, " ct_offset_b: %d\n",
                     pconfig->ct_offset_b);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: CTK Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;

    case RK_ISP_MODULE_AWB:
    {
        struct rk_isp_awb_meas_config *pconfig =
            (struct rk_isp_awb_meas_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: AWB Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, " awb_mode: %d\n",
                     pconfig->awb_mode);
        RKISP_DPRINT(RKISP_DEBUG, " max_y: %d\n",
                     pconfig->max_y);
        RKISP_DPRINT(RKISP_DEBUG, " min_y: %d\n",
                     pconfig->min_y);
        RKISP_DPRINT(RKISP_DEBUG, " max_csum: %d\n",
                     pconfig->max_csum);
        RKISP_DPRINT(RKISP_DEBUG, " min_c: %d\n",
                     pconfig->min_c);
        RKISP_DPRINT(RKISP_DEBUG, " frames: %d\n",
                     pconfig->frames);
        RKISP_DPRINT(RKISP_DEBUG, " awb_ref_cr: %d\n",
                     pconfig->awb_ref_cr);
        RKISP_DPRINT(RKISP_DEBUG, " awb_ref_cb: %d\n",
                     pconfig->awb_ref_cb);
        RKISP_DPRINT(RKISP_DEBUG, " gb_sat: %d\n",
                     pconfig->gb_sat);
        RKISP_DPRINT(RKISP_DEBUG, " gr_sat: %d\n",
                     pconfig->gr_sat);
        RKISP_DPRINT(RKISP_DEBUG, " r_sat: %d\n",
                     pconfig->b_sat);
        RKISP_DPRINT(RKISP_DEBUG, " grid_h_dim: %d\n",
                     pconfig->grid_h_dim);
        RKISP_DPRINT(RKISP_DEBUG, " grid_v_dim: %d\n",
                     pconfig->grid_v_dim);
        RKISP_DPRINT(RKISP_DEBUG, " grid_h_dist: %d\n",
                     pconfig->grid_h_dist);
        RKISP_DPRINT(RKISP_DEBUG, " grid_v_dist: %d\n",
                     pconfig->grid_v_dist);
        RKISP_DPRINT(RKISP_DEBUG,
                     " enable_ymax_cmp: %d\n",
                     pconfig->enable_ymax_cmp);
        RKISP_DPRINT(RKISP_DEBUG, " rgb_meas_pnt: %d\n",
                     pconfig->rgb_meas_pnt);
        RKISP_DPRINT(RKISP_DEBUG, " AWB Window size\n");
        RKISP_DPRINT(RKISP_DEBUG, " h_offs: %d\n",
                     pconfig->awb_wnd.h_offs);
        RKISP_DPRINT(RKISP_DEBUG, " v_offs: %d\n",
                     pconfig->awb_wnd.v_offs);
        RKISP_DPRINT(RKISP_DEBUG, " h_size: %d\n",
                     pconfig->awb_wnd.h_size);
        RKISP_DPRINT(RKISP_DEBUG, " v_size: %d\n",
                     pconfig->awb_wnd.v_size);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: AWB Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;

    case RK_ISP_MODULE_HST:
    {
        struct rk_isp_hst_config *pconfig =
            (struct rk_isp_hst_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: HST Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, " mode: %d\n",
                     pconfig->mode);
        RKISP_DPRINT(RKISP_DEBUG,
                     " histogram_predivider: %d\n",
                     pconfig->histogram_predivider);
        RKISP_DPRINT(RKISP_DEBUG, " HST Window size\n");
        RKISP_DPRINT(RKISP_DEBUG, " h_offs: %d\n",
                     pconfig->meas_window.h_offs);
        RKISP_DPRINT(RKISP_DEBUG, " v_offs: %d\n",
                     pconfig->meas_window.v_offs);
        RKISP_DPRINT(RKISP_DEBUG, " h_size: %d\n",
                     pconfig->meas_window.h_size);
        RKISP_DPRINT(RKISP_DEBUG, " v_size: %d\n",
                     pconfig->meas_window.v_size);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: HST Parameters - END ####\n",
                     ISP_VDEV_NAME);

    }
    break;

    case RK_ISP_MODULE_AEC:
    {
        struct rk_isp_aec_config *pconfig =
            (struct rk_isp_aec_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: AEC Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, " autostop: %d\n",
                     pconfig->autostop);
        RKISP_DPRINT(RKISP_DEBUG, " AEC Window size\n");
        RKISP_DPRINT(RKISP_DEBUG, " h_offs: %d\n",
                     pconfig->meas_window.h_offs);
        RKISP_DPRINT(RKISP_DEBUG, " v_offs: %d\n",
                     pconfig->meas_window.v_offs);
        RKISP_DPRINT(RKISP_DEBUG, " h_size: %d\n",
                     pconfig->meas_window.h_size);
        RKISP_DPRINT(RKISP_DEBUG, " v_size: %d\n",
                     pconfig->meas_window.v_size);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: AEC Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;

    case RK_ISP_MODULE_CPROC:
    {
        struct rk_isp_cproc_config *pconfig =
            (struct rk_isp_cproc_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: CPROC Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, " contrast: %d\n",
                     pconfig->contrast);
        RKISP_DPRINT(RKISP_DEBUG, " hue: %d\n",
                     pconfig->hue);
        RKISP_DPRINT(RKISP_DEBUG, " sat: %d\n",
                     pconfig->sat);
        RKISP_DPRINT(RKISP_DEBUG, " brightness: %d\n",
                     pconfig->brightness);
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: CPROC Parameters - END ####\n",
                     ISP_VDEV_NAME);
    }
    break;
    case RK_ISP_MODULE_YCFLT:
    {
        struct rk_isp_ycflt_config *pconfig =
            (struct rk_isp_ycflt_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: YCFLT Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG, " chr_ss_ctrl: %d\n",
                     RKISP_DPRINT(RKISP_DEBUG, " ctrl: %d\n",
                                  pconfig->ctrl);
                     RKISP_DPRINT(RKISP_DEBUG, " chr_ss_ctrl: %d\n",
                                  pconfig->chr_ss_ctrl);
                     RKISP_DPRINT(RKISP_DEBUG, " chr_ss_fac: %d\n",
                                  pconfig->chr_ss_fac);
                     RKISP_DPRINT(RKISP_DEBUG, " chr_ss_offs: %d\n",
                                  pconfig->chr_ss_offs);
                     RKISP_DPRINT(RKISP_DEBUG, " chr_nr_ctrl: %d\n",
                                  pconfig->chr_nr_ctrl);
                     RKISP_DPRINT(RKISP_DEBUG,
                                  " lum_eenr_edge_gain: %d\n",
                                  pconfig->lum_eenr_edge_gain);
                     RKISP_DPRINT(RKISP_DEBUG,
                                  " lum_eenr_corner_gain: %d\n",
                                  pconfig->lum_eenr_corner_gain);
                     RKISP_DPRINT(RKISP_DEBUG,
                                  " lum_eenr_fc_crop_neg: %d\n",
                                  pconfig->lum_eenr_fc_crop_neg);
                     RKISP_DPRINT(RKISP_DEBUG,
                                  " lum_eenr_fc_crop_pos: %d\n",
                                  pconfig->lum_eenr_fc_crop_pos);
                     RKISP_DPRINT(RKISP_DEBUG,
                                  " lum_eenr_fc_gain_neg: %d\n",
                                  pconfig->lum_eenr_fc_gain_neg);
                     RKISP_DPRINT(RKISP_DEBUG,
                                  " lum_eenr_fc_gain_pos: %d\n",
                                  pconfig->lum_eenr_fc_gain_pos);
                     RKISP_DPRINT(RKISP_DEBUG,
                                  "#### %s: YCFLT Parameters - END ####\n",
                                  ISP_VDEV_NAME);
                     break;
    }
    case RK_ISP_MODULE_AFC:
    {
        struct rk_isp_afc_config *pconfig =
            (struct rk_isp_afc_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "#### %s: AFC Parameters - BEGIN ####\n",
                     ISP_VDEV_NAME);
        RKISP_DPRINT(RKISP_DEBUG,
                     " window A %d %d %d %d\n",
                     pconfig->afm_win[0].h_offs,
                     pconfig->afm_win[0].v_offs,
                     pconfig->afm_win[0].h_size,
                     pconfig->afm_win[0].v_size);
        RKISP_DPRINT(RKISP_DEBUG,
                     " window B %d %d %d %d\n",
                     pconfig->afm_win[1].h_offs,
                     pconfig->afm_win[1].v_offs,
                     pconfig->afm_win[1].h_size,
                     pconfig->afm_win[1].v_size);
        RKISP_DPRINT(RKISP_DEBUG,
                     " window C %d %d %d %d\n",
                     pconfig->afm_win[2].h_offs,
                     pconfig->afm_win[2].v_offs,
                     pconfig->afm_win[2].h_size,
                     pconfig->afm_win[2].v_size);
        RKISP_DPRINT(RKISP_DEBUG, " thres: %d\n",
                     pconfig->thres);
        RKISP_DPRINT(RKISP_DEBUG, " var_shift: %d\n",
                     pconfig->var_shift);
        break;
    }
    case RK_ISP_MODULE_IE:
    {
        struct rk_isp_ie_config *pconfig =
            (struct rk_isp_ie_config *)config;
        RKISP_DPRINT(RKISP_DEBUG,
                     "effect %d, %x, %x, %x, %x, %x, %x %d\n",
                     pconfig->effect, pconfig->color_sel,
                     pconfig->eff_mat_1, pconfig->eff_mat_2,
                     pconfig->eff_mat_3, pconfig->eff_mat_4,
                     pconfig->eff_mat_5, pconfig->eff_tint);
        break;
    }
    default:
        RKISP_DPRINT(RKISP_DEBUG,
                     "####%s: Invalid Module ID ####\n", ISP_VDEV_NAME);
        break;
    }
#endif
}

static void rkisp_reg_dump(const struct rt_rkisp_isp_dev *isp_dev,
                           unsigned int module, int level)
{
    switch (module)
    {
    case RK_ISP_MODULE_DPCC:
        RKISP_DPRINT(level, "#### BPC Registers - BEGIN ####\n");
        RKISP_DPRINT(level, "RK_ISP_DPCC_MODE: %x\n", READ_RKISP_REG(RK_ISP_DPCC_MODE));
        RKISP_DPRINT(level, "RK_ISP_DPCC_OUTPUT_MODE: %x\n", READ_RKISP_REG(RK_ISP_DPCC_OUTPUT_MODE));
        RKISP_DPRINT(level, "RK_ISP_DPCC_SET_USE: %x\n", READ_RKISP_REG(RK_ISP_DPCC_SET_USE));

        RKISP_DPRINT(level, "RK_ISP_DPCC_METHODS_SET_1: %x\n", READ_RKISP_REG(RK_ISP_DPCC_METHODS_SET_1));
        RKISP_DPRINT(level, "RK_ISP_DPCC_METHODS_SET_2: %x\n", READ_RKISP_REG(RK_ISP_DPCC_METHODS_SET_2));
        RKISP_DPRINT(level, "RK_ISP_DPCC_METHODS_SET_3: %x\n", READ_RKISP_REG(RK_ISP_DPCC_METHODS_SET_3));
        for (int i = 0; i < RK_ISP_DPCC_METHODS_MAX; i++)
        {
            RKISP_DPRINT(level, "RK_ISP_DPCC_LINE_THRESH_1: %x\n", READ_RKISP_REG(RK_ISP_DPCC_LINE_THRESH_1 + 0x14 * i));
            RKISP_DPRINT(level, "RK_ISP_DPCC_LINE_MAD_FAC_1: %x\n", READ_RKISP_REG(RK_ISP_DPCC_LINE_MAD_FAC_1 + 0x14 * i));
            RKISP_DPRINT(level, "RK_ISP_DPCC_PG_FAC_1: %x\n", READ_RKISP_REG(RK_ISP_DPCC_PG_FAC_1 + 0x14 * i));
            RKISP_DPRINT(level, "RK_ISP_DPCC_RND_THRESH_1: %x\n", READ_RKISP_REG(RK_ISP_DPCC_RND_THRESH_1 + 0x14 * i));
            RKISP_DPRINT(level, "RK_ISP_DPCC_RG_FAC_1: %x\n", READ_RKISP_REG(RK_ISP_DPCC_RG_FAC_1 + 0x14 * i));
        }

        RKISP_DPRINT(level, "RK_ISP_DPCC_RND_OFFS: %x\n", READ_RKISP_REG(RK_ISP_DPCC_RND_OFFS));
        RKISP_DPRINT(level, "RK_ISP_DPCC_RO_LIMITS: %x\n", READ_RKISP_REG(RK_ISP_DPCC_RO_LIMITS));

        RKISP_DPRINT(level, "#### BPC Registers - END ####\n");
        break;
    case RK_ISP_MODULE_BLS:
        RKISP_DPRINT(level, "#### BLS Registers - BEGIN ####\n");
        RKISP_DPRINT(level, " RK_ISP_BLS_CTRL: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_CTRL));
        RKISP_DPRINT(level, " RK_ISP_BLS_SAMPLES: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_SAMPLES));
        RKISP_DPRINT(level, " RK_ISP_BLS_H1_START: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_H1_START));
        RKISP_DPRINT(level, " RK_ISP_BLS_H1_STOP: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_H1_STOP));
        RKISP_DPRINT(level, " RK_ISP_BLS_H1_START: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_H1_START));
        RKISP_DPRINT(level, " RK_ISP_BLS_V1_START: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_V1_START));
        RKISP_DPRINT(level, " RK_ISP_BLS_V1_STOP: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_V1_STOP));
        RKISP_DPRINT(level, " RK_ISP_BLS_H2_START: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_H2_START));
        RKISP_DPRINT(level, " RK_ISP_BLS_H2_STOP: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_H2_STOP));
        RKISP_DPRINT(level, " RK_ISP_BLS_V2_START: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_V2_START));
        RKISP_DPRINT(level, " RK_ISP_BLS_V2_STOP: %d\n",
                     READ_RKISP_REG(RK_ISP_BLS_V2_STOP));
        RKISP_DPRINT(level, "#### BLS Registers - END ####\n");
        break;
    case RK_ISP_MODULE_LSC:
        RKISP_DPRINT(level, "#### LSC Registers - BEGIN ####\n");
        RKISP_DPRINT(level, "#### LSC Registers - END ####\n");
        break;
    case RK_ISP_MODULE_FLT:
    {
        RKISP_DPRINT(level,
                     "#### %s: FLT Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level,
                     " RK_ISP_FILT_MODE: %d\n",
                     READ_RKISP_REG(RK_ISP_FILT_MODE));
        RKISP_DPRINT(level,
                     " RK_ISP_FILT_LUM_WEIGHT: %d\n",
                     READ_RKISP_REG(RK_ISP_FILT_LUM_WEIGHT));
        RKISP_DPRINT(level,
                     "#### %s: FLT Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;

    case RK_ISP_MODULE_BDM:
    {
        RKISP_DPRINT(level,
                     "#### %s: BDM Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level, " RK_ISP_DEMOSAIC: %d\n",
                     READ_RKISP_REG(RK_ISP_DEMOSAIC));
        RKISP_DPRINT(level,
                     "#### %s: BDM Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;

    case RK_ISP_MODULE_SDG:
    {
        RKISP_DPRINT(level,
                     "#### %s: SDG Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level, " RK_ISP_GAMMA_DX_LO: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_DX_LO));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_DX_HI: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_DX_HI));

        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y0: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y0));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y1: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y1));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y2: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y2));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y3: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y3));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y4: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y4));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y5: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y5));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y6: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y6));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y7: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y7));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y8: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y8));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y9: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y9));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y10: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y10));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y11: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y11));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y12: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y12));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y13: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y13));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y14: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y14));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y15: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y15));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_R_Y16: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_R_Y16));

        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y0: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y0));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y1: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y1));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y2: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y2));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y3: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y3));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y4: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y4));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y5: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y5));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y6: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y6));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y7: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y7));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y8: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y8));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y9: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y9));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y10: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y10));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y11: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y11));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y12: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y12));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y13: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y13));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y14: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y14));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y15: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y15));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_G_Y16: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_G_Y16));

        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y0: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y0));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y1: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y1));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y2: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y2));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y3: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y3));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y4: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y4));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y5: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y5));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y6: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y6));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y7: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y7));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y8: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y8));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y9: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y9));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y10: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y10));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y11: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y11));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y12: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y12));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y13: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y13));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y14: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y14));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y15: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y15));
        RKISP_DPRINT(level, " RK_ISP_GAMMA_B_Y16: %d\n",
                     READ_RKISP_REG(RK_ISP_GAMMA_B_Y16));
        RKISP_DPRINT(level,
                     "#### %s: SDG Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;

    case RK_ISP_MODULE_GOC:
    {
        RKISP_DPRINT(level,
                     "#### %s: GOC Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level,
                     "#### %s: GOC registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;

    case RK_ISP_MODULE_CTK:
    {
        RKISP_DPRINT(level,
                     "#### %s: CTK Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_0: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_0));
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_1: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_1));
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_2: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_2));
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_3: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_3));
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_4: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_4));
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_5: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_5));
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_6: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_6));
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_7: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_7));
        RKISP_DPRINT(level, " RK_ISP_CT_COEFF_8: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_COEFF_8));
        RKISP_DPRINT(level, " RK_ISP_CT_OFFSET_R: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_OFFSET_R));
        RKISP_DPRINT(level, " RK_ISP_CT_OFFSET_G: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_OFFSET_G));
        RKISP_DPRINT(level, " RK_ISP_CT_OFFSET_B: %d\n",
                     READ_RKISP_REG(RK_ISP_CT_OFFSET_B));
        RKISP_DPRINT(level,
                     "#### %s: CTK Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;

    case RK_ISP_MODULE_AWB:
    {
        RKISP_DPRINT(level,
                     "#### %s: AWB Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level, " RK_ISP_AWB_PROP: %x\n",
                     READ_RKISP_REG(RK_ISP_AWB_PROP));
        RKISP_DPRINT(level, " RK_ISP_AWB_GAIN_G: %x\n",
                     READ_RKISP_REG(RK_ISP_AWB_GAIN_G));
        RKISP_DPRINT(level, " RK_ISP_AWB_GAIN_RB: %x\n",
                     READ_RKISP_REG(RK_ISP_AWB_GAIN_RB));
        RKISP_DPRINT(level, " RK_ISP_AWB_REF: %x\n",
                     READ_RKISP_REG(RK_ISP_AWB_REF));
        RKISP_DPRINT(level, " RK_ISP_AWB_GAIN_RB: %x\n",
                     READ_RKISP_REG(RK_ISP_AWB_PROP));
        RKISP_DPRINT(level, " RK_ISP_AWB_FRAMES: %x\n",
                     READ_RKISP_REG(RK_ISP_AWB_FRAMES));
        RKISP_DPRINT(level,
                     "#### %s: AWB Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;

    case RK_ISP_MODULE_HST:
    {
        RKISP_DPRINT(level,
                     "#### %s: HST Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level, " RK_ISP_HIST_PROP: %d\n",
                     READ_RKISP_REG(RK_ISP_HIST_PROP));
        RKISP_DPRINT(level, " RK_ISP_HIST_H_OFFS: %d\n",
                     READ_RKISP_REG(RK_ISP_HIST_H_OFFS));
        RKISP_DPRINT(level, " RK_ISP_HIST_H_SIZE: %d\n",
                     READ_RKISP_REG(RK_ISP_HIST_H_SIZE));
        RKISP_DPRINT(level, " RK_ISP_HIST_V_OFFS: %d\n",
                     READ_RKISP_REG(RK_ISP_HIST_V_OFFS));
        RKISP_DPRINT(level, " RK_ISP_HIST_V_SIZE: %d\n",
                     READ_RKISP_REG(RK_ISP_HIST_V_SIZE));
        RKISP_DPRINT(level,
                     "#### %s: HST Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;

    case RK_ISP_MODULE_AEC:
    {
        RKISP_DPRINT(level,
                     "#### %s: AEC Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level, " RK_ISP_EXP_CTRL: %d\n",
                     READ_RKISP_REG(RK_ISP_EXP_CTRL));
        RKISP_DPRINT(level, " RK_ISP_EXP_H_OFFSET: %d\n",
                     READ_RKISP_REG(RK_ISP_EXP_H_OFFSET));
        RKISP_DPRINT(level, " RK_ISP_EXP_V_OFFSET: %d\n",
                     READ_RKISP_REG(RK_ISP_EXP_V_OFFSET));
        RKISP_DPRINT(level, " RK_ISP_EXP_H_SIZE: %d\n",
                     READ_RKISP_REG(RK_ISP_EXP_H_SIZE));
        RKISP_DPRINT(level, " RK_ISP_EXP_V_SIZE: %d\n",
                     READ_RKISP_REG(RK_ISP_EXP_V_SIZE));
        RKISP_DPRINT(level,
                     "#### %s: AEC Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;

    case RK_ISP_MODULE_CPROC:
    {
        RKISP_DPRINT(level,
                     "#### %s: CPROC Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level, " ctrl: %d\n",
                     READ_RKISP_REG(RK_C_PROC_CTRL));
        RKISP_DPRINT(level, " contrast: %d\n",
                     READ_RKISP_REG(RK_C_PROC_CONTRAST));
        RKISP_DPRINT(level, " hue: %d\n",
                     READ_RKISP_REG(RK_C_PROC_HUE));
        RKISP_DPRINT(level, " sat: %d\n",
                     READ_RKISP_REG(RK_C_PROC_SATURATION));
        RKISP_DPRINT(level, " brightness: %d\n",
                     READ_RKISP_REG(RK_C_PROC_BRIGHTNESS));
        RKISP_DPRINT(level,
                     "#### %s: CPROC Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;
    case RK_ISP_MODULE_AFC:
    {
        RKISP_DPRINT(level,
                     "#### %s: AFC Registers - BEGIN ####\n",
                     RK_ISP_DEVICE_NAME);
        RKISP_DPRINT(level, " afm_ctr: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_CTRL));
        RKISP_DPRINT(level, " afm_lt_a: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_LT_A));
        RKISP_DPRINT(level, " afm_rb_a: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_RB_A));
        RKISP_DPRINT(level, " afm_lt_b: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_LT_B));
        RKISP_DPRINT(level, " afm_rb_b: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_RB_B));
        RKISP_DPRINT(level, " afm_lt_c: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_LT_C));
        RKISP_DPRINT(level, " afm_rb_c: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_RB_C));
        RKISP_DPRINT(level, " afm_thres: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_THRES));
        RKISP_DPRINT(level, " afm_var_shift: %d\n",
                     READ_RKISP_REG(RK_ISP_AFM_VAR_SHIFT));
        RKISP_DPRINT(level,
                     "#### %s: YCFLT Registers - END ####\n",
                     RK_ISP_DEVICE_NAME);
    }
    break;
    default:
        RKISP_DPRINT(level, "####%s: Invalid Module ID ####\n",
                     RK_ISP_DEVICE_NAME);
        break;
    }
}

#endif
