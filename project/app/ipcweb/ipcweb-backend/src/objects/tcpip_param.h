// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_OBJECTS_TCPIP_H__
#define __CGI_OBJECTS_TCPIP_H__

#include <nlohmann/json.hpp>

namespace rockchip {
namespace cgi {

struct tcpip_param {
  unsigned int id;
  std::string mac;
  bool dhcp;
  std::string address;
  std::string gateway;
  std::string netmask;
  std::string broadcast;
  std::vector<std::string> nameservers;
};

void to_json(nlohmann::json &j, const tcpip_param &p) {
  j = nlohmann::json{{"id", p.id},
                     {"mac", p.mac},
                     {"dhcp", p.dhcp},
                     {"address", p.address},
                     {"gateway", p.gateway},
                     {"netmask", p.netmask},
                     {"broadcast", p.broadcast},
                     {"nameservers", p.nameservers}};
}

void from_json(const nlohmann::json &j, tcpip_param &p) {
  p.id = j.at("id").get<int>();
  p.mac = j.at("mac").get<std::string>();
  p.dhcp = j.at("dhcp").get<bool>();
  p.address = j.at("address").get<std::string>();
  p.gateway = j.at("gateway").get<std::string>();
  p.netmask = j.at("netmask").get<std::string>();
  p.broadcast = j.at("broadcast").get<std::string>();
  p.nameservers = j.at("nameservers").get<std::vector<std::string>>();
}

} // namespace cgi
} // namespace rockchip

#endif // __CGI_OBJECTS_TCPIP_H__
