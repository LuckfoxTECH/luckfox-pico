/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_dsi.c
  * @version V0.1
  * @brief   display dsi driver for pisces
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-03-03     Nickey Yang   first implementation
  * 2019-07-15     Huang Jiachai add function comments
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
 *  @{
 */

/** @addtogroup DSI
 *  @{
 */

/** @defgroup DSI_How_To_Use How To Use
 *  @{

 The DSI driver use to init DSI controller and provide api to send cmd to
 communicate with panel.

 @} */

#include <rthw.h>

#ifdef RT_USING_DSI
#include "drv_display.h"

/********************* Private Function Definition ***************************/
/** @defgroup DSI_Private_Function Private Function
 *  @{
 */

/**
 * @brief  Send dsi command.
 * @param  state: display state.
 * @param  cmd: DCS command.
 * @param  data: set command value.
 * @param  len: command length.
 * return rt_err_t.
 */
static rt_err_t mipi_dsi_dcs_write(struct display_state *state, uint8_t cmd,
                                   uint8_t *data, uint8_t len)
{
    struct connector_state *conn_state = &state->conn_state;
    HAL_Status ret;
    uint8_t *tx, type, size;

    if (len > 0)
    {
        size = 1 + len;
        tx = rt_malloc(size);
        if (!tx)
        {
            rt_kprintf("malloc dcs_tx failed\n");
            return -RT_ENOMEM;
        }
        tx[0] = cmd;
        memcpy(&tx[1], data, len);
    }
    else
    {
        tx = &cmd;
        size = 1;
    }

    switch (size)
    {
    case 0:
        return -EINVAL;
    case 1:
        type = MIPI_DSI_DCS_SHORT_WRITE;
        break;
    case 2:
        type = MIPI_DSI_DCS_SHORT_WRITE_PARAM;
        break;
    default:
        type = MIPI_DSI_DCS_LONG_WRITE;
        break;
    }

    ret = HAL_DSI_SendPacket(conn_state->hw_base, type, size, tx);

    if (len > 0)
        free(tx);

    if (ret == HAL_OK)
        return RT_EOK;
    else
        return -RT_ETIMEOUT;
}

/**
 * @brief  Set the brightness value of panel.
 * @param  state: display state.
 * @param  brightness: brightness value.
 * return rt_err_t.
 */
static rt_err_t mipi_dsi_dcs_set_display_brightness(struct display_state *state, uint16_t brightness)
{
    rt_err_t ret;
#if defined(RT_USING_PANEL_S6E3FC2X01) || defined(RT_USING_PANEL_S6E3HC2_X1) || defined(RT_USING_PANEL_S6E3HC2_X4)
    uint8_t payload[2] = { brightness >> 8, brightness & 0xff };
#else
    uint8_t payload[2] = { brightness & 0xff, brightness >> 8 };
#endif

    ret = mipi_dsi_dcs_write(state, MIPI_DCS_SET_DISPLAY_BRIGHTNESS,
                             payload, sizeof(payload));

    return ret;
}

/**
 * @brief  Send tear on command.
 * @param  state: display state.
 * @param  mode: tear on mode.
 * return rt_err_t.
 */
static rt_err_t mipi_dsi_dcs_set_tear_on(struct display_state *state, uint8_t mode)
{
    rt_err_t ret;

    ret = mipi_dsi_dcs_write(state, MIPI_DCS_SET_TEAR_ON,
                             &mode, sizeof(mode));
    return ret;
}

/**
 * @brief  Send column address for area display.
 * @param  state: display state.
 * @param  start: the column start.
 * @param  end: the column end.
 * return rt_err_t.
 */
static rt_err_t mipi_dsi_dcs_set_column_address(struct display_state *state, uint16_t start,
        uint16_t end)
{
    uint8_t payload[4] = { start >> 8, start & 0xff, end >> 8, end & 0xff };
    rt_err_t ret;

    ret = mipi_dsi_dcs_write(state, MIPI_DCS_SET_COLUMN_ADDRESS, payload,
                             sizeof(payload));
    return ret;
}

/**
 * @brief  Send page address for area display.
 * @param  state: display state.
 * @param  start: the page start.
 * @param  end: the page end.
 * return rt_err_t.
 */
static rt_err_t mipi_dsi_dcs_set_page_address(struct display_state *state, uint16_t start,
        uint16_t end)
{
    uint8_t payload[4] = { start >> 8, start & 0xff, end >> 8, end & 0xff };
    rt_err_t ret;

    ret = mipi_dsi_dcs_write(state, MIPI_DCS_SET_PAGE_ADDRESS, payload,
                             sizeof(payload));
    return ret;
}

/**
 * @brief  Send partial columns for area display.
 * @param  state: display state.
 * @param  start: the columns start.
 * @param  end: the columns end.
 * return rt_err_t.
 */
static rt_err_t mipi_dsi_dcs_set_partial_columns(struct display_state *state, uint16_t start,
        uint16_t end)
{
    uint8_t payload[4] = { start >> 8, start & 0xff, end >> 8, end & 0xff };
    rt_err_t ret;

    ret = mipi_dsi_dcs_write(state, MIPI_DCS_SET_PARTIAL_COLUMNS, payload,
                             sizeof(payload));
    return ret;
}

/**
 * @brief  Send partial rows for area display.
 * @param  state: display state.
 * @param  start: the rows start.
 * @param  end: the rows end.
 * return rt_err_t.
 */
static rt_err_t mipi_dsi_dcs_set_partial_rows(struct display_state *state, uint16_t start,
        uint16_t end)
{
    uint8_t payload[4] = { start >> 8, start & 0xff, end >> 8, end & 0xff };
    rt_err_t ret;

    ret = mipi_dsi_dcs_write(state, MIPI_DCS_SET_PARTIAL_ROWS, payload,
                             sizeof(payload));
    return ret;
}

/**
 * @brief  Dsi irq handler.
 * @param  irq: dsi irq number.
 * @param  param: unused now.
 */
static void rockchip_dsi_irq_handler(int irq, void *param)
{
    /* enter interrupt */
    rt_interrupt_enter();

    HAL_DSI_IrqHandler(DSI);

    /* leave interrupt */
    rt_interrupt_leave();
}

/**
 * @brief  Dsi init for hardware.
 * @param  state: display state.
 */
static void rockchip_dsi_init(struct display_state *state)
{
    struct connector_state *conn_state = &state->conn_state;

    conn_state->hw_base = DSI;
    conn_state->enabled = false;
}

/**
 * @brief  Dsi deinit for hardware.
 * @param  state: display state.
 */
static void rockchip_dsi_deinit(struct display_state *state)
{
}

#ifdef IS_FPGA
#include <drivers/i2c.h>
static int dphy_write(struct rt_i2c_bus_device *i2c_bus, rt_uint16_t addr,
                      void *data_buf, size_t data_len)
{
    struct rt_i2c_msg msgs[1];

    msgs[0].addr  = addr;
    msgs[0].flags = RT_I2C_WR;
    msgs[0].buf   = data_buf;
    msgs[0].len   = data_len;

    if (rt_i2c_transfer(i2c_bus, msgs, 1) == 1)
        return 0;
    else
        return -1;
}

static void rockchip_dsi_fpga_init(void)
{
    char *data_buf = NULL;
    struct rt_i2c_bus_device *i2c_bus;

    /* this is cru reg for mipi clk */
    *(volatile long unsigned int *)(0x40050824) = 0x01000100;

    /* this is for m31 dphy ths_zero */
    i2c_bus = (struct rt_i2c_bus_device *)rt_i2c_bus_device_find("i2c1");
    if (i2c_bus == RT_NULL)
    {
        rt_kprintf("Did not find i2c1 device....\n");
        return;
    }

    data_buf = (char *)rt_calloc(1, 2);
    if (!data_buf)
    {
        rt_kprintf("dphy write alloc buf fail\n");
        return;
    }

    data_buf[0] = 0x1a;
    data_buf[1] = 0xd7;
    dphy_write(i2c_bus, 0x68, data_buf, 2);

    data_buf[0] = 0x1c;
    data_buf[1] = 0x38;
    dphy_write(i2c_bus, 0x68, data_buf, 2);

    data_buf[0] = 0x1e;
    data_buf[1] = 0x38;
    dphy_write(i2c_bus, 0x68, data_buf, 2);

    data_buf[0] = 0x20;
    data_buf[1] = 0x30;
    dphy_write(i2c_bus, 0x68, data_buf, 2);

    data_buf[0] = 0x22;
    data_buf[1] = 0x30;
    dphy_write(i2c_bus, 0x68, data_buf, 2);

    rt_free(data_buf);
}
#endif

/**
 * @brief  Dsi controller and phy prepare.
 * @param  state: display state.
 */
static void rockchip_dsi_prepare(struct display_state *state)
{
    struct connector_state *conn_state = &state->conn_state;
    struct panel_state *panel_state = &state->panel_state;
    uint16_t lanembps = panel_state->vmode.lanembps;
    eDSI_DphyRefClk ref_clk_in;
    bool by_osc = true;
    HAL_Status ret;

    HAL_CRU_ClkEnable(PCLK_MIPIDSI_HOST_GATE);
    HAL_CRU_ClkEnable(DCLK_MIPIDSI_HOST_GATE);

#ifdef IS_FPGA
    rockchip_dsi_fpga_init();
#endif

    /*
     * CLK_DPHY_REF can directly from the crystal oscillator by default.
     * We need to modify the relevant settings of dphy reference clk,
     * when this clk and frequency of crystal oscillator are not equal.
     */
    switch (PLL_INPUT_OSC_RATE)
    {
    case 6000000:
        ref_clk_in = DPHY_REF_CLK_IN_6000000;
        break;
    case 12000000:
        ref_clk_in = DPHY_REF_CLK_IN_12000000;
        break;
    case 19200000:
        ref_clk_in = DPHY_REF_CLK_IN_19200000;
        break;
    case 25000000:
        ref_clk_in = DPHY_REF_CLK_IN_25000000;
        break;
    case 26000000:
        ref_clk_in = DPHY_REF_CLK_IN_26000000;
        break;
    case 27000000:
        ref_clk_in = DPHY_REF_CLK_IN_27000000;
        break;
    case 38400000:
        ref_clk_in = DPHY_REF_CLK_IN_38400000;
        break;
    case 40000000:
        ref_clk_in = DPHY_REF_CLK_IN_40000000;
        break;
    default:
        ref_clk_in = DPHY_REF_CLK_IN_27000000;
        by_osc = false;
        break;
    }

    lanembps = HAL_DSI_M31DphyInit(conn_state->hw_base, lanembps, ref_clk_in);
    if (!by_osc)
    {
        HAL_CRU_ClkEnable(CLK_DPHY_REF_FRAC_GATE);
        ret = HAL_CRU_ClkSetFreq(CLK_DPHY_REF, 27000000);
        RT_ASSERT(ret == HAL_OK);
    }
    HAL_DSI_Init(conn_state->hw_base, lanembps);
    HAL_DSI_DpiConfig(conn_state->hw_base, &state->mode, panel_state->bus_format);
    HAL_DSI_PacketHandlerConfig(conn_state->hw_base, &state->mode);
    HAL_DSI_ModeConfig(conn_state->hw_base, &state->mode);
    HAL_DSI_LineTimerConfig(conn_state->hw_base, lanembps, &state->mode);
    HAL_DSI_VerticalTimingConfig(conn_state->hw_base, &state->mode);
    HAL_DSI_DphyTimingConfig(conn_state->hw_base, &state->mode, RT_HW_LCD_DSI_LANES);
    rt_hw_interrupt_install(MIPI_DSI_HOST_IRQn, rockchip_dsi_irq_handler, RT_NULL, RT_NULL);
    rt_hw_interrupt_umask(MIPI_DSI_HOST_IRQn);

}

/**
 * @brief  Dsi controller enable.
 * @param  state: display state.
 */
static void rockchip_dsi_enable(struct display_state *state)
{
    struct connector_state *conn_state = &state->conn_state;
    struct DISPLAY_MODE_INFO *mode = &state->mode;

    if (mode->flags & DSI_MODE_LPM)
        HAL_DSI_MsgHsModeConfig(conn_state->hw_base);

    if (conn_state->enabled)
        return;

    HAL_DSI_M31DphyPowerUp(conn_state->hw_base);
    HAL_DSI_Enable(conn_state->hw_base, &state->mode);
    HAL_DSI_IrqEnable(conn_state->hw_base);

    conn_state->enabled = true;
}

/**
 * @brief  Dsi controller disable.
 * @param  state: display state.
 */
static void rockchip_dsi_disable(struct display_state *state)
{
    struct connector_state *conn_state = &state->conn_state;

    if (!conn_state->enabled)
        return;

    HAL_DSI_IrqDisable(conn_state->hw_base);
    HAL_DSI_Disable(conn_state->hw_base, &state->mode);
    HAL_DSI_M31DphyPowerDown(conn_state->hw_base);
    HAL_CRU_ClkDisable(CLK_DPHY_REF_FRAC_GATE);
    HAL_CRU_ClkDisable(DCLK_MIPIDSI_HOST_GATE);
    HAL_CRU_ClkDisable(PCLK_MIPIDSI_HOST_GATE);

    conn_state->enabled = false;
}

/**
 * @brief  Message transfer between dsi and panel.
 * @param  state: display state.
 * @param  desc: transfer message.
 * return rt_err_t.
 */
static rt_err_t rockchip_dsi_transfer(struct display_state *state, struct rockchip_cmd_desc *desc)
{
    struct connector_state *conn_state = &state->conn_state;
    struct DISPLAY_MODE_INFO *mode = &state->mode;

    HAL_Status ret;

    if (mode->flags & DSI_MODE_LPM)
        HAL_DSI_MsgLpModeConfig(conn_state->hw_base);

    ret = HAL_DSI_SendPacket(conn_state->hw_base, desc->header.data_type,
                             desc->header.payload_length, desc->payload);

    if (ret == HAL_OK)
        return RT_EOK;
    else
        return -RT_ETIMEOUT;
}

/**
 * @brief  Dsi set area for command mode panel.
 * @param  state: display state.
 * @param  display_rect: display rect.
 */
static void rockchip_dsi_set_area(struct display_state *state, struct DISPLAY_RECT *display_rect)
{
    struct connector_state *conn_state = &state->conn_state;

    mipi_dsi_dcs_set_column_address(state, display_rect->x, display_rect->x + display_rect->w - 1);
    mipi_dsi_dcs_set_page_address(state, display_rect->y, display_rect->y + display_rect->h - 1);

    if (0)
    {
        uint16_t lanembps = state->panel_state.vmode.lanembps;

        mipi_dsi_dcs_set_partial_columns(state, display_rect->x, display_rect->x + display_rect->w - 1);
        mipi_dsi_dcs_set_partial_rows(state, display_rect->y, display_rect->y + display_rect->h - 1);
        HAL_DSI_UpdateLineTimer(conn_state->hw_base, lanembps, &state->mode, display_rect);
        mipi_dsi_dcs_set_tear_on(state, MIPI_DSI_DCS_TEAR_MODE_VBLANK);
    };
}
/** @} */

/** @defgroup DSI_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  DSI control functions.
 * @param  init: Init DSI hardware config.
 * @param  deinit: DSI deinit.
 * @param  prepare: Prepare DSI.
 * @param  enable: Enable DSI.
 * @param  disable: Disable DSI.
 * @param  transfer: Transfer message.
 * @param  set_area: Set display area rect.
 */
const struct rockchip_connector_funcs rockchip_dsi_funcs =
{
    .init = rockchip_dsi_init,
    .deinit = rockchip_dsi_deinit,
    .prepare = rockchip_dsi_prepare,
    .enable = rockchip_dsi_enable,
    .disable = rockchip_dsi_disable,
    .transfer = rockchip_dsi_transfer,
    .set_area = rockchip_dsi_set_area,
    .set_brightness = mipi_dsi_dcs_set_display_brightness,
};

/** @} */

#endif

/** @} */

/** @} */
