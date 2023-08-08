// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_ROI_API_H__
#define __CGI_ROI_API_H__

#include "api_handler.h"

namespace rockchip {
namespace cgi {

class ROIApiHandler : public ApiHandler {
public:
  ROIApiHandler() = default;
  ~ROIApiHandler() = default;

  virtual void handler(const HttpRequest &Req, HttpResponse &Resp) final;
};

} // namespace cgi
} // namespace rockchip

#endif // __CGI_ROI_API_H__
