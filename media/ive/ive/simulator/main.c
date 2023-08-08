#include <stdio.h>
#include <string.h>
#include <rk_mpi_ive.h>

#include "rve_tests.h"

void common_usage(void) {
    printf(
        "\n[RVE test suite]\n"
        "\nusage:\n"
        "\nrve-unit-test [module]\n"
        "\noutput result to ../output/rve_xxx_out.yuv\n"
        "\nmodules:\n"
        "\n  gmm, matchupdatebg, canny, ccl\n"
        "\n  stcorner, lkopticalflow, integral\n"
        "\n  lbp, filter, sobel, dma, csc, eqhist,\n"
        "\n  logicop, magandang, map, morph, ncc\n"
        "\n  cast, sad, threshold, median\n"
        "\n"
        "\nROCKCHIP Electronics Co.,Ltd.\n");
}

enum RVE_MODULE_ID {
    RVE_GMM_ID = 0,
    RVE_BGM_ID,
    RVE_CANNY_ID,
    RVE_CCL_ID,
    RVE_STCORNER_ID,
    RVE_LKOPTICALFLOW_ID,
    RVE_INTEGRAL_ID,
    RVE_LBP_ID,
    RVE_FILTER_ID,
    RVE_SOBEL_ID,
    RVE_DMA_ID,
    RVE_CSC_ID,
    RVE_EQHIST_ID,
    RVE_LOGICOP_ID,
    RVE_MAGANDANG_ID,
    RVE_MAP_ID,
    RVE_MORPH_ID,
    RVE_NCC_ID,
    RVE_CAST_ID,
    RVE_SAD_ID,
    RVE_THRESHOLD_ID,
    RVE_MEDIAN_ID,

    RVE_MODULE_ID_CNT,
};

char *rve_entity_name[] = {"gmm",       "matchupdatebg", "canny",     "ccl",
                            "stcorner",  "lkopticalflow", "integral",  "lbp",
                            "filter",    "sobel",         "dma",       "csc",
                            "eqhist",    "logicop",       "magandang", "map",
                            "morph",     "ncc",           "cast",      "sad",
                            "threshold", "median"};

const char *rve_tests_get_entity_name(int id) {
    if (id >= RVE_MODULE_ID_CNT) {
        return NULL;
    }

    return rve_entity_name[id];
}

int rve_tests_get_entity_id(const char *modulename) {
    int i, module_id = -1;

    for (i = 0; i < RVE_MODULE_ID_CNT; i++) {
        if (!strcmp(modulename, rve_tests_get_entity_name(i))) {
            module_id = i;
            break;
        }
    }

    return module_id;
}

int main(int argc, const char **argv) {
    int id;

    RK_U32 u32Width = 352;
    RK_U32 u32Height = 288;
    RK_CHAR *videoFileName = "../data/stream/00_352x288.avi";
    RK_CHAR *streamFileName = "../data/stream/02_352x288_f100.yuv";
    RK_CHAR *fileName = "../data/y/00_352x288_y.yuv";
    RK_CHAR *yuvFileName = "../data/420sp/00_352x288_420sp.yuv";
    RK_CHAR *threshFileName = "../data/y/bin_352x288_y.yuv";
    RK_CHAR *fileName1 = "../data/y/01_352x288_y.yuv";
    RK_CHAR *fileName2 = "../data/y/02_352x288_y.yuv";
    RK_CHAR *outDir = "../output";

    if (argc < 2) {
        common_usage();
        return 0;
    }

    const char *entity_name = argv[1];

    id = rve_tests_get_entity_id(entity_name);

    if (id == -1) {
        printf("rve-test-suite: unknown module name: %s\n", entity_name);
        common_usage();
        return 0;
    }

    printf("%s: show %s(%d) sample\n", argv[0], entity_name, id);

    switch (id) {
        case RVE_GMM_ID: {
            GMM2_Sample_U8C1_PixelCtrl(entity_name, videoFileName, outDir, u32Width, u32Height);
        } break;
        case RVE_BGM_ID: {
            BgModelSample1(entity_name, videoFileName, outDir, u32Width, u32Height);
        } break;
        case RVE_CANNY_ID: {
            CannySample3x3(entity_name, fileName, outDir, u32Width, u32Height);
        } break;
        case RVE_CCL_ID: {
            CCL_Sample01(entity_name, threshFileName, outDir, u32Width, u32Height);
        } break;
        case RVE_STCORNER_ID: {
            ShitomasiSample(entity_name, fileName, outDir, u32Width, u32Height);
        } break;
        case RVE_LKOPTICALFLOW_ID: {
            LKOpticalFlowPyrSample(entity_name, videoFileName, outDir,
                                   u32Width,
                                   u32Height);
        } break;
        case RVE_INTEGRAL_ID: {
            IntegSampleCombine(entity_name, fileName, outDir, u32Width,
                               u32Height);
            IntegSampleSum(entity_name, fileName, outDir, u32Width, u32Height);
            IntegSampleSqsum(entity_name, fileName, outDir, u32Width,
                             u32Height);
        } break;
        case RVE_LBP_ID: {
            LBPSampleNormal(entity_name, fileName, outDir, u32Width, u32Height);
            LBPSampleABS(entity_name, fileName, outDir, u32Width, u32Height);
        } break;
        case RVE_FILTER_ID: {
            FilterSampleY5x5(entity_name, fileName, outDir, u32Width, u32Height);
        } break;
        case RVE_SOBEL_ID: {
            SobelSampleBoth3x3(entity_name, fileName, outDir, u32Width,
                               u32Height);
        } break;
        case RVE_DMA_ID: {
            DMASampleDirectCopy(entity_name, fileName, outDir, u32Width,
                                u32Height);
            DMASampleIntervalCopy(entity_name, fileName, outDir, u32Width,
                                  u32Height);
            DMASampleSet3Byte(entity_name, fileName, outDir, u32Width,
                              u32Height);
            DMASampleSet8Byte(entity_name, fileName, outDir, u32Width,
                              u32Height);
        } break;
        case RVE_CSC_ID: {
            CSCSampleVideoBT601YUV2RGB(entity_name, yuvFileName, outDir, u32Width,
                                       u32Height);
        } break;
        case RVE_EQHIST_ID: {
            HistSample(entity_name, fileName, outDir, u32Width, u32Height);
            EQHistSample(entity_name, fileName, outDir, u32Width, u32Height);
        } break;
        case RVE_LOGICOP_ID: {
            AddSample(entity_name, fileName1, fileName2, outDir, u32Width,
                      u32Height);
            AndSample(entity_name, fileName1, fileName2, outDir, u32Width,
                      u32Height);
            OrSample(entity_name, fileName1, fileName2, outDir, u32Width,
                     u32Height);
            SubSampleABS(entity_name, fileName1, fileName2, outDir, u32Width,
                         u32Height);
            SubSampleSHIFT(entity_name, fileName1, fileName2, outDir, u32Width,
                           u32Height);
            XorSample(entity_name, fileName1, fileName2, outDir, u32Width,
                      u32Height);
        } break;
        case RVE_MAGANDANG_ID: {
            MagAndAngSampleMagAndAng5x5(entity_name, fileName, outDir, u32Width,
                                        u32Height);
        } break;
        case RVE_MAP_ID: {
            MapSample(entity_name, fileName, outDir, u32Width, u32Height);
        } break;
        case RVE_MORPH_ID: {
            DilateSample5x5(entity_name, fileName, outDir, u32Width, u32Height);

            ErodeSample5x5(entity_name, fileName, outDir, u32Width, u32Height);
        } break;
        case RVE_NCC_ID: {
            NCCSample(entity_name, fileName1, fileName2, outDir, u32Width,
                      u32Height);
        } break;
        case RVE_CAST_ID: {
            CastSample(entity_name, fileName, outDir, u32Width, u32Height);
        } break;
        case RVE_SAD_ID: {
            SADSample(entity_name, fileName1, fileName2, outDir, u32Width,
                      u32Height);
        } break;
        case RVE_THRESHOLD_ID: {
            ThreshSampleBinary(entity_name, fileName, outDir, u32Width,
                               u32Height);
            ThreshSampleTrunc(entity_name, fileName, outDir, u32Width,
                              u32Height);
            ThreshSampleToMinval(entity_name, fileName, outDir, u32Width,
                                 u32Height);
            ThreshSampleMinMidMax(entity_name, fileName, outDir, u32Width,
                                  u32Height);
            ThreshSampleOriMidMax(entity_name, fileName, outDir, u32Width,
                                  u32Height);
            ThreshSampleMinMidOri(entity_name, fileName, outDir, u32Width,
                                  u32Height);
            ThreshSampleMinOriMax(entity_name, fileName, outDir, u32Width,
                                  u32Height);
            ThreshSampleOriMinOri(entity_name, fileName, outDir, u32Width,
                                  u32Height);
        } break;
        case RVE_MEDIAN_ID: {
            OrdStaFilterSampleMeidan(entity_name, fileName, outDir, u32Width,
                                     u32Height);
            OrdStaFilterSampleMax(entity_name, fileName, outDir, u32Width,
                                  u32Height);
            OrdStaFilterSampleMin(entity_name, fileName, outDir, u32Width,
                                  u32Height);
        } break;
        default: {
            printf("rve-test-suite: unknown module\n");
            break;
        }
    }

    return 0;
}