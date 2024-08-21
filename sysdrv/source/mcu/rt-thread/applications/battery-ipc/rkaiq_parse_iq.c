#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rtthread.h>
#include <tgmath.h>
#include "rkaiq_parse_iq.h"


/******************************************************************************
 * NR
 *****************************************************************************/
void bayer2dnr_init_params_json_v23(NR_BLC_Context_t* pContext, CalibDbV2_Bayer2dnrV23_t* pCalibdb, int calib_idx, int tuning_idx)
{
    int i = 0;
    int j = 0;
    CalibDbV2_Bayer2dnrV23_C_ISO_t *pCalibIso = NULL;
    CalibDbV2_Bayer2dnrV23_T_ISO_t *pTuningISO = NULL;

    //rt_kprintf("\n>>inint 2dnr params!\n");
    if(pCalibdb == NULL || calib_idx < 0 || tuning_idx < 0) {
        rt_kprintf("\n>>inint 2dnr err pointer NULL!\n");
        pContext->bayernr_params_init_flag = false;
        return;
    }

    pContext->bayernr_params.enable = pCalibdb->TuningPara.enable;
    pContext->bayernr_params.hdrdgain_ctrl_en = pCalibdb->TuningPara.hdrdgain_ctrl_en;

    if(calib_idx < pCalibdb->CalibPara.Setting_len) {
        for(i = 0; i < pCalibdb->CalibPara.Setting[calib_idx].Calib_ISO_len && i < 13; i++) {
            pCalibIso = &pCalibdb->CalibPara.Setting[calib_idx].Calib_ISO[i];
            pContext->bayernr_params.iso[i] = pCalibIso->iso;
            for( j = 0; j < 16; j++) {
                pContext->bayernr_params.Bayer2dnrParamsISO[i].lumapoint[j] = pCalibIso->lumapoint[j];
                pContext->bayernr_params.Bayer2dnrParamsISO[i].sigma[j] = pCalibIso->sigma[j];
            }
        }
    }

    if(tuning_idx < pCalibdb->TuningPara.Setting_len) {
        for(i = 0; i < pCalibdb->TuningPara.Setting[tuning_idx].Tuning_ISO_len && i < RK_BAYERNR_V23_MAX_ISO_NUM; i++) {
            pTuningISO = &pCalibdb->TuningPara.Setting[tuning_idx].Tuning_ISO[i];
            pContext->bayernr_params.iso[i] = pTuningISO->iso;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].filter_strength = pTuningISO->filter_strength;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].edgesofts = pTuningISO->edgesofts;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].weight = pTuningISO->weight;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].ratio = pTuningISO->ratio;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].gauss_guide = pTuningISO->gauss_guide;

            pContext->bayernr_params.Bayer2dnrParamsISO[i].gain_bypass = pTuningISO->gain_bypass;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].gain_scale = pTuningISO->gain_scale;
            for(int k = 0; k < 16; k++) {
                pContext->bayernr_params.Bayer2dnrParamsISO[i].gain_lumapoint[k] = pTuningISO->gain_adj.gain_lumapoint[k];
                pContext->bayernr_params.Bayer2dnrParamsISO[i].gain_adj[k] = pTuningISO->gain_adj.gain_adj[k];
            }

            pContext->bayernr_params.Bayer2dnrParamsISO[i].pix_diff = pTuningISO->pix_diff;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].diff_thld = pTuningISO->diff_thld;

            pContext->bayernr_params.Bayer2dnrParamsISO[i].trans_mode = pTuningISO->trans_mode;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].trans_offset = pTuningISO->trans_offset;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].itrans_offset = pTuningISO->itrans_offset;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].trans_datmax = pTuningISO->trans_datmax;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].hdr_dgain_scale_s = pTuningISO->hdr_dgain_scale_s;
            pContext->bayernr_params.Bayer2dnrParamsISO[i].hdr_dgain_scale_m = pTuningISO->hdr_dgain_scale_m;
        }
    }

    pContext->bayernr_params_init_flag = true;
    //rt_kprintf("\n>>inint 2dnr params done!\n");
}

/******************************************************************************
 * 2DNR PARAMS
 *****************************************************************************/
void calculate_bayernr(NR_BLC_Context_t* pContext, int fix_gain, struct isp32_isp_params_cfg *cfg, float gain_ratio)
{
    int iso = 1;
    float gain = 1.0;
    float gainf = 1.0;
    // float exp_time;
    // float timef = 1.0;
    float fdgain[2];
    int isoLevelCorrect = 0;
    int i, tmp;
    int ypos[8] = {4, 4, 4, 3, 3, 2, 2, 1};
    int xpos[8] = {4, 2, 0, 3, 1, 2, 0, 1};
    int bayernr_sw_bil_gauss_weight[16];
    float tmp1, tmp2, edgesofts;
    RK_Bayer2dnrV23_Params_Select_t pSelect;

    if(!pContext->bayernr_params_init_flag)
        return;
    //parse gain and time
    gainf = ((fix_gain & 0xffff) * 10000) >> 16;
    gainf /= 10000;
    gain = ((fix_gain) >> 16 ) + gainf;

    iso = gain * 50;
    for (i = 0; i < RK_BAYERNR_V23_MAX_ISO_NUM - 1; i++) {
        if (iso >= pContext->bayernr_params.iso[i] && iso <= pContext->bayernr_params.iso[i + 1]) {
            isoLevelCorrect = ((iso - pContext->bayernr_params.iso[i]) <= (pContext->bayernr_params.iso[i + 1] - iso)) ? i : (i + 1);
            break;
        }
    }
    if(iso > pContext->bayernr_params.iso[RK_BAYERNR_V23_MAX_ISO_NUM - 1])
        isoLevelCorrect = RK_BAYERNR_V23_MAX_ISO_NUM - 1;
    if(iso < pContext->bayernr_params.iso[0])
        isoLevelCorrect = 0;

    // select iso done!!
    //rt_kprintf("\n>>calculate 2dnr params! iso %d gain %d.%04d, isoselec is %d\n", iso, (int)gain, (int)(gainf * 10000), (int)(pContext->bayernr_params.iso[isoLevelCorrect]));

    pSelect = pContext->bayernr_params.Bayer2dnrParamsISO[isoLevelCorrect];
    pSelect.enable = pContext->bayernr_params.enable;
    // get current iso level done!!

    if(pContext->bayertnr_params.enable) {
        //3dnr on, force 2dnr on
        cfg->module_ens |= ISP32_MODULE_BAYNR;
        cfg->module_en_update |= ISP32_MODULE_BAYNR;
        cfg->module_cfg_update |= ISP32_MODULE_BAYNR;
    } else {
        //3dnr off, 2dnr up to its own params
        if(pContext->bayernr_params.enable) {
            cfg->module_ens |= ISP32_MODULE_BAYNR;
            cfg->module_en_update |= ISP32_MODULE_BAYNR;
            cfg->module_cfg_update |= ISP32_MODULE_BAYNR;
        } else {
            cfg->module_ens &= ~(ISP32_MODULE_BAYNR);
            cfg->module_en_update |= ISP32_MODULE_BAYNR;
            cfg->module_cfg_update |= ISP32_MODULE_BAYNR;
            return;
        }
    }

    //ISP_BAYNR_3A00_CTRL
    if (pContext->bayertnr_params.enable && !pSelect.gain_bypass) {
        cfg->others.baynr_cfg.bay3d_gain_en = 1;
    } else {
        cfg->others.baynr_cfg.bay3d_gain_en = 0;
    }
    cfg->others.baynr_cfg.lg2_mode               = pSelect.trans_mode;
    cfg->others.baynr_cfg.gauss_en               = pSelect.gauss_guide;
    cfg->others.baynr_cfg.log_bypass             = 0;

    // ISP_BAYNR_3A00_DGAIN0-2
    // to do  dgain config 0 yet
    if (gain_ratio == 0) {
        cfg->others.baynr_cfg.dgain0 = 0;
        cfg->others.baynr_cfg.dgain1 = 0;
        cfg->others.baynr_cfg.dgain2 = 0;
    } else {
        fdgain[0] = gain_ratio * 256;
        fdgain[1] = 256;
        if(pSelect.hdrdgain_ctrl_en) {
            tmp = fdgain[0] * pSelect.hdr_dgain_scale_s;
            cfg->others.baynr_cfg.dgain0 = CLIP(tmp, 0, 0xffff);
            cfg->others.baynr_cfg.dgain1 = CLIP((int)(fdgain[1]), 0, 0xffff);
        }
    }

    // ISP_BAYNR_3A00_PIXDIFF
    cfg->others.baynr_cfg.pix_diff = CLIP(pSelect.pix_diff, 0, 0x3fff);

    // ISP_BAYNR_3A00_THLD
    cfg->others.baynr_cfg.diff_thld = CLIP(pSelect.diff_thld, 0, 0x3ff);
    tmp = (int)(pSelect.ratio / pSelect.filter_strength * (1 << 10));
    cfg->others.baynr_cfg.softthld  = CLIP(tmp, 0, 0x3ff);

    // ISP_BAYNR_3A00_W1_STRENG
    cfg->others.baynr_cfg.bltflt_streng = 0;

    if(pSelect.enable == false)
        cfg->others.baynr_cfg.reg_w1 = 0; // 3dnr on, 2dnr bypass
    else
        cfg->others.baynr_cfg.reg_w1 = CLIP((int)(pSelect.weight * (1 << 10)), 0, 0X3FF);


    // ISP_BAYNR_3A00_SIGMAX0-15   ISP_BAYNR_3A00_SIGMAY0-15
    for(i = 0; i < 16; i++) {
        //cfg->others.bay3d_cfg.sigma_x[i] = bayernr_get_trans_V23(pSelect.bayernrv23_filter_lumapoint[i]);
        tmp = pSelect.lumapoint[i];
        cfg->others.baynr_cfg.sigma_x[i] = CLIP(tmp, 0, 0xffff);
        tmp = pSelect.sigma[i] * pSelect.filter_strength;
        cfg->others.baynr_cfg.sigma_y[i] = CLIP(tmp, 0, 0xffff);
    }

    // ISP_BAYNR_3A00_WRIT_D
    edgesofts = pSelect.edgesofts;
    if (edgesofts > 16.0) {
        edgesofts = 16.0;
    }
    for(i = 0; i < 8; i++)
    {
        tmp1 = (float)(ypos[i] * ypos[i] + xpos[i] * xpos[i]);
        tmp1 = tmp1 / (2 * edgesofts * edgesofts);
        tmp2 = expf(-tmp1);
        bayernr_sw_bil_gauss_weight[i] = (int)(tmp1 * (EXP2RECISION_FIX / (1 << 7)));
        bayernr_sw_bil_gauss_weight[i + 8] = (int)(tmp2 * (1 << FIXVSTINV));
    }

    tmp = bayernr_sw_bil_gauss_weight[13];
    cfg->others.baynr_cfg.weit_d0 = CLIP(tmp, 0, 0x3ff);
    tmp = bayernr_sw_bil_gauss_weight[14];
    cfg->others.baynr_cfg.weit_d1 = CLIP(tmp, 0, 0x3ff);
    tmp = bayernr_sw_bil_gauss_weight[15];
    cfg->others.baynr_cfg.weit_d2 = CLIP(tmp, 0, 0x3ff);

    tmp             = pSelect.trans_offset;
    cfg->others.baynr_cfg.lg2_off   = CLIP(tmp, 0, 0x1fff);
    tmp             = pSelect.itrans_offset;
    cfg->others.baynr_cfg.lg2_lgoff = CLIP(tmp, 0, 0xffff);
    tmp             = pSelect.trans_datmax;
    cfg->others.baynr_cfg.dat_max = CLIP(tmp, 0, 0xfffff);

    cfg->others.baynr_cfg.rgain_off = 0;
    cfg->others.baynr_cfg.bgain_off = 0;

    for (i = 0; i < ISP32_BAYNR_GAIN_NUM; i++) {
        tmp             = pSelect.gain_lumapoint[i];
        cfg->others.baynr_cfg.gain_x[i] = CLIP(tmp, 0, 0xff);
        tmp             = pSelect.gain_adj[i] * pSelect.gain_scale;
        cfg->others.baynr_cfg.gain_y[i] = CLIP(tmp, 0, 0xfff);
    }

    //rt_kprintf("\n>>calculate 2dnr params done!\n");

}

void bayertnr_init_params_json_v23(NR_BLC_Context_t* pContext, CalibDbV2_BayerTnrV23_t* pCalibdb, int calib_idx, int tuning_idx)
{
    int i, k;
    CalibDbV2_BayerTnrV23_C_ISO_t *pCalibIso = NULL;
    CalibDbV2_BayerTnrV23_T_ISO_t *pTuningIso = NULL;

    if(pCalibdb == NULL || calib_idx < 0 || tuning_idx < 0) {
        rt_kprintf("\n>>init tnr params erro pointer NULL\n");
        pContext->bayertnr_params_init_flag = false;
        return;
    }

    //rt_kprintf("\n>>init tnr params\n");

    pContext->bayertnr_params.enable = pCalibdb->TuningPara.enable;
    for(i = 0; i < pCalibdb->CalibPara.Setting[calib_idx].Calib_ISO_len && i < RK_BAYERNR_V23_MAX_ISO_NUM; i++) {
        pCalibIso = &pCalibdb->CalibPara.Setting[calib_idx].Calib_ISO[i];
        pContext->bayertnr_params.iso[i] = pCalibIso->iso;
        for(k = 0; k < 16; k++) {
            pContext->bayertnr_params.bayertnrParamISO[i].lumapoint[k] = pCalibIso->lumapoint[k];
            pContext->bayertnr_params.bayertnrParamISO[i].sigma[k] = pCalibIso->sigma[k];
            pContext->bayertnr_params.bayertnrParamISO[i].lumapoint2[k] = pCalibIso->lumapoint2[k];
            pContext->bayertnr_params.bayertnrParamISO[i].lo_sigma[k] = pCalibIso->lo_sigma[k];
            pContext->bayertnr_params.bayertnrParamISO[i].hi_sigma[k] = pCalibIso->hi_sigma[k];
        }
    }

    for(i = 0; i < pCalibdb->TuningPara.Setting[tuning_idx].Tuning_ISO_len && i < RK_BAYERNR_V23_MAX_ISO_NUM; i++) {
        pTuningIso = &pCalibdb->TuningPara.Setting[tuning_idx].Tuning_ISO[i];
        pContext->bayertnr_params.iso[i] = pTuningIso->iso;
        pContext->bayertnr_params.bayertnrParamISO[i].thumbds_w = pCalibdb->TuningPara.thumbds_w;
        pContext->bayertnr_params.bayertnrParamISO[i].thumbds_h = pCalibdb->TuningPara.thumbds_h;
        pContext->bayertnr_params.bayertnrParamISO[i].lo_enable = pCalibdb->TuningPara.lo_enable;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_enable = pTuningIso->hi_enable;
        pContext->bayertnr_params.bayertnrParamISO[i].lo_filter_strength = pTuningIso->lo_filter_strength;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_strength = pTuningIso->hi_filter_strength;
        pContext->bayertnr_params.bayertnrParamISO[i].soft_threshold_ratio = pTuningIso->soft_threshold_ratio;

        pContext->bayertnr_params.bayertnrParamISO[i].lo_clipwgt = pTuningIso->lo_clipwgt;
        pContext->bayertnr_params.bayertnrParamISO[i].lo_med_en = pTuningIso->lo_med_en;
        pContext->bayertnr_params.bayertnrParamISO[i].lo_gsbay_en = pTuningIso->lo_gsbay_en;
        pContext->bayertnr_params.bayertnrParamISO[i].lo_gslum_en = pTuningIso->lo_gslum_en;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_med_en = pTuningIso->hi_med_en;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_gslum_en = pTuningIso->hi_gslum_en;
        pContext->bayertnr_params.bayertnrParamISO[i].global_pk_en = pTuningIso->global_pk_en;
        pContext->bayertnr_params.bayertnrParamISO[i].global_pksq = pTuningIso->global_pksq;

        pContext->bayertnr_params.bayertnrParamISO[i].hidif_th = pTuningIso->hidif_th;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_wgt_comp = pTuningIso->hi_wgt_comp;

        pContext->bayertnr_params.bayertnrParamISO[i].lo_filter_rat0 = pTuningIso->lo_filter_rat0;
        pContext->bayertnr_params.bayertnrParamISO[i].lo_filter_thed0 = pTuningIso->lo_filter_thed0;

        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_abs_ctrl = pTuningIso->hi_filter_abs_ctrl;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_filt_avg = pTuningIso->hi_filter_filt_avg;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_filt_bay = pTuningIso->hi_filter_filt_bay;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_filt_mode = pTuningIso->hi_filter_filt_mode;

        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_rat0 = pTuningIso->hi_filter_rat0;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_thed0 = pTuningIso->hi_filter_thed0;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_rat1 = pTuningIso->hi_filter_rat1;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_filter_thed1 = pTuningIso->hi_filter_thed1;

        pContext->bayertnr_params.bayertnrParamISO[i].guass_guide_coeff0 = pTuningIso->guass_guide_coeff0;
        pContext->bayertnr_params.bayertnrParamISO[i].guass_guide_coeff1 = pTuningIso->guass_guide_coeff1;
        pContext->bayertnr_params.bayertnrParamISO[i].guass_guide_coeff2 = pTuningIso->guass_guide_coeff2;
        pContext->bayertnr_params.bayertnrParamISO[i].guass_guide_coeff3 = pTuningIso->guass_guide_coeff3;
        pContext->bayertnr_params.bayertnrParamISO[i].wgt_use_mode = pTuningIso->wgt_use_mode;
        pContext->bayertnr_params.bayertnrParamISO[i].wgt_mge_mode = pTuningIso->wgt_mge_mode;
        pContext->bayertnr_params.bayertnrParamISO[i].hi_guass = pTuningIso->hi_guass;
        pContext->bayertnr_params.bayertnrParamISO[i].kl_guass = pTuningIso->kl_guass;
        pContext->bayertnr_params.bayertnrParamISO[i].trans_en = pCalibdb->TuningPara.trans_en;
    }

    pContext->bayertnr_params_init_flag = true;
    //rt_kprintf("\n>>init tnr params done!\n");
}

/******************************************************************************
 * TNR PARAMS
 *****************************************************************************/
void calculate_bayertnr(NR_BLC_Context_t* pContext, int fix_gain, struct isp32_isp_params_cfg *cfg)
{
    int iso = 50;
    float gain = 1;
    float gainf = 1.0;
    int isoLevelCorrect = 0;
    int i, tmp;
    RK_Bayertnr_Params_V23_Select_t pSelect;

    if(!pContext->bayertnr_params_init_flag)
        return;

    gainf = ((fix_gain & 0xffff) * 10000) >> 16;
    gainf /= 10000;
    gain = ((fix_gain) >> 16 ) + gainf;

    iso = gain * 50;
    for (i = 0; i < RK_BAYERNR_V23_MAX_ISO_NUM - 1; i++) {
        if (iso >= pContext->bayertnr_params.iso[i] && iso <= pContext->bayertnr_params.iso[i + 1]) {
            isoLevelCorrect = ((iso - pContext->bayertnr_params.iso[i]) <= (pContext->bayertnr_params.iso[i + 1] - iso)) ? i : (i + 1);
            break;
        }
    }
    if(iso > pContext->bayertnr_params.iso[RK_BAYERNR_V23_MAX_ISO_NUM - 1])
        isoLevelCorrect = RK_BAYERNR_V23_MAX_ISO_NUM - 1;
    if(iso < pContext->bayertnr_params.iso[0])
        isoLevelCorrect = 0;

    // select iso done!!
    //rt_kprintf("\n>>calculate tnr params! iso %d gain %d.%04d, iso select %d\n", iso, (int)gain, (int)(gainf * 10000), (int)(pContext->bayernr_params.iso[isoLevelCorrect]));

    pSelect = pContext->bayertnr_params.bayertnrParamISO[isoLevelCorrect];
    pSelect.enable = pContext->bayertnr_params.enable;


    if(pSelect.enable) {
        cfg->module_ens |= ISP32_MODULE_BAY3D;
        cfg->module_en_update |= ISP32_MODULE_BAY3D;
        cfg->module_cfg_update |= ISP32_MODULE_BAY3D;
    } else {
        cfg->module_ens &= ~(ISP32_MODULE_BAY3D);
        cfg->module_en_update |= ISP32_MODULE_BAY3D;
        cfg->module_cfg_update |= ISP32_MODULE_BAY3D;
        return;
    }


    // BAY3D_BAY3D_CTRL 0x2c00
    cfg->others.bay3d_cfg.bwsaving_en = pSelect.trans_en != 0;
    cfg->others.bay3d_cfg.loswitch_protect = 0;
    cfg->others.bay3d_cfg.glbpk_en = pSelect.global_pk_en;
    cfg->others.bay3d_cfg.logaus3_bypass_en = !pSelect.lo_gslum_en;
    cfg->others.bay3d_cfg.logaus5_bypass_en = !pSelect.lo_gsbay_en;
    cfg->others.bay3d_cfg.lomed_bypass_en = !pSelect.lo_med_en;
    cfg->others.bay3d_cfg.hichnsplit_en = (pSelect.hi_filter_filt_bay == 0) ? (pSelect.lo_enable == 0) : (pSelect.hi_filter_filt_bay > 1);
    cfg->others.bay3d_cfg.hiabs_possel = (pSelect.hi_filter_abs_ctrl == 0) ? (pSelect.lo_enable == 0) : (pSelect.hi_filter_abs_ctrl > 1);
    cfg->others.bay3d_cfg.higaus_bypass_en = !pSelect.hi_gslum_en;
    cfg->others.bay3d_cfg.himed_bypass_en = !pSelect.hi_med_en;
    cfg->others.bay3d_cfg.lobypass_en = !pSelect.lo_enable;
    cfg->others.bay3d_cfg.hibypass_en = !pSelect.hi_enable;
    cfg->others.bay3d_cfg.bypass_en = 0;

    // BAY3D_BAY3D_KALRATIO 0x2c04
    tmp = (int)(pSelect.soft_threshold_ratio * (1 << 10) );
    cfg->others.bay3d_cfg.softwgt = CLIP(tmp, 0, 0x3ff);
    tmp = (int)pSelect.hidif_th;
    cfg->others.bay3d_cfg.hidif_th = CLIP(tmp, 0, 0xffff);

    // BAY3D_BAY3D_GLBPK2 0x2c08
    tmp = pSelect.global_pksq;
    cfg->others.bay3d_cfg.glbpk2 = CLIP(tmp, 0, 0xfffffff);

    // BAY3D_BAY3D_CTRL1 0x2c0c
    cfg->others.bay3d_cfg.hiwgt_opt_en = pSelect.wgt_mge_mode;
    cfg->others.bay3d_cfg.hichncor_en = (pSelect.hi_filter_filt_avg == 0) ? (pSelect.lo_enable == 0) : (pSelect.hi_filter_filt_avg > 1);
    cfg->others.bay3d_cfg.bwopt_gain_dis = 0;
    if(pSelect.thumbds_w == 4  && pSelect.thumbds_h == 4) {
        cfg->others.bay3d_cfg.lo4x8_en = 0;
        cfg->others.bay3d_cfg.lo4x4_en = 1;
    } else if(pSelect.thumbds_w == 8  && pSelect.thumbds_h == 4) {
        cfg->others.bay3d_cfg.lo4x8_en = 1;
        cfg->others.bay3d_cfg.lo4x4_en = 0;
    } else if(pSelect.thumbds_w == 8  && pSelect.thumbds_h == 8) {
        cfg->others.bay3d_cfg.lo4x8_en = 0;
        cfg->others.bay3d_cfg.lo4x4_en = 0;
    } else {
        cfg->others.bay3d_cfg.lo4x8_en = 1;
        cfg->others.bay3d_cfg.lo4x4_en = 0;
    }
    cfg->others.bay3d_cfg.hisig_ind_sel = pSelect.hi_guass;
    cfg->others.bay3d_cfg.pksig_ind_sel = pSelect.kl_guass;
    cfg->others.bay3d_cfg.iirwr_rnd_en = 1;
    cfg->others.bay3d_cfg.curds_high_en = 0;
    tmp = pSelect.hi_filter_filt_mode;
    tmp = CLIP(tmp, 0, 4);
    cfg->others.bay3d_cfg.higaus3_mode = 0;
    cfg->others.bay3d_cfg.higaus5x5_en = 0;
    if(tmp == 0)
        cfg->others.bay3d_cfg.higaus3_mode = 0;
    else if(tmp == 1)
        cfg->others.bay3d_cfg.higaus3_mode = 2;
    else if(tmp == 2)
        cfg->others.bay3d_cfg.higaus3_mode = 1;
    else if(tmp == 3)
        cfg->others.bay3d_cfg.higaus5x5_en = 1;
    else if(tmp == 4)
        cfg->others.bay3d_cfg.higaus5x5_en = 0;
    cfg->others.bay3d_cfg.wgtmix_opt_en = pSelect.wgt_use_mode != 0;

    // BAY3D_BAY3D_WGTLMT 0x2c10
    tmp = (int)(((float)1 - pSelect.lo_clipwgt) * (1 << 10));
    cfg->others.bay3d_cfg.wgtlmt = CLIP(tmp, 0, 0x3ff);
    tmp = pSelect.hi_enable ? (int)(pSelect.hi_wgt_comp * (1 << 10)) : 0;
    cfg->others.bay3d_cfg.wgtratio = CLIP(tmp, 0, 0x3ff);

    for(i = 0; i < 16; i++) {
        // BAY3D_BAY3D_SIG_X0  0x2c14 - 0x2c30
        //no need transfer to log domain
        //tmp = bayertnr_get_trans_V23(pSelect.bayertnrv23_tnr_luma_point[i]);
        tmp = pSelect.lumapoint[i];
        cfg->others.bay3d_cfg.sig0_x[i] = CLIP(tmp, 0, 0xffff);

        // BAY3D_BAY3D_SIG0_Y0 0x2c34 - 0x2c50
        tmp = pSelect.sigma[i];
        cfg->others.bay3d_cfg.sig0_y[i] = CLIP(tmp, 0, 0x3fff);

        // BAY3D_BAY3D_SIG_X0  0x2c54 - 0x2c70
        tmp = pSelect.lumapoint2[i];
        cfg->others.bay3d_cfg.sig1_x[i] = CLIP(tmp, 0, 0xffff);

        // BAY3D_BAY3D_SIG1_Y0 0x2c74 - 0x2c90
        tmp = pSelect.hi_sigma[i] * pSelect.hi_filter_strength;
        cfg->others.bay3d_cfg.sig1_y[i] = CLIP(tmp, 0, (1 << 14) - 1);

        // BAY3D_BAY3D_SIG2_Y0 0x2c94 - 0x2cb0
        tmp = pSelect.lo_sigma[i] * pSelect.lo_filter_strength;
        cfg->others.bay3d_cfg.sig2_y[i] = CLIP(tmp, 0, (1 << 10) - 1);
    }

    // BAY3D_BAY3D_HISIGRAT 0x2ce4
    tmp = (int)(pSelect.hi_filter_rat0 * (1 << 8));
    cfg->others.bay3d_cfg.hisigrat0 = CLIP(tmp, 0, 0xfff);
    tmp = (int)(pSelect.hi_filter_rat1 * (1 << 8));
    cfg->others.bay3d_cfg.hisigrat1 = CLIP(tmp, 0, 0xfff);

    // BAY3D_BAY3D_HISIGOFF 0x2ce8
    tmp = (int)(pSelect.hi_filter_thed0);
    cfg->others.bay3d_cfg.hisigoff0 = CLIP(tmp, 0, 0xfff);
    tmp = (int)(pSelect.hi_filter_thed1);
    cfg->others.bay3d_cfg.hisigoff1 = CLIP(tmp, 0, 0xfff);

    // BAY3D_BAY3D_LOSIG 0x2cec
    tmp = (int)(pSelect.lo_filter_thed0);
    cfg->others.bay3d_cfg.losigoff = CLIP(tmp, 0, 0xfff);
    tmp = (int)(pSelect.lo_filter_rat0 * (1 << 8));
    cfg->others.bay3d_cfg.losigrat = CLIP(tmp, 0, 0xfff);

    // BAY3D_BAY3D_LOSIG 0x2cf0
    tmp = (int)(0);
    cfg->others.bay3d_cfg.rgain_off = CLIP(tmp, 0, 0x3fff);
    tmp = (int)(0);
    cfg->others.bay3d_cfg.bgain_off = CLIP(tmp, 0, 0x3fff);

    // BAY3D_BAY3D_SIGGAUS 0x2cf4
    tmp = (int)(pSelect.guass_guide_coeff0);
    cfg->others.bay3d_cfg.siggaus0 = CLIP(tmp, 0, 0x3f);
    tmp = (int)(pSelect.guass_guide_coeff1);
    cfg->others.bay3d_cfg.siggaus1 = CLIP(tmp, 0, 0x3f);
    tmp = (int)(pSelect.guass_guide_coeff2);
    cfg->others.bay3d_cfg.siggaus2 = CLIP(tmp, 0, 0x3f);
    tmp = (int)(pSelect.guass_guide_coeff3);
    cfg->others.bay3d_cfg.siggaus3 = CLIP(tmp, 0, 0x3f);

    //rt_kprintf("\n>>calculate tnr params done!\n");
}

/******************************************************************************
 * BLC
 *****************************************************************************/
static int blc_json_param_init_v32(AblcParams_V32_t* pParams, const AblcV32ParaV2_t* pBlcCalibParams)
{
    if (pParams == NULL || pBlcCalibParams == NULL) {
        rt_kprintf("\n>>inint blc err pointer NULL!\n");
        return 1;
    }

    pParams->enable = pBlcCalibParams->enable;
    for (int i = 0; i < ABLCV32_MAX_ISO_LEVEL; i++) {
        pParams->iso[i]    = pBlcCalibParams->BLC_Data.ISO[i];
        pParams->blc_r[i]  = pBlcCalibParams->BLC_Data.R_Channel[i];
        pParams->blc_gr[i] = pBlcCalibParams->BLC_Data.Gr_Channel[i];
        pParams->blc_gb[i] = pBlcCalibParams->BLC_Data.Gb_Channel[i];
        pParams->blc_b[i]  = pBlcCalibParams->BLC_Data.B_Channel[i];
    }

    return 0;
}

static int blc_ob_json_param_init_v32(AblcOBParams_V32_t* pParams, const AblcV32OBPara_t* pBlcOBCalibParams)
{

    if (pParams == NULL || pBlcOBCalibParams == NULL) {
        rt_kprintf("\n>>inint blc_ob err pointer NULL!\n");
        return 1;
    }
    pParams->enable = pBlcOBCalibParams->enable;
    for (int i = 0; i < ABLCV32_MAX_ISO_LEVEL; i++) {
        pParams->iso[i]         = pBlcOBCalibParams->BLC_OB_Data.ISO[i];
        pParams->ob_offset[i]   = pBlcOBCalibParams->BLC_OB_Data.isp_ob_Offset[i];
        pParams->ob_predgain[i] = pBlcOBCalibParams->BLC_OB_Data.isp_ob_preDgain[i];
    }
    return 0;
}

static int blc_select_params_by_iso_v32(const AblcParams_V32_t* pParams, AblcSelect_V32_t* pSelect, int fix_gain)
{

    int isoLowlevel  = 0;
    int isoHighlevel = 0;
    int lowIso       = 0;
    int highIso      = 0;
    float ratio      = 0.0f;
    float gain = 1;
    float gainf = 1.0;
    int isoValue     = 50;
    int i            = 0;

    if (pParams == NULL || pSelect == NULL) {
        return 1;
    }

    gainf = ((fix_gain & 0xffff) * 10000) >> 16;
    gainf /= 10000;
    gain = ((fix_gain) >> 16 ) + gainf;

    isoValue = gain * 50;
    for (i = 0; i < ABLCV32_MAX_ISO_LEVEL - 1; i++) {
        if (isoValue >= pParams->iso[i] && isoValue <= pParams->iso[i + 1]) {
            isoLowlevel  = i;
            isoHighlevel = i + 1;
            lowIso       = pParams->iso[i];
            highIso      = pParams->iso[i + 1];
            ratio        = (isoValue - lowIso) / (float)(highIso - lowIso);
            break;
        }
    }

    if (i == ABLCV32_MAX_ISO_LEVEL - 1) {
        if (isoValue < pParams->iso[0]) {
            isoLowlevel  = 0;
            isoHighlevel = 1;
            ratio        = 0;
        }

        if (isoValue > pParams->iso[ABLCV32_MAX_ISO_LEVEL - 1]) {
            isoLowlevel  = ABLCV32_MAX_ISO_LEVEL - 1;
            isoHighlevel = ABLCV32_MAX_ISO_LEVEL - 1;
            ratio        = 0;
        }
    }

    //rt_kprintf("\n>>calculate blc params! iso: %d gain %d.%04d, select ratio: %d\n", isoValue, (int)gain, (int)(gainf * 10000), (int)(ratio * 100));

    pSelect->enable = pParams->enable;

    pSelect->blc_r =
        (short int)(ratio * (pParams->blc_r[isoHighlevel] - pParams->blc_r[isoLowlevel]) +
                    pParams->blc_r[isoLowlevel]);
    pSelect->blc_gr =
        (short int)(ratio * (pParams->blc_gr[isoHighlevel] - pParams->blc_gr[isoLowlevel]) +
                    pParams->blc_gr[isoLowlevel]);
    pSelect->blc_gb =
        (short int)(ratio * (pParams->blc_gb[isoHighlevel] - pParams->blc_gb[isoLowlevel]) +
                    pParams->blc_gb[isoLowlevel]);
    pSelect->blc_b =
        (short int)(ratio * (pParams->blc_b[isoHighlevel] - pParams->blc_b[isoLowlevel]) +
                    pParams->blc_b[isoLowlevel]);

    return 0;
}

static int blc_select_obparams_by_iso_v32(const AblcOBParams_V32_t* pParams, AblcOBSelect_V32_t* pSelect, int fix_gain)
{

    int isoLowlevel  = 0;
    int isoHighlevel = 0;
    int lowIso       = 0;
    int highIso      = 0;
    float ratio      = 0.0f;
    float gain = 1;
    float gainf = 1.0;
    int isoValue     = 50;
    int i            = 0;

    if (pParams == NULL || pSelect == NULL) {
        return 1;
    }

    gainf = ((fix_gain & 0xffff) * 10000) >> 16;
    gainf /= 10000;
    gain = ((fix_gain) >> 16 ) + gainf;
    isoValue = gain * 50;

    for (i = 0; i < ABLCV32_MAX_ISO_LEVEL - 1; i++) {
        if (isoValue >= pParams->iso[i] && isoValue <= pParams->iso[i + 1]) {
            isoLowlevel  = i;
            isoHighlevel = i + 1;
            lowIso       = pParams->iso[i];
            highIso      = pParams->iso[i + 1];
            ratio        = (isoValue - lowIso) / (float)(highIso - lowIso);
            break;
        }
    }

    if (i == ABLCV32_MAX_ISO_LEVEL - 1) {
        if (isoValue < pParams->iso[0]) {
            isoLowlevel  = 0;
            isoHighlevel = 1;
            ratio        = 0;
        }

        if (isoValue > pParams->iso[ABLCV32_MAX_ISO_LEVEL - 1]) {
            isoLowlevel  = ABLCV32_MAX_ISO_LEVEL - 1;
            isoHighlevel = ABLCV32_MAX_ISO_LEVEL - 1;
            ratio        = 0;
        }
    }

    //rt_kprintf("\n>>calculate blc_ob params! iso: %d gain %d.%04d, select ratio: %d\n", isoValue,(int)gain, (int)(gainf * 10000), (int)(ratio * 100));

    pSelect->enable      = pParams->enable;
    pSelect->ob_offset   = (short int)(ratio * (pParams->ob_offset[isoHighlevel] -
                                       pParams->ob_offset[isoLowlevel]) +
                                       pParams->ob_offset[isoLowlevel]);
    pSelect->ob_predgain = (float)(ratio * (pParams->ob_predgain[isoHighlevel] -
                                            pParams->ob_predgain[isoLowlevel]) +
                                   pParams->ob_predgain[isoLowlevel]);

    return 0;
}

void blc_init_params_json_v23(NR_BLC_Context_t* pContext, CalibDbV2_Blc_V32_t* pCalibDb)
{
    int ret = 0;

    ret += blc_json_param_init_v32(&pContext->blc_params.stBlc0Params, &pCalibDb->Blc0TuningPara);
    ret += blc_json_param_init_v32(&pContext->blc_params.stBlc1Params, &pCalibDb->Blc1TuningPara);
    ret += blc_ob_json_param_init_v32(&pContext->blc_params.stBlcOBParams, &pCalibDb->BlcObPara);

    pContext->blc_params_init_flag = true;
}

/******************************************************************************
 * BLC PARAMS
 *****************************************************************************/
void calculate_blc(NR_BLC_Context_t* pContext, int fix_gain, struct isp32_isp_params_cfg *cfg, int hdr_mode)
{
    if(!pContext->blc_params_init_flag)
        return;

    // blc params selct
    if (blc_select_params_by_iso_v32(&pContext->blc_params.stBlc0Params, &pContext->blc_params.stBlc0Select, fix_gain)) {
        rt_kprintf("\n>>selcet blc0 params erro pointer NULL\n");
        return;
    }
    if (blc_select_params_by_iso_v32(&pContext->blc_params.stBlc1Params, &pContext->blc_params.stBlc1Select, fix_gain)) {
        rt_kprintf("\n>>selcet blc1 params erro pointer NULL\n");
        return;
    }
    if (blc_select_obparams_by_iso_v32(&pContext->blc_params.stBlcOBParams, &pContext->blc_params.stBlcOBSelect, fix_gain)) {
        rt_kprintf("\n>>selcet blc_ob params erro pointer NULL\n");
        return;
    }

    //BLC moudle config
    if (pContext->blc_params.stBlc0Select.enable) {
        cfg->module_ens |= ISP3X_MODULE_BLS;
    }
    cfg->module_en_update |= ISP32_MODULE_BLS;
    cfg->module_cfg_update |= ISP32_MODULE_BLS;

    cfg->others.bls_cfg.enable_auto = 0;
    cfg->others.bls_cfg.en_windows  = 0;
    cfg->others.bls_cfg.bls_window1.h_offs = 0;
    cfg->others.bls_cfg.bls_window1.v_offs = 0;
    cfg->others.bls_cfg.bls_window1.h_size = 0;
    cfg->others.bls_cfg.bls_window1.v_size = 0;

    cfg->others.bls_cfg.bls_window2.h_offs = 0;
    cfg->others.bls_cfg.bls_window2.v_offs = 0;
    cfg->others.bls_cfg.bls_window2.h_size = 0;
    cfg->others.bls_cfg.bls_window2.v_size = 0;
    cfg->others.bls_cfg.bls_samples = 0;

    if (hdr_mode == 0) {
        // blc0
        if (pContext->blc_params.stBlc0Select.enable) {
            cfg->others.bls_cfg.fixed_val.r  = pContext->blc_params.stBlc0Select.blc_r;
            cfg->others.bls_cfg.fixed_val.gr = pContext->blc_params.stBlc0Select.blc_gr;
            cfg->others.bls_cfg.fixed_val.gb = pContext->blc_params.stBlc0Select.blc_gb;
            cfg->others.bls_cfg.fixed_val.b  = pContext->blc_params.stBlc0Select.blc_b;
        } else {
            cfg->others.bls_cfg.fixed_val.r  = 0;
            cfg->others.bls_cfg.fixed_val.gr = 0;
            cfg->others.bls_cfg.fixed_val.gb = 0;
            cfg->others.bls_cfg.fixed_val.b  = 0;
        }
        //blc1
        cfg->others.bls_cfg.bls1_en = pContext->blc_params.stBlc1Select.enable;            // TODO bls1 params
        if (pContext->blc_params.stBlc1Select.enable) {
            if (pContext->blc_params.stBlcOBSelect.ob_predgain != 0 ) {
                cfg->others.bls_cfg.bls1_val.r  = CLIP((int)(pContext->blc_params.stBlc1Select.blc_r * pContext->blc_params.stBlcOBSelect.ob_predgain), 0, 32767);
                cfg->others.bls_cfg.bls1_val.gr = CLIP((int)(pContext->blc_params.stBlc1Select.blc_gr * pContext->blc_params.stBlcOBSelect.ob_predgain), 0, 32767);
                cfg->others.bls_cfg.bls1_val.gb = CLIP((int)(pContext->blc_params.stBlc1Select.blc_gb * pContext->blc_params.stBlcOBSelect.ob_predgain), 0, 32767);
                cfg->others.bls_cfg.bls1_val.b  = CLIP((int)(pContext->blc_params.stBlc1Select.blc_b * pContext->blc_params.stBlcOBSelect.ob_predgain), 0, 32767);
            } else {
                cfg->others.bls_cfg.bls1_val.r  = (int)pContext->blc_params.stBlc1Select.blc_r;
                cfg->others.bls_cfg.bls1_val.gr = (int)pContext->blc_params.stBlc1Select.blc_gr;
                cfg->others.bls_cfg.bls1_val.gb = (int)pContext->blc_params.stBlc1Select.blc_gb;
                cfg->others.bls_cfg.bls1_val.b  = (int)pContext->blc_params.stBlc1Select.blc_b;
            }
        } else {
            cfg->others.bls_cfg.bls1_val.r  = 0;
            cfg->others.bls_cfg.bls1_val.gr = 0;
            cfg->others.bls_cfg.bls1_val.gb = 0;
            cfg->others.bls_cfg.bls1_val.b  = 0;
        }
        //blc_ob
        if (pContext->blc_params.stBlcOBSelect.enable) {
            cfg->others.bls_cfg.isp_ob_offset   = CLIP((int)pContext->blc_params.stBlcOBSelect.ob_offset, 0, 511);
            cfg->others.bls_cfg.isp_ob_predgain = CLIP((int)(pContext->blc_params.stBlcOBSelect.ob_predgain * (1 << 8)), 0, 65535);
            cfg->others.bls_cfg.isp_ob_max      = CLIP((int)((4096 * pContext->blc_params.stBlcOBSelect.ob_predgain) - pContext->blc_params.stBlcOBSelect.ob_offset), 0, 1048575);
        } else {
            cfg->others.bls_cfg.isp_ob_offset   = 0;
            cfg->others.bls_cfg.isp_ob_predgain = 0x100;
            cfg->others.bls_cfg.isp_ob_max      = 0xfffff;
        }
    } else {
        /*********hdr mode*********/
        // blc0
        if (pContext->blc_params.stBlc0Select.enable) {
            cfg->others.bls_cfg.fixed_val.r  = pContext->blc_params.stBlc0Select.blc_r;
            cfg->others.bls_cfg.fixed_val.gr = pContext->blc_params.stBlc0Select.blc_gr;
            cfg->others.bls_cfg.fixed_val.gb = pContext->blc_params.stBlc0Select.blc_gb;
            cfg->others.bls_cfg.fixed_val.b  = pContext->blc_params.stBlc0Select.blc_b;
        } else {
            cfg->others.bls_cfg.fixed_val.r  = 0;
            cfg->others.bls_cfg.fixed_val.gr = 0;
            cfg->others.bls_cfg.fixed_val.gb = 0;
            cfg->others.bls_cfg.fixed_val.b  = 0;
        }
        //blc1
        cfg->others.bls_cfg.bls1_en = 0;            // TODO bls1 params
        cfg->others.bls_cfg.bls1_val.r  = 0;
        cfg->others.bls_cfg.bls1_val.gr = 0;
        cfg->others.bls_cfg.bls1_val.gb = 0;
        cfg->others.bls_cfg.bls1_val.b  = 0;
        //blc_ob
        cfg->others.bls_cfg.isp_ob_offset   = 0;
        cfg->others.bls_cfg.isp_ob_predgain = 0x100;
        cfg->others.bls_cfg.isp_ob_max      = 0xfffff;
    }

    //blc2
    cfg->meas.rawawb.bls2_en = false;
    cfg->meas.rawawb.bls2_val.b = 0;
    cfg->meas.rawawb.bls2_val.gb = 0;
    cfg->meas.rawawb.bls2_val.gr = 0;
    cfg->meas.rawawb.bls2_val.r = 0;

    if (hdr_mode == 0) {
        cfg->meas.rawawb.bls2_en = true;
        if (pContext->blc_params.stBlc1Select.enable) {
            cfg->meas.rawawb.bls2_val.r  = (int)pContext->blc_params.stBlc1Select.blc_r;
            cfg->meas.rawawb.bls2_val.gr = (int)pContext->blc_params.stBlc1Select.blc_gr;
            cfg->meas.rawawb.bls2_val.gb = (int)pContext->blc_params.stBlc1Select.blc_gb;
            cfg->meas.rawawb.bls2_val.b  = (int)pContext->blc_params.stBlc1Select.blc_b;
        }
    }
}

static int16_t UtlFloatToFix(float fFloat, int bit)
{
    int16_t ulFix = 0;

    fFloat *= (1 << bit);

    if ((fFloat > 0.0f))
    {
        ulFix = (int16_t)(fFloat + 0.5f);
    }
    else
    {
        ulFix  = (int16_t)(fFloat - 0.5f);
    }
    return ulFix;
}

/******************************************************************************
 * AWB PARAMS
 *****************************************************************************/
int set_awb_params_for_kernel(struct isp32_isp_params_cfg *cfg, CalibDbV2_Wb_Para_V32_t *wb_calib, int hdr_mode, int width, int height)
{
    cfg->module_ens |= ISP3X_MODULE_RAWAWB;
    cfg->module_en_update |= ISP3X_MODULE_RAWAWB;
    cfg->module_cfg_update |= ISP3X_MODULE_RAWAWB;

    if(hdr_mode == 0)
        cfg->meas.rawawb.rawawb_sel =  0;
    else
        cfg->meas.rawawb.rawawb_sel =  1;
    cfg->meas.rawawb.bnr2awb_sel = 0;
    cfg->meas.rawawb.drc2awb_sel = 0;
    cfg->meas.rawawb.uv_en0 = 0;
    cfg->meas.rawawb.xy_en0 = 0;
    cfg->meas.rawawb.yuv3d_en0 = 0;
    cfg->meas.rawawb.in_rshift_to_12bit_en = 0;
    cfg->meas.rawawb.in_overexposure_check_en = 0;
    cfg->meas.rawawb.wind_size = 1;
    cfg->meas.rawawb.rawlsc_bypass_en = 1;
    cfg->meas.rawawb.light_num = 4;
    cfg->meas.rawawb.uv_en1 = 0;
    cfg->meas.rawawb.xy_en1 = 0;
    cfg->meas.rawawb.yuv3d_en1 = 0;
    cfg->meas.rawawb.low12bit_val = 1;
    cfg->meas.rawawb.r_max = 230 * 16;
    cfg->meas.rawawb.g_max = 230 * 16;
    cfg->meas.rawawb.b_max = 230 * 16;
    cfg->meas.rawawb.y_max = 230 * 16;
    cfg->meas.rawawb.r_min = 3 * 16;
    cfg->meas.rawawb.g_min = 3 * 16;
    cfg->meas.rawawb.b_min = 3 * 16;
    cfg->meas.rawawb.y_min = 3 * 16;
    cfg->meas.rawawb.h_offs = 0;
    cfg->meas.rawawb.v_offs = 0;
    cfg->meas.rawawb.h_size = width;
    cfg->meas.rawawb.v_size = height;
    cfg->meas.rawawb.pre_wbgain_inv_r = 256;
    cfg->meas.rawawb.pre_wbgain_inv_g = 256;
    cfg->meas.rawawb.pre_wbgain_inv_b = 256;

    if (wb_calib == NULL)
        return 0;

    CalibDbV2_Wb_Awb_Para_V32_t *autoPara = &wb_calib->autoPara;
    //use xy
    cfg->meas.rawawb.xy_en0           = autoPara->xyDetectionEnable;
    cfg->meas.rawawb.xy_en1           = autoPara->xyDetectionEnable;
    cfg->meas.rawawb.wt0              = autoPara->rgb2TcsPara.pseudoLuminanceWeight[0] * 4096 + 0.5;
    cfg->meas.rawawb.wt1              = autoPara->rgb2TcsPara.pseudoLuminanceWeight[1] * 4096 + 0.5;
    cfg->meas.rawawb.wt2              = autoPara->rgb2TcsPara.pseudoLuminanceWeight[2] * 4096 + 0.5;
    cfg->meas.rawawb.mat0_x           = UtlFloatToFix(autoPara->rgb2TcsPara.rotationMat[0], 12);
    cfg->meas.rawawb.mat1_x           = UtlFloatToFix(autoPara->rgb2TcsPara.rotationMat[1], 12);
    cfg->meas.rawawb.mat2_x           = UtlFloatToFix(autoPara->rgb2TcsPara.rotationMat[2], 12);
    cfg->meas.rawawb.mat0_y           = UtlFloatToFix(autoPara->rgb2TcsPara.rotationMat[3], 12);
    cfg->meas.rawawb.mat1_y           = UtlFloatToFix(autoPara->rgb2TcsPara.rotationMat[4], 12);
    cfg->meas.rawawb.mat2_y           = UtlFloatToFix(autoPara->rgb2TcsPara.rotationMat[5], 12);
    cfg->meas.rawawb.nor_x0_0         = autoPara->earlierAwbAct.xyRegion[0].normal[0];
    cfg->meas.rawawb.nor_x1_0         = autoPara->earlierAwbAct.xyRegion[0].normal[1];
    cfg->meas.rawawb.nor_y0_0         = autoPara->earlierAwbAct.xyRegion[0].normal[2];
    cfg->meas.rawawb.nor_y1_0         = autoPara->earlierAwbAct.xyRegion[0].normal[3];
    cfg->meas.rawawb.big_x0_0         = autoPara->earlierAwbAct.xyRegion[0].big[0];
    cfg->meas.rawawb.big_x1_0         = autoPara->earlierAwbAct.xyRegion[0].big[1];
    cfg->meas.rawawb.big_y0_0         = autoPara->earlierAwbAct.xyRegion[0].big[2];
    cfg->meas.rawawb.big_y1_0         = autoPara->earlierAwbAct.xyRegion[0].big[3];
    cfg->meas.rawawb.nor_x0_1         = autoPara->earlierAwbAct.xyRegion[1].normal[0];
    cfg->meas.rawawb.nor_x1_1         = autoPara->earlierAwbAct.xyRegion[1].normal[1];
    cfg->meas.rawawb.nor_y0_1         = autoPara->earlierAwbAct.xyRegion[1].normal[2];
    cfg->meas.rawawb.nor_y1_1         = autoPara->earlierAwbAct.xyRegion[1].normal[3];
    cfg->meas.rawawb.big_x0_1         = autoPara->earlierAwbAct.xyRegion[1].big[0];
    cfg->meas.rawawb.big_x1_1         = autoPara->earlierAwbAct.xyRegion[1].big[1];
    cfg->meas.rawawb.big_y0_1         = autoPara->earlierAwbAct.xyRegion[1].big[2];
    cfg->meas.rawawb.big_y1_1         = autoPara->earlierAwbAct.xyRegion[1].big[3];
    cfg->meas.rawawb.nor_x0_2         = autoPara->earlierAwbAct.xyRegion[2].normal[0];
    cfg->meas.rawawb.nor_x1_2         = autoPara->earlierAwbAct.xyRegion[2].normal[1];
    cfg->meas.rawawb.nor_y0_2         = autoPara->earlierAwbAct.xyRegion[2].normal[2];
    cfg->meas.rawawb.nor_y1_2         = autoPara->earlierAwbAct.xyRegion[2].normal[3];
    cfg->meas.rawawb.big_x0_2         = autoPara->earlierAwbAct.xyRegion[2].big[0];
    cfg->meas.rawawb.big_x1_2         = autoPara->earlierAwbAct.xyRegion[2].big[1];
    cfg->meas.rawawb.big_y0_2         = autoPara->earlierAwbAct.xyRegion[2].big[2];
    cfg->meas.rawawb.big_y1_2         = autoPara->earlierAwbAct.xyRegion[2].big[3];
    cfg->meas.rawawb.nor_x0_3         = autoPara->earlierAwbAct.xyRegion[3].normal[0];
    cfg->meas.rawawb.nor_x1_3         = autoPara->earlierAwbAct.xyRegion[3].normal[1];
    cfg->meas.rawawb.nor_y0_3         = autoPara->earlierAwbAct.xyRegion[3].normal[2];
    cfg->meas.rawawb.nor_y1_3         = autoPara->earlierAwbAct.xyRegion[3].normal[3];
    cfg->meas.rawawb.big_x0_3         = autoPara->earlierAwbAct.xyRegion[3].big[0];
    cfg->meas.rawawb.big_x1_3         = autoPara->earlierAwbAct.xyRegion[3].big[1];
    cfg->meas.rawawb.big_y0_3         = autoPara->earlierAwbAct.xyRegion[3].big[2];
    cfg->meas.rawawb.big_y1_3         = autoPara->earlierAwbAct.xyRegion[3].big[3];
    return 0;

}

/******************************************************************************
 * RAWAE PARAMS
 *****************************************************************************/
void set_rawae_params_for_kernel(struct isp32_isp_params_cfg *cfg, uint16_t hdr_mode, uint16_t width, uint16_t height)
{
    cfg->module_ens |= ISP32_MODULE_RAWAE1;
    cfg->module_ens |= ISP32_MODULE_RAWAE3;

    cfg->module_en_update |= ISP32_MODULE_RAWAE1;
    cfg->module_cfg_update |= ISP32_MODULE_RAWAE1;
    cfg->module_en_update |= ISP32_MODULE_RAWAE3;
    cfg->module_cfg_update |= ISP32_MODULE_RAWAE3;

    cfg->meas.rawae1.wnd_num = 2;
    cfg->meas.rawae1.win.h_size = width;
    cfg->meas.rawae1.win.v_size = height;
    cfg->meas.rawae1.win.h_offs = 0;
    cfg->meas.rawae1.win.v_offs = 0;

    cfg->meas.rawae3.wnd_num = 2;
    cfg->meas.rawae3.win.h_size = width;
    cfg->meas.rawae3.win.v_size = height;
    cfg->meas.rawae3.win.h_offs = 0;
    cfg->meas.rawae3.win.v_offs = 0;

    if(hdr_mode != NO_HDR) {
        cfg->meas.rawae1.rawae_sel = 0;
        cfg->meas.rawae3.rawae_sel = 0;

        cfg->module_ens |= ISP32_MODULE_RAWAE0;
        cfg->module_en_update |= ISP32_MODULE_RAWAE0;
        cfg->module_cfg_update |= ISP32_MODULE_RAWAE0;

        cfg->meas.rawae0.rawae_sel = 0;
        cfg->meas.rawae0.wnd_num = 1;
        cfg->meas.rawae0.win.h_size = width;
        cfg->meas.rawae0.win.v_size = height;
        cfg->meas.rawae0.win.h_offs = 0;
        cfg->meas.rawae0.win.v_offs = 0;

    } else {
        cfg->meas.rawae1.rawae_sel = 1;
        cfg->meas.rawae3.rawae_sel = 3;
    }

}


/******************************************************************************
 * RAWHIST PARAMS
 *****************************************************************************/
void set_rawhist_params_for_kernel(struct isp32_isp_params_cfg *cfg, uint16_t hdr_mode, uint16_t width, uint16_t height)
{
    cfg->module_ens |= ISP32_MODULE_RAWHIST1;
    cfg->module_ens |= ISP32_MODULE_RAWHIST3;

    cfg->module_en_update |= ISP32_MODULE_RAWHIST1;
    cfg->module_cfg_update |= ISP32_MODULE_RAWHIST1;
    cfg->module_en_update |= ISP32_MODULE_RAWHIST3;
    cfg->module_cfg_update |= ISP32_MODULE_RAWHIST3;

    cfg->meas.rawhist3.data_sel = 0;
    cfg->meas.rawhist3.waterline = 0;
    cfg->meas.rawhist3.mode = 5;
    cfg->meas.rawhist3.wnd_num = 2;
    cfg->meas.rawhist3.stepsize = 0;
    cfg->meas.rawhist3.win.h_size = width;
    cfg->meas.rawhist3.win.v_size = height;
    cfg->meas.rawhist3.win.h_offs = 0;
    cfg->meas.rawhist3.win.v_offs = 0;
    cfg->meas.rawhist3.rcc = 0x4d;
    cfg->meas.rawhist3.gcc = 0x4b;
    cfg->meas.rawhist3.bcc = 0x1d;
    cfg->meas.rawhist3.off = 0x00;
    rt_memset(cfg->meas.rawhist3.weight, 0x01, 225 * sizeof(unsigned char));

    cfg->meas.rawhist1.data_sel = 0;
    cfg->meas.rawhist1.waterline = 0;
    cfg->meas.rawhist1.mode = 5;
    cfg->meas.rawhist1.wnd_num = 2;
    cfg->meas.rawhist1.stepsize = 0;
    cfg->meas.rawhist1.win.h_size = width;
    cfg->meas.rawhist1.win.v_size = height;
    cfg->meas.rawhist1.win.h_offs = 0;
    cfg->meas.rawhist1.win.v_offs = 0;
    cfg->meas.rawhist1.rcc = 0x4d;
    cfg->meas.rawhist1.gcc = 0x4b;
    cfg->meas.rawhist1.bcc = 0x1d;
    cfg->meas.rawhist1.off = 0x00;
    rt_memset(cfg->meas.rawhist1.weight, 0x01, 225 * sizeof(unsigned char));

    if(hdr_mode != NO_HDR) {
        cfg->module_ens |= ISP32_MODULE_RAWHIST0;
        cfg->module_en_update |= ISP32_MODULE_RAWHIST0;
        cfg->module_cfg_update |= ISP32_MODULE_RAWHIST0;

        cfg->meas.rawhist0.data_sel = 0;
        cfg->meas.rawhist0.waterline = 0;
        cfg->meas.rawhist0.mode = 5;
        cfg->meas.rawhist0.stepsize = 0;
        cfg->meas.rawhist0.win.h_size = width;
        cfg->meas.rawhist0.win.v_size = height;
        cfg->meas.rawhist0.win.h_offs = 0;
        cfg->meas.rawhist0.win.v_offs = 0;
        cfg->meas.rawhist0.rcc = 0x4d;
        cfg->meas.rawhist0.gcc = 0x4b;
        cfg->meas.rawhist0.bcc = 0x1d;
        cfg->meas.rawhist0.off = 0x00;
        rt_memset(cfg->meas.rawhist0.weight, 0x01, 25 * sizeof(unsigned char));
    }

}

/******************************************************************************
 * AWBGAIN PARAMS
 *****************************************************************************/
void set_awbgain_params_for_kernel(struct isp32_isp_params_cfg *cfg, uint16_t hdr_mode, int is_ie_effect)
{
    cfg->module_ens |= ISP32_MODULE_AWB_GAIN;
    cfg->module_en_update |= ISP32_MODULE_AWB_GAIN;
    cfg->module_cfg_update |= ISP32_MODULE_AWB_GAIN;

    if (is_ie_effect)
    {
        //bw night mode, enable IE, disable AWB
        cfg->module_ens |= ISP3X_MODULE_IE;
        cfg->module_en_update |= ISP3X_MODULE_IE;
        cfg->module_cfg_update |= ISP3X_MODULE_IE;
        cfg->others.ie_cfg.effect = is_ie_effect;
        cfg->others.awb_gain_cfg.awb1_gain_gb = 256;
        cfg->others.awb_gain_cfg.awb1_gain_gr = 256;
        cfg->others.awb_gain_cfg.awb1_gain_b = 256;
        cfg->others.awb_gain_cfg.awb1_gain_r = 256;

        cfg->others.awb_gain_cfg.gain0_green_b = 256;
        cfg->others.awb_gain_cfg.gain0_green_r = 256;
        cfg->others.awb_gain_cfg.gain0_blue = 256;
        cfg->others.awb_gain_cfg.gain0_red = 256;
        cfg->others.awb_gain_cfg.gain1_green_b = 256;
        cfg->others.awb_gain_cfg.gain1_green_r = 256;
        cfg->others.awb_gain_cfg.gain1_blue = 256;
        cfg->others.awb_gain_cfg.gain1_red = 256;
        cfg->others.awb_gain_cfg.gain2_green_b = 256;
        cfg->others.awb_gain_cfg.gain2_green_r = 256;
        cfg->others.awb_gain_cfg.gain2_blue = 256;
        cfg->others.awb_gain_cfg.gain2_red = 256;
    }
    else
    {
        //color mode, enable AWB
        if(hdr_mode == NO_HDR) {
            cfg->others.awb_gain_cfg.awb1_gain_gb = 256;
            cfg->others.awb_gain_cfg.awb1_gain_gr = 256;
            cfg->others.awb_gain_cfg.awb1_gain_b = 256;
            cfg->others.awb_gain_cfg.awb1_gain_r = 256;

            cfg->others.awb_gain_cfg.gain0_green_b = 256;
            cfg->others.awb_gain_cfg.gain0_green_r = 256;
            cfg->others.awb_gain_cfg.gain0_blue = 256;
            cfg->others.awb_gain_cfg.gain0_red = 256;
            cfg->others.awb_gain_cfg.gain1_green_b = 256;
            cfg->others.awb_gain_cfg.gain1_green_r = 256;
            cfg->others.awb_gain_cfg.gain1_blue = 256;
            cfg->others.awb_gain_cfg.gain1_red = 256;
            cfg->others.awb_gain_cfg.gain2_green_b = 256;
            cfg->others.awb_gain_cfg.gain2_green_r = 256;
            cfg->others.awb_gain_cfg.gain2_blue = 256;
            cfg->others.awb_gain_cfg.gain2_red = 256;
        } else {
            cfg->others.awb_gain_cfg.awb1_gain_gb = 256;
            cfg->others.awb_gain_cfg.awb1_gain_gr = 256;
            cfg->others.awb_gain_cfg.awb1_gain_b = 256;
            cfg->others.awb_gain_cfg.awb1_gain_r = 256;

            cfg->others.awb_gain_cfg.gain0_green_b = 256;
            cfg->others.awb_gain_cfg.gain0_green_r = 256;
            cfg->others.awb_gain_cfg.gain0_blue = 256;
            cfg->others.awb_gain_cfg.gain0_red = 256;
            cfg->others.awb_gain_cfg.gain1_green_b = 256;
            cfg->others.awb_gain_cfg.gain1_green_r = 256;
            cfg->others.awb_gain_cfg.gain1_blue = 256;
            cfg->others.awb_gain_cfg.gain1_red = 256;
            cfg->others.awb_gain_cfg.gain2_green_b = 256;
            cfg->others.awb_gain_cfg.gain2_green_r = 256;
            cfg->others.awb_gain_cfg.gain2_blue = 256;
            cfg->others.awb_gain_cfg.gain2_red = 256;
        }
    }

}

/******************************************************************************
 * HDR PARAMS
 *****************************************************************************/
void set_hdr_params_for_kernel(struct isp32_isp_params_cfg *cfg)
{
    cfg->module_ens |= ISP32_MODULE_HDRMGE;
    cfg->module_en_update |= ISP32_MODULE_HDRMGE;
    cfg->module_cfg_update |= ISP32_MODULE_HDRMGE;

    cfg->others.hdrmge_cfg.mode = 0x1;
    cfg->others.hdrmge_cfg.gain0_inv = 0xfff;
    cfg->others.hdrmge_cfg.gain0 = 0x40;
    cfg->others.hdrmge_cfg.gain1_inv = 0xfff;
    cfg->others.hdrmge_cfg.gain1 = 0x40;
    cfg->others.hdrmge_cfg.gain2 = 0x40;
    cfg->others.hdrmge_cfg.lm_dif_0p15 = 0x26;
    cfg->others.hdrmge_cfg.lm_dif_0p9 = 0xff;
    cfg->others.hdrmge_cfg.ms_diff_0p15 = 0x26;
    cfg->others.hdrmge_cfg.ms_dif_0p8 = 0xff;
    cfg->others.hdrmge_cfg.s_base = 0x0;
    cfg->others.hdrmge_cfg.ms_thd0 = 0x0;
    cfg->others.hdrmge_cfg.ms_thd1 = 0x0;
    cfg->others.hdrmge_cfg.ms_scl = 0x0;
    cfg->others.hdrmge_cfg.lm_thd0 = 0x0;
    cfg->others.hdrmge_cfg.lm_thd1 = 0x0;
    cfg->others.hdrmge_cfg.lm_scl = 0x0;
    cfg->others.hdrmge_cfg.each_raw_en = 0x0;
    cfg->others.hdrmge_cfg.each_raw_gain0 = 0x0;
    cfg->others.hdrmge_cfg.each_raw_gain1 = 0x0;
    cfg->others.hdrmge_cfg.e_y[0] = 0x0;
    cfg->others.hdrmge_cfg.e_y[1] = 0x0;
    cfg->others.hdrmge_cfg.e_y[2] = 0x0;
    cfg->others.hdrmge_cfg.e_y[3] = 0x0;
    cfg->others.hdrmge_cfg.e_y[4] = 0x0;
    cfg->others.hdrmge_cfg.e_y[5] = 0x0;
    cfg->others.hdrmge_cfg.e_y[6] = 0x0;
    cfg->others.hdrmge_cfg.e_y[7] = 0x0;
    cfg->others.hdrmge_cfg.e_y[8] = 0x0;
    cfg->others.hdrmge_cfg.e_y[9] = 0x0;
    cfg->others.hdrmge_cfg.e_y[10] = 0x0;
    cfg->others.hdrmge_cfg.e_y[11] = 0x0;
    cfg->others.hdrmge_cfg.e_y[12] = 0x0;
    cfg->others.hdrmge_cfg.e_y[13] = 0x0;
    cfg->others.hdrmge_cfg.e_y[14] = 0x2;
    cfg->others.hdrmge_cfg.e_y[15] = 0x17;
    cfg->others.hdrmge_cfg.e_y[16] = 0xe4;
    cfg->others.hdrmge_cfg.curve.curve_0[0] = 0x0;
    cfg->others.hdrmge_cfg.curve.curve_0[1] = 0x0;
    cfg->others.hdrmge_cfg.curve.curve_0[2] = 0x0;
    cfg->others.hdrmge_cfg.curve.curve_0[3] = 0x0;
    cfg->others.hdrmge_cfg.curve.curve_0[4] = 0x1;
    cfg->others.hdrmge_cfg.curve.curve_0[5] = 0x4;
    cfg->others.hdrmge_cfg.curve.curve_0[6] = 0xd;
    cfg->others.hdrmge_cfg.curve.curve_0[7] = 0x2b;
    cfg->others.hdrmge_cfg.curve.curve_0[8] = 0x89;
    cfg->others.hdrmge_cfg.curve.curve_0[9] = 0x168;
    cfg->others.hdrmge_cfg.curve.curve_0[10] = 0x29e;
    cfg->others.hdrmge_cfg.curve.curve_0[11] = 0x379;
    cfg->others.hdrmge_cfg.curve.curve_0[12] = 0x3d6;
    cfg->others.hdrmge_cfg.curve.curve_0[13] = 0x3f3;
    cfg->others.hdrmge_cfg.curve.curve_0[14] = 0x3fc;
    cfg->others.hdrmge_cfg.curve.curve_0[15] = 0x3ff;
    cfg->others.hdrmge_cfg.curve.curve_0[16] = 0x3ff;
    cfg->others.hdrmge_cfg.curve.curve_1[0] = 0x0;
    cfg->others.hdrmge_cfg.curve.curve_1[1] = 0x0;
    cfg->others.hdrmge_cfg.curve.curve_1[2] = 0x0;
    cfg->others.hdrmge_cfg.curve.curve_1[3] = 0x0;
    cfg->others.hdrmge_cfg.curve.curve_1[4] = 0x1;
    cfg->others.hdrmge_cfg.curve.curve_1[5] = 0x4;
    cfg->others.hdrmge_cfg.curve.curve_1[6] = 0xd;
    cfg->others.hdrmge_cfg.curve.curve_1[7] = 0x2b;
    cfg->others.hdrmge_cfg.curve.curve_1[8] = 0x89;
    cfg->others.hdrmge_cfg.curve.curve_1[9] = 0x168;
    cfg->others.hdrmge_cfg.curve.curve_1[10] = 0x29e;
    cfg->others.hdrmge_cfg.curve.curve_1[11] = 0x379;
    cfg->others.hdrmge_cfg.curve.curve_1[12] = 0x3d6;
    cfg->others.hdrmge_cfg.curve.curve_1[13] = 0x3f3;
    cfg->others.hdrmge_cfg.curve.curve_1[14] = 0x3fc;
    cfg->others.hdrmge_cfg.curve.curve_1[15] = 0x3ff;
    cfg->others.hdrmge_cfg.curve.curve_1[16] = 0x3ff;

    for(int i = 0; i < 17; i++) {
        cfg->others.hdrmge_cfg.l_raw0[i] = 0x0;
        cfg->others.hdrmge_cfg.l_raw1[i] = 0x0;
    }

}

/******************************************************************************
 * DRC PARAMS
 *****************************************************************************/
void set_drc_params_for_kernel(struct isp32_isp_params_cfg *cfg)
{
    cfg->module_ens |= ISP32_MODULE_DRC;
    cfg->module_en_update |= ISP32_MODULE_DRC;
    cfg->module_cfg_update |= ISP32_MODULE_DRC;

    cfg->others.drc_cfg.bypass_en = 0x0;
    cfg->others.drc_cfg.offset_pow2 = 0x8;
    cfg->others.drc_cfg.compres_scl = 0x1800;
    cfg->others.drc_cfg.position = 0x1000;
    cfg->others.drc_cfg.delta_scalein = 0x40;
    cfg->others.drc_cfg.hpdetail_ratio = 0x0;
    cfg->others.drc_cfg.lpdetail_ratio = 0x0;
    cfg->others.drc_cfg.weicur_pix = 0x60;
    cfg->others.drc_cfg.weipre_frame = 0xcc;
    cfg->others.drc_cfg.bilat_wt_off = 0x0;
    cfg->others.drc_cfg.force_sgm_inv0 = 0x0;
    cfg->others.drc_cfg.motion_scl = 0x0;
    cfg->others.drc_cfg.space_sgm_inv1 = 0xfe4;
    cfg->others.drc_cfg.space_sgm_inv0 = 0xf80;
    cfg->others.drc_cfg.range_sgm_inv1 = 0x666;
    cfg->others.drc_cfg.range_sgm_inv0 = 0x666;
    cfg->others.drc_cfg.weig_maxl = 0x0;
    cfg->others.drc_cfg.weig_bilat = 0x10;
    cfg->others.drc_cfg.enable_soft_thd = 0x1;
    cfg->others.drc_cfg.bilat_soft_thd = 0x265;
    cfg->others.drc_cfg.iir_weight = 0x20;
    cfg->others.drc_cfg.min_ogain = 0x8000;
    cfg->others.drc_cfg.gas_t = 0x0;
    cfg->others.drc_cfg.gas_l0 = 0x18;
    cfg->others.drc_cfg.gas_l1 = 0xa;
    cfg->others.drc_cfg.gas_l2 = 0xa;
    cfg->others.drc_cfg.gas_l3 = 0x5;
    for(int i = 0; i < 17; i++)
        cfg->others.drc_cfg.gain_y[i] = 0x400;

    cfg->others.drc_cfg.compres_y[0] = 0x0;
    cfg->others.drc_cfg.compres_y[1] = 0x155;
    cfg->others.drc_cfg.compres_y[2] = 0x2aa;
    cfg->others.drc_cfg.compres_y[3] = 0x400;
    cfg->others.drc_cfg.compres_y[4] = 0x555;
    cfg->others.drc_cfg.compres_y[5] = 0x6aa;
    cfg->others.drc_cfg.compres_y[6] = 0x800;
    cfg->others.drc_cfg.compres_y[7] = 0x955;
    cfg->others.drc_cfg.compres_y[8] = 0xaaa;
    cfg->others.drc_cfg.compres_y[9] = 0xd55;
    cfg->others.drc_cfg.compres_y[10] = 0x1000;
    cfg->others.drc_cfg.compres_y[11] = 0x12aa;
    cfg->others.drc_cfg.compres_y[12] = 0x1555;
    cfg->others.drc_cfg.compres_y[13] = 0x1800;
    cfg->others.drc_cfg.compres_y[14] = 0x1aaa;
    cfg->others.drc_cfg.compres_y[15] = 0x1d55;
    cfg->others.drc_cfg.compres_y[16] = 0x2000;
    cfg->others.drc_cfg.scale_y[0] = 0x0;
    cfg->others.drc_cfg.scale_y[1] = 0x2;
    cfg->others.drc_cfg.scale_y[2] = 0x14;
    cfg->others.drc_cfg.scale_y[3] = 0x4c;
    cfg->others.drc_cfg.scale_y[4] = 0xc1;
    cfg->others.drc_cfg.scale_y[5] = 0x17d;
    cfg->others.drc_cfg.scale_y[6] = 0x277;
    cfg->others.drc_cfg.scale_y[7] = 0x304;
    cfg->others.drc_cfg.scale_y[8] = 0x397;
    cfg->others.drc_cfg.scale_y[9] = 0x42a;
    cfg->others.drc_cfg.scale_y[10] = 0x4bb;
    cfg->others.drc_cfg.scale_y[11] = 0x5c7;
    cfg->others.drc_cfg.scale_y[12] = 0x6a4;
    cfg->others.drc_cfg.scale_y[13] = 0x747;
    cfg->others.drc_cfg.scale_y[14] = 0x7b0;
    cfg->others.drc_cfg.scale_y[15] = 0x7e8;
    cfg->others.drc_cfg.scale_y[16] = 0x800;

}

int g_rkaiq_isp_hw_ver = 32; //iq parse need!!

/******************************************************************************
 * ISP PARAMS
 *****************************************************************************/
void set_isp_params_for_kernel(struct shared_data *gShare, CamCalibDbV2Context_t* g_main_calib, int is_ie_effect, uint8_t iq_bin_mode)
{
    // 2DNR/TNR/BLC
    NR_BLC_Context_t* pContext = NULL;
    pContext = (NR_BLC_Context_t*)rt_calloc(1, sizeof(NR_BLC_Context_t));
    if (!pContext) {
        rt_kprintf("%s alloc failed\n", __func__);
        return ;
    }

    CalibDbV2_Bayer2dnrV23_t *bayernr_calib = NULL;
    CalibDbV2_BayerTnrV23_t *bayertnr_calib = NULL;
    CalibDbV2_Blc_V32_t *blc_calib = NULL;

    if(iq_bin_mode == FULL_IQ_BIN_MODE || iq_bin_mode == INVALID_IQ_BIN_MODE) {
        bayernr_calib = (CalibDbV2_Bayer2dnrV23_t *)(CALIBDBV2_GET_MODULE_PTR((void *)(g_main_calib), bayer2dnr_v23));
        bayertnr_calib = (CalibDbV2_BayerTnrV23_t *)(CALIBDBV2_GET_MODULE_PTR((void *)(g_main_calib), bayertnr_v23));
        blc_calib = (CalibDbV2_Blc_V32_t *)(CALIBDBV2_GET_MODULE_PTR((void *)(g_main_calib), ablcV32_calib));
    } else if(iq_bin_mode == SIM_IQ_BIN_MODE) {
        bayernr_calib = &g_main_calib->baynr_calib;

        bayertnr_calib = &g_main_calib->baytnr_calib;

        blc_calib = &g_main_calib->blc_calib;
    }

    bayer2dnr_init_params_json_v23(pContext, bayernr_calib, 0, 0);
    bayertnr_init_params_json_v23(pContext, bayertnr_calib, 0, 0);
    blc_init_params_json_v23(pContext, blc_calib);

    if(gShare->hdr_mode == NO_HDR) {
        int sum_gain = FIX_MUL(gShare->exp_gain[0], gShare->exp_isp_dgain[0]);
        calculate_bayertnr(pContext, sum_gain, &gShare->cfg);
        calculate_bayernr(pContext, sum_gain, &gShare->cfg, 0);
        calculate_blc(pContext, sum_gain, &gShare->cfg, gShare->hdr_mode);
    } else {
        int sum_gain1 = FIX_MUL(gShare->exp_gain[1], gShare->exp_isp_dgain[1]);
        int sum_gain0 = FIX_MUL(gShare->exp_gain[0], gShare->exp_isp_dgain[0]);
        float gain_ratio = (float)(FIX_DIV(sum_gain1, sum_gain0) >> 16) + (float)(FIX_DIV(sum_gain1, sum_gain0) & 0xffff) / (float)0xffff;
        calculate_bayertnr(pContext, sum_gain1, &gShare->cfg);
        calculate_bayernr(pContext, sum_gain1, &gShare->cfg, gain_ratio);
        calculate_blc(pContext, sum_gain1, &gShare->cfg, gShare->hdr_mode);
    }

    if(pContext)
        rt_free(pContext);

    // AWB
    CalibDbV2_Wb_Para_V32_t *wb_calib = NULL;
    if(iq_bin_mode == FULL_IQ_BIN_MODE || iq_bin_mode == INVALID_IQ_BIN_MODE)
        wb_calib = (CalibDbV2_Wb_Para_V32_t *)(CALIBDBV2_GET_MODULE_PTR((void *)(g_main_calib), wb_v32));
    else
        wb_calib = &g_main_calib->awb_calib;

    set_awb_params_for_kernel(&gShare->cfg, wb_calib, gShare->hdr_mode, gShare->width, gShare->height);

    // RAWAE/RAWHIST/AWBGAIN/HDR/DRC
    set_rawae_params_for_kernel(&gShare->cfg, gShare->hdr_mode, gShare->width, gShare->height);
    set_rawhist_params_for_kernel(&gShare->cfg, gShare->hdr_mode, gShare->width, gShare->height);
    set_awbgain_params_for_kernel(&gShare->cfg, gShare->hdr_mode, is_ie_effect);

    if(gShare->hdr_mode != NO_HDR) {
        set_hdr_params_for_kernel(&gShare->cfg);
        set_drc_params_for_kernel(&gShare->cfg);
    }

}

/******************************************************************************
 * AE PARSE
 *****************************************************************************/
void parse_ae_params
(
    CamCalibDbV2Context_t     *parse_iq_param,
    struct fastae_init_info   *parse_ae_init,
    uint8_t                    iq_bin_mode
) {
    CalibDb_Aec_ParaV2_t *aec_calib = NULL;
    CalibDb_Sensor_ParaV2_t* sensor_info = NULL;
    int DotLen = 0;

    if(iq_bin_mode == FULL_IQ_BIN_MODE || iq_bin_mode == INVALID_IQ_BIN_MODE) {
        aec_calib = (CalibDb_Aec_ParaV2_t *)(CALIBDBV2_GET_MODULE_PTR((void *)(parse_iq_param), ae_calib));
        sensor_info = parse_iq_param->sensor_info;
    } else {
        aec_calib = &parse_iq_param->ae_calib;
        sensor_info = &parse_iq_param->sensorinfo;
    }

    if (!aec_calib)
        rk_kprintf("----read ae_calib failed!\n");

    parse_ae_init->time_factor[0]       = FLOAT2FIX(sensor_info->Time2Reg.fCoeff[0]);
    parse_ae_init->time_factor[1]       = FLOAT2FIX(sensor_info->Time2Reg.fCoeff[1]);
    parse_ae_init->time_factor[2]       = FLOAT2FIX(sensor_info->Time2Reg.fCoeff[2]);
    parse_ae_init->time_factor[3]       = FLOAT2FIX(sensor_info->Time2Reg.fCoeff[3]);
    if (parse_ae_init->time_factor[2] == 0)
        rk_kprintf("----Exp2Reg C2 is error!%d-%d-%d-%d\n", parse_ae_init->time_factor[0],
                   parse_ae_init->time_factor[1], parse_ae_init->time_factor[2], parse_ae_init->time_factor[3]);

    parse_ae_init->eGainMode = sensor_info->Gain2Reg.GainMode;
    parse_ae_init->gain_range_size = MIN(AE_GAIN_RANGE_NUM, sensor_info->Gain2Reg.GainRange_len / 7);

    for(int i = 0; i < parse_ae_init->gain_range_size; i++) {

        parse_ae_init->gain_range[i].range_min = FLOAT2FIX(sensor_info->Gain2Reg.GainRange[i * 7 + 0]);
        parse_ae_init->gain_range[i].range_max = FLOAT2FIX(sensor_info->Gain2Reg.GainRange[i * 7 + 1]);
        parse_ae_init->gain_range[i].C1        = FLOAT2FIX(sensor_info->Gain2Reg.GainRange[i * 7 + 2]);
        parse_ae_init->gain_range[i].C0        = FLOAT2FIX(sensor_info->Gain2Reg.GainRange[i * 7 + 3]);
        parse_ae_init->gain_range[i].M0        = FLOAT2FIX(sensor_info->Gain2Reg.GainRange[i * 7 + 4]);
        parse_ae_init->gain_range[i].minReg    = FLOAT2FIX(sensor_info->Gain2Reg.GainRange[i * 7 + 5]);
        parse_ae_init->gain_range[i].maxReg    = FLOAT2FIX(sensor_info->Gain2Reg.GainRange[i * 7 + 6]);
        if (parse_ae_init->gain_range[i].C1 == 0)
            rk_kprintf("----Gain2Reg C1 is error!\n");
    }

    //NOTE: here linear/hdr use the same variables
    if(parse_ae_init->is_hdr == false) {
        parse_ae_init->time_range.reg_min = sensor_info->CISTimeSet.Linear.CISTimeRegMin;
        parse_ae_init->time_range.sumreg_fac[0] = FLOAT2FIX(sensor_info->CISTimeSet.Linear.CISLinTimeRegMaxFac.fCoeff[0]);
        parse_ae_init->time_range.sumreg_fac[1] = FLOAT2FIX(sensor_info->CISTimeSet.Linear.CISLinTimeRegMaxFac.fCoeff[1]);
        parse_ae_init->time_range.odevity_fac[0] = sensor_info->CISTimeSet.Linear.CISTimeRegOdevity.fCoeff[0];
        parse_ae_init->time_range.odevity_fac[1] = sensor_info->CISTimeSet.Linear.CISTimeRegOdevity.fCoeff[1];
    } else {
        parse_ae_init->is_ind_gain = sensor_info->CISGainSet.CISHdrGainIndSetEn;
        parse_ae_init->time_range.reg_min = sensor_info->CISTimeSet.Hdr[0].CISTimeRegMin;
        parse_ae_init->time_range.reg_max[0] =  sensor_info->CISTimeSet.Hdr[0].CISTimeRegMax.Coeff[0];
        parse_ae_init->time_range.reg_max[1] =  sensor_info->CISTimeSet.Hdr[0].CISTimeRegMax.Coeff[1];
        parse_ae_init->time_range.sumreg_fac[0] = FLOAT2FIX(sensor_info->CISTimeSet.Hdr[0].CISHdrTimeRegSumFac.fCoeff[0]);
        parse_ae_init->time_range.sumreg_fac[1] = FLOAT2FIX(sensor_info->CISTimeSet.Hdr[0].CISHdrTimeRegSumFac.fCoeff[1]);
        parse_ae_init->time_range.odevity_fac[0] = sensor_info->CISTimeSet.Hdr[0].CISTimeRegOdevity.fCoeff[0];
        parse_ae_init->time_range.odevity_fac[1] = sensor_info->CISTimeSet.Hdr[0].CISTimeRegOdevity.fCoeff[1];
    }

    parse_ae_init->ispdgain_max = FLOAT2FIX(sensor_info->CISGainSet.CISIspDgainRange.Max);

    if (parse_ae_init->time_range.reg_min == 0 || parse_ae_init->time_range.sumreg_fac[0] == 0 || parse_ae_init->time_range.odevity_fac[0] == 0)
        rk_kprintf("----CISTimeRegMin/CISHdrTimeRegSumFac/CISTimeRegOdevity maybe is error!\n");

    parse_ae_init->optype = (aec_calib->CommCtrl.AecOpType == RK_AIQ_OP_MODE_AUTO) ? true : false;

    if(parse_ae_init->is_hdr == false) {

        parse_ae_init->manual_gain[0]       = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.LinearAE.GainValue);
        parse_ae_init->manual_time[0]       = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.LinearAE.TimeValue);
        parse_ae_init->manual_ispgain[0]    = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.LinearAE.IspDGainValue);
        parse_ae_init->tolerance            = FLOAT2FIX(aec_calib->LinearAeCtrl.ToleranceIn);
        parse_ae_init->toleranceOut         = FLOAT2FIX(aec_calib->LinearAeCtrl.ToleranceOut);
        // IspDGainDot
        DotLen = MIN(AE_DOT_NO, aec_calib->LinearAeCtrl.Route.IspDGainDot_len);
        parse_ae_init->ispdgain_max_algo[0] = FLOAT2FIX(aec_calib->LinearAeCtrl.Route.IspDGainDot[DotLen-1]);
        // ExpLevel
        DotLen = MIN(AE_DOT_NO, aec_calib->LinearAeCtrl.DySetpoint.ExpLevel_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->exp_level[i] = FLOAT2FIX(aec_calib->LinearAeCtrl.DySetpoint.ExpLevel[i]);
            else
                parse_ae_init->exp_level[i] = FLOAT2FIX(aec_calib->LinearAeCtrl.DySetpoint.ExpLevel[DotLen-1]);
        }
        // DySetpoint
        DotLen = MIN(AE_DOT_NO, aec_calib->LinearAeCtrl.DySetpoint.DySetpoint_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->setpoint[i] = FLOAT2FIX(aec_calib->LinearAeCtrl.DySetpoint.DySetpoint[i]);
            else
                parse_ae_init->setpoint[i] = FLOAT2FIX(aec_calib->LinearAeCtrl.DySetpoint.DySetpoint[DotLen-1]);
        }
        // TimeDot
        DotLen = MIN(AE_DOT_NO, aec_calib->LinearAeCtrl.Route.TimeDot_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->time_dot[i] = FLOAT2FIX(aec_calib->LinearAeCtrl.Route.TimeDot[i]);
            else
                parse_ae_init->time_dot[i] = FLOAT2FIX(aec_calib->LinearAeCtrl.Route.TimeDot[DotLen-1]);
        }
        // GainDot
        DotLen = MIN(AE_DOT_NO, aec_calib->LinearAeCtrl.Route.GainDot_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->gain_dot[i] = FLOAT2FIX(aec_calib->LinearAeCtrl.Route.GainDot[i]);
            else
                parse_ae_init->gain_dot[i] = FLOAT2FIX(aec_calib->LinearAeCtrl.Route.GainDot[DotLen-1]);
        }

    } else {

        parse_ae_init->manual_gain[0]       = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.GainValue[0]);
        parse_ae_init->manual_gain[1]       = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.GainValue[1]);
        parse_ae_init->manual_gain[2]       = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.GainValue[2]);
        parse_ae_init->manual_time[0]       = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.TimeValue[0]);
        parse_ae_init->manual_time[1]       = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.TimeValue[1]);
        parse_ae_init->manual_time[2]       = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.TimeValue[2]);
        parse_ae_init->manual_ispgain[0]    = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.IspDGainValue[0]);
        parse_ae_init->manual_ispgain[1]    = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.IspDGainValue[1]);
        parse_ae_init->manual_ispgain[2]    = FLOAT2FIX(aec_calib->CommCtrl.AecManualCtrl.HdrAE.IspDGainValue[2]);
        parse_ae_init->tolerance            = FLOAT2FIX(aec_calib->HdrAeCtrl.ToleranceIn);
        parse_ae_init->toleranceOut         = FLOAT2FIX(aec_calib->HdrAeCtrl.ToleranceOut);
        // IspDGainDot
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.Route.Frm0IspDGainDot_len);
        parse_ae_init->ispdgain_max_algo[0] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm0IspDGainDot[DotLen-1]);
        parse_ae_init->ispdgain_max_algo[1] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm1IspDGainDot[DotLen-1]);
        parse_ae_init->ispdgain_max_algo[2] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm2IspDGainDot[DotLen-1]);
        // LExpLevel
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.LExpLevel_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->lexp_level[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.LExpLevel[i]);
            else
                parse_ae_init->lexp_level[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.LExpLevel[DotLen-1]);
        }
        // LSetPoint
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.LSetPoint_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->lsetpoint[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.LSetPoint[i]);
            else
                parse_ae_init->lsetpoint[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.LSetPoint[DotLen-1]);
        }
        // TargetLLLuma
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.TargetLLLuma_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->darksetpoint[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.TargetLLLuma[i]);
            else
                parse_ae_init->darksetpoint[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.LframeCtrl.LfrmSetPoint.TargetLLLuma[DotLen-1]);
        }
        // Frm1TimeDot
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.Route.Frm1TimeDot_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->ltime_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm1TimeDot[i]);
            else
                parse_ae_init->ltime_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm1TimeDot[DotLen-1]);
        }
        // Frm1GainDot
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.Route.Frm1GainDot_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->lgain_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm1GainDot[i]);
            else
                parse_ae_init->lgain_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm1GainDot[DotLen-1]);
        }
        // Frm0TimeDot
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.Route.Frm0TimeDot_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->stime_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm0TimeDot[i]);
            else
                parse_ae_init->stime_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm0TimeDot[DotLen-1]);
        }
        // Frm0GainDot
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.Route.Frm0GainDot_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->sgain_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm0GainDot[i]);
            else
                parse_ae_init->sgain_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.Route.Frm0GainDot[DotLen-1]);
        }
        // RatioExpDot
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.ExpRatioCtrl.ExpRatio.RatioExpDot_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->exp_ratio_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.ExpRatioCtrl.ExpRatio.RatioExpDot[i]);
            else
                parse_ae_init->exp_ratio_dot[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.ExpRatioCtrl.ExpRatio.RatioExpDot[DotLen-1]);
        }
        // M2SRatioFix
        DotLen = MIN(AE_DOT_NO, aec_calib->HdrAeCtrl.ExpRatioCtrl.ExpRatio.M2SRatioFix_len);
        for (int i = 0; i < AE_DOT_NO; i++) {
            if (i < DotLen)
                parse_ae_init->exp_ratio[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.ExpRatioCtrl.ExpRatio.M2SRatioFix[i]);
            else
                parse_ae_init->exp_ratio[i] = FLOAT2FIX(aec_calib->HdrAeCtrl.ExpRatioCtrl.ExpRatio.M2SRatioFix[DotLen-1]);
        }

    }

    parse_ae_init->flicker_enable       = aec_calib->CommCtrl.AecAntiFlicker.enable;
    parse_ae_init->flicker_select       = aec_calib->CommCtrl.AecAntiFlicker.Frequency;
    parse_ae_init->env_calib[0]         = FLOAT2FIX(aec_calib->CommCtrl.AecEnvLvCalib.CurveCoeff[0]);
    parse_ae_init->env_calib[1]         = FLOAT2FIX(aec_calib->CommCtrl.AecEnvLvCalib.CurveCoeff[1]);
    if (parse_ae_init->env_calib[0] <= 0 || parse_ae_init->env_calib[1] <= 0)
        rk_kprintf("----AecEnvLvCalib is error!\n");

    parse_ae_init->is_fps_fix           = aec_calib->CommCtrl.AecFrameRateMode.isFpsFix;
    parse_ae_init->iq_fps_value         = FLOAT2FIX(aec_calib->CommCtrl.AecFrameRateMode.FpsValue);
    parse_ae_init->cis_min_fps          = FLOAT2FIX(sensor_info->CISMinFps);

    for(int i = 0; i < AE_EXP_GRID_ITEMS_NUM; i++) {
        parse_ae_init->AecGridWeight[i] = MAX(INT2FIX(aec_calib->CommCtrl.AecGridWeight[i]), FIX_ONE);
    }

    //NOTE: here linear/hdr use the same variables
    if(parse_ae_init->is_hdr == false) {
        parse_ae_init->dcg_cfg.support_en = (sensor_info->CISDcgSet.Linear.support_en) ? true : false;
        parse_ae_init->dcg_cfg.dcg_optype = (sensor_info->CISDcgSet.Linear.dcg_optype == RK_AIQ_OP_MODE_AUTO) ? true : false;
        parse_ae_init->dcg_cfg.sync_switch = (sensor_info->CISDcgSet.Linear.sync_switch) ? true : false;
        parse_ae_init->dcg_cfg.dcg_mode[0] = sensor_info->CISDcgSet.Linear.dcg_mode.Coeff[0] + 1;
        parse_ae_init->dcg_cfg.dcg_ratio = FLOAT2FIX(sensor_info->CISDcgSet.Linear.dcg_ratio);
        parse_ae_init->dcg_cfg.lcg2hcg_gain_th = FLOAT2FIX(sensor_info->CISDcgSet.Linear.lcg2hcg_gain_th);
        parse_ae_init->dcg_cfg.hcg2lcg_gain_th = FLOAT2FIX(sensor_info->CISDcgSet.Linear.hcg2lcg_gain_th);
    } else {
        parse_ae_init->dcg_cfg.support_en = (sensor_info->CISDcgSet.Hdr.support_en) ? true : false;
        parse_ae_init->dcg_cfg.dcg_optype = (sensor_info->CISDcgSet.Hdr.dcg_optype == RK_AIQ_OP_MODE_AUTO) ? true : false;
        parse_ae_init->dcg_cfg.sync_switch = (sensor_info->CISDcgSet.Hdr.sync_switch) ? true : false;
        parse_ae_init->dcg_cfg.dcg_mode[0] = sensor_info->CISDcgSet.Hdr.dcg_mode.Coeff[0] + 1;
        parse_ae_init->dcg_cfg.dcg_mode[1] = sensor_info->CISDcgSet.Hdr.dcg_mode.Coeff[1] + 1;
        parse_ae_init->dcg_cfg.dcg_mode[2] = sensor_info->CISDcgSet.Hdr.dcg_mode.Coeff[2] + 1;
        parse_ae_init->dcg_cfg.dcg_ratio = FLOAT2FIX(sensor_info->CISDcgSet.Hdr.dcg_ratio);
        parse_ae_init->dcg_cfg.lcg2hcg_gain_th = FLOAT2FIX(sensor_info->CISDcgSet.Hdr.lcg2hcg_gain_th);
        parse_ae_init->dcg_cfg.hcg2lcg_gain_th = FLOAT2FIX(sensor_info->CISDcgSet.Hdr.hcg2lcg_gain_th);
    }

}

/******************************************************************************
 * baynr setting
 *****************************************************************************/
void set_baynr_params_for_rtt(struct isp_baynr_cfg* pbaynr)
{
    pbaynr->bay3d_gain_en = 0x1;
    pbaynr->lg2_mode = 0x0;
    pbaynr->gauss_en = 0x0;
    pbaynr->log_bypass = 0x0;
    //pbaynr->en=0x1;

    pbaynr->dgain0 = 0x0;
    pbaynr->dgain1 = 0x0;
    pbaynr->dgain2 = 0x0;

    pbaynr->pix_diff = 0x3fff;
    pbaynr->diff_thld = 0x3ff;
    pbaynr->softthld = 0x0;

    pbaynr->bltflt_streng = 0x0;
    pbaynr->reg_w1 = 0x200;

    pbaynr->sigma_x[0] = 0x200;
    pbaynr->sigma_x[1] = 0x400;
    pbaynr->sigma_x[2] = 0x600;
    pbaynr->sigma_x[3] = 0x800;
    pbaynr->sigma_x[4] = 0xc00;
    pbaynr->sigma_x[5] = 0x1000;
    pbaynr->sigma_x[6] = 0x1400;
    pbaynr->sigma_x[7] = 0x1800;
    pbaynr->sigma_x[8] = 0x1c00;
    pbaynr->sigma_x[9] = 0x2000;
    pbaynr->sigma_x[10] = 0x2400;
    pbaynr->sigma_x[11] = 0x2800;
    pbaynr->sigma_x[12] = 0x2c00;
    pbaynr->sigma_x[13] = 0x3000;
    pbaynr->sigma_x[14] = 0x3400;
    pbaynr->sigma_x[15] = 0x3800;

    pbaynr->sigma_y[0] = 0x62;
    pbaynr->sigma_y[1] = 0x5f;
    pbaynr->sigma_y[2] = 0x5c;
    pbaynr->sigma_y[3] = 0x59;
    pbaynr->sigma_y[4] = 0x55;
    pbaynr->sigma_y[5] = 0x52;
    pbaynr->sigma_y[6] = 0x50;
    pbaynr->sigma_y[7] = 0x50;
    pbaynr->sigma_y[8] = 0x50;
    pbaynr->sigma_y[9] = 0x51;
    pbaynr->sigma_y[10] = 0x51;
    pbaynr->sigma_y[11] = 0x50;
    pbaynr->sigma_y[12] = 0x4f;
    pbaynr->sigma_y[13] = 0x4f;
    pbaynr->sigma_y[14] = 0x4f;
    pbaynr->sigma_y[15] = 0x4f;

    pbaynr->weit_d0 = 0x12;
    pbaynr->weit_d1 = 0x8a;
    pbaynr->weit_d2 = 0x178;

    pbaynr->lg2_lgoff = 0x8000;
    pbaynr->lg2_off = 0x100;

    pbaynr->dat_max = 0xfffff;

    pbaynr->bgain_off = 0x0;
    pbaynr->rgain_off = 0x0;

    pbaynr->gain_x[0] = 0x10;
    pbaynr->gain_x[1] = 0x20;
    pbaynr->gain_x[2] = 0x30;
    pbaynr->gain_x[3] = 0x40;
    pbaynr->gain_x[4] = 0x50;
    pbaynr->gain_x[5] = 0x60;
    pbaynr->gain_x[6] = 0x70;
    pbaynr->gain_x[7] = 0x80;
    pbaynr->gain_x[8] = 0x90;
    pbaynr->gain_x[9] = 0xa0;
    pbaynr->gain_x[10] = 0xb0;
    pbaynr->gain_x[11] = 0xc0;
    pbaynr->gain_x[12] = 0xd0;
    pbaynr->gain_x[13] = 0xe0;
    pbaynr->gain_x[14] = 0xf0;
    pbaynr->gain_x[15] = 0xff;
    pbaynr->gain_y[0] = 0x2;
    pbaynr->gain_y[1] = 0x4;
    pbaynr->gain_y[2] = 0x20;
    pbaynr->gain_y[3] = 0x60;
    pbaynr->gain_y[4] = 0xa0;
    pbaynr->gain_y[5] = 0xc0;
    pbaynr->gain_y[6] = 0xe0;
    pbaynr->gain_y[7] = 0x100;
    pbaynr->gain_y[8] = 0x120;
    pbaynr->gain_y[9] = 0x140;
    pbaynr->gain_y[10] = 0x160;
    pbaynr->gain_y[11] = 0x180;
    pbaynr->gain_y[12] = 0x1a0;
    pbaynr->gain_y[13] = 0x1c0;
    pbaynr->gain_y[14] = 0x1e0;
    pbaynr->gain_y[15] = 0x200;

}

/******************************************************************************
 * bay3d setting
 *****************************************************************************/
void set_bay3d_params_for_rtt(struct isp_bay3d_cfg* pbay3d, uint32_t w, uint32_t h)
{
    uint32_t cur_size, iir_size, ds_size, wrap_line, wsize, div;
    pbay3d->higaus_bypass_en = 0x1;
    pbay3d->himed_bypass_en = 0x1;
    pbay3d->lobypass_en = 0x0;
    pbay3d->hibypass_en = 0x0;
    pbay3d->bypass_en = 0x0;
    //pbay3d->en=0x1;

    pbay3d->logaus3_bypass_en = 0x0;
    pbay3d->logaus5_bypass_en = 0x0;
    pbay3d->lomed_bypass_en = 0x0;
    pbay3d->hichnsplit_en = 0x0;
    pbay3d->hiabs_possel = 0x0;

    pbay3d->bwsaving_en = 0x1;
    pbay3d->loswitch_protect = 0x0;
    pbay3d->glbpk_en = 0x0;

    pbay3d->hiwgt_opt_en = 0x1;
    pbay3d->hichncor_en = 0x1;
    pbay3d->bwopt_gain_dis = 0x0;
    pbay3d->lo4x8_en = 0x0;
    pbay3d->lo4x4_en = 0x1;
    pbay3d->hisig_ind_sel = 0x0;

    pbay3d->pksig_ind_sel = 0x0;
    pbay3d->iirwr_rnd_en = 0x1;
    pbay3d->curds_high_en = 0x0;
    pbay3d->higaus3_mode = 0x1;
    pbay3d->higaus5x5_en = 0x1;
    pbay3d->wgtmix_opt_en = 0x1;

    pbay3d->siggaus0 = 0x10;
    pbay3d->siggaus1 = 0x8;
    pbay3d->siggaus2 = 0x10;
    pbay3d->siggaus3 = 0x8;

    pbay3d->softwgt = 0x0;
    pbay3d->hidif_th = 0xffff;

    pbay3d->wgtlmt = 0x399;
    pbay3d->wgtratio = 0xa3;

    pbay3d->sig0_x[0] = 0x200;
    pbay3d->sig0_x[1] = 0x400;
    pbay3d->sig0_x[2] = 0x600;
    pbay3d->sig0_x[3] = 0x800;
    pbay3d->sig0_x[4] = 0xc00;
    pbay3d->sig0_x[5] = 0x1000;
    pbay3d->sig0_x[6] = 0x1400;
    pbay3d->sig0_x[7] = 0x1800;
    pbay3d->sig0_x[8] = 0x1c00;
    pbay3d->sig0_x[9] = 0x2000;
    pbay3d->sig0_x[10] = 0x2400;
    pbay3d->sig0_x[11] = 0x2800;
    pbay3d->sig0_x[12] = 0x2c00;
    pbay3d->sig0_x[13] = 0x3000;
    pbay3d->sig0_x[14] = 0x3400;
    pbay3d->sig0_x[15] = 0x3800;

    pbay3d->sig0_y[0] = 0x5d;
    pbay3d->sig0_y[1] = 0x6a;
    pbay3d->sig0_y[2] = 0x73;
    pbay3d->sig0_y[3] = 0x79;
    pbay3d->sig0_y[4] = 0x80;
    pbay3d->sig0_y[5] = 0x82;
    pbay3d->sig0_y[6] = 0x80;
    pbay3d->sig0_y[7] = 0x7c;
    pbay3d->sig0_y[8] = 0x75;
    pbay3d->sig0_y[9] = 0x6e;
    pbay3d->sig0_y[10] = 0x67;
    pbay3d->sig0_y[11] = 0x5f;
    pbay3d->sig0_y[12] = 0x58;
    pbay3d->sig0_y[13] = 0x57;
    pbay3d->sig0_y[14] = 0x57;
    pbay3d->sig0_y[15] = 0x57;

    pbay3d->sig1_x[0] = 0x200;
    pbay3d->sig1_x[1] = 0x400;
    pbay3d->sig1_x[2] = 0x600;
    pbay3d->sig1_x[3] = 0x800;
    pbay3d->sig1_x[4] = 0xc00;
    pbay3d->sig1_x[5] = 0x1000;
    pbay3d->sig1_x[6] = 0x1400;
    pbay3d->sig1_x[7] = 0x1800;
    pbay3d->sig1_x[8] = 0x1c00;
    pbay3d->sig1_x[9] = 0x2000;
    pbay3d->sig1_x[10] = 0x2400;
    pbay3d->sig1_x[11] = 0x2800;
    pbay3d->sig1_x[12] = 0x2c00;
    pbay3d->sig1_x[13] = 0x3000;
    pbay3d->sig1_x[14] = 0x3400;
    pbay3d->sig1_x[15] = 0x3800;

    pbay3d->sig1_y[0] = 0x100;
    pbay3d->sig1_y[1] = 0x100;
    pbay3d->sig1_y[2] = 0x100;
    pbay3d->sig1_y[3] = 0x100;
    pbay3d->sig1_y[4] = 0x100;
    pbay3d->sig1_y[5] = 0x100;
    pbay3d->sig1_y[6] = 0x100;
    pbay3d->sig1_y[7] = 0x100;
    pbay3d->sig1_y[8] = 0x100;
    pbay3d->sig1_y[9] = 0x100;
    pbay3d->sig1_y[10] = 0x100;
    pbay3d->sig1_y[11] = 0x100;
    pbay3d->sig1_y[12] = 0x100;
    pbay3d->sig1_y[13] = 0x100;
    pbay3d->sig1_y[14] = 0x100;
    pbay3d->sig1_y[15] = 0x100;

    pbay3d->sig2_y[0] = 0x20;
    pbay3d->sig2_y[1] = 0x20;
    pbay3d->sig2_y[2] = 0x20;
    pbay3d->sig2_y[3] = 0x20;
    pbay3d->sig2_y[4] = 0x20;
    pbay3d->sig2_y[5] = 0x20;
    pbay3d->sig2_y[6] = 0x20;
    pbay3d->sig2_y[7] = 0x20;
    pbay3d->sig2_y[8] = 0x20;
    pbay3d->sig2_y[9] = 0x20;
    pbay3d->sig2_y[10] = 0x20;
    pbay3d->sig2_y[11] = 0x20;
    pbay3d->sig2_y[12] = 0x20;
    pbay3d->sig2_y[13] = 0x20;
    pbay3d->sig2_y[14] = 0x20;
    pbay3d->sig2_y[15] = 0x20;

    pbay3d->hisigrat0 = 0x33;
    pbay3d->hisigrat1 = 0x100;

    pbay3d->hisigoff0 = 0x0;
    pbay3d->hisigoff1 = 0x0;

    pbay3d->losigoff = 0x0;
    pbay3d->losigrat = 0x100;

    pbay3d->rgain_off = 0x0;
    pbay3d->bgain_off = 0x0;

    pbay3d->glbpk2 = 0x400;

    //calc nr buf size
    bool is_hdr = false;
    bool is_bwsaving = pbay3d->bwsaving_en;
    bool is_glbpk = !!pbay3d->glbpk_en;
    bool is_bwopt_dis = !!pbay3d->bwopt_gain_dis;
    bool is_predgain = false;//isp_bls_cfg ISP32_BLS_ISP_OB_PREDGAIN=0
    bool is_lo8x8 = !pbay3d->lo4x8_en && !pbay3d->lo4x4_en;

    w = (w + 15) / 16 * 16;
    h = (h + 15) / 16 * 16;

    /* bay3d iir buf size */
    wsize = is_bwopt_dis ? w : w * 2;
    if (is_bwsaving)
        wsize = wsize * 3 / 4;
    if (!is_glbpk)
        wsize += w / 8;
    wsize *= 2;
    div = is_bwopt_dis ? 1 : 2;
    iir_size = (wsize * h / div + 15) / 16 * 16;

    /* bay3d ds buf size */
    div = is_lo8x8 ? 64 : 16;
    ds_size = w * h / div;
    ds_size = (ds_size * 2 + 15) / 16 * 16;

    /* bay3d cur buf size */
    wrap_line = is_lo8x8 ? 76 : 36;
    wsize = is_bwopt_dis ? w : w * 2;
    if (is_bwsaving)
        wsize = wsize * 3 / 4;
    if (is_hdr || is_predgain)
        wsize += w / 8;
    wsize = (wsize * 2 + 15) / 16 * 16;
    div = is_bwopt_dis ? 1 : 2;
    cur_size = (wsize * wrap_line / div + 15) / 16 * 16;

    pbay3d->wsize = wsize;
    pbay3d->iir_size = iir_size;
    pbay3d->cur_size = cur_size;
    pbay3d->ds_size = ds_size;
    pbay3d->wrap_line = wrap_line;

    //rt_kprintf("wsize=%d,iir_size=%d,cur_size=%d,ds_size=%d,wrap_line=%d\n",wsize,iir_size,cur_size,ds_size,wrap_line);

}

#ifdef RT_USING_RK_AOV

void get_tnr_sigma(CalibDbV2_BayerTnrV23_Calib_t* pCalibdb, int calib_idx, int iso, int meanluma, struct fastae_md_param* md_param) {

    CalibDbV2_BayerTnrV23_C_ISO_t *pCalibIso1 = NULL;
    CalibDbV2_BayerTnrV23_C_ISO_t *pCalibIso2 = NULL;
    uint8_t i = 0;
    float isoratio = 0;

    // 0) select ISO
    for (i = 0; i < RK_BAYERNR_V23_MAX_ISO_NUM - 1; i++) {
        if (iso >= pCalibdb->Setting[calib_idx].Calib_ISO[i].iso && iso <= pCalibdb->Setting[calib_idx].Calib_ISO[i + 1].iso) {
            isoratio = (iso - pCalibdb->Setting[calib_idx].Calib_ISO[i].iso)\
                       / (pCalibdb->Setting[calib_idx].Calib_ISO[i + 1].iso - pCalibdb->Setting[calib_idx].Calib_ISO[i].iso);
            break;
        }
    }

    // 1) select sigma according to meanluma
    pCalibIso1 = &pCalibdb->Setting[calib_idx].Calib_ISO[i];
    pCalibIso2 = &pCalibdb->Setting[calib_idx].Calib_ISO[MIN(i + 1, RK_BAYERNR_V23_MAX_ISO_NUM - 1)];

    int lindot[18] = {1, 10, 24, 50, 76, 106, 175, 256, 353, 468, 605, 768, 962, 1194, 1465, 1791, 2206, 2651}; // 12bit input

    int logdot[18] = {23, 226, 512, 1024, 1536, 2048, 3072, 4096, 5120, 6144,
                      7168, 8192, 9216, 10240, 11264, 12288, 13312, 14336
                     };  //16741, default lumapoint

    // 1.1) meanluma => linear2log => sigma
    uint32_t log_losigma = 0;
    uint32_t log_hisigma = 0;
    uint8_t j = 0;
    float lumaratio = 0;

    if(meanluma < lindot[2]) {
        j = 0;
        lumaratio = 0;
    } else if (meanluma >= lindot[17]) {
        j = 15;
        lumaratio = 1;
    } else {
        for(j = 0; j < 14; j++) {
            if(meanluma >= lindot[j + 2] && meanluma < lindot[j + 2 + 1]) {
                lumaratio = (meanluma - lindot[j + 2]) / (lindot[j + 2 + 1] - lindot[j + 2]);
                break;
            }
        }
    }

    int log_losigma1 = 0, log_losigma2 = 0, log_hisigma1 = 0, log_hisigma2 = 0;
    log_losigma1 = pCalibIso1->lo_sigma[j] + isoratio * (pCalibIso2->lo_sigma[j] - pCalibIso1->lo_sigma[j]);
    log_losigma2 = pCalibIso1->lo_sigma[MIN(j + 1, 15)] + isoratio * (pCalibIso2->lo_sigma[MIN(j + 1, 15)] - pCalibIso1->lo_sigma[MIN(j + 1, 15)]);
    log_losigma = log_losigma1 + lumaratio * (log_losigma2 - log_losigma1);
    log_hisigma1 = pCalibIso1->hi_sigma[j] + isoratio * (pCalibIso2->hi_sigma[j] - pCalibIso1->hi_sigma[j]);
    log_hisigma2 = pCalibIso1->hi_sigma[MIN(j + 1, 15)] + isoratio * (pCalibIso2->hi_sigma[MIN(j + 1, 15)] - pCalibIso1->hi_sigma[MIN(j + 1, 15)]);
    log_hisigma = log_hisigma1 + lumaratio * (log_hisigma2 - log_hisigma1);

    //rt_kprintf("iso=%d,log_losigma=%d,log_hisigma=%d,isoidx=%d,lumaidx=%d\n",iso,log_losigma,log_hisigma,i,j);
    md_param->log_hisigma = log_hisigma;
    md_param->log_losigma = log_losigma;

    // 1.2) sigma => log2linear

    if(log_losigma < logdot[0]) {
        md_param->lo_sigma = lindot[0];
    } else if (log_losigma >= logdot[17]) {
        md_param->lo_sigma = lindot[17];
    } else {
        for(i = 0; i < 17; i++) {
            if(log_losigma >= logdot[i] && log_losigma < logdot[i + 1]) {
                md_param->lo_sigma = lindot[i] + (lindot[i + 1] - lindot[i]) * (log_losigma - logdot[i]) / (logdot[i + 1] - log_losigma);
                break;
            }
        }
    }

    if(log_hisigma < logdot[0]) {
        md_param->hi_sigma = lindot[0];
    } else if (log_hisigma >= logdot[17]) {
        md_param->hi_sigma = lindot[17];
    } else {
        for(i = 0; i < 17; i++) {
            if(log_hisigma >= logdot[i] && log_hisigma < logdot[i + 1]) {
                md_param->hi_sigma = lindot[i] + (lindot[i + 1] - lindot[i]) * (log_hisigma - logdot[i]) / (logdot[i + 1] - log_hisigma);
                break;
            }
        }
    }


}

#endif
