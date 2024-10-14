// Copyright 2019 Fuzhou Rockchip Electronics Co., Ltd. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "storage_api.h"
#include "common.h"
#include <time.h>
#include <unistd.h>

namespace rockchip {
namespace cgi {

#ifdef USE_RKIPC

nlohmann::json get_snap_plan_0() {
  nlohmann::json param;
  char *tmp = new char[256];
  int value = 0;

  param.emplace("id", 0);
  param.emplace("iShotNumber", 1);
  param.emplace("sImageType", "JPEG");

  rk_video_get_enable_cycle_snapshot(&value);
  param.emplace("iEnabled", value);
  rk_video_get_image_quality(&value);
  param.emplace("iImageQuality", value);
  rk_video_get_snapshot_interval_ms(&value);
  param.emplace("iShotInterval", value);

  rk_video_get_jpeg_resolution(&tmp);
  param.emplace("sResolution", tmp);
  delete[] tmp;

  return param;
}

int set_snap_plan_0(nlohmann::json param) {
  int value_int;
  std::string value;

  if (param.dump().find("iEnabled") != param.dump().npos) {
    value_int = atoi(param.at("iEnabled").dump().c_str());
    rk_video_set_enable_cycle_snapshot(value_int);
  }
  if (param.dump().find("iImageQuality") != param.dump().npos) {
    value_int = atoi(param.at("iImageQuality").dump().c_str());
    rk_video_set_image_quality(value_int);
  }
  if (param.dump().find("iShotInterval") != param.dump().npos) {
    value_int = atoi(param.at("iShotInterval").dump().c_str());
    rk_video_set_snapshot_interval_ms(value_int);
  }
  if (param.dump().find("sResolution") != param.dump().npos) {
    value = param.at("sResolution").dump();
    value.erase(0, 1).erase(value.end() - 1, value.end()); // erase \"
    rk_video_set_jpeg_resolution(value.c_str());
  }

  return 0;
}

void StorageApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  int hdd_id = 3;
  std::string path_api_resource;
  std::string path_specific_resource;
  std::string path_hdd_id;
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
      path_hdd_id = path_specific_resource.substr(
          pos_first + 1, path_specific_resource.size() + 1);
  }

  if (Req.Method == "GET") {
    if (path_specific_resource.find("hdd-list") != std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/hdd-list */
        content = R"(
      [{
	"iFormatProg": 0,
	"iFormatStatus": 0,
	"iFreeSize": 0,
	"iMediaSize": 0,
	"iTotalSize": 0,
	"id": 1,
	"sDev": "",
	"sFormatErr": "",
	"sMountPath": "/mnt/sdcard",
	"sName": "SD Card",
	"sStatus": "unmounted",
	"sType": ""
}, {
	"iFormatProg": 0,
	"iFormatStatus": 0,
	"iFreeSize": 12.0438613891602,
	"iMediaSize": 60972,
	"iTotalSize": 12.1327171325684,
	"id": 3,
	"sAttributes": "rw",
	"sDev": "/dev/block/by-name/media",
	"sFormatErr": "",
	"sMountPath": "/userdata/media",
	"sName": "Emmc",
	"sStatus": "mounted",
	"sType": "ext2"
}, {
	"iFormatProg": 0,
	"iFormatStatus": 0,
	"iFreeSize": 0,
	"iMediaSize": 0,
	"iTotalSize": 0,
	"id": 2,
	"sDev": "",
	"sFormatErr": "",
	"sMountPath": "/media/usb0",
	"sName": "U Disk",
	"sStatus": "unmounted",
	"sType": ""
}]
    )"_json;
      } else { /* path example is storage/hdd-list/1 */
        // hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        // nlohmann::json hdd_list = storage_hdd_list_get();
        // content = hdd_list.at(hdd_id - 1);
      }
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("quota") != std::string::npos) {
      // if (path_hdd_id.empty()) { /* path is storage/quota */
      //   // Can only get current storage device quota, here only for test
      //   for (int i = 0; i < storage_hdd_list_get().size(); i++) {
      //     content.push_back(storage_quota_get(i + 1));
      //   }
      // } else { /* path example is storage/quota/1 */
      //   hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
      //   content = storage_quota_get(hdd_id);
      // }
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("snap-plan") != std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/snap-plan */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        if (hdd_id == 0) { /* path is storage/snap-plan/0 */
          content = get_snap_plan_0();
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else if (hdd_id == 1) { /* path is storage/snap-plan/1 */
          content = R"(
            {
                "iEnabled": 0,
                "iImageQuality": 90,
                "iShotInterval": 1000,
                "iShotNumber": 4,
                "id": 1,
                "sImageType": "JPEG",
                "sResolution": "1920*1080"
            }
          )"_json;
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else if (!path_specific_resource.compare("current-path")) {
      // /* path is storage/current-path */
      // char *str = storage_manager_get_media_path();
      // std::string path = nlohmann::json::parse(str).at("sMountPath");
      // content.emplace("sMountPath", path);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("advance-para") !=
               std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/advance-para */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        hdd_id =
            atoi((char *)path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        // char *str = dbserver_storage_get((char *)TABLE_STORAGE_ADVANCE_PARA);
        if (!hdd_id) {
          content = R"(
            {
              "iEnabled": 0,
              "id": 0
            }
          )"_json;
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    int video_quota_ratio;
    int picture_quota_ratio;
    int camera_id = 0;
    nlohmann::json diff;
    nlohmann::json cfg_old;
    nlohmann::json storage_config = Req.PostObject; /* must be json::object */
    if (path_specific_resource.find("quota") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_hdd_id.empty()) { /* path is storage/quota */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else { /* path example is storage/quota/1 */
        // nlohmann::json hdd_list = storage_hdd_list_get();
        // hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        // std::string path_name = get_hdd_name_by_id(hdd_list, hdd_id);
        // minilog_debug("quota path_name is %s\n", (char *)path_name.c_str());
        // /* Set */
        // if (!path_name.empty())
        //   dbserver_update_storage_config_mountpath((char
        //   *)path_name.c_str());
        // else if (hdd_id == 1)
        //   dbserver_update_storage_config_mountpath((char *)"/mnt/sdcard");
        // else if (hdd_id == 2)
        //   dbserver_update_storage_config_mountpath((char *)"/media/usb0");
        // else if (hdd_id == 3)
        //   dbserver_update_storage_config_mountpath((char
        //   *)"/userdata/media");

        // if (!storage_config.empty()) {
        //   video_quota_ratio =
        //       atoi(storage_config.at("iVideoQuotaRatio").dump().c_str());
        //   picture_quota_ratio =
        //       atoi(storage_config.at("iPictureQuotaRatio").dump().c_str());
        //   dbserver_update_storage_media_folder_duty(camera_id, 0,
        //                                             video_quota_ratio, -1);
        //   dbserver_update_storage_media_folder_duty(camera_id, 1,
        //                                             picture_quota_ratio, -1);
        // }
        // /* Get new info */
        // content = storage_quota_get(hdd_id);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    } else if (path_specific_resource.find("format") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_hdd_id.empty()) { /* path is storage/format */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else { /* path is storage/format/1 */
        // hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        // std::string path_name = get_hdd_name_by_id(NULL, hdd_id);
        // minilog_debug("format path_name is %s\n", (char *)path_name.c_str());
        // /* Set */
        // if (!path_name.empty())
        //   storage_manager_diskformat((char *)path_name.c_str(), (char
        //   *)"fat32");
        // else if (hdd_id == 1)
        //   storage_manager_diskformat((char *)"/mnt/sdcard", (char *)"fat32");
        // else if (hdd_id == 2)
        //   storage_manager_diskformat((char *)"/media/usb0", (char *)"fat32");
        // else if (hdd_id == 3)
        //   storage_manager_diskformat((char *)"/userdata/media", (char
        //   *)"fat32");
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    } else if (!path_specific_resource.compare("search")) {
      // if (!storage_config.empty())
      //   content = search_file_list_get(storage_config);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("snap-plan") != std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/snap-plan */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        if (hdd_id == 0) { /* path is storage/snap-plan/0 */

          /* Erase unchanged data */
          cfg_old = get_snap_plan_0();
          diff = nlohmann::json::diff(cfg_old, storage_config);
          for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
            if (diff.dump().find(x.key()) == diff.dump().npos)
              storage_config.erase(x.key());
          }
          /* Set */
          if (!storage_config.empty())
            set_snap_plan_0(storage_config);
          /* Get new info */
          content = get_snap_plan_0();
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else if (hdd_id == 1) { /* path is storage/snap-plan/1 */
          // /* Erase unchanged data */
          // cfg_old = storage_plan_get(1);
          // diff = nlohmann::json::diff(cfg_old, storage_config);
          // for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
          //   if (diff.dump().find(x.key()) == diff.dump().npos)
          //     storage_config.erase(x.key());
          // }
          // /* Set */
          // if (!storage_config.empty())
          //   dbserver_set_storage_plan_snap(
          //       (char *)storage_config.dump().c_str(), 1);
          // /* Get new info */
          // content = storage_plan_get(1);
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else if (path_specific_resource.find("advance-para") !=
               std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_hdd_id.empty()) { /* path is storage/advance-para */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        if (!hdd_id) {
          // char *str = dbserver_storage_get((char
          // *)TABLE_STORAGE_ADVANCE_PARA);
          // nlohmann::json old_config =
          //     nlohmann::json::parse(str).at("jData").at(hdd_id);
          // nlohmann::json diff =
          //     nlohmann::json::diff(old_config, storage_config);
          // for (auto &x : nlohmann::json::iterator_wrapper(old_config)) {
          //   if (diff.dump().find(x.key()) == diff.dump().npos)
          //     storage_config.erase(x.key());
          // }
          // if (!storage_config.empty()) {
          //   dbserver_storage_set((char *)TABLE_STORAGE_ADVANCE_PARA,
          //                        (char *)storage_config.dump().c_str(),
          //                        hdd_id);
          //   mediaserver_sync_schedules();
          // }
          // str = dbserver_storage_get((char *)TABLE_STORAGE_ADVANCE_PARA);
          // content = nlohmann::json::parse(str).at("jData").at(hdd_id);
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else if (path_specific_resource.find("delete") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      // nlohmann::json file_list = storage_config.at("name");
      // std::string delete_type = storage_config.at("type");
      // int rst = 1;
      // if (!file_list.empty()) {
      //   rst = media_file_delete(delete_type, file_list);
      // }
      // content.emplace("rst", rst);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else {
    Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
  }
}

#else // #ifdef USE_RKIPC

std::string file_address_get(std::string path, std::string name) {
  std::string address;
  std::string ipv4_address = ipv4_address_get();
  address = "http://" + ipv4_address + path + "/" + name;

  return address;
}

int file_path_get(std::string search_type) {
  int path_id = -1;
  if (!search_type.compare("video0"))
    path_id = 0;
  else if (!search_type.compare("photo0"))
    path_id = 1;
  else if (!search_type.compare("video1"))
    path_id = 2;
  else if (!search_type.compare("photo1"))
    path_id = 3;
  else if (!search_type.compare("black_list"))
    path_id = 4;
  else if (!search_type.compare("snapshot"))
    path_id = 5;
  else if (!search_type.compare("control"))
    path_id = 5;
  else if (!search_type.compare("white_list"))
    path_id = 6;
  return path_id;
}

nlohmann::json search_file_list_get(nlohmann::json search_config) {
  int order = search_config.at("order");
  int search_result_position = search_config.at("searchResultPosition");
  int max_results = search_config.at("maxResults");
  std::string limit = std::to_string(search_result_position) + "," +
                      std::to_string(max_results);
  std::string search_type = search_config.at("searchType");
  std::string start_time_s = search_config.at("startTime");
  std::string end_time_s = search_config.at("endTime");
  int start_time = standard_to_timestamp(start_time_s);
  int end_time = standard_to_timestamp(end_time_s);
  nlohmann::json file_list;
  nlohmann::json search_result;

  char *str = storage_manager_get_media_path();
  nlohmann::json scan_path = nlohmann::json::parse(str).at("sScanPath");
  int num_of_matches;
  std::string path;
  int path_id = file_path_get(search_type);
  if (path_id < 0) {
    search_result.emplace("matchList", NULL);
    search_result.emplace("numOfMatches", 0);
    return search_result;
  }
  path = scan_path.at(path_id).at("sMediaPath");
  minilog_debug("path is %s", (char *)path.c_str());
  str = storage_manager_get_filelist_path((char *)path.c_str(), &start_time,
                                          &end_time, order, 0);
  num_of_matches = nlohmann::json::parse(str).at("FILES").size();
  str = storage_manager_get_filelist_path((char *)path.c_str(), &start_time,
                                          &end_time, order,
                                          (char *)limit.c_str());
  nlohmann::json files = nlohmann::json::parse(str).at("FILES");

  for (auto &x : nlohmann::json::iterator_wrapper(files)) {
    nlohmann::json file;
    std::string file_name = x.value().at("sNAME");
    std::string file_address = file_address_get(path, file_name);
    std::string file_size_s = x.value().at("sSIZE");
    float file_size = stoi(file_size_s) / 1024.0 / 1024.0; // Byet to MByte
    // Stamp time to standard time.
    const long int stamp_time = x.value().at("iTIME");
    char file_time[30];
    struct tm *time_info = localtime(&stamp_time);
    strftime(file_time, 30, "%FT%T", time_info);

    file.emplace("fileId", stoi(x.key()));
    file.emplace("fileName", file_name);
    file.emplace("fileAddress", file_address);
    file.emplace("fileSize", file_size);
    file.emplace("fileTime", file_time);
    file_list.push_back(file);
  }
  search_result.emplace("matchList", file_list);
  search_result.emplace("numOfMatches", num_of_matches);

  return search_result;
}

nlohmann::json storage_hdd_list_get(void) {
  char *disks_status = storage_manager_get_disks_status((char *)"");
  nlohmann::json disks_info = nlohmann::json::parse(disks_status);
  nlohmann::json hdd_list;
  float value;

  for (auto &x : nlohmann::json::iterator_wrapper(disks_info)) {
    int hdd_id = atoi(x.key().c_str());
    std::string attributes = disks_info.at(hdd_id).at("sAttributes");
    std::string type = disks_info.at(hdd_id).at("sType");

    if (disks_info.at(hdd_id).at("iStatus") == 1) {
      disks_info.at(hdd_id).emplace("sStatus", "mounted");
    } else {
      disks_info.at(hdd_id).emplace("sStatus", "unmounted");
    }
    disks_info.at(hdd_id).erase("iStatus");

    // Filter information except read, such as relaxtime, etc.
    disks_info.at(hdd_id).erase("sAttributes");
    if (attributes.find("rw") != std::string::npos)
      disks_info.at(hdd_id).emplace("sAttributes", "rw");
    else if (attributes.find("ro") != std::string::npos)
      disks_info.at(hdd_id).emplace("sAttributes", "ro");
    else if (attributes.find("wo") != std::string::npos)
      disks_info.at(hdd_id).emplace("sAttributes", "wo");

    // convent Byte to Giga Byte
    value = atoi(disks_info.at(hdd_id).at("iFreeSize").dump().c_str());
    value /= 1024 * 1024;
    disks_info.at(hdd_id).erase("iFreeSize");
    disks_info.at(hdd_id).emplace("iFreeSize", value);
    value = atoi(disks_info.at(hdd_id).at("iTotalSize").dump().c_str());
    value /= 1024 * 1024;
    disks_info.at(hdd_id).erase("iTotalSize");
    disks_info.at(hdd_id).emplace("iTotalSize", value);

    // disks_info.at(hdd_id).erase("sType");
    // if (type.find("nfts") != std::string::npos)
    //   disks_info.at(hdd_id).emplace("sType", "network");
    // else
    //   disks_info.at(hdd_id).emplace("sType", "local");
  }

  for (int i = 0; i < disks_info.size(); i++) {
    hdd_list.push_back(disks_info.at(i));
  }

  return hdd_list;
}

nlohmann::json storage_quota_get(int hdd_id) {
  char *media_info = storage_manager_get_media_path();
  nlohmann::json hdd_list = storage_hdd_list_get();
  nlohmann::json quota_info = nlohmann::json::object();
  nlohmann::json hdd_info = hdd_list.at(hdd_id - 1);
  nlohmann::json scan_path = nlohmann::json::parse(media_info).at("sScanPath");
  // Temporarily default all disks to camera 0
  nlohmann::json video_info = scan_path.at(0);
  nlohmann::json photo_info = scan_path.at(1);

  float total_volume = atof(hdd_info.at("iTotalSize").dump().c_str());
  float free_volume = atof(hdd_info.at("iFreeSize").dump().c_str());

  int picture_quota_ratio = atoi(photo_info.at("iDuty").dump().c_str());
  float picture_total_volume = total_volume * picture_quota_ratio / 100;
  float picture_use_volume =
      atof(photo_info.at("iUseSize").dump().c_str()) / 1024 / 1024;
  float picture_free_quota = picture_total_volume - picture_use_volume;

  int video_quota_ratio = atoi(video_info.at("iDuty").dump().c_str());
  float video_total_volume = total_volume * video_quota_ratio / 100;
  float video_use_volume =
      atof(video_info.at("iUseSize").dump().c_str()) / 1024 / 1024;
  ;
  float video_free_quota = video_total_volume - video_use_volume;
  // Avoid negative free space on unmounted disks
  if (!picture_total_volume)
    picture_free_quota = 0;
  if (!video_total_volume)
    video_free_quota = 0;

  quota_info.emplace("id", hdd_id);
  quota_info.emplace("iTotalPictureVolume", picture_total_volume);
  quota_info.emplace("iFreePictureQuota", picture_free_quota);
  quota_info.emplace("iPictureQuotaRatio", picture_quota_ratio);
  quota_info.emplace("iTotalVideoVolume", video_total_volume);
  quota_info.emplace("iFreeVideoQuota", video_free_quota);
  quota_info.emplace("iVideoQuotaRatio", video_quota_ratio);

  // quota_info = nlohmann::json::parse(media_info);
  return quota_info;
}

nlohmann::json storage_plan_get(int id) {
  char *str = dbserver_get_storage_plan_snap(id);
  nlohmann::json rst = nlohmann::json::parse(str).at("jData").at(0);
  rst.erase("id");
  return rst;
}

int media_file_delete(std::string delete_type, nlohmann::json file_list) {
  int path_id = file_path_get(delete_type);
  if (path_id < 0) {
    return 0;
  }
  char *str = storage_manager_get_media_path();
  nlohmann::json scan_path = nlohmann::json::parse(str).at("sScanPath");
  std::string path = scan_path.at(path_id).at("sMediaPath");
  for (auto &x : nlohmann::json::iterator_wrapper(file_list)) {
    std::string delete_name = x.value();
    if (delete_name.find("/") == std::string::npos) {
      std::string delete_path = path + "/" + delete_name;
      int rst = unlink((char *)delete_path.c_str());
    } else {
      minilog_debug("media_file_delete: unlaw name is %s",
                    (char *)delete_name.c_str());
    }
  }
  return 1;
}

std::string get_hdd_name_by_id(nlohmann::json hdd_list, int id) {
  if (NULL == hdd_list) {
    hdd_list = storage_hdd_list_get();
  }
  for (auto &x : nlohmann::json::iterator_wrapper(hdd_list)) {
    nlohmann::json pat = x.value();
    int hdd_id = pat.at("id");
    if (hdd_id == id) {
      std::string mount_path = pat.at("sMountPath");
      return mount_path;
    }
  }
  return "";
}

void StorageApiHandler::handler(const HttpRequest &Req, HttpResponse &Resp) {
  int hdd_id = 3;
  std::string path_api_resource;
  std::string path_specific_resource;
  std::string path_hdd_id;
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
      path_hdd_id = path_specific_resource.substr(
          pos_first + 1, path_specific_resource.size() + 1);
  }

  if (Req.Method == "GET") {
    if (path_specific_resource.find("hdd-list") != std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/hdd-list */
        content = storage_hdd_list_get();
      } else { /* path example is storage/hdd-list/1 */
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        nlohmann::json hdd_list = storage_hdd_list_get();
        content = hdd_list.at(hdd_id - 1);
      }
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("quota") != std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/quota */
        // Can only get current storage device quota, here only for test
        for (int i = 0; i < storage_hdd_list_get().size(); i++) {
          content.push_back(storage_quota_get(i + 1));
        }
      } else { /* path example is storage/quota/1 */
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        content = storage_quota_get(hdd_id);
      }
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("snap-plan") != std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/snap-plan */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        if (hdd_id == 0) { /* path is storage/snap-plan/0 */
          content = storage_plan_get(hdd_id);
          content.erase("iShotNumber");
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else if (hdd_id == 1) { /* path is storage/snap-plan/1 */
          content = storage_plan_get(hdd_id);
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else if (!path_specific_resource.compare("current-path")) {
      /* path is storage/current-path */
      char *str = storage_manager_get_media_path();
      std::string path = nlohmann::json::parse(str).at("sMountPath");
      content.emplace("sMountPath", path);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("advance-para") !=
               std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/advance-para */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        hdd_id =
            atoi((char *)path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        char *str = dbserver_storage_get((char *)TABLE_STORAGE_ADVANCE_PARA);
        if (!hdd_id) {
          content = nlohmann::json::parse(str).at("jData").at(hdd_id);
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else {
      Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
    }
  } else if ((Req.Method == "POST") || (Req.Method == "PUT")) {
    int video_quota_ratio;
    int picture_quota_ratio;
    int camera_id = 0;
    nlohmann::json diff;
    nlohmann::json cfg_old;
    nlohmann::json storage_config = Req.PostObject; /* must be json::object */
    if (path_specific_resource.find("quota") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_hdd_id.empty()) { /* path is storage/quota */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else { /* path example is storage/quota/1 */
        nlohmann::json hdd_list = storage_hdd_list_get();
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        std::string path_name = get_hdd_name_by_id(hdd_list, hdd_id);
        minilog_debug("quota path_name is %s\n", (char *)path_name.c_str());
        /* Set */
        if (!path_name.empty())
          dbserver_update_storage_config_mountpath((char *)path_name.c_str());
        else if (hdd_id == 1)
          dbserver_update_storage_config_mountpath((char *)"/mnt/sdcard");
        else if (hdd_id == 2)
          dbserver_update_storage_config_mountpath((char *)"/media/usb0");
        else if (hdd_id == 3)
          dbserver_update_storage_config_mountpath((char *)"/userdata/media");

        if (!storage_config.empty()) {
          video_quota_ratio =
              atoi(storage_config.at("iVideoQuotaRatio").dump().c_str());
          picture_quota_ratio =
              atoi(storage_config.at("iPictureQuotaRatio").dump().c_str());
          dbserver_update_storage_media_folder_duty(camera_id, 0,
                                                    video_quota_ratio, -1);
          dbserver_update_storage_media_folder_duty(camera_id, 1,
                                                    picture_quota_ratio, -1);
        }
        /* Get new info */
        content = storage_quota_get(hdd_id);
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    } else if (path_specific_resource.find("format") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_hdd_id.empty()) { /* path is storage/format */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else { /* path is storage/format/1 */
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        std::string path_name = get_hdd_name_by_id(NULL, hdd_id);
        minilog_debug("format path_name is %s\n", (char *)path_name.c_str());
        /* Set */
        if (!path_name.empty())
          storage_manager_diskformat((char *)path_name.c_str(),
                                     (char *)"fat32");
        else if (hdd_id == 1)
          storage_manager_diskformat((char *)"/mnt/sdcard", (char *)"fat32");
        else if (hdd_id == 2)
          storage_manager_diskformat((char *)"/media/usb0", (char *)"fat32");
        else if (hdd_id == 3)
          storage_manager_diskformat((char *)"/userdata/media",
                                     (char *)"fat32");
        Resp.setHeader(HttpStatus::kOk, "OK");
        Resp.setApiData(content);
      }
    } else if (!path_specific_resource.compare("search")) {
      if (!storage_config.empty())
        content = search_file_list_get(storage_config);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
    } else if (path_specific_resource.find("snap-plan") != std::string::npos) {
      if (path_hdd_id.empty()) { /* path is storage/snap-plan */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        if (hdd_id == 0) { /* path is storage/snap-plan/0 */
          /* Erase unchanged data */
          cfg_old = storage_plan_get(0);
          cfg_old.erase("iShotNumber");
          diff = nlohmann::json::diff(cfg_old, storage_config);
          for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
            if (diff.dump().find(x.key()) == diff.dump().npos)
              storage_config.erase(x.key());
          }
          /* Set */
          if (!storage_config.empty()) {
            dbserver_set_storage_plan_snap(
                (char *)storage_config.dump().c_str(), 0);
            mediaserver_sync_schedules();
          }
          /* Get new info */
          content = storage_plan_get(0);
          content.erase("iShotNumber");
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else if (hdd_id == 1) { /* path is storage/snap-plan/1 */
          /* Erase unchanged data */
          cfg_old = storage_plan_get(1);
          diff = nlohmann::json::diff(cfg_old, storage_config);
          for (auto &x : nlohmann::json::iterator_wrapper(cfg_old)) {
            if (diff.dump().find(x.key()) == diff.dump().npos)
              storage_config.erase(x.key());
          }
          /* Set */
          if (!storage_config.empty())
            dbserver_set_storage_plan_snap(
                (char *)storage_config.dump().c_str(), 1);
          /* Get new info */
          content = storage_plan_get(1);
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else if (path_specific_resource.find("advance-para") !=
               std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      if (path_hdd_id.empty()) { /* path is storage/advance-para */
        Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
      } else {
        hdd_id = atoi(path_hdd_id.substr(0, path_hdd_id.size()).c_str());
        if (!hdd_id) {
          char *str = dbserver_storage_get((char *)TABLE_STORAGE_ADVANCE_PARA);
          nlohmann::json old_config =
              nlohmann::json::parse(str).at("jData").at(hdd_id);
          nlohmann::json diff =
              nlohmann::json::diff(old_config, storage_config);
          for (auto &x : nlohmann::json::iterator_wrapper(old_config)) {
            if (diff.dump().find(x.key()) == diff.dump().npos)
              storage_config.erase(x.key());
          }
          if (!storage_config.empty()) {
            dbserver_storage_set((char *)TABLE_STORAGE_ADVANCE_PARA,
                                 (char *)storage_config.dump().c_str(), hdd_id);
            mediaserver_sync_schedules();
          }
          str = dbserver_storage_get((char *)TABLE_STORAGE_ADVANCE_PARA);
          content = nlohmann::json::parse(str).at("jData").at(hdd_id);
          Resp.setHeader(HttpStatus::kOk, "OK");
          Resp.setApiData(content);
        } else {
          Resp.setErrorResponse(HttpStatus::kNotImplemented, "Not Implemented");
        }
      }
    } else if (path_specific_resource.find("delete") != std::string::npos) {
#ifdef ENABLE_JWT
      if (user_level > 1) {
        Resp.setErrorResponse(HttpStatus::kUnauthorized, "Unauthorized");
        return;
      }
#endif
      nlohmann::json file_list = storage_config.at("name");
      std::string delete_type = storage_config.at("type");
      int rst = 1;
      if (!file_list.empty()) {
        rst = media_file_delete(delete_type, file_list);
      }
      content.emplace("rst", rst);
      Resp.setHeader(HttpStatus::kOk, "OK");
      Resp.setApiData(content);
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
