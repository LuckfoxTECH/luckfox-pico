// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "audio_api.h"
#include "common.h"

namespace rockchip {
namespace cgi {

#ifdef USE_RKIPC

nlohmann::json audio_get_param(int stream_id) {
  nlohmann::json param;
  param.emplace("id", stream_id);

  char *tmp = new char[256];

  int value = 0;

  rk_audio_get_bit_rate(stream_id, &value);
  param.emplace("iBitRate", value);

  rk_audio_get_sample_rate(stream_id, &value);
  param.emplace("iSampleRate", value);

  rk_audio_get_volume(stream_id, &value);
  param.emplace("iVolume", value);

  rk_audio_get_enable_vqe(stream_id, &value);
  if (value)
    param.emplace("sVQE", "open");
  else
    param.emplace("sVQE", "close");

  rk_audio_get_encode_type(stream_id, &tmp);
  param.emplace("sEncodeType", tmp);
  delete[] tmp;

  param.emplace("sInput", "micIn");

  return param;
}

int audio_set_param(int stream_id, nlohmann::json param) {
  int value_int;
  std::string value;

  if (param.dump().find("iBitRate") != param.dump().npos) {
    value_int = atoi(param.at("iBitRate").dump().c_str());
    rk_audio_set_bit_rate(stream_id, value_int);
  }
  if (param.dump().find("iSampleRate") != param.dump().npos) {
    value_int = atoi(param.at("iSampleRate").dump().c_str());
    rk_audio_set_sample_rate(stream_id, value_int);
  }
  if (param.dump().find("iVolume") != param.dump().npos) {
    value_int = atoi(param.at("iVolume").dump().c_str());
    rk_audio_set_volume(stream_id, value_int);
  }
  if (param.dump().find("sVQE") != param.dump().npos) {
    value = param.at("sVQE").dump();
    value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
    if (!strcmp(value.c_str(), "open"))
      rk_audio_set_enable_vqe(stream_id, 1);
    else
      rk_audio_set_enable_vqe(stream_id, 0);
  }
  if (param.dump().find("sEncodeType") != param.dump().npos) {
    value = param.at("sEncodeType").dump();
    value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
    rk_audio_set_encode_type(stream_id, value.c_str());
  }
  rk_audio_restart();

  return 0;
}

void AudioApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif
  if (Req.Method == "GET") {
    content = audio_get_param(0);
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
#ifdef ENABLE_JWT
    if (user_level > 1) {
      Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
      return;
    }
#endif
    nlohmann::json audio_config = Req.PostObject; /* must be json::object */
    /* Erase unchanged data */
    nlohmann::json cfg_old = audio_get_param(0);
    nlohmann::json diff = nlohmann::json::diff(cfg_old, audio_config);
    for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
      if (diff.dump().find(x.key()) == diff.dump().npos)
        audio_config.erase(x.key());
    }

    /* set */
    if (!audio_config.empty())
      audio_set_param(0, audio_config);

    /* get new info */
    content = audio_get_param(0);
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else // USE_RKIPC

static void audio_setting_item_set(nlohmann::json config, int id) {
  if (!config.empty()) {
    nlohmann::json setting_config = config;
    if (setting_config.dump().find("id") != setting_config.dump().npos)
      setting_config.erase("id");
    mediaserver_set((char *)TABLE_AUDIO, id,
                    (char *)setting_config.dump().c_str());
  }
}

void AudioApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif
  if (Req.Method == "GET") {
    nlohmann::json content;
    char *str = dbserver_audio_get();
    nlohmann::json audio_config = nlohmann::json::parse(str).at("jData");
    int pos_first = Req.PathInfo.find_first_of("/");
    int pos_last = Req.PathInfo.find_last_of("/");

    if (pos_first != pos_last)
      content = audio_config.at(0);
    else
      content = audio_config;

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
#ifdef ENABLE_JWT
    if (user_level > 1) {
      Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
      return;
    }
#endif
    nlohmann::json content = nlohmann::json::array();
    nlohmann::json audio_config = Req.PostObject; /* must be json::object */
    int id = 0;
    int pos_first = Req.PathInfo.find_first_of("/");
    int pos_last = Req.PathInfo.find_last_of("/");

    if (pos_first != pos_last)
      id = atoi(Req.PathInfo.substr(pos_last + 1, Req.PathInfo.size()).c_str());

    /* Erase unchanged data */
    char *prev = dbserver_audio_get();
    nlohmann::json cfg_old = nlohmann::json::parse(prev).at("jData");
    nlohmann::json diff = nlohmann::json::diff(cfg_old.at(id), audio_config);
    for (auto &x : nlohmann::json::iterator_wrapper(cfg_old.at(id))) {
      if (diff.dump().find(x.key()) == diff.dump().npos)
        audio_config.erase(x.key());
    }

    /* set */
    if (!audio_config.empty())
      dbserver_audio_set((char *)audio_config.dump().c_str());
    audio_setting_item_set(audio_config, id);

    /* get new info */
    char *str = dbserver_audio_get();
    audio_config = nlohmann::json::parse(str).at("jData");
    content = audio_config.at(id);
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#endif

} // namespace cgi
} // namespace rockchip
