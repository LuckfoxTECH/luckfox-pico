#include <rtthread.h>
#if defined(RT_USING_CLOCK_DARK)
#include <stdio.h>
#include <math.h>
#include <dfs_posix.h>

#include "drv_heap.h"
#include "drv_display.h"
#include "image_info.h"
#include "display.h"

/*
 **************************************************************************************************
 *
 * Macro define
 *
 **************************************************************************************************
 */

/* display win layers */
#define CLOCK_TOP_WIN       0
#define CLOCK_BOTTOM_WIN    1

#define CLOCK_MAX_XRES      392
#define CLOCK_MAX_YRES      392

/* display region define */
#define CLOCK_REGION_X      0
#define CLOCK_REGION_Y      0

#define CLOCK_BOTTOM_COLOR_DEPTH    16
#define CLOCK_BOTTOM_FB_W   392 // must aligned_32
#define CLOCK_BOTTOM_FB_H   392 // must aligned_32

#ifdef CLOCK_DARK_QUALITY_LOW
#define CLOCK_TOP_COLOR_DEPTH       8
#else   //CLOCK_DARK_QUALITY_HIGH
#define CLOCK_TOP_COLOR_DEPTH       CLOCK_BOTTOM_COLOR_DEPTH
#endif
#define CLOCK_TOP_FB_W      CLOCK_BOTTOM_FB_W
#define CLOCK_TOP_FB_H      CLOCK_BOTTOM_FB_H

/* Command define */
#define CMD_REFR_CLOCK              (0x01UL << 0)

/* Event define */
#define EVENT_CLOCK_UPDATE          (0x01UL << 0)
#define EVENT_REFR_UPDATE           (0x01UL << 1)
#define EVENT_REFR_DONE             (0x01UL << 2)

#define APPCLK_CLOCK_UPDATE_TICKS   (RT_TICK_PER_SECOND)   //20ms

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
extern image_info_t clock_bkg_info;
extern image_info_t clock_center_info;
extern image_info_t clock_sec_info;
extern image_info_t clock_min_info;
extern image_info_t clock_hour_info;

/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
struct clock_dark_data
{
    rt_display_data_t disp;
    rt_event_t  disp_event;
    rt_timer_t  clock_timer;
    rt_uint32_t cmd;

    rt_uint32_t ticks;
    rt_uint8_t  hour;
    rt_uint8_t  minute;
    rt_uint8_t  second;
    rt_uint8_t  month;
    rt_uint8_t  day;
    rt_uint8_t  week;

    rt_uint8_t *bottom_fb;
    rt_uint32_t bottom_fblen;
    rt_uint8_t *top_fb;
    rt_uint32_t top_fblen;
    rt_uint8_t  fb_index;
};
static  struct clock_dark_data *g_clock_data = RT_NULL;

/*
 **************************************************************************************************
 *
 * Clock_dark functions.
 *
 **************************************************************************************************
 */

/**
 * Display clock demo timer callback.
 */
static void app_clock_dark_timer(void *parameter)
{
    struct clock_dark_data *pdata = (struct clock_dark_data *)parameter;

    if (++pdata->second >= 60)
    {
        pdata->second = 0;
        if (++pdata->minute == 60)
        {
            pdata->minute = 0;
            if (++pdata->hour >= 24)
            {
                pdata->hour = 0;

                if (++pdata->week == 7)
                {
                    pdata->week = 0;
                }

                if (++pdata->day > 31)
                {
                    pdata->day = 1;
                    if (++pdata->month > 12)
                    {
                        pdata->month = 1;
                    }
                }
            }
        }
    }

    pdata->cmd |= CMD_REFR_CLOCK;
    rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);
}

/**
 * get clock background resource from file.
 */
static rt_err_t app_clock_dark_get_bkg(rt_uint16_t *pbuf, rt_uint16_t w, rt_uint16_t h)
{
    image_info_t img_info;
    rt_memset(&img_info, 0, sizeof(image_info_t));
    //img_info.type = IMG_TYPE_RAW;
    //img_info.pixel = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    //img_info.x = 0;
    //img_info.y = 0;
    img_info.w = 390;
    img_info.h = 390;
    RT_ASSERT(img_info.w <= w);
    RT_ASSERT(img_info.h <= h);

    int fd = open("/clock_bkg_390_390.dta", O_RDONLY, 0);
    RT_ASSERT(fd >= 0);
    //img_info.size = lseek(fd, 0, SEEK_END);
    //RT_ASSERT(img_info.size <= w * h * sizeof(rt_uint16_t));

    rt_uint32_t flen;
    rt_uint32_t xoffset = (w - img_info.w) / 2;
    rt_uint32_t yoffset = (h - img_info.h) / 2;
    lseek(fd, 0, SEEK_SET);
    for (rt_uint16_t i = yoffset; i < yoffset + img_info.h; i++)
    {
        flen = read(fd,
                    pbuf + i * w + xoffset,
                    img_info.w * sizeof(rt_uint16_t));

        if (!flen)
        {
            break;
        }
    }

    close(fd);

    return RT_EOK;
}

/**
 * clock dmeo init.
 */
static rt_err_t app_clock_dark_init(struct clock_dark_data *pdata)
{
    rt_uint32_t num = 1;
    /* top framebuffer malloc */
    RT_ASSERT(((CLOCK_TOP_FB_W * CLOCK_TOP_COLOR_DEPTH / 8) & 0x03) == 0);
    pdata->top_fblen = CLOCK_TOP_FB_W * CLOCK_TOP_FB_H * CLOCK_TOP_COLOR_DEPTH / 8;
#ifdef CLOCK_DARK_QUALITY_LOW
    num =  2;    //for top layer pipo buffer
#endif
    pdata->top_fb   = (rt_uint8_t *)rt_malloc_large(pdata->top_fblen * num);
    RT_ASSERT(pdata->top_fb != RT_NULL);
    rt_memset((void *)pdata->top_fb, 0, pdata->top_fblen * num);
    pdata->fb_index = 0;

    /* bottom framebuffer malloc */
    RT_ASSERT(((CLOCK_BOTTOM_FB_W * CLOCK_BOTTOM_COLOR_DEPTH / 8) & 0x03) == 0);
    pdata->bottom_fblen = CLOCK_BOTTOM_FB_W * CLOCK_BOTTOM_FB_H * CLOCK_BOTTOM_COLOR_DEPTH / 8;
    pdata->bottom_fb     = (rt_uint8_t *)rt_malloc_dtcm(pdata->bottom_fblen);
    RT_ASSERT(pdata->bottom_fb != RT_NULL);
    rt_memset((void *)pdata->bottom_fb, 0, pdata->bottom_fblen);

    /* init background data */
    app_clock_dark_get_bkg((rt_uint16_t *)pdata->bottom_fb, CLOCK_BOTTOM_FB_W, CLOCK_BOTTOM_FB_H);

    /* variable init */
    pdata->hour   = 3;
    pdata->minute = 30;
    pdata->second = 0;

    /* init timer */
    pdata->clock_timer = rt_timer_create("appclk_timer",
                                         app_clock_dark_timer, (void *)pdata,
                                         APPCLK_CLOCK_UPDATE_TICKS, RT_TIMER_FLAG_PERIODIC);
    RT_ASSERT(pdata->clock_timer != RT_NULL);
    rt_timer_start(pdata->clock_timer);

    pdata->cmd |= CMD_REFR_CLOCK;
    rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);

    return RT_EOK;
}

/**
 * clock demo deinit.
 */
static void app_clock_dark_deinit(struct clock_dark_data *pdata)
{
    rt_err_t ret;

    rt_timer_stop(pdata->clock_timer);
    ret = rt_timer_delete(pdata->clock_timer);
    RT_ASSERT(ret == RT_EOK);

    rt_free_large((void *)pdata->top_fb);
    rt_free_dtcm((void *)pdata->bottom_fb);
}

/**
 * top layer refresh.
 */
static rt_err_t app_clock_top_refresh(struct clock_dark_data *pdata,
                                      struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &pdata->disp->info;

    int32_t hour, angle;
    image_info_t *img_info = NULL;

#ifdef CLOCK_DARK_QUALITY_LOW
    rt_uint32_t fblen = pdata->top_fblen;
    rt_uint8_t *fb = pdata->top_fb + (pdata->fb_index * pdata->top_fblen);
    pdata->fb_index = 1 - pdata->fb_index;
    rt_memset((void *)fb, 0, pdata->top_fblen);
#else
    rt_uint32_t fblen = pdata->bottom_fblen;
    rt_uint8_t *fb = pdata->bottom_fb;
    if (pdata->fb_index) fb = pdata->top_fb;
    pdata->fb_index = 1 - pdata->fb_index;
    app_clock_dark_get_bkg((rt_uint16_t *)fb, CLOCK_BOTTOM_FB_W, CLOCK_BOTTOM_FB_H);
#endif
    //draw clock
    if (1)
    {
        rt_uint16_t xoffset = (CLOCK_TOP_FB_W / 2);
        rt_uint16_t yoffset = (CLOCK_TOP_FB_H / 2);

        //draw hour line
        img_info = &clock_hour_info;
        RT_ASSERT(img_info->w <= CLOCK_TOP_FB_W / 2);
        RT_ASSERT(img_info->h <= CLOCK_TOP_FB_H / 2);

        hour = pdata->hour;
        if (hour >= 12)
        {
            hour -= 12;
        }
        angle = hour * (360 / 12) + (pdata->minute * 30) / 60 - 90;
        if (angle < 0)
        {
            angle += 360;
        }
#ifdef CLOCK_DARK_QUALITY_LOW
        rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, (unsigned char *)img_info->data,
                               (unsigned char *)((uint32_t)fb + 1 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                               CLOCK_TOP_FB_W, 4, img_info->h / 2);
#else
        rt_display_rotate_16bit((float)angle, img_info->w, img_info->h, (unsigned short *)img_info->data,
                                (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                                CLOCK_TOP_FB_W, 4, img_info->h / 2);
#endif
        //draw min line
        img_info = &clock_min_info;
        RT_ASSERT(img_info->w <= CLOCK_TOP_FB_W / 2);
        RT_ASSERT(img_info->h <= CLOCK_TOP_FB_H / 2);

        angle = pdata->minute * (360 / 60);
        angle -= 90;
        if (angle < 0)
        {
            angle += 360;
        }
#ifdef CLOCK_DARK_QUALITY_LOW
        rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, (unsigned char *)img_info->data,
                               (unsigned char *)((uint32_t)fb + 1 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                               CLOCK_TOP_FB_W, 4, img_info->h / 2);
#else
        rt_display_rotate_16bit((float)angle, img_info->w, img_info->h, (unsigned short *)img_info->data,
                                (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                                CLOCK_TOP_FB_W, 4, img_info->h / 2);
#endif

        //draw second line
        img_info = &clock_sec_info;
        RT_ASSERT(img_info->w <= CLOCK_TOP_FB_W / 2);
        RT_ASSERT(img_info->h <= CLOCK_TOP_FB_H / 2);

        angle = pdata->second * (360 / 60);
        angle -= 90;
        if (angle < 0)
        {
            angle += 360;
        }
#ifdef CLOCK_DARK_QUALITY_LOW
        rt_display_rotate_8bit((float)angle, img_info->w, img_info->h, (unsigned char *)img_info->data,
                               (unsigned char *)((uint32_t)fb + 1 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                               CLOCK_TOP_FB_W, 4, img_info->h / 2);
#else
        rt_display_rotate_16bit((float)angle, img_info->w, img_info->h, (unsigned short *)img_info->data,
                                (unsigned short *)((uint32_t)fb + 2 * (yoffset * CLOCK_TOP_FB_W + xoffset)),
                                CLOCK_TOP_FB_W, 4, img_info->h / 2);
#endif

        //draw centre
        img_info = &clock_center_info;
        RT_ASSERT(img_info->w <= CLOCK_TOP_FB_W);
        RT_ASSERT(img_info->h <= CLOCK_TOP_FB_H);

        yoffset  -= img_info->h / 2;
        xoffset  -= img_info->w / 2;
        rt_display_img_fill(img_info, fb, CLOCK_TOP_FB_W, xoffset, yoffset);
    }

    wincfg->winId  = CLOCK_TOP_WIN;
#ifdef CLOCK_DARK_QUALITY_LOW
    wincfg->colorkey = COLOR_KEY_EN | 0;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB332;
    wincfg->lut    = bpp_lut;
    wincfg->lutsize = sizeof(bpp_lut) / sizeof(bpp_lut[0]);
#else
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut    = RT_NULL;
    wincfg->lutsize = 0;
#endif
    wincfg->fb    = fb;
    wincfg->fblen = fblen;
    wincfg->w     = CLOCK_TOP_FB_W;
    wincfg->h     = CLOCK_TOP_FB_H;
    wincfg->x     = CLOCK_REGION_X + ((info->width  - wincfg->w) / 2);
    wincfg->y     = CLOCK_REGION_Y + ((info->height - wincfg->h) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * CLOCK_TOP_COLOR_DEPTH) % 32) == 0);
    RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}

#ifdef CLOCK_DARK_QUALITY_LOW
/**
 * bottom layer refresh.
 */
static rt_err_t app_clock_bottom_refresh(struct clock_dark_data *pdata,
        struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &pdata->disp->info;

    wincfg->winId  = CLOCK_BOTTOM_WIN;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB565;
    wincfg->lut    = RT_NULL;
    wincfg->lutsize = 0;
    wincfg->w     = CLOCK_BOTTOM_FB_W;
    wincfg->h     = CLOCK_BOTTOM_FB_H;
    wincfg->fb    = pdata->bottom_fb;
    wincfg->fblen = pdata->bottom_fblen;
    wincfg->x     = CLOCK_REGION_X + ((info->width - wincfg->w) / 2);
    wincfg->y     = CLOCK_REGION_Y + ((info->height - wincfg->h) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * CLOCK_BOTTOM_COLOR_DEPTH) % 32) == 0);
    RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    return RT_EOK;
}
#endif

/*
 **************************************************************************************************
 *
 * clock demo init & thread
 *
 **************************************************************************************************
 */

/**
 * app clock display refresh request: request send data to LCD pannel.
 */
static rt_err_t app_clock_refr_done(void)
{
    return (rt_event_send(g_clock_data->disp_event, EVENT_REFR_DONE));
}

static rt_err_t app_clock_refr_request(struct rt_display_mq_t *disp_mq)
{
    rt_err_t ret;

    //request refresh display data to Pannel
    disp_mq->disp_finish = app_clock_refr_done;
    ret = rt_mq_send(g_clock_data->disp->disp_mq, disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(g_clock_data->disp_event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RT_WAITING_FOREVER, &event);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * app clock display task.
 */
static rt_err_t app_clock_refr_tasks(struct clock_dark_data *pdata)
{
    rt_err_t ret;
    struct rt_display_mq_t disp_mq;

    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));
    disp_mq.win[CLOCK_TOP_WIN].zpos = WIN_TOP_LAYER;
    disp_mq.win[CLOCK_BOTTOM_WIN].zpos = WIN_BOTTOM_LAYER;

    if ((pdata->cmd & CMD_REFR_CLOCK) == CMD_REFR_CLOCK)
    {
        pdata->cmd &= ~CMD_REFR_CLOCK;

#ifdef CLOCK_DARK_QUALITY_LOW
        ret = app_clock_bottom_refresh(pdata, &disp_mq.win[CLOCK_BOTTOM_WIN]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << CLOCK_BOTTOM_WIN);
#endif
        ret = app_clock_top_refresh(pdata, &disp_mq.win[CLOCK_TOP_WIN]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << CLOCK_TOP_WIN);
    }

    if (disp_mq.cfgsta)
    {
        app_clock_refr_request(&disp_mq);
    }

    if (pdata->cmd != 0)
    {
        rt_event_send(pdata->disp_event, EVENT_REFR_UPDATE);
    }

    return RT_EOK;
}

/**
 * clock demo thread
 */
static void app_clock_thread(void *p)
{
    rt_err_t ret;
    uint32_t event;
    struct clock_dark_data *pdata;

    g_clock_data = pdata = (struct clock_dark_data *)rt_malloc(sizeof(struct clock_dark_data));
    RT_ASSERT(pdata != RT_NULL);
    rt_memset((void *)pdata, 0, sizeof(struct clock_dark_data));

    pdata->disp = rt_display_get_disp();
    RT_ASSERT(pdata->disp != RT_NULL);

    //check required pannel size
    struct rt_device_graphic_info *info = &pdata->disp->info;
    if ((info->width < CLOCK_MAX_XRES) || (info->height < CLOCK_MAX_YRES))
    {
        rt_kprintf("Error: the pannel size(%dx%d) is less than required size(%dx%d)!\n",
                   info->width, info->height, CLOCK_MAX_XRES, CLOCK_MAX_YRES);
        RT_ASSERT(!(info->width < CLOCK_MAX_XRES));
        RT_ASSERT(!(info->height < CLOCK_MAX_YRES));
    }

    ret = rt_display_screen_clear();
    RT_ASSERT(ret == RT_EOK);

    pdata->disp_event = rt_event_create("display_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(pdata->disp_event != RT_NULL);

    ret = app_clock_dark_init(pdata);
    RT_ASSERT(ret == RT_EOK);

    while (1)
    {
        ret = rt_event_recv(pdata->disp_event, EVENT_REFR_UPDATE,
                            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                            RT_WAITING_FOREVER, &event);
        if (ret != RT_EOK)
        {
            /* Reserved... */
        }

        if (event & EVENT_REFR_UPDATE)
        {
            ret = app_clock_refr_tasks(pdata);
            RT_ASSERT(ret == RT_EOK);
            continue;
        }
    }

    /* Thread deinit */
    app_clock_dark_deinit(pdata);

    rt_event_delete(pdata->disp_event);

    rt_free(pdata);
}

/**
 * clock demo init
 */
int clock_dark_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("clockdark", app_clock_thread, RT_NULL, 2048, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(clock_dark_init);
#endif
