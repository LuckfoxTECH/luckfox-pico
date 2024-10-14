#include <rtthread.h>
#include <stdio.h>
#include <math.h>
#include <dfs_posix.h>

#include "drv_heap.h"
#include "drv_display.h"
#include "image_info.h"
#include "display.h"

#include <littlevgl2rtt.h>
#include <lvgl/lvgl.h>

#if defined(RT_USING_TOUCH_DRIVERS)
#include "touch.h"
#include "touchpanel.h"
#endif

#include "app_main.h"

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */

/* display region define */
#define ALPHA_REGION_X      0
#define ALPHA_REGION_Y      0

#define ALPHA_WIN_XRES      DISP_XRES
#define ALPHA_WIN_YRES      DISP_YRES
#define ALPHA_WIN_FB_W      ALPHA_WIN_XRES
#define ALPHA_WIN_FB_H      ALPHA_WIN_YRES
#define ALPHA_WIN_COLOR_DEPTH       32

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */

static struct rt_work init_work;

struct g_alpha_win_data_t *g_alpha_win_data = RT_NULL;
struct refrsh_param_t    alpha_win_refrsh_param;

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
/**
 * top layer refresh.
 */
rt_err_t alpha_win_refresh(struct rt_display_config *wincfg, void *param)
{
    struct rt_device_graphic_info *info = &app_main_data->disp->info;
    struct g_alpha_win_data_t *pdata = g_alpha_win_data;
    struct refrsh_param_t *par = (struct refrsh_param_t *)param;
    int offset = pdata->mov_xoffset > 0 ? (pdata->mov_xoffset > ALPHA_WIN_XRES ? ALPHA_WIN_XRES : pdata->mov_xoffset) : 0 ;

    wincfg->winId   = par->win_id;
    wincfg->zpos    = par->win_layer;
    wincfg->alphaEn = 1;
    wincfg->alphaMode = 1;
    wincfg->format  = RTGRAPHIC_PIXEL_FORMAT_ARGB888;
    wincfg->lut     = RT_NULL;
    wincfg->lutsize = 0;
    wincfg->fb    = pdata->fb    + offset * sizeof(rt_uint32_t);
    wincfg->fblen = pdata->fblen - offset * sizeof(rt_uint32_t);;
    wincfg->xVir  = ALPHA_WIN_FB_W;
    wincfg->w     = ALPHA_WIN_XRES - offset;
    wincfg->h     = ALPHA_WIN_YRES;
    wincfg->x     = ALPHA_REGION_X + ((info->width  - ALPHA_WIN_XRES) / 2);
    wincfg->y     = ALPHA_REGION_Y + ((info->height - ALPHA_WIN_YRES) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * ALPHA_WIN_COLOR_DEPTH) % 32) == 0);

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */

/**
 * draw needle.
 */
static void app_alpha_win_init_work(struct rt_work *work, void *work_data)
{
    struct g_alpha_win_data_t *pdata;
    int fd;

    g_alpha_win_data = pdata = (struct g_alpha_win_data_t *)rt_malloc(sizeof(struct g_alpha_win_data_t));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct g_alpha_win_data_t));

    /* framebuffer malloc */
    pdata->fblen = ALPHA_WIN_FB_W * ALPHA_WIN_FB_H * ALPHA_WIN_COLOR_DEPTH / 8;
    pdata->fb   = (rt_uint8_t *)rt_malloc_large(pdata->fblen);
    RT_ASSERT(pdata->fb != RT_NULL);

    fd = open("img_alpha.dta", O_RDONLY, 0);
    read(fd, pdata->fb, pdata->fblen);
    close(fd);
}

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
/**
 * App clock fast init.
 */
void app_alpha_win_init(void)
{
    rt_work_init(&init_work, app_alpha_win_init_work, RT_NULL);
    rt_workqueue_dowork(app_main_data->workqueue, &init_work);
}
