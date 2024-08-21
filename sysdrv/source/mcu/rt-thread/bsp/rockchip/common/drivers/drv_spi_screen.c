/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    drv_spi_screen.c
  * @version V0.1
  * @brief   spi display driver
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-02-17     Chris Zhong     first implementation
  *
  ******************************************************************************
  */

/** @addtogroup RKBSP_Driver_Reference
*  @{
*/

/** @addtogroup SPI_Screen
 *  @{
 */

/** @defgroup SPI_Panel_How_To_Use How To Use
 *  @{

The spi screen driver can be used in the following three scenarios:

    1. Use rt_device_open to config spi bus and malloc the framebuffer.
    2. use rt_device_control(g_display_dev, RTGRAPHIC_CTRL_GET_INFO, &graphic_info)
       to get display parameter.
    3. Use rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWERON, NULL) to
       power on spi screen panel, this function would send some commands to
       power on spi panel.
    4. Fill the display content into graphic_info.framebuffer
    5. Use rt_device_control(g_display_dev, RTGRAPHIC_CTRL_RECT_UPDATE, &graphic_info)
       to fresh the panel.
    6. Repeat steps 5, 6
    7. Use rt_device_control(g_display_dev, RTGRAPHIC_CTRL_POWEROFF, NULL) to
       power off spi panel.
    8. Use rt_device_close to free framebuffer.

    Please find the samlpe code in spi_screen_test.c

 @} */

#include <rtdevice.h>
#include <rtthread.h>
#ifdef RT_USING_SPI_SCREEN

#include "board.h"
#include "drv_clock.h"
#include "hal_base.h"
#include "drv_heap.h"
#include "drv_panel.h"
#include "drv_panel_cfg.h"

struct spi_screen_state
{
    struct rt_device_graphic_info graphic_info;
    struct rt_device lcd;
    struct rt_spi_device *spi_device;
    struct rt_mutex display_lock;
};

static void spi_screen_send_spi_cmd(struct spi_screen_state *state,
                                    uint8_t type, const uint8_t *value,
                                    uint32_t size)
{
    struct rt_spi_device *spi_device = state->spi_device;

    HAL_GPIO_SetPinLevel(GPIO_DCX_GPIO_PORT, GPIO_DCX_GPIO_PIN_OUT, (type == 1) ? GPIO_HIGH : GPIO_LOW);

    /* send data or cmd*/
    rt_spi_transfer(spi_device, value, RT_NULL, size);

    return;
}

/**
 * @brief  Send spi command.
 * @param  state: display state.
 * @param  cmds: init command.
 */
static int spi_screen_send_spi_cmds(struct spi_screen_state *state,
                                    const struct rockchip_cmd *cmds, int size)
{
    uint16_t i;
    struct rt_spi_configuration cfg =
    {
        RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB,
        8, // data_width = 8, since commands are 8 bits data
        0,
        HAL_SPI_PANEL_SCLK,
    };

    if (!cmds)
        return -RT_EINVAL;

    rt_spi_configure(state->spi_device, &cfg);

    for (i = 0; i < size; i++)
    {
        spi_screen_send_spi_cmd(state, cmds[i].data_type,
                                cmds[i].payload, cmds[i].payload_length);

        if (cmds[i].delay_ms)
            rt_thread_delay(cmds[i].delay_ms);
    }

    return RT_EOK;
}

static void spi_screen_commit(struct spi_screen_state *state)
{
    struct rt_spi_device *spi_device = state->spi_device;
    int length = 0;
    struct rt_spi_configuration cfg =
    {
        RT_SPI_MASTER | RT_SPI_MODE_0 | RT_SPI_MSB,
        RT_HW_SPI_SCREEN_BPP,
        0,
        HAL_SPI_PANEL_SCLK,
    };

    if (!state->graphic_info.framebuffer)
        return;

    rt_spi_configure(state->spi_device, &cfg);

    HAL_GPIO_SetPinLevel(GPIO_DCX_GPIO_PORT, GPIO_DCX_GPIO_PIN_OUT, GPIO_HIGH);

    length = state->graphic_info.width * state->graphic_info.height * state->graphic_info.bits_per_pixel / 8;

    rt_spi_transfer(spi_device, state->graphic_info.framebuffer, RT_NULL, length);
}

static void spi_screen_enable(struct spi_screen_state *state)
{
    spi_screen_send_spi_cmds(state, spi_screen_cmd_on, HAL_ARRAY_SIZE(spi_screen_cmd_on));
}

static void spi_screen_disable(struct spi_screen_state *state)
{
    spi_screen_send_spi_cmds(state, spi_screen_cmd_off, HAL_ARRAY_SIZE(spi_screen_cmd_off));
}

/**
 * @brief  Open lcd device.
 * @param  dev: rt_device_t for lcd.
 * @oflag  oflag: open flag.
 * return rt_err_t.
 */
static rt_err_t spi_screen_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct spi_screen_state *state = (struct spi_screen_state *)dev->user_data;

    /* init rt_device_graphic_info structure. */
    state->graphic_info.bits_per_pixel = RT_HW_SPI_SCREEN_BPP;
    state->graphic_info.pixel_format = RT_HW_SPI_SCREEN_BUS_FORMAT;
    state->graphic_info.width = RT_HW_SPI_SCREEN_XRES;
    state->graphic_info.height = RT_HW_SPI_SCREEN_YRES;

    /* malloc fb memory, this is only use by RTGUI */
#if defined(RT_USING_LARGE_HEAP)
    uint32_t fb_length = 0;
    uint32_t *rtt_framebuffer = NULL;

    fb_length = RT_HW_SPI_SCREEN_XRES * RT_HW_SPI_SCREEN_YRES * RT_HW_SPI_SCREEN_BPP / 8;
    rtt_framebuffer = rt_dma_malloc_large(fb_length);
    if (rtt_framebuffer == RT_NULL)
    {
        rt_kprintf("malloc memory: 0x%x failed\n", fb_length);
        return -RT_ENOMEM;
    }
    rt_memset((void *)rtt_framebuffer, 0, fb_length);
    state->graphic_info.framebuffer = (void *)rtt_framebuffer;
#else
    state->graphic_info.framebuffer = NULL;
#endif

    /* init rockchip spi */
    state->spi_device = (struct rt_spi_device *)rt_device_find(HAL_PANEL_SPI);
    RT_ASSERT(state->spi_device);

    HAL_GPIO_SetPinDirection(GPIO_DCX_GPIO_PORT, GPIO_DCX_GPIO_PIN_OUT, GPIO_OUT);

    HAL_GPIO_SetPinLevel(GPIO_DCX_GPIO_PORT, GPIO_DCX_GPIO_PIN_OUT, GPIO_LOW);

    return RT_EOK;
}

/**
 * @brief  Close lcd device.
 * @param  dev: rt_device_t for lcd.
 * return rt_err_t.
 */
static rt_err_t spi_screen_close(rt_device_t dev)
{
#if defined(RT_USING_LARGE_HEAP)
    struct spi_screen_state *state = (struct spi_screen_state *)dev->user_data;

    if (state->graphic_info.framebuffer)
        rt_dma_free_large(state->graphic_info.framebuffer);
#endif

    HAL_GPIO_SetPinLevel(GPIO_DCX_GPIO_PORT, GPIO_DCX_GPIO_PIN_OUT, GPIO_LOW);

    return RT_EOK;
}

/**
 * @brief  Close lcd device.
 * @param  dev: rt_device_t for lcd.
 * @param  cmd: control command.
 * @param  args: args between driver and userspace.
 * return rt_err_t.
 */
static rt_err_t spi_screen_control(rt_device_t dev, int cmd, void *args)
{
    struct spi_screen_state *state = (struct spi_screen_state *)dev->user_data;
    rt_err_t ret = RT_EOK;
    struct rt_device_graphic_info *graphic_info;

    rt_mutex_take(&state->display_lock, RT_WAITING_FOREVER);
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
        graphic_info = args;
        state->graphic_info.framebuffer = graphic_info->framebuffer;
        spi_screen_commit(state);
        break;
    case RTGRAPHIC_CTRL_POWERON:
        spi_screen_enable(state);
        break;
    case RTGRAPHIC_CTRL_POWEROFF:
        spi_screen_disable(state);
        break;
    case RTGRAPHIC_CTRL_GET_INFO:
        memcpy(args, &state->graphic_info, sizeof(state->graphic_info));
        break;
    case RTGRAPHIC_CTRL_SET_MODE:
        break;

    default:
        rt_kprintf("Unsupport lcd control cmd: 0x%lx\n", cmd);
        break;
    }
    rt_mutex_release(&state->display_lock);

    return ret;
}

#ifdef RT_USING_DEVICE_OPS
const static struct rt_device_ops spi_screen_ops =
{
    NULL,
    spi_screen_open,
    spi_screen_close,
    NULL,
    NULL,
    spi_screen_control,
};
#endif

int rt_spi_screen_init(void)
{
    struct spi_screen_state *state;

    state = rt_malloc(sizeof(struct spi_screen_state));
    if (!state)
    {
        rt_kprintf("alloc spi_screen_state failed\n");
        return -RT_ENOMEM;
    }
    rt_memset(state, 0, sizeof(struct spi_screen_state));

    /* init rt_device structure */
    state->lcd.type = RT_Device_Class_Graphic;
#ifdef RT_USING_DEVICE_OPS
    state->lcd.ops = &spi_screen_ops;
#else
    state->lcd.init = NULL;
    state->lcd.open = spi_screen_open;
    state->lcd.close = spi_screen_close;
    state->lcd.read = NULL;
    state->lcd.write = NULL;
    state->lcd.control = spi_screen_control;
#endif
    state->lcd.user_data = (void *)state;
    if (rt_mutex_init(&(state->display_lock), "displayLock", RT_IPC_FLAG_FIFO) != RT_EOK)
        RT_ASSERT(0);

    /* register lcd device to RT-Thread */
    rt_device_register(&state->lcd, "spi_screen", RT_DEVICE_FLAG_RDWR);

    return 0;
}

INIT_DEVICE_EXPORT(rt_spi_screen_init);
#endif

/** @} */

/** @} */
