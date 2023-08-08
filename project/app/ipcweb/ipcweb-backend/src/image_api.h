// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_IMAGE_API_H__
#define __CGI_IMAGE_API_H__

#include "api_handler.h"

#define MAX_JSON_LENGTH_OF_CHANNEL_RESOURCE 3000
#define MAX_JSON_LENGTH_OF_SPECIFIC_RESOURCE 350

#define IMAGE_SCENARIO_NORMAL "normal"
#define IMAGE_SCENARIO_BACKLIGHT "backlight"
#define IMAGE_SCENARIO_FRONTLIGHT "frontlight"
#define IMAGE_SCENARIO_LOW_ILLUMINATION "low-illumination"
#define IMAGE_SCENARIO_CUSTOM1 "custom1"
#define IMAGE_SCENARIO_CUSTOM2 "custom2"

#define PATH_IMAGE_SCENARIO "scenario"

#define PATH_IMAGE_ADJUSTMENT "adjustment"
#define PATH_IMAGE_EXPOSURE "exposure"
#define PATH_IMAGE_NIGHT_TO_DAY "night-to-day"
#define PATH_IMAGE_BLC "blc"
#define PATH_IMAGE_WHITE_BLANCE "white-blance"
#define PATH_IMAGE_ENHANCEMENT "enhancement"
#define PATH_IMAGE_VIDEO_ADJUSTMEN "video-adjustment"
#define PATH_IMAGE_OTHER "other"
#define PATH_IMAGE_AF "af"
#define PATH_IMAGE_AF_CMD "af-cmd"

#define KEY_IMAGE_ADJUSTMENT "imageAdjustment"
#define KEY_IMAGE_EXPOSURE "exposure"
#define KEY_IMAGE_NIGHT_TO_DAY "nightToDay"
#define KEY_IMAGE_BLC "BLC"
#define KEY_IMAGE_WHITE_BLANCE "whiteBlance"
#define KEY_IMAGE_ENHANCEMENT "imageEnhancement"
#define KEY_IMAGE_VIDEO_ADJUSTMEN "videoAdjustment"
#define KEY_IMAGE_OTHER "other"

namespace rockchip {
namespace cgi {

class ImageApiHandler : public ApiHandler {
public:
  ImageApiHandler() = default;
  ~ImageApiHandler() = default;

  virtual void handler(const HttpRequest &Req, HttpResponse &Resp) final;
};

} // namespace cgi
} // namespace rockchip

#endif // __CGI_IMAGE_API_H__
