// Copyright 2019-2022 Fuzhou Rockchip Electronics Co., Ltd. All rights
// reserved. Use of this source code is governed by a BSD-style license that can
// be found in the LICENSE file.

#include "roi_api.h"
#include "common.h"

namespace rockchip {
namespace cgi {

#ifdef USE_RKIPC

nlohmann::json roi_info_get() {
  int value;
  char *str;
  char *tmp = new char[256];
  nlohmann::json roi_info;
  nlohmann::json normalized_screen_size;

  rk_osd_get_normalized_screen_width(&value);
  normalized_screen_size.emplace("iNormalizedScreenWidth", value);
  rk_osd_get_normalized_screen_height(&value);
  normalized_screen_size.emplace("iNormalizedScreenHeight", value);

  // roi_region_list
  nlohmann::json roi_region_list;
  for (int i = 0; i < 6; i++) {
    nlohmann::json roi_region;
    rk_roi_get_position_x(i, &value);
    roi_region.emplace("iPositionX", value);
    rk_roi_get_position_y(i, &value);
    roi_region.emplace("iPositionY", value);
    rk_roi_get_width(i, &value);
    roi_region.emplace("iWidth", value);
    rk_roi_get_height(i, &value);
    roi_region.emplace("iHeight", value);
    rk_roi_get_id(i, &value);
    roi_region.emplace("iROIId", value);
    rk_roi_get_quality_level(i, &value);
    roi_region.emplace("iQualityLevelOfROI", value);
    rk_roi_get_enabled(i, &value);
    roi_region.emplace("iROIEnabled", value);
    roi_region.emplace("iStreamEnabled", 0);
    rk_roi_get_stream_type(i, &tmp);
    roi_region.emplace("sStreamType", tmp);
    rk_roi_get_name(i, &tmp);
    roi_region.emplace("sName", tmp);
    roi_region_list.push_back(roi_region);
  }
  // roi_info
  roi_info.emplace("normalizedScreenSize", normalized_screen_size);
  roi_info.emplace("ROIRegionList", roi_region_list);

  delete[] tmp;

  return roi_info;
}

nlohmann::json roi_specific_get(std::string stream_type, int roi_id) {
  int actual_id, value;
  char *tmp = new char[256];
  nlohmann::json roi_region;

  if (!stream_type.compare("main-stream")) {
    if (roi_id == 1)
      actual_id = 0;
    else if (roi_id == 2)
      actual_id = 1;
  } else if (!stream_type.compare("sub-stream")) {
    if (roi_id == 1)
      actual_id = 2;
    else if (roi_id == 2)
      actual_id = 3;
  } else if (!stream_type.compare("third-stream")) {
    if (roi_id == 1)
      actual_id = 4;
    else if (roi_id == 2)
      actual_id = 5;
  }
  rk_roi_get_position_x(actual_id, &value);
  roi_region.emplace("iPositionX", value);
  rk_roi_get_position_y(actual_id, &value);
  roi_region.emplace("iPositionY", value);
  rk_roi_get_width(actual_id, &value);
  roi_region.emplace("iWidth", value);
  rk_roi_get_height(actual_id, &value);
  roi_region.emplace("iHeight", value);
  rk_roi_get_id(actual_id, &value);
  roi_region.emplace("iROIId", value);
  rk_roi_get_quality_level(actual_id, &value);
  roi_region.emplace("iQualityLevelOfROI", value);
  rk_roi_get_enabled(actual_id, &value);
  roi_region.emplace("iROIEnabled", value);
  roi_region.emplace("iStreamEnabled", 0);
  rk_roi_get_stream_type(actual_id, &tmp);
  roi_region.emplace("sStreamType", tmp);
  rk_roi_get_name(actual_id, &tmp);
  roi_region.emplace("sName", tmp);

  return roi_region;
}

void roi_specific_set(nlohmann::json roi_config, std::string stream_type,
                      int roi_id) {
  int actual_id;
  int value_int;
  std::string value;

  if (!stream_type.compare("main-stream")) {
    if (roi_id == 1)
      actual_id = 0;
    else if (roi_id == 2)
      actual_id = 1;
  } else if (!stream_type.compare("sub-stream")) {
    if (roi_id == 1)
      actual_id = 2;
    else if (roi_id == 2)
      actual_id = 3;
  } else if (!stream_type.compare("third-stream")) {
    if (roi_id == 1)
      actual_id = 4;
    else if (roi_id == 2)
      actual_id = 5;
  }
  if (roi_config.dump().find("sStreamType") != roi_config.dump().npos) {
    value = roi_config.at("sStreamType").dump();
    value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
    rk_roi_set_stream_type(actual_id, value.c_str());
  }
  if (roi_config.dump().find("sName") != roi_config.dump().npos) {
    value = roi_config.at("sName").dump();
    value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
    rk_roi_set_name(actual_id, value.c_str());
  }
  if (roi_config.dump().find("iROIEnabled") != roi_config.dump().npos) {
    value_int = atoi(roi_config.at("iROIEnabled").dump().c_str());
    rk_roi_set_enabled(actual_id, value_int);
  }
  if (roi_config.dump().find("iPositionX") != roi_config.dump().npos) {
    value_int = atoi(roi_config.at("iPositionX").dump().c_str());
    rk_roi_set_position_x(actual_id, value_int);
  }
  if (roi_config.dump().find("iPositionY") != roi_config.dump().npos) {
    value_int = atoi(roi_config.at("iPositionY").dump().c_str());
    rk_roi_set_position_y(actual_id, value_int);
  }
  if (roi_config.dump().find("iHeight") != roi_config.dump().npos) {
    value_int = atoi(roi_config.at("iHeight").dump().c_str());
    rk_roi_set_height(actual_id, value_int);
  }
  if (roi_config.dump().find("iWidth") != roi_config.dump().npos) {
    value_int = atoi(roi_config.at("iWidth").dump().c_str());
    rk_roi_set_width(actual_id, value_int);
  }
  if (roi_config.dump().find("iQualityLevelOfROI") != roi_config.dump().npos) {
    value_int = atoi(roi_config.at("iQualityLevelOfROI").dump().c_str());
    rk_roi_set_quality_level(actual_id, value_int);
  }
  rk_roi_set_all();
}

void ROIApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  std::string path_api_resource;
  std::string path_channel_resource;
  std::string path_specific_resource;
  char *str;
  nlohmann::json content;
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
    if (pos_first != -1) {
      path_specific_resource = path_channel_resource.substr(
          pos_first + 1, path_channel_resource.size());
    }
  }
  if (Req.Method == "GET") {
    if (path_channel_resource.empty()) { // path is roi
      content = roi_info_get();
    } else {
      if (path_specific_resource.empty()) { // path example is roi/main-stream
        // content = roi_channel_get(path_channel_resource);
        // for (auto &x : nlohmann::json::iterator_wrapper(content)) {
        //   x.value().erase("id");
        // }
      } else { // path example is roi/main-stream/1
        int roi_id = stoi(path_specific_resource);
        int pos = path_channel_resource.find_first_of("/");
        std::string stream_type = path_channel_resource.substr(0, pos);
        content = roi_specific_get(stream_type, roi_id);
        content.erase("id");
      }
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    if (path_channel_resource.empty()) { // path is roi
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    } else {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_specific_resource.empty()) { // path example is roi/main-stream
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else { // path example is roi/main-stream/1
        nlohmann::json roi_config = Req.PostObject; /* must be json::object */
        int roi_id = stoi(path_specific_resource);
        int pos = path_channel_resource.find_first_of("/");
        std::string stream_type = path_channel_resource.substr(0, pos);
        /* Set */
        if (!roi_config.empty())
          roi_specific_set(roi_config, stream_type, roi_id);
        /* Get new info */
        content = roi_specific_get(stream_type, roi_id);

        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else // #ifdef USE_RKIPC

nlohmann::json roi_all_info_get(void) {
  nlohmann::json roi_all_info;
  char *str = dbserver_media_get((char *)TABLE_NORMALIZED_SCREEN_SIZE);
  nlohmann::json normalized_screen_size =
      nlohmann::json::parse(str).at("jData").at(0);
  normalized_screen_size.erase("id");
  str = dbserver_media_get((char *)TABLE_ROI);
  nlohmann::json roi_list = nlohmann::json::parse(str).at("jData");
  for (auto &x : nlohmann::json::iterator_wrapper(roi_list)) {
    x.value().erase("id");
  }
  roi_all_info.emplace("normalizedScreenSize", normalized_screen_size);
  roi_all_info.emplace("ROIRegionList", roi_list);

  return roi_all_info;
}

nlohmann::json roi_channel_get(std::string stream_type) {
  char *str;
  std::string stream_type_db;
  nlohmann::json roi_channel;
  nlohmann::json roi_list;

  str = dbserver_media_get((char *)TABLE_ROI);
  roi_list = nlohmann::json::parse(str).at("jData");
  if (!stream_type.compare("main-stream"))
    stream_type_db = "mainStream";
  else if (!stream_type.compare("sub-stream"))
    stream_type_db = "subStream";
  else if (!stream_type.compare("third-stream"))
    stream_type_db = "thirdStream";

  for (auto &x : nlohmann::json::iterator_wrapper(roi_list)) {
    std::string actual_type = x.value().at("sStreamType");
    if (!actual_type.compare(stream_type_db))
      roi_channel.push_back(x.value());
  }

  return roi_channel;
}

nlohmann::json roi_specific_get(std::string stream_type, int roi_id) {
  nlohmann::json roi_channel;
  nlohmann::json roi_specific;
  nlohmann::json stream_enable_config;
  int roi_enable_num = 0;
  int stream_enable_num;

  roi_channel = roi_channel_get(stream_type);
  for (auto &x : nlohmann::json::iterator_wrapper(roi_channel)) {
    int actual_id = x.value().at("iROIId");
    if (actual_id == roi_id)
      roi_specific = x.value();
  }

  // Check the stream enable state
  char *config = new char[255];
  for (auto &x : nlohmann::json::iterator_wrapper(roi_channel)) {
    int roi_enbale = x.value().at("iROIEnabled");
    roi_enable_num += roi_enbale;
  }
  if (roi_enable_num == 0) {
    // if this channel all roi enable is 0, that all stream enable should 0
    stream_enable_num = 0;
  } else {
    // if this channel have one roi enable is 1, that all stream enable should 1
    stream_enable_num = 1;
  }
  stream_enable_config.emplace("iStreamEnabled", stream_enable_num);
  strcpy(config, stream_enable_config.dump().c_str());
  for (auto &x : nlohmann::json::iterator_wrapper(roi_channel)) {
    int table_id = x.value().at("id");
    dbserver_media_set((char *)TABLE_ROI, config, table_id);
  }
  delete[] config;

  return roi_specific;
}

void roi_specific_set(nlohmann::json roi_config, std::string stream_type,
                      int roi_id) {
  int table_id;
  int stream_id;
  char *config = new char[255];
  std::string stream_type_db;
  nlohmann::json old_roi_specific;
  nlohmann::json roi_channel = roi_channel_get(stream_type);

  // Prohibit modification
  roi_config.erase("sStreamType");
  roi_config.erase("iStreamEnabled");

  if (!stream_type.compare("main-stream")) {
    stream_type_db = "mainStream";
    stream_id = 0;
  } else if (!stream_type.compare("sub-stream")) {
    stream_type_db = "subStream";
    stream_id = 1;
  } else if (!stream_type.compare("third-stream")) {
    stream_type_db = "thirdStream";
    stream_id = 2;
  }
  roi_config.emplace("sStreamType", stream_type_db);

  old_roi_specific = roi_specific_get(stream_type, roi_id);
  table_id = old_roi_specific.at("id");
  strcpy(config, roi_config.dump().c_str());
  dbserver_media_set((char *)TABLE_ROI, config, table_id);
  mediaserver_set((char *)TABLE_ROI, stream_id, config);
  delete[] config;
}

void ROIApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  std::string path_api_resource;
  std::string path_channel_resource;
  std::string path_specific_resource;
  char *str;
  nlohmann::json content;
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
    if (pos_first != -1) {
      path_specific_resource = path_channel_resource.substr(
          pos_first + 1, path_channel_resource.size());
    }
  }

  if (Req.Method == "GET") {
    if (path_channel_resource.empty()) { // path is roi
      content = roi_all_info_get();
    } else {
      if (path_specific_resource.empty()) { // path example is roi/main-stream
        content = roi_channel_get(path_channel_resource);
        for (auto &x : nlohmann::json::iterator_wrapper(content)) {
          x.value().erase("id");
        }
      } else { // path example is roi/main-stream/1
        int roi_id = stoi(path_specific_resource);
        int pos = path_channel_resource.find_first_of("/");
        std::string stream_type = path_channel_resource.substr(0, pos);
        content = roi_specific_get(stream_type, roi_id);
        content.erase("id");
      }
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    if (path_channel_resource.empty()) { // path is roi
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    } else {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_specific_resource.empty()) { // path example is roi/main-stream
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else { // path example is roi/main-stream/1
        nlohmann::json roi_config = Req.PostObject; /* must be json::object */
        int roi_id = stoi(path_specific_resource);
        int pos = path_channel_resource.find_first_of("/");
        std::string stream_type = path_channel_resource.substr(0, pos);
        /* Set */
        if (!roi_config.empty())
          roi_specific_set(roi_config, stream_type, roi_id);
        /* Get new info */
        content = roi_specific_get(stream_type, roi_id);

        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#endif

} // namespace cgi
} // namespace rockchip
