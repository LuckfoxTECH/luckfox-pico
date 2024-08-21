#include <rtthread.h>
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
#define ANIMATION_RGB888_WIN  1

#define ANI_MAX_XRES        320
#define ANI_MAX_YRES        385

/* display region define */
#define ANI_REGION_X        0
#define ANI_REGION_Y        0
#define ANI_FB_W            320         /* aligned 2 (32bit) */
#define ANI_FB_H            385         /* aligned 2 */
#define ANI_FB_PIXELS       24

/* Command define */
#define CMD_REFR_ANI        (0x01UL << 0)

/* Event define */
#define EVENT_REFR_UPDATE   (0x01UL << 0)
#define EVENT_REFR_DONE     (0x01UL << 1)

#define ANI_UPDATE_TICKS    ((RT_TICK_PER_SECOND / 1000) * 33)

/*
 **************************************************************************************************
 *
 * Declaration
 *
 **************************************************************************************************
 */
#define ANI_RES_FILE_NUM        6
/*
 **************************************************************************************************
 *
 * Global static struct & data define
 *
 **************************************************************************************************
 */
struct global_ani_data
{
    rt_display_data_t disp;
    rt_event_t  disp_event;
    rt_uint32_t cmd;

    rt_timer_t  ani_timer;
    rt_uint8_t *fb;
    rt_uint32_t fblen;
    rt_uint32_t res_id;
};
static  struct global_ani_data *g_ani_data = RT_NULL;

/*
 **************************************************************************************************
 *
 * Animation display
 *
 **************************************************************************************************
 */

/**
 * Animation timer callback.
 */
static void app_animation_timer(void *parameter)
{
    struct global_ani_data *ani_data = (struct global_ani_data *)parameter;

    ani_data->cmd |= CMD_REFR_ANI;
    rt_event_send(ani_data->disp_event, EVENT_REFR_UPDATE);
}

/**
 * Animation dmeo init.
 */
static rt_err_t app_animation_init(struct global_ani_data *ani_data)
{
    ani_data->fblen = ANI_FB_W * ANI_FB_H * (ANI_FB_PIXELS / 8);
    ani_data->fb     = (rt_uint8_t *)rt_malloc_psram(ani_data->fblen);
    RT_ASSERT(ani_data->fb != RT_NULL);
    rt_memset((void *)ani_data->fb, 0, ani_data->fblen);

    ani_data->ani_timer = rt_timer_create("appAniTimer",
                                          app_animation_timer, (void *)ani_data,
                                          ANI_UPDATE_TICKS, RT_TIMER_FLAG_PERIODIC);
    RT_ASSERT(ani_data->ani_timer != RT_NULL);
    rt_timer_start(ani_data->ani_timer);

    ani_data->cmd |= CMD_REFR_ANI;
    rt_event_send(ani_data->disp_event, EVENT_REFR_UPDATE);

    return RT_EOK;
}

/**
 * Animation demo deinit.
 */
static void app_animation_deinit(struct global_ani_data *ani_data)
{
    rt_err_t ret;

    rt_timer_stop(ani_data->ani_timer);
    ret = rt_timer_delete(ani_data->ani_timer);
    RT_ASSERT(ret == RT_EOK);

    rt_free_psram((void *)ani_data->fb);
}

/**
 * Animation refresh.
 */
static rt_err_t app_animation_refresh(struct global_ani_data *ani_data,
                                      struct rt_display_config *wincfg)
{
    struct rt_device_graphic_info *info = &ani_data->disp->info;
    char fname[256];

    wincfg->winId = ANIMATION_RGB888_WIN;
    wincfg->format = RTGRAPHIC_PIXEL_FORMAT_RGB888;
    wincfg->lut   = RT_NULL;
    wincfg->lutsize  = 0;
    wincfg->fb    = ani_data->fb;
    wincfg->fblen = ani_data->fblen;
    wincfg->w     = ANI_FB_W;
    wincfg->h     = ANI_FB_H;
    wincfg->x     = ANI_REGION_X + ((info->width  - wincfg->w) / 2);
    wincfg->y     = ANI_REGION_Y + ((info->height - wincfg->h) / 2);
    wincfg->ylast = wincfg->y;

    RT_ASSERT(((wincfg->w * ANI_FB_PIXELS) % 32) == 0);
    //RT_ASSERT(((wincfg->y % 2) == 0) && ((wincfg->h % 2) == 0));

    RT_ASSERT((wincfg->x + wincfg->w) <= info->width);
    RT_ASSERT((wincfg->y + wincfg->h) <= info->height);

    RT_ASSERT(wincfg->fblen <= ani_data->fblen);

    if (++ani_data->res_id >= ANI_RES_FILE_NUM)
    {
        ani_data->res_id = 0;
    }

    // read data from file
    rt_memset(fname, 0, sizeof(fname));
    snprintf(fname, sizeof(fname), "/oximetry_%03d.dta", (int)ani_data->res_id);
    //rt_kprintf("app_animation_refresh: %s\n", fname);

    int fd = open(fname, O_RDONLY, 0);
    if (fd < 0)
    {
        rt_kprintf("file open error --> %s\n", fname);
    }
    else
    {
        rt_uint32_t len = read(fd, wincfg->fb, wincfg->fblen);
        if (len != wincfg->fblen)
        {
            rt_kprintf("file read error --> %s, req = %d, read = %d\n", fname, wincfg->fblen, len);
        }

        close(fd);
    }

    return RT_EOK;
}

/*
 **************************************************************************************************
 *
 * Animation demo init & thread
 *
 **************************************************************************************************
 */

/**
 * display refresh request: request send data to LCD pannel.
 */
static rt_err_t app_animation_refr_done(void)
{
    return (rt_event_send(g_ani_data->disp_event, EVENT_REFR_DONE));
}

static rt_err_t app_animation_refr_request(struct rt_display_mq_t *disp_mq)
{
    rt_err_t ret;

    //request refresh display data to Pannel
    disp_mq->disp_finish = app_animation_refr_done;
    ret = rt_mq_send(g_ani_data->disp->disp_mq, disp_mq, sizeof(struct rt_display_mq_t));
    RT_ASSERT(ret == RT_EOK);

    //wait refresh done
    rt_uint32_t event;
    ret = rt_event_recv(g_ani_data->disp_event, EVENT_REFR_DONE,
                        RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                        RT_WAITING_FOREVER, &event);
    RT_ASSERT(ret == RT_EOK);

    return RT_EOK;
}

/**
 * Animation display task.
 */
static rt_err_t app_animation_refr_tasks(struct global_ani_data *ani_data)
{
    rt_err_t ret;
    struct rt_display_mq_t disp_mq;

    rt_memset(&disp_mq, 0, sizeof(struct rt_display_mq_t));
    disp_mq.win[0].zpos = WIN_TOP_LAYER;

    if ((ani_data->cmd & CMD_REFR_ANI) == CMD_REFR_ANI)
    {
        ani_data->cmd &= ~CMD_REFR_ANI;

        ret = app_animation_refresh(ani_data, &disp_mq.win[0]);
        RT_ASSERT(ret == RT_EOK);
        disp_mq.cfgsta |= (0x01 << 0);
    }

    if (disp_mq.cfgsta)
    {
        app_animation_refr_request(&disp_mq);
    }

    if (ani_data->cmd != 0)
    {
        //rt_event_send(ani_data->disp_event, EVENT_REFR_UPDATE);
    }

    return RT_EOK;
}

/**
 * Animation demo thread
 */
static void app_animation_thread(void *p)
{
    rt_err_t ret;
    uint32_t event;
    struct global_ani_data *ani_data;

    g_ani_data = ani_data = (struct global_ani_data *)rt_malloc(sizeof(struct global_ani_data));
    RT_ASSERT(ani_data != RT_NULL);
    rt_memset((void *)ani_data, 0, sizeof(struct global_ani_data));

    ani_data->disp = rt_display_get_disp();
    RT_ASSERT(ani_data->disp != RT_NULL);

    //check required pannel size
    struct rt_device_graphic_info *info = &ani_data->disp->info;
    if ((info->width < ANI_MAX_XRES) || (info->height < ANI_MAX_YRES))
    {
        rt_kprintf("Error: the pannel size(%dx%d) is less than required size(%dx%d)!\n",
                   info->width, info->height, ANI_MAX_XRES, ANI_MAX_YRES);
        RT_ASSERT(!(info->width < ANI_MAX_XRES));
        RT_ASSERT(!(info->height < ANI_MAX_YRES));
    }

    ret = rt_display_screen_clear();
    RT_ASSERT(ret == RT_EOK);

    ani_data->disp_event = rt_event_create("display_event", RT_IPC_FLAG_FIFO);
    RT_ASSERT(ani_data->disp_event != RT_NULL);

    ret = app_animation_init(ani_data);
    RT_ASSERT(ret == RT_EOK);

    while (1)
    {
        ret = rt_event_recv(ani_data->disp_event, EVENT_REFR_UPDATE,
                            RT_EVENT_FLAG_OR | RT_EVENT_FLAG_CLEAR,
                            RT_WAITING_FOREVER, &event);
        if (ret != RT_EOK)
        {
            /* Reserved... */
        }

        if (event & EVENT_REFR_UPDATE)
        {
            ret = app_animation_refr_tasks(ani_data);
            RT_ASSERT(ret == RT_EOK);
        }
    }

    /* Thread deinit */
    app_animation_deinit(ani_data);

    rt_event_delete(ani_data->disp_event);

    rt_free(ani_data);
}

/**
 * Animation demo init
 */
int app_animation_thread_init(void)
{
    rt_thread_t thread;

    thread = rt_thread_create("appAnimation", app_animation_thread, RT_NULL, 2048, 5, 10);
    RT_ASSERT(thread != RT_NULL);

    rt_thread_startup(thread);

    return RT_EOK;
}

INIT_APP_EXPORT(app_animation_thread_init);
