#include <linux/types.h>
#include "aicwf_usb.h"

#define USB_DEVICE_ID_AIC_8800D80       0x8D80
#define USB_DEVICE_ID_AIC_8800D81       0x8D81

#define FW_BASE_NAME_8800D80                "fmacfw_8800d80.bin"
#define FW_RF_BASE_NAME_8800D80             "fmacfw_rf_8800d80.bin"
#define FW_PATCH_BASE_NAME_8800D80          "fw_patch_8800d80.bin"
#define FW_ADID_BASE_NAME_8800D80           "fw_adid_8800d80.bin"
#define FW_PATCH_TABLE_NAME_8800D80         "fw_patch_table_8800d80.bin"

#define FW_BASE_NAME_8800D80_U02            "fmacfw_8800d80_u02.bin"
#define FW_RF_BASE_NAME_8800D80_U02         "lmacfw_rf_8800d80_u02.bin"
#define FW_PATCH_BASE_NAME_8800D80_U02      "fw_patch_8800d80_u02.bin"
#define FW_ADID_BASE_NAME_8800D80_U02       "fw_adid_8800d80_u02.bin"
#define FW_CALIBMODE_NAME_8800D80_U02       "calibmode_8800d80.bin"
#define FW_PATCH_TABLE_NAME_8800D80_U02     "fw_patch_table_8800d80_u02.bin"

#define FLASH_BIN_8800M80                   "host_wb_8800m80.bin"

#define FW_USERCONFIG_NAME_8800D80          "aic_userconfig_8800d80.txt"

#define RAM_FMAC_FW_ADDR_8800D80           0x100000
#define RAM_FMAC_RF_FW_ADDR_8800D80        0x110000
#define FW_RAM_ADID_BASE_ADDR_8800D80      0x002017E0
#define FW_RAM_PATCH_BASE_ADDR_8800D80     0x0020B2B0

#define RAM_FMAC_FW_ADDR_8800D80_U02       0x120000
#define RAM_FMAC_RF_FW_ADDR_8800D80_U02    0x120000
#define FW_RAM_ADID_BASE_ADDR_8800D80_U02  0x00201940
#define FW_RAM_CALIBMODE_ADDR_8800D80_U02  0x1e0000
#define FW_RAM_PATCH_BASE_ADDR_8800D80_U02 0x0020B43c

#define FLASH_BIN_ADDR_8800M80             0x8000000


int aicwf_patch_config_8800d80(struct aic_usb_dev *usb_dev);
int rwnx_plat_userconfig_load_8800d80(struct aic_usb_dev *usbdev);
int system_config_8800d80(struct aic_usb_dev *usb_dev);
int aicfw_download_fw_8800d80(struct aic_usb_dev *usb_dev);

