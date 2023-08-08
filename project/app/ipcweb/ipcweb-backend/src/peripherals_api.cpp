// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "peripherals_api.h"
#include "common.h"

namespace rockchip {
namespace cgi {

#ifdef USE_RKIPC

void PeripheralsApiHandler::handler(const HttpRequest &Req,
                                    HttpResponse &Resp) {
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif
  if (Req.Method == "GET") {
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
#ifdef ENABLE_JWT
    if (user_level > 1) {
      Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
      return;
    }
#endif
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else // #ifdef USE_RKIPC

void PeripheralsApiHandler::handler(const HttpRequest &Req,
                                    HttpResponse &Resp) {
  std::string path_api_resource;
  std::string path_specific_resource;
  char *str;
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  // Get Path Information
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_specific_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
  }

  if (Req.Method == "GET") {
    if (path_specific_resource.find("gate") != std::string::npos) {
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_RELAY);
      nlohmann::json relay = nlohmann::json::parse(str).at("jData").at(0);
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_WEIGEN);
      nlohmann::json weigen = nlohmann::json::parse(str).at("jData").at(0);
      content.emplace("relay", relay);
      content.emplace("weigen", weigen);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("fill-light") != std::string::npos) {
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_FILL_LIGHT);
      content = nlohmann::json::parse(str).at("jData").at(0);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
#ifdef ENABLE_JWT
    if (user_level > 1) {
      Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
      return;
    }
#endif
    nlohmann::json peripherals_config =
        Req.PostObject; /* must be json::object */

    if (path_specific_resource.find("gate") != std::string::npos) {
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_RELAY);
      nlohmann::json old_relay = nlohmann::json::parse(str).at("jData").at(0);
      nlohmann::json relay_diff =
          nlohmann::json::diff(old_relay, peripherals_config.at("relay"));
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_WEIGEN);
      nlohmann::json old_weigen = nlohmann::json::parse(str).at("jData").at(0);
      nlohmann::json weigen_diff =
          nlohmann::json::diff(old_weigen, peripherals_config.at("weigen"));
      for (auto &x : nlohmann::json::iterator_wrapper(old_relay)) {
        if (relay_diff.dump().find(x.key()) == relay_diff.dump().npos)
          peripherals_config.at("relay").erase(x.key());
      }
      for (auto &x : nlohmann::json::iterator_wrapper(old_weigen)) {
        if (weigen_diff.dump().find(x.key()) == weigen_diff.dump().npos)
          peripherals_config.at("weigen").erase(x.key());
      }
      if (!peripherals_config.at("relay").empty())
        dbserver_peripherals_set(
            (char *)TABLE_PERIPHERALS_RELAY,
            (char *)peripherals_config.at("relay").dump().c_str(), 0);
      if (!peripherals_config.at("weigen").empty())
        dbserver_peripherals_set(
            (char *)TABLE_PERIPHERALS_WEIGEN,
            (char *)peripherals_config.at("weigen").dump().c_str(), 0);
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_RELAY);
      nlohmann::json relay = nlohmann::json::parse(str).at("jData").at(0);
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_WEIGEN);
      nlohmann::json weigen = nlohmann::json::parse(str).at("jData").at(0);
      content.emplace("relay", relay);
      content.emplace("weigen", weigen);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("fill-light") != std::string::npos) {
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_FILL_LIGHT);
      nlohmann::json old_cfg = nlohmann::json::parse(str).at("jData").at(0);
      nlohmann::json diff = nlohmann::json::diff(old_cfg, peripherals_config);
      for (auto &x : nlohmann::json::iterator_wrapper(old_cfg)) {
        if (diff.dump().find(x.key()) == diff.dump().npos)
          peripherals_config.erase(x.key());
      }
      if (!peripherals_config.empty())
        dbserver_peripherals_set((char *)TABLE_PERIPHERALS_FILL_LIGHT,
                                 (char *)peripherals_config.dump().c_str(), 0);
      str = dbserver_peripherals_get((char *)TABLE_PERIPHERALS_FILL_LIGHT);
      content = nlohmann::json::parse(str).at("jData").at(0);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#endif

} // namespace cgi
} // namespace rockchip
