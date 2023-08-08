/*************************************************************************
    > File Name: main.cpp
    > Author: jkand.huang
    > Mail: jkand.huang@rock-chips.com
    > Created Time: Mon 20 May 2019 10:56:06 AM CST
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/reboot.h>
#include "bootloader.h"

typedef enum{
    RK_UPGRADE_FINISHED,
    RK_UPGRADE_START,
    RK_UPGRADE_ERR,
}RK_Upgrade_Status_t;

RK_Upgrade_Status_t m_status = RK_UPGRADE_ERR;

void display() {
    LOGI("--misc=now                           Linux A/B mode: Setting the current partition to bootable.\n");
    LOGI("--misc=other                         Linux A/B mode: Setting another partition to bootable.\n");
    LOGI("--misc=update                        Linux A/B mode: Setting the partition to be upgraded.\n");
    LOGI("--misc=display                       Display misc info.\n");
    LOGI("--tar_path=<path>                    Set upgrade firmware path.\n");
    LOGI("--save_dir=<path>                    Set the path for saving the image.\n");
    LOGI("--partition=<uboot/boot/system/all>  Set the partition to be upgraded.('all' means 'uboot', 'boot' and 'system' are included.)\n");
    LOGI("--extra_part=<name>                  Set the extra partition to be upgraded.\n");
    LOGI("--reboot                             Restart the machine at the end of the program.\n");
    // LOGI("--misc=wipe_userdata   Format data partition.\n");
    // LOGI("--misc_custom= < op >  Operation on misc for custom cmdline");
    // LOGI("        op:     read   Read custom cmdline to /tmp/custom_cmdline");
    // LOGI("                write  Write /tmp/custom_cmdline to custom area");
    // LOGI("                clean  clean custom area");
    // LOGI("--update               Upgrade mode.\n");
    // LOGI("--partition=0x3FFC00   Set the partition to be upgraded.(NOTICE: OTA not support upgrade loader and parameter)\n");
    // LOGI("                       0x3FFC00: 0011 1111 1111 1100 0000 0000.\n");
    // LOGI("                                 uboot trust boot recovery rootfs oem\n");
    // LOGI("                                 uboot_a uboot_b boot_a boot_b system_a system_b.\n");
    // LOGI("                       100000000000000000000000: Upgrade loader\n");
    // LOGI("                       010000000000000000000000: Upgrade parameter\n");
    // LOGI("                       001000000000000000000000: Upgrade uboot\n");
    // LOGI("                       000100000000000000000000: Upgrade trust\n");
    // LOGI("                       000010000000000000000000: Upgrade boot\n");
    // LOGI("                       000001000000000000000000: Upgrade recovery\n");
    // LOGI("                       000000100000000000000000: Upgrade rootfs\n");
    // LOGI("                       000000010000000000000000: Upgrade oem\n");
    // LOGI("                       000000001000000000000000: Upgrade uboot_a\n");
    // LOGI("                       000000000100000000000000: Upgrade uboot_b\n");
    // LOGI("                       000000000010000000000000: Upgrade boot_a\n");
    // LOGI("                       000000000001000000000000: Upgrade boot_b\n");
    // LOGI("                       000000000000100000000000: Upgrade system_a\n");
    // LOGI("                       000000000000010000000000: Upgrade system_b\n");
    // LOGI("                       000000000000001000000000: Upgrade misc\n");
    // LOGI("                       000000000000000100000000: Upgrade userdata\n");
    // LOGI("--version_url=url      The path to the file of version.\n");

}

static const struct option engine_options[] = {
  { "misc", required_argument, NULL, 'm' },
  { "tar_path", required_argument, NULL, 't' + 'p'},
  { "save_dir", required_argument, NULL, 's' + 'p'},
  { "partition", required_argument, NULL, 'p' },
  { "extra_part", required_argument, NULL, 'e' + 'p' },
  { "reboot", no_argument, NULL, 'r' },
  { "help", no_argument, NULL, 'h' },
  { NULL, 0, NULL, 0 },
};

int main(int argc, char *argv[]) {
    LOGI("*** rk_ota: Version V1.0.0 ***.\n");
    int arg;
    char *tar_path = NULL;
    char *save_dir = NULL;
    char *misc_func = NULL;
    char *partition = NULL;
    char *extra_part = NULL;
    bool is_reboot = false;


    while ((arg = getopt_long(argc, argv, "", engine_options, NULL)) != -1) {
        switch (arg) {
        case 'm': misc_func = optarg; continue;
        case 't' + 'p': tar_path = optarg; continue;
        case 's' + 'p': save_dir = optarg; continue;
        case 'p': partition = optarg; continue;
        case 'e' + 'p': extra_part = optarg; continue;
        case 'r': is_reboot = true; continue;
        case 'h': display(); break;
        case '?':
            LOGE("Invalid command argument\n");
            continue;
        }
    }

    if (misc_func != NULL) {
        if (strcmp(misc_func, "now") == 0) {
            if (setSlotSucceed() ==0) {
                m_status = RK_UPGRADE_FINISHED;
            }
        } else if (strcmp(misc_func, "other") == 0) {
            if (setSlotActivity() == 0) {
                m_status = RK_UPGRADE_FINISHED;
            }
        } else if (strcmp(misc_func, "update") == 0) {
            if (miscUpdate(tar_path, save_dir, partition, extra_part) == 0) {
                m_status = RK_UPGRADE_FINISHED;
            }
        } else if (strcmp(misc_func, "display") == 0) {
            miscDisplay();
        } else {
            LOGE("unknow misc cmdline : %s.\n", misc_func);
            return 0;
        }
    }

    if (is_reboot && (m_status == RK_UPGRADE_FINISHED)) {
        sync();
        reboot(RB_AUTOBOOT);
        // system(" echo b > /proc/sysrq-trigger ");
    }

    return m_status;
}
