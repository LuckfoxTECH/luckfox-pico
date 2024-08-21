/**
  * Copyright (c) 2020 Fuzhou Rockchip Electronics Co., Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#include <rtdevice.h>
#include <rtthread.h>

#include <dfs_posix.h>
#include <drivers/mtd_nor.h>
#include "hal_base.h"

#ifdef RT_USING_OTA_FROM_HTTP
#include "link_queue.h"
#include "../fwmgr/fw_analysis.h"
#include "../fwmgr/rkpart.h"
#include "http_api.h"
//#include "audio_server.h"
//#include "http_preprocessor.h"

#include "ota.h"
//#include "ota_debug.h"
#include "ota_http.h"

extern uint32_t g_fw_curr_slot;
extern uint32_t g_user_curr_slot;
extern uint32_t g_os_real_size;
extern uint32_t g_data_real_size;
extern uint32_t g_root_real_size;


static ota_http_cfg_t *g_otaHttpCfg;
static ota_http_parameters *g_http_param;
static long gImgFileSize;

static int http_socket = -1;
static unsigned char g_is_chunked = 0;
static unsigned long http_totalsize;

static uint32_t g_fw_pos;
static uint32_t g_skip_size;

#if OTA_OPT_PROTOCOL_HTTP

#define DBG_SECTION_NAME    "OTA_HTTP"
#include "rkdebug.h"

int http_opts_init(struct ota_http_opts_t *self, ota_http_cfg_t *cfg)
{
    int rst = -RT_ERROR;
    int mem_size = HTTP_DOWNLOAD_FILE_NAME_LENGTH;
    char *response = NULL;

REDO:
    response = (char *)rt_malloc(mem_size * sizeof(char));
    LOGD("rt_malloc response %p\n", response);
    if (response == RT_NULL)
        return rst;

    http_socket = -1;

    if (http_open(cfg->target, &response, &http_socket, HTTP_GET, NULL, 0) != HTTP_STATUS_OK)
    {
        goto END;
    }

    g_is_chunked = -RT_ERROR;
    struct http_res_header resp = parse_header(response);
    LOGD("\n>>>>http header parse success:<<<<");
    LOGD("\tHTTP reponse: %d", resp.status_code);
    if (resp.status_code != 200 && resp.status_code != 206)
    {
        if (resp.status_code == 301 || resp.status_code == 302) //301 Moved Permanently / 302 Found
        {
            if (response)
                rt_free(response);
            cfg->target = resp.httpRedirectURL.pParam;
            goto REDO;
        }
        else
        {
            LOGD("file can't be download,status code: %d\n", resp.status_code);
            rst = -RT_ERROR;
            goto END;
        }
    }
    LOGD("\tHTTP file type: %s", resp.content_type);

    g_is_chunked = is_chunked(resp);
    if (g_is_chunked)
    {
        LOGD("\tHTTP transfer-encoding: %s", resp.transfer_encoding);
    }
    else
    {
        LOGD("\tHTTP file length: %ld byte", resp.content_length);
        http_totalsize = resp.content_length;
    }
    cfg->file_size = resp.content_length;
    self->userdata = (void *)&http_socket;
    cfg->frame_size = HTTP_PREPROCESSOR_FRAME_SIZE;

//    LOGD("[%s] open native http ok, http: %s, audio type:%s",
//         cfg->tag,
//         cfg->target,
//         cfg->type);
    rst = RT_EOK;

END:
    if (response)
        rt_free(response);

    if (rst == -RT_ERROR)
    {
        if (http_socket != -1)
        {
            close(http_socket);
            LOGW("****** close socket. ******");
        }
    }

    return rst;
}

int http_opts_read(struct ota_http_opts_t *self, char *data, size_t data_len)
{
    int rs = RT_FALSE;

    if (!self->userdata)
    {
        return rs;
    }

    if (g_is_chunked)
    {
        rs = http_socket_read_chunk(*(int *)self->userdata, data, data_len);
    }
    else
    {
        if (http_totalsize == 0)
            return 0;

        if (data_len > http_totalsize)
            data_len = http_totalsize;

        rs = http_socket_read(*(int *)self->userdata, data, data_len);
        if (rs > 0)
        {
            if (rs != data_len)
            {
//                LOGD("read data underrun,data_len:%d has_read:%d\n", data_len, rs);
            }
            http_totalsize -= rs;
        }
        else if (rs == 0)
        {
            if (http_totalsize > 0)
            {
                LOGE("http read data fail\n");
                return -101; //http not end, return this val for app stop player
            }
            else
            {
                LOGD("http read data over\n");
            }
        }
        else if (rs == -1)
        {
            //socket error, if return -101, watch dog will reset system
            LOGE("socket error\n");
            return 0;
        }
    }

    return rs;
}

void http_opts_destroy(struct ota_http_opts_t *self)
{
    if (!self)
        return;
    if (!self->userdata)
        return;

    int client_socket = *(int *)self->userdata;
    http_close(client_socket);
}

ota_status ota_update_http_init(void *url)
{
    LOGD("%s Enter", __func__);
    if (g_http_param == RT_NULL)
    {
        g_http_param = (ota_http_parameters *)rt_malloc(sizeof(ota_http_parameters));
        if (g_http_param == RT_NULL)
        {
            LOGE("http param malloc fail");
            return OTA_STATUS_ERR;
        }
    }

    /* check wifi whether connect */
    while (rt_wlan_is_ready() != RT_TRUE)
    {
        rt_thread_delay(100);
        LOGW("wlan NG. Pls connect wifi!");
    }

    if (!url)
    {
        if (g_http_param)
            rt_free(g_http_param);
        LOGE("file URL is NULL");
        return OTA_STATUS_ERR;
    }

    rt_memset(g_http_param, 0, sizeof(ota_http_parameters));
    rt_memcpy(g_http_param->url, url, strlen(url));
    LOGD("ota file url = %s", g_http_param->url);

    g_otaHttpCfg = rt_malloc(sizeof(ota_http_cfg_t));
    if (g_otaHttpCfg == RT_NULL)
    {
        LOGE("ota http cfg malloc fail");
        if (g_http_param)
            rt_free(g_http_param);
        return OTA_STATUS_ERR;
    }

    g_otaHttpCfg->target = g_http_param->url;
    g_http_param->http_opts = (ota_http_opts)DEFAULT_HTTP_OPTS;
    int ret = g_http_param->http_opts.init(&g_http_param->http_opts, g_otaHttpCfg);
    if (ret != 0)
    {
        LOGE("### %s(), http_opts.init fail ###", __func__);

        if (g_otaHttpCfg)
        {
            rt_free(g_otaHttpCfg);
            g_otaHttpCfg = RT_NULL;
        }
        if (g_http_param)
        {
            rt_free(g_http_param);
            g_http_param = RT_NULL;
        }

        LOGE("%s(), Fail", __func__);
        return OTA_STATUS_ERR;
    }

    RT_ASSERT(g_http_param->http_opts.read != RT_NULL);

    //the whole update.img's size, we should get the echo slot fw size in http download handle task.
    gImgFileSize = g_otaHttpCfg->file_size;
    LOGD("File size : %ld Bytes", gImgFileSize);
    if (!gImgFileSize)
        return OTA_STATUS_ERR;

    return OTA_STATUS_OK;
}

rt_thread_t gOtaHttpDownloadHdl;
LinkQueue *gDnQueue;
uint8_t eof_flag;
uint8_t ota_http_run;
uint32_t ota_image_left;
volatile uint32_t ota_total_count = 0;
volatile uint32_t ota_downld_count = 0;
volatile uint32_t ota_write_count = 0;

//static rt_uint32_t fw_slot_size;
static void ota_http_seek_to(rt_uint32_t offset)
{
    rt_int32_t readSize = -1;
    rt_uint32_t seek_size = 0;
    rt_uint32_t recv_size;
    rt_uint32_t each_read_size = 0;
    char tmpBuf[OTA_BUF_SIZE] = {0};

    //LOGD("%s Enter...", __func__);
    //LOGD("need to seek to offset %d(%#x)", offset, offset);
    each_read_size = OTA_BUF_SIZE;
    seek_size = offset;

    while (1)
    {
        recv_size = 0;
        rt_memset(tmpBuf, 0, OTA_BUF_SIZE);

        if (seek_size < each_read_size)
        {
            each_read_size = seek_size;
            //LOGD("Total seek pos left: %d", seek_size);
        }
RETRY:
        readSize = g_http_param->http_opts.read(&g_http_param->http_opts, tmpBuf, each_read_size);
        if (readSize <= 0)   //means read fail.retry here.
        {
            LOGE("read size %d, retry...", readSize);
            rt_thread_delay(10);
            goto RETRY;
        }

        seek_size -= readSize;
        //LOGD("leftt seek_size = %d", seek_size);

        //if (seek_size < each_read_size)
        //    LOGD("seek left size = %ld, have readed = %d", seek_size, readSize);

        if (readSize == each_read_size)
        {
            if (seek_size > 0)
                continue; //do nothing, read data we discard.
            else if (seek_size == 0)
                break;   // we have seek to needed postion.
        }
        else if (readSize != each_read_size)
        {
            if (readSize < each_read_size)
            {
                int left = 0;
                recv_size = readSize;
                if (seek_size > 0)
                {
                    if (seek_size >= each_read_size || (seek_size + readSize) >= each_read_size)
                    {
                        /* left data size big than buf_size */
                        do
                        {
                            left = each_read_size - recv_size;
REDO0:
                            readSize = g_http_param->http_opts.read(&g_http_param->http_opts, &tmpBuf[recv_size], left);
                            //LOGD("[retry to read]:have readsize = %ld", readSize);
                            if (readSize < 0)
                            {
                                rt_thread_delay(10);
                                goto REDO0;
                            }

                            recv_size += readSize;
                            seek_size -= readSize;
                            if (recv_size == each_read_size)
                                break;
                        }
                        while (readSize != left);
                    }
                    else
                    {
                        /* left data size small than buf_size */
                        do
                        {
REDO1:
                            readSize = g_http_param->http_opts.read(&g_http_param->http_opts, &tmpBuf[recv_size], seek_size);
                            if (readSize < 0)
                            {
                                rt_thread_delay(10);
                                goto REDO1;
                            }
                            recv_size += readSize;
                            seek_size -= readSize;
                            if (seek_size == 0)
                                break;
                        }
                        while (seek_size != 0);
                    }

                    if (seek_size == 0)
                        break;
                }
                else if (seek_size == 0)
                    break;
            }
        }
    }

    //LOGD("Have sought to %d(%#x) position", offset, offset);
}

static void ota_http_os_downlaod_hdl(void *arg)
{
    int ret = 0;
    int readSize = -1;
    rt_uint32_t recv_size = 0;
    rt_uint32_t fw_buf_size = 0;
    char *fw_data = RT_NULL;
    rt_uint32_t seek_pos = 0;
    char tmpBuf[OTA_BUF_SIZE] = {0};

    //LOGD("%s Enter ...", __func__);
    fw_buf_size = OTA_BUF_SIZE;
    gDnQueue = create_empty_linkqueue();
    if (gDnQueue == RT_NULL)
    {
        LOGE("gRecordQueue create failed.");
        return;
    }

    /* pre-process: seek to desired fw slot position in firmware.img*/
    {
        char *head_data = RT_NULL;
        rt_uint32_t recv = 0;
        rt_uint8_t end_flag = 0;
        PFIRMWARE_HEADER pFwHead = RT_NULL;
        unsigned char magic[8] = {'R', 'E', 'S', 'C', 0, 0, 0, 0};

        if (g_fw_curr_slot == 0)
            seek_pos = firmware_addr2;
        else if (g_fw_curr_slot == 1)
            seek_pos = firmware_addr1;
        else
            return;

        ota_http_seek_to(seek_pos); //like lseek to seek_pos.
        g_fw_pos = seek_pos;

        /*we get fw slot head */
        head_data = (char *)rt_malloc(OTA_BUF_SIZE);
        if (head_data == RT_NULL)
        {
            LOGE("malloc head_data buffer error.");
            return;
        }
        if (ota_fw_head_http_get((uint8_t *)head_data, OTA_BUF_SIZE, &recv, &end_flag) != OTA_STATUS_OK)
        {
            LOGE("get fw slot %d head data fail");
            return;
        }

        pFwHead = (PFIRMWARE_HEADER)head_data;
        if (0 != rt_memcmp(pFwHead->magic, magic, 8))
        {
            LOGE("Fw head magic Error!");
            return;
        }

        /* update fw slot size here */
        gImgFileSize     = pFwHead->data_size + 512 + 4;
        g_os_real_size = gImgFileSize;
        g_fw_pos        += gImgFileSize;

        //LOGD("fw data offset = %u(0x%#08x)", pFwHead->data_offset, pFwHead->data_offset);
        //LOGD("fw data size = %u(0x%#08x)", pFwHead->data_size, pFwHead->data_size);
        //LOGD("fw slot size = %u(0x%#08x)", gImgFileSize, gImgFileSize);

        /*have read OTA_BUF_SIZE to buff*/
        gImgFileSize -= OTA_BUF_SIZE;
        //LOGD("Total image file left: %d", gImgFileSize);

        /* we should push have read 4K data to LinkQueue*/
        ret = push_linkqueue(gDnQueue, head_data);
        if (ret)
        {
            LOGE("[line:%d]push link queue fail", __LINE__);
            rt_free(head_data);
            LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
            return;
        }
        ota_total_count++;
        ota_downld_count++;
        rt_thread_delay(20);
    }

    while (1)
    {
        while (ota_http_run)
        {
            recv_size = 0;
            if (gImgFileSize < fw_buf_size)
            {
                fw_buf_size = gImgFileSize;
                //LOGD("Total image file left: %d", gImgFileSize);
            }

            fw_data = (char *)rt_malloc(fw_buf_size);
            if (fw_data == RT_NULL)
            {
                LOGE("malloc fw_data buffer fail.");
                break;
            }

            rt_memset(fw_data, 0, fw_buf_size);
            ota_image_left = fw_buf_size;

            rt_memset(tmpBuf, 0, OTA_BUF_SIZE);
            RT_ASSERT(g_http_param->http_opts.read != RT_NULL);
RETRY:
            readSize = g_http_param->http_opts.read(&g_http_param->http_opts, tmpBuf, fw_buf_size);
            if (readSize <= 0)   //means read fail.retry here.
            {
                LOGE("read size %d, retry...", readSize);
                rt_thread_delay(10);
                goto RETRY;
            }
            //LOGD("=== have read %u ===", readSize);

            gImgFileSize -= readSize;
            //LOGD("===Fw left %u  to download.===", gImgFileSize);

            //if (gImgFileSize < fw_buf_size)
            //    LOGD("left size = %ld, have readed = %d", gImgFileSize, readSize);

            if (readSize == fw_buf_size)
            {
                if (gImgFileSize > 0)
                {
                    ota_total_count++;
                    ota_downld_count++;
                    eof_flag = 0;

                    rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                    ret = push_linkqueue(gDnQueue, fw_data);
                    if (ret)
                    {
                        LOGE("[line:%d]push link queue fail", __LINE__);
                        rt_free(fw_data);
                        LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                        break;
                    }
                    //LOGD(">>>>>>>>>>>>>>>>>>> download %d times", ota_downld_count);
                    rt_thread_delay(10);
                    continue;
                }
                else if (0 == gImgFileSize)
                {
                    ota_total_count++;
                    ota_downld_count++;
                    eof_flag = 1;
                    ota_http_run = 0;

                    rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                    ret = push_linkqueue(gDnQueue, fw_data);
                    if (ret)
                    {
                        LOGE("[line:%d]push link queue fail", __LINE__);
                        rt_free(fw_data);
                        LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                        break;
                    }

                    //LOGD("Last time download size: %d", fw_buf_size);
                    //LOGD("Total download fw size: %d", g_otaHttpCfg->file_size);
                    //LOGD(">>>>>>>>>>>>>>>>>>> download over,total download %d times", ota_total_count);
                    break;
                }
            }

            if (readSize != fw_buf_size)
            {
                if (readSize <= 0)
                {
                    goto RETRY;
                }
                else if (readSize < fw_buf_size)
                {
                    int left = 0;
                    //uint8_t *tmpBuf = fw_data;

                    recv_size = readSize;
                    if (gImgFileSize > 0)
                    {
                        if (gImgFileSize >= fw_buf_size || (gImgFileSize + readSize) >= fw_buf_size)
                        {
                            /* left data size big than buf_size */
                            do
                            {
                                left = fw_buf_size - recv_size;
                                //LOGD("[left to request]: # %ld #", left);
                                //tmpBuf += recv_size;
REDO0:
                                readSize = g_http_param->http_opts.read(&g_http_param->http_opts, &tmpBuf[recv_size], left);
                                //LOGD("[retry to read]: readsize = %ld", readSize);
                                if (readSize < 0)
                                {
                                    LOGE("read size %d, retry...", readSize);
                                    rt_thread_delay(10);
                                    goto REDO0;
                                }

                                recv_size += readSize;
                                gImgFileSize -= readSize;
                                //LOGD("[retry to read]: have get %u,total left = %ld",recv_size, gImgFileSize);
                                if (recv_size == fw_buf_size)
                                {
                                    break;
                                }
                            }
                            while (readSize != left);

                            rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                        }
                        else
                        {
                            /* left data size small than buf_size */
                            do
                            {
                                //tmpBuf += recv_size;
                                //LOGD("will reqeust %d", gImgFileSize);
REDO1:
                                readSize = g_http_param->http_opts.read(&g_http_param->http_opts, &tmpBuf[recv_size], gImgFileSize);
                                if (readSize < 0)
                                {
                                    LOGE("read size %d, retry...", readSize);
                                    rt_thread_delay(10);
                                    goto REDO1;
                                }
                                //LOGD("retry to read: readsize = %ld", readSize);
                                recv_size += readSize;
                                gImgFileSize -= readSize;
                                //LOGD("retry to read: left size = %ld", gImgFileSize);
                                if (gImgFileSize == 0)
                                {
                                    break;
                                }
                            }
                            while (gImgFileSize != 0);

                            rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                        }

                        if (gImgFileSize)
                        {
                            /*not read to end*/
                            //eof_flag = 0;
                            //LOGD("recv size = %u , request size = %u", recv_size, fw_buf_size);
                            //LOGD("# Total left %u #", gImgFileSize);

                            ota_total_count++;
                            ota_downld_count++;
                            eof_flag = 0;

                            //rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                            ret = push_linkqueue(gDnQueue, fw_data);
                            if (ret)
                            {
                                LOGE("[line:%d]push link queue fail", __LINE__);
                                rt_free(fw_data);
                                LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                                break;
                            }
                            //LOGD(">>>>>>>>>>>>>>>>>>> downlaod 2  %d times", ota_downld_count);
                            rt_thread_delay(10);
                            continue;
                        }
                        else
                        {
                            /*means read over*/
                            eof_flag = 1;
                            //LOGD("http download file over!");
                            ota_total_count++;
                            ota_downld_count++;
                            ota_http_run = 0;

                            //rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                            ret = push_linkqueue(gDnQueue, fw_data);
                            if (ret)
                            {
                                LOGE("[line:%d]push link queue fail", __LINE__);
                                rt_free(fw_data);
                                LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                                break;
                            }

                            rt_thread_delay(10);
                            //LOGD(">>>>>>>>>>>>>>>>>>> download over,total download %d times", ota_total_count);

                            /* destroy http preprocessor*/
                            g_http_param->http_opts.destroy(&g_http_param->http_opts);
                        }
                    }
                    else
                    {
                        /*means read over*/
                        eof_flag = 1;
                        //LOGD("http download file over!!");
                        ota_total_count++;
                        ota_downld_count++;
                        ota_http_run = 0;

                        rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                        ret = push_linkqueue(gDnQueue, fw_data);
                        if (ret)
                        {
                            LOGE("[line:%d]push link queue fail", __LINE__);
                            rt_free(fw_data);
                            LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                            break;
                        }
                        //LOGD(">>>>>>>>>>>>>>>>>>> download over,total download %d times", ota_total_count);

                        /* destroy http preprocessor*/
                        g_http_param->http_opts.destroy(&g_http_param->http_opts);
                    }
                }
            }
        }

        push_linkqueue(gDnQueue, "download_end");
        break;
    }

    //LOGD("exit ota_http_os_dn_hdl task.\n");
    gOtaHttpDownloadHdl = RT_NULL;
}

#ifdef RT_SUPPORT_ROOT_AB
static void ota_http_root_downlaod_hdl(void *arg)
{
    int ret = 0;
    int readSize = -1;
    rt_uint32_t recv_size = 0;
    rt_uint32_t fw_buf_size = 0;
    char *fw_data = RT_NULL;
    rt_uint32_t seek_pos = 0;
    char tmpBuf[OTA_BUF_SIZE] = {0};

    //LOGD("%s Enter ...", __func__);
    fw_buf_size = OTA_BUF_SIZE;
    gDnQueue = create_empty_linkqueue();
    if (gDnQueue == RT_NULL)
    {
        LOGE("gRecordQueue create failed.");
        return;
    }

    /* pre-process: seek to desired root slot position in firmware.img*/
    if (g_user_curr_slot == 0)
        seek_pos = user_start_addr2;
    else if (g_user_curr_slot == 1)
        seek_pos = user_start_addr1;
    else
        return;

    g_skip_size = seek_pos - g_fw_pos;
    ota_http_seek_to(g_skip_size); //skip to desired root position.
    gImgFileSize = user_part_size;

    while (1)
    {
        while (ota_http_run)
        {
            recv_size = 0;
            if (gImgFileSize < fw_buf_size)
            {
                fw_buf_size = gImgFileSize;
                LOGD("Total image file left: %d", gImgFileSize);
            }

            fw_data = (char *)rt_malloc(fw_buf_size);
            if (fw_data == RT_NULL)
            {
                LOGE("malloc fw_data buffer fail.");
                break;
            }

            rt_memset(fw_data, 0, fw_buf_size);
            ota_image_left = fw_buf_size;

            rt_memset(tmpBuf, 0, OTA_BUF_SIZE);
            RT_ASSERT(g_http_param->http_opts.read != RT_NULL);
RETRY:
            readSize = g_http_param->http_opts.read(&g_http_param->http_opts, tmpBuf, fw_buf_size);
            if (readSize <= 0)   //means read fail.retry here.
            {
                LOGE("read size %d, retry...", readSize);
                rt_thread_delay(10);
                goto RETRY;
            }
            //LOGD("=== have read %u ===", readSize);

            gImgFileSize -= readSize;
            //LOGD("===Fw left %u  to download.===", gImgFileSize);

            if (gImgFileSize < fw_buf_size)
                LOGD("left size = %ld, have readed = %d", gImgFileSize, readSize);

            if (readSize == fw_buf_size)
            {
                if (gImgFileSize > 0)
                {
                    ota_total_count++;
                    ota_downld_count++;
                    eof_flag = 0;

                    rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                    ret = push_linkqueue(gDnQueue, fw_data);
                    if (ret)
                    {
                        LOGE("[line:%d]push link queue fail", __LINE__);
                        rt_free(fw_data);
                        LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                        break;
                    }
                    LOGD(">>>>>>>>>>>>>>>>>>> download root data %d times", ota_downld_count);
                    rt_thread_delay(10);
                    continue;
                }
                else if (0 == gImgFileSize)
                {
                    ota_total_count++;
                    ota_downld_count++;
                    eof_flag = 1;
                    ota_http_run = 0;

                    rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                    ret = push_linkqueue(gDnQueue, fw_data);
                    if (ret)
                    {
                        LOGE("[line:%d]push link queue fail", __LINE__);
                        rt_free(fw_data);
                        LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                        break;
                    }
                    //LOGD("Last time download size: %d", fw_buf_size);
                    //LOGD(">>>>>>>>>>>>>>>>>>> download over,total download %d times", ota_total_count);
                    break;
                }
            }

            if (readSize != fw_buf_size)
            {
                if (readSize <= 0)
                {
                    goto RERRY;
                }
                else if (readSize < fw_buf_size)
                {
                    int left = 0;
                    //uint8_t *tmpBuf = fw_data;

                    recv_size = readSize;
                    if (gImgFileSize > 0)
                    {
                        if (gImgFileSize >= fw_buf_size || (gImgFileSize + readSize) >= fw_buf_size)
                        {
                            /* left data size big than buf_size */
                            do
                            {
                                left = fw_buf_size - recv_size;
                                //LOGD("[left to request]: # %ld #", left);
                                //tmpBuf += recv_size;
REDO0:
                                readSize = g_http_param->http_opts.read(&g_http_param->http_opts, &tmpBuf[recv_size], left);
                                //LOGD("[retry to read]: readsize = %ld", readSize);
                                if (readSize < 0)
                                {
                                    LOGE("read size %d, retry...", readSize);
                                    rt_thread_delay(10);
                                    goto REDO0;
                                }

                                recv_size += readSize;
                                gImgFileSize -= readSize;
                                //LOGD("[retry to read]: have get %u,total left = %ld",recv_size, gImgFileSize);
                                if (recv_size == fw_buf_size)
                                {
                                    break;
                                }
                            }
                            while (readSize != left);

                            rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                        }
                        else
                        {
                            /* left data size small than buf_size */
                            do
                            {
                                //tmpBuf += recv_size;
                                //LOGD("will reqeust %d", gImgFileSize);
REDO1:
                                readSize = g_http_param->http_opts.read(&g_http_param->http_opts, &tmpBuf[recv_size], gImgFileSize);
                                if (readSize < 0)
                                {
                                    LOGE("read size %d, retry...", readSize);
                                    rt_thread_delay(10);
                                    goto REDO1;
                                }
                                //LOGD("retry to read: readsize = %ld", readSize);
                                recv_size += readSize;
                                gImgFileSize -= readSize;
                                //LOGD("retry to read: left size = %ld", gImgFileSize);
                                if (gImgFileSize == 0)
                                {
                                    break;
                                }
                            }
                            while (gImgFileSize != 0);

                            rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                        }

                        if (gImgFileSize)
                        {
                            /*not read to end*/
                            //LOGD("recv size = %u , request size = %u", recv_size, fw_buf_size);
                            //LOGD("# Total left %u #", gImgFileSize);

                            ota_total_count++;
                            ota_downld_count++;
                            eof_flag = 0;

                            //rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                            ret = push_linkqueue(gDnQueue, fw_data);
                            if (ret)
                            {
                                LOGE("[line:%d]push link queue fail", __LINE__);
                                rt_free(fw_data);
                                LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                                break;
                            }
                            //LOGD(">>>>>>>>>>>>>>>>>>> downlaod 2 %d times", ota_downld_count);
                            rt_thread_delay(10);
                            continue;
                        }
                        else
                        {
                            /*means read over*/
                            eof_flag = 1;
                            //LOGD("http download root data over!");
                            ota_total_count++;
                            ota_downld_count++;
                            ota_http_run = 0;

                            //rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                            ret = push_linkqueue(gDnQueue, fw_data);
                            if (ret)
                            {
                                LOGE("[line:%d]push link queue fail", __LINE__);
                                rt_free(fw_data);
                                LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                                break;
                            }

                            rt_thread_delay(10);
                            //LOGD(">>>>>>>>>>>>>>>>>>> download over,total download root %d times", ota_total_count);

                            /* destroy http preprocessor*/
                            g_http_param->http_opts.destroy(&g_http_param->http_opts);
                        }
                    }
                    else
                    {
                        /*means read over*/
                        eof_flag = 1;
                        //LOGD("http download root data over!!");
                        ota_total_count++;
                        ota_downld_count++;
                        ota_http_run = 0;

                        rt_memcpy(fw_data, tmpBuf, fw_buf_size);
                        ret = push_linkqueue(gDnQueue, fw_data);
                        if (ret)
                        {
                            LOGE("[line:%d]push link queue fail", __LINE__);
                            rt_free(fw_data);
                            LOGE(">>>>>%d gDnQueue invalid ret = %d", __LINE__, ret);
                            break;
                        }
                        //LOGD(">>>>>>>>>>>>>>>>>>> download over,total download root %d times", ota_total_count);

                        /* destroy http preprocessor*/
                        g_http_param->http_opts.destroy(&g_http_param->http_opts);
                    }
                }
            }
        }

        push_linkqueue(gDnQueue, "download_end");
        break;
    }

    LOGD("exit ota_http_root_hdl task.\n");
    gOtaHttpDownloadHdl = RT_NULL;
}
#endif

int ota_start_download_task(void *arg)
{
    ota_http_run = 1;

    /* http download os partiton */
    if ((uint32_t)arg == 0)
    {
        gOtaHttpDownloadHdl = rt_thread_create("ota_os_downld",
                                               ota_http_os_downlaod_hdl,
                                               RT_NULL,
                                               8192 * 4,
                                               16,
                                               20);
        if (!gOtaHttpDownloadHdl)
        {
            LOGE("ota_os_downld thread create fail");
            return RT_NULL;
        }
        else
            rt_thread_startup(gOtaHttpDownloadHdl);
    }
#ifdef RT_SUPPORT_ROOT_AB
    else if ((uint32_t)arg == 1)
    {
        gOtaHttpDownloadHdl = rt_thread_create("ota_root_downld",
                                               ota_http_root_downlaod_hdl,
                                               RT_NULL,
                                               8192 * 4,
                                               16,
                                               20);
        if (!gOtaHttpDownloadHdl)
        {
            LOGE("ota_root_downld thread create fail");
            return RT_NULL;
        }
        else
            rt_thread_startup(gOtaHttpDownloadHdl);
    }
#endif

    return 0;
}

void ota_exit_download_task(void)
{
    ota_http_run = 0;
    ota_total_count = 0;
    ota_downld_count = 0;
    ota_write_count = 0;

    if (gDnQueue)
    {
        delete_empty_linkqueue(gDnQueue);
        gDnQueue = NULL;
    }
}

ota_status ota_update_http_get(uint8_t *buf, uint32_t buf_size, uint32_t *recv_size, uint8_t *eof)
{
    char *fw_data;

    while (1)
    {
        if (!gDnQueue)
        {
            //LOGD("gDnQueue = NULL !");
            rt_thread_delay(50);
            continue;
        }
        else if (is_empty_linkqueue(gDnQueue))
        {
            //LOGD("LinkQueue is empty !");
            rt_thread_delay(50);
            continue;
        }

        fw_data = pop_linkqueue(gDnQueue);
        if ((int)fw_data < 0)
            LOGE(" ######********* ERROR ********###### fw_data = %d\n ", (int)fw_data);

        if ((int)fw_data == -1)
        {
            rt_thread_delay(10);
            //LOGD("gDnQueue pop Error");
            continue;
        }
        else if (!strncmp(fw_data, "download_end", strlen("download_end")))
        {
            if (!eof_flag)
            {
                *eof = 0;
                *recv_size = 0;
                return OTA_STATUS_ERR;
            }
            //LOGD("---------------- have reached End of Partiton ----------------");
            break;
        }
        else
        {
            //LOGD("FW have writed         @ %d @", ota_write_count);
            ota_write_count++;
        }

        if (ota_write_count == ota_total_count && !ota_http_run)
        {
            //LOGD("****** last will write %d ******", ota_image_left);

            rt_memcpy(buf, fw_data, ota_image_left);
            *recv_size = ota_image_left;
            rt_free((void *)fw_data);
            //LOGD("****** total write == total download ******");

            while (1)
            {
                fw_data = pop_linkqueue(gDnQueue);    //last time
                if ((int)fw_data != -1 && !strncmp(fw_data, "download_end", strlen("download_end")))
                {
                    *eof = 1;
                    //LOGD("FW have End of File");
                    break;
                }
                rt_thread_delay(10);
            }
        }
        else
        {
            //LOGD(" write         @ %d @", ota_write_count);
            rt_memcpy(buf, fw_data, buf_size);
            //LOGD("  free data %p  ",fw_data);
            rt_free((void *)fw_data);
            *recv_size = buf_size;
            *eof = 0;
        }
        //LOGD("################### will write %d,  @ %d @ time", *recv_size, ota_write_count);

        break;
    }

    return OTA_STATUS_OK;
}

ota_status ota_fw_head_http_get(uint8_t *buf, uint32_t buf_size, uint32_t *recv_size, uint8_t *eof_flag)
{
    uint8_t bSucc = RT_FALSE;
    int readSize = 0;
    int leftSize = 0;

    leftSize = buf_size;
    readSize = g_http_param->http_opts.read(&g_http_param->http_opts, (char *)buf, buf_size);
    leftSize -= readSize;

    //LOGD("%s Enter...", __func__);

    if (readSize == buf_size)
    {
        *recv_size = buf_size;
        *eof_flag = 1;
        bSucc = RT_TRUE;
        //LOGD("recv size = %u \n", readSize);
        goto END;;
    }

    if (readSize != buf_size)
    {
        if (readSize <= 0)
        {
            LOGE("get data error");
            bSucc = RT_FALSE;
            goto END;
        }
        else if (readSize < buf_size)
        {
            uint8_t *tmpBuf = buf;
            *recv_size = readSize;
            if (leftSize)
            {
                /* left data size small than buf_size */
                do
                {
                    tmpBuf += readSize;
                    //LOGD("will reqeust %d", leftSize);
                    readSize = g_http_param->http_opts.read(&g_http_param->http_opts, (char *)tmpBuf, leftSize);
                    //LOGD("retry to read: readsize = %ld", readSize);
                    *recv_size += readSize;
                    leftSize -= readSize;
                    //LOGD("retry to read: left size = %ld", leftSize);
                    if (leftSize == 0)
                    {
                        break;
                    }
                }
                while (leftSize != 0);

                if (leftSize == 0)
                {
                    /*means read over*/
                    *eof_flag = 1;
                    //LOGD("http read need fw head data!");
                    bSucc = RT_TRUE;
                }
            }
        }
    }

END:
    /* destroy http preprocessor*/

    if (!bSucc)
        return OTA_STATUS_ERR;
    else
        return OTA_STATUS_OK;
}

#endif  /*OTA_OPT_PROTOCOL_HTTP*/
#endif
