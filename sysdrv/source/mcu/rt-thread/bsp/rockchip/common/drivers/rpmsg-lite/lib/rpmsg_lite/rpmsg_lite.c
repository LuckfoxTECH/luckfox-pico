/*
 * Copyright (c) 2014, Mentor Graphics Corporation
 * Copyright (c) 2015 Xilinx, Inc.
 * Copyright (c) 2016 Freescale Semiconductor, Inc.
 * Copyright 2016-2022 NXP
 * Copyright 2021 ACRIOS Systems s.r.o.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "rpmsg_lite.h"
#include "rpmsg_platform.h"

/* rpmsg_std_hdr contains a reserved field,
 * this implementation of RPMSG uses this reserved
 * field to hold the idx and totlen of the buffer
 * not being returned to the vring in the receive
 * callback function. This way, the no-copy API
 * can use this field to return the buffer later.
 */
struct rpmsg_hdr_reserved
{
    uint16_t rfu; /* reserved for future usage */
    uint16_t idx;
};

RL_PACKED_BEGIN
/*!
 * Common header for all rpmsg messages.
 * Every message sent/received on the rpmsg bus begins with this header.
 */
struct rpmsg_std_hdr
{
    uint32_t src;                       /*!< source endpoint address */
    uint32_t dst;                       /*!< destination endpoint address */
    struct rpmsg_hdr_reserved reserved; /*!< reserved for future use */
    uint16_t len;                       /*!< length of payload (in bytes) */
    uint16_t flags;                     /*!< message flags */
} RL_PACKED_END;

RL_PACKED_BEGIN
/*!
 * Common message structure.
 * Contains the header and the payload.
 */
struct rpmsg_std_msg
{
    struct rpmsg_std_hdr hdr; /*!< RPMsg message header */
    uint8_t data[1];          /*!< bytes of message payload data */
} RL_PACKED_END;

/* Interface which is used to interact with the virtqueue layer,
 * a different interface is used, when the local processor is the MASTER
 * and when it is the REMOTE.
 */
struct virtqueue_ops
{
    void (*vq_tx)(struct virtqueue *vq, void *buffer, uint32_t len, uint16_t idx);
    void *(*vq_tx_alloc)(struct virtqueue *vq, uint32_t *len, uint16_t *idx);
    void *(*vq_rx)(struct virtqueue *vq, uint32_t *len, uint16_t *idx);
    void (*vq_rx_free)(struct virtqueue *vq, void *buffer, uint32_t len, uint16_t idx);
};

/* Zero-Copy extension macros */
#define RPMSG_STD_MSG_FROM_BUF(buf) (struct rpmsg_std_msg *)(void *)((char *)(buf)-offsetof(struct rpmsg_std_msg, data))

#if !(defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1))
/* Check RL_BUFFER_COUNT and RL_BUFFER_SIZE only when RL_ALLOW_CUSTOM_SHMEM_CONFIG is not set to 1 */
#if (!RL_BUFFER_COUNT) || (RL_BUFFER_COUNT & (RL_BUFFER_COUNT - 1))
#error "RL_BUFFER_COUNT must be power of two (2, 4, ...)"
#endif

/* Buffer is formed by payload and struct rpmsg_std_hdr */
#define RL_BUFFER_SIZE (RL_BUFFER_PAYLOAD_SIZE + 16UL)

#if (!RL_BUFFER_SIZE) || (RL_BUFFER_SIZE & (RL_BUFFER_SIZE - 1))
#error \
    "RL_BUFFER_SIZE must be power of two (256, 512, ...)"\
       "RL_BUFFER_PAYLOAD_SIZE must be equal to (240, 496, 1008, ...) [2^n - 16]."
#endif
#endif /* !(defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)) */

/*!
 * @brief
 * Traverse the linked list of endpoints to get the one with defined address.
 *
 * @param rpmsg_lite_dev    RPMsg Lite instance
 * @param addr              Local endpoint address
 *
 * @return       RL_NULL if not found, node pointer containing the ept on success
 *
 */
static struct llist *rpmsg_lite_get_endpoint_from_addr(struct rpmsg_lite_instance *rpmsg_lite_dev, uint32_t addr)
{
    struct llist *rl_ept_lut_head;

    rl_ept_lut_head = rpmsg_lite_dev->rl_endpoints;
    while (rl_ept_lut_head != RL_NULL)
    {
        struct rpmsg_lite_endpoint *rl_ept = (struct rpmsg_lite_endpoint *)rl_ept_lut_head->data;
        if (rl_ept->addr == addr)
        {
            return rl_ept_lut_head;
        }
        rl_ept_lut_head = rl_ept_lut_head->next;
    }
    return RL_NULL;
}

/***************************************************************
   mmm    mm   m      m      mmmmm    mm     mmm  m    m  mmmm
 m"   "   ##   #      #      #    #   ##   m"   " #  m"  #"   "
 #       #  #  #      #      #mmmm"  #  #  #      #m#    "#mmm
 #       #mm#  #      #      #    #  #mm#  #      #  #m      "#
  "mmm" #    # #mmmmm #mmmmm #mmmm" #    #  "mmm" #   "m "mmm#"
****************************************************************/

/*!
 * @brief
 * Called when remote side calls virtqueue_kick()
 * at its transmit virtqueue.
 * In this callback, the buffer is read-out
 * of the rvq and user callback is called.
 *
 * @param vq  Virtqueue affected by the kick
 *
 */
static void rpmsg_lite_rx_callback(struct virtqueue *vq)
{
    struct rpmsg_std_msg *rpmsg_msg;
    uint32_t len;
    uint16_t idx;
    struct rpmsg_lite_endpoint *ept;
    int32_t cb_ret;
    struct llist *node;
    struct rpmsg_lite_instance *rpmsg_lite_dev = (struct rpmsg_lite_instance *)vq->priv;
#if defined(RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION) && (RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION == 1)
    uint32_t rx_freed = RL_FALSE;
#endif

    RL_ASSERT(rpmsg_lite_dev != RL_NULL);

#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    env_lock_mutex(rpmsg_lite_dev->lock);
#endif

    /* Process the received data from remote node */
    rpmsg_msg = (struct rpmsg_std_msg *)rpmsg_lite_dev->vq_ops->vq_rx(rpmsg_lite_dev->rvq, &len, &idx);

    while (rpmsg_msg != RL_NULL)
    {
        node = rpmsg_lite_get_endpoint_from_addr(rpmsg_lite_dev, rpmsg_msg->hdr.dst);

        cb_ret = RL_RELEASE;
        if (node != RL_NULL)
        {
            ept    = (struct rpmsg_lite_endpoint *)node->data;
            cb_ret = ept->rx_cb(rpmsg_msg->data, rpmsg_msg->hdr.len, rpmsg_msg->hdr.src, ept->rx_cb_data);
        }

        if (cb_ret == RL_HOLD)
        {
            rpmsg_msg->hdr.reserved.idx = idx;
        }
        else
        {
            rpmsg_lite_dev->vq_ops->vq_rx_free(rpmsg_lite_dev->rvq, rpmsg_msg, len, idx);
#if defined(RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION) && (RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION == 1)
            rx_freed = RL_TRUE;
#endif
        }
        rpmsg_msg = (struct rpmsg_std_msg *)rpmsg_lite_dev->vq_ops->vq_rx(rpmsg_lite_dev->rvq, &len, &idx);
#if defined(RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION) && (RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION == 1)
        if ((rpmsg_msg == RL_NULL) && (rx_freed == RL_TRUE))
        {
            /* Let the remote device know that some buffers have been freed */
            virtqueue_kick(rpmsg_lite_dev->rvq);
        }
#endif
    }

#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    env_unlock_mutex(rpmsg_lite_dev->lock);
#endif
}

/*!
 * @brief
 * Called when remote side calls virtqueue_kick()
 * at its receive virtqueue.
 *
 * @param vq  Virtqueue affected by the kick
 *
 */
static void rpmsg_lite_tx_callback(struct virtqueue *vq)
{
    struct rpmsg_lite_instance *rpmsg_lite_dev = (struct rpmsg_lite_instance *)vq->priv;

    RL_ASSERT(rpmsg_lite_dev != RL_NULL);
    rpmsg_lite_dev->link_state = 1U;
    env_tx_callback(rpmsg_lite_dev->link_id);
}

/****************************************************************************

 m    m  mmmm         m    m   mm   mm   m mmmm   m      mmmmm  mm   m   mmm
 "m  m" m"  "m        #    #   ##   #"m  # #   "m #        #    #"m  # m"   "
  #  #  #    #        #mmmm#  #  #  # #m # #    # #        #    # #m # #   mm
  "mm"  #    #        #    #  #mm#  #  # # #    # #        #    #  # # #    #
   ##    #mm#"        #    # #    # #   ## #mmm"  #mmmmm mm#mm  #   ##  "mmm"
            #
 In case this processor has the REMOTE role
*****************************************************************************/
/*!
 * @brief
 * Places buffer on the virtqueue for consumption by the other side.
 *
 * @param vq      Virtqueue to use
 * @param buffer  Buffer pointer
 * @param len     Buffer length
 * @idx           Buffer index
 *
 * @return Status of function execution
 *
 */
static void vq_tx_remote(struct virtqueue *tvq, void *buffer, uint32_t len, uint16_t idx)
{
    int32_t status;
    status = virtqueue_add_consumed_buffer(tvq, idx, len);
    RL_ASSERT(status == VQUEUE_SUCCESS); /* must success here */

    /* As long as the length of the virtqueue ring buffer is not shorter
     * than the number of buffers in the pool, this function should not fail.
     * This condition is always met, so we don't need to return anything here */
}

/*!
 * @brief
 * Provides buffer to transmit messages.
 *
 * @param vq      Virtqueue to use
 * @param len     Length of returned buffer
 * @param idx     Buffer index
 *
 * return Pointer to buffer.
 */
static void *vq_tx_alloc_remote(struct virtqueue *tvq, uint32_t *len, uint16_t *idx)
{
    return virtqueue_get_available_buffer(tvq, idx, len);
}

/*!
 * @brief
 * Retrieves the received buffer from the virtqueue.
 *
 * @param vq   Virtqueue to use
 * @param len  Size of received buffer
 * @param idx  Index of buffer
 *
 * @return  Pointer to received buffer
 *
 */
static void *vq_rx_remote(struct virtqueue *rvq, uint32_t *len, uint16_t *idx)
{
    return virtqueue_get_available_buffer(rvq, idx, len);
}

/*!
 * @brief
 * Places the used buffer back on the virtqueue.
 *
 * @param vq   Virtqueue to use
 * @param len  Size of received buffer
 * @param idx  Index of buffer
 *
 */
static void vq_rx_free_remote(struct virtqueue *rvq, void *buffer, uint32_t len, uint16_t idx)
{
    int32_t status;
#if defined(RL_CLEAR_USED_BUFFERS) && (RL_CLEAR_USED_BUFFERS == 1)
    env_memset(buffer, 0x00, len);
#endif
    status = virtqueue_add_consumed_buffer(rvq, idx, len);
    RL_ASSERT(status == VQUEUE_SUCCESS); /* must success here */
                                         /* As long as the length of the virtqueue ring buffer is not shorter
                                          * than the number of buffers in the pool, this function should not fail.
                                          * This condition is always met, so we don't need to return anything here */
}

/****************************************************************************

 m    m  mmmm         m    m   mm   mm   m mmmm   m      mmmmm  mm   m   mmm
 "m  m" m"  "m        #    #   ##   #"m  # #   "m #        #    #"m  # m"   "
  #  #  #    #        #mmmm#  #  #  # #m # #    # #        #    # #m # #   mm
  "mm"  #    #        #    #  #mm#  #  # # #    # #        #    #  # # #    #
   ##    #mm#"        #    # #    # #   ## #mmm"  #mmmmm mm#mm  #   ##  "mmm"
            #
 In case this processor has the MASTER role
*****************************************************************************/

/*!
 * @brief
 * Places buffer on the virtqueue for consumption by the other side.
 *
 * @param vq      Virtqueue to use
 * @param buffer  Buffer pointer
 * @param len     Buffer length
 * @idx           Buffer index
 *
 * @return Status of function execution
 *
 */
static void vq_tx_master(struct virtqueue *tvq, void *buffer, uint32_t len, uint16_t idx)
{
    int32_t status;
    status = virtqueue_add_buffer(tvq, idx);
    RL_ASSERT(status == VQUEUE_SUCCESS); /* must success here */

    /* As long as the length of the virtqueue ring buffer is not shorter
     * than the number of buffers in the pool, this function should not fail.
     * This condition is always met, so we don't need to return anything here */
}

/*!
 * @brief
 * Provides buffer to transmit messages.
 *
 * @param vq      Virtqueue to use
 * @param len     Length of returned buffer
 * @param idx     Buffer index
 *
 * return Pointer to buffer.
 */
static void *vq_tx_alloc_master(struct virtqueue *tvq, uint32_t *len, uint16_t *idx)
{
    return virtqueue_get_buffer(tvq, len, idx);
}

/*!
 * @brief
 * Retrieves the received buffer from the virtqueue.
 *
 * @param vq   Virtqueue to use
 * @param len  Size of received buffer
 * @param idx  Index of buffer
 *
 * @return  Pointer to received buffer
 *
 */
static void *vq_rx_master(struct virtqueue *rvq, uint32_t *len, uint16_t *idx)
{
    return virtqueue_get_buffer(rvq, len, idx);
}

/*!
 * @brief
 * Places the used buffer back on the virtqueue.
 *
 * @param vq   Virtqueue to use
 * @param len  Size of received buffer
 * @param idx  Index of buffer
 *
 */
static void vq_rx_free_master(struct virtqueue *rvq, void *buffer, uint32_t len, uint16_t idx)
{
    int32_t status;
#if defined(RL_CLEAR_USED_BUFFERS) && (RL_CLEAR_USED_BUFFERS == 1)
    env_memset(buffer, 0x00, len);
#endif
    status = virtqueue_add_buffer(rvq, idx);
    RL_ASSERT(status == VQUEUE_SUCCESS); /* must success here */

    /* As long as the length of the virtqueue ring buffer is not shorter
     * than the number of buffers in the pool, this function should not fail.
     * This condition is always met, so we don't need to return anything here */
}

/* Interface used in case this processor is MASTER */
static const struct virtqueue_ops master_vq_ops = {
    vq_tx_master,
    vq_tx_alloc_master,
    vq_rx_master,
    vq_rx_free_master,
};

/* Interface used in case this processor is REMOTE */
static const struct virtqueue_ops remote_vq_ops = {
    vq_tx_remote,
    vq_tx_alloc_remote,
    vq_rx_remote,
    vq_rx_free_remote,
};

/* helper function for virtqueue notification */
static void virtqueue_notify(struct virtqueue *vq)
{
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    struct rpmsg_lite_instance *inst = vq->priv;
    platform_notify(inst->env ? env_get_platform_context(inst->env) : RL_NULL, vq->vq_queue_index);
#else
    platform_notify(vq->vq_queue_index);
#endif
}

/*************************************************

 mmmmmm mmmmm mmmmmmm        mm   m mmmmmmm     m
 #      #   "#   #           #"m  # #     #  #  #
 #mmmmm #mmm#"   #           # #m # #mmmmm" #"# #
 #      #        #           #  # # #      ## ##"
 #mmmmm #        #           #   ## #mmmmm #   #

**************************************************/
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
struct rpmsg_lite_endpoint *rpmsg_lite_create_ept(struct rpmsg_lite_instance *rpmsg_lite_dev,
                                                  uint32_t addr,
                                                  rl_ept_rx_cb_t rx_cb,
                                                  void *rx_cb_data,
                                                  struct rpmsg_lite_ept_static_context *ept_context)
#else
struct rpmsg_lite_endpoint *rpmsg_lite_create_ept(struct rpmsg_lite_instance *rpmsg_lite_dev,
                                                  uint32_t addr,
                                                  rl_ept_rx_cb_t rx_cb,
                                                  void *rx_cb_data)
#endif
{
    struct rpmsg_lite_endpoint *rl_ept;
    struct llist *node;
    uint32_t i;

    if (rpmsg_lite_dev == RL_NULL)
    {
        return RL_NULL;
    }

    env_lock_mutex(rpmsg_lite_dev->lock);
    {
        if (addr == RL_ADDR_ANY)
        {
            /* find lowest free address */
            for (i = 1; i < 0xFFFFFFFFU; i++)
            {
                if (rpmsg_lite_get_endpoint_from_addr(rpmsg_lite_dev, i) == RL_NULL)
                {
                    addr = i;
                    break;
                }
            }
            if (addr == RL_ADDR_ANY)
            {
                /* no address is free, cannot happen normally */
                env_unlock_mutex(rpmsg_lite_dev->lock);
                return RL_NULL;
            }
        }
        else
        {
            if (rpmsg_lite_get_endpoint_from_addr(rpmsg_lite_dev, addr) != RL_NULL)
            {
                /* Already exists! */
                env_unlock_mutex(rpmsg_lite_dev->lock);
                return RL_NULL;
            }
        }

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
        if (ept_context == RL_NULL)
        {
            env_unlock_mutex(rpmsg_lite_dev->lock);
            return RL_NULL;
        }

        rl_ept = &(ept_context->ept);
        node   = &(ept_context->node);
#else
        rl_ept = env_allocate_memory(sizeof(struct rpmsg_lite_endpoint));
        if (rl_ept == RL_NULL)
        {
            env_unlock_mutex(rpmsg_lite_dev->lock);
            return RL_NULL;
        }
        node = env_allocate_memory(sizeof(struct llist));
        if (node == RL_NULL)
        {
            env_free_memory(rl_ept);
            env_unlock_mutex(rpmsg_lite_dev->lock);
            return RL_NULL;
        }
#endif /* RL_USE_STATIC_API */

        env_memset(rl_ept, 0x00, sizeof(struct rpmsg_lite_endpoint));

        rl_ept->addr       = addr;
        rl_ept->rx_cb      = rx_cb;
        rl_ept->rx_cb_data = rx_cb_data;

        node->data = rl_ept;

        add_to_list((struct llist **)&rpmsg_lite_dev->rl_endpoints, node);
    }
    env_unlock_mutex(rpmsg_lite_dev->lock);

    return rl_ept;
}
/*************************************************

 mmmmmm mmmmm mmmmmmm        mmmm   mmmmmm m
 #      #   "#   #           #   "m #      #
 #mmmmm #mmm#"   #           #    # #mmmmm #
 #      #        #           #    # #      #
 #mmmmm #        #           #mmm"  #mmmmm #mmmmm

**************************************************/

int32_t rpmsg_lite_destroy_ept(struct rpmsg_lite_instance *rpmsg_lite_dev, struct rpmsg_lite_endpoint *rl_ept)
{
    struct llist *node;

    if (rpmsg_lite_dev == RL_NULL)
    {
        return RL_ERR_PARAM;
    }

    if (rl_ept == RL_NULL)
    {
        return RL_ERR_PARAM;
    }

    env_lock_mutex(rpmsg_lite_dev->lock);
    node = rpmsg_lite_get_endpoint_from_addr(rpmsg_lite_dev, rl_ept->addr);
    if (node != RL_NULL)
    {
        remove_from_list((struct llist **)&rpmsg_lite_dev->rl_endpoints, node);
        env_unlock_mutex(rpmsg_lite_dev->lock);
#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
        env_free_memory(node);
        env_free_memory(rl_ept);
#endif
        return RL_SUCCESS;
    }
    else
    {
        env_unlock_mutex(rpmsg_lite_dev->lock);
        return RL_ERR_PARAM;
    }
}

/******************************************

mmmmmmm m    m          mm   mmmmm  mmmmm
   #     #  #           ##   #   "#   #
   #      ##           #  #  #mmm#"   #
   #     m""m          #mm#  #        #
   #    m"  "m        #    # #      mm#mm

*******************************************/

uint32_t rpmsg_lite_is_link_up(struct rpmsg_lite_instance *rpmsg_lite_dev)
{
    if (rpmsg_lite_dev == RL_NULL)
    {
        return 0U;
    }

    return (RL_TRUE == rpmsg_lite_dev->link_state ? RL_TRUE : RL_FALSE);
}

void rpmsg_lite_wait_for_link_up(struct rpmsg_lite_instance *rpmsg_lite_dev)
{
    if (rpmsg_lite_dev == RL_NULL)
    {
        return;
    }

    env_wait_for_link_up(&rpmsg_lite_dev->link_state, rpmsg_lite_dev->link_id);
}

/*!
 * @brief
 * Internal function to format a RPMsg compatible
 * message and sends it
 *
 * @param rpmsg_lite_dev    RPMsg Lite instance
 * @param src               Local endpoint address
 * @param dst               Remote endpoint address
 * @param data              Payload buffer
 * @param size              Size of payload, in bytes
 * @param flags             Value of flags field
 * @param timeout           Timeout in ms, 0 if nonblocking
 *
 * @return  Status of function execution, RL_SUCCESS on success
 *
 */
static int32_t rpmsg_lite_format_message(struct rpmsg_lite_instance *rpmsg_lite_dev,
                                         uint32_t src,
                                         uint32_t dst,
                                         char *data,
                                         uint32_t size,
                                         int32_t flags,
                                         uint32_t timeout)
{
    struct rpmsg_std_msg *rpmsg_msg;
    void *buffer;
    uint16_t idx;
    uint32_t tick_count = 0U;
    uint32_t buff_len;

    if (rpmsg_lite_dev == RL_NULL)
    {
        return RL_ERR_PARAM;
    }

    if (data == RL_NULL)
    {
        return RL_ERR_PARAM;
    }

    if (rpmsg_lite_dev->link_state != RL_TRUE)
    {
        return RL_NOT_READY;
    }

    /* Lock the device to enable exclusive access to virtqueues */
    env_lock_mutex(rpmsg_lite_dev->lock);
    /* Get rpmsg buffer for sending message. */
    buffer = rpmsg_lite_dev->vq_ops->vq_tx_alloc(rpmsg_lite_dev->tvq, &buff_len, &idx);
    env_unlock_mutex(rpmsg_lite_dev->lock);

    if ((buffer == RL_NULL) && (timeout == RL_FALSE))
    {
        return RL_ERR_NO_MEM;
    }

    while (buffer == RL_NULL)
    {
        env_sleep_msec(RL_MS_PER_INTERVAL);
        env_lock_mutex(rpmsg_lite_dev->lock);
        buffer = rpmsg_lite_dev->vq_ops->vq_tx_alloc(rpmsg_lite_dev->tvq, &buff_len, &idx);
        env_unlock_mutex(rpmsg_lite_dev->lock);
        tick_count += (uint32_t)RL_MS_PER_INTERVAL;
        if ((tick_count >= timeout) && (buffer == RL_NULL))
        {
            return RL_ERR_NO_MEM;
        }
    }

    rpmsg_msg = (struct rpmsg_std_msg *)buffer;

    /* Initialize RPMSG header. */
    rpmsg_msg->hdr.dst   = dst;
    rpmsg_msg->hdr.src   = src;
    rpmsg_msg->hdr.len   = (uint16_t)size;
    rpmsg_msg->hdr.flags = (uint16_t)flags;

    /* Copy data to rpmsg buffer. */
    env_memcpy(rpmsg_msg->data, data, size);

    env_lock_mutex(rpmsg_lite_dev->lock);
    /* Enqueue buffer on virtqueue. */
    rpmsg_lite_dev->vq_ops->vq_tx(rpmsg_lite_dev->tvq, buffer, buff_len, idx);
    /* Let the other side know that there is a job to process. */
    virtqueue_kick(rpmsg_lite_dev->tvq);
    env_unlock_mutex(rpmsg_lite_dev->lock);

    return RL_SUCCESS;
}

int32_t rpmsg_lite_send(struct rpmsg_lite_instance *rpmsg_lite_dev,
                        struct rpmsg_lite_endpoint *ept,
                        uint32_t dst,
                        char *data,
                        uint32_t size,
                        uint32_t timeout)
{
    if (ept == RL_NULL)
    {
        return RL_ERR_PARAM;
    }

    // FIXME : may be just copy the data size equal to buffer length and Tx it.
#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
    if (size > (uint32_t)RL_BUFFER_PAYLOAD_SIZE(rpmsg_lite_dev->link_id))
#else
    if (size > (uint32_t)RL_BUFFER_PAYLOAD_SIZE)
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */
    {
        return RL_ERR_BUFF_SIZE;
    }

    return rpmsg_lite_format_message(rpmsg_lite_dev, ept->addr, dst, data, size, RL_NO_FLAGS, timeout);
}

#if defined(RL_API_HAS_ZEROCOPY) && (RL_API_HAS_ZEROCOPY == 1)

void *rpmsg_lite_alloc_tx_buffer(struct rpmsg_lite_instance *rpmsg_lite_dev, uint32_t *size, uint32_t timeout)
{
    struct rpmsg_std_msg *rpmsg_msg;
    void *buffer;
    uint16_t idx;
    uint32_t tick_count = 0U;

    if (size == RL_NULL)
    {
        return RL_NULL;
    }

    if (rpmsg_lite_dev->link_state != RL_TRUE)
    {
        *size = 0;
        return RL_NULL;
    }

    /* Lock the device to enable exclusive access to virtqueues */
    env_lock_mutex(rpmsg_lite_dev->lock);
    /* Get rpmsg buffer for sending message. */
    buffer = rpmsg_lite_dev->vq_ops->vq_tx_alloc(rpmsg_lite_dev->tvq, size, &idx);
    env_unlock_mutex(rpmsg_lite_dev->lock);

    if ((buffer == RL_NULL) && (timeout == RL_FALSE))
    {
        *size = 0;
        return RL_NULL;
    }

    while (buffer == RL_NULL)
    {
        env_sleep_msec(RL_MS_PER_INTERVAL);
        env_lock_mutex(rpmsg_lite_dev->lock);
        buffer = rpmsg_lite_dev->vq_ops->vq_tx_alloc(rpmsg_lite_dev->tvq, size, &idx);
        env_unlock_mutex(rpmsg_lite_dev->lock);
        tick_count += (uint32_t)RL_MS_PER_INTERVAL;
        if ((tick_count >= timeout) && (buffer == RL_NULL))
        {
            *size = 0;
            return RL_NULL;
        }
    }

    rpmsg_msg = (struct rpmsg_std_msg *)buffer;

    /* keep idx and totlen information for nocopy tx function */
    rpmsg_msg->hdr.reserved.idx = idx;

    /* return the maximum payload size */
    *size -= sizeof(struct rpmsg_std_hdr);

    return rpmsg_msg->data;
}

int32_t rpmsg_lite_send_nocopy(struct rpmsg_lite_instance *rpmsg_lite_dev,
                               struct rpmsg_lite_endpoint *ept,
                               uint32_t dst,
                               void *data,
                               uint32_t size)
{
    struct rpmsg_std_msg *rpmsg_msg;
    uint32_t src;

    if ((ept == RL_NULL) || (data == RL_NULL))
    {
        return RL_ERR_PARAM;
    }

#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
    if (size > (uint32_t)RL_BUFFER_PAYLOAD_SIZE(rpmsg_lite_dev->link_id))
#else
    if (size > (uint32_t)RL_BUFFER_PAYLOAD_SIZE)
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */
    {
        return RL_ERR_BUFF_SIZE;
    }

    if (rpmsg_lite_dev->link_state != RL_TRUE)
    {
        return RL_NOT_READY;
    }

    src = ept->addr;

#if defined(RL_DEBUG_CHECK_BUFFERS) && (RL_DEBUG_CHECK_BUFFERS == 1)
    RL_ASSERT(
        /* master check */
        ((rpmsg_lite_dev->vq_ops == &master_vq_ops) &&
         (data >= (void *)(rpmsg_lite_dev->sh_mem_base +
                           (rpmsg_lite_dev->rvq->vq_nentries * rpmsg_lite_dev->rvq->vq_ring.desc->len))) &&
         (data <= (void *)(rpmsg_lite_dev->sh_mem_base +
                           (2 * rpmsg_lite_dev->rvq->vq_nentries * rpmsg_lite_dev->rvq->vq_ring.desc->len)))) ||

        /* remote check */
        ((rpmsg_lite_dev->vq_ops == &remote_vq_ops) && (data >= (void *)rpmsg_lite_dev->sh_mem_base) &&
         (data <= (void *)(rpmsg_lite_dev->sh_mem_base +
                           (rpmsg_lite_dev->rvq->vq_nentries * rpmsg_lite_dev->rvq->vq_ring.desc->len)))))
#endif

    rpmsg_msg = RPMSG_STD_MSG_FROM_BUF(data);

    /* Initialize RPMSG header. */
    rpmsg_msg->hdr.dst   = dst;
    rpmsg_msg->hdr.src   = src;
    rpmsg_msg->hdr.len   = (uint16_t)size;
    rpmsg_msg->hdr.flags = (uint16_t)RL_NO_FLAGS;

    env_lock_mutex(rpmsg_lite_dev->lock);
    /* Enqueue buffer on virtqueue. */
    rpmsg_lite_dev->vq_ops->vq_tx(
        rpmsg_lite_dev->tvq, (void *)rpmsg_msg,
        (uint32_t)virtqueue_get_buffer_length(rpmsg_lite_dev->tvq, rpmsg_msg->hdr.reserved.idx),
        rpmsg_msg->hdr.reserved.idx);
    /* Let the other side know that there is a job to process. */
    virtqueue_kick(rpmsg_lite_dev->tvq);
    env_unlock_mutex(rpmsg_lite_dev->lock);

    return RL_SUCCESS;
}

/******************************************

 mmmmm  m    m          mm   mmmmm  mmmmm
 #   "#  #  #           ##   #   "#   #
 #mmmm"   ##           #  #  #mmm#"   #
 #   "m  m""m          #mm#  #        #
 #    " m"  "m        #    # #      mm#mm

 *******************************************/

int32_t rpmsg_lite_release_rx_buffer(struct rpmsg_lite_instance *rpmsg_lite_dev, void *rxbuf)
{
    struct rpmsg_std_msg *rpmsg_msg;

    if (rpmsg_lite_dev == RL_NULL)
    {
        return RL_ERR_PARAM;
    }
    if (rxbuf == RL_NULL)
    {
        return RL_ERR_PARAM;
    }

#if defined(RL_DEBUG_CHECK_BUFFERS) && (RL_DEBUG_CHECK_BUFFERS == 1)
    RL_ASSERT(
        /* master check */
        ((rpmsg_lite_dev->vq_ops == &master_vq_ops) && (rxbuf >= (void *)rpmsg_lite_dev->sh_mem_base) &&
         (rxbuf <= (void *)(rpmsg_lite_dev->sh_mem_base +
                            (rpmsg_lite_dev->rvq->vq_nentries * rpmsg_lite_dev->rvq->vq_ring.desc->len)))) ||

        /* remote check */
        ((rpmsg_lite_dev->vq_ops == &remote_vq_ops) &&
         (rxbuf >= (void *)(rpmsg_lite_dev->sh_mem_base +
                            (rpmsg_lite_dev->rvq->vq_nentries * rpmsg_lite_dev->rvq->vq_ring.desc->len))) &&
         (rxbuf <= (void *)(rpmsg_lite_dev->sh_mem_base +
                            (2 * rpmsg_lite_dev->rvq->vq_nentries * rpmsg_lite_dev->rvq->vq_ring.desc->len)))))
#endif

    rpmsg_msg = RPMSG_STD_MSG_FROM_BUF(rxbuf);

    env_lock_mutex(rpmsg_lite_dev->lock);

    /* Return used buffer, with total length (header length + buffer size). */
    rpmsg_lite_dev->vq_ops->vq_rx_free(
        rpmsg_lite_dev->rvq, rpmsg_msg,
        (uint32_t)virtqueue_get_buffer_length(rpmsg_lite_dev->rvq, rpmsg_msg->hdr.reserved.idx),
        rpmsg_msg->hdr.reserved.idx);

#if defined(RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION) && (RL_ALLOW_CONSUMED_BUFFERS_NOTIFICATION == 1)
    /* Let the remote device know that a buffer has been freed */
    virtqueue_kick(rpmsg_lite_dev->rvq);
#endif

    env_unlock_mutex(rpmsg_lite_dev->lock);

    return RL_SUCCESS;
}

#endif /* RL_API_HAS_ZEROCOPY */

/******************************

 mmmmm  mm   m mmmmm mmmmmmm
   #    #"m  #   #      #
   #    # #m #   #      #
   #    #  # #   #      #
 mm#mm  #   ## mm#mm    #

 *****************************/
#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
struct rpmsg_lite_instance *rpmsg_lite_master_init(void *shmem_addr,
                                                   size_t shmem_length,
                                                   uint32_t link_id,
                                                   uint32_t init_flags,
                                                   struct rpmsg_lite_instance *static_context)
#elif defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
struct rpmsg_lite_instance *rpmsg_lite_master_init(
    void *shmem_addr, size_t shmem_length, uint32_t link_id, uint32_t init_flags, void *env_cfg)
#else
struct rpmsg_lite_instance *rpmsg_lite_master_init(void *shmem_addr,
                                                   size_t shmem_length,
                                                   uint32_t link_id,
                                                   uint32_t init_flags)
#endif
{
    int32_t status;
    void (*callback[2])(struct virtqueue * vq);
    const char *vq_names[2];
    struct vring_alloc_info ring_info;
    struct virtqueue *vqs[2];
    void *buffer;
    uint32_t idx, j;
    struct rpmsg_lite_instance *rpmsg_lite_dev = RL_NULL;

    if (link_id > RL_PLATFORM_HIGHEST_LINK_ID)
    {
        return RL_NULL;
    }

    if (shmem_addr == RL_NULL)
    {
        return RL_NULL;
    }

#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
    /* Get the custom shmem configuration defined per each rpmsg_lite instance
       (i.e. per each link id) from the platform layer */
    rpmsg_platform_shmem_config_t shmem_config = {0};
    if (RL_SUCCESS != platform_get_custom_shmem_config(link_id, &shmem_config))
    {
        return RL_NULL;
    }

    /* shmem_config.buffer_count must be power of two (2, 4, ...) */
    if (0U != (shmem_config.buffer_count & (shmem_config.buffer_count - 1U)))
    {
        return RL_NULL;
    }

    /* buffer size must be power of two (256, 512, ...) */
    if (0U != ((shmem_config.buffer_payload_size + 16UL) & ((shmem_config.buffer_payload_size + 16UL) - 1U)))
    {
        return RL_NULL;
    }

    if ((2U * (uint32_t)shmem_config.buffer_count) >
        ((RL_WORD_ALIGN_DOWN(shmem_length - 2U * shmem_config.vring_size)) /
         (uint32_t)(shmem_config.buffer_payload_size + 16UL)))
    {
        return RL_NULL;
    }
#else
    if ((2U * (uint32_t)RL_BUFFER_COUNT) >
        ((RL_WORD_ALIGN_DOWN(shmem_length - (uint32_t)RL_VRING_OVERHEAD)) / (uint32_t)RL_BUFFER_SIZE))
    {
        return RL_NULL;
    }

#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
    if (static_context == RL_NULL)
    {
        return RL_NULL;
    }
    rpmsg_lite_dev = static_context;
#else
    rpmsg_lite_dev = env_allocate_memory(sizeof(struct rpmsg_lite_instance));
    if (rpmsg_lite_dev == RL_NULL)
    {
        return RL_NULL;
    }
#endif

    env_memset(rpmsg_lite_dev, 0, sizeof(struct rpmsg_lite_instance));
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    status = env_init(&rpmsg_lite_dev->env, env_cfg);
#else
    status = env_init();
#endif
    if (status != RL_SUCCESS)
    {
#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
        env_free_memory(rpmsg_lite_dev);
#endif
        return RL_NULL;
    }

    rpmsg_lite_dev->link_id = link_id;

    /*
     * Since device is RPMSG Remote so we need to manage the
     * shared buffers. Create shared memory pool to handle buffers.
     */
#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
    rpmsg_lite_dev->sh_mem_base = (char *)RL_WORD_ALIGN_UP((uint32_t)(char *)shmem_addr + 2U * shmem_config.vring_size);
    rpmsg_lite_dev->sh_mem_remaining = (RL_WORD_ALIGN_DOWN(shmem_length - 2U * shmem_config.vring_size)) /
                                       (uint32_t)(shmem_config.buffer_payload_size + 16UL);
#else
    rpmsg_lite_dev->sh_mem_base = (char *)RL_WORD_ALIGN_UP((uint32_t)(char *)shmem_addr + (uint32_t)RL_VRING_OVERHEAD);
    rpmsg_lite_dev->sh_mem_remaining =
        (RL_WORD_ALIGN_DOWN(shmem_length - (uint32_t)RL_VRING_OVERHEAD)) / (uint32_t)RL_BUFFER_SIZE;
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */
    rpmsg_lite_dev->sh_mem_total = rpmsg_lite_dev->sh_mem_remaining;

    /* Initialize names and callbacks*/
    vq_names[0]            = "rx_vq";
    vq_names[1]            = "tx_vq";
    callback[0]            = rpmsg_lite_rx_callback;
    callback[1]            = rpmsg_lite_tx_callback;
    rpmsg_lite_dev->vq_ops = &master_vq_ops;

    /* Create virtqueue for each vring. */
    for (idx = 0U; idx < 2U; idx++)
    {
#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
        ring_info.phy_addr =
            (void *)(char *)((uint32_t)(char *)shmem_addr + (uint32_t)((idx == 0U) ? (0U) : (shmem_config.vring_size)));
        ring_info.align     = shmem_config.vring_align;
        ring_info.num_descs = shmem_config.buffer_count;
#else
        ring_info.phy_addr =
            (void *)(char *)((uint32_t)(char *)shmem_addr + (uint32_t)((idx == 0U) ? (0U) : (VRING_SIZE)));
        ring_info.align = VRING_ALIGN;
        ring_info.num_descs = RL_BUFFER_COUNT;
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */

        env_memset((void *)ring_info.phy_addr, 0x00, (uint32_t)vring_size(ring_info.num_descs, ring_info.align));

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
        status = virtqueue_create_static((uint16_t)(RL_GET_VQ_ID(link_id, idx)), vq_names[idx], &ring_info,
                                         callback[idx], virtqueue_notify, &vqs[idx],
                                         (struct vq_static_context *)&rpmsg_lite_dev->vq_ctxt[idx]);
#else
        status = virtqueue_create((uint16_t)(RL_GET_VQ_ID(link_id, idx)), vq_names[idx], &ring_info, callback[idx],
                                  virtqueue_notify, &vqs[idx]);
#endif /* RL_USE_STATIC_API */

        if (status == RL_SUCCESS)
        {
            /* Initialize vring control block in virtqueue. */
            vq_ring_init(vqs[idx]);

            /* Disable callbacks - will be enabled by the application
             * once initialization is completed.
             */
            virtqueue_disable_cb(vqs[idx]);
        }
        else
        {
#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
            env_free_memory(rpmsg_lite_dev);
#endif
            return RL_NULL;
        }

        /* virtqueue has reference to the RPMsg Lite instance */
        vqs[idx]->priv = (void *)rpmsg_lite_dev;
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
        vqs[idx]->env = rpmsg_lite_dev->env;
#endif
    }

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
    status = env_create_mutex((LOCK *)&rpmsg_lite_dev->lock, 1, &rpmsg_lite_dev->lock_static_ctxt);
#else
    status = env_create_mutex((LOCK *)&rpmsg_lite_dev->lock, 1);
#endif
    if (status != RL_SUCCESS)
    {
#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
        env_free_memory(rpmsg_lite_dev);
#endif
        return RL_NULL;
    }

    // FIXME - a better way to handle this , tx for master is rx for remote and vice versa.
    rpmsg_lite_dev->tvq = vqs[1];
    rpmsg_lite_dev->rvq = vqs[0];

    for (j = 0U; j < 2U; j++)
    {
        for (idx = 0U; ((idx < vqs[j]->vq_nentries) && (idx < rpmsg_lite_dev->sh_mem_total)); idx++)
        {
            /* Initialize TX virtqueue buffers for remote device */
            buffer = (rpmsg_lite_dev->sh_mem_remaining > 0U) ?
                         (rpmsg_lite_dev->sh_mem_base +
#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
                          (uint32_t)(shmem_config.buffer_payload_size + 16UL) *
                              (rpmsg_lite_dev->sh_mem_total - rpmsg_lite_dev->sh_mem_remaining--)) :
#else
                          (uint32_t)RL_BUFFER_SIZE *
                              (rpmsg_lite_dev->sh_mem_total - rpmsg_lite_dev->sh_mem_remaining--)) :
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */
                         (RL_NULL);

            RL_ASSERT(buffer != RL_NULL);

#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
            env_memset(buffer, 0x00, (uint32_t)(shmem_config.buffer_payload_size + 16UL));
#else
            env_memset(buffer, 0x00, (uint32_t)RL_BUFFER_SIZE);
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */
            if (vqs[j] == rpmsg_lite_dev->rvq)
            {
#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
                status =
                    virtqueue_fill_avail_buffers(vqs[j], buffer, (uint32_t)(shmem_config.buffer_payload_size + 16UL));
#else
                status = virtqueue_fill_avail_buffers(vqs[j], buffer, (uint32_t)RL_BUFFER_SIZE);
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */
            }
            else if (vqs[j] == rpmsg_lite_dev->tvq)
            {
#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
                status =
                    virtqueue_fill_used_buffers(vqs[j], buffer, (uint32_t)(shmem_config.buffer_payload_size + 16UL));
#else
                status = virtqueue_fill_used_buffers(vqs[j], buffer, (uint32_t)RL_BUFFER_SIZE);
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */
            }
            else
            {
                /* should not happen */
            }

            if (status != RL_SUCCESS)
            {
                /* Clean up! */
                env_delete_mutex(rpmsg_lite_dev->lock);
#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
                env_free_memory(rpmsg_lite_dev);
#endif
                return RL_NULL;
            }
        }
    }

    /* Install ISRs */
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    env_init_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index, rpmsg_lite_dev->rvq);
    env_init_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index, rpmsg_lite_dev->tvq);
    env_disable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index);
    env_disable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index);
    rpmsg_lite_dev->link_state = 1U;
    env_enable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index);
    env_enable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index);
#else
    (void)platform_init_interrupt(rpmsg_lite_dev->rvq->vq_queue_index, rpmsg_lite_dev->rvq);
    (void)platform_init_interrupt(rpmsg_lite_dev->tvq->vq_queue_index, rpmsg_lite_dev->tvq);
    env_disable_interrupt(rpmsg_lite_dev->rvq->vq_queue_index);
    env_disable_interrupt(rpmsg_lite_dev->tvq->vq_queue_index);
    rpmsg_lite_dev->link_state = 1U;
    env_enable_interrupt(rpmsg_lite_dev->rvq->vq_queue_index);
    env_enable_interrupt(rpmsg_lite_dev->tvq->vq_queue_index);
#endif

    /*
     * Let the remote device know that Master is ready for
     * communication.
     */
    virtqueue_kick(rpmsg_lite_dev->rvq);

    return rpmsg_lite_dev;
}

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
struct rpmsg_lite_instance *rpmsg_lite_remote_init(void *shmem_addr,
                                                   uint32_t link_id,
                                                   uint32_t init_flags,
                                                   struct rpmsg_lite_instance *static_context)
#elif defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
struct rpmsg_lite_instance *rpmsg_lite_remote_init(void *shmem_addr,
                                                   uint32_t link_id,
                                                   uint32_t init_flags,
                                                   void *env_cfg)
#else
struct rpmsg_lite_instance *rpmsg_lite_remote_init(void *shmem_addr, uint32_t link_id, uint32_t init_flags)
#endif
{
    int32_t status;
    void (*callback[2])(struct virtqueue * vq);
    const char *vq_names[2];
    struct vring_alloc_info ring_info;
    struct virtqueue *vqs[2];
    uint32_t idx;
    struct rpmsg_lite_instance *rpmsg_lite_dev = RL_NULL;

    if (link_id > RL_PLATFORM_HIGHEST_LINK_ID)
    {
        return RL_NULL;
    }

    if (shmem_addr == RL_NULL)
    {
        return RL_NULL;
    }

#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
    /* Get the custom shmem configuration defined per each rpmsg_lite instance
       (i.e. per each link id) from the platform layer */
    rpmsg_platform_shmem_config_t shmem_config = {0};
    if (RL_SUCCESS != platform_get_custom_shmem_config(link_id, &shmem_config))
    {
        return RL_NULL;
    }

    /* shmem_config.buffer_count must be power of two (2, 4, ...) */
    if (0U != (shmem_config.buffer_count & (shmem_config.buffer_count - 1U)))
    {
        return RL_NULL;
    }

    /* buffer size must be power of two (256, 512, ...) */
    if (0U != ((shmem_config.buffer_payload_size + 16UL) & ((shmem_config.buffer_payload_size + 16UL) - 1U)))
    {
        return RL_NULL;
    }
#endif /* defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1) */

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
    if (static_context == RL_NULL)
    {
        return RL_NULL;
    }
    rpmsg_lite_dev = static_context;
#else
    rpmsg_lite_dev = env_allocate_memory(sizeof(struct rpmsg_lite_instance));
    if (rpmsg_lite_dev == RL_NULL)
    {
        return RL_NULL;
    }
#endif

    env_memset(rpmsg_lite_dev, 0, sizeof(struct rpmsg_lite_instance));
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    status = env_init(&rpmsg_lite_dev->env, env_cfg);
#else
    status = env_init();
#endif

    if (status != RL_SUCCESS)
    {
#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
        env_free_memory(rpmsg_lite_dev);
#endif
        return RL_NULL;
    }

    rpmsg_lite_dev->link_id = link_id;

    vq_names[0]            = "tx_vq"; /* swapped in case of remote */
    vq_names[1]            = "rx_vq";
    callback[0]            = rpmsg_lite_tx_callback;
    callback[1]            = rpmsg_lite_rx_callback;
    rpmsg_lite_dev->vq_ops = &remote_vq_ops;
#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
    rpmsg_lite_dev->sh_mem_base = (char *)RL_WORD_ALIGN_UP((uint32_t)(char *)shmem_addr + 2U * shmem_config.vring_size);
#else
    rpmsg_lite_dev->sh_mem_base = (char *)RL_WORD_ALIGN_UP((uint32_t)(char *)shmem_addr + (uint32_t)RL_VRING_OVERHEAD);
#endif /* defined(RL_ALLOW_CUSTOM_VRING_CONFIG) && (RL_ALLOW_CUSTOM_VRING_CONFIG == 1) */

    /* Create virtqueue for each vring. */
    for (idx = 0U; idx < 2U; idx++)
    {
#if defined(RL_ALLOW_CUSTOM_SHMEM_CONFIG) && (RL_ALLOW_CUSTOM_SHMEM_CONFIG == 1)
        ring_info.phy_addr =
            (void *)(char *)((uint32_t)(char *)shmem_addr + (uint32_t)((idx == 0U) ? (0U) : (shmem_config.vring_size)));
        ring_info.align     = shmem_config.vring_align;
        ring_info.num_descs = shmem_config.buffer_count;
#else
        ring_info.phy_addr =
            (void *)(char *)((uint32_t)(char *)shmem_addr + (uint32_t)((idx == 0U) ? (0U) : (VRING_SIZE)));
        ring_info.align = VRING_ALIGN;
        ring_info.num_descs = RL_BUFFER_COUNT;
#endif /* defined(RL_ALLOW_CUSTOM_VRING_CONFIG) && (RL_ALLOW_CUSTOM_VRING_CONFIG == 1) */

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
        status = virtqueue_create_static((uint16_t)(RL_GET_VQ_ID(link_id, idx)), vq_names[idx], &ring_info,
                                         callback[idx], virtqueue_notify, &vqs[idx],
                                         (struct vq_static_context *)&rpmsg_lite_dev->vq_ctxt[idx]);
#else
        status = virtqueue_create((uint16_t)(RL_GET_VQ_ID(link_id, idx)), vq_names[idx], &ring_info, callback[idx],
                                  virtqueue_notify, &vqs[idx]);
#endif /* RL_USE_STATIC_API */

        if (status != RL_SUCCESS)
        {
#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
            env_free_memory(rpmsg_lite_dev);
#endif
            return RL_NULL;
        }

        /* virtqueue has reference to the RPMsg Lite instance */
        vqs[idx]->priv = (void *)rpmsg_lite_dev;
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
        vqs[idx]->env = rpmsg_lite_dev->env;
#endif
    }

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
    status = env_create_mutex((LOCK *)&rpmsg_lite_dev->lock, 1, &rpmsg_lite_dev->lock_static_ctxt);
#else
    status = env_create_mutex((LOCK *)&rpmsg_lite_dev->lock, 1);
#endif
    if (status != RL_SUCCESS)
    {
#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
        env_free_memory(rpmsg_lite_dev);
#endif
        return RL_NULL;
    }

    // FIXME - a better way to handle this , tx for master is rx for remote and vice versa.
    rpmsg_lite_dev->tvq = vqs[0];
    rpmsg_lite_dev->rvq = vqs[1];

    /* Install ISRs */
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    env_init_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index, rpmsg_lite_dev->rvq);
    env_init_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index, rpmsg_lite_dev->tvq);
    env_disable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index);
    env_disable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index);
    rpmsg_lite_dev->link_state = 0;
    env_enable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index);
    env_enable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index);
#else
    (void)platform_init_interrupt(rpmsg_lite_dev->rvq->vq_queue_index, rpmsg_lite_dev->rvq);
    (void)platform_init_interrupt(rpmsg_lite_dev->tvq->vq_queue_index, rpmsg_lite_dev->tvq);
    env_disable_interrupt(rpmsg_lite_dev->rvq->vq_queue_index);
    env_disable_interrupt(rpmsg_lite_dev->tvq->vq_queue_index);
    rpmsg_lite_dev->link_state = 0;
    env_enable_interrupt(rpmsg_lite_dev->rvq->vq_queue_index);
    env_enable_interrupt(rpmsg_lite_dev->tvq->vq_queue_index);
#endif

    return rpmsg_lite_dev;
}

/*******************************************

 mmmm   mmmmmm mmmmm  mm   m mmmmm mmmmmmm
 #   "m #        #    #"m  #   #      #
 #    # #mmmmm   #    # #m #   #      #
 #    # #        #    #  # #   #      #
 #mmm"  #mmmmm mm#mm  #   ## mm#mm    #

********************************************/

int32_t rpmsg_lite_deinit(struct rpmsg_lite_instance *rpmsg_lite_dev)
{
    if (rpmsg_lite_dev == RL_NULL)
    {
        return RL_ERR_PARAM;
    }

    if (!((rpmsg_lite_dev->rvq != RL_NULL) && (rpmsg_lite_dev->tvq != RL_NULL) && (rpmsg_lite_dev->lock != RL_NULL)))
    {
        /* ERROR - trying to initialize uninitialized RPMSG? */
        RL_ASSERT((rpmsg_lite_dev->rvq != RL_NULL) && (rpmsg_lite_dev->tvq != RL_NULL) &&
                  (rpmsg_lite_dev->lock != RL_NULL));
        return RL_ERR_PARAM;
    }
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    env_disable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index);
    env_disable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index);
    rpmsg_lite_dev->link_state = 0;
    env_enable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index);
    env_enable_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index);

    env_deinit_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->rvq->vq_queue_index);
    env_deinit_interrupt(rpmsg_lite_dev->env, rpmsg_lite_dev->tvq->vq_queue_index);
#else
    env_disable_interrupt(rpmsg_lite_dev->rvq->vq_queue_index);
    env_disable_interrupt(rpmsg_lite_dev->tvq->vq_queue_index);
    rpmsg_lite_dev->link_state = 0;
    env_enable_interrupt(rpmsg_lite_dev->rvq->vq_queue_index);
    env_enable_interrupt(rpmsg_lite_dev->tvq->vq_queue_index);

    (void)platform_deinit_interrupt(rpmsg_lite_dev->rvq->vq_queue_index);
    (void)platform_deinit_interrupt(rpmsg_lite_dev->tvq->vq_queue_index);
#endif

#if defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1)
    virtqueue_free_static(rpmsg_lite_dev->rvq);
    virtqueue_free_static(rpmsg_lite_dev->tvq);
#else
    virtqueue_free(rpmsg_lite_dev->rvq);
    virtqueue_free(rpmsg_lite_dev->tvq);
#endif /* RL_USE_STATIC_API */

    env_delete_mutex(rpmsg_lite_dev->lock);
#if defined(RL_USE_ENVIRONMENT_CONTEXT) && (RL_USE_ENVIRONMENT_CONTEXT == 1)
    (void)env_deinit(rpmsg_lite_dev->env);
#else
    (void)env_deinit();
#endif

#if !(defined(RL_USE_STATIC_API) && (RL_USE_STATIC_API == 1))
    env_free_memory(rpmsg_lite_dev);
#endif /* RL_USE_STATIC_API */

    return RL_SUCCESS;
}
