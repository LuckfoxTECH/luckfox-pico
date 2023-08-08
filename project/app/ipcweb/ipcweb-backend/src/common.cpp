// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "common.h"

namespace rockchip {
namespace cgi {

std::string current_time_get() {
  char current_time[30];
  time_t rawtime;
  time(&rawtime);
  struct tm *time_info = localtime(&rawtime);
  strftime(current_time, 30, "%FT%T", time_info);
  std::string current_time_s = current_time;

  return current_time_s;
}

time_t standard_to_timestamp(std::string standard_time) {
  time_t stamp_time = 0;
  struct tm *timeinfo;

  // get current timeinfo and modify it to the user's choice
  time(&stamp_time);
  timeinfo = localtime(&stamp_time);
  timeinfo->tm_year = stoi(standard_time.substr(0, 4)) - 1900;
  timeinfo->tm_mon = stoi(standard_time.substr(5, 2)) - 1;
  timeinfo->tm_mday = stoi(standard_time.substr(8, 2));
  timeinfo->tm_hour = stoi(standard_time.substr(11, 2));
  timeinfo->tm_min = stoi(standard_time.substr(14, 2));
  timeinfo->tm_sec = stoi(standard_time.substr(17, 2));
  stamp_time = mktime(timeinfo);

  return stamp_time;
}

std::string jwt_token_get(std::string username, int auth, long expiretime) {
  // std::string token =
  //     jwt::create()
  //         .set_issuer("auth0")
  //         .set_type("JWS")
  //         .set_payload_claim("username", jwt::claim(username))
  //         .set_payload_claim("auth", jwt::claim(std::to_string(auth)))
  //         .set_issued_at(std::chrono::system_clock::now())
  //         .set_expires_at(std::chrono::system_clock::now() +
  //                         std::chrono::seconds{expiretime})
  //         .sign(jwt::algorithm::hs256{SECRET});
  std::string token = "";
  return token;
}

unsigned char FromHex(unsigned char x) {
  unsigned char y;
  if (x >= 'A' && x <= 'Z')
    y = x - 'A' + 10;
  else if (x >= 'a' && x <= 'z')
    y = x - 'a' + 10;
  else if (x >= '0' && x <= '9')
    y = x - '0';
  else
    assert(0);
  return y;
}

std::string DecodeURI(std::string &str) {
  std::string strTemp = "";
  size_t length = str.length();
  for (size_t i = 0; i < length; i++) {
    if (str[i] == '+')
      strTemp += ' ';
    else if (str[i] == '%') {
      assert(i + 2 < length);
      unsigned char high = FromHex((unsigned char)str[++i]);
      unsigned char low = FromHex((unsigned char)str[++i]);
      strTemp += high * 16 + low;
    } else
      strTemp += str[i];
  }
  return strTemp;
}

#ifdef USE_RKIPC

#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_arp.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>

std::string ipv4_address_get() {
  char ip[32] = {NULL};
  int ret;
  int sock;
  struct ifreq ifr;

  sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) {
    return NULL;
  }
  memset(&ifr, 0, sizeof(ifr));
  strncpy(ifr.ifr_name, "eth0", sizeof("eth0") - 1);
  ifr.ifr_name[IFNAMSIZ - 1] = '\0';
  ret = ioctl(sock, SIOCGIFADDR, &ifr);
  if (ret) {
    strncpy(ifr.ifr_name, "usb0", sizeof("usb0") - 1);
    ret = ioctl(sock, SIOCGIFADDR, &ifr);
    if (ret) {
      strncpy(ifr.ifr_name, "wlan0", sizeof("wlan0") - 1);
      ret = ioctl(sock, SIOCGIFADDR, &ifr);
    }
  }
  sprintf(ip, "%s", inet_ntoa(((struct sockaddr_in *)(&ifr.ifr_addr))->sin_addr));

  if (sock > 0) {
    close(sock);
  }

  return std::string(ip);
}

#else
std::string ipv4_address_get() {
  char pService[255];
  char *str = netserver_get_service((char *)"ethernet");
  if (!strcmp(str, "[ ]"))
    str = netserver_get_service((char *)"wifi");

  nlohmann::json service = nlohmann::json::parse(str).at(0).at("sService");
  strcpy(pService, std::string(service.get<std::string>()).c_str());
  str = netserver_get_config((char *)pService);
  nlohmann::json cfg = nlohmann::json::parse(str);
  std::string ipv4_address = cfg.at(0).at("ipv4").at("sV4Address");

  return ipv4_address;
}

std::string jwt_token_verify(std::string token, HttpRequest &Req) {
  auto decoded = jwt::decode(token);
  // std::string head = decoded.get_header();
  std::string payload = decoded.get_payload();
  // minilog_debug("head is %s\n", head.c_str());
  // minilog_debug("payload is %s\n", payload.c_str());
  nlohmann::json payload_json = nlohmann::json::parse(payload);
  int expire_time = payload_json.at("exp");
  std::string new_token = "";
  int now_time =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  std::string auth = payload_json.at("auth");
  if ((expire_time - now_time) < REACTIVE_TIME) {
    std::string user_name = payload_json.at("username");
    std::string cmd = "select id from SystemUser WHERE sUserName='" +
                      user_name + "' AND iUserLevel=" + auth;
    char *str =
        dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_SYSTEM_INTERFACE);
    nlohmann::json check_json = nlohmann::json::parse(str).at("jData");
    if (!check_json.empty()) {
      new_token = jwt_token_get(user_name, stoi(auth), EXPIRE_SECONDS);
    } else {
      new_token = jwt_token_get(user_name, stoi(auth), 0);
    }
    // minilog_debug("create token is %s", new_token.c_str());
  }
  Req.UserLevel = stoi(auth);
  auto verifier = jwt::verify()
                      .allow_algorithm(jwt::algorithm::hs256{SECRET})
                      .with_issuer("auth0");
  verifier.verify(decoded);
  return new_token;
}
#endif

} // namespace cgi
} // namespace rockchip
