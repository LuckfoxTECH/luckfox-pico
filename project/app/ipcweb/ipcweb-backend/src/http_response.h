// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_HTTP_RESPONSE_H__
#define __CGI_HTTP_RESPONSE_H__

#include <map>
#include <memory>
#include <string>

#include "export.h"
#include "nlohmann/json.hpp"

namespace rockchip {
namespace cgi {

enum class CGI_EXPORT HttpStatus : int {
  kOk = 200,
  kCreated = 201,
  kAccepted = 202,
  kNoContent = 204,
  kMultipleChoices = 300,
  kMovedPermanently = 301,
  kMovedTemporarily = 302,
  kNotModified = 304,
  kResumeIncomplete = 308,
  kBadRequest = 400,
  kUnauthorized = 401,
  kForbidden = 403,
  kNotFound = 404,
  kInternalServerError = 500,
  kNotImplemented = 501,
  kBadGateway = 502,
  kServiceUnavailable = 503,
};

class HttpResponseImpl;
class CGI_EXPORT HttpResponse {
public:
  HttpResponse();
  virtual ~HttpResponse();

  void setHeader(HttpStatus Status, const std::string &Reason);
  void setHeader(HttpStatus Status);
  void addHeader(const std::string &Key, const std::string &Value);
  void setCookie(const std::string &Name, const std::string &Value,
                 unsigned long MaxAge);
  void setErrorResponse(HttpStatus Status, const std::string &Message);
  void setApiContent(const std::string &Kind);
  void appendApiItem(const std::string &Item);
  void appendApiItem(const nlohmann::json &Item);
  void appendApiItems(const nlohmann::json &Items);
  void setApiData(const nlohmann::json &Data);
  void render(std::ostream &Stream);

private:
  std::unique_ptr<HttpResponseImpl> impl;
};

} // namespace cgi
} // namespace rockchip

#endif // __CGI_HTTP_RESPONSE_H__
