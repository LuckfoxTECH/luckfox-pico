#ifndef _RK_AIQ_UAPI_AGAIN_INT_V1_H_
#define _RK_AIQ_UAPI_AGAIN_INT_V1_H_

#include "base/xcam_common.h"
#include "rk_aiq_algo_des.h"
#include "again/rk_aiq_types_again_algo_int.h"

// need_sync means the implementation should consider
// the thread synchronization
// if called by RkAiqAlscHandleInt, the sync has been done
// in framework. And if called by user app directly,
// sync should be done in inner. now we just need implement
// the case of need_sync == false; need_sync is for future usage.




#endif
