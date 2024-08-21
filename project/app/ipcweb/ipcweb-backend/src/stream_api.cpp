// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "stream_api.h"
#include "common.h"

namespace rockchip {
namespace cgi {

#ifdef USE_RKIPC
void StreamURLApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  nlohmann::json content;
  if (Req.Method == "GET") {
    char *str =
        (char *)"[{\"id\":0,\"sStreamProtocol\":\"HTTP\"},{\"id\":1,"
                "\"sStreamProtocol\":\"HTTP\"},{\"id\":2,\"sStreamProtocol\":"
                "\"HTTP\"}]";
    nlohmann::json stream_url_config = nlohmann::json::parse(str);
    int pos_first = Req.PathInfo.find_first_of("/");
    int pos_last = Req.PathInfo.find_last_of("/");

    /* Get ip address */
    std::string ipv4_address = ipv4_address_get();
    minilog_debug("ipv4_address is %s\n", ipv4_address.c_str());

    /* Get port */
    std::string http_port = "80";
    std::string rtsp_port = "554";
    std::string rtmp_port = "1935";

    /* Get URL by splicing */
    for (int i = 0; i < 3; i++) {
      std::string url;
      std::string stream_type;

      if (i == 0)
        stream_type = "mainstream";
      else if (i == 1)
        stream_type = "substream";
      else if (i == 2)
        stream_type = "thirdstream";

      url = "http://" + ipv4_address + ":" + http_port +
            "/live?port=" + rtmp_port + "&app=live&stream=" + stream_type;
      // example :
      // http://172.16.21.22:80/live?port=1935&app=live&stream=substream
      stream_url_config.at(i).emplace("sURL", url);
    }
    content = stream_url_config;

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else

void StreamURLApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  if (Req.Method == "GET") {
    nlohmann::json content;
    char *str = dbserver_stream_url_get();
    nlohmann::json stream_url_config = nlohmann::json::parse(str).at("jData");
    int pos_first = Req.PathInfo.find_first_of("/");
    int pos_last = Req.PathInfo.find_last_of("/");

    /* Get ip address */
    std::string ipv4_address = ipv4_address_get();

    /* Get port */
    str = dbserver_port_get();
    nlohmann::json port_all_config = nlohmann::json::parse(str).at("jData");
    std::string http_port = (port_all_config.at(0).at("iPortNo").dump());
    std::string rtsp_port = (port_all_config.at(3).at("iPortNo").dump());
    std::string rtmp_port = (port_all_config.at(4).at("iPortNo").dump());

    /* Get URL by splicing */
    for (int i = 0; i < stream_url_config.size(); i++) {
      std::string url;
      std::string stream_type;
      char *stream_protocol = new char[32];

      if (i == 0)
        stream_type = "mainstream";
      else if (i == 1)
        stream_type = "substream";
      else if (i == 2)
        stream_type = "thirdstream";
      else if (i == 3)
        stream_type = "mainstream";
      else if (i == 4)
        stream_type = "substream";
      else if (i == 5)
        stream_type = "thirdstream";
      strcpy(stream_protocol, stream_url_config.at(i)
                                  .at("sStreamProtocol")
                                  .get<std::string>()
                                  .c_str());
      if (!strcmp(stream_protocol, "RTSP"))
        url = "rtsp://" + ipv4_address + ":" + rtsp_port + "/" + stream_type;
      else if (!strcmp(stream_protocol, "RTMP"))
        url =
            "rtmp://" + ipv4_address + ":" + rtmp_port + "/live/" + stream_type;
      else if (!strcmp(stream_protocol, "HTTP"))
        url = "http://" + ipv4_address + ":" + http_port +
              "/live?port=" + rtmp_port + "&app=live&stream=" + stream_type;
      // example :
      // http://172.16.21.22:80/live?port=1935&app=live&stream=substream
      stream_url_config.at(i).emplace("sURL", url);
      delete[] stream_protocol;
    }

    if (pos_first != pos_last) {
      int id =
          atoi(Req.PathInfo.substr(pos_last + 1, Req.PathInfo.size()).c_str());
      content = stream_url_config.at(id);
    } else {
      content = stream_url_config;
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    nlohmann::json content = nlohmann::json::object();
    nlohmann::json stream_url_config =
        Req.PostObject; /* must be json::object */
    int id = 0;
    int pos_first = Req.PathInfo.find_first_of("/");
    int pos_last = Req.PathInfo.find_last_of("/");
    if (pos_first != pos_last)
      id = atoi(Req.PathInfo.substr(pos_last + 1, Req.PathInfo.size()).c_str());

    /* Erase unchanged data */
    char *prev = dbserver_stream_url_get();
    nlohmann::json cfg_old_all = nlohmann::json::parse(prev).at("jData");
    nlohmann::json diff =
        nlohmann::json::diff(cfg_old_all.at(id), stream_url_config);
    for (auto &x : nlohmann::json::iterator_wrapper(cfg_old_all.at(id))) {
      if (diff.dump().find(x.key()) == diff.dump().npos)
        stream_url_config.erase(x.key());
    }

    /* sURL cannot be set */
    if (stream_url_config.dump().find("sURL") != stream_url_config.dump().npos)
      stream_url_config.erase("sURL");

    /* Set new config, only one table is transmitted at a time. */
    if (!stream_url_config.empty())
      dbserver_stream_url_set((char *)stream_url_config.dump().c_str(), id);

    /* Get new info */
    char *str = dbserver_stream_url_get();
    nlohmann::json stream_url_all_config =
        nlohmann::json::parse(str).at("jData");
    content = stream_url_all_config.at(id);
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}
#endif

} // namespace cgi
} // namespace rockchip
