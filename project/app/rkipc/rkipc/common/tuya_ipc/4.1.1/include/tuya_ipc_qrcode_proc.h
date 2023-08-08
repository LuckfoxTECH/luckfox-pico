/*********************************************************************************
* Copyright(C),2019, TUYA www.tuya.com

* FileName:		tuya_ipc_qrcode_proc.h
* Note			tuya_ipc_qrcode_proc enhance
* Version		V1.0.0
* Data			2019.04
**********************************************************************************/

#ifndef _TUYA_IPC_QRCODE_PROC_H_
#define _TUYA_IPC_QRCODE_PROC_H_

#include "tuya_ipc_img_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************
* Tuya qrcode image enhance
* in_data  		input YUV420 ,from platform decode
* int_w			input width	,from platform decode
* in_h			input height ,from platform decode
* out_data		output YUV420 ,send in zbar
* out_w			output width ,send in zbar
* out_h			output height ,send in zabr

* binary_thres	image binary threshold，depends on different ISP para
                binary data range: 100-150, recommend 128.
* scale_flag	image scale, needed when Low-resolution images
                1: scale; 0: no scale
                if origin size 16*9, the output will be 32*18 when seting 1
                recommend original size 640*360, slower when scale to 1280*720.
**********************************************************************************/
OPERATE_RET Tuya_Ipc_QRCode_Enhance(UCHAR_T *in_data, INT_T in_w, INT_T in_h, UCHAR_T **out_data,
                                    INT_T *out_w, INT_T *out_h, INT_T binary_thres,
                                    BOOL_T scale_flag);

#ifdef __cplusplus
}
#endif

#endif // !_TUYA_IPC_QRCODE_PROC_H_
