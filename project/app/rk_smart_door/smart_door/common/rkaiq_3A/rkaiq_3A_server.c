#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> /* low-level i/o */
#include <inttypes.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

#include <linux/videodev2.h>
#include "mediactl/mediactl.h"
#include "uAPI2/rk_aiq_user_api2_sysctl.h"
#include "smartIr/rk_smart_ir_api.h"
#include "smartIr_server.h"

#define MAP_MASK (sysconf(_SC_PAGE_SIZE) - 1)
#define MAP_SIZE_COLOR_MODE (sizeof(int32_t))

#define CLEAR(x) memset(&(x), 0, sizeof(x))
#define DBG(...) do { if(!silent) printf("DBG: " __VA_ARGS__);} while(0)
#define ERR(...) do { fprintf(stderr, "ERR: " __VA_ARGS__); } while (0)


/* Private v4l2 event */
#define CIFISP_V4L2_EVENT_STREAM_START  \
                    (V4L2_EVENT_PRIVATE_START + 1)
#define CIFISP_V4L2_EVENT_STREAM_STOP   \
                    (V4L2_EVENT_PRIVATE_START + 2)

#define RKAIQ_FILE_PATH_LEN                       64
#define RKAIQ_FLASH_NUM_MAX                       2

/* 1 vicap + 2 mipi + 1 bridge + 1 redundance */
#define MAX_MEDIA_NODES                           5

#define IQ_PATH "/etc/iqfiles/"

static int silent = 1;

// TODO: Configurable width and height
static int width = 1920;
static int height = 1080;
static int g_prd_type;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct rkaiq_media_info {
    char sd_isp_path[RKAIQ_FILE_PATH_LEN];
    char vd_params_path[RKAIQ_FILE_PATH_LEN];
    char vd_stats_path[RKAIQ_FILE_PATH_LEN];
    char mainpath[RKAIQ_FILE_PATH_LEN];
    char sensor_entity_name[32];

    char mdev_path[32];
    int available;
    rk_aiq_sys_ctx_t* aiq_ctx;

    int mem_fd;
    int file_size;
    void *iq_mem;
    int rk_night_mode;
    int rk_color_mode;

    pthread_t pid;
};

static struct rkaiq_media_info media_infos;

static void errno_exit(const char *s)
{
    ERR("%s error %d, %s\n", s, errno, strerror(errno));
    exit(EXIT_FAILURE);
}

static int xioctl(int fh, int request, void *arg)
{
    int r;

    do {
        r = ioctl(fh, request, arg);
    } while (-1 == r && EINTR == errno);

    return r;
}

static int read_cmdline_to_buf(void *buf, int len)
{
    int fd;
    int ret;
    if(buf == NULL || len < 0){
        printf("%s: illegal para\n", __func__);
        return -1;
    }
    memset(buf, 0, len);
    fd = open("/proc/cmdline", O_RDONLY);
    if(fd < 0){
        perror("open:");
        return -1;
    }
    ret = read(fd, buf, len);
    close(fd);
    return ret;
}

static long get_cmd_val(const char *string, int len) {
    char *addr;
    long value = 0;
    char key_equal[16];
    static char cmdline[1024];
    static char cmd_init = 0;

    if (cmd_init == 0) {
        cmd_init = 1;
        memset(cmdline, 0, sizeof(cmdline));
        read_cmdline_to_buf(cmdline, sizeof(cmdline));
    }

    snprintf(key_equal, sizeof(key_equal), "%s=", string);
    addr = strstr(cmdline, string);
    if (addr) {
        value = strtol(addr + strlen(string) + 1, NULL, len);
        printf("get %s value: 0x%0lx\n", string, value);
    }
    return value;
}

static int rkaiq_get_devname(struct media_device *device, const char *name, char *dev_name)
{
    const char *devname;
    struct media_entity *entity =  NULL;

    entity = rk_media_get_entity_by_name(device, name, strlen(name));
    if (!entity)
        return -1;

    devname = rk_media_entity_get_devname(entity);

    if (!devname) {
        ERR("can't find %s device path!", name);
        return -1;
    }

    strncpy(dev_name, devname, RKAIQ_FILE_PATH_LEN);

    DBG("get %s devname: %s\n", name, dev_name);

    return 0;
}

int rkaiq_get_media_info(struct rkaiq_media_info *media_info)
{
    struct media_device *device = NULL;
    const char *sensor_name;
    int ret;

    device = rk_media_device_new (media_info->mdev_path);
    if (!device)
        return -ENOMEM;
    /* Enumerate entities, pads and links. */
    ret = rk_media_device_enumerate (device);
    if (ret)
        return ret;
    if (!ret) {
        /* Try rkisp */
        ret = rkaiq_get_devname(device, "rkisp-isp-subdev",
                                media_info->sd_isp_path);
        ret |= rkaiq_get_devname(device, "rkisp-input-params",
                                media_info->vd_params_path);
        ret |= rkaiq_get_devname(device, "rkisp-statistics",
                                media_info->vd_stats_path);
        ret |= rkaiq_get_devname(device, "rkisp_mainpath",
                                media_info->mainpath);
    }
    if (ret) {
        DBG("Could not find rkisp dev names, skipped %s\n", media_info->mdev_path);
        rk_media_device_unref (device);
        return ret;
    }

    sensor_name = rk_aiq_uapi2_sysctl_getBindedSnsEntNmByVd(media_info->mainpath);
    if (sensor_name == NULL || strlen(sensor_name) == 0) {
        ERR("No sensor attached to %s\n", media_info->mdev_path);
        rk_media_device_unref (device);
        return -EINVAL;
    }

    strcpy(media_info->sensor_entity_name, sensor_name);

    rk_media_device_unref (device);

    return ret;
}

static void init_engine(struct rkaiq_media_info *media_info)
{
    int index;
    off_t addr_iq, rk_color_mode_addr;
    rk_aiq_tb_info_t tb_info;
    size_t file_size;
    int fd = -1, ret = 0;
    int rk_color_mode, rk_night_mode;
    void *mem = NULL, *vir_addr = NULL, *iq_mem = NULL, *vir_iqaddr = NULL;

    media_info->mem_fd = -1;
    media_info->iq_mem = NULL;

    memset(&tb_info, 0x00, sizeof(tb_info));

    tb_info.magic      = sizeof(rk_aiq_tb_info_t) - 2;
    tb_info.is_pre_aiq = false;
    tb_info.prd_type   = g_prd_type;

    if((fd = open ("/dev/mem", O_RDWR | O_SYNC)) < 0) {
        perror ("open error");
        goto error;
    }

    rk_aiq_uapi2_sysctl_preInit_tb_info(media_info->sensor_entity_name, &tb_info);

    addr_iq = (off_t)get_cmd_val("rk_iqbin_addr", 16);
    file_size = (size_t)get_cmd_val("rk_iqbin_size", 16);
    iq_mem = mmap (0 , file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr_iq & ~MAP_MASK);
    vir_iqaddr = iq_mem + (addr_iq & MAP_MASK);

    int cam_hdr = (int)get_cmd_val("rk_cam_hdr", 0);
    rk_aiq_working_mode_t hdr_mode =
        (cam_hdr == 5) ? RK_AIQ_WORKING_MODE_ISP_HDR2 : RK_AIQ_WORKING_MODE_NORMAL;

    rk_night_mode = (int)get_cmd_val("rk_night_mode", 0);
    rk_color_mode_addr = (off_t)get_cmd_val("rk_color_mode", 16);
    mem = mmap(0, MAP_SIZE_COLOR_MODE, PROT_READ | PROT_WRITE, MAP_SHARED, fd,
        rk_color_mode_addr & ~(MAP_MASK));
    vir_addr = mem + (rk_color_mode_addr & (MAP_MASK));
    rk_color_mode = *((unsigned long *)vir_addr);
    if (mem != MAP_FAILED)
        munmap(mem, MAP_SIZE_COLOR_MODE);

    if ((rk_night_mode == 2 && rk_color_mode) || rk_night_mode == 4) {
        DBG("=====night mode=====\n");
        ret = rk_aiq_uapi2_sysctl_preInit_scene(media_info->sensor_entity_name, "normal", "night");
        if (ret < 0) {
            ERR("%s: failed to set night scene\n", media_info->sensor_entity_name);
            goto error;
        }
    } else {
        DBG("=====day mode=======\n");
        ret = rk_aiq_uapi2_sysctl_preInit_scene(media_info->sensor_entity_name, "normal", "day");
        if (ret < 0) {
            ERR("%s: failed to set day scene\n", media_info->sensor_entity_name);
            goto error;
        }
    }

    ret = rk_aiq_uapi2_sysctl_preInit_iq_addr(media_info->sensor_entity_name, vir_iqaddr, file_size);
    if (ret < 0) {
        ERR("%s: failed to load binary iqfiles\n", media_info->sensor_entity_name);
        goto error;
    }

    media_info->aiq_ctx = rk_aiq_uapi2_sysctl_init(media_info->sensor_entity_name,
                                                  IQ_PATH, NULL, NULL);

    if (rk_aiq_uapi2_sysctl_prepare(media_info->aiq_ctx, width, height, hdr_mode)) {
        ERR("rkaiq engine prepare failed !\n");
        goto error;
    }

    media_info->mem_fd = fd;
    media_info->iq_mem = iq_mem;
    media_info->file_size = file_size;
    media_info->rk_night_mode = rk_night_mode;
    media_info->rk_color_mode = rk_color_mode;

    return;

error:
    if (fd >= 0)
        close(fd);

    if (iq_mem)
        munmap(iq_mem, file_size);

    exit(-1);
    return;
}

static void start_engine(struct rkaiq_media_info *media_info)
{
    DBG("device manager start\n");
    rk_aiq_uapi2_sysctl_start(media_info->aiq_ctx);
    if (media_info->aiq_ctx == NULL) {
        ERR("rkisp_init engine failed\n");
        exit(-1);
    } else {
        DBG("rkisp_init engine succeed\n");
    }
}

static void stop_engine(struct rkaiq_media_info *media_info)
{
    rk_aiq_uapi2_sysctl_stop(media_info->aiq_ctx, false);
}

static void deinit_engine(struct rkaiq_media_info *media_info)
{
    rk_aiq_uapi2_sysctl_deinit(media_info->aiq_ctx);

    if (media_info->mem_fd >= 0)
        close(media_info->mem_fd);

    if (media_info->iq_mem)
        munmap(media_info->iq_mem, media_info->file_size);
}

// blocked func
static int wait_stream_event(int fd, unsigned int event_type, int time_out_ms)
{
    int ret;
    struct v4l2_event event;

    CLEAR(event);

    do {
        /*
         * xioctl instead of poll.
         * Since poll() cannot wait for input before stream on,
         * it will return an error directly. So, use ioctl to
         * dequeue event and block until sucess.
         */
        ret = xioctl(fd, VIDIOC_DQEVENT, &event);
        if (ret == 0 && event.type == event_type)
            return 0;
    } while (true);

    return -1;

}

static int subscrible_stream_event(struct rkaiq_media_info *media_info, int fd, bool subs)
{
    struct v4l2_event_subscription sub;
    int ret = 0;

    CLEAR(sub);
    sub.type = CIFISP_V4L2_EVENT_STREAM_START;
    ret = xioctl(fd,
                 subs ? VIDIOC_SUBSCRIBE_EVENT : VIDIOC_UNSUBSCRIBE_EVENT,
                 &sub);
    if (ret) {
        ERR("can't subscribe %s start event!\n", media_info->vd_params_path);
        exit(EXIT_FAILURE);
    }

    CLEAR(sub);
    sub.type = CIFISP_V4L2_EVENT_STREAM_STOP;
    ret = xioctl(fd,
                 subs ? VIDIOC_SUBSCRIBE_EVENT : VIDIOC_UNSUBSCRIBE_EVENT,
                 &sub);
    if (ret) {
        ERR("can't subscribe %s stop event!\n", media_info->vd_params_path);
    }

    DBG("subscribe events from %s success !\n", media_info->vd_params_path);

    return 0;
}

void *engine_thread(void *arg)
{
    int ret = 0;
    int isp_fd;
    unsigned int stream_event = -1;
    struct rkaiq_media_info *media_info;

    pthread_detach(pthread_self());

    media_info = (struct rkaiq_media_info *) arg;

    isp_fd = open(media_info->vd_params_path, O_RDWR);
    if (isp_fd < 0) {
        ERR("open %s failed %s\n", media_info->vd_params_path, strerror(errno));
        return NULL;
    }

    init_engine(media_info);
    subscrible_stream_event(media_info, isp_fd, true);

    pthread_mutex_lock(&mutex);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    fprintf(stderr, "start rkaiq_3A_server for sensor : %s\n", media_info->sensor_entity_name);

    for (;;) {
        start_engine(media_info);

        /* NIGHT_IR_AUTO mode */
        if (media_info->rk_night_mode == 2) {
            int init_ir_status = media_info->rk_color_mode > 0 ?
                                 RK_SMART_IR_STATUS_NIGHT : RK_SMART_IR_STATUS_DAY;
            smart_ir_server_start(media_info->aiq_ctx, init_ir_status);
        }

        DBG("%s: wait stream start event...\n", media_info->mdev_path);
        wait_stream_event(isp_fd, CIFISP_V4L2_EVENT_STREAM_START, -1);
        DBG("%s: wait stream start event success ...\n", media_info->mdev_path);

        DBG("%s: wait stream stop event...\n", media_info->mdev_path);
        wait_stream_event(isp_fd, CIFISP_V4L2_EVENT_STREAM_STOP, -1);
        DBG("%s: wait stream stop event success ...\n", media_info->mdev_path);

        if (media_info->rk_night_mode == 2)
            smart_ir_server_stop(media_info->aiq_ctx);

        stop_engine(media_info);
    }

    subscrible_stream_event(media_info, isp_fd, false);
    deinit_engine(media_info);
    close(isp_fd);

    return NULL;
}

void *rkaiq_3a(char *sensor_name, bool is_multi_cam)
{
    int ret, i;

    /* Line buffered so that printf can flash every line if redirected to
     * no-interactive device.
     */
    setlinebuf(stdout);

    g_prd_type = is_multi_cam ? RK_AIQ_PRD_TYPE_TB_DOORLOCK : RK_AIQ_PRD_TYPE_TB_BATIPC;

    for (i = 0; i < MAX_MEDIA_NODES; i++) {
        sprintf(media_infos.mdev_path, "/dev/media%d", i);
        if (rkaiq_get_media_info(&media_infos)) {
            media_infos.available = 0;
            continue;
        }

        // only matches sensor_name
        if(strstr(media_infos.sensor_entity_name, sensor_name)) {
            media_infos.available = 1;
            DBG("sensor name: %s\n", media_infos.sensor_entity_name);
            break;
        } else {
            media_infos.available = 0;
            continue;
        }
    }

    if (!media_infos.available) {
        ERR("sensor name %s not be found.\n", sensor_name);
        return NULL;
    }

    ret = pthread_create(&media_infos.pid, NULL, engine_thread, &media_infos);
    if (ret) {
        media_infos.pid = 0;
        ERR("Failed to create camera engine thread for: %s\n", media_infos.mdev_path);
        errno_exit("Create thread failed");
    }

    // waiting for rk_aiq_uapi2_sysctl_prepare done, then we can start video capture
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond,&mutex);
    pthread_mutex_unlock(&mutex);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return media_infos.aiq_ctx;
}
