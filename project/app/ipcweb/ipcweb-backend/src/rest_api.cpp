// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "rest_api.h"

#include <functional>
#include <sstream>

#include "audio_api.h"
#include "cgicc/Cgicc.h"
#include "common.h"
#include "event_api.h"
#include "image_api.h"
#include "network_api.h"
#include "osd_api.h"
#include "peripherals_api.h"
#include "roi_api.h"
#include "storage_api.h"
#include "stream_api.h"
#include "system_api.h"
#include "video_api.h"

namespace rockchip {
namespace cgi {
inline namespace detail {

std::vector<QueryParam> parseParams(const std::string &QueryString) {
  const std::string ParamDelimiter = "&";
  const std::string ValueDelimiter = "=";
  std::vector<QueryParam> Params;
  QueryParam Param;
  size_t PosStart = 0;
  size_t PosEnd;
  size_t ValStart;
  size_t DelimLen = ParamDelimiter.length();
  std::string Token;

  while ((PosEnd = QueryString.find(ParamDelimiter, PosStart)) !=
         std::string::npos) {
    Token = QueryString.substr(PosStart, PosEnd - PosStart);
    PosStart = PosEnd + DelimLen;
    if ((ValStart = Token.find(ValueDelimiter, 0)) != std::string::npos) {
      Param.Key = Token.substr(0, ValStart);
      ValStart += ValueDelimiter.length();
      Param.Value = Token.substr(ValStart);
      Params.push_back(Param);
    }
  }
  Token = QueryString.substr(PosStart);
  if ((ValStart = Token.find(ValueDelimiter, 0)) != std::string::npos) {
    Param.Key = Token.substr(0, ValStart);
    ValStart += ValueDelimiter.length();
    Param.Value = Token.substr(ValStart);
    Params.push_back(Param);
  }

  return Params;
}
#ifndef USE_RKIPC
#ifdef ENABLE_JWT
std::string checkToken(HttpRequest &Req) {
  if (Req.PathInfo.compare("/system/login") &&
      Req.PathInfo.find("system/para/webPage") == std::string::npos) {
    // in addition to the /system/login and /system/para, all tokens are
    // verified
    if (Req.Cookies.empty())
      throw jwt::token_verification_exception("not found cookie");
    std::string cookie = Req.Cookies;
    int pos = cookie.find("token=");
    cookie = cookie.substr(pos + 6);
    int pos_end = cookie.find(";");
    std::string token;
    if (pos_end == -1)
      token = cookie;
    else
      token = cookie.substr(0, pos_end);
    std::string new_token = jwt_token_verify(token, Req);
    return new_token;
  }
  return "";
}
#endif
#endif
HttpStatus parseRequest(HttpRequest &Req) {
  cgicc::Cgicc cgi;
  Req.ScriptName = cgi.getEnvironment().getScriptName();
  Req.PathInfo = cgi.getEnvironment().getPathInfo();
  Req.Cookies = cgi.getEnvironment().getCookies();
  assert(!Req.PathInfo.empty());

  int pos_first = Req.PathInfo.find_first_of("/");
  int pos_last = Req.PathInfo.find_last_of("/");
  if (pos_first != pos_last) {
    std::string path = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
    int pos_second = path.find_first_of("/");
    Req.Api = Req.PathInfo.substr(pos_first, pos_second + 1).c_str();
  } else {
    Req.Api = Req.PathInfo;
  }

  Req.AuthMethod = cgi.getEnvironment().getAuthType();
  Req.Method = cgi.getEnvironment().getRequestMethod();
  if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    Req.Params = parseParams(cgi.getEnvironment().getQueryString());
    Req.ContentLength = cgi.getEnvironment().getContentLength();
    if (Req.ContentLength > 0) {
      Req.ContentType = cgi.getEnvironment().getContentType();
      if (std::string::npos != Req.ContentType.find("application/json")) {
        std::string PostData = cgi.getEnvironment().getPostData();
        Req.PostObject = nlohmann::json::parse(PostData);
      } else if (std::string::npos !=
                 Req.ContentType.find("multipart/form-data")) {
        Req.Files = cgi.getFiles();
      } else if (std::string::npos != Req.ContentType.find("text/plain")) {
        Req.PostData = cgi.getEnvironment().getPostData();
      } else {
        return HttpStatus::kBadRequest;
      }
    }
  } else if (Req.Method == "GET") {
    Req.Params = parseParams(cgi.getEnvironment().getQueryString());
  } else if (Req.Method == "DELETE") {
    Req.Params = parseParams(cgi.getEnvironment().getQueryString());
  } else {
    return HttpStatus::kBadRequest;
  }
  return HttpStatus::kOk;
}

} // namespace detail

void ApiEntry::run() {

  using namespace std::placeholders;

  // A workaround for unexpected log messages in linked libraries
  // TODO: Find a better way for this feature requirement
  // APPLICATION NOTES:
  //   1. Don't use std::cerr, it will sync C stderr
  //   2. Don't use fprintf(stderr/stdout...)/printf
  //   3. Alternative logging library in this application:
  //     #include <minilogger/log.h>

  std::ios::sync_with_stdio(false);

  HttpRequest Req;
  HttpResponse Resp;
  const std::string network_api = "network";
  const std::string network_ntp_api = "network-ntp";
  const std::string network_ddns_api = "network-ddns";
  const std::string network_pppoe_api = "network-pppoe";
  const std::string network_port_api = "network-port";
  const std::string storage_api = "storage";
  const std::string video_api = "video";
  const std::string audio_api = "audio";
  const std::string stream_url_api = "stream-url";
  const std::string image_api = "image";
  const std::string system_api = "system";
  const std::string osd_api = "osd";
  const std::string roi_api = "roi";
  const std::string event_api = "event";
  const std::string peripherals_api = "peripherals";

  HandlerEntry e;
  e.Api = network_api;
  e.handler =
      std::bind(&NetworkApiHandler::handler, NetworkApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = network_ntp_api;
  e.handler =
      std::bind(&NetworkNTPApiHandler::handler, NetworkNTPApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = network_ddns_api;
  e.handler = std::bind(&NetworkDDNSApiHandler::handler,
                        NetworkDDNSApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = network_pppoe_api;
  e.handler = std::bind(&NetworkPPPoEApiHandler::handler,
                        NetworkPPPoEApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = network_port_api;
  e.handler = std::bind(&NetworkPortApiHandler::handler,
                        NetworkPortApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = storage_api;
  e.handler =
      std::bind(&StorageApiHandler::handler, StorageApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = video_api;
  e.handler = std::bind(&VideoApiHandler::handler, VideoApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = audio_api;
  e.handler = std::bind(&AudioApiHandler::handler, AudioApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = stream_url_api;
  e.handler =
      std::bind(&StreamURLApiHandler::handler, StreamURLApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = image_api;
  e.handler = std::bind(&ImageApiHandler::handler, ImageApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = system_api;
  e.handler = std::bind(&SystemApiHandler::handler, SystemApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = osd_api;
  e.handler = std::bind(&OSDApiHandler::handler, OSDApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = roi_api;
  e.handler = std::bind(&ROIApiHandler::handler, ROIApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = event_api;
  e.handler = std::bind(&EventApiHandler::handler, EventApiHandler(), _1, _2);
  Handlers.push_back(e);
  e.Api = peripherals_api;
  e.handler = std::bind(&PeripheralsApiHandler::handler,
                        PeripheralsApiHandler(), _1, _2);
  Handlers.push_back(e);

  std::string new_token;
  Resp.setHeader(HttpStatus::kOk);
  if (HttpStatus::kOk == parseRequest(Req)) {
#ifndef USE_RKIPC
#ifdef ENABLE_JWT
    new_token = checkToken(Req);
// minilog_debug("level is %d\n", Req.UserLevel);
#endif
#endif
    for (auto h : Handlers) {
      if (!Req.Api.compare(1, 20, h.Api, 0, 20)) {
#ifdef USE_RKIPC
        minilog_debug("Req.PathInfo: %s, use rkipc\n", Req.PathInfo.c_str());
#else // USE_RKIPC
#ifdef MEDIASERVER_ROCKFACE
        minilog_debug("Req.PathInfo: %s, mediaserver rockface enable\n",
                      Req.PathInfo.c_str());
#else
        minilog_debug("Req.PathInfo: %s\n, mediaserver rockface disable",
                      Req.PathInfo.c_str());
#endif
#endif // USE_RKIPC
        h.handler(Req, Resp);
      }
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kBadRequest, "Error parsing request!");
  }
#ifdef ENABLE_JWT
  // if (new_token.size() > 0) {
  //   Resp.setCookie("token", new_token, EXPIRE_SECONDS);
  // }
#endif
  std::ostringstream os;
  Resp.render(os);
  std::cout << os.str();
}

} // namespace cgi
} // namespace rockchip
