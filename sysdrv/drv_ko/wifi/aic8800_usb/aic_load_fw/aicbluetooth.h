#ifndef _AICBLUETOOTH_H
#define _AICBLUETOOTH_H

struct aicbt_patch_table {
	char     *name;
	uint32_t type;
	uint32_t *data;
	uint32_t len;
	struct aicbt_patch_table *next;
};


int aic_bt_platform_init(struct aic_usb_dev *sdiodev);

void aic_bt_platform_deinit(struct aic_usb_dev *sdiodev);

int rwnx_plat_bin_fw_upload_android(struct aic_usb_dev *sdiodev, u32 fw_addr,
                               char *filename);

int rwnx_plat_m2d_flash_ota_android(struct aic_usb_dev *usbdev, char *filename);

int rwnx_plat_m2d_flash_ota_check(struct aic_usb_dev *usbdev, char *filename);

int rwnx_plat_bin_fw_patch_table_upload_android(struct aic_usb_dev *usbdev, char *filename);

int rwnx_plat_userconfig_upload_android(struct aic_usb_dev *usbdev, char *filename);
int rwnx_plat_flash_bin_upload_android(struct aic_usb_dev *usbdev, u32 fw_addr, char *filename);

int8_t rwnx_atoi(char *value);
uint32_t rwnx_atoli(char *value);
int aicbt_patch_table_free(struct aicbt_patch_table *head);
struct aicbt_patch_table *aicbt_patch_table_alloc(struct aic_usb_dev *usbdev, const char *filename);
int aicbt_patch_info_unpack(struct aicbt_patch_info_t *patch_info, struct aicbt_patch_table *head_t);
int aicbt_patch_table_load(struct aic_usb_dev *usbdev, struct aicbt_patch_table *_head);

#endif
