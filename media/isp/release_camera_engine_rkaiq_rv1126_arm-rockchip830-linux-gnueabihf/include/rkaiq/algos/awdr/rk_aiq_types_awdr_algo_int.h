/******************************************************************************
 *
 * Copyright 2019, Fuzhou Rockchip Electronics Co.Ltd . All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Fuzhou Rockchip Electronics Co.Ltd .
 *
 *
 *****************************************************************************/
#ifndef __RK_AIQ_TYPES_AWDR_ALGO_INT_H__
#define __RK_AIQ_TYPES_AWDR_ALGO_INT_H__

#include "RkAiqCalibDbTypes.h"
//#include "isp20/rkisp2-config.h"

#define WDR_PROC_SIZE          48

enum wdr_proc_mode {
    WDR_MODE_GLOBAL = 0,
    WDR_MODE_BLOCK = 1,

};

enum {
    AWDR_NORMAL = 0,
    AWDR_HDR = 1,
    AWDR_NIGHT = 2
};

typedef enum wdr_OpMode_s {
    WDR_OPMODE_API_OFF = 0, // run IQ ahdr
    WDR_OPMODE_AUTO = 1, //run api auto ahdr
    WDR_OPMODE_MANU = 2, //run api manual ahdr
} wdr_OpMode_t;

typedef struct wdrAttrStManu_s {
    int level;
} wdrAttrStManu_t;

typedef struct WdrCurrCtlData_s {
    float Envlv;
    float ISO;
} WdrCurrCtlData_t;

typedef struct wdrAttr_s {
    wdr_OpMode_t mode;
    CalibDb_Awdr_Para_t stAuto;
    wdrAttrStManu_t stManu;
    WdrCurrCtlData_t CtlInfo;
} wdrAttr_t;

typedef struct RkAiqAwdrProcResult_s {
    bool enable;
    enum wdr_proc_mode mode;
    unsigned int c_wdr[WDR_PROC_SIZE];
} RkAiqAwdrProcResult_t;


#endif
