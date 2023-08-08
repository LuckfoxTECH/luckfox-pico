#ifndef __RK_AIQ_ORB_HW_H__
#define __RK_AIQ_ORB_HW_H__

#include "aorb/orb.h"

typedef struct {
    unsigned int num_points;
    unsigned short pXs[MAX_POINTS];
    unsigned short pYs[MAX_POINTS];
    unsigned char pDescriptors[MAX_POINTS * DESCRIPTOR_SIZE];
    unsigned char pScores;
} sim_orb_stat_t;

typedef struct {
    unsigned char valid;
    double homographyMat[9];
} sim_orb_preprocess_result_t;

#endif
