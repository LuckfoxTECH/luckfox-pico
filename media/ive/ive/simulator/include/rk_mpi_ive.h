/*
 * Copyright 2020 Rockchip Electronics Co. LTD
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#ifndef _RK_MPI_IVE_H_
#define _RK_MPI_IVE_H_

#include <stdbool.h>

#include "rk_ive.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* End of #ifdef __cplusplus */

/**
 * @brief 
 * Prototype    : RK_MPI_IVE_Init
 * Description  : Init IVE context
 * @return RK_S32 
 */
RK_S32 RK_MPI_IVE_Init();

/**
 * @brief 
 * Prototype    : RK_MPI_IVE_Deinit
 * Description  : Deinit IVE context
 * @return RK_S32 
 */
RK_S32 RK_MPI_IVE_Deinit();

/**
 * @brief
 * Prototype    : RK_MPI_IVE_CvtImageToData
 * Description  : Convert IVE_IMAGE_S to IVE_DATA_S
 * @param image: Input image.
 * @param data: output data.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_CvtImageToData(IVE_IMAGE_S *image, IVE_DATA_S *data);

/**
 * @brief
 * Prototype    : RK_MPI_IVE_CvtDataToImage
 * Description  : Convert IVE_DATA_S to IVE_IMAGE_S
 * @param image: Input data.
 * @param data: output image.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_CvtDataToImage(IVE_DATA_S *data, IVE_IMAGE_S *image);

/**
 * @brief
 * Prototype    : RK_MPI_IVE_CvtImageToMemInfo
 * Description  : Convert IVE_IMAGE_S to IVE_MEM_INFO_S
 * @param image: Input image.
 * @param data: output mem.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_CvtImageToMemInfo(IVE_IMAGE_S *image, IVE_MEM_INFO_S *mem);

/**
 * @brief
 * Prototype    : RK_MPI_IVE_DMA
 * Description  : Direct memory access (DMA):
 *                  1.Direct memory copy;
 * 				    2. Copy with interval bytes;
 *                  3. Memset using 3 bytes;
 * 				    4. Memset using 8 bytes;
 * @param pHandle: Returned handle ID of a task.
 * @param pstSrc: Input source data.The input data is treated as U8C1 data.
 * @param pstDst: Output result data.
 * @param pstDmaCtrl: DMA control parameter.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_DMA(IVE_HANDLE *pHandle, IVE_DATA_S *pstSrc,
                   IVE_DST_DATA_S *pstDst, IVE_DMA_CTRL_S *pstDmaCtrl,
                   bool bInstant);

/**
 * @brief
 * Prototype    : RK_MPI_IVE_Filter
 * Description  : 5x5 template filter.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.
 * @param pstDst: Output result, of same type with the input.
 * @param pstFltCtrl: Control parameters of filter
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Filter(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                      IVE_DST_IMAGE_S *pstDst, IVE_FILTER_CTRL_S *pstFltCtrl,
                      bool bInstant);

/**
 * @brief
 * Prototype    : IVE_CSC
 * Description  : YUV2RGB\YUV2HSV\YUV2LAB\RGB2YUV color space conversion are
 *                supported.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.
 * @param pstDst: Output result, of same type with the input.
 * @param pstCscCtrl: Control parameters of filter
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_CSC(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                   IVE_DST_IMAGE_S *pstDst, IVE_CSC_CTRL_S *pstCscCtrl,
                   bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Sobel
 * Description  : SOBEL is used to extract the gradient information.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.
 * @param pstDstH The (horizontal) result of input image filtered by the input mask;
 * @param pstDstV The (vertical) result  of input image filtered by the transposed mask;
 * @param pstSobelCtrl: Control parameters;
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Sobel(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                     IVE_DST_IMAGE_S *pstDstH, IVE_DST_IMAGE_S *pstDstV,
                     IVE_SOBEL_CTRL_S *pstSobelCtrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_MagAndAng
 * Description  : MagAndAng is used to extract the edge information.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.
 * @param pstDstMag          Output magnitude.
 * @param pstDstAng          Output angle.
 * @param pstMagAndAngCtrl: Control parameters;
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_MagAndAng(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                         IVE_DST_IMAGE_S *pstDstMag, IVE_DST_IMAGE_S *pstDstAng,
                         IVE_MAG_AND_ANG_CTRL_S *pstMagAndAngCtrl,
                         bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Dilate
 * Parameters   : 5x5 template dilate. Only the U8C1 binary image input is supported. Or else the result is not expected.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input binary image, which consists of 0 or 255;
 * @param pstDst: Output result.
 * @param pstDilateCtrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Dilate(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                      IVE_DST_IMAGE_S *pstDst, IVE_DILATE_CTRL_S *pstDilateCtrl,
                      bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Erode
 * Parameters   : 5x5 template erode. Only the U8C1 binary image input is supported.Or else the result is not correct.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input binary image, which consists of 0 or 255;
 * @param pstDst: Output result.
 * @param pstErodeCtrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Erode(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                     IVE_DST_IMAGE_S *pstDst, IVE_ERODE_CTRL_S *pstErodeCtrl,
                     bool bInstant);

/**
 * @brief
 * Prototype    : IVE_And
 * Parameters   : Binary images' And operation.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc1: The input source1. Only U8C1 input format is supported.
 * @param pstSrc2: The input source2. Only U8C1 input format is supported.
 * @param pstDst: Output result of " src1 & src2 ".
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_And(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc1,
                   IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst,
                   bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Sub
 * Parameters   : Two gray images' Sub operation.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc1: Minuend of the input source.Only the U8C1 input format is supported.
 * @param pstSrc2: Subtrahend of the input source.Only the U8C1 input format is supported.
 * @param pstDst: Output result of src1 minus src2
 * @param pstSubCtrl: Control parameter
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Sub(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc1,
                   IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst,
                   IVE_SUB_CTRL_S *pstSubCtrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Or
 * Parameters   : Binary images' Or operation.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc1: The input source1. Only U8C1 input format is supported.
 * @param pstSrc2: The input source2. Only U8C1 input format is supported.
 * @param pstDst: Output result src1 or src2
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Or(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc1,
                  IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst,
                  bool bInstant);

/**
 * @brief
 * Prototype    : IVE_INTEG
 * Description  : Calculate the input gray image's integral image.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.Only the U8C1 input format is supported.
 * @param pstDst: Output result.Can be U32C1 or U64C1, relied on the control parameter.
 * @param pstIntegCtrl: Control parameters of integral.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Integ(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                     IVE_DST_IMAGE_S *pstDst, IVE_INTEG_CTRL_S *pstIntegCtrl,
                     bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Hist
 * Description  : Calculate the input gray image's histogram.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.Only the U8C1 input format is supported.
 * @param pstDst: Output result.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Hist(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                    IVE_DST_MEM_INFO_S *pstDst, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Thresh
 * Parameters   : Thresh operation to the input U8 image.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data. Only the U8C1 input format is supported.
 * @param pstDst: Output result.
 * @param pstThrCtrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Thresh(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                      IVE_DST_IMAGE_S *pstDst, IVE_THRESH_U8_CTRL_S *pstThrCtrl,
                      bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Thresh_S16
 * Parameters   : S16 image's THRESH operation.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data. Only the S16 input format is supported.
 * @param pstDst: Output result.
 * @param pstThrS16Ctrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Thresh_S16(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                          IVE_DST_IMAGE_S *pstDst,
                          IVE_THRESH_S16_CTRL_S *pstThrS16Ctrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Thresh_U16
 * Parameters   : U16 image's THRESH operation.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data. Only the U16 input format is supported.
 * @param pstDst: Output result.
 * @param pstThrU16Ctrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Thresh_U16(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                          IVE_DST_IMAGE_S *pstDst,
                          IVE_THRESH_U16_CTRL_S *pstThrU16Ctrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_16BitTo8Bit
 * Parameters   : Scale the input 16bit data to the output 8bit data.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.Only U16C1\S16C1 input is supported.
 * @param pstDst: Output result.
 * @param pst16BitTo8BitCtrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_16BitTo8Bit(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                           IVE_DST_IMAGE_S *pstDst,
                           IVE_16BIT_TO_8BIT_CTRL_S *pst16BitTo8BitCtrl,
                           bool bInstant);

/**
 * @brief
 * Prototype    : IVE_8BitTo8Bit
 * Parameters   : Scale the input 8bit data to the output 8bit data.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.Only U8C1\S8C1 input is supported.
 * @param pstDst: Output result.
 * @param pst8BitTo8BitCtrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_8BitTo8Bit(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                              IVE_DST_IMAGE_S *pstDst,
                              IVE_8BIT_TO_8BIT_CTRL_S *pst8BitTo8BitCtrl,
                              bool bInstant);

/**
 * @brief
 * Prototype    : IVE_OrdStatFilter
 * Parameters   : Order Statistic Filter. It can be used as median\max\min value filter.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data. Only U8C1 input is supported
 * @param pstDst: Output result.
 * @param pstOrdStatFltCtrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_OrdStatFilter(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                             IVE_DST_IMAGE_S *pstDst,
                             IVE_ORD_STAT_FILTER_CTRL_S *pstOrdStatFltCtrl,
                             bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Map
 * Parameters   : Map a image to another through a lookup table..
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data. Only U8C1 input is supported
 * @param pstMap: Input lookup table. Must be an U8 array of size 256.
 * @param pstDst: Output result.
 * @param pstMapCtrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Map(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                   IVE_SRC_MEM_INFO_S *pstMap, IVE_DST_IMAGE_S *pstDst,
                   IVE_MAP_CTRL_S *pstMapCtrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_EqualizeHist
 * Parameters   : Enhance the input image's contrast through histogram equalization.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data. Only U8C1 input is supported
 * @param pstDst: Output result.
 * @param pstEqualizeHistCtrl: Control parameters
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_EqualizeHist(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                            IVE_DST_IMAGE_S *pstDst,
                            IVE_EQHIST_CTRL_S *pstEqualizeHistCtrl,
                            bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Add
 * Parameters   : Two gray images' Add operation.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc1: Augend of the input source.Only the U8C1 input format is supported.
 * @param pstSrc2: Augend of the input source.Only the U8C1 input format is supported.
 * @param pstDst: Output result
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Add(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc1,
                   IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst,
                   IVE_ADD_CTRL_S *pstAddCtrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_Xor
 * Parameters   : Two binary images' Xor operation.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc1: The input source1. Only the U8C1 input format is supported.
 * @param pstSrc2: The input source2.
 * @param pstDst: Output result
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Xor(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc1,
                   IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstDst,
                   bool bInstant);

/**
 * @brief
 * Prototype    : IVE_NCC
 * Parameters   : Calculate two gray images' NCC (Normalized Cross Correlation).
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc1: The input source1. Only the U8C1 input format is supported.
 * @param pstSrc2: Input source2. Must be of the same type, size of source1.
 * @param pstDst: Output result
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_NCC(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc1,
                   IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_MEM_INFO_S *pstDst,
                   bool bInstant);

/**
 * @brief
 * Prototype    : IVE_CCL
 * Description  : Connected Component Labeling. Only 8-Connected method is supported.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrcDst: Input source
 * @param pstBlob: Output result of detected region;
 * @param pstCclCtrl: CCL control parameter
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_CCL(IVE_HANDLE *pHandle, IVE_IMAGE_S *pstSrcDst,
                   IVE_DST_MEM_INFO_S *pstBlob, IVE_CCL_CTRL_S *pstCclCtrl,
                   bool bInstant);

/**
 * @brief
 * Prototype    : IVE_GMM
 * Description  : Separate foreground and background using GMM(Gaussian Mixture Model) method;
 *                Gray or RGB GMM are supported.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source. Only support U8C1 or U8C3_PACKAGE input.
 * @param pstFg: Output foreground (Binary) image.
 * @param pstBg: Output background image. Of the sampe type of pstSrc.
 * @param pstMatchModelInfo: Output U8C1 match model info image. Low-1bit is match flag,and high-7bits is max freq index.
 * @param pstModel: Model data.
 * @param pstGmmCtrl: Control parameter
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_GMM(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                      IVE_DST_IMAGE_S *pstFg, IVE_DST_IMAGE_S *pstBg,
                      IVE_DST_IMAGE_S *pstMatchModelInfo,
                      IVE_MEM_INFO_S *pstModel, IVE_GMM_CTRL_S *pstGmmCtrl,
                      bool bInstant);

/**
 * @brief
 * Prototype    : IVE_GMM2
 * Description  : Separate foreground and background using GMM(Gaussian Mixture Model) method;
 *                Gray or RGB GMM are supported.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source. Only support U8C1 or U8C3_PACKAGE input.
 * @param pstFactor: U16C1 input, low-8bits is sensitivity factor, and high-8bits is life update factor.
 * @param pstFg: Output foreground (Binary) image.
 * @param pstBg: Output background image. Of the sampe type of pstSrc.
 * @param pstModel: Model data.
 * @param pstGmm2Ctrl: Control parameter
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_GMM2(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                    IVE_SRC_IMAGE_S *pstFactor, IVE_DST_IMAGE_S *pstFg,
                    IVE_DST_IMAGE_S *pstBg, IVE_DST_IMAGE_S *pstMatchModelInfo,
                    IVE_MEM_INFO_S *pstModel, IVE_GMM2_CTRL_S *pstGmm2Ctrl,
                    bool bInstant);

/**
 * @brief
 * Prototype    : IVE_CannyHysEdge
 * Description  : Canny Edge detection.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: input source. Only the U8C1 input format is supported
 * @param pstEdge: Output result.
 * @param pstStack: OutPut stack for CannyEdge
 * @param pstCannyEdgeCtrl: Control parameter
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_CannyEdge(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                            IVE_DST_IMAGE_S *pstEdge,
                            IVE_DST_MEM_INFO_S *pstStack,
                            IVE_CANNY_EDGE_CTRL_S *pstCannyEdgeCtrl,
                            bool bInstant);

/**
 * @brief
 * Prototype    : IVE_LBP
 * Description  : LBP calculation using the original method and a extensional method.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data.Only the U8C1 input format is supported.
 * @param pstDst: Output result
 * @param pstLbpCtrl: Control parameters.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_LBP(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                   IVE_DST_IMAGE_S *pstDst, IVE_LBP_CTRL_S *pstLbpCtrl,
                   bool bInstant);

/**
 * @brief
 * Prototype    : IVE_NormGrad
 * Description  : Gradient calculation and the output is normalized to S8.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source data
 * @param pstDstH: The (horizontal) result of input image filtered by the input mask;
 * @param pstDstV: The (vertical) result of input image filtered by the input mask;
 * @param pstDstHV: Output the horizontal and vertical component in single image in package format.
 * @param pstNormGradCtrl: Control parameters.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_NormGrad(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                        IVE_DST_IMAGE_S *pstDstH, IVE_DST_IMAGE_S *pstDstV,
                        IVE_DST_IMAGE_S *pstDstHV,
                        IVE_NORM_GRAD_CTRL_S *pstNormGradCtrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_LKOpticalFlowPyr
 * Description  : Calculate LK Optical Flow using multi-layer of the pyramid-images.
 * @param pHandle: Returned handle ID of a task
 * @param astSrcPrevPyr: Prev-frame's pyramid. Must be U8C1 images.
 * @param astSrcNextPyr: Next-frame's pyramid. Same size and type with astSrcPrePyr.
 * @param pstPrevPts: Intresting points on astSrcPrePyr[0].
 * @param pstNextPts: Output points. When bUseInitFlow is true, must have the same
 * 					  size of pstPrevPts as input.
 * @param pstStatus: pointer of status.
 * @param pstErr: pointer of error.
 * @param pstLkOptiFlowCtrl: Control parameters.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_LKOpticalFlowPyr(
    IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S astSrcPrevPyr[],
    IVE_SRC_IMAGE_S astSrcNextPyr[], IVE_SRC_MEM_INFO_S *pstPrevPts,
    IVE_MEM_INFO_S *pstNextPts, IVE_DST_MEM_INFO_S *pstStatus,
    IVE_DST_MEM_INFO_S *pstErr,
    IVE_LK_OPTICAL_FLOW_PYR_CTRL_S *pstLkOptiFlowPyrCtrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_LKOpticalFlow
 * Description  : Calculate LK Optical Flow using multi-layer of the pyramid-images.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrcPre: Prev-frame. Must be U8C1 images.
 * @param pstSrcCur: Next-frame. Same size and type with astSrcPrePyr.
 * @param pstPoint: Intresting points on pstSrcPre.
 * @param pstMv: Output motion vector.
 * @param pstLkOptiFlowCtrl: Control parameters.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_LKOpticalFlow(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrcPre,
                             IVE_SRC_IMAGE_S *pstSrcCur,
                             IVE_SRC_MEM_INFO_S *pstPoint,
                             IVE_SRC_MEM_INFO_S *pstMv,
                             IVE_LK_OPTICAL_FLOW_CTRL_S *pstLkOptiFlowCtrl,
                             bool bInstant);

/**
 * @brief
 * Prototype    : IVE_STCandiCorner
 * Description  : The first part of corners detection using Shi-Tomasi-like method: calculate candidate corners.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source image
 * @param pstCandiCorner: Output result of eig
 * @param pstStCandiCornerCtrl: Control parameters.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_STCandiCorner(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                             IVE_DST_MEM_INFO_S *pstCandiCorner,
                             IVE_ST_CANDI_CORNER_CTRL_S *pstStCandiCornerCtrl,
                             bool bInstant);

/**
 * @brief
 * Prototype    : IVE_STCorner
 * Description  : The second part of corners detection using Shi-Tomasi-like method: select corners by certain rules.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: Input source image
 * @param pstCandiCorner: Input source data
 * @param pstCorner: Output result of Corner
 * @param pstStCornerCtrl: Control parameters.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_STCorner(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                        IVE_DST_MEM_INFO_S *pstCandiCorner,
                        IVE_DST_MEM_INFO_S *pstCorner,
                        IVE_ST_CORNER_CTRL_S *pstStCornerCtrl, bool bInstant);

/**
 * @brief
 * Prototype    : IVE_MatchBgModel
 * Description  :
 * @param pHandle: Returned handle ID of a task
 * @param pstCurImg: Current grayscale image
 * @param pstBgModel: Background model data
 * @param pstMatchBgModelCtrl: Control parameters.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_MatchBgModel(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstCurImg,
                               IVE_DATA_S *pstBgModel,
                               IVE_MATCH_BG_MODEL_CTRL_S *pstMatchBgModelCtrl,
                               bool bInstant);

/**
 * @brief
 * Prototype    : IVE_UpdateBgModel
 * Description  :
 * @param pHandle: Returned handle ID of a task
 * @param pstCurImg: Current grayscale image
 * @param pstBgModel: Background model data
 * @param pstFgFlag               Foreground status image
 * @param pstBgImg                Background grayscale image
 * @param pstUpdateBgModelCtrl: Control parameters.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_UpdateBgModel(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstCurImg,
                                IVE_DATA_S *pstBgModel, IVE_IMAGE_S *pstFgFlag,
                                IVE_DST_IMAGE_S *pstBgImg,
                                IVE_UPDATE_BG_MODEL_CTRL_S *pstUpdateBgModelCtrl,
                                bool bInstant);

/**
 * @brief
 * Prototype    : IVE_SAD
 * Parameters   : Sum of absolute differences.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc1: The input source1. Only U8C1 input format is supported.
 * @param pstSrc2: The input source2. Only U8C1 input format is supported.
 * @param pstSad: Output result of sad value.Only the U8C1/U16C1 format is supported.
 * @param pstThr: Output result of thresh.Only the U8C1 format is supported.
 * @param pstSadCtrl: Control parameter.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_SAD(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc1,
                   IVE_SRC_IMAGE_S *pstSrc2, IVE_DST_IMAGE_S *pstSad,
                   IVE_DST_IMAGE_S *pstThr, IVE_SAD_CTRL_S *pstSadCtrl,
                   bool bInstant);

/**
 * @brief
 * Prototype    : RK_MPI_IVE_Warp_Affine_Init
 * Description  : initialize staging memory of warp affine.
 * @param pstMem: initialize the staging memory.
 * @param u32Width: Width of input image.
 * @param u32Height: Height of input image.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Warp_Affine_Init(IVE_MEM_INFO_S *pstMem, RK_U32 u32Width, RK_U32 u32Height);

/**
 * @brief
 * Prototype    : RK_MPI_IVE_Warp_Affine
 * Description  : Warp affine.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: The input source, Only U8C1 input format is supported.
 * @param pstDst: Output result of warp affine result.
 * @param pstWarpAffineCtrl: Control parameter.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Warp_Affine(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                           IVE_DST_IMAGE_S *pstDst,
                           IVE_WARP_AFFINE_CTRL_S *pstWarpAffineCtrl,
                           bool bInstant);

/**
 * @brief
 * Prototype    : RK_MPI_IVE_Pyramid_GetSize
 * Description  : Get the size of pyramid staging memory.
 * @param u32Width: Width of input image.
 * @param u32Height: Height of input image.
 * @param pstPyramidCtrl: Control parameter.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Pyramid_GetSize(RK_U32 u32Width, RK_U32 u32Height,
                           IVE_PYRAMID_CTRL_S *pstPyramidCtrl);

/**
 * @brief
 * Prototype    : RK_MPI_IVE_Pyramid_Create
 * Description  : Create pyramid image.
 * @param pHandle: Returned handle ID of a task
 * @param pstSrc: The input source, Only U8C1 input format is supported.
 * @param pstPyramid: The output result of pyramid image.
 * @param pstPyramidCtrl: Control parameter.
 * @param bInstant: set bInstant to TRUE blocks the next operation.
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Pyramid_Create(IVE_HANDLE *pHandle, IVE_SRC_IMAGE_S *pstSrc,
                           IVE_DST_IMAGE_S pstPyramid[],
                           IVE_PYRAMID_CTRL_S *pstPyramidCtrl,
                           bool bInstant);
/**
 * @brief
 * Prototype    : IVE_Query
 * Parameters   : This API is used to query the status of a called function by using the returned IveHandle of the function.
 *                In block mode, the system waits until the function that is being queried is called.
 *                In non-block mode, the current status is queried and no action is taken.
 * @param IveHandle: IveHandle of a called function. It is entered by users.
 * @param pbFinish: Returned status
 * @param bBlock: Flag indicating the block mode or non-block mode
 * @return RK_S32: RVE_OK: Success.
 *                 Error codes: Failure.
 */
RK_S32 RK_MPI_IVE_Query(IVE_HANDLE pHandle, bool *pbFinish, bool bBlock);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
#endif/*__IVE_H__*/
