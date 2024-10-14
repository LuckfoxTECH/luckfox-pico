/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
 */

#include "http_api.h"

static size_t g_last_socket_read_size;
static size_t g_last_not_read_chunk_size;
static int g_chunk_data_over;
static char g_chunk_buffer[HTTP_PREPROCESSOR_FRAME_SIZE * 2];

static void get_ip_addr(char *host_name, char *ip_addr);
static void parse_url(const char *url, char *host, int *port, char *download_file_name);

static void get_ip_addr(char *host_name, char *ip_addr)
{
    int i;
    struct hostent *host = gethostbyname(host_name);

    if (!host)
    {
        ip_addr = NULL;
        return;
    }
    for (i = 0; host->h_addr_list[i]; i++)
    {
        strcpy(ip_addr, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]));
        break;
    }
}

static void parse_url(const char *url, char *host, int *port, char *download_file_name)
{
    int j = 0, i;
    int start = 0;
    *port = HTTP_SOCKET_PORT;
    char *patterns[] = {"http://", "https://", NULL};

    for (i = 0; patterns[i]; i++)
    {
        if (strncmp(url, patterns[i], strlen(patterns[i])) == 0)
        {
            start = strlen(patterns[i]);
        }
    }
    for (i = start; url[i] != '/' && url[i] != '\0'; i++, j++)
    {
        host[j] = url[i];
    }
    host[j] = '\0';

    char *pos = strstr(host, ":");
    if (pos)
    {
        sscanf(pos, ":%d", port);
    }

    for (i = 0; i < (int)strlen(host); i++)
    {
        if (host[i] == ':')
        {
            host[i] = '\0';
            break;
        }
    }

    j = 0;
    for (i = start; url[i] != '\0'; i++)
    {
        if (url[i] == '/')
        {
            if (i !=  strlen(url) - 1)
            {
                j = 0;
            }
            continue;
        }
        else
        {
            download_file_name[j++] = url[i];
        }
    }
    download_file_name[j] = '\0';
}

int is_chunked(struct http_res_header resp)
{
    if (!strcmp(resp.transfer_encoding, "Chunked")
        || !strcmp(resp.transfer_encoding, "chunked"))
    {
        return 1;
    }

    return 0;
}

struct http_res_header parse_header(const char *response)
{
    struct http_res_header resp;

    char *pos = strstr(response, "HTTP/");
    if (pos)
    {
        sscanf(pos, "%*s %d", &resp.status_code);
    }

    pos = strstr(response, "Content-Type:");
    if (pos)
    {
        sscanf(pos, "%*s %s", resp.content_type);
    }

    pos = strstr(response, "Transfer-Encoding:");
    if (pos)
    {
        sscanf(pos, "%*s %s", resp.transfer_encoding);
    }

    pos = strstr(response, "Content-Length:");
    if (pos)
    {
        sscanf(pos, "%*s %ld", &resp.content_length);
    }

    pos = strstr(response, "Cookie:");
    if (pos)
    {
        sscanf(pos, "%*s %s", resp.cookie);
    }

    pos = strstr(response, "Location:");
    if (pos)
    {
        char *pEnd;
        sscanf(pos, "%*s %s", resp.httpRedirectURL.pParam);
        pEnd = strstr(resp.httpRedirectURL.pParam, "\r\n");
        if (pEnd)
        {
            resp.httpRedirectURL.nLength =  pEnd - resp.httpRedirectURL.pParam;
        }
    }

    return resp;
}

/**
 * @brief Init http according to url
 * @param[in] url to http opened url
 * @param[in] param1 response of read from socket and in post mode send data buf
 * @param[out] param2 socket to return to operate and in post mode send data size
 * @retval ota_status_t, OTA_STATUS_OK on success
 */
http_status_t http_open(char *url, char **param1, int *param2, int mode, char *cookie, int offset)
{
    int i;
    char host_addr[HTTP_HOST_ADDRESS_LENGTH] = {0};
    int port = HTTP_SOCKET_PORT;
    bool bSucc = RT_FALSE;
    int socket_local;

    struct hostent *host;
    char *download_file_name = NULL;
    char *header = NULL;
    char *buf = NULL;
    char *relative_url = NULL;

    download_file_name = (char *)rt_malloc(HTTP_DOWNLOAD_FILE_NAME_LENGTH * 4);
    if (download_file_name == NULL)
    {
        HTTP_ERR(" No memory!");
        return HTTP_STATUS_ERR;
    }

    header = (char *)rt_malloc(HTTP_HEADER_LENGTH);
    if (header == NULL)
    {
        HTTP_ERR(" No memory!");
        rt_free(download_file_name);
        return HTTP_STATUS_ERR;
    }

    struct sockaddr_in address;
    parse_url(url, host_addr, &port, download_file_name);

    int formatPos = strlen(download_file_name) - 3;
    char *format = download_file_name + formatPos;
    char *_url = strstr(url, "://") + 3;
    if (0 == strncmp(format, "mp3", 3) || strstr(download_file_name, "update") != NULL)
    {
        /* is music url or update.img*/
        relative_url = strstr(_url, "/");
        HTTP_DBG(" url = %s. relative_url = %s ", url, relative_url);
    }
    else       /* is not music url */
    {
        HTTP_DBG(" url = %s ", url);
        relative_url = url;
    }

    socket_local = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;

    if (socket_local == -1)
    {
        bSucc = RT_FALSE;
        goto END;
    }

    if (!(host = gethostbyname(host_addr)))
    {
        HTTP_ERR("gethostbyname failed");
        bSucc = RT_FALSE;
        goto END;
    }

    memcpy(&address.sin_addr.s_addr, *(host->h_addr_list), sizeof(address.sin_addr.s_addr));
    address.sin_port = htons(port);

    if (connect(socket_local, (struct sockaddr *) &address, sizeof(struct sockaddr_in)) == -1)
    {
        HTTP_ERR("connect sock failed\n");
        bSucc = RT_FALSE;
        goto END;
    }

    HTTP_DBG("cfg->target:%s", url);
    HTTP_DBG("  host_addr:%s", host_addr);
    HTTP_DBG("       port:%d", port);
    HTTP_DBG("downLD_file:%s\n", download_file_name);

    if (mode == HTTP_GET)
    {
        sprintf(header, \
                "GET %s HTTP/1.1\r\n"\
                "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537(KHTML, like Gecko) Chrome/47.0.2526Safari/537.36\r\n"\
                "Host: %s\r\n"\
                "Range: bytes=%d-\r\n"\
                "Connection: keep-alive\r\n"\
                "\r\n"\
                , relative_url, host_addr, offset);
    }
    else
    {
        sprintf(header, \
                "POST %s HTTP/1.1\r\n"\
                "Cookie:%s\r\n"
                "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537(KHTML, like Gecko) Chrome/47.0.2526Safari/537.36\r\n"\
                "Host: %s\r\n"\
                "Connection: keep-alive\r\n"\
                "Content-Type: multipart/form-data; boundary=--------------------------957323426414134187562641\r\n"\
                "Accept-Encoding: gzip, deflate\r\n"\
                "Content-Length: %d\r\n"\
                "Connection: keep-alive\r\n"\
                "\r\n"\
                , relative_url, cookie, host_addr, *param2);
    }

    if (write(socket_local, header, strlen(header)) == -1)
    {
        HTTP_DBG("write socket failed");
        bSucc = RT_FALSE;
        goto END;
    }

    if (mode == HTTP_POST)
    {
        if (write(socket_local, *param1, *param2) == -1)
        {
            HTTP_DBG("write post data failed");
            bSucc = RT_FALSE;
            goto END;
        }
        HTTP_WRN("http post upload data over");
    }

    int mem_size = HTTP_DOWNLOAD_FILE_NAME_LENGTH;
    int length = 0;
    int len;
    char *response = *param1;
    buf = (char *) rt_malloc(mem_size * sizeof(char));
    int retry = 10;
    while ((len = http_socket_read(socket_local, buf, 1)) != -1)
    {
        if (len == 0)
        {
            if (retry == 0)
                break;
            retry--;
        }
        else
        {
            retry = 10;
        }

        if (length + len >= mem_size)
        {
            mem_size *= 2;
            char *temp = (char *)rt_realloc(response, sizeof(char) * mem_size);
            if (temp == NULL)
            {
                HTTP_ERR("os_malloc failed %d\n", sizeof(char) * mem_size);
                bSucc = RT_FALSE;
                goto END;
            }
            response = temp;
            *param1 = response;
        }

        buf[len] = '\0';
        strcat(response, buf);

        int flag = 0;
        for (i = strlen(response) - 1; response[i] == '\n' || response[i] == '\r'; i--, flag++)
        {
            ;   //do nothing
        }
        if (flag == 4)
        {
            bSucc = RT_TRUE;
            break;
        }
        length += len;
    }

    g_last_not_read_chunk_size = 0;
    g_last_socket_read_size = 0;
    g_chunk_data_over = 0;

END:
    if (download_file_name)
        rt_free(download_file_name);
    if (header)
        rt_free(header);
    if (buf)
        rt_free(buf);

    if (!bSucc)
    {
        if (socket_local != -1)
        {
            close(socket_local);
        }
        return HTTP_STATUS_ERR;
    }
    *param2 = socket_local;

    HTTP_DBG("http open success \n");
    return HTTP_STATUS_OK;
}

int http_socket_read(int socket, char *data, size_t data_len)
{
    int rs = -1;
    int fd = socket;
    int retry_time = 1;
    int has_read = 0;
    int need_read = data_len;

    while (retry_time--)
    {
        fd_set fdr, fde;
        int timeout = 500;
        struct timeval time = {timeout / 1000, (timeout % 1000) * 1000};
        //int ret = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&time, sizeof(struct timeval));
        //if (ret != 0)
        //   HTTP_ERR("set sockops rcvtimeout fail");

        FD_ZERO(&fdr);
        FD_SET(fd, &fdr);

        FD_ZERO(&fde);
        FD_SET(fd, &fde);

        rs = select(fd + 1, &fdr, NULL, &fde, &time);
        if (rs == 0)
        {
            //HTTP_DBG("recv timeout!!!");
        }
        else if (rs < 0 || FD_ISSET(fd, &fde))
        {
            HTTP_ERR("recv failed: rs = %d", rs);
            return -1; //socket err.
        }
        else if (FD_ISSET(fd, &fdr))
        {
            //HTTP_DBG("before recv ...\n");
            //rs = recv(fd, data + has_read, need_read, MSG_WAITALL);
            rs = recv(fd, data + has_read, need_read, 0);
            //HTTP_DBG("recv return  = %u\n", rs);
            if (rs > 0)
            {
                has_read += rs;
                need_read -= rs;
                if (need_read <= 0)
                {
                    break;
                }
            }
        }
    }

    rs = has_read;

    return rs;
}

int http_socket_read_chunk(int socket, char *data, size_t data_len)
{
    size_t read_chunk_data_size = 0;
    size_t bytes_left = g_last_socket_read_size;
    int g_chunk_size = g_last_not_read_chunk_size;

    HTTP_DBG("http_socket_read_chunk,bytes_left:%d,g_last_not_read_chunk_size:%d", \
             g_last_socket_read_size, g_last_not_read_chunk_size);

    g_last_socket_read_size = 0;
    g_last_not_read_chunk_size = 0;

    int have_read = bytes_left;
    int read_size = 0;
    int buffer_size = HTTP_PREPROCESSOR_FRAME_SIZE * 2;

    if (g_chunk_data_over)
        return 0;

    while (read_chunk_data_size < data_len)
    {
        if (have_read < buffer_size)
        {
            read_size =
                http_socket_read(socket, g_chunk_buffer + have_read, buffer_size - have_read);

            if (read_size <= 0)
            {
                HTTP_WRN("cant read socket, maybe finish = %d", read_chunk_data_size);
#if 0
                if (read_chunk_data_size)
                {
                    g_last_socket_read_size = have_read;
                    g_last_not_read_chunk_size = g_chunk_size;
                    return read_chunk_data_size;
                }
#endif
            }
        }
        have_read += read_size;

        HTTP_DBG("1 http_socket_read_chunk,bytes_left:%d", have_read);

retry:
        if (g_chunk_size == 0)
        {
            int cr_lf_pos = 0;
            int is_found_cr_lf = 0;
            while (1)
            {
                for (; cr_lf_pos < have_read - 2; cr_lf_pos++)
                {
                    if (g_chunk_buffer[cr_lf_pos] == '\r' && g_chunk_buffer[cr_lf_pos + 1] == '\n')
                    {
                        is_found_cr_lf = RT_TRUE;
                        break;
                    }
                }
                if (is_found_cr_lf)
                {
                    break;
                }
                if (have_read < data_len)
                {
                    read_size =
                        http_socket_read(socket, g_chunk_buffer + have_read, buffer_size - have_read);

                    if (read_size < 0)
                    {
                        HTTP_WRN("cant read socket1, maybe finish = %d", read_chunk_data_size);
                        //return read_chunk_data_size;
                    }
                    else
                    {
                        have_read += read_size;
                    }
                }
            }
            if (is_found_cr_lf)
            {
                int n = sscanf(g_chunk_buffer, "%x", &g_chunk_size);
                if (n != 1)
                {
                    HTTP_ERR("chunk format error size = 0");
                    return read_chunk_data_size;
                }
            }

            HTTP_WRN("cr_lf_pos:%d is_found_cr_lf:%d, g_chunk_size:%d",
                     cr_lf_pos,
                     is_found_cr_lf,
                     g_chunk_size);
            have_read -= (cr_lf_pos + HTTP_CR_LF_LEN);
            memmove(g_chunk_buffer, &g_chunk_buffer[cr_lf_pos + HTTP_CR_LF_LEN], have_read);

            if (g_chunk_size == 0)
            {
                HTTP_WRN("read last chunk = %d", read_chunk_data_size);
                g_chunk_data_over = 1;
                return read_chunk_data_size;
            }
        }
        else
        {
            if ((read_chunk_data_size + have_read) >= data_len)
            {
                if (have_read < g_chunk_size)
                {
                    memcpy(&data[read_chunk_data_size],
                           g_chunk_buffer,
                           data_len - read_chunk_data_size);
                    g_last_socket_read_size = have_read - (data_len - read_chunk_data_size);
                    memmove(g_chunk_buffer, &g_chunk_buffer[data_len - read_chunk_data_size], g_last_socket_read_size);
                    g_last_not_read_chunk_size =
                        g_chunk_size - (data_len - read_chunk_data_size);
                    read_chunk_data_size = data_len;
                    HTTP_DBG("read chunk data = %d", read_chunk_data_size);
                    return read_chunk_data_size;
                }
            }

            if (have_read >= (g_chunk_size + HTTP_CR_LF_LEN))
            {
                if (g_chunk_buffer[g_chunk_size] != '\r' || g_chunk_buffer[g_chunk_size + 1] != '\n')
                {
                    HTTP_ERR("chunk format error lost \r\n");
                    return read_chunk_data_size;
                }

                if ((read_chunk_data_size + g_chunk_size) < data_len)
                {
                    memcpy(&data[read_chunk_data_size], g_chunk_buffer, g_chunk_size);
                    memmove(g_chunk_buffer,
                            &g_chunk_buffer[g_chunk_size + HTTP_CR_LF_LEN],
                            have_read - (g_chunk_size + HTTP_CR_LF_LEN));
                    read_chunk_data_size += g_chunk_size;
                    //bytes_left = have_read - (g_chunk_size + HTTP_CR_LF_LEN);
                    have_read -= (g_chunk_size + HTTP_CR_LF_LEN);
                    g_chunk_size = 0;
                    goto retry;
                }
                else
                {
                    memcpy(&data[read_chunk_data_size],
                           g_chunk_buffer,
                           data_len - read_chunk_data_size);
                    g_last_socket_read_size = have_read - (data_len - read_chunk_data_size);
                    memmove(g_chunk_buffer, &g_chunk_buffer[data_len - read_chunk_data_size], g_last_socket_read_size);
                    g_last_not_read_chunk_size =
                        g_chunk_size - (data_len - read_chunk_data_size);
                    read_chunk_data_size = data_len;
                    HTTP_DBG("read chunk data = %d", read_chunk_data_size);
                    return read_chunk_data_size;
                }
            }
        }
    }

    return read_chunk_data_size;
}

http_status_t http_close(int socket)
{
    close(socket);
    return HTTP_STATUS_OK;
}
