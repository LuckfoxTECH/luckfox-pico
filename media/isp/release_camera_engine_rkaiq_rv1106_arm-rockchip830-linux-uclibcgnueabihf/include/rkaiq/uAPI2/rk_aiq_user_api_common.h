

#ifndef __RK_AIQ_USER_API2_COMMON_H__
#define __RK_AIQ_USER_API2_COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum opMode_e {
    OP_AUTO   = 0,
    OP_MANUAL = 1,
    OP_SEMI_AUTO = 2,
    OP_REG_MANUAL = 3,
    OP_INVAL
} opMode_t;

/*
*****************************
* Common
*****************************
*/

typedef enum dayNightScene_e {
    DAYNIGHT_SCENE_DAY   = 0,
    DAYNIGHT_SCENE_NIGHT = 1,
    DAYNIGHT_SCENE_INVAL,
} dayNightScene_t;

typedef struct paRange_s {
    float max;
    float min;
} paRange_t;

typedef enum awbRange_e {
    AWB_RANGE_0 = 0,
    AWB_RANGE_1 = 1,
    AWB_RANGE_INVAL,
} awbRange_t;

typedef enum aeMode_e {
    AE_AUTO          = 0,
    AE_IRIS_PRIOR    = 1,
    AE_SHUTTER_PRIOR = 2,
} aeMode_t;


/*
*****************
*   (-1000, -1000)    (1000, -1000)
*   -------------------
*   |                 |
*   |                 |
*   |       (0,0)     |
*   |                 |
*   |                 |
*   -------------------
*                     (1000, 1000)
*****************
*/
typedef struct paRect_s {
    int x;
    int y;
    unsigned int w;
    unsigned int h;
} paRect_t;

typedef enum aeMeasAreaType_e {
    AE_MEAS_AREA_AUTO = 0,
    AE_MEAS_AREA_UP,
    AE_MEAS_AREA_BOTTOM,
    AE_MEAS_AREA_LEFT,
    AE_MEAS_AREA_RIGHT,
    AE_MEAS_AREA_CENTER,
} aeMeasAreaType_t;

typedef enum expPwrLineFreq_e {
    EXP_PWR_LINE_FREQ_DIS   = 0,
    EXP_PWR_LINE_FREQ_50HZ  = 1,
    EXP_PWR_LINE_FREQ_60HZ  = 2,
} expPwrLineFreq_t;

typedef enum antiFlickerMode_e {
    ANTIFLICKER_NORMAL_MODE = 0,
    ANTIFLICKER_AUTO_MODE   = 1,
} antiFlickerMode_t;

typedef struct frameRateInfo_s {
    opMode_t         mode;
    unsigned int     fps; /* valid when manual mode*/
} frameRateInfo_t;

#ifdef __cplusplus
}
#endif

#endif  /*__RK_AIQ_USER_API2_COMMON_H__*/
