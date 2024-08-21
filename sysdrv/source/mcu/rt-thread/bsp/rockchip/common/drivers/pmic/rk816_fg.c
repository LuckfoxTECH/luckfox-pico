/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    rk816_fg.c
  * @version V0.1
  * @brief   charger interface
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-12-26     Joseph Chen     first implementation
  *
  ******************************************************************************
  */

#include <rtdevice.h>
#include <rtthread.h>

#ifdef RT_USING_FG_RK816
#include <drivers/fuel_gauge.h>
#include "drv_pmic.h"
#include "rk816.h"
#include "rk816_fg.h"

static int dbg_enable = 0;

#define DBG(args...) \
    do { \
        if (dbg_enable) { \
            rt_kprintf(args); \
        } \
    } while (0)


#define BAT_INFO(fmt, ...)          rt_kprintf("rk816: "fmt, ##__VA_ARGS__)

/* dsoc calib param */
#define TERM_CHRG_DSOC              88
#define TERM_CHRG_CURR              600
#define TERM_CHRG_K                 650
#define SIMULATE_CHRG_INTV          8
#define SIMULATE_CHRG_CURR          400
#define SIMULATE_CHRG_K             1500
#define FULL_CHRG_K                 400

/* zero algorithm */
#define MIN_ZERO_DSOC_ACCURACY      10  /*0.01%*/
#define MIN_ZERO_OVERCNT            100
#define MIN_ACCURACY                1
#define DEF_PWRPATH_RES             50
#define WAIT_DSOC_DROP_SEC          15
#define WAIT_SHTD_DROP_SEC          30
#define MIN_ZERO_GAP_XSOC1          10
#define MIN_ZERO_GAP_XSOC2          5
#define MIN_ZERO_GAP_XSOC3          3
#define MIN_ZERO_GAP_CALIB          5
#define MINUTE(x)                   ((x) * 60)
#define ZERO_LOAD_LVL1              1400
#define ZERO_LOAD_LVL2              600
#define MIN_FCC                     500

enum
{
    MODE_ZERO = 0,
    MODE_FINISH,
    MODE_SMOOTH_CHRG,
    MODE_SMOOTH_DISCHRG,
    MODE_SMOOTH,
};

struct battery_pdata
{
    rt_uint32_t ocv_table[21];
    rt_uint32_t ocv_size;
    rt_uint32_t zero_table[21];
    rt_uint32_t zero_size;

    rt_uint32_t pwroff_vol;
    rt_uint32_t monitor_sec;
    rt_uint32_t zero_algorithm_vol;
    rt_uint32_t zero_reserve_dsoc;
    rt_uint32_t bat_res;
    rt_uint32_t design_capacity;
    rt_uint32_t design_qmax;
    rt_uint32_t sleep_enter_current;
    rt_uint32_t sleep_exit_current;
    rt_uint32_t sleep_filter_current;
    rt_uint32_t max_soc_offset;
    rt_uint32_t vritual_mode;
    rt_uint32_t energy_mode;
    rt_uint32_t sample_res;
};

struct rk816_battery
{
    struct battery_pdata    *pdata;
    rt_int32_t    bat_res;
    rt_int32_t    chrg_status;
    rt_int32_t    res_fac;
    rt_int32_t    over_20mR;
    rt_int8_t     is_initialized;
    rt_int8_t     bat_first_power_on;
    rt_int8_t     exist;
    rt_int8_t     status;
    rt_int32_t    current_avg;
    rt_int32_t    current_relax;
    rt_int32_t    current_chrg;
    rt_int32_t    voltage_avg;
    rt_int32_t    voltage_ocv;
    rt_int32_t    voltage_relax;
    rt_int32_t    voltage_k;
    rt_int32_t    voltage_b;
    rt_int32_t    remain_cap;
    rt_int32_t    design_cap;
    rt_int32_t    nac;
    rt_int32_t    fcc;
    rt_int32_t    qmax;
    rt_int32_t    dsoc;
    rt_int32_t    rsoc;
    rt_int32_t    poffset;
    rt_int32_t    fake_offline;
    rt_int32_t    zero_timeout_cnt;
    rt_int32_t    zero_remain_cap;
    rt_int32_t    zero_dsoc;
    rt_int32_t    zero_linek;
    rt_uint64_t   zero_drop_sec;
    rt_uint64_t   shtd_drop_sec;
    rt_int32_t    sm_remain_cap;
    rt_int32_t    sm_linek;
    rt_int32_t    sm_chrg_dsoc;
    rt_int32_t    sm_dischrg_dsoc;
    rt_int32_t    sleep_sum_cap;
    rt_int32_t    sleep_remain_cap;
    rt_uint32_t   sleep_dischrg_sec;
    rt_uint32_t   sleep_sum_sec;
    rt_int8_t     sleep_chrg_online;
    rt_uint8_t    sleep_chrg_status;
    rt_int8_t     s2r; /*suspend to resume*/
    rt_uint32_t   work_mode;
    rt_tick_t     monitor_ticks;
    rt_uint32_t   pwroff_min;
    rt_uint32_t   chrg_finish_base;
    rt_uint32_t   flat_match_sec;
    rt_uint8_t    halt_cnt;
    rt_int8_t     is_halt;
    rt_int8_t     is_max_soc_offset;
    rt_int8_t     is_sw_reset;
    rt_int8_t     is_ocv_calib;
    rt_int8_t     is_first_on;
    rt_int8_t     is_force_calib;
    rt_int32_t    last_dsoc;
    rt_int32_t    ocv_pre_dsoc;
    rt_int32_t    ocv_new_dsoc;
    rt_int32_t    max_pre_dsoc;
    rt_int32_t    max_new_dsoc;
    rt_int32_t    force_pre_dsoc;
    rt_int32_t    force_new_dsoc;
};

#define DIV(x)  ((x) ? (x) : 1)
#define abs(X)  (((X) < 0) ? (-(X)) : (X))
#define DIV_ROUND_UP(n,d) (((n) + (d) - 1) / (d))

static struct pwr_i2c_desc *fg_i2c;
static struct rk816_fg_platform_data *fg_pdata = &rk816_fg_pdata;
static struct rk816_battery fg_di;

static rt_time_t get_boot_sec(void)
{
    return rt_tick_get() / RT_TICK_PER_SECOND;
}

static rt_uint32_t base2sec(rt_time_t x)
{
    return (get_boot_sec() > x) ? (get_boot_sec() - x) : 0;
}

static rt_uint32_t interpolate(rt_int32_t value, rt_uint32_t *table, rt_int32_t size)
{
    rt_uint8_t i;
    rt_uint16_t d;

    for (i = 0; i < size; i++)
    {
        if (value < table[i])
            break;
    }

    if ((i > 0) && (i < size))
    {
        d = (value - table[i - 1]) * (MAX_INTERPOLATE / (size - 1));
        d /= table[i] - table[i - 1];
        d = d + (i - 1) * (MAX_INTERPOLATE / (size - 1));
    }
    else
    {
        d = i * ((MAX_INTERPOLATE + size / 2) / size);
    }

    if (d > 1000)
        d = 1000;

    return d;
}

/* (a*b)/c */
static int32_t ab_div_c(rt_uint32_t a, rt_uint32_t b, rt_uint32_t c)
{
    rt_uint32_t ans = MAX_INT;
    int32_t tmp;
    rt_int8_t sign;

    sign = ((((a ^ b) ^ c) & 0x80000000) != 0);
    if (c != 0)
    {
        if (sign)
            c = -c;
        tmp = (a * b + (c >> 1)) / c;
        if (tmp < MAX_INT)
            ans = tmp;
    }

    if (sign)
        ans = -ans;

    return ans;
}

static rt_int32_t rk816_bat_read(struct rk816_battery *di, rt_uint8_t reg)
{
    struct rt_i2c_bus_device *bus = fg_i2c->bus;
    rt_uint8_t addr = fg_i2c->addr;
    rt_int32_t val;

    val = pmic_read(bus, addr, reg);
    if (val < 0)
        rt_kprintf("failed to read reg: 0x%x\n", reg);

    return val;
}

static rt_int32_t rk816_bat_write(struct rk816_battery *di,
                                  rt_uint8_t reg, rt_uint8_t buf)
{
    struct rt_i2c_bus_device *bus = fg_i2c->bus;
    rt_uint8_t addr = fg_i2c->addr;
    rt_int32_t ret;

    ret = pmic_write(bus, addr, reg, buf);
    if (ret < 0)
        rt_kprintf("failed to write reg: 0x%x\n", reg);

    return ret;
}

static rt_int32_t rk816_bat_get_coulomb_cap(struct rk816_battery *di)
{
    rt_int32_t cap, val = 0;

    val |= rk816_bat_read(di, GASCNT_REG3) << 24;
    val |= rk816_bat_read(di, GASCNT_REG2) << 16;
    val |= rk816_bat_read(di, GASCNT_REG1) << 8;
    val |= rk816_bat_read(di, GASCNT_REG0) << 0;

    if (!di->over_20mR)
        cap = RES_FAC_MUX(val / 2390, di->res_fac);
    else
        cap = RES_FAC_DIV(val / 2390, di->res_fac);

    return cap;
}

static rt_int32_t rk816_bat_get_rsoc(struct rk816_battery *di)
{
    rt_int32_t remain_cap;

    remain_cap = rk816_bat_get_coulomb_cap(di);
    return (remain_cap + di->fcc / 200) * 100 / DIV(di->fcc);
}

static void rk816_bat_enable_gauge(struct rk816_battery *di)
{
    rt_uint8_t buf;

    buf = rk816_bat_read(di, TS_CTRL_REG);
    buf |= GG_EN;
    rk816_bat_write(di, TS_CTRL_REG, buf);
}

static rt_int32_t rk816_bat_get_vcalib0(struct rk816_battery *di)
{
    rt_int32_t val = 0;

    val |= rk816_bat_read(di, VCALIB0_REGL) << 0;
    val |= rk816_bat_read(di, VCALIB0_REGH) << 8;

    return val;
}

static rt_int32_t rk816_bat_get_vcalib1(struct rk816_battery *di)
{
    rt_int32_t val = 0;

    val |= rk816_bat_read(di, VCALIB1_REGL) << 0;
    val |= rk816_bat_read(di, VCALIB1_REGH) << 8;

    return val;
}

static rt_int32_t rk816_bat_get_ioffset(struct rk816_battery *di)
{
    rt_int32_t val = 0;

    val |= rk816_bat_read(di, IOFFSET_REGL) << 0;
    val |= rk816_bat_read(di, IOFFSET_REGH) << 8;

    return val;
}

static rt_int32_t rk816_bat_get_coffset(struct rk816_battery *di)
{
    rt_int32_t val = 0;

    val |= rk816_bat_read(di, CAL_OFFSET_REGL) << 0;
    val |= rk816_bat_read(di, CAL_OFFSET_REGH) << 8;

    return val;
}

static void rk816_bat_init_voltage_kb(struct rk816_battery *di)
{
    rt_int32_t vcalib0, vcalib1;

    vcalib0 = rk816_bat_get_vcalib0(di);
    vcalib1 = rk816_bat_get_vcalib1(di);
    di->voltage_k = (4200 - 3000) * 1000 / DIV(vcalib1 - vcalib0);
    di->voltage_b = 4200 - (di->voltage_k * vcalib1) / 1000;

    DBG("voltage, k=%d(*1000), b=%d\n", di->voltage_k, di->voltage_b);
}

static rt_int32_t rk816_bat_get_ocv_voltage(struct rk816_battery *di)
{
    rt_int32_t vol, val = 0;

    val |= rk816_bat_read(di, BAT_OCV_REGL) << 0;
    val |= rk816_bat_read(di, BAT_OCV_REGH) << 8;
    vol = di->voltage_k * val / 1000 + di->voltage_b;

    return (vol * 1100 / 1000);
}

static rt_int32_t rk816_bat_get_avg_voltage(struct rk816_battery *di)
{
    rt_int32_t vol, val = 0;

    val |= rk816_bat_read(di, BAT_VOL_REGL) << 0;
    val |= rk816_bat_read(di, BAT_VOL_REGH) << 8;
    vol = di->voltage_k * val / 1000 + di->voltage_b;

    return (vol * 1100 / 1000);
}

static rt_int8_t is_rk816_bat_relax_mode(struct rk816_battery *di)
{
    rt_uint8_t status;

    status = rk816_bat_read(di, GGSTS_REG);
    if (!(status & RELAX_VOL1_UPD) || !(status & RELAX_VOL2_UPD))
        return false;
    else
        return true;
}

static rt_uint16_t rk816_bat_get_relax_vol1(struct rk816_battery *di)
{
    rt_uint16_t vol, val = 0;

    val |= rk816_bat_read(di, RELAX_VOL1_REGL) << 0;
    val |= rk816_bat_read(di, RELAX_VOL1_REGH) << 8;
    vol = di->voltage_k * val / 1000 + di->voltage_b;

    return (vol * 1100 / 1000);
}

static rt_uint16_t rk816_bat_get_relax_vol2(struct rk816_battery *di)
{
    rt_uint16_t vol, val = 0;

    val |= rk816_bat_read(di, RELAX_VOL2_REGL) << 0;
    val |= rk816_bat_read(di, RELAX_VOL2_REGH) << 8;
    vol = di->voltage_k * val / 1000 + di->voltage_b;

    return (vol * 1100 / 1000);
}

static rt_uint16_t rk816_bat_get_relax_voltage(struct rk816_battery *di)
{
    rt_uint16_t relax_vol1, relax_vol2;

    if (!is_rk816_bat_relax_mode(di))
        return 0;

    relax_vol1 = rk816_bat_get_relax_vol1(di);
    relax_vol2 = rk816_bat_get_relax_vol2(di);

    return relax_vol1 > relax_vol2 ? relax_vol1 : relax_vol2;
}

static rt_int32_t rk816_bat_get_avg_current(struct rk816_battery *di)
{
    rt_int32_t cur, val = 0;

    val |= rk816_bat_read(di, BAT_CUR_AVG_REGL) << 0;
    val |= rk816_bat_read(di, BAT_CUR_AVG_REGH) << 8;
    if (val & 0x800)
        val -= 4096;

    if (!di->over_20mR)
        cur = RES_FAC_MUX(val * 1506, di->res_fac) / 1000;
    else
        cur = RES_FAC_DIV(val * 1506, di->res_fac) / 1000;

    return cur;
}

static rt_int32_t rk816_bat_get_relax_cur1(struct rk816_battery *di)
{
    rt_int32_t val = 0;

    val |= rk816_bat_read(di, RELAX_CUR1_REGL) << 0;
    val |= rk816_bat_read(di, RELAX_CUR1_REGH) << 8;
    if (val & 0x800)
        val -= 4096;

    return (val * 1506 / 1000);
}

static rt_int32_t rk816_bat_get_relax_cur2(struct rk816_battery *di)
{
    rt_int32_t val = 0;

    val |= rk816_bat_read(di, RELAX_CUR2_REGL) << 0;
    val |= rk816_bat_read(di, RELAX_CUR2_REGH) << 8;
    if (val & 0x800)
        val -= 4096;

    return (val * 1506 / 1000);
}

static rt_int32_t rk816_bat_get_relax_current(struct rk816_battery *di)
{
    rt_int32_t relax_cur1, relax_cur2;

    if (!is_rk816_bat_relax_mode(di))
        return 0;

    relax_cur1 = rk816_bat_get_relax_cur1(di);
    relax_cur2 = rk816_bat_get_relax_cur2(di);

    return (relax_cur1 < relax_cur2) ? relax_cur1 : relax_cur2;
}

static rt_int32_t rk816_bat_vol_to_ocvsoc(struct rk816_battery *di,
        rt_int32_t voltage)
{
    rt_uint32_t *ocv_table, temp;
    rt_int32_t ocv_size, ocv_soc;

    ocv_table = di->pdata->ocv_table;
    ocv_size = di->pdata->ocv_size;
    temp = interpolate(voltage, ocv_table, ocv_size);
    ocv_soc = ab_div_c(temp, MAX_PERCENTAGE, MAX_INTERPOLATE);

    return ocv_soc;
}

static rt_int32_t rk816_bat_vol_to_ocvcap(struct rk816_battery *di,
        rt_int32_t voltage)
{
    rt_uint32_t *ocv_table, temp;
    rt_int32_t ocv_size, cap;

    ocv_table = di->pdata->ocv_table;
    ocv_size = di->pdata->ocv_size;
    temp = interpolate(voltage, ocv_table, ocv_size);
    cap = ab_div_c(temp, di->fcc, MAX_INTERPOLATE);

    return cap;
}

static rt_int32_t rk816_bat_vol_to_zerosoc(struct rk816_battery *di,
        rt_int32_t voltage)
{
    rt_uint32_t *ocv_table, temp;
    rt_int32_t ocv_size, ocv_soc;

    ocv_table = di->pdata->zero_table;
    ocv_size = di->pdata->ocv_size;
    temp = interpolate(voltage, ocv_table, ocv_size);
    ocv_soc = ab_div_c(temp, MAX_PERCENTAGE, MAX_INTERPOLATE);

    return ocv_soc;
}

static rt_int32_t rk816_bat_vol_to_zerocap(struct rk816_battery *di,
        rt_int32_t voltage)
{
    rt_uint32_t *ocv_table, temp;
    rt_int32_t ocv_size, cap;

    ocv_table = di->pdata->zero_table;
    ocv_size = di->pdata->ocv_size;
    temp = interpolate(voltage, ocv_table, ocv_size);
    cap = ab_div_c(temp, di->fcc, MAX_INTERPOLATE);

    return cap;
}

static void rk816_bat_set_ioffset_sample(struct rk816_battery *di)
{
    rt_uint8_t ggcon;

    ggcon = rk816_bat_read(di, GGCON_REG);
    ggcon &= ~ADC_CAL_MIN_MSK;
    ggcon |= ADC_CAL_8MIN;
    rk816_bat_write(di, GGCON_REG, ggcon);
}

static void rk816_bat_set_ocv_sample(struct rk816_battery *di)
{
    rt_uint8_t ggcon;

    ggcon = rk816_bat_read(di, GGCON_REG);
    ggcon &= ~OCV_SAMP_MIN_MSK;
    ggcon |= OCV_SAMP_8MIN;
    rk816_bat_write(di, GGCON_REG, ggcon);
}

static void rk816_bat_restart_relax(struct rk816_battery *di)
{
    rt_uint8_t ggsts;

    ggsts = rk816_bat_read(di, GGSTS_REG);
    ggsts &= ~RELAX_VOL12_UPD_MSK;
    rk816_bat_write(di, GGSTS_REG, ggsts);
}

static void rk816_bat_set_relax_sample(struct rk816_battery *di)
{
    rt_uint8_t buf;
    rt_int32_t enter_thres, exit_thres, filter_thres;
    struct battery_pdata *pdata = di->pdata;


    filter_thres = pdata->sleep_filter_current * 1000 / 1506;

    if (!di->over_20mR)
    {
        enter_thres = RES_FAC_DIV(pdata->sleep_enter_current * 1000,
                                  di->res_fac) / 1506;
        exit_thres = RES_FAC_DIV(pdata->sleep_exit_current * 1000,
                                 di->res_fac) / 1506;
    }
    else
    {
        enter_thres = RES_FAC_MUX(pdata->sleep_enter_current * 1000,
                                  di->res_fac) / 1506;
        exit_thres = RES_FAC_MUX(pdata->sleep_exit_current * 1000,
                                 di->res_fac) / 1506;
    }

    /* set relax enter and exit threshold */
    buf = enter_thres & 0xff;
    rk816_bat_write(di, RELAX_ENTRY_THRES_REGL, buf);
    buf = (enter_thres >> 8) & 0xff;
    rk816_bat_write(di, RELAX_ENTRY_THRES_REGH, buf);

    buf = exit_thres & 0xff;
    rk816_bat_write(di, RELAX_EXIT_THRES_REGL, buf);
    buf = (exit_thres >> 8) & 0xff;
    rk816_bat_write(di, RELAX_EXIT_THRES_REGH, buf);

    /* set sample current threshold */
    buf = filter_thres & 0xff;
    rk816_bat_write(di, SLEEP_CON_SAMP_CUR_REG, buf);

    /* reset relax update state */
    rk816_bat_restart_relax(di);
}

/* high load: current < 0 with charger in.
 * System will not shutdown while dsoc=0% with charging state(ac_in),
 * which will cause over discharge, so oppose status before report states.
 */
static void rk816_bat_lowpwr_check(struct rk816_battery *di)
{
    static rt_uint64_t time;
    rt_int32_t pwr_off_thresd = di->pdata->pwroff_vol;

    if (di->current_avg < 0 && di->voltage_avg < pwr_off_thresd)
    {
        if (!time)
            time = get_boot_sec();

        if ((base2sec(time) > MINUTE(1)) ||
                (di->voltage_avg <= pwr_off_thresd - 50))
        {
            di->fake_offline = 1;
            if (di->voltage_avg <= pwr_off_thresd - 50)
                di->dsoc--;
            BAT_INFO("low power, soc=%d, current=%d\n",
                     di->dsoc, di->current_avg);
        }
    }
    else
    {
        time = 0;
        di->fake_offline = 0;
    }

    DBG("<%s>. t=%lu, dsoc=%d, current=%d, fake_offline=%d\n",
        __func__, base2sec(time), di->dsoc,
        di->current_avg, di->fake_offline);
}

static int is_rk816_bat_exist(struct rk816_battery *di)
{
    return !!(rk816_bat_read(di, SUP_STS_REG) & BAT_EXS);
}

static rt_int8_t is_rk816_bat_first_pwron(struct rk816_battery *di)
{
    rt_uint8_t buf;

    buf = rk816_bat_read(di, GGSTS_REG);
    if (buf & BAT_CON)
    {
        buf &= ~BAT_CON;
        rk816_bat_write(di, GGSTS_REG, buf);
        return true;
    }

    return false;
}

static rt_uint8_t rk816_bat_get_pwroff_min(struct rk816_battery *di)
{
    rt_uint8_t now_min, last_min;

    now_min = rk816_bat_read(di, NON_ACT_TIMER_CNT_REG);
    last_min = rk816_bat_read(di, NON_ACT_TIMER_CNT_REG_SAVE);
    rk816_bat_write(di, NON_ACT_TIMER_CNT_REG_SAVE, now_min);

    return (now_min != last_min) ? now_min : 0;
}

static rt_uint8_t is_rk816_bat_initialized(struct rk816_battery *di)
{
    rt_uint8_t val = rk816_bat_read(di, MISC_MARK_REG);

    if (val & FG_INIT)
    {
        val &= ~FG_INIT;
        rk816_bat_write(di, MISC_MARK_REG, val);
        return true;
    }
    else
    {
        return false;
    }
}

static rt_int8_t is_rk816_bat_ocv_valid(struct rk816_battery *di)
{
    return (!di->is_initialized && di->pwroff_min >= 30) ? true : false;
}

static void rk816_bat_save_cap(struct rk816_battery *di, rt_int32_t capacity)
{
    rt_uint8_t buf;
    static rt_uint32_t old_cap;

    if (capacity >= di->qmax)
        capacity = di->qmax;
    if (capacity <= 0)
        capacity = 0;
    if (old_cap == capacity)
        return;

    old_cap = capacity;
    buf = (capacity >> 24) & 0xff;
    rk816_bat_write(di, REMAIN_CAP_REG3, buf);
    buf = (capacity >> 16) & 0xff;
    rk816_bat_write(di, REMAIN_CAP_REG2, buf);
    buf = (capacity >> 8) & 0xff;
    rk816_bat_write(di, REMAIN_CAP_REG1, buf);
    buf = (capacity >> 0) & 0xff;
    rk816_bat_write(di, REMAIN_CAP_REG0, buf);
}

static rt_int32_t rk816_bat_get_prev_cap(struct rk816_battery *di)
{
    rt_int32_t val = 0;

    val |= rk816_bat_read(di, REMAIN_CAP_REG3) << 24;
    val |= rk816_bat_read(di, REMAIN_CAP_REG2) << 16;
    val |= rk816_bat_read(di, REMAIN_CAP_REG1) << 8;
    val |= rk816_bat_read(di, REMAIN_CAP_REG0) << 0;

    return val;
}

static void rk816_bat_save_fcc(struct rk816_battery *di, rt_uint32_t fcc)
{
    rt_uint8_t buf;

    buf = (fcc >> 24) & 0xff;
    rk816_bat_write(di, NEW_FCC_REG3, buf);
    buf = (fcc >> 16) & 0xff;
    rk816_bat_write(di, NEW_FCC_REG2, buf);
    buf = (fcc >> 8) & 0xff;
    rk816_bat_write(di, NEW_FCC_REG1, buf);
    buf = (fcc >> 0) & 0xff;
    rk816_bat_write(di, NEW_FCC_REG0, buf);

    BAT_INFO("save fcc: %d\n", fcc);
}

static rt_int32_t rk816_bat_get_fcc(struct rk816_battery *di)
{
    rt_uint32_t fcc = 0;

    fcc |= rk816_bat_read(di, NEW_FCC_REG3) << 24;
    fcc |= rk816_bat_read(di, NEW_FCC_REG2) << 16;
    fcc |= rk816_bat_read(di, NEW_FCC_REG1) << 8;
    fcc |= rk816_bat_read(di, NEW_FCC_REG0) << 0;

    if (fcc < MIN_FCC)
    {
        BAT_INFO("Invalid fcc(%d), use design cap", fcc);
        fcc = di->pdata->design_capacity;
        rk816_bat_save_fcc(di, fcc);
    }
    else if (fcc > di->pdata->design_qmax)
    {
        BAT_INFO("Invalid fcc(%d), use qmax", fcc);
        fcc = di->pdata->design_qmax;
        rk816_bat_save_fcc(di, fcc);
    }

    return fcc;
}

static void rk816_bat_save_dsoc(struct rk816_battery *di, rt_uint8_t save_soc)
{
    static rt_int32_t last_soc = -1;

    if (last_soc != save_soc)
    {
        rk816_bat_write(di, SOC_REG, save_soc);
        last_soc = save_soc;
    }
}

static rt_int32_t rk816_bat_get_prev_dsoc(struct rk816_battery *di)
{
    return rk816_bat_read(di, SOC_REG);
}

static void rk816_bat_init_coulomb_cap(struct rk816_battery *di,
                                       rt_uint32_t capacity)
{
    rt_uint8_t buf;
    rt_uint32_t cap;

    if (!di->over_20mR)
        cap = RES_FAC_DIV(capacity * 2390, di->res_fac);
    else
        cap = RES_FAC_MUX(capacity * 2390, di->res_fac);

    buf = (cap >> 24) & 0xff;
    rk816_bat_write(di, GASCNT_CAL_REG3, buf);
    buf = (cap >> 16) & 0xff;
    rk816_bat_write(di, GASCNT_CAL_REG2, buf);
    buf = (cap >> 8) & 0xff;
    rk816_bat_write(di, GASCNT_CAL_REG1, buf);
    buf = (cap >> 0) & 0xff;
    rk816_bat_write(di, GASCNT_CAL_REG0, buf);

    di->remain_cap = capacity;
    di->rsoc = rk816_bat_get_rsoc(di);
}

static rt_uint8_t rk816_bat_get_halt_cnt(struct rk816_battery *di)
{
    return rk816_bat_read(di, HALT_CNT_REG);
}

static void rk816_bat_inc_halt_cnt(struct rk816_battery *di)
{
    rt_uint8_t cnt;

    cnt = rk816_bat_read(di, HALT_CNT_REG);
    rk816_bat_write(di, HALT_CNT_REG, ++cnt);
}

static rt_int8_t is_rk816_bat_last_halt(struct rk816_battery *di)
{
    rt_int32_t pre_cap = rk816_bat_get_prev_cap(di);
    rt_int32_t now_cap = rk816_bat_get_coulomb_cap(di);

    /* over 10%: system halt last time */
    if (abs(now_cap - pre_cap) > (di->fcc / 10))
    {
        rk816_bat_inc_halt_cnt(di);
        return true;
    }
    else
    {
        return false;
    }
}

static void rk816_bat_first_pwron(struct rk816_battery *di)
{
    rt_int32_t ocv_vol;

    rk816_bat_save_fcc(di, di->design_cap);
    ocv_vol = rk816_bat_get_ocv_voltage(di);
    di->fcc = rk816_bat_get_fcc(di);
    di->nac = rk816_bat_vol_to_ocvcap(di, ocv_vol);
    di->rsoc = rk816_bat_vol_to_ocvsoc(di, ocv_vol);
    di->dsoc = di->rsoc;
    di->is_first_on = true;

    BAT_INFO("first on: dsoc=%d, rsoc=%d cap=%d, fcc=%d, ov=%d\n",
             di->dsoc, di->rsoc, di->nac, di->fcc, ocv_vol);
}

static void rk816_bat_not_first_pwron(struct rk816_battery *di)
{
    rt_int32_t now_cap, pre_soc, pre_cap, ocv_cap, ocv_soc, ocv_vol;

    di->fcc = rk816_bat_get_fcc(di);
    pre_soc = rk816_bat_get_prev_dsoc(di);
    pre_cap = rk816_bat_get_prev_cap(di);
    now_cap = rk816_bat_get_coulomb_cap(di);
    di->is_halt = is_rk816_bat_last_halt(di);
    di->halt_cnt = rk816_bat_get_halt_cnt(di);
    di->is_initialized = is_rk816_bat_initialized(di);
    di->is_ocv_calib = is_rk816_bat_ocv_valid(di);

    if (di->is_initialized)
    {
        BAT_INFO("initialized yet..\n");
        goto finish;
    }
    else if (di->is_halt)
    {
        BAT_INFO("system halt last time... cap: pre=%d, now=%d\n",
                 pre_cap, now_cap);
        if (now_cap < 0)
            now_cap = 0;
        rk816_bat_init_coulomb_cap(di, now_cap);
        pre_cap = now_cap;
        pre_soc = di->rsoc;
        goto finish;
    }
    else if (di->is_ocv_calib)
    {
        ocv_vol = rk816_bat_get_ocv_voltage(di);
        ocv_soc = rk816_bat_vol_to_ocvsoc(di, ocv_vol);
        ocv_cap = rk816_bat_vol_to_ocvcap(di, ocv_vol);
        pre_cap = ocv_cap;
        di->ocv_pre_dsoc = pre_soc;
        di->ocv_new_dsoc = ocv_soc;
        if (abs(ocv_soc - pre_soc) >= di->pdata->max_soc_offset)
        {
            di->ocv_pre_dsoc = pre_soc;
            di->ocv_new_dsoc = ocv_soc;
            di->is_max_soc_offset = true;
            BAT_INFO("trigger max soc offset, dsoc: %d -> %d\n",
                     pre_soc, ocv_soc);
            pre_soc = ocv_soc;
        }
        BAT_INFO("OCV calib: cap=%d, rsoc=%d\n", ocv_cap, ocv_soc);
    }
    else if (di->pwroff_min > 0)
    {
        ocv_vol = rk816_bat_get_ocv_voltage(di);
        ocv_soc = rk816_bat_vol_to_ocvsoc(di, ocv_vol);
        ocv_cap = rk816_bat_vol_to_ocvcap(di, ocv_vol);
        di->force_pre_dsoc = pre_soc;
        di->force_new_dsoc = ocv_soc;
        if (abs(ocv_soc - pre_soc) >= 80)
        {
            di->is_force_calib = true;
            BAT_INFO("dsoc force calib: %d -> %d\n",
                     pre_soc, ocv_soc);
            pre_soc = ocv_soc;
            pre_cap = ocv_cap;
        }
    }

finish:
    di->dsoc = pre_soc;
    di->nac = pre_cap;
    if (di->nac < 0)
        di->nac = 0;

    rk816_bat_init_coulomb_cap(di, di->nac);

    BAT_INFO("dl=%d rl=%d, cap=%d c=%d v=%d ov=%d rv=%d min=%d pdl=%d pcap=%d\n",
             di->dsoc, di->rsoc, di->nac, rk816_bat_get_avg_current(di),
             rk816_bat_get_avg_voltage(di), rk816_bat_get_ocv_voltage(di),
             rk816_bat_get_relax_voltage(di),
             di->pwroff_min, rk816_bat_get_prev_dsoc(di),
             rk816_bat_get_prev_cap(di));
}

static rt_int8_t rk816_bat_ocv_sw_reset(struct rk816_battery *di)
{
    rt_uint8_t buf;

    buf = rk816_bat_read(di, MISC_MARK_REG);
    if (((buf & FG_RESET_LATE) && di->pwroff_min >= 30) ||
            (buf & FG_RESET_NOW))
    {
        buf &= ~FG_RESET_LATE;
        buf &= ~FG_RESET_NOW;
        rk816_bat_write(di, MISC_MARK_REG, buf);
        BAT_INFO("manual reset fuel gauge\n");
        return true;
    }
    else
    {
        return false;
    }
}

static void rk816_bat_init_rsoc(struct rk816_battery *di)
{
    di->bat_first_power_on = is_rk816_bat_first_pwron(di);
    di->is_sw_reset = rk816_bat_ocv_sw_reset(di);
    di->pwroff_min = rk816_bat_get_pwroff_min(di);

    if (di->bat_first_power_on || di->is_sw_reset)
        rk816_bat_first_pwron(di);
    else
        rk816_bat_not_first_pwron(di);
}

static rt_uint8_t rk816_bat_get_chrg_status(struct rk816_battery *di)
{
    rt_uint8_t status;

    status = rk816_bat_read(di, SUP_STS_REG) & CHRG_STATUS_MSK;
    switch (status)
    {
    case CHARGE_OFF:
        DBG("CHARGE-OFF ...\n");
        break;
    case DEAD_CHARGE:
        BAT_INFO("DEAD CHARGE...\n");
        break;
    case TRICKLE_CHARGE:
        BAT_INFO("TRICKLE CHARGE...\n ");
        break;
    case CC_OR_CV:
        DBG("CC or CV...\n");
        break;
    case CHARGE_FINISH:
        DBG("CHARGE FINISH...\n");
        break;
    case USB_OVER_VOL:
        BAT_INFO("USB OVER VOL...\n");
        break;
    case BAT_TMP_ERR:
        BAT_INFO("BAT TMP ERROR...\n");
        break;
    case TIMER_ERR:
        BAT_INFO("TIMER ERROR...\n");
        break;
    case USB_EXIST:
        BAT_INFO("USB EXIST...\n");
        break;
    case USB_EFF:
        BAT_INFO("USB EFF...\n");
        break;
    default:
        BAT_INFO("UNKNOWN STATUS...\n");
        break;
    }

    return status;
}

static void rk816_bat_init_poffset(struct rk816_battery *di)
{
    rt_int32_t coffset, ioffset;

    coffset = rk816_bat_get_coffset(di);
    ioffset = rk816_bat_get_ioffset(di);
    di->poffset = coffset - ioffset;
}

static void rk816_bat_select_sample_res(struct rk816_battery *di)
{
    if (di->pdata->sample_res == 20)
    {
        di->over_20mR = 0;
        di->res_fac = 10;
    }
    else if (di->pdata->sample_res > 20)
    {
        di->over_20mR = 1;
        di->res_fac = di->pdata->sample_res * 10 / 20;
    }
    else
    {
        di->over_20mR = 0;
        di->res_fac = 20 * 10 / di->pdata->sample_res;
    }
}

static void rk816_bat_init_zero_table(struct rk816_battery *di)
{
    rt_int32_t i, diff, min, max;
    size_t ocv_size;

    ocv_size = di->pdata->ocv_size;
    min = di->pdata->pwroff_vol,
    max = di->pdata->ocv_table[ocv_size - 4];
    diff = (max - min) / DIV(ocv_size - 1);
    for (i = 0; i < ocv_size; i++)
        di->pdata->zero_table[i] = min + (i * diff);
}

static void rk816_bat_calc_sm_linek(struct rk816_battery *di)
{
    rt_int32_t linek, current_avg;
    rt_uint8_t diff, delta;

    delta = abs(di->dsoc - di->rsoc);
    diff = delta * 3;/* speed:3/4 */
    current_avg = rk816_bat_get_avg_current(di);
    if (current_avg >= 0)
    {
        if (di->dsoc < di->rsoc)
            linek = 1000 * (delta + diff) / DIV(diff);
        else if (di->dsoc > di->rsoc)
            linek = 1000 * diff / DIV(delta + diff);
        else
            linek = 1000;
    }
    else
    {
        if (di->dsoc < di->rsoc)
            linek = -1000 * diff / DIV(delta + diff);
        else if (di->dsoc > di->rsoc)
            linek = -1000 * (delta + diff) / DIV(diff);
        else
            linek = -1000;
    }

    di->sm_linek = linek;
    di->sm_remain_cap = di->remain_cap;
}

static void rk816_bat_calc_zero_linek(struct rk816_battery *di)
{
    rt_int32_t dead_voltage, ocv_voltage;
    rt_int32_t voltage_avg, current_avg, vsys;
    rt_int32_t ocv_cap, dead_cap, xsoc;
    rt_int32_t ocv_soc, dead_soc;
    rt_int32_t pwroff_vol, org_linek = 0;
    rt_int32_t min_gap_xsoc;

    if ((abs(di->current_avg) < 400) && (di->dsoc > 5))
        pwroff_vol = di->pdata->pwroff_vol + 50;
    else
        pwroff_vol = di->pdata->pwroff_vol;

    /* calc estimate ocv voltage */
    voltage_avg = rk816_bat_get_avg_voltage(di);
    current_avg = rk816_bat_get_avg_current(di);
    vsys = voltage_avg + (current_avg * DEF_PWRPATH_RES) / 1000;

    DBG("ZERO0: shtd_vol: org = %d, now = %d, zero_reserve_dsoc = %d\n",
        di->pdata->pwroff_vol, pwroff_vol, di->pdata->zero_reserve_dsoc);

    dead_voltage = pwroff_vol - current_avg *
                   (di->bat_res + DEF_PWRPATH_RES) / 1000;
    ocv_voltage = voltage_avg - (current_avg * di->bat_res) / 1000;
    DBG("ZERO0: dead_voltage(shtd) = %d, ocv_voltage(now) = %d\n",
        dead_voltage, ocv_voltage);

    /* calc estimate soc and cap */
    dead_soc = rk816_bat_vol_to_zerosoc(di, dead_voltage);
    dead_cap = rk816_bat_vol_to_zerocap(di, dead_voltage);
    DBG("ZERO0: dead_soc = %d, dead_cap = %d\n",
        dead_soc, dead_cap);

    ocv_soc = rk816_bat_vol_to_zerosoc(di, ocv_voltage);
    ocv_cap = rk816_bat_vol_to_zerocap(di, ocv_voltage);
    DBG("ZERO0: ocv_soc = %d, ocv_cap = %d\n",
        ocv_soc, ocv_cap);

    /* xsoc: available rsoc */
    xsoc = ocv_soc - dead_soc;

    /* min_gap_xsoc: reserve xsoc */
    if (abs(current_avg) > ZERO_LOAD_LVL1)
        min_gap_xsoc = MIN_ZERO_GAP_XSOC3;
    else if (abs(current_avg) > ZERO_LOAD_LVL2)
        min_gap_xsoc = MIN_ZERO_GAP_XSOC2;
    else
        min_gap_xsoc = MIN_ZERO_GAP_XSOC1;

    if ((xsoc <= 30) && (di->dsoc >= di->pdata->zero_reserve_dsoc))
        min_gap_xsoc = min_gap_xsoc + MIN_ZERO_GAP_CALIB;

    di->zero_remain_cap = di->remain_cap;
    di->zero_timeout_cnt = 0;
    if ((di->dsoc <= 1) && (xsoc > 0))
    {
        di->zero_linek = 400;
        di->zero_drop_sec = 0;
    }
    else if (xsoc >= 0)
    {
        di->zero_drop_sec = 0;
        di->zero_linek = (di->zero_dsoc + xsoc / 2) / DIV(xsoc);
        org_linek = di->zero_linek;
        /* battery energy mode to use up voltage */
        if ((di->pdata->energy_mode) &&
                (xsoc - di->dsoc >= MIN_ZERO_GAP_XSOC3) &&
                (di->dsoc <= 10) && (di->zero_linek < 300))
        {
            di->zero_linek = 300;
            DBG("ZERO-new: zero_linek adjust step0...\n");
            /* reserve enough power yet, slow down any way */
        }
        else if ((xsoc - di->dsoc >= min_gap_xsoc) ||
                 ((xsoc - di->dsoc >= MIN_ZERO_GAP_XSOC2) &&
                  (di->dsoc <= 10) && (xsoc > 15)))
        {
            if (xsoc <= 20 &&
                    di->dsoc >= di->pdata->zero_reserve_dsoc)
                di->zero_linek = 1200;
            else if (xsoc - di->dsoc >= 2 * min_gap_xsoc)
                di->zero_linek = 400;
            else if (xsoc - di->dsoc >= 3 + min_gap_xsoc)
                di->zero_linek = 600;
            else
                di->zero_linek = 800;
            DBG("ZERO-new: zero_linek adjust step1...\n");
            /* control zero mode beginning enter */
        }
        else if ((di->zero_linek > 1800) && (di->dsoc > 70))
        {
            di->zero_linek = 1800;
            DBG("ZERO-new: zero_linek adjust step2...\n");
            /* dsoc close to xsoc: it must reserve power */
        }
        else if ((di->zero_linek > 1000) && (di->zero_linek < 1200))
        {
            di->zero_linek = 1200;
            DBG("ZERO-new: zero_linek adjust step3...\n");
            /* dsoc[5~15], dsoc < xsoc */
        }
        else if ((di->dsoc <= 15 && di->dsoc > 5) &&
                 (di->zero_linek <= 1200))
        {
            /* slow down */
            if ((xsoc - di->dsoc) >= min_gap_xsoc)
                di->zero_linek = 800;
            /* reserve power */
            else
                di->zero_linek = 1200;
            DBG("ZERO-new: zero_linek adjust step4...\n");
            /* dsoc[5, 100], dsoc < xsoc */
        }
        else if ((di->zero_linek < 1000) && (di->dsoc >= 5))
        {
            if ((xsoc - di->dsoc) < min_gap_xsoc)
            {
                /* reserve power */
                di->zero_linek = 1200;
            }
            else
            {
                if (abs(di->current_avg) > 500)/* heavy */
                    di->zero_linek = 900;
                else
                    di->zero_linek = 1000;
            }
            DBG("ZERO-new: zero_linek adjust step5...\n");
            /* dsoc[0~5], dsoc < xsoc */
        }
        else if ((di->zero_linek < 1000) && (di->dsoc <= 5))
        {
            if ((xsoc - di->dsoc) <= 3)
                di->zero_linek = 1200;
            else
                di->zero_linek = 800;
            DBG("ZERO-new: zero_linek adjust step6...\n");
        }
    }
    else
    {
        /* xsoc < 0 */
        di->zero_linek = 1000;
        if (!di->zero_drop_sec)
            di->zero_drop_sec = get_boot_sec();
        if (base2sec(di->zero_drop_sec) >= WAIT_DSOC_DROP_SEC)
        {
            DBG("ZERO0: t=%lu\n", base2sec(di->zero_drop_sec));
            di->zero_drop_sec = 0;
            di->dsoc--;
            di->zero_dsoc = (di->dsoc + 1) * 1000 -
                            MIN_ACCURACY;
        }
    }

    if (voltage_avg < pwroff_vol - 70)
    {
        if (!di->shtd_drop_sec)
            di->shtd_drop_sec = get_boot_sec();
        if (base2sec(di->shtd_drop_sec) > WAIT_SHTD_DROP_SEC)
        {
            BAT_INFO("voltage extreme low... soc: %d->0. v=%d, c=%d, cap=%d\n",
                     di->dsoc, voltage_avg, current_avg, di->remain_cap);
            di->shtd_drop_sec = 0;
            di->dsoc = 0;
        }
    }
    else
    {
        di->shtd_drop_sec = 0;
    }

    DBG("ZERO-new: org_linek=%d, zero_linek=%d, dsoc=%d, Xsoc=%d, rsoc=%d, gap=%d, v=%d, vsys=%d\n"
        "ZERO-new: di->zero_dsoc=%d, zero_remain_cap=%d, zero_drop=%ld, sht_drop=%ld\n\n",
        org_linek, di->zero_linek, di->dsoc, xsoc, di->rsoc,
        min_gap_xsoc, voltage_avg, vsys, di->zero_dsoc, di->zero_remain_cap,
        base2sec(di->zero_drop_sec), base2sec(di->shtd_drop_sec));
}

static void rk816_bat_finish_algo_prepare(struct rk816_battery *di)
{
    di->chrg_finish_base = get_boot_sec();
    if (!di->chrg_finish_base)
        di->chrg_finish_base = 1;
}

static void rk816_bat_smooth_algo_prepare(struct rk816_battery *di)
{
    rt_int32_t tmp_soc;

    tmp_soc = di->sm_chrg_dsoc / 1000;
    if (tmp_soc != di->dsoc)
        di->sm_chrg_dsoc = di->dsoc * 1000;

    tmp_soc = di->sm_dischrg_dsoc / 1000;
    if (tmp_soc != di->dsoc)
        di->sm_dischrg_dsoc =
            (di->dsoc + 1) * 1000 - MIN_ACCURACY;

    DBG("<%s>. tmp_soc=%d, dsoc=%d, dsoc:sm_dischrg=%d, sm_chrg=%d\n",
        __func__, tmp_soc, di->dsoc, di->sm_dischrg_dsoc, di->sm_chrg_dsoc);

    rk816_bat_calc_sm_linek(di);
}

static void rk816_bat_zero_algo_prepare(struct rk816_battery *di)
{
    rt_int32_t tmp_dsoc;

    di->zero_timeout_cnt = 0;
    tmp_dsoc = di->zero_dsoc / 1000;
    if (tmp_dsoc != di->dsoc)
        di->zero_dsoc = (di->dsoc + 1) * 1000 - MIN_ACCURACY;

    DBG("<%s>. first calc, reinit linek\n", __func__);

    rk816_bat_calc_zero_linek(di);
}

static void rk816_bat_calc_zero_algorithm(struct rk816_battery *di)
{
    rt_int32_t tmp_soc = 0, sm_delta_dsoc = 0;

    tmp_soc = di->zero_dsoc / 1000;
    if (tmp_soc == di->dsoc)
        goto out;

    DBG("<%s>. enter: dsoc=%d, rsoc=%d\n", __func__, di->dsoc, di->rsoc);
    /* when discharge slow down, take sm chrg into calc */
    if (di->dsoc < di->rsoc)
    {
        /* take sm charge rest into calc */
        tmp_soc = di->sm_chrg_dsoc / 1000;
        if (tmp_soc == di->dsoc)
        {
            sm_delta_dsoc = di->sm_chrg_dsoc - di->dsoc * 1000;
            di->sm_chrg_dsoc = di->dsoc * 1000;
            di->zero_dsoc += sm_delta_dsoc;
            DBG("ZERO1: take sm chrg,delta=%d\n", sm_delta_dsoc);
        }
    }

    /* when discharge speed up, take sm dischrg into calc */
    if (di->dsoc > di->rsoc)
    {
        /* take sm discharge rest into calc */
        tmp_soc = di->sm_dischrg_dsoc / 1000;
        if (tmp_soc == di->dsoc)
        {
            sm_delta_dsoc = di->sm_dischrg_dsoc -
                            ((di->dsoc + 1) * 1000 - MIN_ACCURACY);
            di->sm_dischrg_dsoc = (di->dsoc + 1) * 1000 -
                                  MIN_ACCURACY;
            di->zero_dsoc += sm_delta_dsoc;
            DBG("ZERO1: take sm dischrg,delta=%d\n", sm_delta_dsoc);
        }
    }

    /* check overflow */
    if (di->zero_dsoc > (di->dsoc + 1) * 1000 - MIN_ACCURACY)
    {
        DBG("ZERO1: zero dsoc overflow: %d\n", di->zero_dsoc);
        di->zero_dsoc = (di->dsoc + 1) * 1000 - MIN_ACCURACY;
    }

    /* check new dsoc */
    tmp_soc = di->zero_dsoc / 1000;
    if (tmp_soc != di->dsoc)
    {
        /* avoid dsoc jump when heavy load */
        if ((di->dsoc - tmp_soc) > 1)
        {
            di->dsoc--;
            di->zero_dsoc = (di->dsoc + 1) * 1000 - MIN_ACCURACY;
            DBG("ZERO1: heavy load...\n");
        }
        else
        {
            di->dsoc = tmp_soc;
        }
        di->zero_drop_sec = 0;
    }

out:
    DBG("ZERO1: zero_dsoc(Y0)=%d, dsoc=%d, rsoc=%d, tmp_soc=%d\n",
        di->zero_dsoc, di->dsoc, di->rsoc, tmp_soc);
    DBG("ZERO1: sm_dischrg_dsoc=%d, sm_chrg_dsoc=%d\n",
        di->sm_dischrg_dsoc, di->sm_chrg_dsoc);
}

static void rk816_bat_zero_algorithm(struct rk816_battery *di)
{
    rt_int32_t delta_cap = 0, delta_soc = 0;

    di->zero_timeout_cnt++;
    delta_cap = di->zero_remain_cap - di->remain_cap;
    delta_soc = di->zero_linek * (delta_cap * 100) / DIV(di->fcc);

    DBG("ZERO1: zero_linek=%d, zero_dsoc(Y0)=%d, dsoc=%d, rsoc=%d\n"
        "ZERO1: delta_soc(X0)=%d, delta_cap=%d, zero_remain_cap = %d\n"
        "ZERO1: timeout_cnt=%d, sm_dischrg=%d, sm_chrg=%d\n\n",
        di->zero_linek, di->zero_dsoc, di->dsoc, di->rsoc,
        delta_soc, delta_cap, di->zero_remain_cap,
        di->zero_timeout_cnt, di->sm_dischrg_dsoc, di->sm_chrg_dsoc);

    if ((delta_soc >= MIN_ZERO_DSOC_ACCURACY) ||
            (di->zero_timeout_cnt > MIN_ZERO_OVERCNT) ||
            (di->zero_linek == 0))
    {
        DBG("ZERO1:--------- enter calc -----------\n");
        di->zero_timeout_cnt = 0;
        di->zero_dsoc -= delta_soc;
        rk816_bat_calc_zero_algorithm(di);
        rk816_bat_calc_zero_linek(di);
    }
}

static void rk816_bat_debug_info(struct rk816_battery *di)
{
    const char *work_mode[] = {"ZERO", "FINISH", "UN", "UN", "SMOOTH"};
    rt_uint8_t usb_ctrl, chrg_ctrl1, chrg_ctrl3;
    rt_uint32_t chrg_sel;

    usb_ctrl = rk816_bat_read(di, USB_CTRL_REG);
    chrg_ctrl1 = rk816_bat_read(di, CHRG_CTRL_REG1);
    chrg_ctrl3 = rk816_bat_read(di, CHRG_CTRL_REG3);
    chrg_sel = bat_cur_level[chrg_ctrl1 & 0x0f];
    if (!di->over_20mR)
        chrg_sel = RES_FAC_MUX(chrg_sel, di->res_fac);
    else
        chrg_sel = RES_FAC_DIV(chrg_sel, di->res_fac);

    di->current_chrg = chrg_sel;

    DBG("USB_CTRL=0x%2x, CHRG_CTRL:REG1=0x%2x, REG3=0x%2x\n",
        usb_ctrl, chrg_ctrl1, chrg_ctrl3);
    DBG("\n\n");
    DBG("------------------------------------------------------------------\n");
    DBG("Dsoc=%d, Rsoc=%d, Vavg=%d, Iavg=%d, Cap=%d, Fcc=%d\n"
        "K=%d, Virt=%d, Mode=%s, Is=%d, Ip=%d, Vs=%d, Vocv=%d\n",
        di->dsoc, di->rsoc, di->voltage_avg, di->current_avg,
        di->remain_cap, di->fcc, di->sm_linek, di->pdata->vritual_mode,
        work_mode[di->work_mode], chrg_sel, src_cur_level[usb_ctrl & 0x0f],
        bat_vol_level[(chrg_ctrl1 & 0x70) >> 4], di->voltage_ocv);
    DBG("off:i=0x%x, c=0x%x, p=%d, Rbat=%d, Rsam=%d(%d), exist=%d\n",
        rk816_bat_get_ioffset(di), rk816_bat_get_coffset(di),
        di->poffset, di->bat_res, di->pdata->sample_res, di->res_fac, di->exist);
    DBG("ocv_c=%d: %d->%d; max_c=%d: %d->%d; force_c=%d: %d->%d, Vzero=%d\n",
        di->is_ocv_calib, di->ocv_pre_dsoc, di->ocv_new_dsoc,
        di->is_max_soc_offset, di->max_pre_dsoc, di->max_new_dsoc,
        di->is_force_calib, di->force_pre_dsoc, di->force_new_dsoc,
        di->pdata->zero_algorithm_vol);
    DBG("min=%d, init=%d, sw=%d, first=%d, changed=%d, Vshtd=%d, sm_cap=%d\n",
        di->pwroff_min, di->is_initialized, di->is_sw_reset,
        di->is_first_on, di->last_dsoc, di->pdata->pwroff_vol,
        di->sm_remain_cap);
    DBG("------------------------------------------------------------------\n");
}

static void rk816_bat_finish_algorithm(struct rk816_battery *di)
{
    rt_uint32_t finish_sec, soc_sec;
    rt_int32_t plus_soc, finish_current, rest = 0;

    /* rsoc */
    if ((di->remain_cap != di->fcc) &&
            (di->chrg_status == CHARGE_FINISH))
    {
        rk816_bat_init_coulomb_cap(di, di->fcc);
    }

    /* dsoc */
    if (di->dsoc < 100)
    {
        if (!di->chrg_finish_base)
            di->chrg_finish_base = get_boot_sec();

        finish_current = di->current_chrg;
        finish_sec = base2sec(di->chrg_finish_base);
        soc_sec = di->fcc * 3600 / 100 / DIV(finish_current);
        plus_soc = finish_sec / DIV(soc_sec);
        if (finish_sec > soc_sec)
        {
            rest = finish_sec % soc_sec;
            di->dsoc += plus_soc;
            di->chrg_finish_base = get_boot_sec();
            if (di->chrg_finish_base > rest)
                di->chrg_finish_base = get_boot_sec() - rest;
        }
        DBG("<%s>.CHARGE_FINISH:dsoc<100,dsoc=%d\n"
            "soc_time=%lu, sec_finish=%lu, plus_soc=%d, rest=%d\n",
            __func__, di->dsoc, soc_sec, finish_sec, plus_soc, rest);
    }
}

static void rk816_bat_calc_smooth_dischrg(struct rk816_battery *di)
{
    rt_int32_t tmp_soc = 0, sm_delta_dsoc = 0, zero_delta_dsoc = 0;

    tmp_soc = di->sm_dischrg_dsoc / 1000;
    if (tmp_soc == di->dsoc)
        goto out;

    DBG("<%s>. enter: dsoc=%d, rsoc=%d\n", __func__, di->dsoc, di->rsoc);
    /* when dischrge slow down, take sm charge rest into calc */
    if (di->dsoc < di->rsoc)
    {
        tmp_soc = di->sm_chrg_dsoc / 1000;
        if (tmp_soc == di->dsoc)
        {
            sm_delta_dsoc = di->sm_chrg_dsoc - di->dsoc * 1000;
            di->sm_chrg_dsoc = di->dsoc * 1000;
            di->sm_dischrg_dsoc += sm_delta_dsoc;
            DBG("<%s>. take sm dischrg, delta=%d\n",
                __func__, sm_delta_dsoc);
        }
    }

    /* when discharge speed up, take zero discharge rest into calc */
    if (di->dsoc > di->rsoc)
    {
        tmp_soc = di->zero_dsoc / 1000;
        if (tmp_soc == di->dsoc)
        {
            zero_delta_dsoc = di->zero_dsoc - ((di->dsoc + 1) *
                                               1000 - MIN_ACCURACY);
            di->zero_dsoc = (di->dsoc + 1) * 1000 - MIN_ACCURACY;
            di->sm_dischrg_dsoc += zero_delta_dsoc;
            DBG("<%s>. take zero schrg, delta=%d\n",
                __func__, zero_delta_dsoc);
        }
    }

    /* check up overflow */
    if ((di->sm_dischrg_dsoc) > ((di->dsoc + 1) * 1000 - MIN_ACCURACY))
    {
        DBG("<%s>. dischrg_dsoc up overflow\n", __func__);
        di->sm_dischrg_dsoc = (di->dsoc + 1) *
                              1000 - MIN_ACCURACY;
    }

    /* check new dsoc */
    tmp_soc = di->sm_dischrg_dsoc / 1000;
    if (tmp_soc != di->dsoc)
    {
        di->dsoc = tmp_soc;
        di->sm_chrg_dsoc = di->dsoc * 1000;
    }
out:
    DBG("<%s>. dsoc=%d, rsoc=%d, dsoc:sm_dischrg=%d, sm_chrg=%d, zero=%d\n",
        __func__, di->dsoc, di->rsoc, di->sm_dischrg_dsoc, di->sm_chrg_dsoc,
        di->zero_dsoc);
}

static void rk816_bat_calc_smooth_chrg(struct rk816_battery *di)
{
    rt_int32_t tmp_soc = 0, sm_delta_dsoc = 0, zero_delta_dsoc = 0;

    tmp_soc = di->sm_chrg_dsoc / 1000;
    if (tmp_soc == di->dsoc)
        goto out;

    DBG("<%s>. enter: dsoc=%d, rsoc=%d\n", __func__, di->dsoc, di->rsoc);
    /* when charge slow down, take zero & sm dischrg into calc */
    if (di->dsoc > di->rsoc)
    {
        /* take sm discharge rest into calc */
        tmp_soc = di->sm_dischrg_dsoc / 1000;
        if (tmp_soc == di->dsoc)
        {
            sm_delta_dsoc = di->sm_dischrg_dsoc -
                            ((di->dsoc + 1) * 1000 - MIN_ACCURACY);
            di->sm_dischrg_dsoc = (di->dsoc + 1) * 1000 -
                                  MIN_ACCURACY;
            di->sm_chrg_dsoc += sm_delta_dsoc;
            DBG("<%s>. take sm dischrg, delta=%d\n",
                __func__, sm_delta_dsoc);
        }

        /* take zero discharge rest into calc */
        tmp_soc = di->zero_dsoc / 1000;
        if (tmp_soc == di->dsoc)
        {
            zero_delta_dsoc = di->zero_dsoc -
                              ((di->dsoc + 1) * 1000 - MIN_ACCURACY);
            di->zero_dsoc = (di->dsoc + 1) * 1000 - MIN_ACCURACY;
            di->sm_chrg_dsoc += zero_delta_dsoc;
            DBG("<%s>. take zero dischrg, delta=%d\n",
                __func__, zero_delta_dsoc);
        }
    }

    /* check down overflow */
    if (di->sm_chrg_dsoc < di->dsoc * 1000)
    {
        DBG("<%s>. chrg_dsoc down overflow\n", __func__);
        di->sm_chrg_dsoc = di->dsoc * 1000;
    }

    /* check new dsoc */
    tmp_soc = di->sm_chrg_dsoc / 1000;
    if (tmp_soc != di->dsoc)
    {
        di->dsoc = tmp_soc;
        di->sm_dischrg_dsoc = (di->dsoc + 1) * 1000 - MIN_ACCURACY;
    }
out:
    DBG("<%s>.dsoc=%d, rsoc=%d, dsoc: sm_dischrg=%d, sm_chrg=%d, zero=%d\n",
        __func__, di->dsoc, di->rsoc, di->sm_dischrg_dsoc, di->sm_chrg_dsoc,
        di->zero_dsoc);
}

static void rk816_bat_smooth_algorithm(struct rk816_battery *di)
{
    rt_int32_t ydsoc = 0, delta_cap = 0, old_cap = 0;
    rt_uint32_t tgt_sec = 0;

    di->remain_cap = rk816_bat_get_coulomb_cap(di);

    /* full charge: slow down */
    if ((di->dsoc == 99) && (di->chrg_status == CC_OR_CV) &&
            (di->current_avg > 0))
    {
        di->sm_linek = FULL_CHRG_K;
        /* terminal charge, slow down */
    }
    else if ((di->current_avg >= TERM_CHRG_CURR) &&
             (di->chrg_status == CC_OR_CV) && (di->dsoc >= TERM_CHRG_DSOC))
    {
        di->sm_linek = TERM_CHRG_K;
        DBG("<%s>. terminal mode..\n", __func__);
        /* simulate charge, speed up */
    }
    else if ((di->current_avg <= SIMULATE_CHRG_CURR) &&
             (di->current_avg > 0) && (di->chrg_status == CC_OR_CV) &&
             (di->dsoc < TERM_CHRG_DSOC) &&
             ((di->rsoc - di->dsoc) >= SIMULATE_CHRG_INTV))
    {
        di->sm_linek = SIMULATE_CHRG_K;
        DBG("<%s>. simulate mode..\n", __func__);
    }
    else
    {
        /* charge and discharge switch */
        if ((di->sm_linek * di->current_avg <= 0) ||
                (di->sm_linek == TERM_CHRG_K) ||
                (di->sm_linek == FULL_CHRG_K) ||
                (di->sm_linek == SIMULATE_CHRG_K))
        {
            DBG("<%s>. linek mode, retinit sm linek..\n", __func__);
            rk816_bat_calc_sm_linek(di);
        }
    }

    old_cap = di->sm_remain_cap;
    /*
     * when dsoc equal rsoc(not include full, term, simulate case),
     * sm_linek should change to -1000/1000 smoothly to avoid dsoc+1/-1
     * right away, so change it after flat seconds
     */
    if ((di->dsoc == di->rsoc) && (abs(di->sm_linek) != 1000) &&
            (di->sm_linek != FULL_CHRG_K && di->sm_linek != TERM_CHRG_K &&
             di->sm_linek != SIMULATE_CHRG_K))
    {
        if (!di->flat_match_sec)
            di->flat_match_sec = get_boot_sec();
        tgt_sec = di->fcc * 3600 / 100 / DIV(abs(di->current_avg)) / 3;
        if (base2sec(di->flat_match_sec) >= tgt_sec)
        {
            di->flat_match_sec = 0;
            di->sm_linek = (di->current_avg >= 0) ? 1000 : -1000;
        }
        DBG("<%s>. flat_sec=%ld, tgt_sec=%ld, sm_k=%d\n", __func__,
            base2sec(di->flat_match_sec), tgt_sec, di->sm_linek);
    }
    else
    {
        di->flat_match_sec = 0;
    }

    /* abs(k)=1000 or dsoc=100, stop calc */
    if ((abs(di->sm_linek) == 1000) || (di->current_avg >= 0 &&
                                        di->chrg_status == CC_OR_CV && di->dsoc >= 100))
    {
        DBG("<%s>. sm_linek=%d\n", __func__, di->sm_linek);
        if (abs(di->sm_linek) == 1000)
        {
            di->dsoc = di->rsoc;
            di->sm_linek = (di->sm_linek > 0) ? 1000 : -1000;
            DBG("<%s>. dsoc == rsoc, sm_linek=%d\n",
                __func__, di->sm_linek);
        }
        di->sm_remain_cap = di->remain_cap;
        di->sm_chrg_dsoc = di->dsoc * 1000;
        di->sm_dischrg_dsoc = (di->dsoc + 1) * 1000 - MIN_ACCURACY;
        DBG("<%s>. sm_dischrg_dsoc=%d, sm_chrg_dsoc=%d\n",
            __func__, di->sm_dischrg_dsoc, di->sm_chrg_dsoc);
    }
    else
    {
        delta_cap = di->remain_cap - di->sm_remain_cap;
        if (delta_cap == 0)
        {
            DBG("<%s>. delta_cap = 0\n", __func__);
            return;
        }
        ydsoc = di->sm_linek * abs(delta_cap) * 100 / DIV(di->fcc);
        if (ydsoc == 0)
        {
            DBG("<%s>. ydsoc = 0\n", __func__);
            return;
        }
        di->sm_remain_cap = di->remain_cap;

        DBG("<%s>. k=%d, ydsoc=%d; cap:old=%d, new:%d; delta_cap=%d\n",
            __func__, di->sm_linek, ydsoc, old_cap,
            di->sm_remain_cap, delta_cap);

        /* discharge mode */
        if (ydsoc < 0)
        {
            di->sm_dischrg_dsoc += ydsoc;
            rk816_bat_calc_smooth_dischrg(di);
            /* charge mode */
        }
        else
        {
            di->sm_chrg_dsoc += ydsoc;
            rk816_bat_calc_smooth_chrg(di);
        }

        if (di->s2r)
        {
            di->s2r = false;
            rk816_bat_calc_sm_linek(di);
        }
    }
}

static rt_int8_t rk816_bat_fake_finish_mode(struct rk816_battery *di)
{
    if ((di->rsoc == 100) && (di->chrg_status == CC_OR_CV) &&
            (abs(di->current_avg) <= 100))
        return true;
    else
        return false;
}

static void rk816_bat_display_smooth(struct rk816_battery *di)
{
    /* discharge: reinit "zero & smooth" algorithm to avoid handling dsoc */
    if (di->s2r && !di->sleep_chrg_online)
    {
        DBG("s2r: discharge, reset algorithm...\n");
        di->s2r = false;
        rk816_bat_zero_algo_prepare(di);
        rk816_bat_smooth_algo_prepare(di);
        return;
    }

    if (di->work_mode == MODE_FINISH)
    {
        DBG("step1: charge finish...\n");
        rk816_bat_finish_algorithm(di);
        if ((di->chrg_status != CHARGE_FINISH) &&
                !rk816_bat_fake_finish_mode(di))
        {
            if ((di->current_avg < 0) &&
                    (di->voltage_avg < di->pdata->zero_algorithm_vol))
            {
                DBG("step1: change to zero mode...\n");
                rk816_bat_zero_algo_prepare(di);
                di->work_mode = MODE_ZERO;
            }
            else
            {
                DBG("step1: change to smooth mode...\n");
                rk816_bat_smooth_algo_prepare(di);
                di->work_mode = MODE_SMOOTH;
            }
        }
    }
    else if (di->work_mode == MODE_ZERO)
    {
        DBG("step2: zero algorithm...\n");
        rk816_bat_zero_algorithm(di);
        if ((di->voltage_avg >= di->pdata->zero_algorithm_vol + 50) ||
                (di->current_avg >= 0))
        {
            DBG("step2: change to smooth mode...\n");
            rk816_bat_smooth_algo_prepare(di);
            di->work_mode = MODE_SMOOTH;
        }
        else if ((di->chrg_status == CHARGE_FINISH) ||
                 rk816_bat_fake_finish_mode(di))
        {
            DBG("step2: change to finish mode...\n");
            rk816_bat_finish_algo_prepare(di);
            di->work_mode = MODE_FINISH;
        }
    }
    else
    {
        DBG("step3: smooth algorithm...\n");
        rk816_bat_smooth_algorithm(di);
        if ((di->current_avg < 0) &&
                (di->voltage_avg < di->pdata->zero_algorithm_vol))
        {
            DBG("step3: change to zero mode...\n");
            rk816_bat_zero_algo_prepare(di);
            di->work_mode = MODE_ZERO;
        }
        else if ((di->chrg_status == CHARGE_FINISH) ||
                 rk816_bat_fake_finish_mode(di))
        {
            DBG("step3: change to finish mode...\n");
            rk816_bat_finish_algo_prepare(di);
            di->work_mode = MODE_FINISH;
        }
    }
}

static const char *bat_status[] =
{
    "charge-off", "dead-charge", "trickle-charge", "cccv",
    "finish", "usb-over-vol", "temp-error", "timer-error",
};

static void rk816_bat_report(struct rk816_battery *di)
{
    rt_uint8_t status, thermal;
    static rt_int32_t old_soc = -1;

    if (di->dsoc > 100)
        di->dsoc = 100;
    else if (di->dsoc < 0)
        di->dsoc = 0;

    if (old_soc == -1)
        old_soc = di->dsoc;

    if (di->dsoc == old_soc)
        return;

    thermal = rk816_bat_read(di, THERMAL_REG);
    status = rk816_bat_read(di, SUP_STS_REG);
    status = (status & CHRG_STATUS_MSK) >> 4;
    old_soc = di->dsoc;
    di->last_dsoc = di->dsoc;

    BAT_INFO("dl=%d rl=%d c=%d v=%d ov=%d pv=%d cap=%d f=%d st=%s ht=%d\n",
             di->dsoc, di->rsoc, di->current_avg,  di->voltage_avg,
             di->voltage_ocv, di->pdata->pwroff_vol, di->remain_cap,
             di->fcc, bat_status[status], !!(thermal & HOTDIE_STS));
}

static void rk816_bat_update_info(struct rk816_battery *di)
{
    di->voltage_avg = rk816_bat_get_avg_voltage(di);
    di->current_avg = rk816_bat_get_avg_current(di);
    di->chrg_status = rk816_bat_get_chrg_status(di);
    di->voltage_relax = rk816_bat_get_relax_voltage(di);
    di->rsoc = rk816_bat_get_rsoc(di);
    di->remain_cap = rk816_bat_get_coulomb_cap(di);

    /* smooth charge */
    if (di->remain_cap > di->fcc)
    {
        di->sm_remain_cap -= (di->remain_cap - di->fcc);
        DBG("<%s>. cap: remain=%d, sm_remain=%d\n",
            __func__, di->remain_cap, di->sm_remain_cap);
        rk816_bat_init_coulomb_cap(di, di->fcc);
    }

    if (di->chrg_status != CHARGE_FINISH)
        di->chrg_finish_base = get_boot_sec();
}

static void rk816_bat_save_data(struct rk816_battery *di)
{
    rk816_bat_save_dsoc(di, di->dsoc);
    rk816_bat_save_cap(di, di->remain_cap);
}

static void rk816_bat_init_param(struct rk816_battery *di)
{
    di->pdata = malloc(sizeof(struct battery_pdata));
    if (!di->pdata)
    {
        rt_kprintf("No memory\n");
        return;
    }

    di->pdata->ocv_size = 21;
    di->pdata->zero_size = 21;
    memcpy(di->pdata->ocv_table, fg_pdata->ocv_table,
           di->pdata->ocv_size * sizeof(rt_uint32_t));
    di->pdata->bat_res = fg_pdata->bat_res;
    di->pdata->pwroff_vol =  fg_pdata->power_off_thresd;
    di->pdata->monitor_sec = fg_pdata->monitor_sec;
    di->pdata->design_qmax = fg_pdata->design_qmax;
    di->pdata->design_capacity = fg_pdata->design_capacity;
    di->pdata->zero_algorithm_vol = fg_pdata->zero_algorithm_vol;
    di->pdata->vritual_mode = fg_pdata->virtual_power;
    di->pdata->sample_res = fg_pdata->sample_res;
    di->pdata->sleep_enter_current = 300;
    di->pdata->sleep_exit_current = 300;
    di->pdata->sleep_filter_current = 100;
    di->pdata->zero_reserve_dsoc = 10;
    di->pdata->max_soc_offset = 80;
    di->pdata->energy_mode = 0;

    di->sleep_chrg_status = rk816_bat_get_chrg_status(di);
    di->chrg_status = rk816_bat_get_chrg_status(di);
    di->monitor_ticks = fg_pdata->monitor_sec * RT_TICK_PER_SECOND;
    di->design_cap = di->pdata->design_capacity;
    di->qmax = di->pdata->design_qmax;
    di->bat_res = di->pdata->bat_res;
    di->chrg_finish_base = 0;
}

static void rk816_bat_init_mode(struct rk816_battery *di)
{
    if (di->chrg_status == CHARGE_FINISH)
    {
        rk816_bat_finish_algo_prepare(di);
        di->work_mode = MODE_FINISH;
    }
    else
    {
        rk816_bat_smooth_algo_prepare(di);
        di->work_mode = MODE_SMOOTH;
    }
}

static void rk816_fg_main_thread(void *battery)
{
    struct rk816_battery *di = battery;

    while (1)
    {
        rk816_bat_update_info(di);
        rk816_bat_lowpwr_check(di);
        rk816_bat_display_smooth(di);
        rk816_bat_report(di);
        rk816_bat_save_data(di);
        rk816_bat_debug_info(di);
        rt_thread_sleep(di->monitor_ticks);
    }
}

static rt_err_t rk816_fg_init(rt_device_t dev)
{
    struct rk816_battery *di = &fg_di;
    static rt_thread_t thread = RT_NULL;

    rk816_bat_init_param(di);
    rk816_bat_enable_gauge(di);
    rk816_bat_init_voltage_kb(di);
    rk816_bat_init_poffset(di);
    rk816_bat_select_sample_res(di);
    rk816_bat_set_relax_sample(di);
    rk816_bat_set_ioffset_sample(di);
    rk816_bat_set_ocv_sample(di);
    rk816_bat_init_rsoc(di);
    rk816_bat_init_coulomb_cap(di, di->nac);
    rk816_bat_init_zero_table(di);
    rk816_bat_init_mode(di);

    di->exist = is_rk816_bat_exist(di);
    di->voltage_avg = rk816_bat_get_avg_voltage(di);
    di->voltage_ocv = rk816_bat_get_ocv_voltage(di);
    di->voltage_relax = rk816_bat_get_relax_voltage(di);
    di->current_avg = rk816_bat_get_avg_current(di);
    di->current_relax = rk816_bat_get_relax_current(di);
    di->remain_cap = rk816_bat_get_coulomb_cap(di);

    DBG("nac=%d cap=%d ov=%d v=%d rv=%d dl=%d rl=%d c=%d\n",
        di->nac, di->remain_cap, di->voltage_ocv, di->voltage_avg,
        di->voltage_relax, di->dsoc, di->rsoc, di->current_avg);

    thread = rt_thread_create("rk816_main_thread",
                              rk816_fg_main_thread, di, 1024, 20, 20);
    if (thread != RT_NULL)
    {
        rt_thread_startup(thread);
    }

    return RT_EOK;
}

static rt_err_t rk816_fg_get_status(struct rt_fg_status *status)
{
    struct rk816_battery *di = &fg_di;

    status->soc = di->dsoc;
    status->fcc = di->fcc;
    status->present = di->exist;
    status->current = di->current_avg;
    status->voltage = di->voltage_avg;

    return RT_EOK;
}

static rt_err_t rk816_fg_control(rt_device_t dev, int cmd, void *args)
{
    RT_ASSERT(dev != RT_NULL);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_FG_GET:
    {
        rk816_fg_get_status((struct rt_fg_status *)args);
    }
    break;

    default:
        return RT_EINVAL;
    }

    return RT_EOK;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops fg_ops =
{
    rk816_fg_init,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    RT_NULL,
    rk816_fg_control,
};
#endif

int rt_rk816_fg_init(void)
{
    static struct rt_device fg_dev;

    fg_i2c = pmic_get_i2c_desc();
    if (fg_i2c == RT_NULL)
        return -RT_EINVAL;

    RT_ASSERT(!rt_device_find("fuel_gauge"));

    fg_dev.type    = RT_Device_Class_PM;
#ifdef RT_USING_DEVICE_OPS
    fg_dev.ops     = &fg_ops;
#else
    fg_dev.init    = rk816_fg_init;
    fg_dev.open    = RT_NULL;
    fg_dev.close   = RT_NULL;
    fg_dev.read    = RT_NULL;
    fg_dev.write   = RT_NULL;
    fg_dev.control = rk816_fg_control;
#endif

    return rt_device_register(&fg_dev, "fuel_gauge", RT_DEVICE_FLAG_RDWR);
}
INIT_DEVICE_EXPORT(rt_rk816_fg_init);

#if defined(RT_USING_FINSH) && defined(FINSH_USING_MSH)
static int fg_rk816(uint8_t argc, char **argv)
{
    if (argc > 1)
        dbg_enable = !!atoi(argv[1]);

    return RT_EOK;
}

MSH_CMD_EXPORT(fg_rk816, debug message enable or disable);
#endif

#endif /* RT_USING_FG_RK816 */
