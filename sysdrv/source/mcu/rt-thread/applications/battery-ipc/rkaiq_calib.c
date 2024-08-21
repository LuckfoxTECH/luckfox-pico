#include "rkaiq_calib.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct __map_index {
    void *dst_offset;
    void *ptr_offset;
    size_t len;
} map_index_t;

int rkaiq_parse_bin_map(uint8_t *data, size_t len) {
    size_t map_len = *(size_t *)(data + (len - sizeof(size_t)));
    size_t map_offset = *(size_t *)(data + (len - sizeof(size_t) * 2));
    size_t map_index = 0;
    map_index_t *map_addr = NULL;

    map_addr = (map_index_t *)(data + map_offset);

    for (map_index = 0; map_index < map_len; map_index++) {
        map_index_t tmap = (map_addr[map_index]);
        void **dst_obj_addr = (void **)(data + (size_t)tmap.dst_offset);
        *dst_obj_addr = data + (uintptr_t)tmap.ptr_offset;
    }

    return 0;
}

CamCalibDbProj_t *rkaiq_calib_load(void *offset, size_t len) {
    CamCalibDbProj_t *calibproj = NULL;

    if (!offset || len <= 0) {
        return NULL;
    }

    rkaiq_parse_bin_map((uint8_t *)offset, len);

    calibproj = (CamCalibDbProj_t *)offset;

    return calibproj;
}

int rkaiq_get_default_calib
(
    CamCalibDbProj_t *calibproj,
    CamCalibDbV2Context_t *def_calib,
    int mode
)
{
    if (!def_calib || !calibproj || !calibproj->main_scene ||
            !calibproj->main_scene[0].sub_scene) {
        printf("%s input invalied!\n", __func__);
        return -1;
    }
    int i = 0;

    if(mode == 0/*NO_HDR*/) {
        for(i = 0; i < calibproj->main_scene_len; i++) {
            if(!strcmp(calibproj->main_scene[i].name, "normal")) {
                def_calib->calib_scene = (char *)(calibdbv2_get_scene_ptr(
                                                      &calibproj->main_scene[i].sub_scene[0]));
                break;
            }
        }
    } else {
        for(i = 0; i < calibproj->main_scene_len; i++) {
            if(!strcmp(calibproj->main_scene[i].name, "hdr")) {
                def_calib->calib_scene = (char *)(calibdbv2_get_scene_ptr(
                                                      &calibproj->main_scene[i].sub_scene[0]));
                break;
            }
        }
    }

    if(i == calibproj->main_scene_len)
        def_calib->calib_scene = (char *)(calibdbv2_get_scene_ptr(
                                              &calibproj->main_scene[0].sub_scene[0]));

    def_calib->sensor_info = &calibproj->sensor_calib;
    def_calib->module_info = &calibproj->module_calib;
    def_calib->sys_cfg = &calibproj->sys_static_cfg;

    return 0;
}

int rkaiq_get_night_calib
(
    CamCalibDbProj_t *calibproj,
    CamCalibDbV2Context_t *def_calib,
    int mode
)
{
    if (!def_calib || !calibproj || !calibproj->main_scene || !calibproj->main_scene[0].sub_scene) {
        printf("%s input invalied!\n", __func__);
        return -1;
    }

    char* main_scene_name;
    if(mode == 0/*NO_HDR*/)
        main_scene_name = "normal";
    else
        main_scene_name = "hdr";

    for(int i = 0; i < calibproj->main_scene_len; i++) {

        if(!strcmp(calibproj->main_scene[i].name, main_scene_name)) {

            for(int j = 0; j < calibproj->main_scene[i].sub_scene_len; j++) {
                if(!strcmp(calibproj->main_scene[i].sub_scene[j].name, "night")) {
                    def_calib->calib_scene = (char *)(calibdbv2_get_scene_ptr(&calibproj->main_scene[i].sub_scene[j]));

                    return 0;
                }
            }
            break;
        }
    }

    return -1;
}

int restoreBinStructMap(uint8_t *data, size_t len, uintptr_t restore_ptr)
{
    size_t map_len = *(size_t *)(data + (len - sizeof(size_t)));
    size_t map_offset = *(size_t *)(data + (len - sizeof(size_t) * 2));
    size_t map_index = 0;
    map_index_t *map_addr = NULL;

    map_addr = (map_index_t *)(data + map_offset);
    for (map_index = 0; map_index < map_len; map_index++) {
        map_index_t tmap = (map_addr[map_index]);
        void** dst_obj_addr = (void**)(data + (size_t)tmap.dst_offset);
        *dst_obj_addr = (void *)(restore_ptr + (uintptr_t)tmap.ptr_offset);
    }

    return 0;
}

void bin2calib(void *bin_buff, CamCalibDbV2Context_t* def_calib) {

#if defined(ISP_HW_V32)
    uint8_t *buf_start = (uint8_t *)bin_buff;
    calib2bin_header_t *header;
    calib2bin_block_t *block_list;

    header = (calib2bin_header_t *)bin_buff;
    block_list = (calib2bin_block_t *)(buf_start + header->block_offset);
    buf_start += header->bin_offset;
    for (int i = 0; i < header->block_len; i++) {
        if (strcmp("CalibDb_Sensor_ParaV2_t", block_list[i].name) == 0)
            memcpy(&def_calib->sensorinfo, (CalibDb_Sensor_ParaV2_t *)rkaiq_calib_load(buf_start + block_list[i].offset, block_list[i].size), sizeof(CalibDb_Sensor_ParaV2_t));
        else if (strcmp("CalibDb_Aec_ParaV2_t", block_list[i].name) == 0)
            memcpy(&def_calib->ae_calib, (CalibDb_Aec_ParaV2_t *)rkaiq_calib_load(buf_start + block_list[i].offset, block_list[i].size), sizeof(CalibDb_Aec_ParaV2_t));
        else if (strcmp("CalibDbV2_Bayer2dnrV23_t", block_list[i].name) == 0)
            memcpy(&def_calib->baynr_calib, (CalibDbV2_Bayer2dnrV23_t *)rkaiq_calib_load(buf_start + block_list[i].offset, block_list[i].size), sizeof(CalibDbV2_Bayer2dnrV23_t));
        else if (strcmp("CalibDbV2_BayerTnrV23_t", block_list[i].name) == 0)
            memcpy(&def_calib->baytnr_calib, (CalibDbV2_BayerTnrV23_t *)rkaiq_calib_load(buf_start + block_list[i].offset, block_list[i].size), sizeof(CalibDbV2_BayerTnrV23_t));
        else if (strcmp("CalibDbV2_Wb_Para_V32_t", block_list[i].name) == 0)
            memcpy(&def_calib->awb_calib, (CalibDbV2_Wb_Para_V32_t *)rkaiq_calib_load(buf_start + block_list[i].offset, block_list[i].size), sizeof(CalibDbV2_Wb_Para_V32_t));
        else if (strcmp("CalibDbV2_Blc_V32_t", block_list[i].name) == 0)
            memcpy(&def_calib->blc_calib, (CalibDbV2_Blc_V32_t *)rkaiq_calib_load(buf_start + block_list[i].offset, block_list[i].size), sizeof(CalibDbV2_Blc_V32_t));

        //rt_kprintf("header info: start %p offset %d size %d\n", buf_start, block_list[i].offset, block_list[i].size);

    }


    /*rt_kprintf("sensor calib: %d %d %d\n", def_calib->sensorinfo.CISDcgSet.Linear.dcg_mode.Coeff[0],
                                          def_calib->sensorinfo.CISDcgSet.Linear.dcg_mode.Coeff[1],
                                          def_calib->sensorinfo.CISDcgSet.Linear.dcg_mode.Coeff[2]);


    rt_kprintf("ae calib: rout: IspDGainDot: ");
    for (int i = 0; i < def_calib->ae_calib.LinearAeCtrl.Route.IspDGainDot_len; i++) {
        rt_kprintf("%f ", def_calib->ae_calib.LinearAeCtrl.Route.IspDGainDot[i]);
    }
    rt_kprintf("\n");

    rt_kprintf("ae calib: rout: PIrisDot: ");
    for (int i = 0; i < def_calib->ae_calib.LinearAeCtrl.Route.PIrisDot_len; i++) {
        rt_kprintf("%d ", def_calib->ae_calib.LinearAeCtrl.Route.PIrisDot[i]);
    }
    rt_kprintf("\n");

    rt_kprintf("ae calib: rout: TimeDot: ");
    for (int i = 0; i < def_calib->ae_calib.LinearAeCtrl.Route.TimeDot_len; i++) {
        rt_kprintf("%f ", def_calib->ae_calib.LinearAeCtrl.Route.TimeDot[i]);
    }
    rt_kprintf("\n");

    rt_kprintf("ae calib: rout: GainDot: ");
    for (int i = 0; i < def_calib->ae_calib.LinearAeCtrl.Route.GainDot_len; i++) {
        rt_kprintf("%f ", def_calib->ae_calib.LinearAeCtrl.Route.GainDot[i]);
    }
    rt_kprintf("\n");

    rt_kprintf("2dnr: %d %d %s\n", def_calib->baynr_calib.TuningPara.hdrdgain_ctrl_en,
                                  def_calib->baynr_calib.TuningPara.Setting_len,
                                  def_calib->baynr_calib.TuningPara.Setting[0].SNR_Mode);

    rt_kprintf("tnr: %d %d %d %d %s\n", def_calib->baytnr_calib.TuningPara.thumbds_w,
                                  def_calib->baytnr_calib.TuningPara.thumbds_h,
                                  def_calib->baytnr_calib.TuningPara.trans_en,
                                  def_calib->baytnr_calib.TuningPara.Setting_len,
                                  def_calib->baytnr_calib.TuningPara.Setting[0].SNR_Mode);

    rt_kprintf("awb: %d %d %d %f %f %f %f\n", def_calib->awb_calib.control.byPass,
                                          def_calib->awb_calib.control.mode,
                                          def_calib->awb_calib.manualPara.mode,
                                          def_calib->awb_calib.manualPara.cfg.mwbGain[0],
                                          def_calib->awb_calib.manualPara.cfg.mwbGain[1],
                                          def_calib->awb_calib.manualPara.cfg.mwbGain[2],
                                          def_calib->awb_calib.manualPara.cfg.mwbGain[3]);

    rt_kprintf("blc %f %f %f %f\n", def_calib->blc_calib.Blc0TuningPara.BLC_Data.R_Channel[0],
                             def_calib->blc_calib.Blc0TuningPara.BLC_Data.B_Channel[0],
                             def_calib->blc_calib.Blc0TuningPara.BLC_Data.Gb_Channel[0],
                             def_calib->blc_calib.Blc0TuningPara.BLC_Data.Gr_Channel[0]);*/
#endif
}

