#ifndef __RKISP_API_CTX__
#define __RKISP_API_CTX__

#include <linux/media-bus-format.h>
#include <linux/videodev2.h>
#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif
enum {
  CAM_TYPE_RKISP1,
  CAM_TYPE_RKCIF,
  CAM_TYPE_USB,
};

/*
 * The context of rkisp capturing device
 *
 * @dev_path:   The file path of video device, eg:
 *                - /dev/video0
 * @fd:         The fd of open(), application could select() on it
 * @width:      The target width of frame resolution
 * @height:     The target height of frame resolution
 * @fcc:        The format in fcc defined in linux/videodev2.h, eg:
 *                - V4L2_PIX_FMT_NV12
 * @uselocal3A: Integrate 3A in this process or not.
 */
struct rkisp_api_ctx {
  int fd;
  char dev_path[256];
  int width;
  int height;
  int fcc;
  int uselocal3A;
};

#define RKISP_EXPO_WEIGHT_GRID 81
#define RKISP_MAX_LUMINANCE_GRID 81
#define RKISP_MAX_HISTOGRAM_BIN 64

/*
 * The linked buffer held frame data.
 * For particular formats(e.g. YUV422M), every frame may contains multi planes.
 *
 * @buf:            The buffer pointer if exist
 * @fd:             The fd of buffer
 * @size:           The size of this buffer/plane
 * @sequence:       The sequence count of this frame from v4l2_buffer
 * @frame_id:       The frame id from librkisp.so
 * @next_plane:     Link to the next plane if this is multi-planes buffer
 *
 * If @uselocal3A is true, some more data as below available:
 *   @expo_time:            The expo time in nano second of this frame
 *   @gain:                 The gain of this frame
 *   @luminance_grid:       The luminance of 9x9(or 5x5) grid of this frame
 *   @luminance_grid_count: The array size of @luminance_grid
 *   @hist_bins:            The histogram array of this frame
 *   @hist_bins_count:      The array size of @hist_bins
 */
struct rkisp_api_buf {
  void *buf;
  int fd;
  int size;
  uint64_t sequence;
  struct {
    int64_t expo_time;
    int gain;
    unsigned char luminance_grid[RKISP_MAX_LUMINANCE_GRID];
    int luminance_grid_count;
    int hist_bins[RKISP_MAX_HISTOGRAM_BIN];
    int hist_bins_count;
    int64_t frame_id;
  } metadata;
  struct timeval timestamp;
  struct rkisp_api_buf *next_plane;
};

#define RKISP_FILE_PATH_LEN 64
#define RKISP_CAMS_NUM_MAX 2
#define RKISP_FLASH_NUM_MAX 2

/* The media topology instance that describes video device and
 * sub-device informations.
 *
 * @vd_main_path, the isp mp device path, e.g. /dev/video0
 * @vd_self_path, the isp sp-device path, e.g. /dev/video1
 * @sd_isp_path, the isp sub-device path, e.g. /dev/v4l-subdev0
 * @vd_params_path, the params video device path
 * @vd_stats_path, the stats video device path
 * @cams, multipe cameras can attache to isp, but only one can be active
 * @sd_sensor_path, the sensor sub-device path
 * @sd_lens_path, the lens sub-device path that attached to sensor
 * @sd_flash_path, the flash sub-device path that attached to sensor,
 *      can be two or more.
 * @link_enabled, the link status of this sensor
 */
struct rkisp_media_info {
  char vd_mb_path[RKISP_FILE_PATH_LEN];
  char vd_s0_path[RKISP_FILE_PATH_LEN];
  char vd_s1_path[RKISP_FILE_PATH_LEN];
  char vd_s2_path[RKISP_FILE_PATH_LEN];
  char sd_isp_path[RKISP_FILE_PATH_LEN];
  char vd_params_path[RKISP_FILE_PATH_LEN];
  char vd_stats_path[RKISP_FILE_PATH_LEN];
  char vd_cif_path[RKISP_FILE_PATH_LEN];

  struct {
    char sd_sensor_path[RKISP_FILE_PATH_LEN];
    char sd_sensor_entity_name[RKISP_FILE_PATH_LEN];
    char sd_lens_path[RKISP_FILE_PATH_LEN];
    char sd_flash_path[RKISP_FLASH_NUM_MAX][RKISP_FILE_PATH_LEN];
    int link_enabled;
  } cams[RKISP_CAMS_NUM_MAX];
};

int rkisp_get_media_info(struct rkisp_media_info *media_info,
                         const char *mdev_path);

/*
 * Open rkisp video device and check device capability. Required.
 *
 * @dev_path:   The video device path, e.g. /dev/video1
 * @uselocal3A: Integrate 3A in this process if true.
 *               - True if app want to use manual AE/GAIN/FPS, or to get
 *                 AE/GAIN metadata of frame
 *               - False if app does not care about AE/GAIN metadata and will
 *                 use auto 3A.
 *
 * If failed return NULL.
 */

const struct rkisp_api_ctx *rkisp_open_device(const char *dev_path,
                                              int uselocal3A);

/*
 * Open rkisp video device and check device capability for ispp. Required.
 *
 * @type:   The video type, e.g. CAM_TYPE_RKISP1 or CAM_TYPE_RKCIF
 *
 * If failed return NULL.
 */
const struct rkisp_api_ctx *rkisp_open_device2(int type);

/* Set crop of frames.  The crop operation is before scaling. Optional.
 * This can be called only when stream off.
 *
 * For example, sensor output 1920x1080, set crop to 1728x1080, and set
 * #{rkisp_set_fmt} to 1280x800, it will be:
 * 1920x1080  crop --> 1728x1080 scale down --> 1280x800
 *
 * @x:  The left offset
 * @y:  The top offset
 * @w:  The target width
 * @h:  The target height
 *
 * Return 0 if success.
 */
int rkisp_video_set_crop(const struct rkisp_priv *priv, int x, int y, int w,
                         int h);
/*
 * Set the frame format, incluing resolution and fourcc. Optional.
 * This can be called only when stream off.
 *
 * If app does set format, the default values are
 *   - width:  640
 *   - height: 480
 *   - fcc:     V4L2_PIX_FMT_NV12
 *
 * Return 0 if success, error num if format is not supported.
 */
int rkisp_set_fmt(const struct rkisp_api_ctx *ctx, int w, int h, int fcc);

/*
 * Set sensor format, incluing resolution and MEDIA_BUS_FMT. Optional.
 * This can be called only when stream off.
 *
 * If sensor switches to a smaller size, this routine will trigger pipeline
 * setting to change isp subdev format size.
 *
 * @mbus_code:   defined in linux/media-bus-format.h
 *
 * Return 0 if success, error num if format is not supported.
 */

int rkisp_set_sensor_fmt(const struct rkisp_api_ctx *ctx, int width, int height,
                         int mbus_code);

/*
 * Set the isp subdev fmt, including the sink and source pads. Optional.
 * This can be called only when stream off.
 *
 * NOTE: This function is not commonly used.
 *
 * It is mainly used in when sensor output size is changed, or
 * when app want capture bayer raw data (e.g. TOF sensor) which need change
 * @out_code.
 *
 * @in_code:  the mbus code of sind pad
 * @out_code: the mbus code of source pad
 *
 * mbus codes defined in linux/media-bus-format.h
 */
int rkisp_set_ispsd_fmt(const struct rkisp_api_ctx *ctx, int in_w, int in_h,
                        int in_code, int out_w, int out_h, int out_code);
/*
 * Request buffers bases on frame format. Optional.
 *
 * @ctx:        The context returned by #{rkisp_open_device()}
 * @buf_count:  The buffer count shall not less than 2. 4 is recommended.
 *              If cpu is heavy loading, a larger buffer may
 *              reduce buffer dropping, but normally 8 is large enough.
 * @dmabuf_fds: If app uses extenal dmabuf, set dmabuf fd array to it.
 *                - If dmabuf_fds is NULL, buffers are allocated from isp driver
 *                  and exported to user
 *                - If dmabuf_fds is not NULL, buffers are allocated from app
 * and
 *                  import to isp driver
 *              @dmabuf_fds array size should be @buf_count
 * @dmabuf_size:The buffer size of dmabuf if @dmabuf_fds is not NULL
 *
 * Return 0 if success, error num if fail
 */
int rkisp_set_buf(const struct rkisp_api_ctx *ctx, int buf_count,
                  const int dmabuf_fds[], int dmabuf_size);

/*
 * Start streaming.
 * This setup buffers first and then starting stream.
 *
 * @ctx:        The context returned by #{rkisp_open_device()}
 *
 * Return 0 if success, error num if fail
 */
int rkisp_start_capture(const struct rkisp_api_ctx *ctx);

/*
 * Get a frame from rkisp. It will block until there's a frame or timeout.
 * App shall return back frame buffer by #{rkisp_put_frame}
 *
 * @ctx:        The context returned by #{rkisp_open_device()}
 * @timeout_ms:    Wait at most @timeout_ms if larger than 0.
 *                 Note: It uses select() to wait for frame data. In case
 *                       app wants to poll()/select() ctx->fd on its own,
 *                       @timeout_ms shall be 0 then.
 *
 * Return linked buffer list that contains one or more planes,
 * or NULL if something wrong.
 */
const struct rkisp_api_buf *rkisp_get_frame(const struct rkisp_api_ctx *ctx,
                                            int timeout_ms);

/*
 * Return back a frame buffer.
 *
 * @ctx:        The context returned by #{rkisp_open_device()}
 * @buf:        The linked buffer list obtained from #{rkisp_get_frame()}
 *
 */
void rkisp_put_frame(const struct rkisp_api_ctx *ctx,
                     const struct rkisp_api_buf *buf);

/*
 * Stop capture.
 * After capture stopped, #{rkisp_get_frame()} behavior is undefined.
 *
 * @ctx:        The context returned by #{rkisp_open_device()}
 */
void rkisp_stop_capture(const struct rkisp_api_ctx *ctx);

/*
 * Close device and uninit.
 * Please make sure all buffers from #{rkisp_get_frame()} are returned back
 * by #{rkisp_put_frame()} before closing device.
 *
 * @ctx:        The context returned by #{rkisp_open_device()}
 */
void rkisp_close_device(const struct rkisp_api_ctx *ctx);

int rkisp_set_manual_expo(const struct rkisp_api_ctx *ctx, int on);
int rkisp_update_expo(const struct rkisp_api_ctx *ctx, int gain,
                      int64_t expo_time_ns);

int rkisp_set_max_expotime(const struct rkisp_api_ctx *ctx,
                           int64_t max_expo_time_ns);
int rkisp_get_max_expotime(const struct rkisp_api_ctx *ctx,
                           int64_t *max_expo_time_ns);
int rkisp_set_max_gain(const struct rkisp_api_ctx *ctx, int max_gain);
int rkisp_get_max_gain(const struct rkisp_api_ctx *ctx, int *max_gain);

/*
 * The get/set_expo_weights() will get or set exposure weights
 * of RKISP_EXPO_WEIGHT_GRID(that is 9x9) grid.
 *
 * @weights:        The array of weight, size shall be at least 81(9x9).
 * @size:           The size of array.
 *
 * Return 0 if success, or < 0 if error.
 */
int rkisp_set_expo_weights(const struct rkisp_api_ctx *ctx,
                           unsigned char *weights, unsigned int size);
int rkisp_get_expo_weights(const struct rkisp_api_ctx *ctx,
                           unsigned char *weights, unsigned int size);
int rkisp_set_fps_range(const struct rkisp_api_ctx *ctx, int max_fps);

#ifdef __cplusplus
}
#endif

#endif
