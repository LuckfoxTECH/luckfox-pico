#ifndef __RKAIQ_CALIB_H__
#define __RKAIQ_CALIB_H__

#include "RkAiqCalibDbTypesV2.h"
#include "RkAiqCalibDbV2Helper.h"
#include "stddef.h"

extern CamCalibDbProj_t *rkaiq_calib_load(void *offset, size_t len);
int rkaiq_parse_bin_map(uint8_t *data, size_t len);

int rkaiq_get_default_calib(CamCalibDbProj_t *calibproj, CamCalibDbV2Context_t *def_calib, int mode);

int rkaiq_get_night_calib(CamCalibDbProj_t *calibproj, CamCalibDbV2Context_t *def_calib, int mode);

extern int restoreBinStructMap(uint8_t *data, size_t len, uintptr_t restore_ptr);
extern void bin2calib(void *bin_buff, CamCalibDbV2Context_t* def_calib);

#endif /*__RKAIQ_CALIB_H__*/
