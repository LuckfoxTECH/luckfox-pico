// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <gtest/gtest.h>

#include "objects/tcpip_param.h"

namespace rockchip {
namespace cgi {

TEST(ObjectsTest, Tcpip) {
  tcpip_param p = {0,
                   "aa:bb:cc:dd:ee:ff",
                   true,
                   "192.168.0.2",
                   "192.168.0.1",
                   "255.255.255.0",
                   "192.168.0.255",
                   {"1.1.1.1", "8.8.8.8"}};
  nlohmann::json j = p;
  auto p2 = j.get<tcpip_param>();

  EXPECT_EQ(p.id, p2.id);
}

} // namespace cgi
} // namespace rockchip

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
