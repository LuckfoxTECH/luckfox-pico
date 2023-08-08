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

#ifndef __RKGENMESHVERSION_H__
 /*!
  * ==================== GENMESH VERSION HISTORY ====================
  *
  * v1.0.0
  *  - date: from 2020-09-11 to 2020-09-14
  *  - FEC & LDCH: Keep the max FOV in the horizontal(X direction)
  *  - FEC: 1) Add "correctX" and "correctY" flag:
			   a) correctX = 1, correctY = 1: correct both directions
			   b) correctX = 1, correctY = 0: correct x direction
			   c) correctX = 0, correctY = 1: correct y direction
			   d) correctX = 0, correctY = 0: no correct
			2) Add "saveMesh4bin" flag: whether to save 4 mesh bins(meshxi,meshxf,meshyi,meshyf) or not
			   a) saveMesh4bin = 1: save
			   b) saveMesh4bin = 0: not save
			3) Add "mesh4binPath" : set the path for saving 4 mesh bins, for example, mesh4binPath = "../data_out/"
			   when 4 mesh bins saved, the prefix file names of each group of bins are
			   a) level255_both_correct_: correct both directions
			   b) level255_x_correct_: correct x direction
			   c) level255_y_correct_: correct y direction
			   d) level255_no_correct_: no correct
  *  - LDCH: 1) Add judgment: calculate the max correction capability("maxLevel") of LDCH, and divide it into 255 levels.
                so the value of level can still vary from 0 to 255 when the function is called.
             2) Add "saveMeshX" flag: whether to save mesh bin or not
			   a) saveMeshX = 1: save
			   b) saveMeshX = 0: not save
             3) Add "meshPath" : set the path for saving mesh bin, for example, meshPath = "../data_out/"
  
  * v1.0.1
  *  - date: 2020-09-15
  *  - LDCH: optimize the "calcLdchMaxLevel" function to reduce the cost time,
			 and it takes about 160ms in PC

  * v1.0.2
  *  - date: 2020-09-16
  *  - LDCH: continue to optimize the "calcLdchMaxLevel" function to reduce the cost time,
			 and it takes about 50ms in PC

  * v1.0.3
  *  - date: 2020-09-17
  *  - LDCH: continue to optimize the "calcLdchMaxLevel" function to reduce the cost time,
			 and it takes about 8ms in PC

  * v2.0.0
  *  - date: 2020-09-17
  *  - FEC&LDCH: change to use rho-cotTheta(or rho-tanTheta) in polynomial fitting,
                 and it saves some time in function "genFECMeshNLevel" and "genLDCMeshNLevel" for each level
  *  - LDCH: continue to optimize the "calcLdchMaxLevel" function to reduce the cost time,
			 and it only takes 4ms in PC

  * v2.0.1
  *  - date: 2020-09-18
  *  - FEC&LDCH: due to a bug in polynomial fitting, last version will returns error when the I/O resolutions are different,
                 it has been solved in this version
	 - FEC: the fixed-point number is changed from 7 to 8

  * v2.0.2
  *  - date: 2020-09-23
  *  - FEC&LDCH: adjust some code to facilitate branch prediction logic and reduce unnecessary memory references to improve efficiency

  * v2.0.3
  *  - date: 2020-10-10
  *  - FEC&LDCH: fix some small bugs
  
  * v2.0.4
  *  - date: 2020-10-15
  *  - FEC: change mesh border from (srcW-3, srcH-3) to (srcW-1, srcH-1)

  * v2.0.5
  *  - date: 2020-10-28
  *  - FEC: consider the case when the width and height of the output image are different from the input,
			the naming rules of mesh table are specified as(using "meshxi, level=0, input=3840x2160, output=1920x1080, both x and y direction are corrected" as an example):
			level000_1920x1080from3840x2160_both_correct_meshxi.bin

  * v3.0.0
  *  - date: 2020-11-10
  *  - FEC: add an option: saveMaxFovX--flag of retaining maximum FOV in X direction(horizontal direction)
			so there are 6 options when combined with correctX and correctY flags:
			correctX	correctY	saveMaxFovX
			   1			0		 (0 or 1)
			   0			1		 (0 or 1)
			   1			1		 (0 or 1)
  *  - LDCH: add an option: saveMaxFovX
			 users can choose whether to retain the max FOV in X direction or not.
			 when saveMaxFovX=1, users choose to save max FOV in X direction and tolerate slight compression distortion in middle part.

  * v3.0.1
  *  - date: 2020-11-12
  *  - FEC & LDCH: add a Macro definition "INV_POLY_COEFF_NUM" to define the coefficients' number of polynomial fitting
  *  - FEC: the code was test in the case when the horizontal FOV is close to 180 degrees and the result is OK.

  * v3.0.2
  *  - date: 2020-11-27
  *  - FEC: add an option: isFecOld--flag for different versions of FEC hardware
			isFecOld = 1: for old FEC
			isFecOld = 0: for new FEC
 
 * v3.0.3
  *  - date: 2020-12-22
  *  - LDCH: add an option: isLdchOld--flag for different versions of LDCH hardware
			isLdchOld = 1: for old LDCH
			isLdchOld = 0: for new LDCH

  * v4.0.0
  *  - date: 2021-01-19
  *  - genScaleMeshNLevel:
			add a new function interface: LDCH mesh(for x direction) + FEC mesh(for y direction)
			1) CameraCoeff of original resolution, e.g. 3840*2160 --> LDCH mesh --> LDCH
			2) CameraCoeff of original resolution, e.g. 3840*2160 --> 1920*1080 FEC mesh + 1920*1080 image data --> FEC

  * v4.0.1
  *  - date: 2021-01-21
  *  - genCropScaleMeshNLevel:
			add a new function interface: LDCH mesh(for x direction) + FEC mesh(for y direction)
			1) CameraCoeff of original resolution, e.g. 3840*2160 --> LDCH mesh --> LDCH
			2) CameraCoeff of original resolution, e.g. 3840*2160 --> crop at any position(pW,pH) to get any size, e.g. 1600*900 --> scale to 1920*1080 FEC mesh + 1920*1080 image data --> FEC

  * v4.0.2
  *  - date: 2021-06-23
  *  - LDCH: add genLdchMeshFixedZeroCorrect function
  
  * v4.0.3
  *  - date: 2021-06-28
  *  - LDCH: modify the last row of zero correct mesh for LDCH bug.

  * v4.0.4
  *  - date: 2021-07-05
  *  - LDCH: modify the middle row of mesh(minus 6 pixels) when saveMaxFovX == 1 for old LDCH bug(avoid the last pixel in the row map to the interval of [last-5, last-3]).
			 valid flag: need to set isLdchOld = 1.
  
  * v4.0.5
  *  - date: 2021-08-12
  *  - LDCH: last version didnot completely solve this bug: the middle row of mesh(minus 6 pixels) when saveMaxFovX == 1 for old LDCH bug(avoid the last pixel in the row map to the interval of [last-5, last-3]).
			 valid flag: need to set isLdchOld = 1.
  
  * v4.0.6
  *  - date: 2021-08-14
  *  - LDCH: consider these case:
             1.rhoMaxW < rohMaxH for old LDCH bug(avoid the last pixel in the row map to the interval of [last-5, last-3]).
			 2.identity mesh between penultimate and last rows may cause old LDCH bug when interpolation.
			 valid flag: need to set isLdchOld = 1.

  * v5.0.0
  *  - date: 2021-09-23
  *  - FEC: add function cropFecMesh:
			the mesh wiil be cropped according to the ROI parameters of the output image.

  * v5.0.1
  *  - date: 2021-10-09
  *  - FEC: add function genFecMeshInit8kTo4k: generate FEC mesh for 8k image ---> 2 images of 4k.
  *  - LDCH: add function genLdchMeshInit8kTo4k: generate LDCH mesh for 8k image ---> 2 images of 4k.
	 - FEC & LDCH: some other changes.
*/

#define RK_GENMESH_VERSION_REAL_V "v5.0.1"
#define RK_GENMESH_RELEASE_DATE "2021-10-09"

  /******* DO NOT EDIT THE FOLLOWINGS *******/

#define RK_GENMESH_VERSION_HEAD "GENMESH "
#define RK_GENMESH_VERSION \
    RK_GENMESH_VERSION_HEAD\
    RK_GENMESH_VERSION_REAL_V

#endif // !__RKGENMESHVERSION_H__
