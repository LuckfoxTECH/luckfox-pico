#ifndef _RK_AIQ_ASD_ALGOS_H_
#define _RK_AIQ_ASD_ALGOS_H_

#include "rk_aiq_comm.h"

typedef struct {
    unsigned char valid;
    unsigned int pos;
    unsigned int hsize;
    unsigned int vsize;
} asd_target_info_t;

typedef struct {
    unsigned char asd_scene_changed;
    asd_target_info_t asd_target_info;
    uint8_t cpsl_on;
    bool sals; /* strong area light scene */
    bool spls; /* strong point light scene */
    bool nrobj; /* near object scene, mean macro lens */
    bool sflr; /* strong flashlight reflection */
} asd_preprocess_result_t;

typedef struct {
    int cpsl_mode;
    uint8_t cpsl_on;
    float cpsl_sensitivity;
    uint32_t cpsl_sw_interval;
} asd_preprocess_in_t;


#endif
