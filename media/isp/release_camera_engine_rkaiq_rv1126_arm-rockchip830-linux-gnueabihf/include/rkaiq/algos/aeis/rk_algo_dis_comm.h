#ifndef __RK_ALGO_DIS_COMM_H__
#define __RK_ALGO_DIS_COMM_H__

#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#define RKALGO_DIS_PYR_NUM_MIN 2                    /* min pyramid images' num */
#define RKALGO_DIS_PYR_NUM_MAX 6                    /* max pyramid images' num */
#define RKALGO_DIS_MESH_STEP_MIN 1                  /* min mesh step */
#define RKALGO_DIS_MESH_STEP_MAX 256                /* max mesh step */
#define RKALGO_DIS_CROP_RATIO_MIN 50                /* min crop ratio, unit: 0.01 */
#define RKALGO_DIS_CROP_RATIO_MAX 98                /* max crop ratio, unit: 0.01 */
#define RKALGO_DIS_IMG_SZ_MIN 256                   /* min image(src/dst) size(width/height) */
#define RKALGO_DIS_IMG_SZ_MAX 8192                  /* max image(src/dst) size(width/height) */
    /*
    * @enum    RKALGO_DIS_STATUS_E
    * @brief   the return status of function
    * RKALGO_DIS_STATUS_EOF              : error of function inside
    * RKALGO_DIS_STATUS_OK               : run successfully
    * RKALGO_DIS_STATUS_FILE_READ_ERROR  : error: fail to read file
    * RKALGO_DIS_STATUS_FILE_WRITE_ERROR : error: fail to write file
    * RKALGO_DIS_STATUS_INVALID_PARAM    : error: invalid parameter
    * RKALGO_DIS_STATUS_ALLOC_FAILED     : error: fail to alloc buffer
    * RKALGO_DIS_STATUS_BUTT             : reserved fields
    */
    typedef enum
    {
        RKALGO_DIS_STATUS_EOF = -1,
        RKALGO_DIS_STATUS_OK = 0,
        RKALGO_DIS_STATUS_FILE_READ_ERROR = 1,
        RKALGO_DIS_STATUS_FILE_WRITE_ERROR = 2,
        RKALGO_DIS_STATUS_INVALID_PARAM = 3,
        RKALGO_DIS_STATUS_ALLOC_FAILED = 4,
        RKALGO_DIS_STATUS_BUTT
    }RKALGO_DIS_STATUS_E;

    /*
    * @enum    RKALGO_DIS_BOOL
    * @brief   define the bool enum
    */
    typedef enum
    {
        RKALGO_DIS_FALSE = 0,
        RKALGO_DIS_TRUE = 1,
    }RKALGO_DIS_BOOL;

    /*
    * @enum    RKALGO_DIS_IMG_FORMAT_E
    * @brief   define the images format
    */
    typedef enum
    {
        RKALGO_DIS_IMG_FORMAT_GRAY = 0,
        RKALGO_DIS_IMG_FORMAT_RGB = 1,
        RKALGO_DIS_IMG_FORMAT_NV12 = 2,
        RKALGO_DIS_IMG_FORMAT_BUTT
    }RKALGO_DIS_IMG_FORMAT_E;

    /*
    * @enum    RKALGO_DIS_MODE
    * @brief   define the mode of DIS
    */
    typedef enum
    {
        RKALGO_DIS_MODE_BYPASS = 0,
        RKALGO_DIS_MODE_STILLCROP = 1,
        RKALGO_DIS_MODE_DIS = 2,
        RKALGO_DIS_MODE_BUTT,
    }RKALGO_DIS_MODE;

    /*
    * @enum    RKALGO_DIS_ALGORITHM
    * @brief   define the option of DIS algorithm
    */
    typedef enum
    {
        RKALGO_DIS_ALGORITHM_GME_4DOF = 0,
        RKALGO_DIS_ALGORITHM_GME_6DOF = 1,
        RKALGO_DIS_ALGORITHM_GYRO = 2,
        RKALGO_DIS_ALGORITHM_BUTT,
    }RKALGO_DIS_ALGORITHM;

    /*
    * @enum    RKALGO_DIS_PDT_TYPE_E
    * @brief   define the product type
    */
    typedef enum
    {
        RKALGO_DIS_PDT_TYPE_IPC = 0,
        RKALGO_DIS_PDT_TYPE_DV = 1,
        RKALGO_DIS_PDT_TYPE_DRONE = 2,
        RKALGO_DIS_PDT_TYPE_BUTT,
    }RKALGO_DIS_PDT_TYPE_E;

    /*
    * @enum    RKALGO_DIS_MOTION_LEVEL_E
    * @brief   define the motion level
    */
    typedef enum
    {
        RKALGO_DIS_MOTION_LEVEL_LOW = 0,
        RKALGO_DIS_MOTION_LEVEL_NORMAL = 1,
        RKALGO_DIS_MOTION_LEVEL_HIGH = 2,
        RKALGO_DIS_MOTION_LEVEL_BUTT,
    }RKALGO_DIS_MOTION_LEVEL_E;

    /*
    * @enum    RKALGO_DIS_MESH_TYPE
    * @brief   define the output mesh type DIS
    */
    typedef enum
    {
        RKALGO_DIS_MESH_TYPE_GPU = 0,
        RKALGO_DIS_MESH_TYPE_DEBUG = 1,
        RKALGO_DIS_MESH_TYPE_FEC = 2,
        RKALGO_DIS_MESH_TYPE_BUTT,
    }RKALGO_DIS_MESH_TYPE;

    /*
    * @enum    RKALGO_DIS_FEC_VER_E
    * @brief   define the version of FEC
    */
    typedef enum
    {
        RKALGO_DIS_FEC_VER_1109_1126 = 0,
        RKALGO_DIS_FEC_VER_3588 = 1,
        RKALGO_DIS_FEC_VER_BUTT,
    } RKALGO_DIS_FEC_VER_E;

#ifdef __cplusplus
} /* extern "C" { */
#endif


#endif // !__RK_ALGO_DIS_COMM_H__