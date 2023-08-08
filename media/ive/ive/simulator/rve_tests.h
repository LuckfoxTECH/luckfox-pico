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
#ifndef RVE_TESTS_H_
#define RVE_TESTS_H_
#include "rk_type.h"
#include "sample_gfx.h"

RK_VOID AddSample(const RK_CHAR *moduleName, RK_CHAR *fileName1, RK_CHAR *fileName2,
                  RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID AndSample(const RK_CHAR *moduleName, RK_CHAR *fileName1, RK_CHAR *fileName2,
                  RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID BgModelSample1(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height);
RK_VOID CannySample3x3(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height);
RK_VOID CannySample5x5(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height);
RK_VOID CastSample(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                   RK_U32 u32Width, RK_U32 u32Height);
RK_VOID CCL_Sample01(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                     RK_U32 u32Width, RK_U32 u32Height);
RK_VOID CSCSampleVideoBT601YUV2RGB(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                   RK_CHAR *outDir, RK_U32 u32Width,
                                   RK_U32 u32Height);
RK_VOID CSCSampleVideoBT601RGB2YUV(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                   RK_CHAR *outDir, RK_U32 u32Width,
                                   RK_U32 u32Height);
RK_VOID DilateSample3x3(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height);
RK_VOID DilateSample5x5(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height);
RK_VOID DMASampleDirectCopy(const RK_CHAR *moduleName, RK_CHAR *fileName,
                            RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID DMASampleIntervalCopy(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height);
RK_VOID DMASampleSet3Byte(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID DMASampleSet8Byte(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID ErodeSample3x3(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height);
RK_VOID ErodeSample5x5(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height);
RK_VOID FilterSampleY5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                         RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID GMM_Sample_U8C1(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height);
RK_VOID GMM2_Sample_U8C1_PixelCtrl(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height);
RK_VOID HistSample(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                   RK_U32 u32Width, RK_U32 u32Height);
RK_VOID EQHistSample(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                     RK_U32 u32Width, RK_U32 u32Height);
RK_VOID IntegSampleCombine(const RK_CHAR *moduleName, RK_CHAR *fileName,
                           RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID IntegSampleSum(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                       RK_U32 u32Width, RK_U32 u32Height);
RK_VOID IntegSampleSqsum(const RK_CHAR *moduleName, RK_CHAR *fileName,
                         RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID LBPSampleNormal(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height);
RK_VOID LBPSampleABS(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                     RK_U32 u32Width, RK_U32 u32Height);
RK_VOID LKOpticalFlowPyrSample(const RK_CHAR *moduleName, RK_CHAR *fileName,
                               RK_CHAR *outDir, RK_U32 u32Width,
                               RK_U32 u32Height);
RK_VOID MagAndAngSampleMagAndAng3x3(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                    RK_CHAR *outDir, RK_U32 u32Width,
                                    RK_U32 u32Height);
RK_VOID MagAndAngSampleMagAndAng5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                    RK_CHAR *outDir, RK_U32 u32Width,
                                    RK_U32 u32Height);
RK_VOID MapSample(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                  RK_U32 u32Width, RK_U32 u32Height);
RK_VOID NCCSample(const RK_CHAR *moduleName, RK_CHAR *fileName1, RK_CHAR *fileName2,
                  RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID OrSample(const RK_CHAR *moduleName, RK_CHAR *fileName1, RK_CHAR *fileName2,
                 RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID OrdStaFilterSampleMeidan(const RK_CHAR *moduleName, RK_CHAR *fileName,
                                 RK_CHAR *outDir, RK_U32 u32Width,
                                 RK_U32 u32Height);
RK_VOID OrdStaFilterSampleMax(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height);
RK_VOID OrdStaFilterSampleMin(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height);
RK_VOID SADSample(const RK_CHAR *moduleName, RK_CHAR *fileName1, RK_CHAR *fileName2,
                  RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID SobelSampleBoth3x3(const RK_CHAR *moduleName, RK_CHAR *fileName,
                           RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID SobelSampleHor3x3(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID SobelSampleHor5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID SobelSampleVer3x3(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID SobelSampleVer5x5(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID ShitomasiSample(const RK_CHAR *moduleName, RK_CHAR *fileName, RK_CHAR *outDir,
                        RK_U32 u32Width, RK_U32 u32Height);
RK_VOID SubSampleABS(const RK_CHAR *moduleName, RK_CHAR *fileName1,
                     RK_CHAR *fileName2, RK_CHAR *outDir, RK_U32 u32Width,
                     RK_U32 u32Height);
RK_VOID SubSampleSHIFT(const RK_CHAR *moduleName, RK_CHAR *fileName1,
                       RK_CHAR *fileName2, RK_CHAR *outDir, RK_U32 u32Width,
                       RK_U32 u32Height);
RK_VOID ThreshSampleBinary(const RK_CHAR *moduleName, RK_CHAR *fileName,
                           RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID ThreshSampleTrunc(const RK_CHAR *moduleName, RK_CHAR *fileName,
                          RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);
RK_VOID ThreshSampleToMinval(const RK_CHAR *moduleName, RK_CHAR *fileName,
                             RK_CHAR *outDir, RK_U32 u32Width,
                             RK_U32 u32Height);
RK_VOID ThreshSampleMinMidMax(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height);
RK_VOID ThreshSampleOriMidMax(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height);
RK_VOID ThreshSampleMinMidOri(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height);
RK_VOID ThreshSampleMinOriMax(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height);
RK_VOID ThreshSampleOriMinOri(const RK_CHAR *moduleName, RK_CHAR *fileName,
                              RK_CHAR *outDir, RK_U32 u32Width,
                              RK_U32 u32Height);
RK_VOID XorSample(const RK_CHAR *moduleName, RK_CHAR *fileName1, RK_CHAR *fileName2,
                  RK_CHAR *outDir, RK_U32 u32Width, RK_U32 u32Height);

#endif