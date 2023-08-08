// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "event_api.h"
#include "common.h"
#include <time.h>
#include <unistd.h>

namespace rockchip {
namespace cgi {

int GetScheduleId(std::string schedule_name) {
  if (!schedule_name.compare("motion")) {
    return 0;
  } else if (!schedule_name.compare("intrusion")) {
    return 1;
  } else if (!schedule_name.compare("video-plan")) {
    return 2;
  } else if (!schedule_name.compare("screenshot")) {
    return 3;
  } else {
    return -1;
  }
}

#ifdef USE_RKIPC

nlohmann::json regional_invasion_get() {
  int value;
  char *str;
  nlohmann::json ret;
  nlohmann::json regional_invasion;
  nlohmann::json normalized_screen_size;

  rk_osd_get_normalized_screen_width(&value);
  normalized_screen_size.emplace("iNormalizedScreenWidth", value);
  rk_osd_get_normalized_screen_height(&value);
  normalized_screen_size.emplace("iNormalizedScreenHeight", value);

  rk_event_ri_get_enabled(&value);
  regional_invasion.emplace("iEnabled", value);
  rk_event_ri_get_position_x(&value);
  regional_invasion.emplace("iPositionX", value);
  rk_event_ri_get_position_y(&value);
  regional_invasion.emplace("iPositionY", value);
  rk_event_ri_get_width(&value);
  regional_invasion.emplace("iWidth", value);
  rk_event_ri_get_height(&value);
  regional_invasion.emplace("iHeight", value);
  rk_event_ri_get_proportion(&value);
  regional_invasion.emplace("iProportion", value);
  rk_event_ri_get_sensitivity_level(&value);
  regional_invasion.emplace("iSensitivityLevel", value);
  rk_event_ri_get_time_threshold(&value);
  regional_invasion.emplace("iTimeThreshold", value);

  ret.emplace("normalizedScreenSize", normalized_screen_size);
  ret.emplace("regionalInvasion", regional_invasion);

  return ret;
}

void EventApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  std::string path_api_resource;
  std::string path_specific_resource;
  std::string path_channel_resource;
  char *str;
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  // Get Path Information
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_specific_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
    pos_first = path_specific_resource.find_first_of("/");
    if (pos_first != -1) {
      path_channel_resource = path_specific_resource.substr(
          pos_first + 1, path_specific_resource.size());
    }
  }
  if (Req.Method == "GET") {
    if (path_specific_resource.find("triggers") != std::string::npos) {
      // get triggers info
      if (!path_channel_resource.compare("vmd_0")) {
        content = R"(
                      {
                        "iNotificationCenterEnabled": 0,
                        "iNotificationEmailEnabled": 0,
                        "iNotificationFTPEnabled": 0,
                        "iNotificationIO1Enabled": 0,
                        "iNotificationRecord1Enabled": 0,
                        "iVideoInputChannelID": 0,
                        "id": 0,
                        "sEventType": "VMD"
                      }
                    )"_json;
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (!path_channel_resource.compare("vri_0")) {
        content = R"(
                      {
                        "iNotificationCenterEnabled": 0,
                        "iNotificationEmailEnabled": 0,
                        "iNotificationFTPEnabled": 0,
                        "iNotificationIO1Enabled": 0,
                        "iNotificationRecord1Enabled": 0,
                        "iVideoInputChannelID": 0,
                        "id": 1,
                        "sEventType": "VRI"
                      }
                    )"_json;
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("schedules") != std::string::npos) {
      // get schedules info
      if (!path_channel_resource.empty()) {
        int para_id = GetScheduleId(path_channel_resource);
        // minilog_debug("id is %d", para_id);
        if (para_id >= 0) {
          content = R"(
                        {
                          "sSchedulesJson": "[[],[],[],[],[],[],[]]"
                        }
                      )"_json;
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("motion-detection") !=
               std::string::npos) {
      // get motion detection info
      if (!path_channel_resource.compare("0")) {
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("regional-invasion") !=
               std::string::npos) {
      // get regional invasion info
      if (!path_channel_resource.compare("0")) {
        content = regional_invasion_get();
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (!path_specific_resource.compare("face-list")) {
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("face/") != std::string::npos) {
      // get face info
      if (!path_channel_resource.empty()) {
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    } else if (!path_specific_resource.compare("face-config")) {
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("face-waiting")) {
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("smart") != std::string::npos) {
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("last-face")) {
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("get-record-status")) {
      int record_status;
      rk_storage_record_statue_get(&record_status);
      content = record_status;
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    nlohmann::json event_config = Req.PostObject; /* must be json::object */
    char *config = new char[Req.ContentLength + 1];
    strncpy(config, event_config.dump().c_str(), Req.ContentLength);
    config[Req.ContentLength] = '\0';
    if (path_specific_resource.find("triggers/") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!path_channel_resource.compare("vmd_0")) {
        // // set
        // dbserver_event_set((char *)TABLE_EVENT_TRIGGERS, config, 0);
        // mediaserver_sync_schedules();
        // // get new info
        // str = dbserver_event_get((char *)TABLE_EVENT_TRIGGERS);
        // content = nlohmann::json::parse(str).at("jData").at(0);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (!path_channel_resource.compare("vri_0")) {
        // // set
        // dbserver_event_set((char *)TABLE_EVENT_TRIGGERS, config, 1);
        // // get new info
        // str = dbserver_event_get((char *)TABLE_EVENT_TRIGGERS);
        // content = nlohmann::json::parse(str).at("jData").at(1);
        content = R"(
                      {
                        "iNotificationCenterEnabled": 0,
                        "iNotificationEmailEnabled": 0,
                        "iNotificationFTPEnabled": 0,
                        "iNotificationIO1Enabled": 0,
                        "iNotificationRecord1Enabled": 0,
                        "iVideoInputChannelID": 0,
                        "id": 1,
                        "sEventType": "VRI"
                      }
                    )"_json;
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("motion-detection/") !=
               std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!path_channel_resource.compare("0")) {
        // // set
        // dbserver_event_set((char *)TABLE_MOVE_DETECTION, config, 0);
        // // get new info
        // str = dbserver_event_get((char *)TABLE_MOVE_DETECTION);
        // content = nlohmann::json::parse(str).at("jData").at(0);
        // move_detect_item_set(content, 0);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("regional-invasion/") !=
               std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!path_channel_resource.compare("0")) {
        // set
        int value_int;
        nlohmann::json param = event_config;
        if (param.dump().find("iEnabled") != param.dump().npos) {
          value_int = atoi(param.at("iEnabled").dump().c_str());
          rk_event_ri_set_enabled(value_int);
        }
        if (param.dump().find("iPositionX") != param.dump().npos) {
          value_int = atoi(param.at("iPositionX").dump().c_str());
          rk_event_ri_set_position_x(value_int);
        }
        if (param.dump().find("iPositionY") != param.dump().npos) {
          value_int = atoi(param.at("iPositionY").dump().c_str());
          rk_event_ri_set_position_y(value_int);
        }
        if (param.dump().find("iHeight") != param.dump().npos) {
          value_int = atoi(param.at("iHeight").dump().c_str());
          rk_event_ri_set_height(value_int);
        }
        if (param.dump().find("iWidth") != param.dump().npos) {
          value_int = atoi(param.at("iWidth").dump().c_str());
          rk_event_ri_set_width(value_int);
        }
        if (param.dump().find("iProportion") != param.dump().npos) {
          value_int = atoi(param.at("iProportion").dump().c_str());
          rk_event_ri_set_proportion(value_int);
        }
        if (param.dump().find("iSensitivityLevel") != param.dump().npos) {
          value_int = atoi(param.at("iSensitivityLevel").dump().c_str());
          rk_event_ri_set_sensitivity_level(value_int);
        }
        if (param.dump().find("iTimeThreshold") != param.dump().npos) {
          value_int = atoi(param.at("iTimeThreshold").dump().c_str());
          rk_event_ri_set_time_threshold(value_int);
        }
        // get new info
        content = regional_invasion_get();
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (path_specific_resource.find("schedules") !=
                 std::string::npos) {
#ifdef ENABLE_JWT
        if (user_level > 1) {
          Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
          return;
        }
#endif
        if (!path_channel_resource.empty()) {
          // set
          int para_id = GetScheduleId(path_channel_resource);
          if (para_id >= 0) {
            // dbserver_event_set((char *)TABLE_EVENT_SCHEDULES, config,
            // para_id);
            // mediaserver_sync_schedules();
            // str = dbserver_event_get((char *)TABLE_EVENT_SCHEDULES);
            // content = nlohmann::json::parse(str).at("jData").at(para_id);
            // content.erase("id");
            content = R"(
                        {
                          "sSchedulesJson": "[[],[],[],[],[],[],[]]"
                        }
                      )"_json;
            Resp.setHeader(HttpStatus::kOk, "OK");
            Resp.setApiData(content);
          } else {
            Resp.setErrorResponse(HttpStatus::kNotImplemented,
                                  "Not Implemented");
          }
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (!path_specific_resource.compare("take-photo")) {
      rk_take_photo();
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else if (!path_specific_resource.compare("start-record")) {
      rk_storage_record_start();
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else if (!path_specific_resource.compare("stop-record")) {
      rk_storage_record_stop();
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else // #ifdef USE_RKIPC

nlohmann::json regional_invasion_get() {
  char *str;
  nlohmann::json ret;
  nlohmann::json regional_invasion;
  nlohmann::json normalized_screen_size;

  str = dbserver_event_get((char *)TABLE_REGIONAL_INVASION);
  regional_invasion = nlohmann::json::parse(str).at("jData").at(0);
  regional_invasion.erase("id");
  str = dbserver_media_get((char *)TABLE_NORMALIZED_SCREEN_SIZE);
  normalized_screen_size = nlohmann::json::parse(str).at("jData").at(0);
  normalized_screen_size.erase("id");
  ret.emplace("normalizedScreenSize", normalized_screen_size);
  ret.emplace("regionalInvasion", regional_invasion);

  return ret;
}

std::string face_list_set(nlohmann::json event_config, int id) {
  time_t raw_time;
  struct tm *time_info;
  int age;
  char current_year[30];
  char current_time[30];
  std::string name;
  std::string type_path;
  std::string picture_path;
  std::string type = event_config.at("sType");
  std::string birthday = event_config.at("sBirthday");
  if (event_config.dump().find("sName") == std::string::npos)
    name = "User";
  else
    name = event_config.at("sName");

  time(&raw_time);
  time_info = localtime(&raw_time);
  strftime(current_time, 30, "%FT%T", time_info);
  strncpy(current_year, current_time, 30);
  age = atoi(current_year) - stoi(birthday.substr(0, 4));

  char *str = storage_manager_get_media_path();
  std::string mount_path = nlohmann::json::parse(str).at("sMountPath");
  if (!type.compare("whiteList"))
    type_path = "/white_list/";
  else
    type_path = "/black_list/";
  picture_path =
      mount_path + type_path + name + "_" + std::to_string(id) + ".jpg";

  event_config.emplace("iAge", age);
  event_config.emplace("sRegistrationTime", current_time);
  event_config.emplace("sPicturePath", picture_path);

  dbserver_event_set((char *)TABLE_FACE_LIST,
                     (char *)event_config.dump().c_str(), id);
  return picture_path;
}

nlohmann::json face_list_search_by_condition(nlohmann::json serach_conditions) {
  char *str;
  nlohmann::json result;
  nlohmann::json match_list;
  nlohmann::json search_result;
  int age_min = serach_conditions.at("minAge");
  int age_max = serach_conditions.at("maxAge");
  int begin_position = serach_conditions.at("beginPosition");
  int end_position = serach_conditions.at("endPosition");
  int access_card_number = serach_conditions.at("accessCardNumber");
  std::string begin_time = serach_conditions.at("beginTime");
  std::string end_time = serach_conditions.at("endTime");
  std::string type = serach_conditions.at("type");
  std::string gender = serach_conditions.at("gender");
  std::string gender_condition;
  std::string type_condition;
  if (!gender.compare("all"))
    gender_condition = "(1=1)";
  else
    gender_condition = "(sGender = '" + gender + "')";
  if (!type.compare("all"))
    // type_condition = "((sType = 'whiteList') or (sType = 'blackList'))";
    type_condition = "(1=1)";
  else
    type_condition = "(sType = '" + type + "')";

  std::string conditions =
      "from FaceList where (iAge between " + std::to_string(age_min) + " and " +
      std::to_string(age_max) + ") and (instr(iAccessCardNumber," +
      std::to_string(access_card_number) +
      ")>0) and "
      "(sRegistrationTime between '" +
      begin_time + "' and '" + end_time + "') and " + gender_condition +
      " and " + type_condition;
  std::string cmd = "select count(*) " + conditions;
  str = dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  std::string result_num =
      nlohmann::json::parse(str).at("jData").at(0).at("count(*)");
  int num_of_matches = stoi(result_num);

  // limit number
  cmd = "select * " + conditions + "limit " + std::to_string(begin_position) +
        "," + std::to_string(end_position - begin_position + 1);
  // minilog_debug("cmd is %s\n", cmd.c_str());
  str = dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  match_list = nlohmann::json::parse(str).at("jData");

  std::string ipv4_address = ipv4_address_get();
  for (auto &x : nlohmann::json::iterator_wrapper(match_list)) {
    std::string path = x.value().at("sPicturePath");
    std::string download_path = "http://" + ipv4_address + path;
    std::string note = x.value().at("sNote");
    if (!note.compare("undone")) {
      x.value().erase("sNote");
      x.value().emplace("sNote", "");
    }
    x.value().erase("sPicturePath");
    x.value().emplace("sPicturePath", download_path);
  }
  result.emplace("matchList", match_list);
  result.emplace("numOfMatches", num_of_matches);

  return result;
}

nlohmann::json face_list_search_by_name(nlohmann::json serach_conditions) {
  nlohmann::json result;
  nlohmann::json match_list;
  nlohmann::json search_result;
  int begin_position = serach_conditions.at("beginPosition");
  int end_position = serach_conditions.at("endPosition");
  std::string name = serach_conditions.at("name");
  std::string condition = "from FaceList where sName like '%" + name + "%'";
  std::string cmd = "select count(*) " + condition;
  char *str =
      dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  std::string result_num =
      nlohmann::json::parse(str).at("jData").at(0).at("count(*)");
  int num_of_matches = stoi(result_num);

  // limit number
  cmd = "select * " + condition + " limit " + std::to_string(begin_position) +
        "," + std::to_string(end_position - begin_position + 1);
  // minilog_debug("cmd is %s\n", cmd.c_str());
  str = dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  match_list = nlohmann::json::parse(str).at("jData");

  std::string ipv4_address = ipv4_address_get();
  for (auto &x : nlohmann::json::iterator_wrapper(match_list)) {
    std::string path = x.value().at("sPicturePath");
    std::string download_path = "http://" + ipv4_address + path;
    std::string note = x.value().at("sNote");
    if (!note.compare("undone")) {
      x.value().erase("sNote");
      x.value().emplace("sNote", "");
    }
    x.value().erase("sPicturePath");
    x.value().emplace("sPicturePath", download_path);
  }
  result.emplace("matchList", match_list);
  result.emplace("numOfMatches", num_of_matches);

  return result;
}

nlohmann::json snapshot_record_search(nlohmann::json serach_conditions) {
  nlohmann::json result;
  nlohmann::json match_list;
  nlohmann::json search_result;
  int begin_position = serach_conditions.at("beginPosition");
  int end_position = serach_conditions.at("endPosition");
  std::string begin_time = serach_conditions.at("beginTime");
  std::string end_time = serach_conditions.at("endTime");
  std::string conditions = "from FaceSnapshotRecord where (sTime between '" +
                           begin_time + "' and '" + end_time + "')";
  std::string cmd = "select count(*) " + conditions;
  char *str =
      dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  std::string result_num =
      nlohmann::json::parse(str).at("jData").at(0).at("count(*)");
  int num_of_matches = stoi(result_num);

  // limit number
  cmd = "select * " + conditions + "limit " + std::to_string(begin_position) +
        "," + std::to_string(end_position - begin_position + 1);
  // minilog_debug("cmd is %s\n", cmd.c_str());
  str = dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  match_list = nlohmann::json::parse(str).at("jData");

  std::string ipv4_address = ipv4_address_get();
  for (auto &x : nlohmann::json::iterator_wrapper(match_list)) {
    std::string path = x.value().at("sPicturePath");
    std::string download_path = "http://" + ipv4_address + path;
    x.value().erase("sPicturePath");
    x.value().emplace("sPicturePath", download_path);
    x.value().emplace("sSnapshotName", path);
  }
  result.emplace("matchList", match_list);
  result.emplace("numOfMatches", num_of_matches);

  return result;
}

nlohmann::json
control_record_search_by_condition(nlohmann::json serach_conditions) {
  nlohmann::json result;
  nlohmann::json match_list;
  int age_min = serach_conditions.at("minAge");
  int age_max = serach_conditions.at("maxAge");
  int begin_position = serach_conditions.at("beginPosition");
  int end_position = serach_conditions.at("endPosition");
  int access_card_number = serach_conditions.at("accessCardNumber");
  std::string begin_time = serach_conditions.at("beginTime");
  std::string end_time = serach_conditions.at("endTime");
  std::string status = serach_conditions.at("status");
  std::string gender = serach_conditions.at("gender");

  std::string gender_condition;
  if (!gender.compare("all"))
    gender_condition = "(1=1)";
  else
    gender_condition = "(FaceList.sGender = '" + gender + "')";

#ifdef MEDIASERVER_ROCKFACE
  std::string conditions =
      "from FaceList inner join FaceControlRecord where (FaceList.iFaceDBId = "
      "FaceControlRecord.iFaceId) and (FaceList.iAge between " +
      std::to_string(age_min) + " and " + std::to_string(age_max) +
      ") and (FaceList.iAccessCardNumber = " +
      std::to_string(access_card_number) +
      ") and "
      "(FaceControlRecord.sTime between '" +
      begin_time + "' and '" + end_time + "') and " + gender_condition;
  std::string cmd = "select count(*) " + conditions;
  char *str =
      dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  std::string result_num =
      nlohmann::json::parse(str).at("jData").at(0).at("count(*)");
  int num_of_matches = stoi(result_num);
#else
  std::string conditions =
      "from FaceList inner join FaceControlRecord where (FaceList.id = "
      "FaceControlRecord.iFaceId) and (FaceList.iAge between " +
      std::to_string(age_min) + " and " + std::to_string(age_max) +
      ") and (FaceList.iAccessCardNumber = " +
      std::to_string(access_card_number) +
      ") and "
      "(FaceControlRecord.sTime between '" +
      begin_time + "' and '" + end_time + "') and " + gender_condition;
  std::string cmd = "select count(*) " + conditions;
  char *str =
      dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  std::string result_num =
      nlohmann::json::parse(str).at("jData").at(0).at("count(*)");
  int num_of_matches = stoi(result_num);
#endif

  // limit number
  cmd = "select * " + conditions + "limit " + std::to_string(begin_position) +
        "," + std::to_string(end_position - begin_position + 1);
  minilog_debug("cmd is %s\n", cmd.c_str());
  str = dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  match_list = nlohmann::json::parse(str).at("jData");

  std::string ipv4_address = ipv4_address_get();
  for (auto &x : nlohmann::json::iterator_wrapper(match_list)) {
    std::string picture_path = x.value().at("sPicturePath");
    std::string snapshot_path = x.value().at("sSnapshotPath");
    std::string picture_address = "http://" + ipv4_address + picture_path;
    std::string snapshot_address = "http://" + ipv4_address + snapshot_path;
    std::string note = x.value().at("sNote");
    if (!note.compare("undone")) {
      x.value().erase("sNote");
      x.value().emplace("sNote", "");
    }
    x.value().erase("sPicturePath");
    x.value().erase("sSnapshotPath");
    x.value().emplace("sPicturePath", picture_address);
    x.value().emplace("sSnapshotPath", snapshot_address);
    x.value().emplace("sSnapshotName", snapshot_path);
  }
  result.emplace("matchList", match_list);
  result.emplace("numOfMatches", num_of_matches);

  return result;
}

nlohmann::json control_record_search_by_name(nlohmann::json serach_conditions) {
  nlohmann::json result;
  nlohmann::json match_list;
  int begin_position = serach_conditions.at("beginPosition");
  int end_position = serach_conditions.at("endPosition");
  std::string name = serach_conditions.at("name");
#ifdef MEDIASERVER_ROCKFACE
  std::string conditions =
      "from FaceList inner join FaceControlRecord where "
      "(FaceList.iFaceDBId = FaceControlRecord.iFaceId) and "
      "(FaceList.sName like '%" +
      name + "%')";

  std::string cmd = "select count(*) " + conditions;
  char *str =
      dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  std::string result_num =
      nlohmann::json::parse(str).at("jData").at(0).at("count(*)");
  int num_of_matches = stoi(result_num);
#else
  std::string conditions = "from FaceList inner join FaceControlRecord where "
                           "(FaceList.id = FaceControlRecord.iFaceId) and "
                           "(FaceList.sName like '%" +
                           name + "%')";
  std::string cmd = "select count(*) " + conditions;
  char *str =
      dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  std::string result_num =
      nlohmann::json::parse(str).at("jData").at(0).at("count(*)");
  int num_of_matches = stoi(result_num);
#endif
  cmd = "select * " + conditions + " limit " + std::to_string(begin_position) +
        "," + std::to_string(end_position - begin_position + 1);
  minilog_debug("cmd is %s\n", cmd.c_str());
  str = dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  match_list = nlohmann::json::parse(str).at("jData");

  std::string ipv4_address = ipv4_address_get();
  for (auto &x : nlohmann::json::iterator_wrapper(match_list)) {
    std::string picture_path = x.value().at("sPicturePath");
    std::string snapshot_path = x.value().at("sSnapshotPath");
    std::string picture_address = "http://" + ipv4_address + picture_path;
    std::string snapshot_address = "http://" + ipv4_address + snapshot_path;
    std::string note = x.value().at("sNote");
    if (!note.compare("undone")) {
      x.value().erase("sNote");
      x.value().emplace("sNote", "");
    }
    x.value().erase("sPicturePath");
    x.value().erase("sSnapshotPath");
    x.value().emplace("sPicturePath", picture_address);
    x.value().emplace("sSnapshotName", snapshot_path);
    x.value().emplace("sSnapshotPath", snapshot_address);
  }
  result.emplace("matchList", match_list);
  result.emplace("numOfMatches", num_of_matches);

  return result;
}

void move_detect_item_set(nlohmann::json config, int id) {
  if (!config.empty()) {
    mediaserver_set((char *)TABLE_MOVE_DETECTION, id,
                    (char *)config.dump().c_str());
  }
}

void region_invade_item_set(nlohmann::json config, int id) {
  if (!config.empty()) {
    nlohmann::json param = config.at("regionalInvasion");
    if (!config.empty()) {
      mediaserver_set((char *)TABLE_REGIONAL_INVASION, id,
                      (char *)param.dump().c_str());
    }
  }
}

void smart_cover_item_set(nlohmann::json smart_cover) {
  if (smart_cover.empty())
    return;
  std::string smart_cover_string = smart_cover.dump();

  if (smart_cover_string.find("iFaceEnabled") != std::string::npos) {
    int enable = smart_cover.at("iFaceEnabled");
    mediaserver_feature_id_int((char *)"SetFaceDetectEn", 0, enable);
  }
  if (smart_cover_string.find("iImageOverlayEnabled") != std::string::npos) {
    int enable = smart_cover.at("iImageOverlayEnabled");
    mediaserver_feature_id_int((char *)"SetFaceCaptureEn", 0, enable);
  }
  if (smart_cover_string.find("iStreamOverlayEnabled") != std::string::npos) {
    int enable = smart_cover.at("iStreamOverlayEnabled");
    mediaserver_feature_id_int((char *)"SetDrawFaceEn", 0, enable);
  }
  if (smart_cover_string.find("iFaceRecognitionEnabled") != std::string::npos) {
    int enable = smart_cover.at("iFaceRecognitionEnabled");
    mediaserver_feature_id_int((char *)"SetFaceRegEn", 0, enable);
  }
}

nlohmann::json smart_cover_get() {
  char *str;
  nlohmann::json db_cover;
  nlohmann::json db_overlays;
  str = dbserver_event_get((char *)TABLE_SMART_COVER);
  db_cover = nlohmann::json::parse(str).at("jData").at(0);
  str = dbserver_event_get((char *)TABLE_SMART_COVER_OVERLAY);
  db_overlays = nlohmann::json::parse(str).at("jData");
  db_cover.emplace("infoOverlay", db_overlays);
  return db_cover;
}

void smart_cover_set(nlohmann::json smart_cover) {
  char *event_config = new char[1024];
  std::string smart_cover_string = smart_cover.dump();
  if (smart_cover_string.find("infoOverlay") != std::string::npos) {
    nlohmann::json overlay_info = smart_cover.at("infoOverlay");
    for (auto &x : nlohmann::json::iterator_wrapper(overlay_info)) {
      nlohmann::json overlay_part = x.value();
      int id = overlay_part.at("id");
      strcpy(event_config, overlay_part.dump().c_str());
      dbserver_event_set((char *)TABLE_SMART_COVER_OVERLAY, event_config, id);
    }
    smart_cover.erase("infoOverlay");
  }

  strcpy(event_config, smart_cover.dump().c_str());
  dbserver_event_set((char *)TABLE_SMART_COVER, event_config, 0);
  delete[] event_config;
}

void set_mediaserver_face_config(nlohmann::json new_cfg,
                                 nlohmann::json old_cfg) {
  nlohmann::json diff = nlohmann::json::diff(new_cfg, old_cfg);
  // minilog_error("diff %s\n", diff.dump().c_str());
  for (auto &x : nlohmann::json::iterator_wrapper(old_cfg)) {
    if (diff.dump().find(x.key()) == diff.dump().npos) {
      new_cfg.erase(x.key());
    }
  }
  mediaserver_set((char *)TABLE_FACE_CONFIG, 0, new_cfg.dump().c_str());
}

void reset_face_db_and_delete_pic(std::string table, std::string path_key,
                                  int delete_face_db) {
  std::string cmd = "select " + path_key + " from " + table;
  char *str =
      dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
  nlohmann::json rst;
  if (str) {
    rst = nlohmann::json::parse(str).at("jData");
  } else {
    return;
  }
  // TODO: Delete target dir
  for (auto &x : nlohmann::json::iterator_wrapper(rst)) {
    nlohmann::json pat = x.value();
    if (!pat.empty()) {
      std::string file_path = pat.at(path_key);
      unlink((char *)file_path.c_str());
    }
  }
#ifdef MEDIASERVER_ROCKFACE
  if (delete_face_db)
    mediaserver_clear_face_db();
#endif
  dbserver_face_reset((char *)table.c_str());
  dbserver_reset_face_table((char *)table.c_str());
  minilog_debug("delete table is %s, pic path key is %s\n",
                (char *)table.c_str(), (char *)path_key.c_str());
}

void EventApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  std::string path_api_resource;
  std::string path_specific_resource;
  std::string path_channel_resource;
  char *str;
  nlohmann::json content;
#ifdef ENABLE_JWT
  int user_level = Req.UserLevel;
#endif

  // Get Path Information
  int pos_first = Req.PathInfo.find_first_of("/");
  path_api_resource = Req.PathInfo.substr(pos_first + 1, Req.PathInfo.size());
  pos_first = path_api_resource.find_first_of("/");
  if (pos_first != -1) {
    path_specific_resource =
        path_api_resource.substr(pos_first + 1, path_api_resource.size());
    pos_first = path_specific_resource.find_first_of("/");
    if (pos_first != -1) {
      path_channel_resource = path_specific_resource.substr(
          pos_first + 1, path_specific_resource.size());
    }
  }
  if (Req.Method == "GET") {
    if (path_specific_resource.find("triggers") != std::string::npos) {
      // get triggers info
      if (!path_channel_resource.compare("vmd_0")) {
        str = dbserver_event_get((char *)TABLE_EVENT_TRIGGERS);
        content = nlohmann::json::parse(str).at("jData").at(0);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (!path_channel_resource.compare("vri_0")) {
        str = dbserver_event_get((char *)TABLE_EVENT_TRIGGERS);
        content = nlohmann::json::parse(str).at("jData").at(1);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("schedules") != std::string::npos) {
      // get schedules info
      if (!path_channel_resource.empty()) {
        int para_id = GetScheduleId(path_channel_resource);
        // minilog_debug("id is %d", para_id);
        if (para_id >= 0) {
          str = dbserver_event_get((char *)TABLE_EVENT_SCHEDULES);
          content = nlohmann::json::parse(str).at("jData").at(para_id);
          content.erase("id");
          // for calculating info for device
          // std::string test =
          // nlohmann::json::parse(str).at("jData").at(para_id).at("sSchedulesJson");
          // content = nlohmann::json::parse((char*)test.c_str());
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("motion-detection") !=
               std::string::npos) {
      // get motion detection info
      if (!path_channel_resource.compare("0")) {
        str = dbserver_event_get((char *)TABLE_MOVE_DETECTION);
        content = nlohmann::json::parse(str).at("jData").at(0);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("regional-invasion") !=
               std::string::npos) {
      // get regional invasion info
      if (!path_channel_resource.compare("0")) {
        content = regional_invasion_get();
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (!path_specific_resource.compare("face-list")) {
      // get face list
      str = dbserver_event_get((char *)TABLE_FACE_LIST);
      content = nlohmann::json::parse(str).at("jData");
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("face/") != std::string::npos) {
      // get face info
      if (!path_channel_resource.empty()) {
        // path example is face/0
        int id = stoi(path_channel_resource);
        str = dbserver_event_get_by_id((char *)TABLE_FACE_LIST, id);
        content = nlohmann::json::parse(str).at("jData").at(0);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    } else if (!path_specific_resource.compare("face-config")) {
      // get face config
      str = dbserver_event_get((char *)TABLE_FACE_CONFIG);
      content = nlohmann::json::parse(str).at("jData").at(0);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("face-waiting")) {
      // get the number of face lists without feature values loaded
      std::string cmd =
          "select count(*) from FaceList where (iLoadCompleted == 0)";
      str = dbserver_sql((char *)cmd.c_str(), (char *)DBSERVER_EVENT_INTERFACE);
      std::string count =
          nlohmann::json::parse(str).at("jData").at(0).at("count(*)");
      int waiting_num = stoi(count);
      content.emplace("numOfWaiting", waiting_num);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("smart") != std::string::npos) {
      if (!path_channel_resource.compare("cover")) {
        content = smart_cover_get();
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (!path_specific_resource.compare("get-record-status")) {
      content = mediaserver_get_record_status(0);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("last-face")) {
      int id = -1;
      str = dbserver_sql(
          (char *)"select * from FaceList ORDER BY id DESC LIMIT 1",
          (char *)DBSERVER_EVENT_INTERFACE);
      nlohmann::json face_last = nlohmann::json::parse(str).at("jData");
      if (!face_last.empty()) {
        id = face_last.at(0).at("id");
      }
      content.emplace("id", id);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    nlohmann::json event_config = Req.PostObject; /* must be json::object */
    char *config = new char[Req.ContentLength + 1];
    strncpy(config, event_config.dump().c_str(), Req.ContentLength);
    config[Req.ContentLength] = '\0';
    if (path_specific_resource.find("triggers/") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!path_channel_resource.compare("vmd_0")) {
        // set
        dbserver_event_set((char *)TABLE_EVENT_TRIGGERS, config, 0);
        mediaserver_sync_schedules();
        // get new info
        str = dbserver_event_get((char *)TABLE_EVENT_TRIGGERS);
        content = nlohmann::json::parse(str).at("jData").at(0);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else if (!path_channel_resource.compare("vri_0")) {
        // set
        dbserver_event_set((char *)TABLE_EVENT_TRIGGERS, config, 1);
        // get new info
        str = dbserver_event_get((char *)TABLE_EVENT_TRIGGERS);
        content = nlohmann::json::parse(str).at("jData").at(1);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("schedules") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!path_channel_resource.empty()) {
        // set
        int para_id = GetScheduleId(path_channel_resource);
        if (para_id >= 0) {
          dbserver_event_set((char *)TABLE_EVENT_SCHEDULES, config, para_id);
          mediaserver_sync_schedules();
          str = dbserver_event_get((char *)TABLE_EVENT_SCHEDULES);
          content = nlohmann::json::parse(str).at("jData").at(para_id);
          content.erase("id");
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("motion-detection/") !=
               std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!path_channel_resource.compare("0")) {
        // set
        dbserver_event_set((char *)TABLE_MOVE_DETECTION, config, 0);
        // get new info
        str = dbserver_event_get((char *)TABLE_MOVE_DETECTION);
        content = nlohmann::json::parse(str).at("jData").at(0);
        move_detect_item_set(content, 0);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (path_specific_resource.find("regional-invasion/") !=
               std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (!path_channel_resource.compare("0")) {
        // set
        dbserver_event_set((char *)TABLE_REGIONAL_INVASION, config, 0);
        // get new info
        content = regional_invasion_get();
        region_invade_item_set(content, 0);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else if (!path_specific_resource.compare("face-config")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      // get old info
      str = dbserver_event_get((char *)TABLE_FACE_CONFIG);
      nlohmann::json old_cfg = nlohmann::json::parse(str).at("jData").at(0);
      // set
      int volume = event_config.at("iPromptVolume");
      std::string cmd = "amixer -q set Master " + std::to_string(volume) + "%";
      system(cmd.c_str());
      int sig = system("alsactl store -f /userdata/asound.state");
      if (sig)
        system("alsactl store -f /etc/asound.state");
      dbserver_event_set((char *)TABLE_FACE_CONFIG, config, 0);
      // get new info
      str = dbserver_event_get((char *)TABLE_FACE_CONFIG);
      content = nlohmann::json::parse(str).at("jData").at(0);
      // set to mediaserver
      set_mediaserver_face_config(content, old_cfg);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("face-list")) {
      // search face list
      for (auto p : Req.Params) {
        if (p.Key == "search") {
          if (p.Value == "condition")
            content = face_list_search_by_condition(event_config);
          else if (p.Value == "name")
            content = face_list_search_by_name(event_config);
        }
      }
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("snapshot-record")) {
      // search snapshot record
      content = snapshot_record_search(event_config);
      // dbserver_snapshot_record_set("/data/test_1.jpg");
      // str = dbserver_event_get((char *)TABLE_FACE_SNAPSHOT_RECORD);
      // content = nlohmann::json::parse(str).at("jData");
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("control-record")) {
      // search control record
      for (auto p : Req.Params) {
        if (p.Key == "search") {
          if (p.Value == "condition") {
            content = control_record_search_by_condition(event_config);
            // dbserver_control_record_set(1, "/data/test_2.jpg", "open",
            // "72.22");
            // str = dbserver_event_get((char *)TABLE_FACE_CONTROL_RECORD);
            // content = nlohmann::json::parse(str).at("jData");
          } else if (p.Value == "name") {
            content = control_record_search_by_name(event_config);
          }
        }
      }
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("check-face")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      for (auto p : Req.Params) {
        if (p.Key == "id") {
          std::string id_str = p.Value;
          std::string cmd = "select sName, iLoadCompleted, sPicturePath from "
                            "FaceList where (id > " +
                            id_str + ") and (iLoadCompleted != 1)";
          str = dbserver_sql((char *)cmd.c_str(),
                             (char *)DBSERVER_EVENT_INTERFACE);
          if (str) {
            content = nlohmann::json::parse(str).at("jData");
          }
          std::string cmd_delete = "delete from FaceList where (id > " +
                                   id_str + ") and (iLoadCompleted != 1)";
          dbserver_sql((char *)cmd_delete.c_str(),
                       (char *)DBSERVER_EVENT_INTERFACE);
          if (content.empty()) {
            break;
          }
          for (auto &x : nlohmann::json::iterator_wrapper(content)) {
            nlohmann::json pat = x.value();
            if (!pat.empty()) {
              std::string file_path = pat.at("sPicturePath");
              unlink((char *)file_path.c_str());
            }
          }
          break;
        }
      }
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("reset-face")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      reset_face_db_and_delete_pic(TABLE_FACE_LIST, "sPicturePath", 1);
      reset_face_db_and_delete_pic(TABLE_FACE_CONTROL_RECORD, "sSnapshotPath",
                                   0);
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else if (!path_specific_resource.compare("reset-snap")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      reset_face_db_and_delete_pic(TABLE_FACE_SNAPSHOT_RECORD, "sPicturePath",
                                   0);
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else if (!path_specific_resource.compare("reset-control")) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      reset_face_db_and_delete_pic(TABLE_FACE_CONTROL_RECORD, "sSnapshotPath",
                                   0);
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else if (path_specific_resource.find("face-picture") !=
               std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      // upload face picture
      for (auto p : Req.Params) {
        if (p.Key == "path") {
          std::string file_path = p.Value;
          std::string new_file_path = DecodeURI(file_path);
          for (int i = 0; i < Req.Files.size(); i++) {
            if (Req.Files.at(i).getDataLength() > (1024 * 1024)) { // 1Mbyte
              Resp.setErrorResponse(HttpStatus::kBadRequest, "jpg is too big!");
            } else {
              int end_position = 0;
              // overwrite
              std::ofstream picture_file(new_file_path, std::ofstream::out);
              picture_file << Req.Files.at(i).getData();
              end_position = picture_file.tellp();
              picture_file.close();
              if (end_position == Req.Files.at(i).getDataLength()) {
#ifdef MEDIASERVER_ROCKFACE
                str = dbserver_event_get_by_key_char(
                    (char *)TABLE_FACE_LIST, (char *)"sPicturePath",
                    (char *)new_file_path.c_str());
                nlohmann::json info_cfg =
                    nlohmann::json::parse(str).at("jData");
                if (!info_cfg.empty()) {
                  int id = info_cfg.at(0).at("id");
                  mediaserver_set_face_image(id, new_file_path.c_str());
                }
#endif
                Resp.setHeader(HttpStatus::kOk, "OK");
              } else {
                Resp.setErrorResponse(HttpStatus::kBadRequest,
                                      "picture upload failed!");
              }
            }
          }
        } else if (p.Key == "copy-path") {
          std::string copy_path = p.Value;
          std::string new_copy_path = DecodeURI(copy_path);
          std::string src_path = event_config.at("path");
          std::string new_src_path = DecodeURI(src_path);
          // copy modification time and access
          std::string cmd =
              "cp -p \"" + new_src_path + "\" \"" + new_copy_path + "\"";
          int ret = system(cmd.c_str());
          content.emplace("copyResultCode", ret);
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        }
      }
    } else if (path_specific_resource.find("face") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      // add or modify
      int id = 0;
      if (path_channel_resource.empty()) {
        // path is face
        str = dbserver_sql(
            (char *)"select * from FaceList ORDER BY id DESC LIMIT 1",
            (char *)DBSERVER_EVENT_INTERFACE);
        nlohmann::json face_last = nlohmann::json::parse(str).at("jData");
        if (!face_last.empty()) {
          id = face_last.at(0).at("id");
          id++;
        }
        // set
        std::string picture_path = face_list_set(event_config, id);
        content.emplace("sPicturePath", picture_path);
        content.emplace("id", id);
      } else {
        // path example is face/0
        std::string path_old;
        std::string path_new;
        int old_face_id;
        id = stoi(path_channel_resource); // modify table
        str = dbserver_event_get_by_id((char *)TABLE_FACE_LIST, id);
        if (str != NULL) {
          path_old =
              nlohmann::json::parse(str).at("jData").at(0).at("sPicturePath");
          old_face_id =
              nlohmann::json::parse(str).at("jData").at(0).at("iFaceDBId");
          // set
          face_list_set(event_config, id);
          str = dbserver_event_get_by_id((char *)TABLE_FACE_LIST, id);
          if (str != NULL) {
            content = nlohmann::json::parse(str).at("jData").at(0);
            path_new = content.at("sPicturePath");
            // move picture to new path
            int face_id = content.at("iFaceDBId");
            // minilog_debug("in modify, old_face_id is %d, face id is %d\n",
            // old_face_id, face_id);
            // before a new pic set, send face_id = -2 to delete old data
            if (face_id == -2 && old_face_id >= 0) {
              mediaserver_delete_face(id, old_face_id);
            }
            if (path_old.compare(path_new)) {
              std::string cmd = "mv \"" + path_old + "\" \"" + path_new + "\"";
              // minilog_debug("cmd is %s\n", cmd.c_str());
              system(cmd.c_str());
            }
          } else {
            minilog_debug("dbserver_event_get_by_id fail!, new id is %d\n", id);
          }
        } else {
          minilog_debug("dbserver_event_get_by_id fail!, id is %d\n", id);
        }
      }
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (!path_specific_resource.compare("take-photo")) {
      mediaserver_take_photo(0, 1);
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else if (!path_specific_resource.compare("start-record")) {
      mediaserver_start_record(0);
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else if (!path_specific_resource.compare("stop-record")) {
      mediaserver_stop_record(0);
      Resp.setHeader(HttpStatus::kOk, "OK");
    } else if (path_specific_resource.find("smart") != std::string::npos) {
      if (!path_channel_resource.compare("cover")) {
#ifdef ENABLE_JWT
        if (user_level > 1) {
          Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
          return;
        }
#endif
        /* Erase unchanged data */
        nlohmann::json cfg_old = smart_cover_get();
        nlohmann::json diff = nlohmann::json::diff(cfg_old, event_config);
        for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
          if (diff.dump().find(x.key()) == diff.dump().npos)
            event_config.erase(x.key());
        }
        /* Set */
        if (!event_config.empty())
          smart_cover_set(event_config);
        smart_cover_item_set(event_config);
        /* Get new info */
        content = smart_cover_get();
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      } else {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
    delete[] config;
  } else if (Req.Method == "DELETE") {
    if (path_specific_resource.find("face") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_channel_resource.empty()) {
        // path is face
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        // path example is face/0
        int id = stoi(path_channel_resource);
        char *delete_string = dbserver_face_list_delete(id);
        if (strlen(delete_string) > 0) {
          nlohmann::json delete_info =
              nlohmann::json::parse(delete_string).at("jData");
          std::string path = delete_info.at(0).at("sPicturePath");
          unlink((char *)path.c_str());
#ifdef MEDIASERVER_ROCKFACE
          int face_id = delete_info.at(0).at("iFaceDBId");
#else
          int complete_status = delete_info.at(0).at("iLoadCompleted");
          int face_id = -1;
          if (complete_status == 1)
            face_id = id;
#endif

          if (face_id >= 0) {
            int id = delete_info.at(0).at("id");
#ifdef MEDIASERVER_ROCKFACE
            mediaserver_delete_face(id, face_id);
#endif
            char *delete_str = dbserver_event_get_by_key_int(
                (char *)TABLE_FACE_CONTROL_RECORD, (char *)"iFaceId", face_id);
            nlohmann::json control_data =
                nlohmann::json::parse(delete_str).at("jData");
            if (control_data.size() > 0) {
              nlohmann::json delete_list;
              for (auto &x : nlohmann::json::iterator_wrapper(control_data)) {
                nlohmann::json item_info = x.value();
                std::string sanp_path = item_info["sSnapshotPath"];
                if (!sanp_path.empty()) {
                  unlink((char *)sanp_path.c_str());
                }
              }
              dbserver_event_delete_by_key_int(
                  (char *)TABLE_FACE_CONTROL_RECORD, (char *)"iFaceId",
                  face_id);
            }
          }
        } else {
          minilog_debug("delete_string is empty");
        }
        Resp.setHeader(HttpStatus::kOk, "OK");
      }
    } else if (!path_specific_resource.compare("control-record")) {
#ifdef ENABLE_JWT
      if (user_level > 0) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (Req.Params.empty()) {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        for (auto p : Req.Params) {
          if (p.Key == "delete") {
            int id = stoi(p.Value);
            dbserver_event_delete_by_key_int((char *)TABLE_FACE_CONTROL_RECORD,
                                             (char *)"id", id);
          }
        }
        Resp.setHeader(HttpStatus::kOk, "OK");
      }
    } else if (!path_specific_resource.compare("snapshot-record")) {
#ifdef ENABLE_JWT
      if (user_level > 0) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (Req.Params.empty()) {
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        for (auto p : Req.Params) {
          if (p.Key == "delete") {
            int id = stoi(p.Value);
            dbserver_event_delete_by_key_int((char *)TABLE_FACE_SNAPSHOT_RECORD,
                                             (char *)"id", id);
          }
        }
        Resp.setHeader(HttpStatus::kOk, "OK");
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}
#endif

} // namespace cgi
} // namespace rockchip
