/*
 *  Copyright (c) 2020 Rockchip Corporation
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

#ifndef __GENMESH_H__
#define __GENMESH_H__

#define INV_POLY_COEFF_NUM 21								/* 多项式系数个数, 最高次数(INV_POLY_COEFF_NUM-1)次 */

 /* 相机参数 */
struct CameraCoeff
{
	double cx, cy;											/* 镜头的光心 */
	double a0, a2, a3, a4;									/* 镜头的畸变系数 */
	double c, d, e;											/* 内参[c d;e 1] */
	double sf;												/* sf控制视角, sf越大视角越大 */

	/* level = 0时的rho-tanTheta多项式拟合 */
	int invPolyTanNum0;										/* 拟合后的系数个数 */
	double invPolyTanCoeff0[INV_POLY_COEFF_NUM];		/* 多项式系数, 最高次数(INV_POLY_COEFF_NUM-1)次 */
	/* level = 0时的rho-cotTheta多项式拟合 */
	int invPolyCotNum0;										/* 拟合后的系数个数 */
	double invPolyCotCoeff0[INV_POLY_COEFF_NUM];		/* 多项式系数, 最高次数(INV_POLY_COEFF_NUM-1)次 */

	/* level = 255时的rho-tanTheta多项式拟合 */
	int invPolyTanNum255;									/* 拟合后的系数个数 */
	double invPolyTanCoeff255[INV_POLY_COEFF_NUM];		/* 多项式系数, 最高次数(INV_POLY_COEFF_NUM-1)次 */
	/* level = 255时的rho-cotTheta多项式拟合 */
	int invPolyCotNum255;									/* 拟合后的系数个数 */
	double invPolyCotCoeff255[INV_POLY_COEFF_NUM];		/* 多项式系数, 最高次数(INV_POLY_COEFF_NUM-1)次 */
};

/* 生成FEC映射表相关的参数 */
struct FecParams
{
	int correctX;										/* 水平x方向校正: 1代表校正, 0代表不校正 */
	int correctY;										/* 垂直y方向校正: 1代表校正, 0代表不校正 */
	int saveMaxFovX;									/* 保留水平x方向最大FOV: 1代表保留, 0代表不保留 */
	int isFecOld;										/* 是否旧版FEC: 1代表是，0代表不是 */
	int saveMesh4bin;									/* 是否保存meshxi,xf,yi,yf4个bin文件：1代表保存, 0代表不保存 */
	char mesh4binPath[256];								/* 保存meshxi,xf,yi,yf4个bin文件的路径 */
	int srcW, srcH, dstW, dstH;							/* 输入输出图像的分辨率 */
	int srcW_ex, srcH_ex, dstW_ex, dstH_ex;				/* 扩展后的输入输出分辨率 */
	double cropStepW[2000], cropStepH[2000];
	double cropStartW[2000], cropStartH[2000];
	int meshSizeW, meshSizeH;
	double meshStepW, meshStepH;
	int meshSize1bin;
	int meshSize4bin;
	unsigned short	SpbNum;
	unsigned long	MeshPointNumW;
	unsigned short	SpbMeshPNumH;
	unsigned short	LastSpbMeshPNumH;

	unsigned short	*pMeshXY;

};

/* 生成LDCH映射表相关的参数 */
struct LdchParams
{
	int saveMaxFovX;									/* 保留水平x方向最大FOV: 1代表保留, 0代表不保留 */
	int isLdchOld;										/* 是否旧版LDCH: 1代表是，0代表不是 */
	int saveMeshX;										/* 是否保存MeshX.bin文件: 1代表保存, 0代表不保存 */
	char meshPath[256];									/* 保存MeshX.bin文件的路径 */
	int srcW, srcH, dstW, dstH;							/* 输入输出图像的分辨率 */
	int meshSizeW, meshSizeH;
	double meshStepW, meshStepH;
	int mapxFixBit;										/* 定点化左移位数 */
	int meshSize;
	int maxLevel;
	double *mapx;
	double *mapy;
};


/* =============================================================================================================================================================================== */

/* FEC: 初始化, 根据图像输出分辨率, 计算FEC映射表的相关参数, 申请需要的buffer */
void genFecMeshInit(int srcW, int srcH, int dstW, int dstH, FecParams &fecParams, CameraCoeff &camCoeff);

/* FEC: 反初始化 */
void genFecMeshDeInit(FecParams &fecParams);

/* FEC: 预先计算的部分: 浮点未校正的小表和level=0,level=255的多项式参数 */
void genFecPreCalcPart(FecParams &fecParams, CameraCoeff &camCoeff);

/* FEC: 4个mesh 内存申请 */
void mallocFecMesh(int meshSize, unsigned short **pMeshXI, unsigned char **pMeshXF, unsigned short **pMeshYI, unsigned char **pMeshYF);

/* FEC: 4个mesh 内存释放 */
void freeFecMesh(unsigned short *pMeshXI, unsigned char *pMeshXF, unsigned short *pMeshYI, unsigned char *pMeshYF);

/*
函数功能: 生成不同校正程度的mesh映射表, 用于ISP的FEC模块
	输入:
	1）FEC映射表的相关参数, 申请需要的buffer: FecParams &fecParams
	2）相机标定参数: CameraCoeff &camCoeff
	3）需要校正的程度: level(0-255: 0表示校正程度为0%, 255表示校正程度为100%)
	输出:
	1）bool 是否成功生成
	2）pMeshXI, pMeshXF, pMeshYI, pMeshYF
*/
bool genFECMeshNLevel(FecParams &fecParams, CameraCoeff &camCoeff, int level, unsigned short *pMeshXI, unsigned char *pMeshXF, unsigned short *pMeshYI, unsigned char *pMeshYF);


/* =============================================================================================================================================================================== */

/* LDCH: 初始化, 根据图像输出分辨率, 计算LDCH映射表的相关参数, 申请需要的buffer */
void genLdchMeshInit(int srcW, int srcH, int dstW, int dstH, LdchParams &ldchParams, CameraCoeff &camCoeff);

/* LDCH: 反初始化 */
void genLdchMeshDeInit(LdchParams &ldchParams);

/* LDCH: 预先计算的部分: 浮点未校正的小表和level=0,level=255的多项式参数 */
void genLdchPreCalcPart(LdchParams &ldchParams, CameraCoeff &camCoeff);

/* LDCH: 计算LDCH能够校正的最大程度 */
void calcLdchMaxLevel(LdchParams &ldchParams, CameraCoeff &camCoeff);

/*
函数功能: 生成不同校正程度的mesh映射表, 用于ISP的LDCH模块

	输入:
	1）LDCH映射表的相关参数, 申请需要的buffer: LdchParams &ldchParams
	2）相机标定参数: CameraCoeff &camCoeff
	3）需要校正的程度: level(0-255: 0表示校正程度为0%, 255表示校正程度为100%)
	输出:
	1）bool 是否成功生成
	2）pMeshX
*/
bool genLDCMeshNLevel(LdchParams &ldchParams, CameraCoeff &camCoeff, int level, unsigned short *pMeshX);

/* =============================================================================================================================================================================== */

/* 输出图像的ROI区域相关参数 */
struct RoiParams
{
	int startW;		/* ROI区域的起始点 */
	int startH;
	int roiW;		/* ROI区域的宽高 */
	int roiH;
};

/* 根据输出图像的ROI参数，对FEC mesh进行裁剪, 得到最终需求的尺寸 */
bool cropFecMesh(FecParams &fecParams, RoiParams &roiParams, int level, unsigned short *pCropMeshXI, unsigned char *pCropMeshXF, unsigned short *pCropMeshYI, unsigned char *pCropMeshYF);

/* 根据输出图像的ROI参数，对LDCH mesh进行裁剪, 得到最终需求的尺寸 */
bool cropLdchMesh(LdchParams &ldchParams, RoiParams &roiParams, int level, unsigned short *pMeshX, unsigned short *pRoiMeshX);

/* FEC: 初始化, 用于8k ---> 2个4k */
void genFecMeshInit8kTo4k(int srcW, int srcH, int dstW, int dstH, int margin,
	CameraCoeff &camCoeff, CameraCoeff &camCoeff_left, CameraCoeff &camCoeff_right,
	FecParams &fecParams, FecParams &fecParams_left, FecParams &fecParams_right);

/* LDCH: 8k ---> 2个4k */
void genLdchMeshInit8kTo4k(int srcW, int srcH, int dstW, int dstH, int margin,
	CameraCoeff &camCoeff, CameraCoeff &camCoeff_left, CameraCoeff &camCoeff_right,
	LdchParams &ldchParams, LdchParams &ldchParams_left, LdchParams &ldchParams_right);

#endif // !__GENMESH_H__
