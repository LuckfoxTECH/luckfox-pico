#ifndef _RK_AIQ_ALGO_TYPES_INT_H_
#define _RK_AIQ_ALGO_TYPES_INT_H_

#if OPENCV_SUPPORT
int gen_default_mesh_table(int imgWidth, int imgHeight, int mesh_density,
                           unsigned short* pMeshXI, unsigned short* pMeshYI, unsigned char* pMeshXF, unsigned char* pMeshYF);
#endif
#endif
