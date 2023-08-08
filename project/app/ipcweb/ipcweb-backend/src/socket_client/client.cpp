#include "api_handler.h"
#include "socket.h"
#include <string.h>

int rk_client_get_int_by_id(char *func, int id, int *value) {
  int fd;
  int ret = 0;

  fd = cli_begin(func);
  /* Transmission parameters */
  sock_write(fd, &id, sizeof(int));
  sock_read(fd, value, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_client_set_int_by_id(char *func, int id, int value) {
  int fd;
  int ret = 0;

  fd = cli_begin(func);
  /* Transmission parameters */
  sock_write(fd, &id, sizeof(int));
  sock_write(fd, &value, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_client_get_int(char *func, int *value) {
  int fd;
  int ret = 0;

  fd = cli_begin(func);
  /* Transmission parameters */
  sock_read(fd, value, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_client_set_int(char *func, int value) {
  int fd;
  int ret = 0;

  fd = cli_begin(func);
  /* Transmission parameters */
  sock_write(fd, &value, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_client_get_string_by_id(char *func, int id, char **value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin(func);
  /* Transmission parameters */
  sock_write(fd, &id, sizeof(id));
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, *value, len);
  memset(*value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_client_set_string_by_id(char *func, int id, const char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin(func);
  /* Transmission parameters */
  sock_write(fd, &id, sizeof(id));
  len = strlen(value) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, value, len);
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_client_get_string(char *func, char **value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin(func);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, *value, len);
  memset(*value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_client_set_string(char *func, const char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin(func);
  /* Transmission parameters */
  len = strlen(value) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, value, len);
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_isp_set(const char *json) {
  return rk_client_set_string((char *)__func__, json);
}

int rk_isp_get_scenario(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_scenario(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_contrast(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_contrast(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_brightness(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_brightness(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_saturation(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_saturation(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_sharpness(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_sharpness(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_hue(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_hue(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

// isp exposure
int rk_isp_get_exposure_mode(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_exposure_mode(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_gain_mode(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_gain_mode(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_exposure_time(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_exposure_time(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_exposure_gain(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_exposure_gain(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_frame_rate(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_frame_rate(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

// night_to_day
int rk_isp_get_night_to_day(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_night_to_day(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_fill_light_mode(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_fill_light_mode(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_light_brightness(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_light_brightness(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_night_to_day_filter_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_night_to_day_filter_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_night_to_day_filter_time(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_night_to_day_filter_time(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

// blc
int rk_isp_get_hdr(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_hdr(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_blc_region(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_blc_region(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_hlc(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_hlc(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_hdr_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_hdr_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_blc_strength(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_blc_strength(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_hlc_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_hlc_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_dark_boost_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_dark_boost_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

// white_blance
int rk_isp_get_white_blance_style(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_white_blance_style(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_white_blance_red(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_white_blance_red(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_white_blance_green(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_white_blance_green(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_white_blance_blue(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_white_blance_blue(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

// enhancement
int rk_isp_get_noise_reduce_mode(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_noise_reduce_mode(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_dehaze(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_dehaze(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_gray_scale_mode(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_gray_scale_mode(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_distortion_correction(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_distortion_correction(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_spatial_denoise_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_spatial_denoise_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_temporal_denoise_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_temporal_denoise_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_dehaze_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_dehaze_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_fec_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_fec_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_ldch_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_ldch_level(int cam_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, cam_id, value);
}

// video_adjustment
int rk_isp_get_power_line_frequency_mode(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_power_line_frequency_mode(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_image_flip(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_image_flip(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

// auto focus
int rk_isp_get_af_mode(int cam_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_set_af_mode(int cam_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_zoom_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_get_focus_level(int cam_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, cam_id, value);
}

int rk_isp_af_zoom_in(int cam_id) {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_write(fd, &cam_id, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_isp_af_zoom_out(int cam_id) {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_write(fd, &cam_id, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_isp_af_focus_in(int cam_id) {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_write(fd, &cam_id, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_isp_af_focus_out(int cam_id) {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_write(fd, &cam_id, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_isp_af_focus_once(int cam_id) {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_write(fd, &cam_id, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

// video
int rk_video_set(const char *json) {
  return rk_client_set_string((char *)__func__, json);
}

int rk_video_restart() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_video_get_gop(int stream_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_gop(int stream_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_max_rate(int stream_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_max_rate(int stream_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_RC_mode(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_RC_mode(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_output_data_type(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_output_data_type(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_rc_quality(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_rc_quality(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_smart(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_smart(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_gop_mode(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_gop_mode(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_stream_type(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_stream_type(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_h264_profile(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_h264_profile(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_resolution(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_resolution(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_frame_rate(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_frame_rate(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_frame_rate_in(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_set_frame_rate_in(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

int rk_video_get_rotation(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_video_set_rotation(int value) {
  return rk_client_set_int((char *)__func__, value);
}

// jpeg
int rk_video_get_enable_cycle_snapshot(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_video_set_enable_cycle_snapshot(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_video_get_image_quality(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_video_set_image_quality(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_video_get_snapshot_interval_ms(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_video_set_snapshot_interval_ms(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_video_get_jpeg_resolution(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_video_set_jpeg_resolution(const char *value) {
  return rk_client_set_string((char *)__func__, value);
}

// audio
int rk_audio_set(const char *json) {
  return rk_client_set_string((char *)__func__, json);
}

int rk_audio_restart() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_audio_get_bit_rate(int stream_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, stream_id, value);
}

int rk_audio_set_bit_rate(int stream_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, stream_id, value);
}

int rk_audio_get_sample_rate(int stream_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, stream_id, value);
}

int rk_audio_set_sample_rate(int stream_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, stream_id, value);
}

int rk_audio_get_volume(int stream_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, stream_id, value);
}

int rk_audio_set_volume(int stream_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, stream_id, value);
}

int rk_audio_get_enable_vqe(int stream_id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, stream_id, value);
}

int rk_audio_set_enable_vqe(int stream_id, int value) {
  return rk_client_set_int_by_id((char *)__func__, stream_id, value);
}

int rk_audio_get_encode_type(int stream_id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, stream_id, value);
}

int rk_audio_set_encode_type(int stream_id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, stream_id, value);
}

// system

int rk_system_capability_get_video(char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  if (len != 0)
    sock_read(fd, value, len);
  memset(value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_capability_get_image_adjustment(char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  if (len != 0)
    sock_read(fd, value, len);
  memset(value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_capability_get_image_blc(char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  if (len != 0)
    sock_read(fd, value, len);
  memset(value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_capability_get_image_enhancement(char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  if (len != 0)
    sock_read(fd, value, len);
  memset(value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_capability_get_image_exposure(char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  if (len != 0)
    sock_read(fd, value, len);
  memset(value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_capability_get_image_night_to_day(char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  if (len != 0)
    sock_read(fd, value, len);
  memset(value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_capability_get_image_video_adjustment(char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  if (len != 0)
    sock_read(fd, value, len);
  memset(value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_capability_get_image_white_blance(char *value) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &len, sizeof(int));
  if (len != 0)
    sock_read(fd, value, len);
  memset(value + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_get_deivce_name(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_telecontrol_id(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_model(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_serial_number(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_firmware_version(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_encoder_version(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_web_version(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_plugin_version(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_channels_number(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_hard_disks_number(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_alarm_inputs_number(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_alarm_outputs_number(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_firmware_version_info(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_manufacturer(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_get_hardware_id(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_system_set_deivce_name(const char *value) {
  return rk_client_set_string((char *)__func__, value);
}

int rk_system_set_telecontrol_id(const char *value) {
  return rk_client_set_string((char *)__func__, value);
}

// action

int rk_system_reboot() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_factory_reset() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_export_log(const char *path) {
  return rk_client_set_string((char *)__func__, path);
}

int rk_system_export_db(const char *path) {
  return rk_client_set_string((char *)__func__, path);
}

int rk_system_import_db(const char *path) {
  return rk_client_set_string((char *)__func__, path);
}

int rk_system_upgrade(const char *path) {
  return rk_client_set_string((char *)__func__, path);
}

// user
int rk_system_get_user_num(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_system_set_user_num(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_system_get_user_level(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_system_set_user_level(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_system_get_user_name(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_system_set_user_name(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_system_get_password(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_system_set_password(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_system_add_user(int id, int user_level, const char *user_name,
                       const char *password) {
  int fd, user_name_len, password_len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_write(fd, &id, sizeof(int));
  sock_write(fd, &user_level, sizeof(int));
  user_name_len = strlen(user_name) + 1;
  sock_write(fd, &user_name_len, sizeof(int));
  sock_write(fd, user_name, user_name_len);
  password_len = strlen(password) + 1;
  sock_write(fd, &password_len, sizeof(int));
  sock_write(fd, password, password_len);
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_system_del_user(int id) {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_write(fd, &id, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

// osd
// osd.common
int rk_osd_get_is_presistent_text(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_osd_set_is_presistent_text(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_osd_get_font_size(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_osd_set_font_size(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_osd_get_boundary(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_osd_set_boundary(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_osd_get_normalized_screen_width(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_osd_get_normalized_screen_height(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_osd_get_attribute(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_osd_set_attribute(const char *value) {
  return rk_client_set_string((char *)__func__, value);
}

int rk_osd_get_font_color_mode(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_osd_set_font_color_mode(const char *value) {
  return rk_client_set_string((char *)__func__, value);
}

int rk_osd_get_font_color(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_osd_set_font_color(const char *value) {
  return rk_client_set_string((char *)__func__, value);
}

int rk_osd_get_alignment(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_osd_set_alignment(const char *value) {
  return rk_client_set_string((char *)__func__, value);
}

int rk_osd_get_font_path(char **value) {
  return rk_client_get_string((char *)__func__, value);
}

int rk_osd_set_font_path(const char *value) {
  return rk_client_set_string((char *)__func__, value);
}

// osd.x
int rk_osd_get_enabled(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_osd_set_enabled(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_osd_get_position_x(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_osd_set_position_x(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_osd_get_position_y(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_osd_set_position_y(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_osd_get_height(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_osd_set_height(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_osd_get_width(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_osd_set_width(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_osd_get_display_week_enabled(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_osd_set_display_week_enabled(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_osd_get_date_style(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_osd_set_date_style(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_osd_get_time_style(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_osd_set_time_style(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_osd_get_type(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_osd_set_type(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_osd_get_display_text(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_osd_set_display_text(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_osd_get_image_path(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_osd_set_image_path(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_osd_restart() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

// roi.x
int rk_roi_get_stream_type(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_roi_set_stream_type(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_roi_get_name(int id, char **value) {
  return rk_client_get_string_by_id((char *)__func__, id, value);
}

int rk_roi_set_name(int id, const char *value) {
  return rk_client_set_string_by_id((char *)__func__, id, value);
}

int rk_roi_get_id(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_roi_set_id(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_roi_get_enabled(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_roi_set_enabled(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_roi_get_position_x(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_roi_set_position_x(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_roi_get_position_y(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_roi_set_position_y(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_roi_get_height(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_roi_set_height(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_roi_get_width(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_roi_set_width(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_roi_get_quality_level(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_roi_set_quality_level(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_roi_set_all() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

// region_clip.x
int rk_region_clip_get_enabled(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_set_enabled(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_get_position_x(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_set_position_x(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_get_position_y(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_set_position_y(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_get_height(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_set_height(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_get_width(int id, int *value) {
  return rk_client_get_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_set_width(int id, int value) {
  return rk_client_set_int_by_id((char *)__func__, id, value);
}

int rk_region_clip_set_all() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

// network
int rk_network_ipv4_get(const char *interface, char *method, char *address,
                        char *netmask, char *gateway) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(interface) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, interface, len);
  // read
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, method, len);
  memset(method + len, '\0', 1); // set terminator
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, address, len);
  memset(address + len, '\0', 1); // set terminator
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, netmask, len);
  memset(netmask + len, '\0', 1); // set terminator
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, gateway, len);
  memset(gateway + len, '\0', 1); // set terminator

  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_network_ipv4_set(char *interface, char *method, char *address,
                        char *netmask, char *gateway) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(interface) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, interface, len);
  if (method) {
    len = strlen(method) + 1;
    sock_write(fd, &len, sizeof(int));
    sock_write(fd, method, len);
  } else {
    len = 0;
    sock_write(fd, &len, sizeof(int));
  }
  if (address) {
    len = strlen(address) + 1;
    sock_write(fd, &len, sizeof(int));
    sock_write(fd, address, len);
  } else {
    len = 0;
    sock_write(fd, &len, sizeof(int));
  }
  if (netmask) {
    len = strlen(netmask) + 1;
    sock_write(fd, &len, sizeof(int));
    sock_write(fd, netmask, len);
  } else {
    len = 0;
    sock_write(fd, &len, sizeof(int));
  }
  if (gateway) {
    len = strlen(gateway) + 1;
    sock_write(fd, &len, sizeof(int));
    sock_write(fd, gateway, len);
  } else {
    len = 0;
    sock_write(fd, &len, sizeof(int));
  }
  // read
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_network_dns_get(char *ethernet_v4_dns, char *ethernet_v4_dns2) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // read
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, ethernet_v4_dns, len);
  memset(ethernet_v4_dns + len, '\0', 1); // set terminator
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, ethernet_v4_dns2, len);
  memset(ethernet_v4_dns2 + len, '\0', 1); // set terminator

  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_network_dns_set(char *dns1, char *dns2) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(dns1) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, dns1, len);
  len = strlen(dns2) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, dns2, len);

  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_network_get_mac(const char *ifname, char *mac) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(ifname) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, ifname, len);
  // read
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, mac, len);
  memset(mac + len, '\0', 1); // set terminator

  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_network_nicspeed_get(const char *ifname, int *speed, int *duplex,
                            int *autoneg) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(ifname) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, ifname, len);
  // read
  sock_read(fd, speed, sizeof(int));
  sock_read(fd, duplex, sizeof(int));
  sock_read(fd, autoneg, sizeof(int));

  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_network_nicspeed_set(const char *ifname, int speed, int duplex,
                            int autoneg) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(ifname) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, ifname, len);
  sock_write(fd, &speed, sizeof(int));
  sock_write(fd, &duplex, sizeof(int));
  sock_write(fd, &autoneg, sizeof(int));
  // read
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_network_nicspeed_support_get(const char *ifname, char *nic_supported) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(ifname) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, ifname, len);
  // read
  sock_read(fd, &len, sizeof(int));
  sock_read(fd, nic_supported, len);
  memset(nic_supported + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_wifi_power_get(int *on) {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // read
  sock_read(fd, on, sizeof(int));
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_wifi_power_set(int on) {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  sock_write(fd, &on, sizeof(int));
  // read
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_wifi_scan_wifi() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_wifi_get_list(char **wifi_list) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // read
  sock_read(fd, &len, sizeof(int));
  *wifi_list = (char *)malloc(len + 1);
  sock_read(fd, *wifi_list, len);
  memset(*wifi_list + len, '\0', 1); // set terminator
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_wifi_connect_with_ssid(const char *ssid, const char *psk) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(ssid) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, ssid, len);
  len = strlen(psk) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, psk, len);
  // read
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}
int rk_wifi_forget_with_ssid(const char *ssid) {
  int fd, len;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  // write
  len = strlen(ssid) + 1;
  sock_write(fd, &len, sizeof(int));
  sock_write(fd, ssid, len);
  // read
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

// storage
int rk_storage_record_start() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_storage_record_stop() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

int rk_storage_record_statue_get(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_take_photo() {
  int fd;
  int ret = 0;

  fd = cli_begin((char *)__func__);
  /* Transmission parameters */
  sock_read(fd, &ret, sizeof(int));
  /* End transmission parameters */
  ret = cli_end(fd);

  return ret;
}

// event
int rk_event_ri_get_enabled(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_event_ri_set_enabled(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_event_ri_get_position_x(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_event_ri_set_position_x(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_event_ri_get_position_y(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_event_ri_set_position_y(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_event_ri_get_width(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_event_ri_set_width(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_event_ri_get_height(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_event_ri_set_height(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_event_ri_get_proportion(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_event_ri_set_proportion(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_event_ri_get_sensitivity_level(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_event_ri_set_sensitivity_level(int value) {
  return rk_client_set_int((char *)__func__, value);
}

int rk_event_ri_get_time_threshold(int *value) {
  return rk_client_get_int((char *)__func__, value);
}

int rk_event_ri_set_time_threshold(int value) {
  return rk_client_set_int((char *)__func__, value);
}
