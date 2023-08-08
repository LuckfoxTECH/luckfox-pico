/*********************************************************************************
 *Copyright(C),2015-2020, TUYA www.tuya.comm
 *FileName:    tuya_ipc_sd_demo
 **********************************************************************************/
#include "tuya_ipc.h"
#include "tuya_ipc_stream_storage.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/types.h>
#include <unistd.h>

/************************
Description: Using the TUYA SD card storage and playback function,
the developers need to implement the relevant interface.
Note：Interface names cannot be changed, and these interfaces are declared and used in SDK.
This demo file gives the management implementation of SD card operation for typical embedded devices
in Linux system. Developers can modify it according to their practical application.

VOID tuya_ipc_sd_format(VOID);
VOID tuya_ipc_sd_remount(VOID);
E_SD_STATUS tuya_ipc_sd_get_status(VOID);
VOID tuya_ipc_sd_get_capacity(UINT_T *p_total, UINT_T *p_used, UINT_T *p_free);
CHAR_T *tuya_ipc_get_sd_mount_path(VOID);

************************/
#define MAX_MMC_NAME_LEN 16
#define MAX_MOUNTS_INFO_LEN 1024
#define LINUX_SD_DEV_FILE "/dev/mmcblk0"
#define LINUX_MOUNT_INFO_FILE "/proc/mounts"
#define SD_MOUNT_PATH "/tmp" // keep same to IPC_APP_SD_BASE_PATH
#define FORMAT_CMD "mkfs.vfat"

STATIC CHAR_T s_mounts_info[MAX_MOUNTS_INFO_LEN];
STATIC CHAR_T s_mmcblk_name[MAX_MMC_NAME_LEN] = {0};

// Implementation Interface of Formatting Operation
VOID tuya_ipc_sd_format(VOID) {
	CHAR_T format_cmd[256] = {0};
	char buffer[512] = {0};
	printf("sd format begin\n");

	snprintf(format_cmd, 256, "umount %s;%s %s;mkdir -p /mnt/sdcard;mount -t auto %s /mnt/sdcard;",
	         s_mmcblk_name, FORMAT_CMD, s_mmcblk_name, s_mmcblk_name);
	printf("execute: %s\n", format_cmd);
	FILE *pp = popen(format_cmd, "r");
	if (NULL != pp) {
		fgets(buffer, sizeof(buffer), pp);
		printf("%s\n", buffer);
		pclose(pp);
	} else {
		printf("format_sd_card failed\n");
	}
	printf("sd format end\n");
}

// Implementation Interface for Remounting
VOID tuya_ipc_sd_remount(VOID) {
	// CHAR_T format_cmd[128] = {0};
	// char buffer[512] = {0};
	// E_SD_STATUS status = SD_STATUS_UNKNOWN;

	// status = tuya_ipc_sd_get_status();
	// if(SD_STATUS_NORMAL == status)
	// {
	//     printf("sd don't need to remount!\n");
	//     return;
	// }
	// printf("remount_sd_card ..... \n");

	// snprintf(format_cmd,128,"umount %s;sleep 1;mount -t auto %s
	// /mnt/sdcard;",s_mmcblk_name,s_mmcblk_name); FILE *pp = popen(format_cmd, "r"); if(NULL != pp)
	// {
	//     fgets(buffer,sizeof(buffer),pp);
	//     printf("%s\n",buffer);
	//     pclose(pp);
	// }
	// else
	// {
	//     printf("remount_sd_card failed\n");
	// }
}

STREAM_STORAGE_WRITE_MODE_E tuya_ipc_sd_get_mode_config(VOID) {
	//     BOOL_T sd_on = IPC_APP_get_sd_record_onoff();
	//     if(sd_on)
	//     {
	//         UINT_T sd_mode = IPC_APP_get_sd_record_mode();
	//         if(0 == sd_mode)
	//         {
	//             return SS_WRITE_MODE_EVENT;
	//         }
	//         else
	//         {
	//             return SS_WRITE_MODE_ALL;
	//         }
	//     }
	//     else
	//     {
	//         return SS_WRITE_MODE_NONE;
	//     }
}

// Implementation Interface for Obtaining SD Card Status
E_SD_STATUS tuya_ipc_sd_get_status(VOID) {
	// 	return SD_STATUS_NORMAL;// debugg

	//     // UserTodo
	//     FILE *fp = fopen(LINUX_SD_DEV_FILE, "rb");
	//     if(!fp)
	//     {
	//         return SD_STATUS_NOT_EXIST;
	//     }
	//     fclose(fp);

	//     strcpy(s_mmcblk_name,LINUX_SD_DEV_FILE);
	//     if(0 == access(LINUX_SD_DEV_FILE"p1",F_OK)) //Default node name information
	//     {
	//         strcat(s_mmcblk_name,"p1");
	//     }

	//     fp = fopen(LINUX_MOUNT_INFO_FILE, "rb");
	//     if(fp)
	//     {
	//         memset(s_mounts_info,0,sizeof(s_mounts_info));
	//         fread(s_mounts_info,1,MAX_MOUNTS_INFO_LEN,fp);
	//         fclose(fp);
	//         CHAR_T *mmcblk_name_start = strstr(s_mounts_info,"/dev/mmcblk"); //Confirm the final
	//         node name in Mount information CHAR_T *mmcblk_name_end = strstr(s_mounts_info,"
	//         /mnt/sdcard"); if(mmcblk_name_start && mmcblk_name_end)
	//         {
	//             int mmcblk_name_len = mmcblk_name_end-mmcblk_name_start;
	//             if(mmcblk_name_len >= MAX_MMC_NAME_LEN)
	//             {
	//                  return SD_STATUS_ABNORMAL;
	//             }
	//             strncpy(s_mmcblk_name, mmcblk_name_start, mmcblk_name_len);
	//             s_mmcblk_name[mmcblk_name_len] = '\0';
	//         }
	//         //There are device nodes but no mount information. Generally, the card format is
	//         incorrect and report abnormal. else
	//         {
	//             return SD_STATUS_ABNORMAL;
	//         }
	//         //If the mount information of the SD card is not at the end and there is a ro mount
	//         behind it, there will be a problem. if(NULL != strstr(mmcblk_name_start,"ro,"))
	//         {
	//             return SD_STATUS_ABNORMAL;
	//         }
	//         if(NULL == strstr(mmcblk_name_start,"vfat"))
	//         {
	//             return SD_STATUS_ABNORMAL;
	//         }
	//         if (access(s_mmcblk_name,0))
	//         {
	//             return SD_STATUS_ABNORMAL;
	//         }

	//         return SD_STATUS_NORMAL;
	//     }
	//     else
	//     {
	//         return SD_STATUS_UNKNOWN;
	//     }
}

// SD card capacity acquisition interface, unit: KB
VOID tuya_ipc_sd_get_capacity(UINT_T *p_total, UINT_T *p_used, UINT_T *p_free) {
	*p_total = 1024 * 1024;
	*p_used = 1024 * 20;
	*p_free = *p_total - *p_used;
	return;

	struct statfs sd_fs;
	if (statfs("/mnt/sdcard", &sd_fs) != 0) {
		printf("statfs failed!/n");
		return;
	}

	*p_total = (UINT_T)(((UINT64_T)sd_fs.f_blocks * (UINT64_T)sd_fs.f_bsize) >> 10);
	*p_used =
	    (UINT_T)((((UINT64_T)sd_fs.f_blocks - (UINT64_T)sd_fs.f_bfree) * (UINT64_T)sd_fs.f_bsize) >>
	             10);
	*p_free = (UINT_T)(((UINT64_T)sd_fs.f_bavail * (UINT64_T)sd_fs.f_bsize) >> 10);
	printf("sd capacity: total: %d KB, used %d KB, free %d KB\n", *p_total, *p_used, *p_free);
	return;
}

// get the path of mounting sdcard
CHAR_T *tuya_ipc_get_sd_mount_path(VOID) { return SD_MOUNT_PATH; }

INT_T tuya_ipc_sd_status_upload(INT_T status) {
	// IPC_APP_report_sd_status_changed(status);
	return 0;
}

// OPERATE_RET TUYA_APP_Init_Stream_Storage(TUYA_IPC_SDK_LOCAL_STORAGE_S * p_local_storage_info)
// {
//     STATIC BOOL_T s_stream_storage_inited = FALSE;
//     if(s_stream_storage_inited == TRUE)
//     {
//         printf("The Stream Storage Is Already Inited");
//         return OPRT_OK;
//     }

//     if(p_local_storage_info == NULL)
//     {
//         printf("Init Stream Storage fail. Param is null");
//         return OPRT_INVALID_PARM;
//     }

//     TUYA_IPC_STORAGE_VAR_S stg_var;

//     IPC_MEDIA_INFO_S* p_media_info = IPC_APP_Get_Media_Info();
//     if(p_media_info == NULL)
//     {
//         return OPRT_COM_ERROR;
//     }

//     memset(&stg_var, 0, SIZEOF(TUYA_IPC_STORAGE_VAR_S));
//     memcpy(stg_var.base_path, p_local_storage_info->storage_path, SS_BASE_PATH_LEN);
//     memcpy(&stg_var.media_setting, p_media_info, SIZEOF(IPC_MEDIA_INFO_S));
//     stg_var.max_event_per_day = p_local_storage_info->max_event_num_per_day;
//     stg_var.sd_status_changed_cb = p_local_storage_info->sd_status_cb;

//     printf("Init Stream_Storage SD:%s", p_local_storage_info->storage_path);
//     OPERATE_RET ret = tuya_ipc_ss_init(&stg_var);
//     if(ret != OPRT_OK)
//     {
//         printf("Init Main Video Stream_Storage Fail. %d", ret);
//         return OPRT_COM_ERROR;
//     }
//     return OPRT_OK;
// }
