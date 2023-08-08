// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "rest_api.h"

using namespace rockchip::cgi;
int main(int argc, char **argv) {
  __minilog_log_init(argv[0], NULL, false, true, "cgi", "1.0");
  ApiEntry::getInstance().run();
  return 0;
}
