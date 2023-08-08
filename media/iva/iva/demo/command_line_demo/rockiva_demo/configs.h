#ifndef _IVA_APP_CONFIGS_H
#define _IVA_APP_CONFIGS_H

#ifdef __cplusplus
extern "C" {
#endif

#define MODEL_DATA_PATH "/userdata/rockiva_data"
#define LICENSE_PATH "/userdata/key.lic"
#define FACE_DATABASE_PATH "/userdata/face.db"

#define OUT_DIR_PATH "./out"
#define OUT_FRAMES_PATH "./out/frames"
#define OUT_CAPTURE_PATH "./out/captures"

// 为输入图像分配的内存类型：0:CPU 1:DMA
#define ALLOC_IMAGE_BUFFER_TYPE 0

#ifdef __cplusplus
} // extern "C"
#endif

#endif