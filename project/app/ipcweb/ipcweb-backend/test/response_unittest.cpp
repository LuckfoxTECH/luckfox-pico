// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>

#include "http_response.h"

namespace rockchip {
namespace cgi {

TEST(ResponseTest, StatusString) {
  HttpResponse Resp;
  Resp.setHeader(HttpStatus::kOk);
  std::ostringstream OutStr;
  Resp.render(OutStr);
  EXPECT_EQ("HTTP/1.1 200 OK\n\n", OutStr.str());
}

TEST(ResponseTest, ErrorMessage) {
  HttpResponse Resp;
  std::string Result = "HTTP/1.1 400 Bad Request Message!\n";
  Result.append("Content-Type: application/json;charset-utf8\n");
  Result.append("Content-Length: 55\n");
  Result.append("\n");
  Result.append(
      "{\"error\":{\"code\":400,\"message\":\"Bad Request Message!\"}}");
  Resp.setErrorResponse(HttpStatus::kBadRequest, "Bad Request Message!");
  std::ostringstream OutStr;
  Resp.render(OutStr);
  EXPECT_EQ(Result, OutStr.str());
}

TEST(ResponseTest, JsonContent) {
  HttpResponse Resp;
  std::string Result = "HTTP/1.1 200 OK\n";
  Result.append("Content-Type: application/json;charset-utf8\n");
  Result.append("Content-Length: 85\n\n");
  Result.append("{\"apiVersion\":\"1.0\",\"data\":{\"items\":[{\"a\":\"b\","
                "\"c\":{\"d\":\"e\",\"f\":\"g\"}}]},\"kind\":\"test\"}");

  nlohmann::json J = R"({
  "a": "b",
  "c": {
    "d": "e",
    "f": "g"
  }
  })"_json;

  Resp.setHeader(HttpStatus::kOk);
  Resp.setApiContent("test");
  Resp.appendApiItem(J);
  std::ostringstream OutStr;
  Resp.render(OutStr);

  EXPECT_EQ(Result, OutStr.str());
}

} // namespace cgi
} // namespace rockchip

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
