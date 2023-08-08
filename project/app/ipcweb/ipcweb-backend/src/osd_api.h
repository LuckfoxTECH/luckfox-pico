// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef __CGI_OSD_API_H__
#define __CGI_OSD_API_H__

#include "api_handler.h"

#define BMP_FILE_PATH "/oem/usr/share/mediaserver/image.bmp"

#define KEY_NORMALIZED_SCREEN_SIZE "normalizedScreenSize"
#define KEY_OSD_ATTRIBUTE "attribute"
#define KEY_OSD_CHANNEL_NAME_OVERLAY "channelNameOverlay"
#define KEY_OSD_DATE_TIME_OVERLAY "dateTimeOverlay"
#define KEY_OSD_CHARACTER_OVERLAY "characterOverlay"
#define KEY_OSD_PRIVACY_MASK "privacyMask"
#define KEY_OSD_IMAGE_OVERLAY "imageOverlay"

enum OsdDbRegionId {
  OSD_DB_REGION_ID_CHANNEL = 0,
  OSD_DB_REGION_ID_DATETIME,
  OSD_DB_REGION_ID_TEXT0,
  OSD_DB_REGION_ID_TEXT1,
  OSD_DB_REGION_ID_TEXT2,
  OSD_DB_REGION_ID_TEXT3,
  OSD_DB_REGION_ID_TEXT4,
  OSD_DB_REGION_ID_TEXT5,
  OSD_DB_REGION_ID_TEXT6,
  OSD_DB_REGION_ID_TEXT7,
  OSD_DB_REGION_ID_MASK0,
  OSD_DB_REGION_ID_MASK1,
  OSD_DB_REGION_ID_MASK2,
  OSD_DB_REGION_ID_MASK3,
  OSD_DB_REGION_ID_IMAGE
};

namespace rockchip {
namespace cgi {

class OSDApiHandler : public ApiHandler {
public:
  OSDApiHandler() = default;
  ~OSDApiHandler() = default;

  virtual void handler(const HttpRequest &Req, HttpResponse &Resp) final;
};

} // namespace cgi
} // namespace rockchip

#endif // __CGI_OSD_API_H__
