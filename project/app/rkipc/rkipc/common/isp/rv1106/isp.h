int rk_isp_init(int cam_id, char *iqfile_path);
int rk_isp_deinit(int cam_id);
int rk_isp_group_init(int cam_group_id, char *iqfile_path);
int rk_isp_group_deinit(int cam_group_id);
int rk_isp_get_frame_rate(int cam_id, int *value);
int rk_isp_set_frame_rate(int cam_id, int value);
int rk_isp_set_frame_rate_without_ini(int cam_id, int value);
int rk_isp_set_from_ini(int cam_id);
int rk_isp_fastboot_init(int cam_id);
int rk_isp_fastboot_deinit(int cam_id);
int rk_isp_set_group_ldch_level_form_buffer(int cam_id, void *ldch_0, void *ldch_1, int ldch_size_0,
                                            int ldch_size_1);
// isp scenario
int rk_isp_get_scenario(int cam_id, const char **value);
int rk_isp_set_scenario(int cam_id, const char *value);
// image adjustment
int rk_isp_get_contrast(int cam_id, int *value);
int rk_isp_set_contrast(int cam_id, int value);
int rk_isp_get_brightness(int cam_id, int *value);
int rk_isp_set_brightness(int cam_id, int value);
int rk_isp_get_saturation(int cam_id, int *value);
int rk_isp_set_saturation(int cam_id, int value);
int rk_isp_get_sharpness(int cam_id, int *value);
int rk_isp_set_sharpness(int cam_id, int value);
int rk_isp_get_hue(int cam_id, int *value);
int rk_isp_set_hue(int cam_id, int value);
// exposure
int rk_isp_get_exposure_mode(int cam_id, const char **value);
int rk_isp_set_exposure_mode(int cam_id, const char *value);
int rk_isp_get_gain_mode(int cam_id, const char **value);
int rk_isp_set_gain_mode(int cam_id, const char *value);
int rk_isp_get_exposure_time(int cam_id, const char **value);
int rk_isp_set_exposure_time(int cam_id, const char *value);
int rk_isp_get_exposure_gain(int cam_id, int *value);
int rk_isp_set_exposure_gain(int cam_id, int value);
// night_to_day
int rk_isp_get_night_to_day(int cam_id, const char **value);
int rk_isp_set_night_to_day(int cam_id, const char *value);
int rk_isp_get_fill_light_mode(int cam_id, const char **value);
int rk_isp_set_fill_light_mode(int cam_id, const char *value);
int rk_isp_get_light_brightness(int cam_id, int *value);
int rk_isp_set_light_brightness(int cam_id, int value);
int rk_isp_get_night_to_day_filter_level(int cam_id, int *value);
int rk_isp_set_night_to_day_filter_level(int cam_id, int value);
int rk_isp_get_night_to_day_filter_time(int cam_id, int *value);
int rk_isp_set_night_to_day_filter_time(int cam_id, int value);
// blc
int rk_isp_get_hdr(int cam_id, const char **value);
int rk_isp_set_hdr(int cam_id, const char *value);
int rk_isp_get_blc_region(int cam_id, const char **value);
int rk_isp_set_blc_region(int cam_id, const char *value);
int rk_isp_get_hlc(int cam_id, const char **value);
int rk_isp_set_hlc(int cam_id, const char *value);
int rk_isp_get_hdr_level(int cam_id, int *value);
int rk_isp_set_hdr_level(int cam_id, int value);
int rk_isp_get_blc_strength(int cam_id, int *value);
int rk_isp_set_blc_strength(int cam_id, int value);
int rk_isp_get_hlc_level(int cam_id, int *value);
int rk_isp_set_hlc_level(int cam_id, int value);
int rk_isp_get_dark_boost_level(int cam_id, int *value);
int rk_isp_set_dark_boost_level(int cam_id, int value);
// white_blance
int rk_isp_get_white_blance_style(int cam_id, const char **value);
int rk_isp_set_white_blance_style(int cam_id, const char *value);
int rk_isp_get_white_blance_red(int cam_id, int *value);
int rk_isp_set_white_blance_red(int cam_id, int value);
int rk_isp_get_white_blance_green(int cam_id, int *value);
int rk_isp_set_white_blance_green(int cam_id, int value);
int rk_isp_get_white_blance_blue(int cam_id, int *value);
int rk_isp_set_white_blance_blue(int cam_id, int value);
// enhancement
int rk_isp_get_noise_reduce_mode(int cam_id, const char **value);
int rk_isp_set_noise_reduce_mode(int cam_id, const char *value);
int rk_isp_get_dehaze(int cam_id, const char **value);
int rk_isp_set_dehaze(int cam_id, const char *value);
int rk_isp_get_gray_scale_mode(int cam_id, const char **value);
int rk_isp_set_gray_scale_mode(int cam_id, const char *value);
int rk_isp_get_distortion_correction(int cam_id, const char **value);
int rk_isp_set_distortion_correction(int cam_id, const char *value);
int rk_isp_get_spatial_denoise_level(int cam_id, int *value);
int rk_isp_set_spatial_denoise_level(int cam_id, int value);
int rk_isp_get_temporal_denoise_level(int cam_id, int *value);
int rk_isp_set_temporal_denoise_level(int cam_id, int value);
int rk_isp_get_dehaze_level(int cam_id, int *value);
int rk_isp_set_dehaze_level(int cam_id, int value);
int rk_isp_get_fec_level(int cam_id, int *value);
int rk_isp_set_fec_level(int cam_id, int value);
int rk_isp_get_ldch_level(int cam_id, int *value);
int rk_isp_set_ldch_level(int cam_id, int value);
// video_adjustment
int rk_isp_get_power_line_frequency_mode(int cam_id, const char **value);
int rk_isp_set_power_line_frequency_mode(int cam_id, const char *value);
int rk_isp_get_image_flip(int cam_id, const char **value);
int rk_isp_set_image_flip(int cam_id, const char *value);
// auto focus
int rk_isp_get_af_mode(int cam_id, const char **value);
int rk_isp_set_af_mode(int cam_id, const char *value);
int rk_isp_get_zoom_level(int cam_id, int *value);
int rk_isp_get_focus_level(int cam_id, int *value);
int rk_isp_af_zoom_in(int cam_id);
int rk_isp_af_zoom_out(int cam_id);
int rk_isp_af_focus_in(int cam_id);
int rk_isp_af_focus_out(int cam_id);
int rk_isp_af_focus_once(int cam_id);
