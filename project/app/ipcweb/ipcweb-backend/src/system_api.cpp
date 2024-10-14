// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "system_api.h"
#include "common.h"
#include <fstream>
#include <sys/statfs.h>

#ifdef MEDIASERVER_ROCKFACE
#include <mediaserver.h>
#include <storage_manager.h>
#endif

namespace rockchip {
namespace cgi {

std::string create_fireware_location() {
  int file_id;
  int exist = 1;
  char *str;
  std::ofstream file;
  // Make sure files are not duplicated
  srand((int)time(NULL));
  while (exist) {
    file_id = rand() % 1000;
    file.open("/data/" + file_id);
    if (!file) // open fail
      exist = 0;
    file.close();
  }
  // Create new file
  std::string file_name = "/data/" + std::to_string(file_id);
  std::ofstream new_file(file_name.c_str());
  new_file.close();
  // Get ip address and location
  std::string ipv4_address = ipv4_address_get();
  std::string location = "http://" + ipv4_address +
                         "/cgi-bin/entry.cgi/system/firmware-upgrade?id=" +
                         std::to_string(file_id);

  return location;
}

#ifdef USE_RKIPC

nlohmann::json device_info_get() {
  nlohmann::json device_info;
  nlohmann::json tmp_json;
  char *tmp = new char[256];

  rk_system_get_deivce_name(&tmp);
  tmp_json.emplace("id", 0);
  tmp_json.emplace("name", "deviceName");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "false");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_telecontrol_id(&tmp);
  tmp_json.emplace("id", 1);
  tmp_json.emplace("name", "telecontrolID");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "false");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_model(&tmp);
  tmp_json.emplace("id", 2);
  tmp_json.emplace("name", "model");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_serial_number(&tmp);
  tmp_json.emplace("id", 3);
  tmp_json.emplace("name", "serialNumber");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_firmware_version(&tmp);
  tmp_json.emplace("id", 4);
  tmp_json.emplace("name", "firmwareVersion");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_encoder_version(&tmp);
  tmp_json.emplace("id", 5);
  tmp_json.emplace("name", "encoderVersion");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_web_version(&tmp);
  tmp_json.emplace("id", 6);
  tmp_json.emplace("name", "webVersion");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_plugin_version(&tmp);
  tmp_json.emplace("id", 7);
  tmp_json.emplace("name", "pluginVersion");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_channels_number(&tmp);
  tmp_json.emplace("id", 8);
  tmp_json.emplace("name", "channelsNumber");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_hard_disks_number(&tmp);
  tmp_json.emplace("id", 9);
  tmp_json.emplace("name", "hardDisksNumber");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_alarm_inputs_number(&tmp);
  tmp_json.emplace("id", 10);
  tmp_json.emplace("name", "alarmInputsNumber");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_alarm_outputs_number(&tmp);
  tmp_json.emplace("id", 11);
  tmp_json.emplace("name", "alarmOutputsNumber");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_firmware_version_info(&tmp);
  tmp_json.emplace("id", 12);
  tmp_json.emplace("name", "firmwareVersionInfo");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_manufacturer(&tmp);
  tmp_json.emplace("id", 13);
  tmp_json.emplace("name", "manufacturer");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  tmp_json.clear();
  rk_system_get_hardware_id(&tmp);
  tmp_json.emplace("id", 14);
  tmp_json.emplace("name", "hardwareId");
  tmp_json.emplace("value", tmp);
  tmp_json.emplace("ro", "true");
  device_info.push_back(tmp_json);

  delete[] tmp;

  return device_info;
}

int device_info_set(nlohmann::json param) {
  std::string value = param.at("value").dump().c_str();
  value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"

  if (!param.at("name").dump().compare("\"deviceName\"")) {
    rk_system_set_deivce_name(value.c_str());
  } else if (!param.at("name").dump().compare("\"telecontrolID\"")) {
    rk_system_set_telecontrol_id(value.c_str());
  } else {
    minilog_debug("device_info_set, no support set %s\n",
                  param.at("name").dump().c_str());
  }

  return 0;
}

nlohmann::json user_info_get() {
  nlohmann::json user_info;
  nlohmann::json tmp_json;
  char *tmp = new char[256];
  int value;
  int user_num;

  rk_system_get_user_num(&user_num);
  minilog_debug("user_num is %d\n", user_num);
  for (int i = 0; i < user_num; i++) {
    tmp_json.clear();
    rk_system_get_user_name(i, &tmp);
    tmp_json.emplace("id", i);
    tmp_json.emplace("sUserName", tmp);
    rk_system_get_password(i, &tmp);
    tmp_json.emplace("sPassword", tmp);
    rk_system_get_user_level(i, &value);
    tmp_json.emplace("iUserLevel", value);
    tmp_json.emplace("iFixed", 1);     // temporarily useless
    tmp_json.emplace("iAuthLevel", 1); // temporarily useless
    user_info.push_back(tmp_json);
  }
  delete[] tmp;

  return user_info;
}

nlohmann::json is_register_user(nlohmann::json put_info) {
  int id_mark;
  std::string user_name = put_info.at("sUserName");
  std::string password = put_info.at("sPassword");
  nlohmann::json res;
  nlohmann::json user_info = user_info_get();
  for (auto &x : nlohmann::json::iterator_wrapper(user_info)) {
    id_mark = x.value().at("id");
    std::string db_name = x.value().at("sUserName");
    if (!user_name.compare(db_name)) {
      std::string db_password = x.value().at("sPassword");
      if (!password.compare(db_password)) {
        int auth = x.value().at("iUserLevel");
        res.emplace("status", id_mark);
        res.emplace("auth", auth);
        return res;
      } else {
        res.emplace("status", -1);
        res.emplace("auth", 0);
        return res;
      }
    }
  }
  // |status| - 1 = id for add
  id_mark = -2 - id_mark;
  res.emplace("status", id_mark);
  res.emplace("auth", 0);

  return res;
}

void create_new_user(std::string user_name, std::string password,
                     int user_level) {
  int user_num;
  rk_system_get_user_num(&user_num);
  rk_system_add_user(user_num, user_level, user_name.c_str(), password.c_str());
}

// -1:admin password wrong, >=0:ok, -2:find no item have the same user name
int user_alter_verify(nlohmann::json put_info) {
  char *str;
  int user_id = 0;
  std::string in_admin_password = put_info.at("sPassword");
  nlohmann::json user_info = user_info_get();
  std::string db_admin_password = user_info.at(0).at("sPassword");
  if (in_admin_password.compare(db_admin_password)) {
    return -1;
  }
  std::string user_name = put_info.at("newUserName");
  for (auto &x : nlohmann::json::iterator_wrapper(user_info)) {
    std::string db_name = x.value().at("sUserName");
    if (!user_name.compare(db_name)) {
      user_id = x.value().at("id");
      return user_id;
    }
  }

  return -2;
}

nlohmann::json GetPara(std::string para_name) {
  nlohmann::json content;
  if (!para_name.compare("screenshot")) {
    content =
        "{\"dynamic\":{\"id\":{\"0\":{\"iShotInterval\":{\"for\":\"timing\","
        "\"range\":{\"max\":604800000,\"min\":1000},\"type\":\"range\"},"
        "\"timeUnit\":{\"for\":\"timing\",\"options\":[\"seconds\",\"minutes\","
        "\"hours\",\"days\"],\"type\":\"options\"}},\"1\":{\"iShotInterval\":{"
        "\"for\":\"timing\",\"range\":{\"max\":65535,\"min\":1000},\"type\":"
        "\"range\"},\"timeUnit\":{\"for\":\"timing\",\"options\":[\"seconds\","
        "\"minutes\"],\"type\":\"options\"}}}},\"relation\":{\"iImageQuality\":"
        "{\"50\":\"low\",\"70\":\"middle\",\"90\":\"high\"}},\"static\":{"
        "\"iImageQuality\":{\"options\":[50,70,90],\"type\":\"options\"},"
        "\"iShotNumber\":{\"range\":{\"max\":120,\"min\":1},\"type\":\"range\"}"
        ",\"sImageType\":{\"options\":[\"JPEG\"],\"type\":\"options\"},"
        "\"sResolution\":{\"refer\":[4,\"para\",\"dynamic\",\"sStreamType\","
        "\"mainStream\",\"sResolution\"],\"type\":\"refer\"}}}";
  } else if (!para_name.compare("screenshot-schedule")) {
    content = "[{\"color\":\"#87CEEB\",\"name\":\"timing\"}]";
  } else if (!para_name.compare("video-plan-schedule")) {
    content = "[{\"color\":\"#87CEEB\",\"name\":\"timing\"},{\"color\":\"#"
              "74B558\",\"name\":\"motion-detect\"},{\"color\":\"#D71820\","
              "\"name\":\"alarm\"},{\"color\":\"#E58705\",\"name\":"
              "\"motionOrAlarm\"},{\"color\":\"#B9E2FE\",\"name\":"
              "\"motionAndAlarm\"},{\"color\":\"#AA6FFF\",\"name\":\"event\"}]";
  } else if (!para_name.compare("smart-cover")) {
    content = "";
  } else if (!para_name.compare("video-encoder")) {
    char cap[4096];
    rk_system_capability_get_video(cap);
    if (strlen(cap) == 0) {
      content =
          "{\"disabled\":[{\"name\":\"sStreamType\",\"options\":{\"subStream\":"
          "{"
          "\"sOutputDataType\":\"H.264\",\"sSmart\":\"close\"},\"thirdStream\":"
          "{"
          "\"sSmart\":\"close\"}},\"type\":\"disabled/"
          "limit\"},{\"name\":\"sSmart\",\"options\":{\"open\":{\"iGOP\":null,"
          "\"iStreamSmooth\":null,\"sH264Profile\":null,\"sRCMode\":null,"
          "\"sRCQuality\":null,\"sSVC\":null}},\"type\":\"disabled\"},{"
          "\"name\":"
          "\"sRCMode\",\"options\":{\"CBR\":{\"sRCQuality\":null}},\"type\":"
          "\"disabled\"},{\"name\":\"sOutputDataType\",\"options\":{\"H.265\":{"
          "\"sH264Profile\":null}},\"type\":\"disabled\"},{\"name\":"
          "\"unspport\","
          "\"options\":{\"iStreamSmooth\":null,\"sSVC\":null,\"sVideoType\":"
          "null}"
          ",\"type\":\"disabled\"}],\"dynamic\":{\"sSmart\":{\"open\":{"
          "\"iMinRate\":{\"dynamicRange\":{\"max\":\"iMaxRate\",\"maxRate\":1,"
          "\"min\":\"iMaxRate\",\"minRate\":0.125},\"type\":\"dynamicRange\"}}}"
          ","
          "\"sStreamType\":{\"mainStream\":{\"iMaxRate\":{\"options\":[256,512,"
          "1024,2048,3072,4096,6144,8192,12288,16384],\"type\":\"options\"},"
          "\"sResolution\":{\"options\":[\"2560*1520\",\"1280*720\",\"640*"
          "480\"],"
          "\"type\":\"options\"}}}},\"layout\":{\"encoder\":[\"sStreamType\","
          "\"sVideoType\",\"sResolution\",\"sRCMode\",\"sRCQuality\","
          "\"sFrameRate\",\"sOutputDataType\",\"sSmart\",\"sH264Profile\","
          "\"sSVC\",\"iMaxRate\",\"iMinRate\",\"iGOP\",\"iStreamSmooth\"]},"
          "\"static\":{\"iGOP\":{\"range\":{\"max\":400,\"min\":0},\"type\":"
          "\"range\"},\"iStreamSmooth\":{\"range\":{\"max\":100,\"min\":1,"
          "\"step\":1},\"type\":\"range\"},\"sFrameRate\":{\"dynamicRange\":{"
          "\"max\":\"sFrameRateIn\",\"maxRate\":1},\"options\":[\"1/16\",\"1/"
          "8\",\"1/4\",\"1/"
          "2\",\"1\",\"2\",\"4\",\"6\",\"8\",\"10\",\"12\",\"14\",\"16\","
          "\"18\","
          "\"20\",\"25\",\"30\"],\"type\":\"options/"
          "dynamicRange\"},\"sH264Profile\":{\"options\":[\"high\",\"main\","
          "\"baseline\"],\"type\":\"options\"},\"sOutputDataType\":{"
          "\"options\":["
          "\"H.264\",\"H.265\"],\"type\":\"options\"},\"sRCMode\":{\"options\":"
          "["
          "\"CBR\",\"VBR\"],\"type\":\"options\"},\"sRCQuality\":{\"options\":["
          "\"lowest\",\"lower\",\"low\",\"medium\",\"high\",\"higher\","
          "\"highest\"],\"type\":\"options\"},\"sSVC\":{\"options\":[\"open\","
          "\"close\"],\"type\":\"options\"},\"sSmart\":{\"options\":[\"open\","
          "\"close\"],\"type\":\"options\"},\"sStreamType\":{\"options\":["
          "\"mainStream\",\"subStream\",\"thirdStream\"],\"type\":\"options\"},"
          "\"sVideoType\":{\"options\":[\"videoStream\",\"compositeStream\"],"
          "\"type\":\"options\"}}}";
    } else {
      content = cap;
    }
  } else if (!para_name.compare("isp")) {
    char cap[4096];
    rk_system_capability_get_image_adjustment(cap);
    content.emplace("image_adjustment", cap);
    rk_system_capability_get_image_blc(cap);
    content.emplace("image_blc", cap);
    rk_system_capability_get_image_enhancement(cap);
    content.emplace("image_enhancement", cap);
    rk_system_capability_get_image_exposure(cap);
    content.emplace("image_exposure", cap);
    rk_system_capability_get_image_night_to_day(cap);
    content.emplace("image_night_to_day", cap);
    rk_system_capability_get_image_video_adjustment(cap);
    content.emplace("image_video_adjustment", cap);
    rk_system_capability_get_image_white_blance(cap);
    content.emplace("image_white_blance", cap);
    if (strlen(cap) == 0) {
      content = R"(
        {
          "image_adjustment":
          "{\"layout\":{\"image_adjustment\":[\"iBrightness\",\"iContrast\",\"iSaturation\",\"iSharpness\",\"iHue\"]},\"static\":{\"iBrightness\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iContrast\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iHue\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iSaturation\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iSharpness\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}",
          "image_blc":
          "{\"disabled\":[{\"name\":\"sHLC\",\"options\":{\"open\":{\"sBLCRegion\":null}},\"type\":\"disabled\"},{\"name\":\"sBLCRegion\",\"options\":{\"open\":{\"iDarkBoostLevel\":null,\"iHLCLevel\":null,\"sHLC\":null}},\"type\":\"disabled\"}],\"dynamic\":{\"sBLCRegion\":{\"open\":{\"iBLCStrength\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}},\"sHDR\":{\"HDR2\":{\"iHDRLevel\":{\"options\":[1,2,3,4,5],\"type\":\"options\"}},\"close\":{\"sBLCRegion\":{\"options\":[\"close\",\"open\"],\"type\":\"options\"},\"sHLC\":{\"options\":[\"close\",\"open\"],\"type\":\"options\"}}},\"sHLC\":{\"open\":{\"iDarkBoostLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iHLCLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}},\"sWDR\":{\"open\":{\"iWDRLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_blc\":[\"sHDR\",\"iHDRLevel\",\"sBLCRegion\",\"iBLCStrength\",\"sHLC\",\"iHLCLevel\",\"iDarkBoostLevel\"]},\"static\":{\"sHDR\":{\"options\":[\"close\",\"HDR2\"],\"type\":\"options\"}}}",
          "image_enhancement":
          "{\"dynamic\":{\"sDehaze\":{\"open\":{\"iDehazeLevel\":{\"range\":{\"max\":10,\"min\":0,\"step\":1},\"type\":\"range\"}}},\"sDistortionCorrection\":{\"FEC\":{\"iFecLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}},\"LDCH\":{\"iLdchLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}},\"sNoiseReduceMode\":{\"2dnr\":{\"iSpatialDenoiseLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}},\"3dnr\":{\"iTemporalDenoiseLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}},\"mixnr\":{\"iSpatialDenoiseLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iTemporalDenoiseLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_enhancement\":[\"sNoiseReduceMode\",\"iSpatialDenoiseLevel\",\"iTemporalDenoiseLevel\",\"sDehaze\",\"iDehazeLevel\",\"sGrayScaleMode\",\"sDistortionCorrection\",\"iLdchLevel\",\"iFecLevel\",\"iImageRotation\"]},\"static\":{\"iImageRotation\":{\"options\":[0,90,270],\"type\":\"options\"},\"sDIS\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"},\"sDehaze\":{\"options\":[\"open\",\"close\",\"auto\"],\"type\":\"options\"},\"sDistortionCorrection\":{\"options\":[\"LDCH\",\"close\"],\"type\":\"options\"},\"sFEC\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"},\"sGrayScaleMode\":{\"options\":[\"[0-255]\"],\"type\":\"options\"},\"sNoiseReduceMode\":{\"options\":[\"close\",\"2dnr\",\"3dnr\",\"mixnr\"],\"type\":\"options\"}}}",
          "image_exposure":
          "{\"dynamic\":{\"sExposureMode\":{\"auto\":{\"iAutoIrisLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}},\"manual\":{\"sExposureTime\":{\"options\":[\"1\",\"1/3\",\"1/6\",\"1/12\",\"1/25\",\"1/50\",\"1/100\",\"1/150\",\"1/200\",\"1/250\",\"1/500\",\"1/750\",\"1/1000\",\"1/2000\",\"1/4000\",\"1/10000\",\"1/100000\"],\"type\":\"options\"},\"sGainMode\":{\"options\":[\"auto\",\"manual\"],\"type\":\"options\"}}},\"sGainMode\":{\"manual\":{\"iExposureGain\":{\"range\":{\"max\":100,\"min\":1,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_exposure\":[\"sExposureMode\",\"sExposureTime\",\"sGainMode\",\"iExposureGain\"]},\"static\":{\"sExposureMode\":{\"options\":[\"auto\",\"manual\"],\"type\":\"options\"}}}",
          "image_night_to_day":
          "{\"disabled\":[{\"name\":\"sNightToDay\",\"options\":{\"day\":{\"iLightBrightness\":null,\"sFillLightMode\":null},\"night\":{\"iDarkBoostLevel\":null,\"iHDRLevel\":null,\"iHLCLevel\":null,\"sHDR\":null,\"sHLC\":\"close\"}},\"type\":\"disabled\"}],\"dynamic\":{\"sNightToDay\":{\"auto\":{\"iNightToDayFilterLevel\":{\"options\":[0,1,2,3,4,5,6,7],\"type\":\"options\"},\"iNightToDayFilterTime\":{\"range\":{\"max\":10,\"min\":3,\"step\":1},\"type\":\"range\"}},\"schedule\":{\"sDawnTime\":{\"input\":\"time\",\"type\":\"input\"},\"sDuskTime\":{\"input\":\"time\",\"type\":\"input\"}}},\"sOverexposeSuppress\":{\"open\":{\"sOverexposeSuppressType\":{\"options\":[\"auto\",\"manual\"],\"type\":\"options\"}}},\"sOverexposeSuppressType\":{\"manual\":{\"iDistanceLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_night_to_day\":[\"sNightToDay\",\"iNightToDayFilterLevel\",\"iNightToDayFilterTime\",\"sDawnTime\",\"sDuskTime\",\"sFillLightMode\",\"iLightBrightness\"]},\"static\":{\"iLightBrightness\":{\"range\":{\"max\":100,\"min\":0,\"step\":10},\"type\":\"range\"},\"sNightToDay\":{\"options\":[\"day\",\"night\"],\"type\":\"options\"},\"sFillLightMode\":{\"type\":\"options\",\"options\":[\"IR\"]}}}",
          "image_video_adjustment":
          "{\"layout\":{\"image_video_adjustment\":[\"sPowerLineFrequencyMode\",\"sImageFlip\"]},\"static\":{\"sImageFlip\":{\"options\":[\"close\",\"flip\",\"mirror\",\"centrosymmetric\"],\"type\":\"options\"},\"sPowerLineFrequencyMode\":{\"options\":[\"PAL(50HZ)\",\"NTSC(60HZ)\"],\"type\":\"options\"},\"sSceneMode\":{\"options\":[\"indoor\",\"outdoor\"],\"type\":\"options\"}}}",
          "image_white_blance":
          "{\"dynamic\":{\"sWhiteBlanceStyle\":{\"manualWhiteBalance\":{\"iWhiteBalanceBlue\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iWhiteBalanceGreen\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iWhiteBalanceRed\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_white_blance\":[\"sWhiteBlanceStyle\",\"iWhiteBalanceRed\",\"iWhiteBalanceGreen\",\"iWhiteBalanceBlue\"]},\"static\":{\"sWhiteBlanceStyle\":{\"options\":[\"manualWhiteBalance\",\"autoWhiteBalance\",\"lockingWhiteBalance\",\"fluorescentLamp\",\"incandescent\",\"warmLight\",\"naturalLight\"],\"type\":\"options\"}}}"
        }
      )"_json;
    }
  } else {
    content =
        "{\"auth\":4,\"item\":[{\"auth\":4,\"name\":\"preview\"},{\"auth\":-1,"
        "\"item\":[{\"auth\":4,\"item\":[{\"auth\":0,\"name\":\"delete\"}],"
        "\"name\":\"videoRecord\"},{\"auth\":4,\"item\":[{\"auth\":0,\"name\":"
        "\"delete\"}],\"name\":\"pictureRecord\"}],\"name\":\"download\"},{"
        "\"auth\":-1,\"item\":[{\"auth\":4,\"item\":[{\"auth\":4,\"item\":[{"
        "\"auth\":1,\"name\":\"modify\"}],\"name\":\"ListManagement\"},{"
        "\"auth\":1,\"name\":\"AddOne\"},{\"auth\":1,\"name\":\"BatchInput\"}],"
        "\"name\":\"MemberList\"},{\"auth\":-1,\"item\":[{\"auth\":4,\"item\":["
        "{\"auth\":0,\"name\":\"modify\"}],\"name\":\"SnapShot\"}],\"name\":"
        "\"SnapShot\"},{\"auth\":-1,\"item\":[{\"auth\":4,\"item\":[{\"auth\":"
        "0,\"name\":\"modify\"}],\"name\":\"Control\"}],\"name\":\"Control\"},{"
        "\"auth\":1,\"item\":[{\"auth\":1,\"name\":\"ParaConfig\"}],\"name\":"
        "\"Config\"}],\"name\":\"face\"},{\"auth\":-1,\"item\":[{\"auth\":1,"
        "\"item\":[{\"auth\":1,\"name\":\"FacePara\"},{\"auth\":1,\"name\":"
        "\"ROI\"}],\"name\":\"Config\"}],\"name\":\"face-para\"},{\"auth\":-1,"
        "\"item\":[{\"auth\":4,\"item\":[{\"auth\":4,\"item\":[{\"auth\":1,"
        "\"name\":\"modify\"}],\"name\":\"MemberList\"},{\"auth\":1,\"name\":"
        "\"AddOne\"},{\"auth\":1,\"name\":\"BatchInput\"},{\"auth\":4,\"item\":"
        "[{\"auth\":0,\"name\":\"modify\"}],\"name\":\"SnapShot\"},{\"auth\":4,"
        "\"item\":[{\"auth\":0,\"name\":\"modify\"}],\"name\":\"Control\"}],"
        "\"name\":\"Manage\"}],\"name\":\"face-manage\"},{\"auth\":1,\"item\":["
        "{\"auth\":1,\"item\":[{\"auth\":1,\"item\":[{\"auth\":1,\"name\":"
        "\"basic\"},{\"auth\":1,\"name\":\"time\"}],\"name\":\"Settings\"},{"
        "\"auth\":1,\"item\":[{\"auth\":1,\"name\":\"upgrade\"},{\"auth\":-1,"
        "\"name\":\"log\"}],\"name\":\"Maintain\"},{\"auth\":-1,\"item\":[{"
        "\"auth\":-1,\"name\":\"authentication\"},{\"auth\":-1,\"name\":"
        "\"ipAddrFilter\"},{\"auth\":-1,\"name\":\"securityService\"}],"
        "\"name\":\"Security\"},{\"auth\":0,\"name\":\"User\"}],\"name\":"
        "\"System\"},{\"auth\":1,\"item\":[{\"auth\":1,\"item\":[{\"auth\":1,"
        "\"name\":\"TCPIP\"},{\"auth\":-1,\"name\":\"DDNS\"},{\"auth\":-1,"
        "\"name\":\"PPPoE\"},{\"auth\":1,\"name\":\"Port\"},{\"auth\":-1,"
        "\"name\":\"uPnP\"}],\"name\":\"Basic\"},{\"auth\":1,\"item\":[{"
        "\"auth\":1,\"name\":\"Wi-Fi\"},{\"auth\":-1,\"name\":\"SMTP\"},{"
        "\"auth\":-1,\"name\":\"FTP\"},{\"auth\":-1,\"name\":\"eMail\"},{"
        "\"auth\":-1,\"name\":\"Cloud\"},{\"auth\":-1,\"name\":\"Protocol\"},{"
        "\"auth\":-1,\"name\":\"QoS\"},{\"auth\":-1,\"name\":\"Https\"}],"
        "\"name\":\"Advanced\"}],\"name\":\"Network\"},{\"auth\":1,\"item\":[{"
        "\"auth\":1,\"name\":\"Encoder\"},{\"auth\":1,\"name\":"
        "\"AdvancedEncoder\"},{\"auth\":1,\"name\":\"ROI\"},{\"auth\":1,"
        "\"name\":\"RegionCrop\"}],\"name\":\"Video\"},{\"auth\":1,\"item\":[{"
        "\"auth\":1,\"name\":\"AudioParam\"}],\"name\":\"Audio\"},{\"auth\":1,"
        "\"item\":[{\"auth\":1,\"name\":\"DisplaySettings\"},{\"auth\":1,"
        "\"name\":\"OSDSettings\"},{\"auth\":1,\"name\":\"PrivacyCover\"},{"
        "\"auth\":1,\"name\":\"PictureMask\"}],\"name\":\"Image\"},{\"auth\":1,"
        "\"item\":[{\"auth\":1,\"name\":\"MotionDetect\"},{\"auth\":1,\"name\":"
        "\"IntrusionDetection\"},{\"auth\":-1,\"name\":\"AlarmInput\"},{"
        "\"auth\":-1,\"name\":\"AlarmOutput\"},{\"auth\":-1,\"name\":"
        "\"Abnormal\"}],\"name\":\"Event\"},{\"auth\":1,\"item\":[{\"auth\":1,"
        "\"item\":[{\"auth\":1,\"name\":"
        "\"ScreenshotPlan\"},{\"auth\":1,\"name\":\"ScreenshotPara\"}],"
        "\"name\":\"PlanSettings\"},{\"auth\":1,\"item\":[{\"auth\":1,\"name\":"
        "\"HardDiskManagement\"},{\"auth\":-1,\"name\":\"NAS\"},{\"auth\":-1,"
        "\"name\":\"CloudStorage\"}],\"name\":\"StorageManage\"}],\"name\":"
        "\"Storage\"},{\"auth\":1,\"item\":[{\"auth\":1,\"name\":\"MarkCover\"}"
        ",{\"auth\":-1,\"name\":\"MaskArea\"},{\"auth\":-1,\"name\":"
        "\"RuleSettings\"},{\"auth\":-1,\"name\":\"AdvancedCFG\"}],\"name\":"
        "\"Intel\"},{\"auth\":-1,\"item\":[{\"auth\":-1,\"name\":"
        "\"GateConfig\"},{\"auth\":-1,\"name\":\"ScreenConfig\"}],\"name\":"
        "\"Peripherals\"}],\"name\":\"config\"},{\"auth\":4,\"name\":\"about\"}"
        "],\"name\":\"header\"}";
  }
  return content;
}

void SystemApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  char *str;
  std::string path_api_resource;
  std::string path_specific_resource;
  std::string para_channel;
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  /* Get Path Information */
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_specific_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
    pos_first = path_specific_resource.find_first_of("/");
    if (pos_first != -1)
      para_channel = path_specific_resource.substr(
          pos_first + 1, path_specific_resource.size() + 1);
  }

  if (Req.Method == "GET") {
    if (!path_specific_resource.compare("device-info")) {
      content = device_info_get();
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("remain-space")) {
      struct statfs diskInfo;
      statfs("/userdata/", &diskInfo);
      // Free space for non-root users
      unsigned long long availableDisk = diskInfo.f_bavail * diskInfo.f_bsize;
      content.emplace("availableDisk", availableDisk);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("para") != std::string::npos) {
      if (para_channel.empty()) {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        content = GetPara(para_channel);
        if (!content.empty()) {
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else if (!path_specific_resource.compare("login")) {
      content = user_info_get();
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    nlohmann::json system_config = Req.PostObject; /* must be json::object */
    if (!path_specific_resource.compare("device-info")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!Req.PostObject.at("ro").dump().compare("\"true\"")) {
        Resp.setErrorResponse(HttpStatus::kBadRequest, "Value is read-only!");
      } else {
        /* Set */
        device_info_set(Req.PostObject);
        /* Get new info */
        // content = device_info_get();
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    } else if (!path_specific_resource.compare("firmware-upgrade")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      // int total_length = 0;
      int transmitted_length;
      int begin_position;
      int end_position;
      int current_transfer_length;
      std::string progress_rate = "";
      // for (auto p : Req.Params) {
      //   if (p.Key == "total-length")
      //     total_length = atoi(p.Value.c_str());
      // }
      for (auto p : Req.Params) {
        if ((p.Key == "upload-type") && (p.Value == "resumable")) {
          // Create a transmission resource
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.addHeader("X-Location", create_fireware_location());
        } else if (p.Key == "id") {
          std::ofstream file;
          std::string file_id = p.Value;

          file.open("/data/" + file_id, std::ofstream::app); // append write
          if (!file) {
            Resp.setErrorResponse(HttpStatus::kNotFound, "Not Found");
          } else if (Req.ContentLength == 0) {
            // Query the current progress
            file.seekp(0, std::ofstream::end);
            transmitted_length = file.tellp();
            progress_rate = "bytes 0-" + std::to_string(transmitted_length - 1);
            Resp.setHeader(HttpStatus::kResumeIncomplete, "Resume Incomplete");
            content.emplace("range", progress_rate);
            Resp.setApiData(content);
          } else {
            // Transfer
            begin_position = file.tellp();
            file << Req.PostData;
            end_position = file.tellp();
            current_transfer_length = end_position - begin_position;
            // transmitted_length is not necessarily equal to end_position,
            // it is possible to change a paragraph in the middle of the file
            file.seekp(0, std::ofstream::end);
            transmitted_length = file.tellp();
            if (Req.ContentLength != current_transfer_length) {
              // Request for retransmission due to transmission error
              progress_rate = "bytes 0-" + std::to_string(begin_position - 1);
              Resp.setHeader(HttpStatus::kResumeIncomplete,
                             "Resume Incomplete");
            } else {
              progress_rate =
                  "bytes 0-" + std::to_string(transmitted_length - 1);
              if (Req.ContentLength < 512 * 1024) {
                // Transfer completed
                Resp.setHeader(HttpStatus::kCreated, "Created");
              } else if (Req.ContentLength == 512 * 1024) {
                Resp.setHeader(HttpStatus::kResumeIncomplete,
                               "Resume Incomplete");
              } else {
                Resp.setHeader(HttpStatus::kResumeIncomplete,
                               "More than 512KB!");
              }
            }
            content.emplace("range", progress_rate);
            Resp.setApiData(content);
          }
          file.close();
        } else if (p.Key == "start") {
          int fireware_id = stoi(p.Value);
          std::string path = "/userdata/" + std::to_string(fireware_id);
          rk_system_upgrade(path.c_str());
          Resp.setHeader(HttpStatus::kOk, "OK");
        }
      }
    } else if (!path_specific_resource.compare("reboot")) {
      rk_system_reboot();
    } else if (!path_specific_resource.compare("pre-factory-reset")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      //       // for face db
      // #ifdef MEDIASERVER_ROCKFACE
      //       char *str = storage_manager_get_media_path();
      //       std::string mount_path =
      //       nlohmann::json::parse(str).at("sMountPath");
      //       minilog_debug("format when reset, path is %s\n", (char
      //       *)mount_path.c_str());
      //       storage_manager_diskformat((char *)mount_path.c_str(), (char
      //       *)"fat32");
      //       mediaserver_clear_face_db();
      // #else
      //       dbserver_face_reset((char *)TABLE_FACE_LIST);
      // #endif
    } else if (!path_specific_resource.compare("factory-reset")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      rk_system_factory_reset();
    } else if (!path_specific_resource.compare("export-log")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      const char *path = (const char *)"/userdata/export.log";
      rk_system_export_log(path);
      std::string ipv4_address = ipv4_address_get();
      std::string location = "http://" + ipv4_address + path;
      content.emplace("location", location);
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("export-db")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      const char *path = (const char *)"/userdata/export.ini";
      rk_system_export_db(path);
      std::string ipv4_address = ipv4_address_get();
      std::string location = "http://" + ipv4_address + path;
      content.emplace("location", location);
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("import-db")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (Req.Params.empty()) {
        int end_position = 0;
        // overwrite
        std::ofstream db_file("/userdata/import.ini", std::ofstream::out);
        db_file << Req.Files.at(0).getData();
        end_position = db_file.tellp();
        db_file.close();
        if (end_position == Req.Files.at(0).getDataLength()) {
          Resp.setHeader(HttpStatus::kOk, "OK");
        } else {
          Resp.setErrorResponse(HttpStatus::kBadRequest, "db upload failed!");
        }
      }
      for (auto p : Req.Params) {
        if (p.Key == "start")
          rk_system_import_db((const char *)"/userdata/import.ini");
      }
    } else if (path_specific_resource.find("login") != std::string::npos) {
      if (para_channel.empty()) { /* path is login */
        std::string username = system_config.at("sUserName");
        content = is_register_user(system_config);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
        int auth = content.at("auth");
        int status = content.at("status");
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
        long expire_time = EXPIRE_SECONDS;
        if (status >= 0) {
          for (auto p : Req.Params) {
            if (p.Key == "expire") {
              std::string expire_stirng = p.Value;
              if (!expire_stirng.compare("day")) {
                expire_time = 86400;
              } else if (!expire_stirng.compare("week")) {
                expire_time = 604800;
              } else if (!expire_stirng.compare("month")) {
                expire_time = 2592000;
              }
              break;
            }
          }
          std::string token = jwt_token_get(username, auth, expire_time);
          Resp.setCookie("token", token, expire_time);
        }
      } else if (!para_channel.compare("modify")) { /* path is login/modify */
#ifdef ENABLE_JWT
        if (user_level > 0) {
          Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
          return;
        }
#endif
        int user_id = user_alter_verify(system_config);
        if (user_id >= 0) {
          std::string new_name = system_config.at("newUserName");
          std::string new_pw = system_config.at("newPassword");
          int user_level = system_config.at("iUserLevel");
          // rk_system_set_user_name(user_id, new_name.c_str());
          rk_system_set_password(user_id, new_pw.c_str());
          rk_system_set_user_level(user_id, user_level);
        }
        content.emplace("status", user_id);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (!para_channel.compare("add")) { /* path is login/add */
#ifdef ENABLE_JWT
        if (user_level > 0) {
          Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
          return;
        }
#endif
        int res = user_alter_verify(system_config);
        if (res == -2) {
          std::string new_name = system_config.at("newUserName");
          std::string new_pw = system_config.at("newPassword");
          int new_user_level =
              atoi(system_config.at("iUserLevel").dump().c_str());
          create_new_user(new_name, new_pw, new_user_level);
        }
        content.emplace("status", res);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (!para_channel.compare("delete")) { /* path is login/delete */
#ifdef ENABLE_JWT
        if (user_level > 0) {
          Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
          return;
        }
#endif
        int user_id = user_alter_verify(system_config);
        if (user_id > 0) {
          rk_system_del_user(user_id);
        }
        content.emplace("status", user_id);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
    }
  } else if (Req.Method == "DELETE") {
    if (!path_specific_resource.compare("firmware-upgrade")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!Req.Params.empty()) {
        for (auto p : Req.Params) {
          if (p.Key == "id") {
            std::string file_id_s = p.Value;
            std::string file_name = "/data/" + file_id_s;
            if (!remove(file_name.c_str()))
              Resp.setHeader(HttpStatus::kOk, "OK");
          }
        }
      } else {
        Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else // #ifdef USE_RKIPC

nlohmann::json is_register_user(nlohmann::json put_info) {
  std::string user_name = put_info.at("sUserName");
  std::string password = put_info.at("sPassword");
  char *str;
  nlohmann::json res;
  int id_mark;
  str = dbserver_system_get((char *)TABLE_SYSTEM_USER);
  nlohmann::json user_info = nlohmann::json::parse(str).at("jData");
  for (auto &x : nlohmann::json::iterator_wrapper(user_info)) {
    id_mark = x.value().at("id");
    std::string db_name = x.value().at("sUserName");
    if (!user_name.compare(db_name)) {
      std::string db_password = x.value().at("sPassword");
      if (!password.compare(db_password)) {
        int auth = x.value().at("iUserLevel");
        res.emplace("status", id_mark);
        res.emplace("auth", auth);
        return res;
      } else {
        res.emplace("status", -1);
        res.emplace("auth", 0);
        return res;
      }
    }
  }
  // |status| - 1 = id for add
  id_mark = -2 - id_mark;
  res.emplace("status", id_mark);
  res.emplace("auth", 0);
  return res;
}

void create_new_user(nlohmann::json user_info) {
  int user_id = 0;
  char *str;
  str =
      dbserver_sql((char *)"select * from SystemUser ORDER BY id DESC LIMIT 1",
                   (char *)DBSERVER_SYSTEM_INTERFACE);
  nlohmann::json user_last = nlohmann::json::parse(str).at("jData");
  if (!user_last.empty()) {
    user_id = user_last.at(0).at("id");
    user_id++;
  }
  dbserver_system_set((char *)TABLE_SYSTEM_USER,
                      (char *)user_info.dump().c_str(), user_id);
}

// -1:admin password wrong, >=0:ok, -2:find no item have the same user name
int user_alter_verify(nlohmann::json put_info) {
  char *str;
  int user_id = 0;
  std::string in_admin_password = put_info.at("sPassword");
  str = dbserver_sql(
      (char *)"select sPassword from SystemUser WHERE sUserName='admin'",
      (char *)DBSERVER_SYSTEM_INTERFACE);
  nlohmann::json admin_json = nlohmann::json::parse(str).at("jData").at(0);
  std::string db_admin_password = admin_json.at("sPassword");
  if (in_admin_password.compare(db_admin_password)) {
    return -1;
  }
  std::string in_user_name = put_info.at("newUserName");
  std::string cmd =
      "select id from SystemUser WHERE sUserName='" + in_user_name + "'";
  str = dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_SYSTEM_INTERFACE);
  nlohmann::json new_json = nlohmann::json::parse(str).at("jData");
  if (!new_json.empty()) {
    user_id = new_json.at(0).at("id");
  } else {
    user_id = -2;
  }
  return user_id;
}

nlohmann::json GetPara(std::string para_name) {
  std::string para_key = "para";
  char *str = dbserver_system_get((char *)TABLE_SYSTEM_PARA);
  nlohmann::json data = nlohmann::json::parse(str).at("jData");
  nlohmann::json content;
  if (!para_name.compare("screenshot")) {
    content = data.at(1).at(para_key);
  } else if (!para_name.compare("screenshot-schedule")) {
    content = data.at(2).at(para_key);
  } else if (!para_name.compare("video-plan-schedule")) {
    content = data.at(3).at(para_key);
  } else if (!para_name.compare("smart-cover")) {
    content = data.at(14).at(para_key);
  } else if (!para_name.compare("video-encoder")) {
    content = data.at(4).at(para_key);
  } else if (!para_name.compare("isp")) {
    for (int i = 7; i <= 13; i++) {
      std::string name = data.at(i).at("name");
      content[name] = data.at(i).at(para_key);
    }
  } else {
    str = dbserver_system_para_get_by_name((char *)para_name.c_str());
    data = nlohmann::json::parse(str).at("jData");
    if (!data.empty()) {
      content = data.at(0).at(para_key);
    }
  }
  return content;
}

void SystemApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  char *str;
  std::string path_api_resource;
  std::string path_specific_resource;
  std::string para_channel;
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  /* Get Path Information */
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_specific_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
    pos_first = path_specific_resource.find_first_of("/");
    if (pos_first != -1)
      para_channel = path_specific_resource.substr(
          pos_first + 1, path_specific_resource.size() + 1);
  }

  if (Req.Method == "GET") {
    if (!path_specific_resource.compare("device-info")) {
      str = dbserver_system_get((char *)TABLE_SYSTEM_DEVICE_INFO);
      content = nlohmann::json::parse(str).at("jData");
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("remain-space")) {
      struct statfs diskInfo;
      statfs("/userdata/", &diskInfo);
      // Free space for non-root users
      unsigned long long availableDisk = diskInfo.f_bavail * diskInfo.f_bsize;
      content.emplace("availableDisk", availableDisk);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("para") != std::string::npos) {
      if (para_channel.empty()) {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        content = GetPara(para_channel);
        if (!content.empty()) {
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else if (!path_specific_resource.compare("login")) {
      /* path is system/login */
      str = dbserver_sql(
          (char
               *)"SELECT id, sUserName, iAuthLevel, iUserLevel FROM SystemUser",
          (char *)DBSERVER_SYSTEM_INTERFACE);
      content = nlohmann::json::parse(str).at("jData");
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    nlohmann::json system_config = Req.PostObject; /* must be json::object */
    if (!path_specific_resource.compare("device-info")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      int id = system_config.at("id");
      str = dbserver_system_get((char *)TABLE_SYSTEM_DEVICE_INFO);
      nlohmann::json cfg_old = nlohmann::json::parse(str).at("jData").at(id);
      if (!cfg_old.at("ro").dump().compare("\"true\"")) {
        Resp.setErrorResponse(HttpStatus::kBadRequest, "Value is read-only!");
      } else {
        /* Erase can't change data */
        for (auto &x : nlohmann::json::iterator_wrapper(Req.PostObject)) {
          if (x.key() != "value")
            system_config.erase(x.key());
        }
        /* Set */
        dbserver_system_set((char *)TABLE_SYSTEM_DEVICE_INFO,
                            (char *)system_config.dump().c_str(), id);
        /* Get new info */
        str = dbserver_system_get((char *)TABLE_SYSTEM_DEVICE_INFO);
        content = nlohmann::json::parse(str).at("jData").at(id);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    } else if (!path_specific_resource.compare("firmware-upgrade")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      // int total_length = 0;
      int transmitted_length;
      int begin_position;
      int end_position;
      int current_transfer_length;
      std::string progress_rate = "";
      // for (auto p : Req.Params) {
      //   if (p.Key == "total-length")
      //     total_length = atoi(p.Value.c_str());
      // }
      for (auto p : Req.Params) {
        if ((p.Key == "upload-type") && (p.Value == "resumable")) {
          // Create a transmission resource
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.addHeader("X-Location", create_fireware_location());
        } else if (p.Key == "id") {
          std::ofstream file;
          std::string file_id = p.Value;

          file.open("/data/" + file_id, std::ofstream::app); // append write
          if (!file) {
            Resp.setErrorResponse(HttpStatus::kNotFound, "Not Found");
          } else if (Req.ContentLength == 0) {
            // Query the current progress
            file.seekp(0, std::ofstream::end);
            transmitted_length = file.tellp();
            progress_rate = "bytes 0-" + std::to_string(transmitted_length - 1);
            Resp.setHeader(HttpStatus::kResumeIncomplete, "Resume Incomplete");
            content.emplace("range", progress_rate);
            Resp.setApiData(content);
          } else {
            // Transfer
            begin_position = file.tellp();
            file << Req.PostData;
            end_position = file.tellp();
            current_transfer_length = end_position - begin_position;
            // transmitted_length is not necessarily equal to end_position,
            // it is possible to change a paragraph in the middle of the file
            file.seekp(0, std::ofstream::end);
            transmitted_length = file.tellp();
            if (Req.ContentLength != current_transfer_length) {
              // Request for retransmission due to transmission error
              progress_rate = "bytes 0-" + std::to_string(begin_position - 1);
              Resp.setHeader(HttpStatus::kResumeIncomplete,
                             "Resume Incomplete");
            } else {
              progress_rate =
                  "bytes 0-" + std::to_string(transmitted_length - 1);
              if (Req.ContentLength < 512 * 1024) {
                // Transfer completed
                Resp.setHeader(HttpStatus::kCreated, "Created");
              } else if (Req.ContentLength == 512 * 1024) {
                Resp.setHeader(HttpStatus::kResumeIncomplete,
                               "Resume Incomplete");
              } else {
                Resp.setHeader(HttpStatus::kResumeIncomplete,
                               "More than 512KB!");
              }
            }
            content.emplace("range", progress_rate);
            Resp.setApiData(content);
          }
          file.close();
        } else if (p.Key == "start") {
          int fireware_id = stoi(p.Value);
          std::string path = "/userdata/" + std::to_string(fireware_id);
          system_upgrade(path.c_str());
          Resp.setHeader(HttpStatus::kOk, "OK");
        }
      }
    } else if (!path_specific_resource.compare("reboot")) {
      system_reboot();
    } else if (!path_specific_resource.compare("pre-factory-reset")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
// for face db
#ifdef MEDIASERVER_ROCKFACE
      char *str = storage_manager_get_media_path();
      std::string mount_path = nlohmann::json::parse(str).at("sMountPath");
      minilog_debug("format when reset, path is %s\n",
                    (char *)mount_path.c_str());
      storage_manager_diskformat((char *)mount_path.c_str(), (char *)"fat32");
      mediaserver_clear_face_db();
#else
      dbserver_face_reset((char *)TABLE_FACE_LIST);
#endif
    } else if (!path_specific_resource.compare("factory-reset")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      system_factory_reset();
    } else if (!path_specific_resource.compare("export-log")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      const char *path = (const char *)"/userdata/export.log";
      system_export_log(path);
      std::string ipv4_address = ipv4_address_get();
      std::string location = "http://" + ipv4_address + path;
      content.emplace("location", location);
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("export-db")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      const char *path = (const char *)"/userdata/export.db";
      system_export_db(path);
      std::string ipv4_address = ipv4_address_get();
      std::string location = "http://" + ipv4_address + path;
      content.emplace("location", location);
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("import-db")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (Req.Params.empty()) {
        int end_position = 0;
        // overwrite
        std::ofstream db_file("/userdata/import.db", std::ofstream::out);
        db_file << Req.Files.at(0).getData();
        end_position = db_file.tellp();
        db_file.close();
        if (end_position == Req.Files.at(0).getDataLength()) {
          Resp.setHeader(HttpStatus::kOk, "OK");
        } else {
          Resp.setErrorResponse(HttpStatus::kBadRequest, "db upload failed!");
        }
      }
      for (auto p : Req.Params) {
        if (p.Key == "start")
          system_import_db((const char *)"/userdata/import.db");
      }
    } else if (path_specific_resource.find("login") != std::string::npos) {
      if (para_channel.empty()) { /* path is login */
        std::string username = system_config.at("sUserName");
        content = is_register_user(system_config);
        int auth = content.at("auth");
        int status = content.at("status");
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
        long expire_time = EXPIRE_SECONDS;
        if (status >= 0) {
          for (auto p : Req.Params) {
            if (p.Key == "expire") {
              std::string expire_stirng = p.Value;
              if (!expire_stirng.compare("day")) {
                expire_time = 86400;
              } else if (!expire_stirng.compare("week")) {
                expire_time = 604800;
              } else if (!expire_stirng.compare("month")) {
                expire_time = 2592000;
              }
              break;
            }
          }
          std::string token = jwt_token_get(username, auth, expire_time);
          Resp.setCookie("token", token, expire_time);
        }
      } else if (!para_channel.compare("modify")) { /* path is login/modify */
#ifdef ENABLE_JWT
        if (user_level > 0) {
          Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
          return;
        }
#endif
        int res = user_alter_verify(system_config);
        if (res >= 0) {
          std::string new_name = system_config.at("newUserName");
          std::string new_pw = system_config.at("newPassword");
          int user_level = system_config.at("iUserLevel");
          nlohmann::json modify_json;
          modify_json.emplace("sPassword", new_pw);
          modify_json.emplace("iUserLevel", user_level);
          dbserver_system_set((char *)TABLE_SYSTEM_USER,
                              (char *)modify_json.dump().c_str(), res);
        }
        content.emplace("status", res);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (!para_channel.compare("add")) { /* path is login/add */
#ifdef ENABLE_JWT
        if (user_level > 0) {
          Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
          return;
        }
#endif
        int res = user_alter_verify(system_config);
        if (res == -2) {
          std::string new_name = system_config.at("newUserName");
          std::string new_pw = system_config.at("newPassword");
          nlohmann::json add_json;
          add_json.emplace("sUserName", new_name);
          add_json.emplace("sPassword", new_pw);
          create_new_user(add_json);
        }
        content.emplace("status", res);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (!para_channel.compare("delete")) { /* path is login/delete */
#ifdef ENABLE_JWT
        if (user_level > 0) {
          Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
          return;
        }
#endif
        int res = user_alter_verify(system_config);
        if (res > 0) {
          dbserver_system_user_delete(res);
        }
        content.emplace("status", res);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
    }
  } else if (Req.Method == "DELETE") {
    if (!path_specific_resource.compare("firmware-upgrade")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!Req.Params.empty()) {
        for (auto p : Req.Params) {
          if (p.Key == "id") {
            std::string file_id_s = p.Value;
            std::string file_name = "/data/" + file_id_s;
            if (!remove(file_name.c_str()))
              Resp.setHeader(HttpStatus::kOk, "OK");
          }
        }
      } else {
        Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kBadRequest, "Not Implemented");
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#endif

} // namespace cgi
} // namespace rockchip
