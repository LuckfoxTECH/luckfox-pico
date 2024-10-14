#include <stdint.h>

#include <rthw.h>
#include <rtthread.h>

#ifdef RT_USING_COMMON_TEST_RPMSG_LITE

#include "hal_base.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"


#define MASTER_ID   ((uint32_t)1)
#define REMOTE_ID_1 ((uint32_t)0)
#define REMOTE_ID_2 ((uint32_t)2)
#define REMOTE_ID_3 ((uint32_t)3)

#define RPMSG_CMD_PROB ((uint8_t)0x80)
#define RPMSG_ACK_PROB ((uint8_t)0x81)
#define RPMSG_CMD_TEST ((uint8_t)0x82)
#define RPMSG_ACK_TEST ((uint8_t)0x83)

extern uint32_t __share_rpmsg_start__[];
extern uint32_t __share_rpmsg_end__[];

#define RPMSG_MEM_BASE ((uint32_t)&__share_rpmsg_start__)
#define RPMSG_MEM_END  ((uint32_t)&__share_rpmsg_end__)

#define EPT_M2R_ADDR(addr) (addr + VRING_SIZE)  // covert master endpoint number to remote endpoint number
#define EPT_R2M_ADDR(addr) (addr - VRING_SIZE)  // covert remote endpoint number to master endpoint number

struct rpmsg_block_t
{
    uint32_t len;
    uint8_t buffer[32 - 4];
};

struct rpmsg_ept_map_t
{
    uint32_t base;          // share memory base addr
    uint32_t size;          // share memory size
    uint32_t m_ept_addr;    // master endpoint number
    uint32_t r_ept_addr;    // remote endpoint number
};

struct rpmsg_info_t
{
    struct rpmsg_lite_instance *instance;
    struct rpmsg_lite_endpoint *ept;
    struct rpmsg_ept_map_t *map;
    rpmsg_queue_handle queue;
    uint32_t s_addr;        // source endpoint number
    void *private;
};

#define RPMSG_TEST_MEM_SIZE (2UL * RL_VRING_OVERHEAD)
#define RPMSG_TEST0_BASE    (RPMSG_MEM_BASE + 0 * RPMSG_TEST_MEM_SIZE)
#define RPMSG_TEST1_BASE    (RPMSG_MEM_BASE + 1 * RPMSG_TEST_MEM_SIZE)
#define RPMSG_TEST2_BASE    (RPMSG_MEM_BASE + 2 * RPMSG_TEST_MEM_SIZE)
#define RPMSG_TEST3_BASE    (RPMSG_MEM_BASE + 3 * RPMSG_TEST_MEM_SIZE)

// define endpoint number for test
#define RPMSG_TEST0_EPT 0x80000000UL
#define RPMSG_TEST1_EPT 0x80000001UL
#define RPMSG_TEST2_EPT 0x80000002UL
#define RPMSG_TEST3_EPT 0x80000003UL

static struct rpmsg_ept_map_t rpmsg_ept_map_table[4] =
{
    { RPMSG_TEST0_BASE, RPMSG_TEST_MEM_SIZE, RPMSG_TEST0_EPT, EPT_M2R_ADDR(RPMSG_TEST0_EPT) },
    { RPMSG_TEST1_BASE, RPMSG_TEST_MEM_SIZE, RPMSG_TEST1_EPT, EPT_M2R_ADDR(RPMSG_TEST1_EPT) },
    { RPMSG_TEST2_BASE, RPMSG_TEST_MEM_SIZE, RPMSG_TEST2_EPT, EPT_M2R_ADDR(RPMSG_TEST2_EPT) },
    { RPMSG_TEST3_BASE, RPMSG_TEST_MEM_SIZE, RPMSG_TEST3_EPT, EPT_M2R_ADDR(RPMSG_TEST3_EPT) },
};

static void rpmsg_share_mem_check(void)
{
    if ((RPMSG_TEST3_BASE + RPMSG_TEST_MEM_SIZE) > RPMSG_MEM_END)
    {
        rt_kprintf("share memory size error: (RPMSG_TEST3_BASE + RPMSG_TEST_MEM_SIZE)(0x%x08x) > RPMSG_MEM_END(0x%x08x)\n",
                   RPMSG_TEST3_BASE + RPMSG_TEST_MEM_SIZE, RPMSG_MEM_END);
        while (1)
        {
            ;
        }
    }
}

static uint32_t remote_id_table[3] = { REMOTE_ID_1, REMOTE_ID_2, REMOTE_ID_3 };
static uint32_t rpmsg_get_remote_index(uint32_t cpu_id)
{
    uint32_t i;

    for (i = 0; i < 3; i++)
    {
        if (remote_id_table[i] == cpu_id)
        {
            return i;
        }
    }

    return -1;
}

#if defined(PRIMARY_CPU) /*CPU1*/
static struct rpmsg_info_t *p_rpmsg_info[3];
static void rpmsg_master_init(void)
{
    uint32_t i;
    uint32_t master_id, remote_id;
    struct rpmsg_info_t *info;

    rpmsg_share_mem_check();

    master_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    /****************** Initial rpmsg ept **************/
    for (i = 0; i < 3; i++)
    {
        remote_id = remote_id_table[i];

        info = rt_malloc(sizeof(struct rpmsg_info_t));
        if (info == NULL)
        {
            rt_kprintf("info malloc error!\n");
            while (1)
            {
                ;
            }
        }
        info->private = rt_malloc(sizeof(struct rpmsg_block_t));
        if (info->private == NULL)
        {
            rt_kprintf("info malloc error!\n");
            while (1)
            {
                ;
            }
        }

        info->map = &rpmsg_ept_map_table[remote_id];
        info->instance = rpmsg_master_get_instance(master_id, remote_id);
        info->queue = rpmsg_queue_create(info->instance);
        info->ept = rpmsg_lite_create_ept(info->instance, info->map->m_ept_addr, rpmsg_queue_rx_cb, info->queue);

        p_rpmsg_info[i] = info;
    }
}

static void rpmsg_master_deinit(void)
{
    uint32_t i;

    for (i = 0; i < 3; i++)
    {
        rt_free(p_rpmsg_info[i]->private);
        rt_free(p_rpmsg_info[i]);
    }
}

static void rpmsg_master_test(void)
{
    uint32_t i, j;
    uint32_t master_id, remote_id;
    struct rpmsg_info_t *info;
    struct rpmsg_block_t block, *rblock;

    rpmsg_master_init();

    master_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    /** probe remote ept, wait for remote ept initialized **/
    block.buffer[0] = RPMSG_CMD_PROB;
    block.buffer[1] = (uint8_t)master_id;
    block.len = 2;

    for (i = 0; i < 3; i++)
    {
        info = p_rpmsg_info[i];
        remote_id = remote_id_table[i];
        rblock = (struct rpmsg_block_t *)info->private;

        for (j = 0; j < 20; j++)
        {
            rpmsg_lite_send(info->instance, info->ept, info->map->r_ept_addr, block.buffer, block.len, 10/*RL_BLOCK*/);

            /* example to use rpmsg_queue_recv() with data copy, the received data auto copy to "rblock->buffer" */
            if (RL_SUCCESS == rpmsg_queue_recv(info->instance, info->queue,
                                               &info->s_addr, rblock->buffer,
                                               RL_BUFFER_PAYLOAD_SIZE, &rblock->len, 10/*RL_BLOCK*/))
            {
                if (rblock->buffer[0] == RPMSG_ACK_PROB)
                {
                    rt_kprintf("rpmsg probe remote cpu(%d) ept(0x%08x) sucess!\n", rblock->buffer[1], info->s_addr);
                    break;
                }
            }
        }
        if (j >= 20)
        {
            rt_kprintf("rpmsg probe remote cpu(%d) error!\n", remote_id);
        }
    }

    /****************** rpmsg test run **************/
    block.buffer[0] = RPMSG_CMD_TEST;
    block.buffer[1] = (uint8_t)master_id;
    block.buffer[2] = 0x55;
    block.buffer[3] = 0x55;
    block.buffer[4] = 0x55;
    block.len = 5;
    for (i = 0; i < 3; i++)
    {
        info = p_rpmsg_info[i];
        remote_id = remote_id_table[i];
        rt_kprintf("rpmsg_master_send: master[%d]-->remote[%d], remote ept addr = 0x%08x\n", master_id, remote_id, info->map->r_ept_addr);
        rpmsg_lite_send(info->instance, info->ept, info->map->r_ept_addr, block.buffer, block.len, RL_BLOCK);

        for (j = 0; j < 20; j++)
        {
            uint32_t recv_len;
            uint8_t *recv_data;
            rblock = (struct rpmsg_block_t *)info->private;

            /* example to use rpmsg_queue_recv_nocopy(), received data point to "recv_data" */
            if (RL_SUCCESS == rpmsg_queue_recv_nocopy(info->instance, info->queue,
                    &info->s_addr, &recv_data,
                    &recv_len, 10/*RL_BLOCK*/))
            {
                rblock->len = recv_len;
                rt_memcpy(rblock->buffer, recv_data, recv_len);   // user must copy data to buffer by use "nocopy"

                if (rblock->buffer[0] == RPMSG_ACK_TEST)
                {
                    remote_id = rblock->buffer[1];
                    block.buffer[i + 2] = rblock->buffer[i + 2];
                    rt_kprintf("rpmsg_master_recv: master[%d]<--remote[%d], remote ept addr = 0x%08x\n", master_id, remote_id, info->s_addr);
                    break;
                }
            }
        }
        if (j >= 20)
        {
            rt_kprintf("rpmsg test remote cpu(%d) error!\n", remote_id);
        }
    }

    if ((block.buffer[0 + 2] == 0xff) &&
            (block.buffer[1 + 2] == 0xff) &&
            (block.buffer[2 + 2] == 0xff))
    {
        rt_kprintf("rpmsg test OK!\n");
    }
}

#ifdef RT_USING_FINSH
#include <finsh.h>
MSH_CMD_EXPORT(rpmsg_master_test, rpmsg_test test for driver);
#endif

//#endif
#else
//#if defined(CPU0)// || defined(CPU2) || defined(CPU3)

static void rpmsg_remote_test(void)
{
    uint32_t i, master_id, remote_id;
    uint32_t recv_len;
    struct rpmsg_info_t *info;
    struct rpmsg_block_t *block;

    rpmsg_share_mem_check();

    master_id = MASTER_ID;
    remote_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();

    info = rt_malloc(sizeof(struct rpmsg_info_t));
    if (info == NULL)
    {
        rt_kprintf("info malloc error!\n");
        while (1)
        {
            ;
        }
    }
    info->private = rt_malloc(sizeof(struct rpmsg_block_t));
    if (info->private == NULL)
    {
        rt_kprintf("info malloc error!\n");
        while (1)
        {
            ;
        }
    }

    info->map = &rpmsg_ept_map_table[remote_id];
    info->instance = rpmsg_remote_get_instance(master_id, remote_id);
    info->queue = rpmsg_queue_create(info->instance);
    info->ept = rpmsg_lite_create_ept(info->instance, info->map->r_ept_addr, rpmsg_queue_rx_cb, info->queue);

    while (1)
    {
        block = (struct rpmsg_block_t *)info->private;
        if (RL_SUCCESS == rpmsg_queue_recv(info->instance, info->queue,
                                           &info->s_addr, &block->buffer,
                                           RL_BUFFER_PAYLOAD_SIZE, &block->len, RL_BLOCK))
        {
            // CMD(ACK): RPMSG_CMD_PROB
            if (block->buffer[0] == RPMSG_CMD_PROB)
            {
                block->buffer[0] = RPMSG_ACK_PROB;
                block->buffer[1] = remote_id;
                block->len = 2;
                rpmsg_lite_send(info->instance, info->ept, info->map->m_ept_addr, block->buffer, block->len, RL_BLOCK);
            }
            // CMD(ACK): RPMSG_CMD_TEST
            else if (block->buffer[0] == RPMSG_CMD_TEST)
            {
                rt_kprintf("rpmsg_remote_recv: remote[%d]<--master[%d], master ept addr = 0x%08x\n", remote_id, block->buffer[1], info->s_addr);

                block->buffer[0] = RPMSG_ACK_TEST;
                block->buffer[1] = remote_id;

                i = rpmsg_get_remote_index(remote_id);
                block->buffer[i + 2] |= 0xaa;

                rt_kprintf("rpmsg_remote_send: remote[%d]-->master[%d], master ept addr = 0x%08x\n", remote_id, master_id, info->map->m_ept_addr);
                rpmsg_lite_send(info->instance, info->ept, info->map->m_ept_addr, block->buffer, block->len, RL_BLOCK);
            }
            // CMD(ACK): ......
            else
            {
                //......
            }
        }
    }

    rt_free(info->private);
    rt_free(info);
}

INIT_APP_EXPORT(rpmsg_remote_test);

#endif

#endif  //RT_USING_COMMON_TEST_RPMSG_LITE

#ifdef RT_USING_COMMON_TEST_LINUX_RPMSG_LITE

#include "hal_base.h"
#include "rpmsg_lite.h"
#include "rpmsg_queue.h"
#include "rpmsg_ns.h"

// test is CPU0 as master and CPU3 as remote.
#define MASTER_ID   ((uint32_t)0)
#define REMOTE_ID_3 ((uint32_t)3)

// define endpoint id for test
#define RPMSG_RTT_REMOTE_TEST3_EPT_ID 0x3003U
#define RPMSG_RTT_REMOTE_TEST_EPT3_NAME "rpmsg-ap3-ch0"

#define RPMSG_RTT_TEST_MSG "Rockchip rpmsg linux test!"

/* TODO: These are defined in the linked script gcc_arm.ld.S */
extern uint32_t __linux_share_rpmsg_start__[];
extern uint32_t __linux_share_rpmsg_end__[];

#define RPMSG_LINUX_MEM_BASE ((uint32_t)&__linux_share_rpmsg_start__)
#define RPMSG_LINUX_MEM_END  ((uint32_t)&__linux_share_rpmsg_end__)
#define RPMSG_LINUX_MEM_SIZE (2UL * RL_VRING_OVERHEAD)

struct rpmsg_block_t
{
    uint32_t len;
    uint8_t buffer[496 - 4];
};

struct rpmsg_info_t
{
    struct rpmsg_lite_instance *instance;
    struct rpmsg_lite_endpoint *ept;
    uint32_t cb_sta;    // callback status flags
    void *private;
};

static void rpmsg_share_mem_check(void)
{
    if ((RPMSG_LINUX_MEM_BASE + RPMSG_LINUX_MEM_SIZE) > RPMSG_LINUX_MEM_END)
    {
        rt_kprintf("share memory size error!\n");
        while (1)
        {
            ;
        }
    }
}

rpmsg_ns_new_ept_cb rpmsg_ns_cb(uint32_t new_ept, const char *new_ept_name, uint32_t flags, void *user_data)
{
    uint32_t cpu_id;
    char ept_name[RL_NS_NAME_SIZE];

    cpu_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    strncpy(ept_name, new_ept_name, RL_NS_NAME_SIZE);
    printf("rpmsg remote: name service callback cpu_id-%ld\n", cpu_id);
    printf("rpmsg remote: new_ept-0x%lx name-%s\n", new_ept, ept_name);
}

static void rpmsg_linux_test(void)
{
    int j;
    uint32_t master_id, remote_id;
    struct rpmsg_info_t *info;
    struct rpmsg_block_t *block;
    rpmsg_queue_handle remote_queue;
    char *rx_msg = (char *)rt_malloc(RL_BUFFER_PAYLOAD_SIZE);
    uint32_t master_ept_id;
    uint32_t ept_flags;
    void *ns_cb_data;

    rpmsg_share_mem_check();
    master_id = MASTER_ID;
    remote_id = HAL_CPU_TOPOLOGY_GetCurrentCpuId();
    rt_kprintf("rpmsg remote: remote core cpu_id-%ld\n", remote_id);
    rt_kprintf("rpmsg remote: shmem_base-0x%lx shmem_end-%lx\n", RPMSG_LINUX_MEM_BASE, RPMSG_LINUX_MEM_END);

    info = malloc(sizeof(struct rpmsg_info_t));
    if (info == NULL)
    {
        rt_kprintf("info malloc error!\n");
        while (1)
        {
            ;
        }
    }
    info->private = malloc(sizeof(struct rpmsg_block_t));
    if (info->private == NULL)
    {
        rt_kprintf("info malloc error!\n");
        while (1)
        {
            ;
        }
    }

    info->instance = rpmsg_lite_remote_init((void *)RPMSG_LINUX_MEM_BASE, RL_PLATFORM_SET_LINK_ID(master_id, remote_id), RL_NO_FLAGS);
    rpmsg_lite_wait_for_link_up(info->instance);
    rt_kprintf("rpmsg remote: link up! link_id-0x%lx\n", info->instance->link_id);
    rpmsg_ns_bind(info->instance, rpmsg_ns_cb, &ns_cb_data);
    remote_queue  = rpmsg_queue_create(info->instance);
    info->ept = rpmsg_lite_create_ept(info->instance, RPMSG_RTT_REMOTE_TEST3_EPT_ID, rpmsg_queue_rx_cb, remote_queue);
    ept_flags = RL_NS_CREATE;
    rpmsg_ns_announce(info->instance, info->ept, RPMSG_RTT_REMOTE_TEST_EPT3_NAME, ept_flags);

    /****************** rpmsg test run **************/
    for (j = 0; j < 100; j++)
    {
        rpmsg_queue_recv(info->instance, remote_queue, (uint32_t *)&master_ept_id, rx_msg, RL_BUFFER_PAYLOAD_SIZE, RL_NULL, RL_BLOCK);
        //    rpmsg_queue_recv_nocopy(remote_rpmsg, remote_queue, (uint32_t *)&master_ept_id, (char **)&rx_msg, RL_NULL, RL_BLOCK);
        //rt_kprintf("rpmsg remote: master_ept_id-0x%lx rx_msg: %s\n", master_ept_id, rx_msg);
        rpmsg_lite_send(info->instance, info->ept, master_ept_id, RPMSG_RTT_TEST_MSG, strlen(RPMSG_RTT_TEST_MSG), RL_BLOCK);
    }
}

INIT_APP_EXPORT(rpmsg_linux_test);
#endif  //RT_USING_COMMON_TEST_LINUX_RPMSG_LITE