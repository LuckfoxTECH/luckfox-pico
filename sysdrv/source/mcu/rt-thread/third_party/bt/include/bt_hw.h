// Copyright (c) 2019 Fuzhou Rockchip Electronics Co., Ltd
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "osi/bt_config.h"
//#include "rk_bt.h"
#include <stdlib.h>
#include <stdint.h>
#include "rk_err.h"
#include "osi/thread.h"
#include "osi/types.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef CONFIG_BT_ENABLED
typedef uint8_t uint8;
typedef uint32_t uint32;
typedef uint16_t uint16;

struct bd_addr
{
    uint8 addr[6];
};


#define BD_ADDR_LEN 6




#define BD_ADDR(bdaddr, a, b, c, d, e, f) do{ \
                                        bdaddr->addr[0] = a; \
                        bdaddr ->addr[1] = b; \
                        bdaddr->addr[2] = c; \
                        bdaddr->addr[3] = d; \
                        bdaddr->addr[4] = e; \
                       bdaddr->addr[5] = f; }while(0)


#define bd_addr_set(addr1, addr2) do { \
                                   (addr1)->addr[0] = (addr2)->addr[0]; \
                   (addr1)->addr[1] = (addr2)->addr[1]; \
                   (addr1)->addr[2] = (addr2)->addr[2]; \
                   (addr1)->addr[3] = (addr2)->addr[3]; \
                   (addr1)->addr[4] = (addr2)->addr[4]; \
                   (addr1)->addr[5] = (addr2)->addr[5]; }while(0)


#define bd_addr_cmp(addr1, addr2) (((addr1)->addr[0] == (addr2)->addr[0]) && \
                   ((addr1)->addr[1] == (addr2)->addr[1]) && \
                   ((addr1)->addr[2] == (addr2)->addr[2]) && \
                   ((addr1)->addr[3] == (addr2)->addr[3]) && \
                   ((addr1)->addr[4] == (addr2)->addr[4]) && \
                   ((addr1)->addr[5] == (addr2)->addr[5]))

typedef int HDC;

#define ERR_OK          0    /* No error, everything OK. */
#define ERR_MEM        -1    /* Out of memory error.     */
#define ERR_BUF        -2    /* Buffer error.            */
#define ERR_TIMEOUT    -3    /* Timeout.                 */
#define ERR_RTE        -4    /* Routing problem. */

#ifdef __cplusplus
}
#endif

#endif /* __RK_BT_H__ */
