/*
 *  Copyright (c) 2019 Rockchip Corporation
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

#ifndef RK_AIQ_USER_API_SYSCTL_H
#define RK_AIQ_USER_API_SYSCTL_H

#include "rk_aiq.h"
#include "rk_aiq_algo_des.h"

RKAIQ_BEGIN_DECLARE

typedef struct rk_aiq_sys_ctx_s rk_aiq_sys_ctx_t;

/*!
 * \brief pre-settings before init
 *
 * \param[in] sns_ent_name    active sensor media entity name. This represents the unique camera module
 *                            in system. And the whole active pipeline could be retrieved by this.
 * \param[in] mode            pipleline working mode, just used by \ref
 *                            rk_aiq_uapi_sysctl_init to select iq file
 *                            according to hdr mode. Unused if params \ref
 *                            iq_file is specified.
 * \param[in] iq_file         optional, forcely used this iq file
 * \note Optional API, should be called before\ref rk_aiq_uapi_sysctl_init. This
 *       API extends the functionality of \ref rk_aiq_uapi_sysctl_init. And just
 *       used to help select the satisfied iq file when multiple iq files
 *       exsist.
 */
XCamReturn
rk_aiq_uapi_sysctl_preInit(const char* sns_ent_name,
                           rk_aiq_working_mode_t mode,
                           const char* force_iq_file);

/*!
 * \brief set readback mode on/off
 * \note if sensor is linked to vicap, aiq will only work on readback mode, this api is unvalid;
 * only if sensor is linked to isp, you can set isp to work on no_readback mode;
 * should be called after init and before prepare!
 * \param[in] on            bool flag for mode on/off
 *
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_setReadBackMode(rk_aiq_sys_ctx_t* ctx, bool on);

/*!
 * \brief initialze aiq control system context
 * Should call before any other APIs
 *
 * \param[in] sns_ent_name    active sensor media entity name. This represents the unique camera module\n
 *                            in system. And the whole active pipeline could be retrieved by this.
 * \param[in] iq_file_dir     define the search directory of the iq files.
 * \param[in] err_cb          not mandatory. it's used to return system errors to user.
 * \param[in] metas_cb        not mandatory. it's used to return the metas(sensor exposure settings,\n
 *                            isp settings, etc.) for each frame
 * \return return system context if success, or NULL if failure.
 */
rk_aiq_sys_ctx_t*
rk_aiq_uapi_sysctl_init(const char* sns_ent_name,
                        const char* iq_file_dir,
                        rk_aiq_error_cb err_cb,
                        rk_aiq_metas_cb metas_cb);

/*!
 * \brief deinitialze aiq context
 * Should not be called in started state
 *
 * \param[in] ctx             the context returned by \ref rk_aiq_uapi_sysctl_init
 */
void
rk_aiq_uapi_sysctl_deinit( rk_aiq_sys_ctx_t* ctx);

/*!
 * \brief set raw buffer total number, should be called after init and before prepare!
 *
 * \param[in] buf_num        the buffer number to set
 */
void rk_aiq_uapi_setRawBufNum(rk_aiq_sys_ctx_t* ctx, uint16_t buf_num);

/*!
 * \brief prepare aiq control system before runninig
 * prepare AIQ running enviroment, should be called before \ref rk_aiq_uapi_sysctl_start.\n
 * And if re-prepared is required after \ref rk_aiq_uapi_sysctl_start is called,\n
 * should call \ref rk_aiq_uapi_sysctl_stop firstly.
 *
 * \param[in] ctx             the context returned by \ref rk_aiq_uapi_sysctl_init
 * \param[in] width           sensor active output width, just used to check internally
 * \param[in] height          sensor active output height, just used to check internally
 * \param[in] mode            pipleline working mode
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_prepare(const rk_aiq_sys_ctx_t* ctx,
                           uint32_t  width, uint32_t  height,
                           rk_aiq_working_mode_t mode);

/*!
 * \brief start aiq control system
 * should be called after \ref rk_aiq_uapi_sysctl_prepare. After this call,
 * the aiq system repeats getting 3A statistics from ISP driver, running
 * aiq algorimths(AE, AWB, AF, etc.), setting new parameters to drivers.
 *
 * \param[in] ctx             the context returned by \ref rk_aiq_uapi_sysctl_init
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_start(const rk_aiq_sys_ctx_t* ctx);

/*!
 * \brief stop aiq control system
 *
 * \param[in] ctx             the context returned by \ref rk_aiq_uapi_sysctl_init
 * \param[in] keep_ext_hw_st  do not change external devices status, like ircut/cpsl
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_stop(const rk_aiq_sys_ctx_t* ctx, bool keep_ext_hw_st);

XCamReturn
rk_aiq_uapi_sysctl_getStaticMetas(const char* sns_ent_name, rk_aiq_static_info_t* static_info);

/*!
 * \brief enum static camera infos
 *
 * \param[in] index           which camera info will be enum
 * \param[out] static_info    returned camera infos
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_enumStaticMetas(int index, rk_aiq_static_info_t* static_info);

/*!
 * \brief get sensor entity name from video node
 * \param[in] vd             pp stream video node full path
 * \return return the binded sensor name
 */
const char*
rk_aiq_uapi_sysctl_getBindedSnsEntNmByVd(const char* vd);

XCamReturn
rk_aiq_uapi_sysctl_getMetaData(const rk_aiq_sys_ctx_t* ctx, uint32_t frame_id, rk_aiq_metas_t* metas);

#if 0
int32_t
rk_aiq_uapi_sysctl_getState(const rk_aiq_sys_ctx_t* ctx);
#endif

XCamReturn
rk_aiq_uapi_sysctl_setModuleCtl(const rk_aiq_sys_ctx_t* ctx, rk_aiq_module_id_t mId, bool mod_en);

int32_t
rk_aiq_uapi_sysctl_getModuleCtl(const rk_aiq_sys_ctx_t* ctx, rk_aiq_module_id_t mId, bool *mod_en);

/*!
 * \brief register customized algo lib
 *
 * \param[in] ctx context
 * \param[in,out] algo_lib_des allocate a new unique id value for algo_lib_des->id if success.\n
 *                             this id could be used in \ref rk_aiq_uapi_sysctl_unRegLib
 *                             or other lib APIs. The deference object by
 *                             \ref algo_lib_des should be valid until \ref rk_aiq_uapi_sysctl_unRegLib
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_regLib(const rk_aiq_sys_ctx_t* ctx,
                          RkAiqAlgoDesComm* algo_lib_des);

/*!
 * \brief unregister customized algo lib
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \param[in] lib_id          returned by call \ref rk_aiq_uapi_sysctl_regLib
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_unRegLib(const rk_aiq_sys_ctx_t* ctx,
                            const int algo_type,
                            const int lib_id);

/*!
 * \brief enable or disable algo lib
 * If the \ref lib_id is the same as the current running algo, this interface
 * could be called in any state except for the context uninitialized. Otherwise,
 * it could only be called in prepared or initialized state followed by
 * call \ref rk_aiq_uapi_sysctl_prepare , and in this case, the old algo which type
 * is \ref algo_type will be replaced by the new algo \ref lib_id.
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \param[in] lib_id          0 for system integrated algos;\n
 *                            returned by call \ref rk_aiq_uapi_sysctl_regLib for customer algos
 * \param[in] enable          enable or disable algos. enable means running the algo's processes\n
 *                            defined in \ref RkAiqAlgoDesComm; disable means\n
 *                            bypass the algo's prcosses.
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_enableAxlib(const rk_aiq_sys_ctx_t* ctx,
                               const int algo_type,
                               const int lib_id,
                               bool enable);

/*!
 * \brief get algo lib enabled status
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \param[in] lib_id          0 for system integrated algos;\n
 *                            returned by call \ref rk_aiq_uapi_sysctl_regLib for customer algos
 * \return                    return true if enabled , false if disabled or unregistered
 */
bool
rk_aiq_uapi_sysctl_getAxlibStatus(const rk_aiq_sys_ctx_t* ctx,
                                  const int algo_type,
                                  const int lib_id);

/*!
 * \brief get enabled algo lib context
 * The returned algo context will be used as the first param of algo private APIs.
 * For those customer's algo lib, this interface should be called after
 * \ref rk_aiq_uapi_sysctl_enableAxlib, or will return NULL.
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \return return NULL if failed
 */
RkAiqAlgoContext*
rk_aiq_uapi_sysctl_getEnabledAxlibCtx(const rk_aiq_sys_ctx_t* ctx, const int algo_type);

/*!
 * \brief get algo lib context
 * The returned algo context will be used as the first param of algo private APIs.
 * For those customer's algo lib, this interface should be called after
 * \ref rk_aiq_uapi_sysctl_regLib, or will return NULL.
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \param[in] lib_id          0 for system integrated algos;\n
 *                            returned by call \ref rk_aiq_uapi_sysctl_regLib for customer algos
 * \return return NULL if failed
 */
RkAiqAlgoContext*
rk_aiq_uapi_sysctl_getAxlibCtx(const rk_aiq_sys_ctx_t* ctx, const int algo_type, const int lib_id);

/*!
 * \brief get 3a stats
 *
 * \param[in] ctx             context
 * \param[out] stats          stats params
 * \return return 0 if success
 * \note non-blocked interface, and copy the result to stats.
 */
XCamReturn
rk_aiq_uapi_sysctl_get3AStats(const rk_aiq_sys_ctx_t* ctx,
                              rk_aiq_isp_stats_t *stats);

/*!
 * \brief get 3a stats
 *
 * \param[in] ctx             context
 * \param[out] stats          stats params ref
 * \param[in] timeout_ms      -1 means wait always until stats is available or
 *                            stopped
 * \return return 0 if success
 * \note blocked interface, and return the stats ref, user should
 *       call \ref rk_aiq_uapi_sysctl_release3AStatsRef to release.
 */
XCamReturn
rk_aiq_uapi_sysctl_get3AStatsBlk(const rk_aiq_sys_ctx_t* ctx,
                              rk_aiq_isp_stats_t **stats, int timeout_ms);

/*!
 * \brief release 3a stats result ref
 *
 * \param[in] ctx             context
 * \param[out] stats          stats ref
 * \return void
 * \note called with \ref rk_aiq_uapi_sysctl_get3AStatsBlk
 */
void
rk_aiq_uapi_sysctl_release3AStatsRef(const rk_aiq_sys_ctx_t* ctx,
                                     rk_aiq_isp_stats_t *stats);
/*!
 * \brief set compensation light config
 *
 * \param[in] ctx             context
 * \param[out] cfg            cpsl configs
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_setCpsLtCfg(const rk_aiq_sys_ctx_t* ctx,
                               rk_aiq_cpsl_cfg_t* cfg);
/*!
 * \brief get compensation light info
 *
 * \param[in] ctx             context
 * \param[out] info           current cpsl settings
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_getCpsLtInfo(const rk_aiq_sys_ctx_t* ctx,
                                rk_aiq_cpsl_info_t* info);
/*!
 * \brief query compensation light capability
 *
 * \param[in] ctx             context
 * \param[out] cap            cpsl cap info
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_queryCpsLtCap(const rk_aiq_sys_ctx_t* ctx,
                                 rk_aiq_cpsl_cap_t* cap);

/*!
 * \brief prepare RK-raw-format data process environment
 *
 * \param[in] ctx             context
 * \param[in] prop            prepare params
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_prepareRkRaw(const rk_aiq_sys_ctx_t* ctx, rk_aiq_raw_prop_t prop);

/*!
 * \brief queue RK-Raw-format buffer into aiq control system
 *
 * \param[in] ctx             context
 * \param[in] rawdata         RK-Raw-format buffer
 * \param[in] sync            sync flag, true means sync mode,calling process will be blocked,
 *                            until the queued frame is processed. false means async mode, calling
 *                            process is not blocked, if you want to free rawdata or reuse it, callback
 *                            should be registered,after frame is processed, callback function would be called.
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_enqueueRkRawBuf(const rk_aiq_sys_ctx_t* ctx, void *rawdata, bool sync);

/*!
 * \brief queue RK-Raw-format file into aiq control system
 *
 * \param[in] ctx             context
 * \param[in] path            RK-Raw-format file path
 * calling process will be blocked until the queued frame is processed
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_enqueueRkRawFile(const rk_aiq_sys_ctx_t* ctx, const char *path);

/*!
 * \brief regist RK-Raw-format buffer callback into aiq control system
 *
 * \param[in] ctx             context
 * \param[in] callback        callback function pointer
 * if callback function is registered,  (when rk_aiq_uapi_sysctl_enqueueRkRawBuf used in sync mode)
 * callback will be called in sync after the queued raw buffer is processed, raw buffer pointer
 * which passed into aiq by rk_aiq_uapi_sysctl_enqueueRkRawBuf would be passed back into the callback
 * function you registered.
 * this function is not required.
 *
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_registRkRawCb(const rk_aiq_sys_ctx_t* ctx, void (*callback)(void*));

/*!
 * \brief get RkRaw frame info after ...enqueueRkRaw...()
 *
 * \param[in] ctx             context
 * \param[out] frame_info     frame info to get
 * called after rk_aiq_uapi_sysctl_enqueueRkRawBuf() or rk_aiq_uapi_sysctl_enqueueRkRawFile()
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_getRkRawFrameInfo(const rk_aiq_sys_ctx_t* ctx, rkraw_frame_info_t *frame_info);

/*!
 * \brief set the bypass stream rotation
 *
 * \param[in] ctx             context
 * \param[in] rot             rotation val
 * \return return 0 if success
 */
XCamReturn
rk_aiq_uapi_sysctl_setSharpFbcRotation(const rk_aiq_sys_ctx_t* ctx, rk_aiq_rotation_t rot);

/*!
 * \brief get aiq version infos
 *
 * \param[in,out] vers         version infos
 * \return return 0 if success
 */
void rk_aiq_uapi_get_version_info(rk_aiq_ver_info_t* vers);

/*!
 * \brief switch working mode dynamically
 * this aims to switch the isp pipeline working mode fast, and can be called on
 * streaming status. On non streaming status, should call rk_aiq_uapi_sysctl_prepare
 * instead of this to set working mode.
 */
XCamReturn
rk_aiq_uapi_sysctl_swWorkingModeDyn(const rk_aiq_sys_ctx_t* ctx, rk_aiq_working_mode_t mode);

/*!
 * \brief set multiple cameras working concurrently
 * Notify this AIQ ctx will run with other sensor's AIQ ctx.

 * \param[in] cc        set cams concurrently used or not
 * \note should be called before rk_aiq_uapi_sysctl_start
 */
void
rk_aiq_uapi_sysctl_setMulCamConc(const rk_aiq_sys_ctx_t* ctx, bool cc);

/*!
 * \brief set crop window of isp input
 * This API will affect the isp pipeline resolution.
 *
 * \param[in] rect      set cams crop prop
 * \note Optinal API, should be called before rk_aiq_uapi_sysctl_prepare
 */
XCamReturn
rk_aiq_uapi_sysctl_setCrop(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_rect_t rect);

/*!
 * \brief get crop window of isp input
 *
 * \param[in] rect       get cams crop prop
 */
XCamReturn
rk_aiq_uapi_sysctl_getCrop(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_rect_t *rect);

/*!
 * \brief apply an new iq file when stream on
 *
 * \param[in] iqfile       iqfile which will be applied
 */

XCamReturn
rk_aiq_uapi_sysctl_updateIq(const rk_aiq_sys_ctx_t* sys_ctx, char* iqfile);

/*!
 * \brief get current used calibDb struct point
 *
 * \param[in] ctx             context
 * \return current used calibDb struct point
 */

CamCalibDbContext_t*
rk_aiq_uapi_sysctl_getCurCalib(const rk_aiq_sys_ctx_t* ctx);

/*!
 * \brief update calibdb dynamically
 *
 * \param[in] ctx             context
 * \param[out] calib          original calib point could be got by
 *                            API rk_aiq_uapi_sysctl_getCurCalib
 */

XCamReturn
rk_aiq_uapi_sysctl_upateCalib(const rk_aiq_sys_ctx_t* ctx, CamCalibDbContext_t* calib);

/*!
 * \brief get describtion of sensor
 *
 * \param[out] sensorDes
 */
XCamReturn
rk_aiq_uapi_sysctl_getSensorDiscrib(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_exposure_sensor_descriptor *sensorDes);

RKAIQ_END_DECLARE

#endif
