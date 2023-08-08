
#ifndef __RKAIQ_TYPES_ALGO_AGIC_H__
#define __RKAIQ_TYPES_ALGO_AGIC_H__

typedef struct AgicConfig_s {
    unsigned char gic_en;
    unsigned char edge_open;

    unsigned short regmingradthrdark2;
    unsigned short regmingradthrdark1;
    unsigned short regminbusythre;

    unsigned short regdarkthre;
    unsigned short regmaxcorvboth;
    unsigned short regdarktthrehi;

    unsigned char regkgrad2dark;
    unsigned char regkgrad1dark;
    //unsigned char regstrengthglobal_fix;//
    unsigned char regdarkthrestep;
    unsigned char regkgrad2;
    unsigned char regkgrad1;
    unsigned char reggbthre;

    unsigned short regmaxcorv;
    unsigned short regmingradthr2;
    unsigned short regmingradthr1;

    unsigned char gr_ratio;
    float dnloscale;
    float dnhiscale;
    unsigned char reglumapointsstep;

    float gvaluelimitlo;
    float gvaluelimithi;
    float fusionratiohilimt1;

    //unsigned char regstrength_fix;

    float sigma_y[15];

    unsigned char noise_cut_en;
    unsigned short noise_coe_a;

    unsigned short noise_coe_b;
    unsigned short diff_clip;
    //
    unsigned short smoothness_gb;
    unsigned short smoothness_gb_weak;
    unsigned short max_cut_v;
    float textureStrength;
    float globalStrength;
    float noiseCurve_0 ;
    float noiseCurve_1 ;
    unsigned short lumaPoints[15];
} AgicConfig_t;

typedef struct AgicProcResult_s {
    AgicConfig_t config;
} AgicProcResult_t;

#endif//__RKAIQ_TYPES_ALGO_AGIC_H__