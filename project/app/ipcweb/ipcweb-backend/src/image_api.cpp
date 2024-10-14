// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "image_api.h"
#include "common.h"

#ifdef USE_RKIPC
#include "socket_client/client.h"
#endif

namespace rockchip {
namespace cgi {

#ifdef USE_RKIPC

nlohmann::json image_specific_resource_get(std::string string, int cam_id) {
  nlohmann::json specific_resource =
      nlohmann::json::object(); /* one of all the same resources */
  minilog_debug("%s: string is %s\n", __func__, string.c_str());
  int value_int;
  char *tmp = new char[256];

  if (!string.compare(PATH_IMAGE_ADJUSTMENT)) {
    rk_isp_get_brightness(cam_id, &value_int);
    specific_resource.emplace("iBrightness", value_int);
    rk_isp_get_contrast(cam_id, &value_int);
    specific_resource.emplace("iContrast", value_int);
    rk_isp_get_saturation(cam_id, &value_int);
    specific_resource.emplace("iSaturation", value_int);
    rk_isp_get_sharpness(cam_id, &value_int);
    specific_resource.emplace("iSharpness", value_int);
    rk_isp_get_hue(cam_id, &value_int);
    specific_resource.emplace("iHue", value_int);
  } else if (!string.compare(PATH_IMAGE_EXPOSURE)) {
    rk_isp_get_exposure_mode(cam_id, &tmp);
    specific_resource.emplace("sExposureMode", tmp);
    rk_isp_get_gain_mode(cam_id, &tmp);
    specific_resource.emplace("sGainMode", tmp);
    rk_isp_get_exposure_time(cam_id, &tmp);
    specific_resource.emplace("sExposureTime", tmp);
    rk_isp_get_exposure_gain(cam_id, &value_int);
    specific_resource.emplace("iExposureGain", value_int);
    rk_isp_get_frame_rate(cam_id, &value_int);
    specific_resource.emplace("iFPS", value_int);
  } else if (!string.compare(PATH_IMAGE_NIGHT_TO_DAY)) {
    rk_isp_get_night_to_day(cam_id, &tmp);
    specific_resource.emplace("sNightToDay", tmp);
    rk_isp_get_fill_light_mode(cam_id, &tmp);
    specific_resource.emplace("sFillLightMode", tmp);
    rk_isp_get_light_brightness(cam_id, &value_int);
    specific_resource.emplace("iLightBrightness", value_int);
    rk_isp_get_night_to_day_filter_level(cam_id, &value_int);
    specific_resource.emplace("iNightToDayFilterLevel", value_int);
    rk_isp_get_night_to_day_filter_time(cam_id, &value_int);
    specific_resource.emplace("iNightToDayFilterTime", value_int);
  } else if (!string.compare(PATH_IMAGE_BLC)) {
    rk_isp_get_hdr(cam_id, &tmp);
    specific_resource.emplace("sHDR", tmp);
    rk_isp_get_blc_region(cam_id, &tmp);
    specific_resource.emplace("sBLCRegion", tmp);
    rk_isp_get_hlc(cam_id, &tmp);
    specific_resource.emplace("sHLC", tmp);
    rk_isp_get_hdr_level(cam_id, &value_int);
    specific_resource.emplace("iHDRLevel", value_int);
    rk_isp_get_blc_strength(cam_id, &value_int);
    specific_resource.emplace("iBLCStrength", value_int);
    rk_isp_get_hlc_level(cam_id, &value_int);
    specific_resource.emplace("iHLCLevel", value_int);
    rk_isp_get_dark_boost_level(cam_id, &value_int);
    specific_resource.emplace("iDarkBoostLevel", value_int);
  } else if (!string.compare(PATH_IMAGE_WHITE_BLANCE)) {
    rk_isp_get_white_blance_style(cam_id, &tmp);
    specific_resource.emplace("sWhiteBlanceStyle", tmp);
    rk_isp_get_white_blance_red(cam_id, &value_int);
    specific_resource.emplace("iWhiteBalanceRed", value_int);
    rk_isp_get_white_blance_green(cam_id, &value_int);
    specific_resource.emplace("iWhiteBalanceGreen", value_int);
    rk_isp_get_white_blance_blue(cam_id, &value_int);
    specific_resource.emplace("iWhiteBalanceBlue", value_int);
  } else if (!string.compare(PATH_IMAGE_ENHANCEMENT)) {
    rk_isp_get_noise_reduce_mode(cam_id, &tmp);
    specific_resource.emplace("sNoiseReduceMode", tmp);
    rk_isp_get_dehaze(cam_id, &tmp);
    specific_resource.emplace("sDehaze", tmp);
    rk_isp_get_gray_scale_mode(cam_id, &tmp);
    specific_resource.emplace("sGrayScaleMode", tmp);
    rk_isp_get_distortion_correction(cam_id, &tmp);
    specific_resource.emplace("sDistortionCorrection", tmp);
    rk_isp_get_spatial_denoise_level(cam_id, &value_int);
    specific_resource.emplace("iSpatialDenoiseLevel", value_int);
    rk_isp_get_temporal_denoise_level(cam_id, &value_int);
    specific_resource.emplace("iTemporalDenoiseLevel", value_int);
    rk_isp_get_dehaze_level(cam_id, &value_int);
    specific_resource.emplace("iDehazeLevel", value_int);
    rk_isp_get_fec_level(cam_id, &value_int);
    specific_resource.emplace("iFecLevel", value_int);
    rk_isp_get_ldch_level(cam_id, &value_int);
    specific_resource.emplace("iLdchLevel", value_int);
  } else if (!string.compare(PATH_IMAGE_VIDEO_ADJUSTMEN)) {
    rk_isp_get_power_line_frequency_mode(cam_id, &tmp);
    specific_resource.emplace("sPowerLineFrequencyMode", tmp);
    rk_isp_get_image_flip(cam_id, &tmp);
    specific_resource.emplace("sImageFlip", tmp);
    rk_video_get_rotation(&value_int);
    specific_resource.emplace("iImageRotation", value_int);
  } else if (!string.compare(PATH_IMAGE_AF)) {
    rk_isp_get_af_mode(cam_id, &tmp);
    specific_resource.emplace("sAFMode", tmp);
    rk_isp_get_zoom_level(cam_id, &value_int);
    specific_resource.emplace("iZoomLevel", value_int);
    rk_isp_get_focus_level(cam_id, &value_int);
    specific_resource.emplace("iFocusLevel", value_int);
  } else if (!string.compare(PATH_IMAGE_SCENARIO)) {
    rk_isp_get_scenario(cam_id, &tmp);
    specific_resource.emplace("sScenario", tmp);
  }
  delete[] tmp;

  return specific_resource;
}

nlohmann::json image_channel_resource_get(int cam_id) {
  nlohmann::json resource = nlohmann::json::object();

  resource.emplace("id", cam_id);
  resource.emplace(KEY_IMAGE_ADJUSTMENT,
                   image_specific_resource_get(PATH_IMAGE_ADJUSTMENT, cam_id));
  resource.emplace(KEY_IMAGE_EXPOSURE,
                   image_specific_resource_get(PATH_IMAGE_EXPOSURE, cam_id));
  resource.emplace(
      KEY_IMAGE_NIGHT_TO_DAY,
      image_specific_resource_get(PATH_IMAGE_NIGHT_TO_DAY, cam_id));
  resource.emplace(KEY_IMAGE_BLC,
                   image_specific_resource_get(PATH_IMAGE_BLC, cam_id));
  resource.emplace(
      KEY_IMAGE_WHITE_BLANCE,
      image_specific_resource_get(PATH_IMAGE_WHITE_BLANCE, cam_id));
  resource.emplace(KEY_IMAGE_ENHANCEMENT,
                   image_specific_resource_get(PATH_IMAGE_ENHANCEMENT, cam_id));
  resource.emplace(
      KEY_IMAGE_VIDEO_ADJUSTMEN,
      image_specific_resource_get(PATH_IMAGE_VIDEO_ADJUSTMEN, cam_id));

  return resource;
}

void image_specific_resource_set(std::string string, nlohmann::json data,
                                 int cam_id) {
  char *table;
  int value_int;
  std::string value;

  minilog_debug("%s: string is %s\n", __func__, string.c_str());
  minilog_debug("data is %s\n", data.dump().c_str());
  if (!string.compare(PATH_IMAGE_ADJUSTMENT)) {
    if (data.dump().find("iBrightness") != data.dump().npos) {
      value_int = atoi(data.at("iBrightness").dump().c_str());
      rk_isp_set_brightness(cam_id, value_int);
    }
    if (data.dump().find("iContrast") != data.dump().npos) {
      value_int = atoi(data.at("iContrast").dump().c_str());
      rk_isp_set_contrast(cam_id, value_int);
    }
    if (data.dump().find("iSaturation") != data.dump().npos) {
      value_int = atoi(data.at("iSaturation").dump().c_str());
      rk_isp_set_saturation(cam_id, value_int);
    }
    if (data.dump().find("iSharpness") != data.dump().npos) {
      value_int = atoi(data.at("iSharpness").dump().c_str());
      rk_isp_set_sharpness(cam_id, value_int);
    }
    if (data.dump().find("iHue") != data.dump().npos) {
      value_int = atoi(data.at("iHue").dump().c_str());
      rk_isp_set_hue(cam_id, value_int);
    }
  } else if (!string.compare(PATH_IMAGE_EXPOSURE)) {
    if (data.dump().find("iExposureGain") != data.dump().npos) {
      value_int = atoi(data.at("iExposureGain").dump().c_str());
      rk_isp_set_exposure_gain(cam_id, value_int);
    }
    if (data.dump().find("sExposureMode") != data.dump().npos) {
      value = data.at("sExposureMode").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_exposure_mode(cam_id, value.c_str());
    }
    if (data.dump().find("sExposureTime") != data.dump().npos) {
      value = data.at("sExposureTime").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_exposure_time(cam_id, value.c_str());
    }
    if (data.dump().find("sGainMode") != data.dump().npos) {
      value = data.at("sGainMode").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_gain_mode(cam_id, value.c_str());
    }
    if (data.dump().find("iFPS") != data.dump().npos) {
      value_int = atoi(data.at("iFPS").dump().c_str());
      rk_isp_set_frame_rate(cam_id, value_int);
    }
  } else if (!string.compare(PATH_IMAGE_NIGHT_TO_DAY)) {
    if (data.dump().find("sNightToDay") != data.dump().npos) {
      value = data.at("sNightToDay").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_night_to_day(cam_id, value.c_str());
    }
    if (data.dump().find("sFillLightMode") != data.dump().npos) {
      value = data.at("sFillLightMode").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_fill_light_mode(cam_id, value.c_str());
    }
    if (data.dump().find("iLightBrightness") != data.dump().npos) {
      value_int = atoi(data.at("iLightBrightness").dump().c_str());
      rk_isp_set_light_brightness(cam_id, value_int);
    }
    if (data.dump().find("iNightToDayFilterLevel") != data.dump().npos) {
      value_int = atoi(data.at("iNightToDayFilterLevel").dump().c_str());
      rk_isp_set_night_to_day_filter_level(cam_id, value_int);
    }
    if (data.dump().find("iNightToDayFilterTime") != data.dump().npos) {
      value_int = atoi(data.at("iNightToDayFilterTime").dump().c_str());
      rk_isp_set_night_to_day_filter_time(cam_id, value_int);
    }
  } else if (!string.compare(PATH_IMAGE_BLC)) {
    if (data.dump().find("sBLCRegion") != data.dump().npos) {
      value = data.at("sBLCRegion").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_blc_region(cam_id, value.c_str());
    }
    if (data.dump().find("sHLC") != data.dump().npos) {
      value = data.at("sHLC").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_hlc(cam_id, value.c_str());
    }
    if (data.dump().find("iHDRLevel") != data.dump().npos) {
      value_int = atoi(data.at("iHDRLevel").dump().c_str());
      rk_isp_set_hdr_level(cam_id, value_int);
    }
    if (data.dump().find("iBLCStrength") != data.dump().npos) {
      value_int = atoi(data.at("iBLCStrength").dump().c_str());
      rk_isp_set_blc_strength(cam_id, value_int);
    }
    if (data.dump().find("iHLCLevel") != data.dump().npos) {
      value_int = atoi(data.at("iHLCLevel").dump().c_str());
      rk_isp_set_hlc_level(cam_id, value_int);
    }
    if (data.dump().find("iDarkBoostLevel") != data.dump().npos) {
      value_int = atoi(data.at("iDarkBoostLevel").dump().c_str());
      rk_isp_set_dark_boost_level(cam_id, value_int);
    }
    // it will restart app, must be the last call
    if (data.dump().find("sHDR") != data.dump().npos) {
      value = data.at("sHDR").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_hdr(cam_id, value.c_str());
    }
  } else if (!string.compare(PATH_IMAGE_WHITE_BLANCE)) {
    if (data.dump().find("sWhiteBlanceStyle") != data.dump().npos) {
      value = data.at("sWhiteBlanceStyle").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_white_blance_style(cam_id, value.c_str());
    }
    if (data.dump().find("iWhiteBalanceRed") != data.dump().npos) {
      value_int = atoi(data.at("iWhiteBalanceRed").dump().c_str());
      rk_isp_set_white_blance_red(cam_id, value_int);
    }
    if (data.dump().find("iWhiteBalanceGreen") != data.dump().npos) {
      value_int = atoi(data.at("iWhiteBalanceGreen").dump().c_str());
      rk_isp_set_white_blance_green(cam_id, value_int);
    }
    if (data.dump().find("iWhiteBalanceBlue") != data.dump().npos) {
      value_int = atoi(data.at("iWhiteBalanceBlue").dump().c_str());
      rk_isp_set_white_blance_blue(cam_id, value_int);
    }
  } else if (!string.compare(PATH_IMAGE_ENHANCEMENT)) {
    if (data.dump().find("sNoiseReduceMode") != data.dump().npos) {
      value = data.at("sNoiseReduceMode").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_noise_reduce_mode(cam_id, value.c_str());
    }
    if (data.dump().find("sDehaze") != data.dump().npos) {
      value = data.at("sDehaze").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_dehaze(cam_id, value.c_str());
    }
    if (data.dump().find("sGrayScaleMode") != data.dump().npos) {
      value = data.at("sGrayScaleMode").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_gray_scale_mode(cam_id, value.c_str());
    }
    if (data.dump().find("sDistortionCorrection") != data.dump().npos) {
      value = data.at("sDistortionCorrection").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_distortion_correction(cam_id, value.c_str());
    }
    if (data.dump().find("iSpatialDenoiseLevel") != data.dump().npos) {
      value_int = atoi(data.at("iSpatialDenoiseLevel").dump().c_str());
      rk_isp_set_spatial_denoise_level(cam_id, value_int);
    }
    if (data.dump().find("iTemporalDenoiseLevel") != data.dump().npos) {
      value_int = atoi(data.at("iTemporalDenoiseLevel").dump().c_str());
      rk_isp_set_temporal_denoise_level(cam_id, value_int);
    }
    if (data.dump().find("iDehazeLevel") != data.dump().npos) {
      value_int = atoi(data.at("iDehazeLevel").dump().c_str());
      rk_isp_set_dehaze_level(cam_id, value_int);
    }
    if (data.dump().find("iFecLevel") != data.dump().npos) {
      value_int = atoi(data.at("iFecLevel").dump().c_str());
      rk_isp_set_fec_level(cam_id, value_int);
    }
    if (data.dump().find("iLdchLevel") != data.dump().npos) {
      value_int = atoi(data.at("iLdchLevel").dump().c_str());
      rk_isp_set_ldch_level(cam_id, value_int);
    }
  } else if (!string.compare(PATH_IMAGE_VIDEO_ADJUSTMEN)) {
    if (data.dump().find("sPowerLineFrequencyMode") != data.dump().npos) {
      value = data.at("sPowerLineFrequencyMode").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_power_line_frequency_mode(cam_id, value.c_str());
    }
    if (data.dump().find("sImageFlip") != data.dump().npos) {
      value = data.at("sImageFlip").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_image_flip(cam_id, value.c_str());
    }
    if (data.dump().find("iImageRotation") != data.dump().npos) {
      value_int = atoi(data.at("iImageRotation").dump().c_str());
      rk_video_set_rotation(value_int);
    }
  } else if (!string.compare(PATH_IMAGE_AF)) {
    if (data.dump().find("sAFMode") != data.dump().npos) {
      value = data.at("sAFMode").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_af_mode(cam_id, value.c_str());
    }
  } else if (!string.compare(PATH_IMAGE_SCENARIO)) {
    if (data.dump().find("sScenario") != data.dump().npos) {
      value = data.at("sScenario").dump();
      value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
      rk_isp_set_scenario(cam_id, value.c_str());
    }
  }
}

void ImageApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  int cam_id = 0;
  nlohmann::json content;
  std::string path_api_resource = "";
  std::string path_channel_resource = "";
  std::string path_specific_resource = "";
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  /* Get Path Information */
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_channel_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
    pos_first = path_channel_resource.find_first_of("/");
    if (pos_first != -1)
      path_specific_resource = path_channel_resource.substr(
          pos_first + 1, path_channel_resource.size());
  }
  if (!path_channel_resource.empty())
    cam_id = atoi(
        path_channel_resource.substr(0, path_channel_resource.size()).c_str());

  if (Req.Method == "GET") {
    /* Get based on path information */
    if (path_channel_resource.empty()) { /* path info is /image */
      content = nlohmann::json::array();
      // content.push_back(image_channel_resource_get(1, id));
    } else {
      if (path_specific_resource.empty()) { /* path info is /image/0 */
        content = image_channel_resource_get(cam_id);
      } else { /* path example is /image/0/blc */
        content = image_specific_resource_get(path_specific_resource, cam_id);
      }
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    nlohmann::json diff;
    nlohmann::json cfg_old;
    nlohmann::json image_config = Req.PostObject; /* must be json::object */

    if (path_specific_resource
            .empty()) { /* path info is /image/0 or /image/1 */
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      //  cfg_old = image_channel_resource_get(channel_id, id);
      //   /* Erase unexist data */
      //   for (auto &x : nlohmann::json::iterator_wrapper(image_config)) {
      //     if (cfg_old.dump().find(x.key()) == cfg_old.dump().npos)
      //         image_config.erase(x.key());
      //   }
      //   /* Erase unchanged data */
      //   diff = nlohmann::json::diff(cfg_old, image_config);
      //   for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
      //     if (diff.dump().find(x.key()) == diff.dump().npos)
      //       image_config.erase(x.key());
      //   }
      //   /* Set */
      //   if (!image_config.empty())
      //     image_channel_resource_set(image_config, id);
      //   /* Get new info */
      //   content = image_channel_resource_get(channel_id, id);
    } else { /* path example is /image/0/blc */
      if (!path_specific_resource.compare(PATH_IMAGE_AF_CMD)) {
        if (image_config.dump().find("cmd") != std::string::npos) {
          std::string af_cmd = image_config.at("cmd");
          int ret = 0;
          if (af_cmd == "ZoomIn")
            ret = rk_isp_af_zoom_in(0);
          else if (af_cmd == "ZoomOut")
            ret = rk_isp_af_zoom_out(0);
          else if (af_cmd == "FocusIn")
            ret = rk_isp_af_focus_in(0);
          else if (af_cmd == "FocusOut")
            ret = rk_isp_af_focus_out(0);
          else if (af_cmd == "FocusOnce")
            ret = rk_isp_af_focus_once(0);
          else
            minilog_debug("unsupport, af_cmd is %s\n", af_cmd.c_str());
          // TODO: overRange, onlyManualSupport, NotAutoSupport
          if (ret)
            content.emplace("rst", "fail");
          else
            content.emplace("rst", "success");
        } else {
          content.emplace("rst", "illegalCmd");
        }
      } else {
        // TODO: only set diff
        /* Set */
        image_specific_resource_set(path_specific_resource, image_config,
                                    cam_id);
        /* Get new info */
        content = image_specific_resource_get(path_specific_resource, cam_id);
      }
    }
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else // USE_RKIPC

nlohmann::json image_specific_config_get(std::string string) {
  nlohmann::json specific_config = nlohmann::json::array();
  char *table;

  if (!string.compare(PATH_IMAGE_SCENARIO))
    table = TABLE_IMAGE_SCENARIO;
  if (!string.compare(PATH_IMAGE_ADJUSTMENT))
    table = TABLE_IMAGE_ADJUSTMENT;
  if (!string.compare(PATH_IMAGE_EXPOSURE))
    table = TABLE_IMAGE_EXPOSURE;
  if (!string.compare(PATH_IMAGE_NIGHT_TO_DAY))
    table = TABLE_IMAGE_NIGHT_TO_DAY;
  if (!string.compare(PATH_IMAGE_BLC))
    table = TABLE_IMAGE_BLC;
  if (!string.compare(PATH_IMAGE_WHITE_BLANCE))
    table = TABLE_IMAGE_WHITE_BLANCE;
  if (!string.compare(PATH_IMAGE_ENHANCEMENT))
    table = TABLE_IMAGE_ENHANCEMENT;
  if (!string.compare(PATH_IMAGE_VIDEO_ADJUSTMEN))
    table = TABLE_IMAGE_VIDEO_ADJUSTMEN;
  specific_config =
      nlohmann::json::parse(dbserver_media_get(table)).at("jData");

  return specific_config;
}

nlohmann::json image_specific_resource_get(std::string string, int id) {
  nlohmann::json specific_config =
      nlohmann::json::array(); /* array of all the same resources */
  nlohmann::json specific_resource =
      nlohmann::json::object(); /* one of all the same resources */

  specific_config = image_specific_config_get(string);
  specific_config.at(id).erase("id");
  specific_resource = specific_config.at(id);

  return specific_resource;
}

nlohmann::json image_channel_resource_get(int channel_id, int id) {
  nlohmann::json resource = nlohmann::json::object();
  nlohmann::json image_adjustment =
      image_specific_config_get(PATH_IMAGE_ADJUSTMENT);
  nlohmann::json image_exposure =
      image_specific_config_get(PATH_IMAGE_EXPOSURE);
  nlohmann::json image_night_to_day =
      image_specific_config_get(PATH_IMAGE_NIGHT_TO_DAY);
  nlohmann::json image_BLC = image_specific_config_get(PATH_IMAGE_BLC);
  nlohmann::json image_white_blance =
      image_specific_config_get(PATH_IMAGE_WHITE_BLANCE);
  nlohmann::json image_enhancement =
      image_specific_config_get(PATH_IMAGE_ENHANCEMENT);
  nlohmann::json image_video_adjustment =
      image_specific_config_get(PATH_IMAGE_VIDEO_ADJUSTMEN);

  image_adjustment.at(id).erase("id");
  image_exposure.at(id).erase("id");
  image_night_to_day.at(id).erase("id");
  image_BLC.at(id).erase("id");
  image_white_blance.at(id).erase("id");
  image_enhancement.at(id).erase("id");
  image_video_adjustment.at(id).erase("id");

  resource.emplace("id", channel_id);
  resource.emplace(KEY_IMAGE_ADJUSTMENT, image_adjustment.at(id));
  resource.emplace(KEY_IMAGE_EXPOSURE, image_exposure.at(id));
  resource.emplace(KEY_IMAGE_NIGHT_TO_DAY, image_night_to_day.at(id));
  resource.emplace(KEY_IMAGE_BLC, image_BLC.at(id));
  resource.emplace(KEY_IMAGE_WHITE_BLANCE, image_white_blance.at(id));
  resource.emplace(KEY_IMAGE_ENHANCEMENT, image_enhancement.at(id));
  resource.emplace(KEY_IMAGE_VIDEO_ADJUSTMEN, image_video_adjustment.at(id));

  return resource;
}

void image_specific_resource_set(std::string current, nlohmann::json data,
                                 int id) {
  char *table;

  if (!current.compare(PATH_IMAGE_SCENARIO))
    table = TABLE_IMAGE_SCENARIO;
  if (!current.compare(PATH_IMAGE_ADJUSTMENT))
    table = TABLE_IMAGE_ADJUSTMENT;
  if (!current.compare(PATH_IMAGE_EXPOSURE))
    table = TABLE_IMAGE_EXPOSURE;
  if (!current.compare(PATH_IMAGE_NIGHT_TO_DAY))
    table = TABLE_IMAGE_NIGHT_TO_DAY;
  if (!current.compare(PATH_IMAGE_BLC))
    table = TABLE_IMAGE_BLC;
  if (!current.compare(PATH_IMAGE_WHITE_BLANCE))
    table = TABLE_IMAGE_WHITE_BLANCE;
  if (!current.compare(PATH_IMAGE_ENHANCEMENT))
    table = TABLE_IMAGE_ENHANCEMENT;
  if (!current.compare(PATH_IMAGE_VIDEO_ADJUSTMEN))
    table = TABLE_IMAGE_VIDEO_ADJUSTMEN;

  dbserver_media_set(table, (char *)data.dump().c_str(), id);
  mediaserver_set((char *)TABLE_VIDEO, 0, (char *)data.dump().c_str());
}

void image_channel_resource_set(nlohmann::json json_config, int id) {
  for (auto &x : nlohmann::json::iterator_wrapper(json_config)) {
    auto val = x.value(); /* string or int */
    std::string resource;
    if (x.key() == KEY_IMAGE_ADJUSTMENT)
      resource = PATH_IMAGE_ADJUSTMENT;
    else if (x.key() == KEY_IMAGE_EXPOSURE)
      resource = PATH_IMAGE_EXPOSURE;
    else if (x.key() == KEY_IMAGE_NIGHT_TO_DAY)
      resource = PATH_IMAGE_NIGHT_TO_DAY;
    else if (x.key() == KEY_IMAGE_BLC)
      resource = PATH_IMAGE_BLC;
    else if (x.key() == KEY_IMAGE_WHITE_BLANCE)
      resource = PATH_IMAGE_WHITE_BLANCE;
    else if (x.key() == KEY_IMAGE_ENHANCEMENT)
      resource = PATH_IMAGE_ENHANCEMENT;
    else if (x.key() == KEY_IMAGE_VIDEO_ADJUSTMEN)
      resource = PATH_IMAGE_VIDEO_ADJUSTMEN;
    else
      continue;

    /* Erase unchanged data */
    nlohmann::json resource_old = image_specific_resource_get(resource, id);
    nlohmann::json resource_diff = nlohmann::json::diff(resource_old, val);
    for (auto &y : nlohmann::json::iterator_wrapper(resource_old)) {
      if (resource_diff.dump().find(y.key()) == resource_diff.dump().npos)
        val.erase(y.key());
    }
    image_specific_resource_set(resource, val, id);
  }
}

void ImageApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  int id;
  int channel_id = 0;
  nlohmann::json content;
  nlohmann::json scenario;
  std::string scenario_str = "";
  std::string path_api_resource = "";
  std::string path_channel_resource = "";
  std::string path_specific_resource = "";
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  /* Get the current scene and the corresponding table id */
  scenario = image_specific_resource_get(PATH_IMAGE_SCENARIO, 0);
  scenario_str = scenario.at("sScenario").dump();
  scenario_str.erase(0, 1); /* erase " */
  scenario_str.erase(scenario_str.end() - 1, scenario_str.end());
  if (!scenario_str.compare(IMAGE_SCENARIO_NORMAL))
    id = 0;
  if (!scenario_str.compare(IMAGE_SCENARIO_BACKLIGHT))
    id = 1;
  if (!scenario_str.compare(IMAGE_SCENARIO_FRONTLIGHT))
    id = 2;
  if (!scenario_str.compare(IMAGE_SCENARIO_LOW_ILLUMINATION))
    id = 3;
  if (!scenario_str.compare(IMAGE_SCENARIO_CUSTOM1))
    id = 4;
  if (!scenario_str.compare(IMAGE_SCENARIO_CUSTOM2))
    id = 5;

  /* Get Path Information */
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_channel_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
    pos_first = path_channel_resource.find_first_of("/");
    if (pos_first != -1)
      path_specific_resource = path_channel_resource.substr(
          pos_first + 1, path_channel_resource.size());
  }
  if (!path_channel_resource.empty())
    channel_id = atoi(
        path_channel_resource.substr(0, path_channel_resource.size()).c_str());
  id = id + 6 * channel_id; /* 6 scenarios for each channel */

  if (Req.Method == "GET") {
    /* Get based on path information */
    if (path_channel_resource.empty()) { /* path info is /image */
      content = nlohmann::json::array();
      /* TODO: channel 1 need dbserver to create 6 more tables and test */
      if (channel_id == 0) {
        content.push_back(image_channel_resource_get(0, id));
        // content.push_back(image_channel_resource_get(1, id + 6));
      } else if (channel_id == 1) {
        content.push_back(image_channel_resource_get(0, id - 6));
        // content.push_back(image_channel_resource_get(1, id));
      }
      // content.push_back(image_channel_resource_get(1, id));
    } else {
      if (path_specific_resource.empty()) { /* path info is /image/0 */
        content = image_channel_resource_get(channel_id, id);
      } else { /* path example is /image/0/blc */
        if (!path_specific_resource.compare(PATH_IMAGE_SCENARIO))
          id = 0;
        content = image_specific_resource_get(path_specific_resource, id);
      }
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    nlohmann::json diff;
    nlohmann::json cfg_old;
    nlohmann::json image_config = Req.PostObject; /* must be json::object */
    if (path_specific_resource
            .empty()) { /* path info is /image/0 or /image/1 */
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif

      cfg_old = image_channel_resource_get(channel_id, id);
      /* Erase unexist data */
      for (auto &x : nlohmann::json::iterator_wrapper(image_config)) {
        if (cfg_old.dump().find(x.key()) == cfg_old.dump().npos)
          image_config.erase(x.key());
      }
      /* Erase unchanged data */
      diff = nlohmann::json::diff(cfg_old, image_config);
      for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          image_config.erase(x.key());
      }
      /* Set */
      if (!image_config.empty())
        image_channel_resource_set(image_config, id);
      /* Get new info */
      content = image_channel_resource_get(channel_id, id);
    } else { /* path example is /image/0/blc */
      if (!path_specific_resource.compare(PATH_IMAGE_SCENARIO))
        id = 0;
      cfg_old = image_specific_resource_get(path_specific_resource, id);
      /* Erase unexist data */
      for (auto &x : nlohmann::json::iterator_wrapper(image_config)) {
        if (cfg_old.dump().find(x.key()) == cfg_old.dump().npos)
          image_config.erase(x.key());
      }
      /* Erase unchanged data */
      diff = nlohmann::json::diff(cfg_old, image_config);
      for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          image_config.erase(x.key());
      }
      /* Set */
      if (!image_config.empty())
        image_specific_resource_set(path_specific_resource, image_config, id);
      /* Get new info */
      content = image_specific_resource_get(path_specific_resource, id);
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}
#endif

} // namespace cgi
} // namespace rockchip
