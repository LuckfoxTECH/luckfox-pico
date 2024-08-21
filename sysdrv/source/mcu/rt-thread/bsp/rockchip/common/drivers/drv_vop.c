/**
  * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_vop.c
  * @version V0.1
  * @brief   display vop driver for pisces
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

/** @addtogroup VOP
 *  @{
 */

/** @defgroup VOP_How_To_Use How To Use
 *  @{

 - **The VOP driver use to provide driver display controller VOP, mainly include the
 follwing function**:

    - 1) Set plane config for overlay;
    - 2) Set post scale;
    - 3) Set display quality adjust;
    - 4) Power on or off VOP;
    - 5) Set MIPI switch;
    - 6) Other post function;

 @} */

#include <rthw.h>
#include <rtdevice.h>
#include <rtthread.h>
#ifdef RT_USING_VOP

#include "drv_clock.h"
#include "drv_display.h"
#include "hal_base.h"

/********************* Private MACRO Definition ******************************/
#define VOP_ACLK_FREQ      300000000

#ifndef BIT
#define BIT(nr) (1UL << (nr))
#endif

/********************* Private Variable Definition ******************************/
static struct display_state *gstate;

/********************* Private Function Definition ***************************/
/** @defgroup VOP_Private_Function Private Function
 *  @{
 */

/**
 * @brief  Poweron VOP.
 * @param  state: display state.
 */
static void rockchip_vop_power_on(struct display_state *state)
{
    HAL_CRU_ClkEnable(HCLK_VOP_GATE);
    HAL_CRU_ClkEnable(ACLK_VOP_GATE);
    HAL_CRU_VopDclkEnable(DCLK_VOP_GATE);
}

/**
 * @brief  Poweroff VOP.
 * @param  state: display state.
 */
static void rockchip_vop_power_off(struct display_state *state)
{
    HAL_CRU_VopDclkDisable(DCLK_VOP_GATE);
    HAL_CRU_ClkDisable(ACLK_VOP_GATE);
    HAL_CRU_ClkDisable(HCLK_VOP_GATE);
}

/**
 * @brief  Provide format convert from userspace to VOP hardware.
 * @param  data_format: userspace format.
 * @param  win_en: win state.
 * @return VOP hardware format.
 */
static uint8_t rockchip_vop_convert_format(uint8_t data_format, uint8_t win_en)
{
    uint8_t val;

    switch (data_format)
    {
    case RTGRAPHIC_PIXEL_FORMAT_ARGB888:
    case RTGRAPHIC_PIXEL_FORMAT_ABGR888:
        val = VOP_FMT_ARGB8888;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_RGB888:
        val = VOP_FMT_RGB888;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_RGB565:
        val = VOP_FMT_RGB565;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_RGB444:
        val = VOP_FMT_RGB444;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_YUV420:
        val = VOP_FMT_YUV420SP;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_YUV422:
        val = VOP_FMT_YUV422SP;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_YUV444:
        val = VOP_FMT_YUV444SP;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_YVYU422:
        val = VOP_FMT_YVYU422;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_VYUY422:
        val = VOP_FMT_VYUY422;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_YUV420_4:
        val = VOP_FMT_YUV420SP_4;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_YUV422_4:
        val = VOP_FMT_YUV422SP_4;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_YUV444_4:
        val = VOP_FMT_YUV444SP_4;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_YVYU422_4:
        val = VOP_FMT_YVYU422_4;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_VYUY422_4:
        val = VOP_FMT_VYUY422_4;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_RGB332:
    case RTGRAPHIC_PIXEL_FORMAT_GRAY256:
        val = VOP_FMT_8BPP;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_GRAY16:
        val = VOP_FMT_4BPP;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_GRAY4:
        val = VOP_FMT_2BPP;
        break;
    case RTGRAPHIC_PIXEL_FORMAT_GRAY1:
        val = VOP_FMT_1BPP;
        break;
    default:
        if (win_en)
            rt_kprintf("Unsupport format: 0x%x\n", data_format);
        val = VOP_FMT_ARGB8888;
        break;
    }

    return val;
}

/**
 * @brief  VOP irq work for display sync.
 * @param  work: rt-thread work.
 * @param  work_data: unused now.
 */
static void rockchip_vop_isr_work(struct rt_work *work, void *work_data)
{
    int i = 0;
    struct display_state *state = gstate;
    uint32_t vop_event = 0;

    rt_mutex_take(&state->display_lock, RT_WAITING_FOREVER);
    for (i = 0; i < RK_DISPLAY_MAX_CRTC_WIN_NUM; i++)
    {
        if (state->crtc_state.win_state[i].winUpdate)
            vop_event |= BIT(state->crtc_state.win_state[i].winId);
        state->crtc_state.win_state[i].winEn = 0;
        state->crtc_state.win_state[i].winUpdate = 0;
        if (vop_event)
            rt_event_send(&state->event, vop_event);
    }
    rt_mutex_release(&state->display_lock);
}

/**
 * @brief  VOP irq function.
 * @param  irq: VOP irq number.
 * @param  param: unused now.
 */
static void rockchip_vop_irq_handle(int irq, void *param)
{
    struct display_state *state = gstate;
    struct crtc_state *crtc_state = &state->crtc_state;
    uint32_t val;
    rt_base_t level;

    /* disable interrupt */
    level = rt_hw_interrupt_disable();

    val = HAL_VOP_IrqHandler(VOP);
    if (val & VOP_INTR_STATUS_DSP_HOLD_VALID_INTR_STS_MASK)
    {
        rt_work_init(&state->work, rockchip_vop_isr_work, NULL);
        rt_workqueue_dowork(state->isr_workqueue, &state->work);
    }

    if (val & VOP_INTR_STATUS_DMA_FRM_FSH_INTR_STS_MASK)
    {
        if (crtc_state->wait_frm_fsh == RT_TRUE)
            rt_event_send(crtc_state->frm_fsh_event, true);
    }

    /* enable interrupt */
    rt_hw_interrupt_enable(level);
}

/**
 * @brief  Commit one frame display.
 * @param  state: display state.
 */
static void rockchip_vop_commit(struct display_state *state)
{
    int i = 0;
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_Commit(crtc_state->hw_base);
    if (state->panel_state.display_mode == DISPLAY_CMD_MODE)
        HAL_VOP_EdpiFrmSt(crtc_state->hw_base);

    /**
     * In CMD mode, we must wait the current frame start to display
     * and the next frame wait current display complete.
     * we can use config done state to identify the frame start,
     * and use dsp hold mode to identify the display complete state.
     */
    if (state->panel_state.display_mode == DISPLAY_CMD_MODE)
    {
        while (HAL_VOP_CommitPost(crtc_state->hw_base))
        {
            if (i++ > RK_DISPLAY_TIME_OUT)
            {
                rt_kprintf("rockchip_vop_commit wait vop display time out\n");
                break;
            }
            rt_thread_mdelay(1);
        }
    }

}

/**
 * @brief  Load lookup table for bpp format.
 * @param  state: display state.
 * @param  winId: win id.
 * @param  lut: lookup table.
 * @param  lut_size: lookup table size.
 */
static void rockchip_vop_load_lut(struct display_state *state, uint8_t winId,
                                  uint32_t *lut, uint16_t lut_size)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_LoadLut(crtc_state->hw_base, winId, lut, lut_size);
}

/**
 * @brief  Disable lookup table for bpp format.
 * @param  state: display state.
 * @param  winId: win id.
 */
static void rockchip_vop_disable_lut(struct display_state *state, uint8_t winId)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_DisableLut(crtc_state->hw_base, winId);
}

/**
 * @brief  Set VOP post scale.
 * @param  state: display state.
 */
static void rockchip_vop_set_scale(struct display_state *state)
{
    struct crtc_state *crtc_state = &state->crtc_state;
    struct VOP_POST_SCALE_INFO *post_scale = &crtc_state->post_scale;

    if (post_scale->dstX % state->panel_state.xpos_align)
        rt_kprintf("Error: post scale dstX[%d] is not aligned as:%d\n",
                   post_scale->dstX, state->panel_state.xpos_align);

    if (post_scale->dstY % state->panel_state.ypos_align)
        rt_kprintf("Error: post scale dstY[%d] is not aligned as:%d\n",
                   post_scale->dstY, state->panel_state.ypos_align);

    if (post_scale->dstW % state->panel_state.xact_align)
        rt_kprintf("Error: post scale dstW[%d] is not aligned as:%d\n",
                   post_scale->dstW, state->panel_state.xact_align);

    if (post_scale->dstH % state->panel_state.yact_align)
        rt_kprintf("Error: post scale dstH[%d] is not aligned as:%d\n",
                   post_scale->dstH, state->panel_state.yact_align);

    if (post_scale->dstX + post_scale->dstW > state->panel_state.vmode.xres)
    {
        rt_kprintf("Error: post scale dstX[%d]+dstW[%d] > panel xres[%d]\n",
                   post_scale->dstX, post_scale->dstW, state->panel_state.vmode.xres);
        post_scale->dstW = state->panel_state.vmode.xres - post_scale->dstX;
    }

    if (post_scale->dstY + post_scale->dstH > state->panel_state.vmode.yres)
    {
        rt_kprintf("Error: post scale dstY[%d]+dstH[%d] > panel yres[%d]\n",
                   post_scale->dstY, post_scale->dstH, state->panel_state.vmode.yres);
        post_scale->dstH = state->panel_state.vmode.yres - post_scale->dstH;
    }

    if (state->panel_state.area_display == DISABLE_AREA_DISPLAY)
    {
        uint8_t post_h_scale_fac, post_v_scale_fac;

        post_h_scale_fac = post_scale->dstW / post_scale->srcW;
        post_v_scale_fac = post_scale->dstH / post_scale->srcH;
        post_scale->dstX = 0;
        post_scale->dstY = 0;
        post_scale->dstW = state->panel_state.vmode.xres;
        post_scale->dstH = state->panel_state.vmode.yres;
        post_scale->srcW = post_scale->dstW / post_h_scale_fac;
        post_scale->srcH = post_scale->dstH / post_v_scale_fac;
    }

    if ((post_scale->dstW  == post_scale->srcW) ||
            (post_scale->dstW  == 2 * post_scale->srcW) ||
            (post_scale->dstW  == 3 * post_scale->srcW) ||
            (post_scale->dstW  == 4 * post_scale->srcW))
    {
        post_scale->postSclHmode = post_scale->dstW / post_scale->srcW - 1;
    }
    else
    {
        post_scale->postSclHmode = 0;
        rt_kprintf("Error: Unsupport post scale size: srcw:%d, dstw:%d\n",
                   post_scale->srcW, post_scale->dstW);
        post_scale->dstW = post_scale->srcW;
    }

    if ((post_scale->dstH  == post_scale->srcH) ||
            (post_scale->dstH  == 2 * post_scale->srcH) ||
            (post_scale->dstH  == 3 * post_scale->srcH) ||
            (post_scale->dstH  == 4 * post_scale->srcH))
    {
        post_scale->postSclVmode = post_scale->dstH / post_scale->srcH - 1;
    }
    else
    {
        post_scale->postSclVmode = 0;
        rt_kprintf("Error: Unsupport post scale size: srch:%d, dsth:%d\n",
                   post_scale->srcH, post_scale->dstH);
        post_scale->dstH = post_scale->srcH;
    }

    if (post_scale->postSclHmode || post_scale->postSclVmode)
        post_scale->postScaleEn = 1;
    else
        post_scale->postScaleEn = 0;

    if (state->panel_state.area_display == ENABLE_AREA_DISPLAY)
        rk_display_vmode_to_drm_mode(&state->panel_state.vmode,
                                     &state->mode, post_scale->dstW, post_scale->dstH);

    HAL_VOP_ModeInit(crtc_state->hw_base, &state->mode, &crtc_state->post_scale);
    HAL_VOP_PostScaleInit(crtc_state->hw_base, &state->mode, &crtc_state->post_scale);
}

/**
 * @brief  Set VOP win config.
 * @param  state: display state.
 * @param  win_state: win state.
 */
static void rockchip_vop_set_plane(struct display_state *state,
                                   struct CRTC_WIN_STATE *win_state)
{
    uint8_t format;
    HAL_Status ret;
    struct crtc_state *crtc_state = &state->crtc_state;

    format = win_state->format;
    win_state->hwFormat = rockchip_vop_convert_format(format, win_state->winEn);

    if (win_state->srcX + win_state->srcW > state->panel_state.vmode.xres)
    {
        rt_kprintf("Error: set plane size, srcX[%d] + srcW[%d] > panel xres[%d]\n",
                   win_state->srcX, win_state->srcW, state->panel_state.vmode.xres);
        win_state->srcW = state->panel_state.vmode.xres - win_state->srcX;
    }

    if (win_state->srcY + win_state->srcH > state->panel_state.vmode.yres)
    {
        rt_kprintf("Error: set plane size, srcY[%d] + srcH[%d] > panel yres[%d]\n",
                   win_state->srcY, win_state->srcH, state->panel_state.vmode.yres);
        win_state->srcH = state->panel_state.vmode.yres - win_state->srcY;
    }

    if (state->panel_state.area_display == ENABLE_AREA_DISPLAY)
    {
        win_state->hwCrtcX = win_state->srcX;
        win_state->hwCrtcY = win_state->srcY - crtc_state->post_scale.dstY;
    }
    else
    {
        win_state->hwCrtcX = win_state->srcX;
        win_state->hwCrtcY = win_state->srcY;
    }

    ret = HAL_VOP_SetPlane(crtc_state->hw_base, win_state, &state->mode);

    if (ret)
        rt_kprintf("Error: rockchip_vop_set_plane failed, ret = %d\n", ret);
}

/**
 * @brief  Enable VOP power.
 * @param  state: display state.
 */
static void rockchip_vop_enable(struct display_state *state)
{
    struct crtc_state *crtc_state = &state->crtc_state;

#ifndef IS_FPGA
    int ret = 0;
#endif

    rockchip_vop_power_on(state);
#ifndef IS_FPGA
    ret = HAL_CRU_ClkSetFreq(ACLK_VOP, VOP_ACLK_FREQ);
    RT_ASSERT(ret == HAL_OK);
    ret = HAL_CRU_ClkSetFreq(DCLK_VOP_S, state->mode.crtcClock * 1000);
    RT_ASSERT(ret == HAL_OK);
#endif

    HAL_VOP_Init(crtc_state->hw_base, &state->mode);
    HAL_VOP_ModeInit(crtc_state->hw_base, &state->mode, &crtc_state->post_scale);
    if (state->panel_state.cmd_type == CMD_TYPE_MCU)
        HAL_VOP_McuModeInit(crtc_state->hw_base);
    HAL_VOP_PostScaleInit(crtc_state->hw_base, &state->mode, &crtc_state->post_scale);
    HAL_VOP_PolarityInit(crtc_state->hw_base, &state->mode,
                         state->panel_state.conn_type);
    HAL_VOP_OutputInit(crtc_state->hw_base,
                       &state->mode, state->panel_state.bus_format);
    if (state->panel_state.display_mode == DISPLAY_CMD_MODE)
        HAL_VOP_EdpiInit(crtc_state->hw_base, !!state->panel_state.gpio_trigger_pin_info);

    HAL_VOP_EnableFsIrq(crtc_state->hw_base);
    HAL_VOP_EnableFrmFshIrq(crtc_state->hw_base);
    HAL_VOP_EnableDebugIrq(crtc_state->hw_base);
}

/**
 * @brief  Disable VOP power.
 * @param  state: display state.
 */
static void rockchip_vop_disable(struct display_state *state)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_DisableFsIrq(crtc_state->hw_base);
    HAL_VOP_DisableFrmFshIrq(crtc_state->hw_base);
    HAL_VOP_DisableDebugIrq(crtc_state->hw_base);
    HAL_VOP_DeInit(crtc_state->hw_base);

    rockchip_vop_power_off(state);
}

/**
 * @brief  VOP hardware init.
 * @param  state: display state.
 */
static void rockchip_vop_init(struct display_state *state)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    crtc_state->post_scale.srcW = state->mode.crtcHdisplay;
    crtc_state->post_scale.srcH = state->mode.crtcVdisplay;
    crtc_state->post_scale.dstX = 0;
    crtc_state->post_scale.dstY = 0;
    crtc_state->post_scale.dstW = state->mode.crtcHdisplay;
    crtc_state->post_scale.dstH = state->mode.crtcVdisplay;
    crtc_state->post_scale.postScaleEn = 0;
    crtc_state->post_scale.postSclHmode = 1;
    crtc_state->post_scale.postSclVmode = 1;

    crtc_state->irqno = VOP_IRQn;
    crtc_state->hw_base = VOP;

    gstate = state;

    crtc_state->frm_fsh_event = rt_event_create("frm_fsh_event", RT_IPC_FLAG_FIFO);
    crtc_state->wait_frm_fsh = RT_FALSE;

    rt_hw_interrupt_install(crtc_state->irqno, rockchip_vop_irq_handle, RT_NULL, RT_NULL);
    rt_hw_interrupt_umask(crtc_state->irqno);
}

/**
 * @brief  Set color matrix for display quality adjust.
 * @param  state: display state.
 * @param  color_matrix_info: color matrix info.
 */
static void rockchip_vop_set_color_matrix_coe(struct display_state *state,
        struct VOP_COLOR_MATRIX_INFO *color_matrix_info)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_PostColorMatrixInit(crtc_state->hw_base, color_matrix_info);
}

/**
 * @brief  Set gamma adjust.
 * @param  state: display state.
 * @param  gamma_info: gamma config info.
 */
static void rockchip_vop_set_gamma_coe(struct display_state *state,
                                       struct VOP_GAMMA_COE_INFO *gamma_info)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_PostGammaInit(crtc_state->hw_base, gamma_info);
}

/**
 * @brief  Set bcsh for display quality adjust.
 * @param  state: display state.
 * @param  bcsh_info: include brightness, contrast, saturation and hue.
 */
static void rockchip_vop_set_bcsh(struct display_state *state,
                                  struct VOP_BCSH_INFO *bcsh_info)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_PostBCSH(crtc_state->hw_base, bcsh_info);
}

/**
 * @brief  Mipi switch path for AP and current soc
 * @param  state: display state.
 * @param  path: mipi switch path.
 */
static void rockchip_vop_mipi_switch(struct display_state *state, eVOP_MipiSwitchPath path)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_MipiSwitch(crtc_state->hw_base, path);
}

/**
 * @brief  Set clip config.
 * @param  state: display state.
 * @param  clip_info: clip info.
 */
static void rockchip_vop_set_post_clip(struct display_state *state,
                                       struct VOP_POST_CLIP_INFO *clip_info)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_PostClipInit(crtc_state->hw_base, clip_info);
}

/**
 * @brief  Send MCU CMD and data for MCU panel.
 * @param  state: display state.
 * @param  type: cmd or data.
 * @param  value: config value.
 */
static void rockchip_vop_send_mcu_cmd(struct display_state *state,
                                      uint8_t type, uint32_t value)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_SendMcuCmd(crtc_state->hw_base, type, value);
}

/**
 * @brief  Set VOP display area for DSC panel.
 * @param  state: display state.
 * @param  display_rect: display area rect.
 */
static void rockchip_vop_set_area(struct display_state *state,
                                  struct DISPLAY_RECT *display_rect)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    HAL_VOP_SetArea(crtc_state->hw_base, display_rect);
}

/**
 * @brief  Control dsp hold mode.
 * @param  state: display state.
 * @param  enable: enable or disable hold mode.
 */
static void rockchip_enable_dsp_hold_mode(struct display_state *state,
        uint8_t enable)
{
    struct crtc_state *crtc_state = &state->crtc_state;

    if (enable)
        HAL_VOP_EnableDspHoldIrq(crtc_state->hw_base);
    else
        HAL_VOP_DisableDspHoldIrq(crtc_state->hw_base);
}

/**
 * @brief  get display sync state.
 * @param  state: display state.
 * @return display sync state.
 */
static int rockchip_display_sync(struct display_state *state)
{
    struct crtc_state *crtc_state = &state->crtc_state;
    uint32_t event = false;
    rt_err_t ret;

    if (!HAL_VOP_CommitPrepare(crtc_state->hw_base))
    {
        crtc_state->wait_frm_fsh = RT_TRUE;
        ret = rt_event_recv(crtc_state->frm_fsh_event,
                            true,
                            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                            RK_DISPLAY_TIME_OUT, &event);
        if (!event)
            rt_kprintf("Wait frame finish timeout %d\n", ret);

        crtc_state->wait_frm_fsh = RT_FALSE;
    }

    return event;
}

/** @} */

/** @defgroup VOP_Public_Functions Public Functions
 *  @{
 */

/**
 * @brief  VOP control functions.
 * @param  init: Init VOP hardware config and other config.
 * @param  enable: Enable VOP power.
 * @param  disable: Disable VOP power.
 * @param  set_plane: Set VOP wins config.
 * @param  set_scale: Set post scale config.
 * @param  set_area: Set VOP area config, this only be called when in DSC mode.
 * @param  commit: The last callback for one frame commit.
 * @param  load_lut: Load look up table for bpp format.
 * @param  set_color_matrix_coe: Set color adjust matrix config.
 * @param  set_gamma_coe: Set gamma config for color adjust.
 * @param  set_bcsh: Set bcsh config to adjust brightness, contrast, saturation and hue.
 * @param  mipi_switch: Set mipi switch to control use internal dphy or AP dphy.
 * @param  send_mcu_cmd: Send MCU init cmd.
 * @param  enable_dsp_hold_mode: Control dsp hold mode.
 */
const struct rockchip_crtc_funcs rockchip_vop_funcs =
{
    .init = rockchip_vop_init,
    .enable = rockchip_vop_enable,
    .disable = rockchip_vop_disable,
    .set_plane = rockchip_vop_set_plane,
    .set_scale = rockchip_vop_set_scale,
    .set_area = rockchip_vop_set_area,
    .commit = rockchip_vop_commit,
    .load_lut = rockchip_vop_load_lut,
    .disable_lut = rockchip_vop_disable_lut,
    .set_color_matrix_coe = rockchip_vop_set_color_matrix_coe,
    .set_gamma_coe = rockchip_vop_set_gamma_coe,
    .set_bcsh = rockchip_vop_set_bcsh,
    .set_post_clip = rockchip_vop_set_post_clip,
    .mipi_switch = rockchip_vop_mipi_switch,
    .send_mcu_cmd = rockchip_vop_send_mcu_cmd,
    .enable_dsp_hold_mode = rockchip_enable_dsp_hold_mode,
    .display_sync = rockchip_display_sync,
    .power_on = rockchip_vop_power_on,
    .power_off = rockchip_vop_power_off,
};
/** @} */

#endif

/** @} */

/** @} */
