/**
  * Copyright (c) 2019 Rockchip Electronic Co.,Ltd
  *
  * SPDX-License-Identifier: Apache-2.0
  ******************************************************************************
  * @file    vicap.c
  * @version V0.0.1
  * @brief   mipi-dphy-csi-rx device abstract for swallow
  *
  * Change Logs:
  * Date           Author          Notes
  * 2020-07-22     ISP Team      first implementation
  *
  ******************************************************************************
  */

#if defined(__RT_THREAD__)
#include "drv_mipi_dphy.h"
#include "drv_clock.h"
#include "board.h"
#elif defined(__RK_OS__)
#include "driver/drv_mipi_dphy.h"
#endif

#ifdef RT_USING_MIPI_DPHY

#define VIP_DEBUG 1
#if VIP_DEBUG
#include <stdio.h>
#define MIPI_DBG(...) rk_kprintf("[MIPI_DPHY]:");rk_kprintf(__VA_ARGS__)
#else
#define MIPI_DBG(...)
#endif

#define WRITE_MIPI_REG(base, addr, val) (*(unsigned int *)(addr+(base)) = val)
#define READ_MIPI_REG(base, addr)       (*(unsigned int *)(addr+(base)))

#define CSIHOST_PHY_TEST_CTRL1 0x34
#define CSIHOST_PHY_TEST_CTRL0 0x30

#define GRF_BASE_MIPI 0x40240000
#define GRF_SOC_CON2  0x00208
#define GRF_SOC_CON3  0x0020c

#define GRF_SOC_STATUS0 0x00280
#define GRF_SOC_STATUS1 0x00284

#define ISP_BASE_MIPI 0x40170000

#if defined(RT_USING_GC2053)
#define LANE_NUMBER   1
#else
#define LANE_NUMBER   2
#endif

typedef volatile unsigned int       REG32;

#define WriteReg32(addr, data)              (*(REG32 *)(addr) = data)
#define ReadReg32(addr)                     (*(REG32 *)(addr))
#define WriteReg32(addr, data)              (*(REG32 *)(addr) = data)
#define SetRegBits32(addr, databit)         WriteReg32(addr, ReadReg32(addr)|(databit))
#define ClrRegBits32(addr, databit)         WriteReg32(addr, ReadReg32(addr)&~(databit))
#define SetRegBit32(addr,bit)               WriteReg32(addr,(ReadReg32(addr)|(1<<bit)))
#define ClrRegBit32(addr,bit)               WriteReg32(addr,(ReadReg32(addr)&(~(1<<bit))))
#define GetRegBit32(addr,bit)               (ReadReg32(addr)&(1<<bit))
#define MaskRegBits32(addr, y, z)           WriteReg32(addr, (ReadReg32(addr)&~(y))|(z))

#if defined(__RT_THREAD__)

static int rk_mipiphy_wr_reg(unsigned int csi_addr,
                             unsigned char addr,
                             unsigned char data
                            )
{
#if 0
    //TESTEN =1,TESTDIN=addr
    WRITE_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL1, (0x00010000 | addr));
    //TESTCLK=0
    WRITE_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL0, 0x00000000);
    //TESTEN =0,TESTDIN=data
    WRITE_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL1, (0x00000000 | data));
    //TESTCLK=1
    WRITE_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL0, 0x00000002);
#else
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x02000200);         //TESTCLK=1
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x00ff0000 | addr);   //TESTDIN=addr
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x01000100);         //TESTEN =1
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x02000000);         //TESTCLK=0
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x01000000);         //TESTEN =0
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x02000000);         //TESTCLK=0
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x00ff0000 | data);   //TESTDIN=data
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x02000200);         //TESTCLK=1
#endif

    return 0;
}

static int rk_mipiphy_rd_reg(unsigned int csi_addr,
                             unsigned char addr
                            )
{
#if 0
    uint8_t data;
    //TESTEN =1,TESTDIN=addr
    WRITE_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL1, (0x00010000 | addr));
    //TESTCLK=0
    WRITE_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL0, 0x00000000);
    data = (READ_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL1) & 0xff00) >> 0x8;

    //TESTEN =0
    WRITE_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL1, 0x00000000);
    //TESTCLK=1
    WRITE_MIPI_REG(csi_addr, CSIHOST_PHY_TEST_CTRL0, 0x00000002);
#else
    uint8_t data;
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x01ff0100 | addr); //TESTEN =1,TESTDIN=addr
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x02000000);     //TESTCLK=0,TESTDIN=addr
    data = READ_MIPI_REG(csi_addr, GRF_SOC_STATUS1) & 0xff;
    WRITE_MIPI_REG(csi_addr, GRF_SOC_CON2, 0x03000200);     //TESTEN =0,TESETCLK=1
    //printf("REG addr=0x%x, data=0x%x\n",addr,data);
#endif

    return data;
}

int rk_csi_phy_cfg(struct cam_csi_phy *csi_phy)
{
    WRITE_MIPI_REG(ISP_MIPI_BASE, ISP_MIPI_MIPI_CTRL_OFFSET, 0x00000000);   //SHUTDOWNZ=0 isp mipi lanenum 2
    WriteReg32(GRF_BASE + GRF_SOC_CON0_OFFSET,
               (GRF_SOC_CON0_DPHY_RX0_CLK_INV_SEL_MASK |
                (1 << (GRF_SOC_CON0_DPHY_RX0_CLK_INV_SEL_SHIFT + 16))));

    WRITE_MIPI_REG(GRF_BASE, GRF_SOC_CON2_OFFSET, 0x02000200); //TESTCLK=1
    WRITE_MIPI_REG(GRF_BASE, GRF_SOC_CON2_OFFSET, 0x04000400); //TESTCLR=1
    //WRITE_MIPI_REG(GRF_BASE, GRF_SOC_CON2_OFFSET, 0x04000000); //TESTCLR=0

    //rk_mipiphy_rd_reg(GRF_BASE_MIPI, 0x00);

#if (LANE_NUMBER == 1)
    //rk_mipiphy_wr_reg(GRF_BASE_MIPI, 0x44, 0x52);
    WRITE_MIPI_REG(GRF_BASE_MIPI, GRF_SOC_CON3, 0x00030001); //ENABLE_0 =0,ENABLE_1=0
#elif (LANE_NUMBER == 2)
    //rk_mipiphy_wr_reg(GRF_BASE_MIPI, 0x44, 0x52);
    //rk_mipiphy_wr_reg(GRF_BASE_MIPI, 0x54, 0x52);
    WRITE_MIPI_REG(GRF_BASE_MIPI, GRF_SOC_CON3, 0x00030003); //ENABLE_0 =0,ENABLE_1=0
#endif

    //rk_mipiphy_rd_reg(GRF_BASE_MIPI, 0x00);                    //Normal operation
    WRITE_MIPI_REG(ISP_MIPI_BASE, ISP_MIPI_MIPI_CTRL_OFFSET, 0x00041000);   //SHUTDOWNZ=0 isp mipi lanenum 2
#if (LANE_NUMBER == 1)
    WRITE_MIPI_REG(ISP_MIPI_BASE, ISP_MIPI_MIPI_CTRL_OFFSET, 0x00040f01);   //SHUTDOWNZ=1
#elif (LANE_NUMBER == 2)
    WRITE_MIPI_REG(ISP_MIPI_BASE, ISP_MIPI_MIPI_CTRL_OFFSET, 0x00041f01);    //SHUTDOWNZ=1
#endif
    /* 配置完shutdownz后延时1ms */
    HAL_DelayMs(1);

    WRITE_MIPI_REG(GRF_BASE, GRF_SOC_CON2_OFFSET, 0x04000000); //TESTCLR=0

    while (1)
    {
        unsigned int mipi_status = READ_MIPI_REG(ISP_MIPI_BASE, ISP_MIPI_MIPI_STATUS_OFFSET);
        unsigned int cru_gate_status = READ_MIPI_REG(CRU_BASE, CRU_GATE_CON04_OFFSET);
#if (LANE_NUMBER == 1)
        if (((mipi_status >> ISP_MIPI_MIPI_STATUS_STOPSTATE_SHIFT) & 1) &&
                ((mipi_status >> ISP_MIPI_MIPI_STATUS_S_STOPSTATE_CLK_SHIFT) & 1))
        {
            break;
        }
#elif (LANE_NUMBER == 2)
        if (((mipi_status >> ISP_MIPI_MIPI_STATUS_STOPSTATE_SHIFT) & 1) &&
                ((mipi_status >> (ISP_MIPI_MIPI_STATUS_STOPSTATE_SHIFT + 1)) & 1) &&
                ((mipi_status >> ISP_MIPI_MIPI_STATUS_S_STOPSTATE_CLK_SHIFT) & 1))
        {
            break;
        }
#endif
        /* 等待1ms，尝试读取下一次完成信息 */
        HAL_DelayMs(1);
        rt_kprintf("wait mipi status done %x gating %x\n", mipi_status, cru_gate_status);
    }

#if (LANE_NUMBER == 1)
    rk_mipiphy_wr_reg(GRF_BASE_MIPI, 0x44, 0x52); //Please make sure the frequency range configure match with sensor data rate;
#elif (LANE_NUMBER == 2)
    rk_mipiphy_wr_reg(GRF_BASE_MIPI, 0x44, 0x52); //Please make sure the frequency range configure match with sensor data rate;
    rk_mipiphy_wr_reg(GRF_BASE_MIPI, 0x54, 0x52); //Please make sure the frequency range configure match with sensor data rate;
#endif

    MIPI_DBG("(%s) MIPI CTRL %x %x\n", __FUNCTION__,
             READ_MIPI_REG(ISP_MIPI_BASE, ISP_MIPI_MIPI_STATUS_OFFSET),
             READ_MIPI_REG(CRU_BASE, CRU_GATE_CON04_OFFSET));
    return 0;

}


static ret_err_t rk_rtthread_mipi_dphy_control(rk_device *dev,
        dt_cmd_t cmd,
        void *args)
{
    struct rk_mipi_dphy_device *mipi;
    ret_err_t ret;
    uint32_t phy_index = *(uint32_t *)args;

    MIPI_DBG("(%s) enter \n", __FUNCTION__);

    RT_ASSERT(dev != RK_NULL);
    mipi = (struct rk_mipi_dphy_device *)dev;

    if (cmd == 0)
    {
        mipi->csi_phy[phy_index].data_en_bit = 0;
    }
    else
    {
        mipi->csi_phy[phy_index].data_en_bit = 2; /* default mipi lane num is 2 */
    }
    rk_csi_phy_cfg(&mipi->csi_phy[phy_index]);

    MIPI_DBG("(%s) exit \n", __FUNCTION__);

    return ret;
}

rt_err_t rk_rtthread_mipi_dphy_open(rt_device_t dev, rt_uint16_t oflag)
{
    struct rk_mipi_dphy_device *mipi_dphy_device = (struct rk_mipi_dphy_device *)dev;
    clk_enable(mipi_dphy_device->clkgate);
    return RT_EOK;
}

rt_err_t rk_rtthread_mipi_dphy_close(rt_device_t dev)
{
    struct rk_mipi_dphy_device *mipi_dphy_device = (struct rk_mipi_dphy_device *)dev;
    clk_disable(mipi_dphy_device->clkgate);
    return RT_EOK;
}

const static struct rt_device_ops rk_mipi_dphy_dev_ops =
{
    RK_NULL,
    rk_rtthread_mipi_dphy_open,
    rk_rtthread_mipi_dphy_close,
    RK_NULL,
    RK_NULL,
    rk_rtthread_mipi_dphy_control
};

int rk_rtthread_mipi_dphy_register(void)
{
    int ret;
    rk_device *device;
    struct rk_mipi_dphy_device *mipi;

    MIPI_DBG("(%s) enter \n", __FUNCTION__);

    mipi = rt_calloc(1, sizeof(*mipi));
    RT_ASSERT(mipi != RK_NULL);

    device = &(mipi->dev);

    device->type = RT_Device_Class_Miscellaneous;
    device->rx_indicate = RK_NULL;
    device->tx_complete = RK_NULL;

#ifdef RT_USING_DEVICE_OPS
    device->ops = &rk_mipi_dphy_dev_ops;
#else
    device->init = RK_NULL;
    device->open = RK_NULL;
    device->close = RK_NULL;
    device->read = RK_NULL;
    device->write = RK_NULL;
    device->control = rk_rtthread_mipi_dphy_control;
#endif
    mipi->clkgate = get_clk_gate_from_id(CLK_MIPIPHY0_CFG_GATE);

    ret = rt_device_register(device, "mipi_dphy", RT_DEVICE_FLAG_RDWR);
    MIPI_DBG("(%s) exit \n", __FUNCTION__);

    return ret;
}
#endif

#if defined(__RT_THREAD__)
INIT_DEVICE_EXPORT(rk_rtthread_mipi_dphy_register);
#endif

#endif
