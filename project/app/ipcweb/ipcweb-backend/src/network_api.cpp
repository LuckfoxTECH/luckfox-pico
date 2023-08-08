// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "network_api.h"
#include "common.h"
#include <time.h>

namespace rockchip {
namespace cgi {

#ifdef USE_RKIPC

nlohmann::json network_get_config(std::string interface) {
  int speed, duplex, autoneg;
  char mac[64];
  char dns1[64];
  char dns2[64];
  char method[64];
  char address[64];
  char netmask[64];
  char gateway[64];
  char nic_support[256];
  nlohmann::json config, ipv4, link;

  rk_network_ipv4_get(interface.c_str(), method, address, netmask, gateway);
  ipv4.emplace("sV4Address", address);
  ipv4.emplace("sV4Gateway", gateway);
  ipv4.emplace("sV4Method", method);
  ipv4.emplace("sV4Netmask", netmask);

  rk_network_dns_get(dns1, dns2);
  link.emplace("sDNS1", dns1);
  link.emplace("sDNS2", dns2);
  rk_network_get_mac(interface.c_str(), mac);
  link.emplace("sAddress", mac);
  link.emplace("sInterface", interface.c_str());

  if (!interface.compare("wlan0")) {
    link.emplace("iPower", 1); // todo
  } else {
    // default eth0 on
    link.emplace("iPower", 1);
    rk_network_nicspeed_get(interface.c_str(), &speed, &duplex, &autoneg);
    link.emplace("iDuplex", duplex);
    link.emplace("iNicSpeed", speed);
    if (autoneg)
      link.emplace("sNicSpeed", "Auto");
    else if ((speed == 10) && (duplex == 0))
      link.emplace("sNicSpeed", "10baseT/Half");
    else if ((speed == 10) && (duplex == 1))
      link.emplace("sNicSpeed", "10baseT/Full");
    else if ((speed == 100) && (duplex == 0))
      link.emplace("sNicSpeed", "100baseT/Half");
    else if ((speed == 100) && (duplex == 1))
      link.emplace("sNicSpeed", "100baseT/Full");
    else if ((speed == 1000) && (duplex == 1))
      link.emplace("sNicSpeed", "1000baseT/Full");
    rk_network_nicspeed_support_get(interface.c_str(), nic_support);
    link.emplace("sNicSpeedSupport", nic_support);
  }

  config.emplace("ipv4", ipv4);
  config.emplace("link", link);

  return config;
}

nlohmann::json wifi_power_get() {
  nlohmann::json config;
  int on;
  rk_wifi_power_get(&on);
  config.emplace("iPower", on);
  config.emplace("id", 1);
  config.emplace("sType", "wifi");

  return config;
}

nlohmann::json wifi_list_get() {
  nlohmann::json config;
  char *wifi_list = NULL;
  rk_wifi_get_list(&wifi_list);
  config = nlohmann::json::parse(wifi_list);
  free(wifi_list);

  return config;
}

void NetworkApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  char *str;
  char pService[255];
  nlohmann::json content;
  std::string path_api_resource;
  std::string path_specific_resource;
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
    if (!path_specific_resource.compare("lan")) {
      content = network_get_config("eth0");
    } else if (!path_specific_resource.compare("wlan")) {
      content = network_get_config("wlan0");
    } else if (!path_specific_resource.compare("wifi")) {
      content = wifi_power_get();
    } else if (!path_specific_resource.compare("wifi-list")) {
      content = wifi_list_get();
    } else if (!path_specific_resource.compare("auto-cnt-wifi")) {
      // str = dbserver_sql((char *)"select sService from NetworkService WHERE
      // (iAutoconnect = 1) LIMIT 1",
      //                      (char *)DBSERVER_NET_INTERFACE);
      // content = nlohmann::json::parse(str).at("jData");
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
    nlohmann::json cfg_new = Req.PostObject; // must be json::object
    std::string interface;
    // Set network/wlan or network/lan
    if (path_specific_resource.find("lan") != std::string::npos) {
      if (!path_specific_resource.compare("lan"))
        interface = "eth0";
      else if (!path_specific_resource.compare("wlan"))
        interface = "wlan0";
      nlohmann::json cfg_old = network_get_config(interface);
      nlohmann::json diff = nlohmann::json::diff(cfg_old, cfg_new);
      // Set according to the differences
      // Set DNS1 and DNS2
      if (diff.dump().find("link") != diff.dump().npos) {
        std::string dns1 = "";
        std::string dns2 = "";
        std::string sNicSpeed = cfg_new.at("link").at("sNicSpeed").dump();
        std::string Method = cfg_new.at("ipv4").at("sV4Method").dump();
        sNicSpeed.erase(0, 1).erase(sNicSpeed.end() - 1, sNicSpeed.end());
        Method.erase(0, 1).erase(Method.end() - 1, Method.end()); // erase \"
        if (Method == "manual") {
          dns1 = cfg_new.at("link").at("sDNS1").dump();
          dns2 = cfg_new.at("link").at("sDNS2").dump();
          dns1.erase(0, 1).erase(dns1.end() - 1, dns1.end());
          dns2.erase(0, 1).erase(dns2.end() - 1, dns2.end());
        }
        rk_network_dns_set((char *)dns1.c_str(), (char *)dns2.c_str());

        if (!sNicSpeed.compare("10baseT/Half"))
          rk_network_nicspeed_set((char *)interface.c_str(), 10, 0, 0);
        else if (!sNicSpeed.compare("10baseT/Full"))
          rk_network_nicspeed_set((char *)interface.c_str(), 10, 1, 0);
        else if (!sNicSpeed.compare("100baseT/Half"))
          rk_network_nicspeed_set((char *)interface.c_str(), 100, 0, 0);
        else if (!sNicSpeed.compare("100baseT/Full"))
          rk_network_nicspeed_set((char *)interface.c_str(), 100, 1, 0);
        else if (!sNicSpeed.compare("1000baseT/Full"))
          rk_network_nicspeed_set((char *)interface.c_str(), 1000, 1, 0);
        else
          rk_network_nicspeed_set((char *)interface.c_str(), -1, -1, 1);
      }
      if (diff.dump().find("ipv4") != diff.dump().npos) {
        nlohmann::json cfg_ipv4 = cfg_new.at("ipv4");
        std::string Method = cfg_ipv4.at("sV4Method").dump();
        Method.erase(0, 1).erase(Method.end() - 1, Method.end()); // erase \"
        if (Method == "manual") {
          std::string Address = cfg_ipv4.at("sV4Address").dump();
          Address.erase(0, 1).erase(Address.end() - 1, Address.end());
          std::string Netmask = cfg_ipv4.at("sV4Netmask").dump();
          Netmask.erase(0, 1).erase(Netmask.end() - 1, Netmask.end());
          std::string Gateway = cfg_ipv4.at("sV4Gateway").dump();
          Gateway.erase(0, 1).erase(Gateway.end() - 1, Gateway.end());
          rk_network_ipv4_set((char *)interface.c_str(), (char *)"static",
                              (char *)Address.c_str(), (char *)Netmask.c_str(),
                              (char *)Gateway.c_str());
        } else {
          rk_network_ipv4_set((char *)interface.c_str(), (char *)"dhcp", NULL,
                              NULL, NULL);
        }
      }
      // Update info
      content = network_get_config(interface);
    } else if (!path_specific_resource.compare("wifi")) {
      // Set wifi power
      if (!Req.Params.empty()) {
        for (auto p : Req.Params) {
          if (p.Key == "power") {
            if (p.Value == "on")
              rk_wifi_power_set(1);
            else
              rk_wifi_power_set(0);
            content = wifi_power_get();
          } else if (p.Key == "scan") {
            rk_wifi_scan_wifi();
            content = wifi_list_get();
          }
        }
      }
      // Set wifi connect
      if (!cfg_new.empty()) {
        std::string ssid = cfg_new.at("sName").dump();
        ssid.erase(0, 1).erase(ssid.end() - 1, ssid.end());
        std::string password = cfg_new.at("sPassword").dump();
        password.erase(0, 1).erase(password.end() - 1, password.end());
        rk_wifi_connect_with_ssid(ssid.c_str(), password.c_str());
        content = wifi_list_get();
      }
    }
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if (Req.Method == "DELETE") {
    if (!path_specific_resource.compare("wifi")) {
      for (auto p : Req.Params) {
        if (p.Key == "service") {
          std::string ssid = p.Value;
          rk_wifi_forget_with_ssid(ssid.c_str());
          Resp.setHeader(HttpStatus::kOk, "OK");
        }
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

void NetworkNTPApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
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

void NetworkDDNSApiHandler::handler(const HttpRequest &Req,
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

void NetworkPPPoEApiHandler::handler(const HttpRequest &Req,
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

void NetworkPortApiHandler::handler(const HttpRequest &Req,
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

#else // USE_RKIPC

nlohmann::json network_get_config(std::string interface) {
  char *str = netserver_get_networkip((char *)interface.c_str());
  nlohmann::json config = nlohmann::json::parse(str).at(0);
  std::string dns1 = "";
  std::string dns2 = "";
  std::string duplex_s = "";
  int duplex = config.at("link").at("iDuplex");
  int speed = config.at("link").at("iNicSpeed");
  std::string v4_method = config.at("dbconfig").at("sV4Method");
  std::string sNicSpeed = config.at("dbconfig").at("sNicSpeed");

  config.at("link").erase("sNicSpeed");
  config.at("link").emplace("sNicSpeed", sNicSpeed);
  if (!v4_method.compare("manual")) {
    dns1 = config.at("dbconfig").at("sDNS1");
    dns2 = config.at("dbconfig").at("sDNS2");
    config.at("link").erase("sDNS1");
    config.at("link").erase("sDNS2");
    config.at("link").emplace("sDNS1", dns1);
    config.at("link").emplace("sDNS2", dns2);
  }
  config.erase("dbconfig");
  config.at("ipv4").emplace("sV4Method", v4_method);

  return config;
}

void NetworkApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  char *str;
  char pService[255];
  nlohmann::json content;
  std::string path_api_resource;
  std::string path_specific_resource;
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
    if (!path_specific_resource.compare("lan")) {
      content = network_get_config("eth0");
    } else if (!path_specific_resource.compare("wlan")) {
      content = network_get_config("wlan0");
    } else if (!path_specific_resource.compare("wifi")) {
      str = dbserver_wifi_power_get();
      content = nlohmann::json::parse(str).at("jData").at(0);
    } else if (!path_specific_resource.compare("wifi-list")) {
      str = netserver_get_service((char *)"wifi");
      content = nlohmann::json::parse(str);
    } else if (!path_specific_resource.compare("auto-cnt-wifi")) {
      str = dbserver_sql((char *)"select sService from NetworkService WHERE "
                                 "(iAutoconnect = 1) LIMIT 1",
                         (char *)DBSERVER_NET_INTERFACE);
      content = nlohmann::json::parse(str).at("jData");
    }

    if (!content.empty()) {
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
#ifdef ENABLE_JWT
    if (user_level > 1) {
      Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
      return;
    }
#endif
    nlohmann::json cfg_new = Req.PostObject; // must be json::object
    std::string interface;
    // Set network/wlan or network/lan
    if (path_specific_resource.find("lan") != std::string::npos) {
      if (!path_specific_resource.compare("lan"))
        interface = "eth0";
      else if (!path_specific_resource.compare("wlan"))
        interface = "wlan0";
      nlohmann::json cfg_old = network_get_config(interface);
      nlohmann::json diff = nlohmann::json::diff(cfg_old, cfg_new);
      // Set according to the differences
      if (diff.dump().find("ipv4") != diff.dump().npos) {
        nlohmann::json cfg_ipv4 = cfg_new.at("ipv4");
        std::string Method = cfg_ipv4.at("sV4Method").dump();
        Method.erase(0, 1).erase(Method.end() - 1, Method.end()); // erase \"
        if (Method == "manual") {
          std::string Address = cfg_ipv4.at("sV4Address").dump();
          Address.erase(0, 1).erase(Address.end() - 1, Address.end());
          std::string Netmask = cfg_ipv4.at("sV4Netmask").dump();
          Netmask.erase(0, 1).erase(Netmask.end() - 1, Netmask.end());
          std::string Gateway = cfg_ipv4.at("sV4Gateway").dump();
          Gateway.erase(0, 1).erase(Gateway.end() - 1, Gateway.end());
          dbserver_network_ipv4_set((char *)interface.c_str(), (char *)"manual",
                                    (char *)Address.c_str(),
                                    (char *)Netmask.c_str(),
                                    (char *)Gateway.c_str());
        } else {
          dbserver_network_ipv4_set((char *)interface.c_str(), (char *)"dhcp",
                                    NULL, NULL, NULL);
        }
      }
      // Set DNS1 and DNS2
      if (diff.dump().find("link") != diff.dump().npos) {
        std::string dns1 = "";
        std::string dns2 = "";
        std::string sNicSpeed = cfg_new.at("link").at("sNicSpeed").dump();
        std::string Method = cfg_new.at("ipv4").at("sV4Method").dump();
        sNicSpeed.erase(0, 1).erase(sNicSpeed.end() - 1, sNicSpeed.end());
        Method.erase(0, 1).erase(Method.end() - 1, Method.end()); // erase \"
        if (Method == "manual") {
          dns1 = cfg_new.at("link").at("sDNS1").dump();
          dns2 = cfg_new.at("link").at("sDNS2").dump();
          dns1.erase(0, 1).erase(dns1.end() - 1, dns1.end());
          dns2.erase(0, 1).erase(dns2.end() - 1, dns2.end());
        }
        dbserver_network_dns_set((char *)interface.c_str(),
                                 (char *)dns1.c_str(), (char *)dns2.c_str());
        dbserver_network_nicspeed_set((char *)interface.c_str(),
                                      (char *)sNicSpeed.c_str());
      }
      // Update info
      content = network_get_config(interface);
    } else if (!path_specific_resource.compare("wifi")) {
      // Set wifi power
      if (!Req.Params.empty()) {
        for (auto p : Req.Params) {
          if (p.Key == "power") {
            if (p.Value == "on")
              dbserver_wifi_power_set(1);
            else
              dbserver_wifi_power_set(0);
            str = dbserver_wifi_power_get();
            content = nlohmann::json::parse(str).at("jData").at(0);
          } else if (p.Key == "scan") {
            netserver_scan_wifi();
            str = netserver_get_service((char *)"wifi");
            content = nlohmann::json::parse(str);
          }
        }
      }
      // Set wifi connect
      if (!cfg_new.empty()) {
        // netserver_scan_wifi();
        str = netserver_get_service((char *)"wifi");
        nlohmann::json wifi_list = nlohmann::json::parse(str);
        std::string service = cfg_new.at("sService").dump();
        service.erase(0, 1).erase(service.end() - 1, service.end());
        std::string password = cfg_new.at("sPassword").dump();
        password.erase(0, 1).erase(password.end() - 1, password.end());
        int favorite = 1; // invalid parameter
        int autoconnect = atoi(cfg_new.at("iAutoconnect").dump().c_str());
        dbserver_network_service_connect_set((char *)service.c_str(),
                                             (char *)password.c_str(),
                                             &favorite, &autoconnect);
        str = netserver_get_config((char *)service.c_str());
        content = nlohmann::json::parse(str);
      }
    }
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if (Req.Method == "DELETE") {
    if (!path_specific_resource.compare("wifi")) {
      for (auto p : Req.Params) {
        if (p.Key == "service") {
          std::string service = p.Value;
          dbserver_network_service_delete((char *)service.c_str());
          Resp.setHeader(HttpStatus::kOk, "OK");
        }
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

void NetworkNTPApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  char *str;
  std::string path_api_resource;
  std::string path_specific_resource;
  nlohmann::json content;

  /* Get Path Information */
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_specific_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
  }

  if (Req.Method == "GET") {
    if (path_specific_resource.empty()) {
      str = dbserver_ntp_get();
      content = nlohmann::json::parse(str).at("jData").at(0);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("zone") != std::string::npos) {
      str = dbserver_zone_get();
      content = nlohmann::json::parse(str).at("jData");
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("time") != std::string::npos) {
      content.emplace("time", current_time_get());
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    if (path_specific_resource.empty()) {
      if (!Req.PostObject.empty()) {
        nlohmann::json ntp_config = Req.PostObject;
        std::string service = ntp_config.at("sNtpServers").dump();
        service.erase(0, 1).erase(service.end() - 1, service.end()); // erase \"
        std::string zone = ntp_config.at("sTimeZone").dump();
        zone.erase(0, 1).erase(zone.end() - 1, zone.end());
        std::string zone_file = ntp_config.at("sTimeZoneFile").dump();
        zone_file.erase(0, 1).erase(zone_file.end() - 1, zone_file.end());
        std::string zone_file_dst = ntp_config.at("sTimeZoneFileDst").dump();
        zone_file_dst.erase(0, 1).erase(zone_file_dst.end() - 1,
                                        zone_file_dst.end());
        // automatic daylight saving time
        int auto_dst = atoi(ntp_config.at("iAutoDst").dump().c_str());
        int update_time = atoi(ntp_config.at("iRefreshTime").dump().c_str());
        int auto_mode = atoi(ntp_config.at("iAutoMode").dump().c_str());
        dbserver_ntp_set((char *)service.c_str(), (char *)zone.c_str(),
                         (char *)zone_file.c_str(),
                         (char *)zone_file_dst.c_str(), &auto_dst, &auto_mode,
                         &update_time);
      }
      // Update info
      content = nlohmann::json::parse(dbserver_ntp_get()).at("jData").at(0);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("time") != std::string::npos) {
      nlohmann::json time_config = Req.PostObject;
      std::string time_s = time_config.at("time");
      time_t stamp_time = standard_to_timestamp(time_s);
      int ret = stime(&stamp_time);
      system("hwclock -w -u");
      if (!ret) {
        content.emplace("time", current_time_get());
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Set Time Fail");
      }
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

void NetworkDDNSApiHandler::handler(const HttpRequest &Req,
                                    HttpResponse &Resp) {
  if (Req.Method == "GET") {
    nlohmann::json content = nlohmann::json::array();
    auto TEST = R"(
      {
        "enable":"true",
        "DDNSType":"HiDDNS",
        "serveraddress":"www.rock-chips.com",
        "hostName":"wzy02",
        "port":""
      }
    )"_json;
    content.push_back(TEST);
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

void NetworkPPPoEApiHandler::handler(const HttpRequest &Req,
                                     HttpResponse &Resp) {
  if (Req.Method == "GET") {
    nlohmann::json content = nlohmann::json::array();
    auto TEST = R"(
      {
        "enable":"true",
        "deviceDynamicIP":"0.0.0.0",
        "userName":"admin",
        "password":"123456",
        "passwordConfirmation":"123456"
      }
    )"_json;
    content.push_back(TEST);
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

void NetworkPortApiHandler::handler(const HttpRequest &Req,
                                    HttpResponse &Resp) {
  if (Req.Method == "GET") {
    nlohmann::json content;
    char *str = dbserver_port_get();
    nlohmann::json port_all_config = nlohmann::json::parse(str).at("jData");
    nlohmann::json port_current_config;
    int pos_first = Req.PathInfo.find_first_of("/");
    int pos_last = Req.PathInfo.find_last_of("/");

    if (pos_first != pos_last) {
      int id =
          atoi(Req.PathInfo.substr(pos_last + 1, Req.PathInfo.size()).c_str());
      port_current_config = port_all_config.at(id);
      content = port_current_config;
    } else {
      content = port_all_config;
    }

    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    nlohmann::json content = nlohmann::json::object();
    nlohmann::json port_config = Req.PostObject; // must be json::object
    int id = 0;
    int pos_first = Req.PathInfo.find_first_of("/");
    int pos_last = Req.PathInfo.find_last_of("/");
    if (pos_first != pos_last)
      id = atoi(Req.PathInfo.substr(pos_last + 1, Req.PathInfo.size()).c_str());

    // Erase unchanged data
    char *prev = dbserver_port_get();
    nlohmann::json cfg_old_all = nlohmann::json::parse(prev).at("jData");
    nlohmann::json diff = nlohmann::json::diff(cfg_old_all.at(id), port_config);
    for (auto &x : nlohmann::json::iterator_wrapper(cfg_old_all.at(id))) {
      if (diff.dump().find(x.key()) == diff.dump().npos)
        port_config.erase(x.key());
    }
    // The port protocol cannot be changed
    if (port_config.dump().find("sProtocol") != port_config.dump().npos)
      port_config.erase("sProtocol");

    // Set new config, only one table is transmitted at a time.
    if (!port_config.empty())
      dbserver_port_set((char *)port_config.dump().c_str(), id);

    // Get new info
    char *str = dbserver_port_get();
    nlohmann::json port_all_config = nlohmann::json::parse(str).at("jData");
    content = port_all_config.at(id);
    Resp.setHeader(HttpStatus::kOk, "OK");
    Resp.setApiData(content);
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#endif

} // namespace cgi
} // namespace rockchip
