// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_API_HANDLER_H__
#define __CGI_API_HANDLER_H__

#include <functional>

#include "export.h"
#include "http_request.h"
#include "http_response.h"
#include "rest_api.h"

namespace rockchip {
namespace cgi {

class CGI_EXPORT ApiHandler {
public:
  ApiHandler() = default;
  virtual ~ApiHandler() = default;

  virtual void handler(const HttpRequest &Req, HttpResponse &Resp) = 0;
};

} // namespace cgi
} // namespace rockchip

#endif // __CGI_API_HANDLER_H__