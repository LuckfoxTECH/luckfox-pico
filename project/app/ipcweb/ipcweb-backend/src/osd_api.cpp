// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "osd_api.h"
#include "common.h"
#include <fstream>

namespace rockchip {
namespace cgi {

#ifdef USE_RKIPC

#define OSD_CHANNAL_ID 0
#define OSD_DATE_TIME_ID 1
#define OSD_IMAGE_ID 6

nlohmann::json osd_overlays_get() {
  int value;
  char *str;
  char *tmp = new char[256];
  nlohmann::json osd_overlays;
  nlohmann::json normalized_screen_size;

  rk_osd_get_normalized_screen_width(&value);
  normalized_screen_size.emplace("iNormalizedScreenWidth", value);
  rk_osd_get_normalized_screen_height(&value);
  normalized_screen_size.emplace("iNormalizedScreenHeight", value);

  // attribute, all OSD tables have the same attributes
  nlohmann::json attribute;
  rk_osd_get_attribute(&tmp);
  attribute.emplace("sOSDAttribute", tmp);
  rk_osd_get_font_size(&value);
  sprintf(tmp, "%d*%d", value, value);
  attribute.emplace("sOSDFontSize", tmp);
  rk_osd_get_font_color_mode(&tmp);
  attribute.emplace("sOSDFrontColorMode", tmp);
  rk_osd_get_font_color(&tmp);
  attribute.emplace("sOSDFrontColor", tmp);
  rk_osd_get_alignment(&tmp);
  attribute.emplace("sAlignment", tmp);
  rk_osd_get_boundary(&value);
  attribute.emplace("iBoundary", value);

  // channel_name
  nlohmann::json channel_name;
  rk_osd_get_position_x(OSD_CHANNAL_ID, &value);
  channel_name.emplace("iPositionX", value);
  rk_osd_get_position_y(OSD_CHANNAL_ID, &value);
  channel_name.emplace("iPositionY", value);
  rk_osd_get_enabled(OSD_CHANNAL_ID, &value);
  channel_name.emplace("iChannelNameOverlayEnabled", value);
  rk_osd_get_display_text(OSD_CHANNAL_ID, &tmp);
  channel_name.emplace("sChannelName", tmp);

  // date_time
  nlohmann::json date_time;
  rk_osd_get_position_x(OSD_DATE_TIME_ID, &value);
  date_time.emplace("iPositionX", value);
  rk_osd_get_position_y(OSD_DATE_TIME_ID, &value);
  date_time.emplace("iPositionY", value);
  rk_osd_get_enabled(OSD_DATE_TIME_ID, &value);
  date_time.emplace("iDateTimeOverlayEnabled", value);
  rk_osd_get_date_style(OSD_DATE_TIME_ID, &tmp);
  date_time.emplace("sDateStyle", tmp);
  rk_osd_get_time_style(OSD_DATE_TIME_ID, &tmp);
  date_time.emplace("sTimeStyle", tmp);
  rk_osd_get_display_week_enabled(OSD_DATE_TIME_ID, &value);
  date_time.emplace("iDisplayWeekEnabled", value);
  // character
  nlohmann::json all_character;
  for (int i = 2; i < 4; i++) {
    nlohmann::json character;
    rk_osd_get_position_x(i, &value);
    character.emplace("iPositionX", value);
    rk_osd_get_position_y(i, &value);
    character.emplace("iPositionY", value);
    rk_osd_get_enabled(i, &value);
    character.emplace("iTextOverlayEnabled", value);
    rk_osd_get_is_presistent_text(&value);
    character.emplace("sIsPersistentText", value);
    rk_osd_get_display_text(i, &tmp);
    character.emplace("sDisplayText", tmp);
    character.emplace("id", i - 2);

    all_character.push_back(character);
  }
  // overlays
  osd_overlays.emplace("normalizedScreenSize", normalized_screen_size);
  osd_overlays.emplace("attribute", attribute);
  osd_overlays.emplace("channelNameOverlay", channel_name);
  osd_overlays.emplace("dateTimeOverlay", date_time);
  osd_overlays.emplace("characterOverlay", all_character);

  delete[] tmp;

  return osd_overlays;
}

nlohmann::json osd_image_get() {
  int value;
  char *str;
  char *tmp = new char[256];
  nlohmann::json osd_image;

  nlohmann::json normalized_screen_size;
  rk_osd_get_normalized_screen_width(&value);
  normalized_screen_size.emplace("iNormalizedScreenWidth", value);
  rk_osd_get_normalized_screen_height(&value);
  normalized_screen_size.emplace("iNormalizedScreenHeight", value);

  nlohmann::json image_overlay;
  rk_osd_get_position_x(OSD_IMAGE_ID, &value);
  image_overlay.emplace("iPositionX", value);
  rk_osd_get_position_y(OSD_IMAGE_ID, &value);
  image_overlay.emplace("iPositionY", value);
  rk_osd_get_enabled(OSD_IMAGE_ID, &value);
  image_overlay.emplace("iImageOverlayEnabled", value);
  image_overlay.emplace("iTransparentColorEnabled", 1);

  // get bmp picture width and height
  rk_osd_get_image_path(OSD_IMAGE_ID, &tmp);
  FILE *picture_file = fopen(tmp, "rb");
  int32_t bmp_width;
  int32_t bmp_height;
  int offset = sizeof(uint16_t) * 3 + sizeof(uint32_t) * 3;
  // skip other header information
  fseek(picture_file, offset, SEEK_SET);
  fread(&bmp_width, 1, sizeof(bmp_width), picture_file);
  fread(&bmp_height, 1, sizeof(bmp_height), picture_file);
  fclose(picture_file);
  image_overlay.emplace("iImageWidth", bmp_width);
  image_overlay.emplace("iImageHeight", bmp_height);

  osd_image.emplace("normalizedScreenSize", normalized_screen_size);
  osd_image.emplace("imageOverlay", image_overlay);

  delete[] tmp;
  return osd_image;
}

nlohmann::json osd_privacy_mask_get() {
  int value;
  char *str;
  char *tmp = new char[256];
  nlohmann::json osd_privacy_mask;
  nlohmann::json all_privacy_mask;

  nlohmann::json normalized_screen_size;
  rk_osd_get_normalized_screen_width(&value);
  normalized_screen_size.emplace("iNormalizedScreenWidth", value);
  rk_osd_get_normalized_screen_height(&value);
  normalized_screen_size.emplace("iNormalizedScreenHeight", value);

  for (int i = 4; i < 6; i++) {
    nlohmann::json privacy_mask;
    rk_osd_get_position_x(i, &value);
    privacy_mask.emplace("iPositionX", value);
    rk_osd_get_position_y(i, &value);
    privacy_mask.emplace("iPositionY", value);
    rk_osd_get_enabled(i, &value);
    privacy_mask.emplace("iPrivacyMaskEnabled", value);
    rk_osd_get_width(i, &value);
    privacy_mask.emplace("iMaskWidth", value);
    rk_osd_get_height(i, &value);
    privacy_mask.emplace("iMaskHeight", value);
    privacy_mask.emplace("id", i - 4);
    all_privacy_mask.push_back(privacy_mask);
  }
  osd_privacy_mask.emplace("normalizedScreenSize", normalized_screen_size);
  osd_privacy_mask.emplace("privacyMask", all_privacy_mask);

  delete[] tmp;
  return osd_privacy_mask;
}

void osd_overlays_set(nlohmann::json osd_overlays) {
  int value_int;
  std::string value;

  for (auto &x : nlohmann::json::iterator_wrapper(osd_overlays)) {
    if (x.key() == KEY_NORMALIZED_SCREEN_SIZE) {
      // auto val = x.value(); /* string or int */
      // dbserver_media_set((char *)TABLE_NORMALIZED_SCREEN_SIZE,
      //                                (char *)val.dump().c_str(), 0);
    } else if (x.key() == KEY_OSD_ATTRIBUTE) {
      nlohmann::json param = osd_overlays.at(KEY_OSD_ATTRIBUTE);
      if (param.dump().find("iBoundary") != param.dump().npos) {
        value_int = atoi(param.at("iBoundary").dump().c_str());
        rk_osd_set_boundary(value_int);
      }
      if (param.dump().find("sOSDFontSize") != param.dump().npos) {
        value = param.at("sOSDFontSize").dump();
        value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
        sscanf(value.c_str(), "%d*%d", &value_int, &value_int);
        rk_osd_set_font_size(value_int);
      }
      if (param.dump().find("sAlignment") != param.dump().npos) {
        value = param.at("sAlignment").dump();
        value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
        rk_osd_set_alignment(value.c_str());
      }
      if (param.dump().find("sOSDAttribute") != param.dump().npos) {
        value = param.at("sOSDAttribute").dump();
        value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
        rk_osd_set_attribute(value.c_str());
      }
      if (param.dump().find("sOSDFrontColor") != param.dump().npos) {
        value = param.at("sOSDFrontColor").dump();
        value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
        rk_osd_set_font_color(value.c_str());
      }
      if (param.dump().find("sOSDFrontColorMode") != param.dump().npos) {
        value = param.at("sOSDFrontColorMode").dump();
        value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
        rk_osd_set_font_color_mode(value.c_str());
      }
    } else if (x.key() == KEY_OSD_CHANNEL_NAME_OVERLAY) {
      nlohmann::json param = osd_overlays.at(KEY_OSD_CHANNEL_NAME_OVERLAY);
      if (param.dump().find("iChannelNameOverlayEnabled") !=
          param.dump().npos) {
        value_int = atoi(param.at("iChannelNameOverlayEnabled").dump().c_str());
        rk_osd_set_enabled(OSD_CHANNAL_ID, value_int);
      }
      if (param.dump().find("iPositionX") != param.dump().npos) {
        value_int = atoi(param.at("iPositionX").dump().c_str());
        rk_osd_set_position_x(OSD_CHANNAL_ID, value_int);
      }
      if (param.dump().find("iPositionY") != param.dump().npos) {
        value_int = atoi(param.at("iPositionY").dump().c_str());
        rk_osd_set_position_y(OSD_CHANNAL_ID, value_int);
      }
      if (param.dump().find("sChannelName") != param.dump().npos) {
        value = param.at("sChannelName").dump();
        value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
        rk_osd_set_display_text(OSD_CHANNAL_ID, value.c_str());
      }
    } else if (x.key() == KEY_OSD_DATE_TIME_OVERLAY) {
      nlohmann::json param = osd_overlays.at(KEY_OSD_DATE_TIME_OVERLAY);
      if (param.dump().find("iDateTimeOverlayEnabled") != param.dump().npos) {
        value_int = atoi(param.at("iDateTimeOverlayEnabled").dump().c_str());
        rk_osd_set_enabled(OSD_DATE_TIME_ID, value_int);
      }
      if (param.dump().find("iDisplayWeekEnabled") != param.dump().npos) {
        value_int = atoi(param.at("iDisplayWeekEnabled").dump().c_str());
        rk_osd_set_display_week_enabled(OSD_DATE_TIME_ID, value_int);
      }
      if (param.dump().find("iPositionX") != param.dump().npos) {
        value_int = atoi(param.at("iPositionX").dump().c_str());
        rk_osd_set_position_x(OSD_DATE_TIME_ID, value_int);
      }
      if (param.dump().find("iPositionY") != param.dump().npos) {
        value_int = atoi(param.at("iPositionY").dump().c_str());
        rk_osd_set_position_y(OSD_DATE_TIME_ID, value_int);
      }
      if (param.dump().find("sDateStyle") != param.dump().npos) {
        value = param.at("sDateStyle").dump();
        value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
        rk_osd_set_date_style(OSD_DATE_TIME_ID, value.c_str());
      }
      if (param.dump().find("sTimeStyle") != param.dump().npos) {
        value = param.at("sTimeStyle").dump();
        value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
        rk_osd_set_time_style(OSD_DATE_TIME_ID, value.c_str());
      }
    } else if (x.key() == KEY_OSD_CHARACTER_OVERLAY) {
      nlohmann::json all_character = osd_overlays.at(KEY_OSD_CHARACTER_OVERLAY);
      for (auto &x : nlohmann::json::iterator_wrapper(all_character)) {
        nlohmann::json param = x.value();
        int id = param.at("id");
        if (id > 1)
          continue;
        if (param.dump().find("iTextOverlayEnabled") != param.dump().npos) {
          value_int = atoi(param.at("iTextOverlayEnabled").dump().c_str());
          rk_osd_set_enabled(id + 2, value_int);
        }
        if (param.dump().find("iPositionX") != param.dump().npos) {
          value_int = atoi(param.at("iPositionX").dump().c_str());
          rk_osd_set_position_x(id + 2, value_int);
        }
        if (param.dump().find("iPositionY") != param.dump().npos) {
          value_int = atoi(param.at("iPositionY").dump().c_str());
          rk_osd_set_position_y(id + 2, value_int);
        }
        if (param.dump().find("sDisplayText") != param.dump().npos) {
          value = param.at("sDisplayText").dump();
          value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
          rk_osd_set_display_text(id + 2, value.c_str());
        }
      }
    }
  }
}

void osd_image_set(nlohmann::json osd_image_config) {
  // get bmp picture width and height
  int value_int;
  char *tmp = new char[256];
  // rk_osd_get_image_path(OSD_IMAGE_ID, &tmp);
  // FILE *picture_file = fopen(BMP_FILE_PATH, "rb");
  // int32_t bmp_width;
  // int32_t bmp_height;
  // int offset = sizeof(uint16_t) * 3 + sizeof(uint32_t) * 3;
  // // skip other header information
  // fseek(picture_file, offset, SEEK_SET);
  // fread(&bmp_width, 1, sizeof(bmp_width), picture_file);
  // fread(&bmp_height, 1, sizeof(bmp_height), picture_file);
  // fclose(picture_file);
  for (auto &x : nlohmann::json::iterator_wrapper(osd_image_config)) {
    if (x.key() == KEY_NORMALIZED_SCREEN_SIZE) {
      // auto val = x.value(); /* string or int */
      // dbserver_media_set((char *)TABLE_NORMALIZED_SCREEN_SIZE,
      //                                (char *)val.dump().c_str(), 0);
    } else if (x.key() == KEY_OSD_IMAGE_OVERLAY) {
      nlohmann::json param = osd_image_config.at(KEY_OSD_IMAGE_OVERLAY);
      if (param.dump().find("iImageOverlayEnabled") != param.dump().npos) {
        value_int = atoi(param.at("iImageOverlayEnabled").dump().c_str());
        rk_osd_set_enabled(OSD_IMAGE_ID, value_int);
      }
      if (param.dump().find("iPositionX") != param.dump().npos) {
        value_int = atoi(param.at("iPositionX").dump().c_str());
        rk_osd_set_position_x(OSD_IMAGE_ID, value_int);
      }
      if (param.dump().find("iPositionY") != param.dump().npos) {
        value_int = atoi(param.at("iPositionY").dump().c_str());
        rk_osd_set_position_y(OSD_IMAGE_ID, value_int);
      }
    }
  }
  delete[] tmp;
}

void osd_privacy_mask_set(nlohmann::json osd_privacy_mask_config) {
  int value_int;
  for (auto &x : nlohmann::json::iterator_wrapper(osd_privacy_mask_config)) {
    if (x.key() == KEY_NORMALIZED_SCREEN_SIZE) {
      // auto val = x.value(); /* string or int */
      // dbserver_media_set((char *)TABLE_NORMALIZED_SCREEN_SIZE,
      //                                (char *)val.dump().c_str(), 0);
    } else if (x.key() == KEY_OSD_PRIVACY_MASK) {
      nlohmann::json all_privacy_mask =
          osd_privacy_mask_config.at(KEY_OSD_PRIVACY_MASK);
      for (auto &x : nlohmann::json::iterator_wrapper(all_privacy_mask)) {
        nlohmann::json param = x.value();
        int id = param.at("id");
        if (id > 1)
          continue;
        if (param.dump().find("iPrivacyMaskEnabled") != param.dump().npos) {
          value_int = atoi(param.at("iPrivacyMaskEnabled").dump().c_str());
          rk_osd_set_enabled(id + 4, value_int);
        }
        if (param.dump().find("iMaskWidth") != param.dump().npos) {
          value_int = atoi(param.at("iMaskWidth").dump().c_str());
          rk_osd_set_width(id + 4, value_int);
        }
        if (param.dump().find("iMaskHeight") != param.dump().npos) {
          value_int = atoi(param.at("iMaskHeight").dump().c_str());
          rk_osd_set_height(id + 4, value_int);
        }
        if (param.dump().find("iPositionX") != param.dump().npos) {
          value_int = atoi(param.at("iPositionX").dump().c_str());
          rk_osd_set_position_x(id + 4, value_int);
        }
        if (param.dump().find("iPositionY") != param.dump().npos) {
          value_int = atoi(param.at("iPositionY").dump().c_str());
          rk_osd_set_position_y(id + 4, value_int);
        }
        if (id == 1)
          break;
      }
    }
  }
}

void OSDApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  std::string path_api_resource;
  std::string path_specific_resource;
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  /* Get Path Information */
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_specific_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
  }

  if (Req.Method == "GET") {
    if (!path_specific_resource.compare("overlays")) {
      content = osd_overlays_get();
    } else if (!path_specific_resource.compare("image")) {
      content = osd_image_get();
    } else if (!path_specific_resource.compare("privacy-mask")) {
      content = osd_privacy_mask_get();
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
#ifdef ENABLE_JWT
    if (user_level > 1) {
      Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
      return;
    }
#endif
    nlohmann::json diff;
    nlohmann::json cfg_old;
    nlohmann::json osd_config = Req.PostObject; /* must be json::object */
    if (!path_specific_resource.compare("overlays")) {
      /* Erase unchanged data */
      cfg_old = osd_overlays_get();
      diff = nlohmann::json::diff(cfg_old, osd_config);
      for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          osd_config.erase(x.key());
      }
      minilog_debug("osd_config is %s\n", osd_config.dump().c_str());
      /* Set */
      if (!osd_config.empty()) {
        osd_overlays_set(osd_config);
        rk_osd_restart();
      }
      /* Get new info */
      content = osd_overlays_get();
      // osd_set_mediaserver(osd_config);
      // minilog_debug("content is %s\n", content.dump().c_str());
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("image")) {
      /* Erase unchanged data */
      cfg_old = osd_image_get();
      diff = nlohmann::json::diff(cfg_old, osd_config);
      for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          osd_config.erase(x.key());
      }
      /* Set */
      if (!osd_config.empty()) {
        osd_image_set(osd_config);
        rk_osd_restart();
      }
      /* Get new info */
      content = osd_image_get();
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("privacy-mask")) {
      /* Erase unchanged data */
      cfg_old = osd_privacy_mask_get();
      diff = nlohmann::json::diff(cfg_old, osd_config);
      for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          osd_config.erase(x.key());
      }
      /* Set */
      if (!osd_config.empty()) {
        osd_privacy_mask_set(osd_config);
        rk_osd_restart();
      }
      /* Get new info */
      minilog_debug("1\n");
      content = osd_privacy_mask_get();
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("image/picture")) {
      for (int i = 0; i < Req.Files.size(); i++) {
        if (!Req.Files.at(i).getDataType().compare("image/bmp")) {
          if (Req.Files.at(i).getDataLength() > (256 * 1024)) { // 256kb
            Resp.setErrorResponse(HttpStatus::kBadRequest,
                                  "Picture is too big!");
          } else {
            int end_position = 0;
            char *tmp = new char[256];
            rk_osd_get_image_path(OSD_IMAGE_ID, &tmp);
            std::ofstream picture_file(tmp, std::ofstream::out);
            delete[] tmp;
            picture_file << Req.Files.at(i).getData();
            end_position = picture_file.tellp();
            picture_file.close();
            if (end_position == Req.Files.at(i).getDataLength()) {
              rk_osd_restart();
              Resp.setHeader(HttpStatus::kOk, "OK");
            } else {
              Resp.setErrorResponse(HttpStatus::kBadRequest,
                                    "Picture upload failed!");
            }
          }
          i = Req.Files.size();
        } else if ((i == Req.Files.size() - 1) &&
                   (Req.Files.at(i).getDataType().compare("image/bmp"))) {
          Resp.setErrorResponse(HttpStatus::kBadRequest, "Can't find bmp!");
        }
      }
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else // #ifdef USE_RKIPC

nlohmann::json osd_overlays_get() {
  char *str;
  nlohmann::json osd_overlays;
  nlohmann::json osd_data;
  nlohmann::json db_osd;

  str = dbserver_media_get((char *)TABLE_NORMALIZED_SCREEN_SIZE);
  nlohmann::json normalized_screen_size =
      nlohmann::json::parse(str).at("jData").at(0);
  normalized_screen_size.erase("id");

  str = dbserver_osd_get();
  osd_data = nlohmann::json::parse(str);
  // attribute, all OSD tables have the same attributes
  db_osd = osd_data.at(0);
  nlohmann::json attribute = db_osd;
  for (auto &x : nlohmann::json::iterator_wrapper(db_osd)) {
    if ((x.key() != "sOSDAttribute") && (x.key() != "sOSDFontSize") &&
        (x.key() != "sOSDFrontColorMode") && (x.key() != "sOSDFrontColor") &&
        (x.key() != "sAlignment") && (x.key() != "iBoundary")) {
      attribute.erase(x.key());
    }
  }
  // channel_name
  db_osd = osd_data.at(0);
  nlohmann::json channel_name = db_osd;
  for (auto &x : nlohmann::json::iterator_wrapper(db_osd)) {
    if ((x.key() != "iPositionX") && (x.key() != "iPositionY") &&
        (x.key() != "iEnabled") && (x.key() != "sDisplayText")) {
      channel_name.erase(x.key());
    }
  }
  channel_name.emplace("sChannelName", channel_name.at("sDisplayText"));
  channel_name.emplace("iChannelNameOverlayEnabled",
                       channel_name.at("iEnabled"));
  channel_name.erase("sDisplayText");
  channel_name.erase("iEnabled");
  // date_time
  db_osd = osd_data.at(1);
  nlohmann::json date_time = db_osd;
  for (auto &x : nlohmann::json::iterator_wrapper(db_osd)) {
    if ((x.key() != "iPositionX") && (x.key() != "iPositionY") &&
        (x.key() != "iEnabled") && (x.key() != "sDateStyle") &&
        (x.key() != "sTimeStyle") && (x.key() != "iDisplayWeekEnabled")) {
      date_time.erase(x.key());
    }
  }
  date_time.emplace("iDateTimeOverlayEnabled", date_time.at("iEnabled"));
  date_time.erase("iEnabled");
  // character
  nlohmann::json all_character;
  for (int i = 2; i < 10; i++) {
    db_osd = osd_data.at(i);
    nlohmann::json character = db_osd;
    for (auto &x : nlohmann::json::iterator_wrapper(db_osd)) {
      if ((x.key() != "iPositionX") && (x.key() != "iPositionY") &&
          (x.key() != "iEnabled") && (x.key() != "sDisplayText") &&
          (x.key() != "sIsPersistentText") && (x.key() != "id")) {
        character.erase(x.key());
      }
    }
    character.erase("id");
    character.emplace("id", i - 2);
    character.emplace("iTextOverlayEnabled", character.at("iEnabled"));
    character.erase("iEnabled");
    all_character.push_back(character);
  }
  // overlays
  osd_overlays.emplace("normalizedScreenSize", normalized_screen_size);
  osd_overlays.emplace("attribute", attribute);
  osd_overlays.emplace("channelNameOverlay", channel_name);
  osd_overlays.emplace("dateTimeOverlay", date_time);
  osd_overlays.emplace("characterOverlay", all_character);

  return osd_overlays;
}

nlohmann::json osd_image_get() {
  char *str;
  nlohmann::json osd_image;

  str = dbserver_media_get((char *)TABLE_NORMALIZED_SCREEN_SIZE);
  nlohmann::json normalized_screen_size =
      nlohmann::json::parse(str).at("jData").at(0);
  str = dbserver_osd_get();
  nlohmann::json db_osd = nlohmann::json::parse(str).at(14);

  nlohmann::json image_overlay = db_osd;
  normalized_screen_size.erase("id");
  for (auto &x : nlohmann::json::iterator_wrapper(db_osd)) {
    if ((x.key() != "iPositionX") && (x.key() != "iPositionY") &&
        (x.key() != "iEnabled") && (x.key() != "iTransparentColorEnabled") &&
        (x.key() != "iWidth") && (x.key() != "iHeight")) {
      image_overlay.erase(x.key());
    }
  }
  image_overlay.emplace("iImageWidth", image_overlay.at("iWidth"));
  image_overlay.emplace("iImageHeight", image_overlay.at("iHeight"));
  image_overlay.emplace("iImageOverlayEnabled", image_overlay.at("iEnabled"));
  image_overlay.erase("iWidth");
  image_overlay.erase("iHeight");
  image_overlay.erase("iEnabled");

  // get bmp picture width and height
  FILE *picture_file = fopen(BMP_FILE_PATH, "rb");
  int32_t bmp_width;
  int32_t bmp_height;
  int offset = sizeof(uint16_t) * 3 + sizeof(uint32_t) * 3;
  // skip other header information
  fseek(picture_file, offset, SEEK_SET);
  fread(&bmp_width, 1, sizeof(bmp_width), picture_file);
  fread(&bmp_height, 1, sizeof(bmp_height), picture_file);
  fclose(picture_file);
  int32_t db_image_width = image_overlay.at("iImageWidth");
  int32_t db_image_height = image_overlay.at("iImageHeight");
  if ((db_image_width != bmp_width) || (db_image_height != bmp_height)) {
    image_overlay.erase("iImageWidth");
    image_overlay.erase("iImageHeight");
    image_overlay.emplace("iImageWidth", bmp_width);
    image_overlay.emplace("iImageHeight", bmp_height);
  }

  osd_image.emplace("normalizedScreenSize", normalized_screen_size);
  osd_image.emplace("imageOverlay", image_overlay);

  return osd_image;
}

nlohmann::json osd_privacy_mask_get() {
  char *str;
  nlohmann::json osd_privacy_mask;
  nlohmann::json all_privacy_mask;

  str = dbserver_media_get((char *)TABLE_NORMALIZED_SCREEN_SIZE);
  nlohmann::json normalized_screen_size =
      nlohmann::json::parse(str).at("jData").at(0);
  normalized_screen_size.erase("id");
  str = dbserver_osd_get();
  nlohmann::json db_osd = nlohmann::json::parse(str);

  for (int i = 10; i < 14; i++) {
    nlohmann::json privacy_mask = db_osd.at(i);
    for (auto &x : nlohmann::json::iterator_wrapper(db_osd.at(i))) {
      if ((x.key() != "iPositionX") && (x.key() != "iPositionY") &&
          (x.key() != "iEnabled") && (x.key() != "id") &&
          (x.key() != "iWidth") && (x.key() != "iHeight")) {
        privacy_mask.erase(x.key());
      }
    }
    privacy_mask.erase("id");
    privacy_mask.emplace("id", i - 10);
    privacy_mask.emplace("iMaskWidth", privacy_mask.at("iWidth"));
    privacy_mask.emplace("iMaskHeight", privacy_mask.at("iHeight"));
    privacy_mask.emplace("iPrivacyMaskEnabled", privacy_mask.at("iEnabled"));
    privacy_mask.erase("iWidth");
    privacy_mask.erase("iHeight");
    privacy_mask.erase("iEnabled");
    all_privacy_mask.push_back(privacy_mask);
  }
  osd_privacy_mask.emplace("normalizedScreenSize", normalized_screen_size);
  osd_privacy_mask.emplace("privacyMask", all_privacy_mask);

  return osd_privacy_mask;
}

void osd_overlays_set(nlohmann::json osd_overlays) {
  for (auto &x : nlohmann::json::iterator_wrapper(osd_overlays)) {
    if (x.key() == KEY_NORMALIZED_SCREEN_SIZE) {
      auto val = x.value(); /* string or int */
      dbserver_media_set((char *)TABLE_NORMALIZED_SCREEN_SIZE,
                         (char *)val.dump().c_str(), 0);
    } else if (x.key() == KEY_OSD_ATTRIBUTE) {
      nlohmann::json attribute = osd_overlays.at(KEY_OSD_ATTRIBUTE);
      for (int i = 0; i < 10; i++) {
        dbserver_media_set((char *)TABLE_OSD, (char *)attribute.dump().c_str(),
                           i);
      }
    } else if (x.key() == KEY_OSD_CHANNEL_NAME_OVERLAY) {
      nlohmann::json channel_name =
          osd_overlays.at(KEY_OSD_CHANNEL_NAME_OVERLAY);
      channel_name.emplace("sDisplayText", channel_name.at("sChannelName"));
      channel_name.emplace("iEnabled",
                           channel_name.at("iChannelNameOverlayEnabled"));
      channel_name.erase("sChannelName");
      channel_name.erase("iChannelNameOverlayEnabled");
      dbserver_media_set((char *)TABLE_OSD, (char *)channel_name.dump().c_str(),
                         0);
    } else if (x.key() == KEY_OSD_DATE_TIME_OVERLAY) {
      nlohmann::json date_time = osd_overlays.at(KEY_OSD_DATE_TIME_OVERLAY);
      date_time.emplace("iEnabled", date_time.at("iDateTimeOverlayEnabled"));
      date_time.erase("iDateTimeOverlayEnabled");
      dbserver_media_set((char *)TABLE_OSD, (char *)date_time.dump().c_str(),
                         1);
    } else if (x.key() == KEY_OSD_CHARACTER_OVERLAY) {
      nlohmann::json all_character = osd_overlays.at(KEY_OSD_CHARACTER_OVERLAY);
      for (auto &x : nlohmann::json::iterator_wrapper(all_character)) {
        nlohmann::json character = x.value();
        int id = character.at("id");
        character.erase("id");
        character.emplace("id", id + 2);
        character.emplace("iEnabled", character.at("iTextOverlayEnabled"));
        character.erase("iTextOverlayEnabled");
        dbserver_media_set((char *)TABLE_OSD, (char *)character.dump().c_str(),
                           id + 2);
      }
    }
  }
}

void osd_image_set(nlohmann::json osd_image_config) {
  // get bmp picture width and height
  FILE *picture_file = fopen(BMP_FILE_PATH, "rb");
  int32_t bmp_width;
  int32_t bmp_height;
  int offset = sizeof(uint16_t) * 3 + sizeof(uint32_t) * 3;
  // skip other header information
  fseek(picture_file, offset, SEEK_SET);
  fread(&bmp_width, 1, sizeof(bmp_width), picture_file);
  fread(&bmp_height, 1, sizeof(bmp_height), picture_file);
  fclose(picture_file);

  for (auto &x : nlohmann::json::iterator_wrapper(osd_image_config)) {
    if (x.key() == KEY_NORMALIZED_SCREEN_SIZE) {
      auto val = x.value(); /* string or int */
      dbserver_media_set((char *)TABLE_NORMALIZED_SCREEN_SIZE,
                         (char *)val.dump().c_str(), 0);
    } else if (x.key() == KEY_OSD_IMAGE_OVERLAY) {
      nlohmann::json image_overlay = osd_image_config.at(KEY_OSD_IMAGE_OVERLAY);
      image_overlay.emplace("iWidth", bmp_width);
      image_overlay.emplace("iHeight", bmp_height);
      image_overlay.emplace("iEnabled",
                            image_overlay.at("iImageOverlayEnabled"));
      image_overlay.erase("iImageWidth");
      image_overlay.erase("iImageHeight");
      image_overlay.erase("iImageOverlayEnabled");
      dbserver_media_set((char *)TABLE_OSD,
                         (char *)image_overlay.dump().c_str(), 14);
    }
  }
}

void osd_privacy_mask_set(nlohmann::json osd_privacy_mask_config) {
  for (auto &x : nlohmann::json::iterator_wrapper(osd_privacy_mask_config)) {
    if (x.key() == KEY_NORMALIZED_SCREEN_SIZE) {
      auto val = x.value(); /* string or int */
      dbserver_media_set((char *)TABLE_NORMALIZED_SCREEN_SIZE,
                         (char *)val.dump().c_str(), 0);
    } else if (x.key() == KEY_OSD_PRIVACY_MASK) {
      nlohmann::json all_privacy_mask =
          osd_privacy_mask_config.at(KEY_OSD_PRIVACY_MASK);
      for (auto &x : nlohmann::json::iterator_wrapper(all_privacy_mask)) {
        nlohmann::json privacy_mask = x.value();
        int id = privacy_mask.at("id");
        privacy_mask.erase("id");
        privacy_mask.emplace("id", id + 10);
        privacy_mask.emplace("iWidth", privacy_mask.at("iMaskWidth"));
        privacy_mask.emplace("iHeight", privacy_mask.at("iMaskHeight"));
        privacy_mask.emplace("iEnabled",
                             privacy_mask.at("iPrivacyMaskEnabled"));
        privacy_mask.erase("iMaskWidth");
        privacy_mask.erase("iMaskHeight");
        privacy_mask.erase("iPrivacyMaskEnabled");
        dbserver_media_set((char *)TABLE_OSD,
                           (char *)privacy_mask.dump().c_str(), id + 10);
      }
    }
  }
}

void osd_set_mediaserver(nlohmann::json osd_change) {
  char *str = dbserver_osd_get();
  nlohmann::json db_osd = nlohmann::json::parse(str);

  for (auto &x : nlohmann::json::iterator_wrapper(osd_change)) {
    if (x.key() == KEY_OSD_ATTRIBUTE) {
      for (int i = OSD_DB_REGION_ID_CHANNEL; i <= OSD_DB_REGION_ID_TEXT7; i++) {
        nlohmann::json config = db_osd.at(i);
        mediaserver_set((char *)TABLE_OSD, i, (char *)config.dump().c_str());
      }
    } else if (x.key() == KEY_OSD_DATE_TIME_OVERLAY) {
      nlohmann::json config = db_osd.at(OSD_DB_REGION_ID_DATETIME);
      mediaserver_set((char *)TABLE_OSD, OSD_DB_REGION_ID_DATETIME,
                      (char *)config.dump().c_str());
    } else if (x.key() == KEY_OSD_CHANNEL_NAME_OVERLAY) {
      nlohmann::json config = db_osd.at(OSD_DB_REGION_ID_CHANNEL);
      mediaserver_set((char *)TABLE_OSD, OSD_DB_REGION_ID_CHANNEL,
                      (char *)config.dump().c_str());
    } else if (x.key() == KEY_OSD_CHARACTER_OVERLAY) {
      for (int i = OSD_DB_REGION_ID_TEXT0; i <= OSD_DB_REGION_ID_TEXT7; i++) {
        nlohmann::json config = db_osd.at(i);
        mediaserver_set((char *)TABLE_OSD, i, (char *)config.dump().c_str());
      }
    } else if (x.key() == KEY_OSD_PRIVACY_MASK) {
      for (int i = OSD_DB_REGION_ID_MASK0; i <= OSD_DB_REGION_ID_MASK3; i++) {
        nlohmann::json config = db_osd.at(i);
        mediaserver_set((char *)TABLE_OSD, i, (char *)config.dump().c_str());
      }
    } else if (x.key() == KEY_OSD_IMAGE_OVERLAY) {
      nlohmann::json config = db_osd.at(OSD_DB_REGION_ID_IMAGE);
      mediaserver_set((char *)TABLE_OSD, OSD_DB_REGION_ID_IMAGE,
                      (char *)config.dump().c_str());
    }
  }
}

void OSDApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  std::string path_api_resource;
  std::string path_specific_resource;
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  /* Get Path Information */
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_specific_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
  }

  if (Req.Method == "GET") {
    if (!path_specific_resource.compare("overlays")) {
      content = osd_overlays_get();
    } else if (!path_specific_resource.compare("image")) {
      content = osd_image_get();
    } else if (!path_specific_resource.compare("privacy-mask")) {
      content = osd_privacy_mask_get();
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
#ifdef ENABLE_JWT
    if (user_level > 1) {
      Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
      return;
    }
#endif
    nlohmann::json diff;
    nlohmann::json cfg_old;
    nlohmann::json osd_config = Req.PostObject; /* must be json::object */
    if (!path_specific_resource.compare("overlays")) {
      /* Erase unchanged data */
      cfg_old = osd_overlays_get();
      diff = nlohmann::json::diff(cfg_old, osd_config);
      for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          osd_config.erase(x.key());
      }
      /* Set */
      if (!osd_config.empty())
        osd_overlays_set(osd_config);
      /* Get new info */
      content = osd_overlays_get();
      osd_set_mediaserver(osd_config);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("image")) {
      /* Erase unchanged data */
      cfg_old = osd_image_get();
      diff = nlohmann::json::diff(cfg_old, osd_config);
      for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          osd_config.erase(x.key());
      }
      /* Set */
      if (!osd_config.empty())
        osd_image_set(osd_config);
      /* Get new info */
      content = osd_image_get();
      osd_set_mediaserver(osd_config);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("privacy-mask")) {
      /* Erase unchanged data */
      cfg_old = osd_privacy_mask_get();
      diff = nlohmann::json::diff(cfg_old, osd_config);
      for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          osd_config.erase(x.key());
      }
      /* Set */
      if (!osd_config.empty())
        osd_privacy_mask_set(osd_config);
      /* Get new info */
      content = osd_privacy_mask_get();
      osd_set_mediaserver(osd_config);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("image/picture")) {
      for (int i = 0; i < Req.Files.size(); i++) {
        if (!Req.Files.at(i).getDataType().compare("image/bmp")) {
          if (Req.Files.at(i).getDataLength() > (256 * 1024)) { // 256kb
            Resp.setErrorResponse(HttpStatus::kBadRequest,
                                  "Picture is too big!");
          } else {
            int end_position = 0;
            std::ofstream picture_file(BMP_FILE_PATH, std::ofstream::out);
            picture_file << Req.Files.at(i).getData();
            end_position = picture_file.tellp();
            picture_file.close();
            if (end_position == Req.Files.at(i).getDataLength()) {
              Resp.setHeader(HttpStatus::kOk, "OK");
            } else {
              Resp.setErrorResponse(HttpStatus::kBadRequest,
                                    "Picture upload failed!");
            }
          }
          i = Req.Files.size();
        } else if ((i == Req.Files.size() - 1) &&
                   (Req.Files.at(i).getDataType().compare("image/bmp"))) {
          Resp.setErrorResponse(HttpStatus::kBadRequest, "Can't find bmp!");
        }
      }
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#endif

} // namespace cgi
} // namespace rockchip
