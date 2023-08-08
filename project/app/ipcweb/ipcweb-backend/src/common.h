// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_COMMON_API_H__
#define __CGI_COMMON_API_H__

#include "api_handler.h"
//#include "jwt-cpp/jwt.h"

#ifdef USE_RKIPC
#include "socket_client/client.h"
#else
#include <dbserver.h>
#include <mediaserver.h>
#include <netserver.h>
#include <storage_manager.h>
#include <system_manager.h>
#endif
#include <fstream>
#include <time.h>

#define SECRET "rockchip"
#define EXPIRE_SECONDS 3600
#define REACTIVE_TIME 1800

namespace rockchip {
namespace cgi {

std::string ipv4_address_get();
std::string current_time_get();
time_t standard_to_timestamp(std::string standard_time);
std::string jwt_token_get(std::string username, int auth, long expiretime);
std::string jwt_token_verify(std::string token, HttpRequest &Req);
unsigned char FromHex(unsigned char x);
std::string DecodeURI(std::string &str);

} // namespace cgi
} // namespace rockchip

#endif // __CGI_COMMON_API_H__