/*
 * Copyright (c) 2015 iComm-semi Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _DEBUG_FS_H_
#define _DEBUG_FS_H_

#define CLI_ARG_SIZE                10
#define PROC_DIR_ENTRY              "ssv"
#define PROC_SSV_CMD_ENTRY          "ssv_cmd"

struct ssv_cmd_table {
    const char *cmd;
    int (*cmd_func_ptr)(void *, int, char **);
    const char *usage;
    const int  result_buffer_size;
};

struct ssv_cmd_data {
    char *ssv6xxx_result_buf;
    u32  rsbuf_len;
    u32  rsbuf_size;
    bool cmd_in_proc;
    struct proc_dir_entry *proc_dev_entry;
};

void ssv_snprintf_res(struct ssv_cmd_data *cmd_data, const char *fmt, ... );

#endif /* _DEBUG_FS_H_ */


