/* Copyright 2020 Rockchip Electronics Co. LTD
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
 * author: rimon.xu@rock-chips.com
 *   date: 2020-11-06
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "rk_debug.h"
#include "rk_comm_mb.h"
#include "rk_comm_aio.h"
#include "rk_comm_vdec.h"
#include "rk_mpi_sys.h"
#include "rk_mpi_mb.h"

#include "test_comm_argparse.h"

enum _testAllocType {
    TEST_ALLOC_TYPE_INTERNAL = 0,
    TEST_ALLOC_TYPE_EXTERNAL_HAS_FREE,
    TEST_ALLOC_TYPE_EXTERNAL
};

RK_S32 test_dump_vstream_info(VDEC_STREAM_S *vstream) {
    MB_BLK mb = vstream->pMbBlk;
    RK_LOGI("test dump vtsream info:");
    RK_LOGI("mb (%p), data(%p), fd(%d), len(%d), pts(%lld), eos(%d)",
             mb, RK_MPI_MB_Handle2VirAddr(mb), RK_MPI_MB_Handle2Fd(mb),
             vstream->u32Len, vstream->u64PTS, vstream->bEndOfStream);
    return RK_SUCCESS;
}

RK_S32 test_video_stream_buffer_free(void *ctx) {
    if (ctx) {
        free(ctx);
        ctx = RK_NULL;
    }
    return RK_SUCCESS;
}

RK_S32 test_video_stream(RK_S32 type) {
    VDEC_STREAM_S vstream;
    switch (type) {
      case TEST_ALLOC_TYPE_INTERNAL: {
          vstream.bEndOfStream = RK_FALSE;
          vstream.u32Len       = 0;
          vstream.u64PTS       = 50;
          RK_MPI_SYS_MmzAlloc(&(vstream.pMbBlk), RK_NULL, RK_NULL, 250);

          // do something.....
          test_dump_vstream_info(&vstream);

          RK_MPI_MB_ReleaseMB(vstream.pMbBlk);
      } break;
      case TEST_ALLOC_TYPE_EXTERNAL: {
          RK_U8 *data          = reinterpret_cast<RK_U8 *>(malloc(250));
          vstream.bEndOfStream = RK_FALSE;
          vstream.u32Len       = 100;
          vstream.u64PTS       = 50;
          MB_EXT_CONFIG_S extConfig = {0};
          extConfig.pu8VirAddr = data;
          extConfig.u64Size    = 250;
          RK_MPI_SYS_CreateMB(&(vstream.pMbBlk), &extConfig);

          // do something.....
          test_dump_vstream_info(&vstream);

          RK_MPI_SYS_Free(vstream.pMbBlk);
          if (data) {
              free(data);
              data = RK_NULL;
          }
      } break;
      case TEST_ALLOC_TYPE_EXTERNAL_HAS_FREE: {
          vstream.bEndOfStream = RK_FALSE;
          vstream.u32Len       = 100;
          vstream.u64PTS       = 50;
          MB_EXT_CONFIG_S extConfig = {0};
          extConfig.pu8VirAddr = reinterpret_cast<RK_U8 *>(malloc(250));
          extConfig.u64Size    = 250;
          extConfig.pFreeCB    = test_video_stream_buffer_free;
          extConfig.pOpaque  = extConfig.pu8VirAddr;
          RK_MPI_SYS_CreateMB(&(vstream.pMbBlk), &extConfig);
          // do something.....
          test_dump_vstream_info(&vstream);
          RK_MPI_SYS_Free(vstream.pMbBlk);
      } break;
      default:
        break;
    }

    return RK_SUCCESS;
}

RK_S32 unit_test_mpi_vaio_struct() {
    for (RK_S32 i = 0; i < 3; i++) {
        test_video_stream(i);
    }
    return RK_SUCCESS;
}


static const char *const usages[] = {
    "[options]: ",
    NULL,
};

RK_S32 main(int argc, const char **argv) {
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("basic options:"),
        OPT_END(),
    };

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nselect a test case to run.",
                                 "\nuse --help for details.");

    argc = argparse_parse(&argparse, argc, argv);
    argparse_usage(&argparse);

    unit_test_mpi_vaio_struct();

    return 0;
}

