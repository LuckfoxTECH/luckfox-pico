/**
  * Copyright (c) 2022 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @version V0.0.1
  *
  * Change Logs:
  * Date           Author          Notes
  * 2022-07-07     ISP Team      first implementation
  *
  ******************************************************************************
  */

#include <hal_base.h>
#include <rtdevice.h>
#include <rthw.h>
#include <rtthread.h>
#include "config.h"
#include "rkaiq_calib.h"
#include "timer.h"

#include "rk_meta_wakeup_param.h"
#include "rk_meta.h"

#ifdef RT_USING_ISP3

#define TARGET_WIDTH 1920
#define TARGET_HEIGHT 1080

#define ISP_PRECAPTURE_FRAME_NUM 5

#define ISP_RTT_MODE_NORMAL 0
#define ISP_RTT_MODE_MULTI_FRAME 1
#define ISP_RTT_MODE_SINGLE_FRAME 2

#define CMD_AP_SUSPEND      0x12345600
#define CMD_AP_RESUME       0x12345601

#include "stdlib.h"
#include "../drivers/isp3/drv_isp3.h"
#include "fast_ae.h"

#define STREAM_DEBUG                    1

#define STREAM_INFO(...)                \
        do { \
            rk_kprintf("[STREAM]: ");   \
            rk_kprintf(__VA_ARGS__);    \
        }while(0)

#if STREAM_DEBUG
#define STREAM_DBG(...)                \
        do { \
            rk_kprintf("[STREAM]: ");   \
            rk_kprintf(__VA_ARGS__);    \
        }while(0)
#else
#define STREAM_DBG(...)
#endif

#define STREAM_ERR(...)                 \
        do { \
            rk_kprintf("[STREAM]: ");   \
            rk_kprintf(__VA_ARGS__);    \
        }while(0)

#define DEVICE_NAME_SIZE                (12)

#define PTR_RAW_DATA(raw_data, off) (union __packed rkisp_raw_data *)((int)raw_data + off)

enum stream_state
{
    STREAM_RUN,
    STREAM_OK,
    STREAM_NG
};

enum cap_frame_policy
{
    POLICY_EACH,                /* pop every frame to share memory */
    POLICY_DISCARD_INVALID,     /* pop frames that's had been match target AE */
};

struct parameters
{
    char dev_name[DEVICE_NAME_SIZE];
    uint32_t buf_num;
    uint32_t frame_num;
    uint32_t block_num;
    eISP_Bufmemtype buf_type;
    struct rk_camera_pix_format pixformat;
    struct rk_camera_crop_rect crop;
    struct isp_video_requestbuffers reqbuf;
    struct shared_data *share;
    enum rk_camera_hdr_mode hdr_mode;
    enum cap_frame_policy policy;
};

static struct rk_isp_dev *ispdev;
static struct parameters options =
{
    .dev_name   = "isp_0",
    .frame_num  = 10,
    /*
     * The buf_num should reserve 1 frame, work-around the final DQBUF moment,
     * isp had already capture the next data.
     */
    .buf_num    = 5,//ISP_PRECAPTURE_FRAME_NUM + 1,
    .block_num  = 10,
    .buf_type   = ISP_BUF_MEM_TYPE_NORMAL,
    .policy     = POLICY_DISCARD_INVALID,
};

static ret_err_t stream_mode_preview(rk_device *dev, bool is_sample, struct shared_data *share)
{
    struct rk_camera_exp_val exp;
    ret_err_t ret;
    struct isp_video_buf buf;

    buf.listtype = ISP_BUF_LIST_TYPE_MP;
    ret = rk_device_control(dev, RK_DEVICE_CTRL_ISP_DQBUF, &buf);
    if (ret != RET_SYS_EOK)
    {
        STREAM_ERR("dqueue buf fail,stop streaming!\n");
        return ret;
    }

    get_expinfo(buf.frame_id, &exp);
    STREAM_INFO("L %d, %d, %d, 0x%x, %d, 0x%x, 0x%x, tick:%d\n",
                share->frame_num,
                buf.frame_id,
                buf.timestamp,
                buf.addr,
                buf.bufsize,
                exp.reg_time[0],
                exp.reg_gain[0],
                rt_tick_get());
    share->frame_num++;
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)share, sizeof(struct shared_data));

    ret = rk_device_control(dev, RK_DEVICE_CTRL_ISP_QBUF, &buf);
    if (ret != RET_SYS_EOK)
    {
        STREAM_ERR("queue buf[%d] failed on streaming!\n", buf.index);
    }

    return ret;
}

#ifdef RT_USING_CAM_STREAM_ON_LATE
struct firstae_param_s
{
    rk_device *dev;
    rk_device *sensor;
    struct shared_data *share;
    rt_sem_t sem;
};

static void firstae_thread(void *arg)
{
    struct firstae_param_s *firstae_param = (struct firstae_param_s *)arg;
    rk_kprintf("%s %d tick %d\n", __FUNCTION__, __LINE__, rt_tick_get());
    set_firstae(firstae_param->dev, firstae_param->sensor, firstae_param->share, &firstae_param->sem);
}
#endif

static void sec_camera_init_thread(void *arg)
{
    rt_device_t sensor_assist;
    struct rk_camera_mbus_framefmt mbus_fmt;
    struct rk_camera_dst_config cam_config = {0};
    struct config_param *param = (struct config_param *)arg;

    sensor_assist = rt_device_find(RK_ISP_ASSIST_SUBDEV_NAME);
    if (sensor_assist == RT_NULL)
    {
        STREAM_ERR("can't find %s device!\n", RK_ISP_ASSIST_SUBDEV_NAME);
        return ;
    }

    secondary_config(&cam_config);

    // 2) set secondary sensor resolution
    if (param->secondary_sensor.width && param->secondary_sensor.height)
    {
        mbus_fmt.width = param->secondary_sensor.width;
        mbus_fmt.height = param->secondary_sensor.height;

        rt_device_control(sensor_assist,
                          RK_DEVICE_CTRL_CID_MATCH_CAM_CONFIG,
                          &cam_config);
    }
    else
    {
        mbus_fmt.width = TARGET_WIDTH;
        mbus_fmt.height = TARGET_HEIGHT;
    }
    rt_device_control(sensor_assist,
                      RK_DEVICE_CTRL_CAMERA_SET_FORMAT,
                      &mbus_fmt);
}

static ret_err_t start_stream(rk_device *dev, struct config_param *param)
{
    ret_err_t ret;
    uint8_t index;
    struct isp_video_requestbuffers reqbuf;
    struct rk_camera_device *subdev;
    struct shared_data *share = (struct shared_data *)param->isp.share_mem_addr;
    rt_sem_t sem;
#ifdef RT_USING_CAM_STREAM_ON_LATE
    struct firstae_param_s firstae_param = {0};
    rt_thread_t tid = RT_NULL;
#endif

    ret = rk_device_open(dev, RT_DEVICE_OFLAG_RDWR);
    if (ret != RET_SYS_EOK)
    {
        STREAM_ERR("Err: open isp device failded!\n");
        return ret;
    }

    subdev = ((struct rk_isp_dev *)dev->user_data)->subdev;
    options.pixformat.pixelformat = V4L2_PIX_FMT_NV12;
    options.pixformat.width = subdev->info.mbus_fmt.width;
    options.pixformat.height = subdev->info.mbus_fmt.height;

    ret = rk_device_control(dev, RK_DEVICE_CTRL_ISP_SET_FMT, &options.pixformat);
    if (ret != RET_SYS_EOK)
    {
        STREAM_ERR("Err: set isp format failded!\n");
        return ret;
    }

    //options.hdr_mode = subdev->info.hdr_mode;
    reqbuf.count = options.buf_num;

    reqbuf.type = options.buf_type;
    ret = rk_device_control(dev, RK_DEVICE_CTRL_ISP_REQBUF, &reqbuf);
    if (ret != RET_SYS_EOK)
    {
        STREAM_ERR("Err: request isp buf num failded!\n");
        return ret;
    }

    if (reqbuf.count < options.buf_num)
    {
        STREAM_ERR("Warning:allocated buf num is:%d, lower than requested num:%d\n",
                   reqbuf.count, options.buf_num);
    }

    for (index = 0; index < reqbuf.count; index++)
    {
        struct isp_video_buf buf;

        buf.index = index;
        ret = rk_device_control(dev, RK_DEVICE_CTRL_ISP_QBUF, &buf);
        if (ret != RET_SYS_EOK)
        {
            STREAM_ERR("Err: qbuf buf[%d] failded!\n", index);
            return ret;
        }
    }

#ifdef RT_USING_CAM_STREAM_ON_LATE
    sem = rt_sem_create("firstae_sem", 0, RT_IPC_FLAG_FIFO);
    firstae_param.dev = dev;
    firstae_param.sensor = &subdev->parent;
    firstae_param.sem = sem;
    firstae_param.share = share;
    tid = rt_thread_create("firstae_thread", firstae_thread, &firstae_param, 4096, RT_THREAD_PRIORITY_MAX / 2, 10);
    if (tid)
        rt_thread_startup(tid);
    ret = rk_device_control(&subdev->parent, RK_DEVICE_CTRL_CAMERA_STREAM_ON, RK_NULL);
    rk_kprintf("%s %d tick %d\n", __FUNCTION__, __LINE__, rt_tick_get());
    ret = rk_semaphore_take(sem, RK_WAITING_FOREVER);
    rk_kprintf("%s %d tick %d\n", __FUNCTION__, __LINE__, rt_tick_get());
#else
    set_firstae(dev, &subdev->parent, share, &sem);
#endif
    ret = rk_device_control(dev, RK_DEVICE_CTRL_ISP_STREAM_ON, RK_NULL);
    if (ret != RET_SYS_EOK)
    {
        STREAM_ERR("Err: stream on isp failed!\n");
        return ret;
    }
#ifdef RT_USING_CAM_STREAM_ON_LATE
    ret = rk_device_control(&subdev->parent, RK_DEVICE_CTRL_CAMERA_STREAM_ON_LATE, RK_NULL);
#endif
    start_ae(dev, &subdev->parent);
    share->enable = 1;
    share->complete = STREAM_RUN;
    share->width = subdev->info.mbus_fmt.width;
    share->height = subdev->info.mbus_fmt.height;
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)share, sizeof(struct shared_data));
    if (share->camera_num == 2) {
        tid = rt_thread_create("sec_camera_init", sec_camera_init_thread, param, 2048, RT_THREAD_PRIORITY_MAX / 3, 10);
        if (tid)
            rt_thread_startup(tid);
    }
#ifdef RT_USING_CAM_STREAM_ON_LATE
    if (sem)
    {
        rk_semaphore_delete(sem);
        sem = RK_NULL;
    }
#endif
    return RET_SYS_EOK;
}

static void stop_stream(rk_device *dev, struct config_param *param, bool is_normal)
{
    struct shared_data *share = (struct shared_data *)param->isp.share_mem_addr;
    struct rk_isp_dev *rkisp = ispdev;
    bool is_force = true;

    STREAM_DBG("%s start, tick %d\n", __func__, rt_tick_get());

    stop_ae(rkisp);

#ifdef RT_USING_RK_AOV
    // MD need
    is_force = false;
#endif
    rk_device_control(dev, RK_DEVICE_CTRL_ISP_STREAM_OFF, &is_force);

    rk_device_close(dev);

    share->complete = is_normal ? STREAM_OK : STREAM_NG;
    if (share->camera_num == 2) {
        struct shared_data *share_next = share + 1;
        share_next->enable = share->enable;
        share_next->complete = share->complete;
        share_next->frame_num = share->frame_num;
    }
    rt_hw_cpu_dcache_ops(RT_HW_CACHE_FLUSH, (void *)share, sizeof(struct shared_data) * share->camera_num);
    STREAM_DBG("%s end, complete:%d tick %d\n", __func__, share->complete, rt_tick_get());
}

static struct rk_isp_dev *process_find_device(char *name)
{
    char *tmp;
    uint8_t object_id;
    rk_device *dev = RK_NULL;
    struct rk_isp_dev *ispdev = RK_NULL;

    tmp = rk_strstr(name, "_");
    tmp += 1;
    object_id = atoi(tmp);
    STREAM_DBG("the device to be found:%s, object_id:%d\n", name, object_id);
    dev = rk_find_device(name);
    if (dev)
    {
        ispdev = (struct rk_isp_dev *)dev;
        if (rk_strncmp(name, ispdev->name, rk_strlen(ispdev->name)))
        {
            STREAM_ERR("Error: the device is not matched!\n");
            RT_ASSERT(RT_FALSE)
        }
    }
    else
    {
        STREAM_ERR("the device:%s is not found.\n", name);
    }

    return ispdev;

}

extern struct config_param g_param;

static int isp_stream(void)
{
    struct config_param *param = &g_param;//(struct config_param *)CONFIG_ADDRESS;
    struct shared_data *share = (struct shared_data *)param->isp.share_mem_addr;
    static rk_device *dev;
    ret_err_t ret;
    bool is_normal = true;
    uint32_t dst_vts;
    int fastae_max_run_frame = 20;
    int mbox_chn;
    struct rk_camera_mbus_framefmt mbus_fmt;
    struct rk_camera_device *subdev = NULL;
    struct rk_camera_exp_val exp;
    struct MBOX_CMD_DAT tx_msg;
    struct MBOX_REG *pmbox = MBOX;
    struct MBOX_CLIENT mbox_cl =
    {
        .name = "mcu-status",
    };

    STREAM_DBG("share size:%d (%d %d %d)\n",
               sizeof(struct shared_data),
               sizeof(struct isp32_isp_params_cfg),
               sizeof(struct isp32_isp_meas_cfg),
               sizeof(struct isp32_isp_other_cfg));

    options.share = share;
    memset(share, 0, sizeof(*share));
    ispdev = process_find_device(options.dev_name);
    if (!ispdev)
    {
        STREAM_ERR("find device:%s failed!\n", options.dev_name);
        return RT_EIO;
    }

    share->enable = 0;
    share->frame_num = 0;
    share->complete = STREAM_NG;
    dev = &ispdev->parent.parent;
    ret = start_stream(dev, param);
    if (ret != RET_SYS_EOK)
    {
        is_normal = false;
        goto OUT;
    }

#ifndef RT_USING_RK_AOV
    mbox_chn = MBOX_CH_1;
#else
    mbox_chn = MBOX_CH_0;
#endif
    HAL_MBOX_Init(pmbox, false);

    ret = HAL_MBOX_RegisterClient(pmbox, mbox_chn, &mbox_cl);
    RT_ASSERT(ret == HAL_OK);
    pmbox->B2A_INTEN |= BIT(mbox_chn);

    fastae_max_run_frame = fast_ae_get_max_frame();
    do
    {
#ifndef RT_USING_RK_AOV
        //use yuv dqbuf/qbuf
        ret = stream_mode_preview(dev, true, share);
        if (ret != RET_SYS_EOK)
        {
            is_normal = false;
            break;
        }
#else
        uint32_t sleep_ms = 1000 / get_rtt_fps() / 2;
        rt_thread_sleep(sleep_ms);//unit:tick = ms
#endif
        if (isae_match() || isae_over_range())
            break;
#ifndef RT_USING_RK_AOV
        rt_hw_cpu_dcache_ops(RT_HW_CACHE_INVALIDATE, (void *)share, sizeof(struct shared_data));
#endif
    }
    while (share->frame_num < fastae_max_run_frame);
OUT:
    stop_stream(dev, param, is_normal);

#ifndef RT_USING_RK_AOV
    //TODO switch sensor resolution and set exposure
    if (param->sensor.width && param->sensor.height)
    {
        mbus_fmt.width = param->sensor.width;
        mbus_fmt.height = param->sensor.height;
    }
    else
    {
        mbus_fmt.width = TARGET_WIDTH;
        mbus_fmt.height = TARGET_HEIGHT;
    }
    subdev = ((struct rk_isp_dev *)dev->user_data)->subdev;
    rt_device_control(&subdev->parent,
                      RK_DEVICE_CTRL_CAMERA_SET_FORMAT,
                      &mbus_fmt);

    dst_vts = param->sensor.dst_vts;
    rt_device_control(&subdev->parent,
                      RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL,
                      &dst_vts);

    if (share->complete != STREAM_NG)
    {

        if (subdev->info.hdr_mode == NO_HDR)
        {
            exp.reg_time[0] = share->exp_time_reg[0];
            exp.reg_gain[0] = share->exp_gain_reg[0];
            exp.dcg_mode[0] = share->dcg_mode[0];
        }
        else if (subdev->info.hdr_mode == HDR_X2)
        {
            exp.reg_time[0] = share->exp_time_reg[0];
            exp.reg_time[1] = share->exp_time_reg[1];
            exp.reg_gain[0] = share->exp_gain_reg[0];
            exp.reg_gain[1] = share->exp_gain_reg[1];
            exp.dcg_mode[0] = share->dcg_mode[0];
            exp.dcg_mode[1] = share->dcg_mode[1];
        }
        rt_device_control(&subdev->parent,
                          RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL,
                          &exp);
    }

    // SET SECONDARY SENSOR CONFIG
    if (share->camera_num == 2) {
        rt_device_t sensor_assist;
        struct shared_data *share_next_camera = share + 1;

        // 1) find secondary sensor device
        sensor_assist = rt_device_find(RK_ISP_ASSIST_SUBDEV_NAME);
        if (sensor_assist == RT_NULL)
        {
            STREAM_ERR("can't find %s device!\n", RK_ISP_ASSIST_SUBDEV_NAME);
            return RT_ERROR;
        }

        // 3) set secondary sensor vts
        dst_vts = param->secondary_sensor.dst_vts;
        rt_device_control(sensor_assist,
                          RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL,
                          &dst_vts);

        // 4) set secondary sensor exp
        if (share->complete != STREAM_NG)
        {
            if (subdev->info.hdr_mode == NO_HDR)
            {
                exp.reg_time[0] = share_next_camera->exp_time_reg[0];
                exp.reg_gain[0] = share_next_camera->exp_gain_reg[0];
                exp.dcg_mode[0] = share_next_camera->dcg_mode[0];
            }
            else if (subdev->info.hdr_mode == HDR_X2)
            {
                exp.reg_time[0] = share_next_camera->exp_time_reg[0];
                exp.reg_time[1] = share_next_camera->exp_time_reg[1];
                exp.reg_gain[0] = share_next_camera->exp_gain_reg[0];
                exp.reg_gain[1] = share_next_camera->exp_gain_reg[1];
                exp.dcg_mode[0] = share_next_camera->dcg_mode[0];
                exp.dcg_mode[1] = share_next_camera->dcg_mode[1];
            }
            rt_device_control(sensor_assist,
                              RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL,
                              &exp);
            if (ret != RT_EOK)
            {
                STREAM_ERR("set assist sensor exp failed! ret is :%d\n", ret);
                return ret;
            }
        }
    }

    STREAM_INFO("will go to wfi, tick:%d!\n", rt_tick_get());
    tx_msg.CMD = 0x0000f00d;
    tx_msg.DATA = 0xdeadbeef;
    HAL_ICACHE_Invalidate();
    HAL_ICACHE_Disable();
    HAL_DCACHE_CleanInvalidate();
    HAL_DCACHE_Disable();
    ret = HAL_MBOX_SendMsg2(pmbox, mbox_chn, &tx_msg, false);
    rt_hw_interrupt_disable();
    sysTick_stop();
    while (1)
        asm("wfi");
#else
    struct wakeup_param_info* wakeup_param = (struct wakeup_param_info *)(RT_USING_META_DDR_ADRESS + WAKEUP_PARAM_OFFSET);
    wakeup_param->mcu_run_count++;

    struct shared_data *share_data = (struct shared_data *)param->isp.share_mem_addr;
    if(wakeup_param->ae_wakeup_mode == 1)
        share_data->md_flag = 1;
    else if(wakeup_param->ae_wakeup_mode == 2)
        share_data->md_flag = 0;

    if(share_data->md_flag == 1)
        wakeup_param->arm_run_count++;

    if(wakeup_param->mcu_max_run_count != -1 && wakeup_param->mcu_run_count > wakeup_param->mcu_max_run_count) {
        share_data->md_flag = 1;
    }

    uint32_t gpio0_int_status = GPIO0->INT_STATUS;
    if (gpio0_int_status)
        share_data->rtt_mode = ISP_RTT_MODE_MULTI_FRAME;
    else
        share_data->rtt_mode = ISP_RTT_MODE_SINGLE_FRAME;
    STREAM_INFO("gpio0_int_status is %d, share_data->md_flag is %d, tick:%d!\n",
                gpio0_int_status, share_data->md_flag, rt_tick_get());

    if (gpio0_int_status || share_data->md_flag)
    {
        //TODO switch sensor resolution and set exposure
        if (param->sensor.width && param->sensor.height)
        {
            mbus_fmt.width = param->sensor.width;
            mbus_fmt.height = param->sensor.height;
        }
        else
        {
            mbus_fmt.width = TARGET_WIDTH;
            mbus_fmt.height = TARGET_HEIGHT;
        }
        subdev = ((struct rk_isp_dev *)dev->user_data)->subdev;
        rt_device_control(&subdev->parent,
                          RK_DEVICE_CTRL_CAMERA_SET_FORMAT,
                          &mbus_fmt);

        dst_vts = param->sensor.dst_vts;
        rt_device_control(&subdev->parent,
                          RK_DEVICE_CTRL_CAMERA_SET_VTS_VAL,
                          &dst_vts);

        if (share->complete != STREAM_NG)
        {
            if (subdev->info.hdr_mode == NO_HDR)
            {
                exp.reg_time[0] = share->exp_time_reg[0];
                exp.reg_gain[0] = share->exp_gain_reg[0];
                exp.dcg_mode[0] = share->dcg_mode[0];
            }
            else if (subdev->info.hdr_mode == HDR_X2)
            {
                exp.reg_time[0] = share->exp_time_reg[0];
                exp.reg_time[1] = share->exp_time_reg[1];
                exp.reg_gain[0] = share->exp_gain_reg[0];
                exp.reg_gain[1] = share->exp_gain_reg[1];
                exp.dcg_mode[0] = share->dcg_mode[0];
                exp.dcg_mode[1] = share->dcg_mode[1];
            }
            rt_device_control(&subdev->parent,
                              RK_DEVICE_CTRL_CAMERA_SET_EXP_VAL,
                              &exp);
        }
    }
    if (gpio0_int_status || share_data->md_flag)
        tx_msg.CMD = CMD_AP_RESUME;
    else
        tx_msg.CMD = CMD_AP_SUSPEND;
    tx_msg.DATA = 0;
    HAL_ICACHE_Invalidate();
    HAL_ICACHE_Disable();
    HAL_DCACHE_CleanInvalidate();
    HAL_DCACHE_Disable();
    ret = HAL_MBOX_SendMsg2(pmbox, mbox_chn, &tx_msg, false);
    rt_hw_interrupt_disable();
    sysTick_stop();
    while (1)
        asm("wfi");
#endif

    return RT_EOK;
}

INIT_APP_EXPORT(isp_stream);

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(isp_stream, isp stream. e.g: isp_stream);
#endif

#endif

