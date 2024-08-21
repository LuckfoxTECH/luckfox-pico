/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_display.c
  * @version V0.1
  * @brief   display driver for pisces
  *
  * Change Logs:
  * Date           Author          Notes
  * 2019-02-20     Huang Jiachai   first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
*  @{
*/

/** @addtogroup Display
 *  @{
 */

/** @defgroup Display_How_To_Use How To Use
 *  @{

The Display driver use to Init display control include VOP, MIPI DSI and other display port.
 it can be used in the following three scenarios:

- **Init display port**:
    - Use rk_display_init to init display control and display port hardware resource.
    - rt_err_t rk_display_init(struct display_state *state): init hardware resource
    - rt_err_t rk_connector_init(struct display_state *state): init connect callback function
    - rt_err_t rk_connector_dsi_init(struct display_state *state): init mipi dsi callback function

- **Enable/disable display output**:
    - rt_err_t rk_display_enable(struct display_state *state): enable display output;
    - rt_err_t rk_display_disable(struct display_state *state): disable display output;

- **Commit display config**:
    - rt_err_t rk_display_commit(struct display_state *state): commit one frame display config;

- **Te handle thread**:
    - void  rk_display_te_handle_thread(void *p): emulate TE irq to commit one frame;

 @} */

#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>
#include <drv_pm.h>
#include "board.h"
#ifdef RT_USING_VOP

#include "drv_display.h"
#include "drv_heap.h"
#include "hal_base.h"

/********************* Private MACRO Definition ******************************/
#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b) ((a) < (b) ? (b) : (a))
#endif
#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif
#if DISPLAY_DEBUG
static bool boot_flag = true;
#endif

/********************* Private Variable Definition ******************************/
extern const struct rockchip_crtc_funcs rockchip_vop_funcs;
extern const struct rockchip_connector_funcs rockchip_rgb_funcs;
#ifdef RT_USING_DSI
extern const struct rockchip_connector_funcs rockchip_dsi_funcs;
#endif
extern const struct rockchip_panel_funcs rockchip_panel_funcs;
/********************* Public Function Definitions ******************************/
void rk_display_vmode_to_drm_mode(struct video_mode *mode_input,
                                  struct DISPLAY_MODE_INFO *mode_output,
                                  uint16_t area_w, uint16_t area_h);

/**
 * @brief  Connect callback function init for DSI.
 * @param  state: display state.
 * return rt_err_t.
 */
static rt_err_t rk_connector_dsi_init(struct display_state *state)
{
#ifdef RT_USING_DSI
    state->conn_state.funcs = &rockchip_dsi_funcs;
#endif
    return RT_EOK;
}

/**
 * @brief  Connect callback function init for others.
 * @param  state: display state.
 * return rt_err_t.
 */
RT_WEAK rt_err_t rk_connector_init(struct display_state *state)
{
    rt_kprintf("Unsupported connect type: %d!\n", state->panel_state.conn_type);

    return RT_EOK;
}

/**
 * @brief  Display init for panel, connect and crtc.
 * @param  state: display state.
 * return rt_err_t.
 */
static rt_err_t rk_display_init(struct display_state *state)
{
    state->panel_state.funcs = &rockchip_panel_funcs;
    /**
     * get panel config:
     * such as timing, power, cmd, etc.
     */
    state->panel_state.funcs->init(state);
    if (state->panel_state.conn_type == RK_DISPLAY_CONNECTOR_DSI)
        rk_connector_dsi_init(state);
    else
        rk_connector_init(state);
    state->crtc_state.funcs = &rockchip_vop_funcs;
    /**
     * get conn config:
     * such as lane num, dsi channel num, etc.
     */
    if (state->conn_state.funcs && state->conn_state.funcs->init)
        state->conn_state.funcs->init(state);
    rk_display_vmode_to_drm_mode(&state->panel_state.vmode,
                                 &state->mode, 0, 0);
    /**
      * get crtc config:
      * such as vop version, irq num, etc.
      */
    state->crtc_state.funcs->init(state);
    /**
     * malloc fb memory, this is only use by RTGUI
     * default format is 8bpp, so is the length = h * v.
     */

#if defined(RT_USING_RTGUI) && defined(RT_USING_LARGE_HEAP)
    uint32_t fb_length = 0;

    fb_length = state->mode.crtcHdisplay * state->mode.crtcVdisplay;
    state->rtt_framebuffer = rt_dma_malloc_large(fb_length);
    if (state->rtt_framebuffer == RT_NULL)
    {
        rt_kprintf("malloc memory: 0x%x failed\n", fb_length);
        return -RT_ENOMEM;
    }
    rt_memset((void *)state->rtt_framebuffer, 0, fb_length);
#endif

    return RT_EOK;
}

/**
 * @brief  Enable display module.
 * @param  state: display state.
 * return rt_err_t.
 */
static rt_err_t rk_display_enable(struct display_state *state)
{
    pm_runtime_request(PM_RUNTIME_ID_VOP);

    if (state->conn_state.funcs && state->conn_state.funcs->prepare)
        state->conn_state.funcs->prepare(state);
    state->crtc_state.funcs->enable(state);
    if (state->conn_state.funcs && state->conn_state.funcs->enable)
        state->conn_state.funcs->enable(state);

    return RT_EOK;
}

/**
 * @brief  Disable display module.
 * @param  state: display state.
 * return rt_err_t.
 */
static rt_err_t rk_display_disable(struct display_state *state)
{
    if (state->conn_state.funcs && state->conn_state.funcs->disable)
        state->conn_state.funcs->disable(state);
    state->crtc_state.funcs->disable(state);

    pm_runtime_release(PM_RUNTIME_ID_VOP);

    return RT_EOK;
}

/**
 * @brief  Commit one frame to display.
 * @param  state: display state.
 * return rt_err_t.
 */
static rt_err_t rk_display_commit(struct display_state *state)
{
#ifdef RT_HW_LCD_GPIO_TRIGGER_PIN
    int ret = 0;
    state->te_irq = false;
    ret = rt_pin_irq_enable(state->panel_state.gpio_trigger_pin_info, PIN_IRQ_ENABLE);
    RT_ASSERT(ret == RT_EOK);
    uint32_t event = false;

    ret = rt_event_recv(state->te_event,
                        true,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RK_DISPLAY_TIME_OUT, &event);
    if (!event)
        rt_kprintf("rk_display_commit wait te_irq time out\n");

    ret = rt_pin_irq_enable(state->panel_state.gpio_trigger_pin_info, PIN_IRQ_DISABLE);
    RT_ASSERT(ret == RT_EOK);
#endif
    if (state->panel_state.area_display == ENABLE_AREA_DISPLAY &&
            state->conn_state.funcs && state->conn_state.funcs->set_area)
    {
        struct DISPLAY_RECT display_rect;

        display_rect.x = state->crtc_state.post_scale.dstX;
        display_rect.y = state->crtc_state.post_scale.dstY;
        display_rect.w =  state->crtc_state.post_scale.dstW;
        display_rect.h =  state->crtc_state.post_scale.dstH;

        if (state->mode.flags & DSC_ENABLE)
            state->crtc_state.funcs->set_area(state, &display_rect);
        state->conn_state.funcs->set_area(state, &display_rect);
    }
    state->crtc_state.funcs->commit(state);

    return RT_EOK;
}

#ifdef RT_HW_LCD_GPIO_TRIGGER_PIN
/**
 * @brief  Panel TE irq callback.
 */
static void te_irq_callback(void *args)
{
    struct display_state *state = (struct display_state *)args;

    state->te_irq = true;
    rt_event_send(state->te_event, state->te_irq);
}

/**
 * @brief  register te interrupt.
 */
static void init_te_interrupt(struct display_state *state)
{
    rt_err_t ret;

    state->te_event = rt_event_create("gpio_te_event", RT_IPC_FLAG_FIFO);
    rt_pin_mode(state->panel_state.gpio_trigger_pin_info, PIN_MODE_INPUT);
    ret = rt_pin_attach_irq(state->panel_state.gpio_trigger_pin_info,
                            PIN_IRQ_MODE_RISING,
                            te_irq_callback, state);
    RT_ASSERT(ret == RT_EOK);
}
#endif

/**
 * @brief  Init lcd device.
 * @param  dev: rt_device_t for lcd.
 * return rt_err_t.
 */
static rt_err_t rt_lcd_init(rt_device_t dev)
{
    struct display_state *state = (struct display_state *)dev->user_data;

    rk_display_init(state);

    /**
     * init rt_device_graphic_info structure.
     */
    state->graphic_info.bits_per_pixel = 8;
    state->graphic_info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_RGB332;
    state->graphic_info.framebuffer = (void *)state->rtt_framebuffer;
    state->graphic_info.width = state->mode.crtcHdisplay;
    state->graphic_info.height = state->mode.crtcVdisplay;

#ifdef RT_HW_LCD_GPIO_TRIGGER_PIN
    init_te_interrupt(state);
    state->te_irq = false;
#endif

#if DISPLAY_DEBUG
    state->inited = true;
#endif
    return RT_EOK;
}

/**
 * @brief  Open lcd device.
 * @param  dev: rt_device_t for lcd.
 * @oflag  oflag: open flag.
 * return rt_err_t.
 */
static rt_err_t rt_lcd_open(rt_device_t dev, rt_uint16_t oflag)
{
    return RT_EOK;
}

/**
 * @brief  Close lcd device.
 * @param  dev: rt_device_t for lcd.
 * return rt_err_t.
 */
static rt_err_t rt_lcd_close(rt_device_t dev)
{
#if defined(RT_USING_RTGUI) && defined(RT_USING_LARGE_HEAP)
    struct display_state *state = (struct display_state *)dev->user_data;

    rt_dma_free_large(state->rtt_framebuffer);
#endif
    return RT_EOK;
}

/**
 * @brief  Close lcd device.
 * @param  dev: rt_device_t for lcd.
 * @param  cmd: control command.
 * @param  args: args between driver and userspace.
 * return rt_err_t.
 */
static rt_err_t rt_lcd_control(rt_device_t dev, int cmd, void *args)
{
    struct display_state *state = (struct display_state *)dev->user_data;
    const struct rockchip_crtc_funcs *crtc_funcs = state->crtc_state.funcs;
    struct crtc_lut_state *lut_state = NULL;
    struct CRTC_WIN_STATE *win_state;
    rt_err_t ret = RT_EOK;
    int i = 0;

#if DISPLAY_DEBUG
    if (!state->inited)
        display_dbg(dev, "[please check] display not inited do: 0x%lx\n", cmd);

    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
    case RTGRAPHIC_CTRL_SET_MODE:
    case RK_DISPLAY_CTRL_COMMIT:
    case RK_DISPLAY_CTRL_DISPLAY_SYNC:
    case RK_DISPLAY_CTRL_LOAD_LUT:
    case RK_DISPLAY_CTRL_DISABLE_LUT:
    case RK_DISPLAY_CTRL_SET_AOD_OFF:
    case RK_DISPLAY_CTRL_SET_AOD_ON:
    case RK_DISPLAY_CTRL_SET_BCSH:
    case RK_DISPLAY_CTRL_SET_COLOR_MATRIX:
    case RK_DISPLAY_CTRL_SET_GAMMA_COE:
    case RK_DISPLAY_CTRL_SET_PLANE:
    case RK_DISPLAY_CTRL_SET_POST_CLIP:
    case RK_DISPLAY_CTRL_SET_SCALE:
    case RK_DISPLAY_CTRL_UPDATE_BL:
        if (!state->enabled)
            display_dbg(dev, "[please check] display not enabled when do: 0x%lx\n", cmd);
        break;
    /*
     * mipi_switch only needs vop's clk enabled, vop's clk enable by default,
     * So we can do mipi_switch when first boot.
     */
    case RK_DISPLAY_CTRL_MIPI_SWITCH:
        if ((!state->enabled) && !boot_flag)
            display_dbg(dev, "[please check] display not enabled when do: 0x%lx\n", cmd);
        break;
    case RK_DISPLAY_CTRL_ENABLE:
    case RTGRAPHIC_CTRL_POWERON:
        display_dbg(dev, "%s enable \n", __func__);
        if (state->enabled)
            display_dbg(dev, "%s re-enable: 0x%lx \n", __func__, cmd);
        boot_flag = false;
        state->enabled = true;
        break;
    case RK_DISPLAY_CTRL_DISABLE:
    case RTGRAPHIC_CTRL_POWEROFF:
        display_dbg(dev, "%s disable \n", __func__);
        if (!state->enabled)
            display_dbg(dev, "%s re-disable: 0x%lx\n", __func__, cmd);
        boot_flag = false;
        state->enabled = false;
        break;
    case RTGRAPHIC_CTRL_GET_INFO:
    case RK_DISPLAY_CTRL_AP_COP_MODE:
    default:
        break;
    }
#endif

    rt_mutex_take(&state->display_lock, RT_WAITING_FOREVER);
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
#if defined(RT_USING_LITTLEVGL2RTT)
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, state->rtt_framebuffer, (RT_LV_HOR_RES * RT_LV_VER_RES));
#elif defined(RT_USING_RTGUI)
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, state->rtt_framebuffer, (state->mode.crtcHdisplay * state->mode.crtcVdisplay));
#endif
        state->crtc_state.funcs->set_scale(state);
        state->crtc_state.funcs->set_plane(state, &state->crtc_state.win_state[0]);
        rk_display_commit(state);
        break;
    case RTGRAPHIC_CTRL_POWERON:
        pm_runtime_request(PM_RUNTIME_ID_VOP);

        if (state->conn_state.funcs && state->conn_state.funcs->prepare)
            state->conn_state.funcs->prepare(state);
        state->panel_state.funcs->prepare(state);
        state->crtc_state.funcs->enable(state);
        if (state->conn_state.funcs && state->conn_state.funcs->enable)
            state->conn_state.funcs->enable(state);
        if (!state->AP_COP_mode)
            state->panel_state.funcs->enable(state);
        break;
    case RTGRAPHIC_CTRL_POWEROFF:
        state->panel_state.funcs->disable(state);
        if (!state->AP_COP_mode)
            state->panel_state.funcs->unprepare(state);
        if (state->conn_state.funcs && state->conn_state.funcs->disable)
            state->conn_state.funcs->disable(state);
        state->crtc_state.funcs->disable(state);

        pm_runtime_release(PM_RUNTIME_ID_VOP);
        break;
    case RTGRAPHIC_CTRL_GET_INFO:
        memcpy(args, &state->graphic_info, sizeof(state->graphic_info));
        break;
    case RTGRAPHIC_CTRL_SET_MODE:
        break;
    case RK_DISPLAY_CTRL_SET_PLANE:
        win_state = (struct CRTC_WIN_STATE *)args;
        memcpy(&state->crtc_state.win_state[win_state->winId],
               args, sizeof(struct CRTC_WIN_STATE));
        if (win_state->yrgbLength)
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)win_state->yrgbAddr,
                                 win_state->yrgbLength);
        if (win_state->cbcrLength)
            rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)win_state->cbcrAddr,
                                 win_state->cbcrLength);
        break;
    case RK_DISPLAY_CTRL_SET_SCALE:
        memcpy(&state->crtc_state.post_scale, args, sizeof(struct VOP_POST_SCALE_INFO));
        state->crtc_state.funcs->set_scale(state);
        break;
    case RK_DISPLAY_CTRL_COMMIT:
        for (i = 0; i < RK_DISPLAY_MAX_CRTC_WIN_NUM; i++)
        {
            state->crtc_state.win_state[i].winId = i;
            if (state->crtc_state.win_state[i].winUpdate)
                crtc_funcs->set_plane(state, &state->crtc_state.win_state[i]);
        }
        rk_display_commit(state);
        break;
    case RK_DISPLAY_CTRL_LOAD_LUT:
        lut_state = (struct crtc_lut_state *)args;
        state->crtc_state.funcs->load_lut(state, lut_state->win_id,
                                          lut_state->lut, lut_state->lut_size);
        break;
    case RK_DISPLAY_CTRL_DISABLE_LUT:
        lut_state = (struct crtc_lut_state *)args;
        state->crtc_state.funcs->disable_lut(state, lut_state->win_id);
        break;
    case RK_DISPLAY_CTRL_ENABLE:
        rk_display_enable(state);
        break;
    case RK_DISPLAY_CTRL_DISABLE:
        rk_display_disable(state);
        break;
    case RK_DISPLAY_CTRL_SET_AOD_OFF:
        state->panel_state.funcs->set_aod(state, false);
        break;
    case RK_DISPLAY_CTRL_SET_AOD_ON:
        state->panel_state.funcs->set_aod(state, true);
        break;
    case RK_DISPLAY_CTRL_SET_COLOR_MATRIX:
        crtc_funcs->set_color_matrix_coe(state, (struct VOP_COLOR_MATRIX_INFO *)args);
        break;
    case RK_DISPLAY_CTRL_SET_GAMMA_COE:
        crtc_funcs->set_gamma_coe(state, (struct VOP_GAMMA_COE_INFO *)args);
        break;
    case RK_DISPLAY_CTRL_SET_BCSH:
        crtc_funcs->set_bcsh(state, (struct VOP_BCSH_INFO *)args);
        break;
    case RK_DISPLAY_CTRL_MIPI_SWITCH:
        crtc_funcs->mipi_switch(state, *(eVOP_MipiSwitchPath *)args);
        break;
    case RK_DISPLAY_CTRL_SET_POST_CLIP:
        crtc_funcs->set_post_clip(state, (struct VOP_POST_CLIP_INFO *)args);
        break;
    case RK_DISPLAY_CTRL_DISPLAY_SYNC:
        if (crtc_funcs->display_sync(state))
            ret = RT_EOK;
        else
            ret = -RT_EBUSY;
        break;
    case RK_DISPLAY_CTRL_AP_COP_MODE:
        state->AP_COP_mode = (uint8_t)(uint32_t)args;
        break;
    case RK_DISPLAY_CTRL_UPDATE_BL:
        state->panel_state.funcs->update_bl(state, *(uint16_t *)args);
        break;
    case RK_DISPLAY_CTRL_CLOSE_WIN:
        state->crtc_state.win_state[(int)args].winEn = 0;
        state->crtc_state.win_state[(int)args].winUpdate = 1;
        break;
    default:
        rt_kprintf("Unsupport lcd control cmd: 0x%lx\n", cmd);
        break;
    }
    rt_mutex_release(&state->display_lock);

    return ret;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops rt_lcd_ops =
{
    rt_lcd_init,
    rt_lcd_open,
    rt_lcd_close,
    NULL,
    NULL,
    rt_lcd_control,
};
#endif

/** @defgroup Display_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  Regsiter lcd device.
 * return int.
 */
int rt_hw_lcd_init(void)
{
    struct display_state *state;

    state = rt_malloc(sizeof(struct display_state));
    if (!state)
    {
        rt_kprintf("alloc display_state failed\n");
        return -RT_ENOMEM;
    }
    rt_memset(state, 0, sizeof(struct display_state));

    /* init rt_device structure */
    state->lcd.type = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    state->lcd.ops = &rt_lcd_ops;
#else
    state->lcd.init = rt_lcd_init;
    state->lcd.open = rt_lcd_open;
    state->lcd.close = rt_lcd_close;
    state->lcd.read = NULL;
    state->lcd.write = NULL;
    state->lcd.control = rt_lcd_control;
#endif
    state->lcd.user_data = (void *)state;
    if (rt_mutex_init(&(state->display_lock), "displayLock", RT_IPC_FLAG_FIFO) != RT_EOK)
        RT_ASSERT(0);

    state->isr_workqueue = rt_workqueue_create("displayWQ", 256, 5);
    RT_ASSERT(state->isr_workqueue != NULL);
    rt_event_init(&state->event, "displayEvent", RT_IPC_FLAG_FIFO);

    /* register lcd device to RT-Thread */
    rt_device_register(&state->lcd, "lcd", RT_DEVICE_FLAG_RDWR);

    return 0;
}

/**
 * @brief  Convert vmode to drm mode.
 * @param  mode_input: input vmode.
 * @param  mode_output: output drm mode.
 * @param  area_w: area_w for post scale.
 * @param  area_h: area_h for post scale.
 */
void rk_display_vmode_to_drm_mode(struct video_mode *mode_input,
                                  struct DISPLAY_MODE_INFO *mode_output,
                                  uint16_t area_w, uint16_t area_h)
{
    mode_output->crtcClock = mode_input->pixclock;
    if (area_w)
        mode_output->crtcHdisplay = area_w;
    else
        mode_output->crtcHdisplay = mode_input->xres;
    mode_output->crtcHsyncStart = mode_output->crtcHdisplay +
                                  mode_input->left_margin;
    mode_output->crtcHsyncEnd = mode_output->crtcHsyncStart +
                                mode_input->hsync_len;
    mode_output->crtcHtotal = mode_output->crtcHsyncEnd +
                              mode_input->right_margin;

    if (area_h)
        mode_output->crtcVdisplay = area_h;
    else
        mode_output->crtcVdisplay = mode_input->yres;
    mode_output->crtcVsyncStart = mode_output->crtcVdisplay +
                                  mode_input->lower_margin;
    mode_output->crtcVsyncEnd = mode_output->crtcVsyncStart +
                                mode_input->vsync_len;
    mode_output->crtcVtotal = mode_output->crtcVsyncEnd +
                              mode_input->upper_margin;
    mode_output->flags = mode_input->flag;
}

INIT_DEVICE_EXPORT(rt_hw_lcd_init);

/** @} */

#endif /* RT_USING_VOP */

/** @} */

/** @} */
