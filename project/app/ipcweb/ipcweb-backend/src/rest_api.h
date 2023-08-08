// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_REST_API_H__
#define __CGI_REST_API_H__

#include "export.h"
#include "http_request.h"
#include "http_response.h"

#if 0
#include "minilogger/backtrace.h"
#include "minilogger/log.h"
#else
#define minilog_warn(...)
#define minilog_error(...)
#define minilog_info(...)
#define minilog_debug(...)
#define __minilog_log_init(...)
#endif

namespace rockchip {
namespace cgi {

typedef std::function<void(const HttpRequest &, HttpResponse &)> Handler;

struct HandlerEntry {
  std::string Api;
  Handler handler;
};

class CGI_EXPORT ApiEntry {
public:
  ApiEntry() = default;
  ~ApiEntry() = default;
  ApiEntry(const ApiEntry &) = delete;
  ApiEntry &operator=(const ApiEntry &) = delete;

  static ApiEntry &getInstance() {
    static ApiEntry instance;
    return instance;
  }
  void run();

  std::vector<HandlerEntry> Handlers;
};

} // namespace cgi
} // namespace rockchip

#endif // __CGI_REST_API_H__
