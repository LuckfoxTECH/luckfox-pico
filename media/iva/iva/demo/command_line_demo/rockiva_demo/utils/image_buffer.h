#ifndef _IMAGE_BUFFER_MANAGER_H_
#define _IMAGE_BUFFER_MANAGER_H_

#include "rockiva_common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  IMAGE_BUFFER_UNINITIAL = 0,
  IMAGE_BUFFER_UNUSED,
  IMAGE_BUFFER_USING
} IvaImageBufState;

typedef struct {
  int index;
  int type; // 0:CPU 1:DMA
  IvaImageBufState state;
  RockIvaImage image;
  void *extraInfo;
} IvaImageBuf;

typedef void *ImageBufferManager;

// buf_type: 0:CPU 1:DMA
ImageBufferManager CreateImageBufferManagerPreAlloc(int count, int w, int h,
                                                    int c, int buf_type);

ImageBufferManager CreateImageBufferManager(int count);

IvaImageBuf *GetImageBuffer(ImageBufferManager manager, int index);

int DestroyImageBufferManager(ImageBufferManager manager);

IvaImageBuf *AcquireImageBuffer(ImageBufferManager manager,
                                unsigned long timeoutms);

IvaImageBuf *FindImageBufferByFrameId(ImageBufferManager manager, int frameId);

void RetureImageBuffer(ImageBufferManager manager, IvaImageBuf *imgBuf);

#ifdef __cplusplus
} // extern "C"
#endif

#endif