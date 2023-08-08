/*
 * Copyright (c) 2021 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <misc.h>
#include <mmc.h>
#include <spl.h>
#include <asm/io.h>
#include <asm/arch/cpu.h>
#include <asm/arch/hardware.h>
#include <asm/arch/ioc_rk3588.h>
#include <dt-bindings/clock/rk3588-cru.h>

DECLARE_GLOBAL_DATA_PTR;

#define FIREWALL_DDR_BASE		0xfe030000
#define FW_DDR_MST5_REG			0x54
#define FW_DDR_MST13_REG		0x74
#define FW_DDR_MST21_REG		0x94
#define FW_DDR_MST26_REG		0xa8
#define FW_DDR_MST27_REG		0xac
#define FIREWALL_SYSMEM_BASE		0xfe038000
#define FW_SYSM_MST5_REG		0x54
#define FW_SYSM_MST13_REG		0x74
#define FW_SYSM_MST21_REG		0x94
#define FW_SYSM_MST26_REG		0xa8
#define FW_SYSM_MST27_REG		0xac
#define SYS_GRF_BASE			0xfd58c000
#define SYS_GRF_SOC_CON6		0x0318
#define USBGRF_BASE			0xfd5ac000
#define USB_GRF_USB3OTG0_CON1		0x001c
#define BUS_SGRF_BASE			0xfd586000
#define BUS_SGRF_FIREWALL_CON18		0x288
#define PMU_BASE			0xfd8d0000
#define PMU_PWR_GATE_SFTCON1		0x8150

#define USB2PHY1_GRF_BASE		0xfd5d4000
#define USB2PHY2_GRF_BASE		0xfd5d8000
#define USB2PHY3_GRF_BASE		0xfd5dc000
#define USB2PHY_GRF_CON2		0x0008

#define PMU1_IOC_BASE			0xfd5f0000
#define PMU2_IOC_BASE			0xfd5f4000

#define BUS_IOC_BASE			0xfd5f8000
#define BUS_IOC_GPIO2A_IOMUX_SEL_L	0x40
#define BUS_IOC_GPIO2B_IOMUX_SEL_L	0x48
#define BUS_IOC_GPIO2D_IOMUX_SEL_L	0x58
#define BUS_IOC_GPIO2D_IOMUX_SEL_H	0x5c
#define BUS_IOC_GPIO3A_IOMUX_SEL_L	0x60

#define VCCIO3_5_IOC_BASE		0xfd5fa000
#define IOC_VCCIO3_5_GPIO2A_DS_H	0x44
#define IOC_VCCIO3_5_GPIO2B_DS_L	0x48
#define IOC_VCCIO3_5_GPIO2B_DS_H	0x4c
#define IOC_VCCIO3_5_GPIO3A_DS_L	0x60
#define IOC_VCCIO3_5_GPIO3A_DS_H	0x64
#define IOC_VCCIO3_5_GPIO3C_DS_H	0x74

#define EMMC_IOC_BASE			0xfd5fd000
#define EMMC_IOC_GPIO2A_DS_L		0x40
#define EMMC_IOC_GPIO2D_DS_L		0x58
#define EMMC_IOC_GPIO2D_DS_H		0x5c

#define CRU_BASE			0xfd7c0000
#define CRU_SOFTRST_CON77		0x0b34

#define PMU1CRU_BASE			0xfd7f0000
#define PMU1CRU_SOFTRST_CON03		0x0a0c
#define PMU1CRU_SOFTRST_CON04		0x0a10

#define HDMIRX_NODE_FDT_PATH		"/hdmirx-controller@fdee0000"
#define RK3588_PHY_CONFIG		0xfdee00c0

#ifdef CONFIG_ARM64
#include <asm/armv8/mmu.h>

static struct mm_region rk3588_mem_map[] = {
	{
		.virt = 0x0UL,
		.phys = 0x0UL,
		.size = 0xf0000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_NORMAL) |
			 PTE_BLOCK_INNER_SHARE
	}, {
		.virt = 0xf0000000UL,
		.phys = 0xf0000000UL,
		.size = 0x10000000UL,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},  {
		.virt = 0x900000000,
		.phys = 0x900000000,
		.size = 0x150000000,
		.attrs = PTE_BLOCK_MEMTYPE(MT_DEVICE_NGNRNE) |
			 PTE_BLOCK_NON_SHARE |
			 PTE_BLOCK_PXN | PTE_BLOCK_UXN
	},  {
		/* List terminator */
		0,
	}
};

struct mm_region *mem_map = rk3588_mem_map;
#endif

/* GPIO0B_IOMUX_SEL_L */
enum {
	GPIO0B0_SHIFT		= 0,
	GPIO0B0_MASK		= GENMASK(3, 0),
	GPIO0B0_UART0_RX_M1	= 4,

	GPIO0B1_SHIFT		= 4,
	GPIO0B1_MASK		= GENMASK(7, 4),
	GPIO0B1_UART0_TX_M1	= 4,
};

/* GPIO0C_IOMUX_SEL_H */
enum {
	GPIO0C4_SHIFT		= 0,
	GPIO0C4_MASK		= GENMASK(3, 0),
	GPIO0C4_UART0_RX_M0	= 4,

	GPIO0C5_SHIFT		= 4,
	GPIO0C5_MASK		= GENMASK(7, 4),
	GPIO0C5_UART0_TX_M0	= 4,
};

/* GPIO0B_IOMUX_SEL_H */
enum {
	GPIO0B5_SHIFT		= 4,
	GPIO0B5_MASK		= GENMASK(7, 4),
	GPIO0B5_REFER		= 8,
	GPIO0B5_UART2_TX_M0	= 10,

	GPIO0B6_SHIFT		= 8,
	GPIO0B6_MASK		= GENMASK(11, 8),
	GPIO0B6_REFER		= 8,
	GPIO0B6_UART2_RX_M0	= 10,
};

/* GPIO0D_IOMUX_SEL_L */
enum {
	GPIO0D1_SHIFT		= 4,
	GPIO0D1_MASK		= GENMASK(7, 4),
	GPIO0D1_REFER		= 8,
	GPIO0D1_UART1_TX_M2	= 10,

	GPIO0D2_SHIFT		= 8,
	GPIO0D2_MASK		= GENMASK(11, 8),
	GPIO0D2_REFER		= 8,
	GPIO0D2_UART1_RX_M2	= 10,
};

/* GPIO1A_IOMUX_SEL_L */
enum {
	GPIO1A0_SHIFT		= 0,
	GPIO1A0_MASK		= GENMASK(3, 0),
	GPIO1A0_UART6_RX_M1	= 10,

	GPIO1A1_SHIFT		= 4,
	GPIO1A1_MASK		= GENMASK(7, 4),
	GPIO1A1_UART6_TX_M1	= 10,
};

/* GPIO1B_IOMUX_SEL_L */
enum {
	GPIO1B2_SHIFT		= 8,
	GPIO1B2_MASK		= GENMASK(11, 8),
	GPIO1B2_UART4_RX_M2	= 10,

	GPIO1B3_SHIFT		= 12,
	GPIO1B3_MASK		= GENMASK(15, 12),
	GPIO1B3_UART4_TX_M2	= 10,
};

/* GPIO1B_IOMUX_SEL_H */
enum {
	GPIO1B4_SHIFT		= 0,
	GPIO1B4_MASK		= GENMASK(3, 0),
	GPIO1B4_UART7_RX_M2	= 10,

	GPIO1B5_SHIFT		= 4,
	GPIO1B5_MASK		= GENMASK(7, 4),
	GPIO1B5_UART7_TX_M2	= 10,

	GPIO1B6_SHIFT		= 8,
	GPIO1B6_MASK		= GENMASK(11, 8),
	GPIO1B6_UART1_TX_M1	= 10,

	GPIO1B7_SHIFT		= 12,
	GPIO1B7_MASK		= GENMASK(15, 12),
	GPIO1B7_UART1_RX_M1	= 10,
};

/* GPIO1C_IOMUX_SEL_L */
enum {
	GPIO1C0_SHIFT		= 0,
	GPIO1C0_MASK		= GENMASK(3, 0),
	GPIO1C0_UART3_RX_M0	= 10,

	GPIO1C1_SHIFT		= 4,
	GPIO1C1_MASK		= GENMASK(7, 4),
	GPIO1C1_UART3_TX_M0	= 10,
};

/* GPIO1D_IOMUX_SEL_L */
enum {
	GPIO1D0_SHIFT		= 0,
	GPIO1D0_MASK		= GENMASK(3, 0),
	GPIO1D0_UART6_TX_M2	= 10,

	GPIO1D1_SHIFT		= 4,
	GPIO1D1_MASK		= GENMASK(7, 4),
	GPIO1D1_UART6_RX_M2	= 10,

	GPIO1D2_SHIFT		= 8,
	GPIO1D2_MASK		= GENMASK(11, 8),
	GPIO1D2_UART4_TX_M0	= 10,

	GPIO1D3_SHIFT		= 12,
	GPIO1D3_MASK		= GENMASK(15, 12),
	GPIO1D3_UART4_RX_M0	= 10,
};

/* GPIO2A_IOMUX_SEL_H */
enum {
	GPIO2A6_SHIFT		= 8,
	GPIO2A6_MASK		= GENMASK(11, 8),
	GPIO2A6_UART6_RX_M0	= 10,

	GPIO2A7_SHIFT		= 12,
	GPIO2A7_MASK		= GENMASK(15, 12),
	GPIO2A7_UART6_TX_M0	= 10,
};

/* GPIO2B_IOMUX_SEL_H */
enum {
	GPIO2B4_SHIFT		= 0,
	GPIO2B4_MASK		= GENMASK(3, 0),
	GPIO2B4_UART7_RX_M0	= 10,

	GPIO2B5_SHIFT		= 4,
	GPIO2B5_MASK		= GENMASK(7, 4),
	GPIO2B5_UART7_TX_M0	= 10,

	GPIO2B6_SHIFT		= 8,
	GPIO2B6_MASK		= GENMASK(11, 8),
	GPIO2B6_UART1_RX_M0	= 10,

	GPIO2B7_SHIFT		= 12,
	GPIO2B7_MASK		= GENMASK(15, 12),
	GPIO2B7_UART1_TX_M0	= 10,
};

/* GPIO2C_IOMUX_SEL_L */
enum {
	GPIO2C2_SHIFT		= 8,
	GPIO2C2_MASK		= GENMASK(11, 8),
	GPIO2C2_UART9_TX_M0	= 10,
};

/* GPIO2C_IOMUX_SEL_H */
enum {
	GPIO2C4_SHIFT		= 0,
	GPIO2C4_MASK		= GENMASK(3, 0),
	GPIO2C4_UART9_RX_M0	= 10,
};

/* GPIO2D_IOMUX_SEL_H */
enum {
	GPIO2D4_SHIFT		= 0,
	GPIO2D4_MASK		= GENMASK(3, 0),
	GPIO2D4_UART5_RX_M2	= 10,

	GPIO2D5_SHIFT		= 4,
	GPIO2D5_MASK		= GENMASK(7, 4),
	GPIO2D5_UART5_TX_M2	= 10,
};

/* GPIO3A_IOMUX_SEL_H */
enum {
	GPIO3A2_SHIFT		= 8,
	GPIO3A2_MASK		= GENMASK(11, 8),
	GPIO3A2_UART8_TX_M1	= 10,

	GPIO3A3_SHIFT		= 12,
	GPIO3A3_MASK		= GENMASK(15, 12),
	GPIO3A3_UART8_RX_M1	= 10,
};

/* GPIO3B_IOMUX_SEL_L */
enum {
	GPIO3B1_SHIFT		= 4,
	GPIO3B1_MASK		= GENMASK(7, 4),
	GPIO3B1_UART2_TX_M2	= 10,

	GPIO3B2_SHIFT		= 8,
	GPIO3B2_MASK		= GENMASK(11, 8),
	GPIO3B2_UART2_RX_M2	= 10,
};

/* GPIO3B_IOMUX_SEL_H */
enum {
	GPIO3B5_SHIFT		= 4,
	GPIO3B5_MASK		= GENMASK(7, 4),
	GPIO3B5_UART3_TX_M1	= 10,

	GPIO3B6_SHIFT		= 8,
	GPIO3B6_MASK		= GENMASK(11, 8),
	GPIO3B6_UART3_RX_M1	= 10,
};

/* GPIO3C_IOMUX_SEL_L */
enum {
	GPIO3C0_SHIFT		= 0,
	GPIO3C0_MASK		= GENMASK(3, 0),
	GPIO3C0_UART7_TX_M1	= 10,

	GPIO3C1_SHIFT		= 4,
	GPIO3C1_MASK		= GENMASK(7, 4),
	GPIO3C1_UART7_RX_M1	= 10,
};

/* GPIO3C_IOMUX_SEL_H */
enum {
	GPIO3C4_SHIFT		= 0,
	GPIO3C4_MASK		= GENMASK(3, 0),
	GPIO3C4_UART5_TX_M1	= 10,

	GPIO3C5_SHIFT		= 4,
	GPIO3C5_MASK		= GENMASK(7, 4),
	GPIO3C5_UART5_RX_M1	= 10,
};

/* GPIO3D_IOMUX_SEL_L */
enum {
	GPIO3D0_SHIFT		= 0,
	GPIO3D0_MASK		= GENMASK(3, 0),
	GPIO3D0_UART4_RX_M1	= 10,

	GPIO3D1_SHIFT		= 4,
	GPIO3D1_MASK		= GENMASK(7, 4),
	GPIO3D1_UART4_TX_M1	= 10,
};

/* GPIO3D_IOMUX_SEL_H */
enum {
	GPIO3D4_SHIFT		= 0,
	GPIO3D4_MASK		= GENMASK(3, 0),
	GPIO3D4_UART9_RX_M2	= 10,

	GPIO3D5_SHIFT		= 4,
	GPIO3D5_MASK		= GENMASK(7, 4),
	GPIO3D5_UART9_TX_M2	= 10,
};

/* GPIO4A_IOMUX_SEL_L */
enum {
	GPIO4A3_SHIFT		= 12,
	GPIO4A3_MASK		= GENMASK(15, 12),
	GPIO4A3_UART0_TX_M2	= 10,
};

/* GPIO4A_IOMUX_SEL_H */
enum {
	GPIO4A4_SHIFT		= 0,
	GPIO4A4_MASK		= GENMASK(3, 0),
	GPIO4A4_UART0_RX_M2	= 10,

	GPIO4A5_SHIFT		= 4,
	GPIO4A5_MASK		= GENMASK(7, 4),
	GPIO4A5_UART3_TX_M2	= 10,

	GPIO4A6_SHIFT		= 8,
	GPIO4A6_MASK		= GENMASK(11, 8),
	GPIO4A6_UART3_RX_M2	= 10,
};

/* GPIO4B_IOMUX_SEL_L */
enum {
	GPIO4B0_SHIFT		= 0,
	GPIO4B0_MASK		= GENMASK(3, 0),
	GPIO4B0_UART8_TX_M0	= 10,

	GPIO4B1_SHIFT		= 4,
	GPIO4B1_MASK		= GENMASK(7, 4),
	GPIO4B1_UART8_RX_M0	= 10,
};

/* GPIO4B_IOMUX_SEL_H */
enum {
	GPIO4B4_SHIFT		= 0,
	GPIO4B4_MASK		= GENMASK(3, 0),
	GPIO4B4_UART9_TX_M1	= 10,

	GPIO4B5_SHIFT		= 4,
	GPIO4B5_MASK		= GENMASK(7, 4),
	GPIO4B5_UART9_RX_M1	= 10,
};

/* GPIO4D_IOMUX_SEL_L */
enum {
	GPIO4D0_SHIFT		= 0,
	GPIO4D0_MASK		= GENMASK(3, 0),
	GPIO4D0_GPIO		= 0,
	GPIO4D0_SDMMC_D0	= 1,
	GPIO4D0_PDM1_SDI3_M0	= 2,
	GPIO4D0_JTAG_TCK_M1	= 5,
	GPIO4D0_I2C3_SCL_M4	= 9,
	GPIO4D0_UART2_TX_M1	= 10,
	GPIO4D0_PWM8_M1		= 12,

	GPIO4D1_SHIFT		= 4,
	GPIO4D1_MASK		= GENMASK(7, 4),
	GPIO4D1_GPIO		= 0,
	GPIO4D1_SDMMC_D1	= 1,
	GPIO4D1_PDM1_SDI2_M0	= 2,
	GPIO4D1_JTAG_TMS_M1	= 5,
	GPIO4D1_I2C3_SDA_M4	= 9,
	GPIO4D1_UART2_RX_M1	= 10,
	GPIO4D1_PWM9_M1		= 12,
};

/* GPIO4D_IOMUX_SEL_H */
enum {
	GPIO4D4_SHIFT		= 0,
	GPIO4D4_MASK		= GENMASK(3, 0),
	GPIO4D4_UART5_RX_M0	= 10,

	GPIO4D5_SHIFT		= 4,
	GPIO4D5_MASK		= GENMASK(7, 4),
	GPIO4D5_UART5_TX_M0	= 10,
};

void board_debug_uart_init(void)
{
	__maybe_unused static struct rk3588_bus_ioc * const bus_ioc = (void *)BUS_IOC_BASE;

/* UART 0 */
#if defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfd890000)

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)
	static struct rk3588_pmu2_ioc * const pmu2_ioc = (void *)PMU2_IOC_BASE;

	/* UART0_M0 Switch iomux */
	rk_clrsetreg(&pmu2_ioc->gpio0c_iomux_sel_h,
			GPIO0C4_MASK | GPIO0C5_MASK,
			GPIO0C4_UART0_RX_M0 << GPIO0C4_SHIFT |
			GPIO0C5_UART0_TX_M0 << GPIO0C5_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)
	static struct rk3588_pmu1_ioc * const pmu1_ioc = (void *)PMU1_IOC_BASE;

	/* UART0_M1 Switch iomux */
	rk_clrsetreg(&pmu1_ioc->gpio0b_iomux_sel_l,
			GPIO0B0_MASK | GPIO0B1_MASK,
			GPIO0B0_UART0_RX_M1 << GPIO0B0_SHIFT |
			GPIO0B1_UART0_TX_M1 << GPIO0B1_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART0_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio4a_iomux_sel_h,
			GPIO4A4_MASK,
			GPIO4A4_UART0_RX_M2 << GPIO4A4_SHIFT);
	rk_clrsetreg(&bus_ioc->gpio4a_iomux_sel_l,
			GPIO4A3_MASK,
			GPIO4A3_UART0_TX_M2 << GPIO4A3_SHIFT);

#endif

/* UART 1 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfeb40000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART1_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio2b_iomux_sel_h,
			GPIO2B6_MASK | GPIO2B7_MASK,
			GPIO2B6_UART1_RX_M0 << GPIO2B6_SHIFT |
			GPIO2B7_UART1_TX_M0 << GPIO2B7_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART1_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio1b_iomux_sel_h,
			GPIO1B7_MASK | GPIO1B6_MASK,
			GPIO1B7_UART1_RX_M1 << GPIO1B7_SHIFT |
			GPIO1B6_UART1_TX_M1 << GPIO1B6_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	static struct rk3588_pmu2_ioc * const pmu2_ioc = (void *)PMU2_IOC_BASE;

	/* Refer to BUS_IOC */
	rk_clrsetreg(&pmu2_ioc->gpio0d_iomux_sel_l,
			GPIO0D2_MASK | GPIO0D1_MASK,
			GPIO0D2_REFER << GPIO0D2_SHIFT |
			GPIO0D1_REFER << GPIO0D1_SHIFT);

	/* UART1_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio0d_iomux_sel_l,
			GPIO0D2_MASK | GPIO0D1_MASK,
			GPIO0D2_UART1_RX_M2 << GPIO0D2_SHIFT |
			GPIO0D1_UART1_TX_M2 << GPIO0D1_SHIFT);

#endif

/* UART 2 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfeb50000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	static struct rk3588_pmu2_ioc * const pmu2_ioc = (void *)PMU2_IOC_BASE;

	/* Refer to BUS_IOC */
	rk_clrsetreg(&pmu2_ioc->gpio0b_iomux_sel_h,
			GPIO0B6_MASK | GPIO0B5_MASK,
			GPIO0B6_REFER << GPIO0B6_SHIFT |
			GPIO0B5_REFER << GPIO0B5_SHIFT);

	/* UART2_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio0b_iomux_sel_h,
			GPIO0B6_MASK | GPIO0B5_MASK,
			GPIO0B6_UART2_RX_M0 << GPIO0B6_SHIFT |
			GPIO0B5_UART2_TX_M0 << GPIO0B5_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART2_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio4d_iomux_sel_l,
			GPIO4D1_MASK | GPIO4D0_MASK,
			GPIO4D1_UART2_RX_M1 << GPIO4D1_SHIFT |
			GPIO4D0_UART2_TX_M1 << GPIO4D0_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART2_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio3b_iomux_sel_l,
			GPIO3B2_MASK | GPIO3B1_MASK,
			GPIO3B2_UART2_RX_M2 << GPIO3B2_SHIFT |
			GPIO3B1_UART2_TX_M2 << GPIO3B1_SHIFT);

#endif

/* UART 3 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfeb60000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART3_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio1c_iomux_sel_l,
			GPIO1C0_MASK | GPIO1C1_MASK,
			GPIO1C0_UART3_RX_M0 << GPIO1C0_SHIFT |
			GPIO1C1_UART3_TX_M0 << GPIO1C1_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART3_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio3b_iomux_sel_h,
			GPIO3B6_MASK | GPIO3B5_MASK,
			GPIO3B6_UART3_RX_M1 << GPIO3B6_SHIFT |
			GPIO3B5_UART3_TX_M1 << GPIO3B5_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART3_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio4a_iomux_sel_h,
			GPIO4A6_MASK | GPIO4A5_MASK,
			GPIO4A6_UART3_RX_M2 << GPIO4A6_SHIFT |
			GPIO4A5_UART3_TX_M2 << GPIO4A5_SHIFT);

#endif

/* UART 4 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfeb70000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART4_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio1d_iomux_sel_l,
			GPIO1D3_MASK | GPIO1D2_MASK,
			GPIO1D3_UART4_RX_M0 << GPIO1D3_SHIFT |
			GPIO1D2_UART4_TX_M0 << GPIO1D2_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART4_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio3d_iomux_sel_l,
			GPIO3D0_MASK | GPIO3D1_MASK,
			GPIO3D0_UART4_RX_M1 << GPIO3D0_SHIFT |
			GPIO3D1_UART4_TX_M1 << GPIO3D1_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART4_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio1b_iomux_sel_l,
			GPIO1B2_MASK | GPIO1B3_MASK,
			GPIO1B2_UART4_RX_M2 << GPIO1B2_SHIFT |
			GPIO1B3_UART4_TX_M2 << GPIO1B3_SHIFT);

#endif

/* UART 5 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfeb80000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART5_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio4d_iomux_sel_h,
			GPIO4D4_MASK | GPIO4D5_MASK,
			GPIO4D4_UART5_RX_M0 << GPIO4D4_SHIFT |
			GPIO4D5_UART5_TX_M0 << GPIO4D5_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART5_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio3c_iomux_sel_h,
			GPIO3C5_MASK | GPIO3C4_MASK,
			GPIO3C5_UART5_RX_M1 << GPIO3C5_SHIFT |
			GPIO3C4_UART5_TX_M1 << GPIO3C4_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART5_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio2d_iomux_sel_h,
			GPIO2D4_MASK | GPIO2D5_MASK,
			GPIO2D4_UART5_RX_M2 << GPIO2D4_SHIFT |
			GPIO2D5_UART5_TX_M2 << GPIO2D5_SHIFT);

#endif

/* UART 6 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfeb90000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART6_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio2a_iomux_sel_h,
			GPIO2A6_MASK | GPIO2A7_MASK,
			GPIO2A6_UART6_RX_M0 << GPIO2A6_SHIFT |
			GPIO2A7_UART6_TX_M0 << GPIO2A7_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART6_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio1a_iomux_sel_l,
			GPIO1A0_MASK | GPIO1A1_MASK,
			GPIO1A0_UART6_RX_M1 << GPIO1A0_SHIFT |
			GPIO1A1_UART6_TX_M1 << GPIO1A1_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART6_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio1d_iomux_sel_l,
			GPIO1D1_MASK | GPIO1D0_MASK,
			GPIO1D1_UART6_RX_M2 << GPIO1D1_SHIFT |
			GPIO1D0_UART6_TX_M2 << GPIO1D0_SHIFT);

#endif

/* UART 7 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfeba0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART7_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio2b_iomux_sel_h,
			GPIO2B4_MASK | GPIO2B5_MASK,
			GPIO2B4_UART7_RX_M0 << GPIO2B4_SHIFT |
			GPIO2B5_UART7_TX_M0 << GPIO2B5_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART7_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio3c_iomux_sel_l,
			GPIO3C1_MASK | GPIO3C0_MASK,
			GPIO3C1_UART7_RX_M1 << GPIO3C1_SHIFT |
			GPIO3C0_UART7_TX_M1 << GPIO3C0_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART7_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio1b_iomux_sel_h,
			GPIO1B4_MASK | GPIO1B5_MASK,
			GPIO1B4_UART7_RX_M2 << GPIO1B4_SHIFT |
			GPIO1B5_UART7_TX_M2 << GPIO1B5_SHIFT);

#endif

/* UART 8 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfebb0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART8_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio4b_iomux_sel_l,
			GPIO4B1_MASK | GPIO4B0_MASK,
			GPIO4B1_UART8_RX_M0 << GPIO4B1_SHIFT |
			GPIO4B0_UART8_TX_M0 << GPIO4B0_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART8_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio3a_iomux_sel_l,
			GPIO3A3_MASK | GPIO3A2_MASK,
			GPIO3A3_UART8_RX_M1 << GPIO3A3_SHIFT |
			GPIO3A2_UART8_TX_M1 << GPIO3A2_SHIFT);

#endif

/* UART 9 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xfebc0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART9_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio2c_iomux_sel_h,
			GPIO2C4_MASK,
			GPIO2C4_UART9_RX_M0 << GPIO2C4_SHIFT);

	/* UART9_M0 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio2c_iomux_sel_l,
			GPIO2C2_MASK,
			GPIO2C2_UART9_TX_M0 << GPIO2C2_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART9_M1 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio4b_iomux_sel_h,
			GPIO4B5_MASK | GPIO4B4_MASK,
			GPIO4B5_UART9_RX_M1 << GPIO4B5_SHIFT |
			GPIO4B4_UART9_TX_M1 << GPIO4B4_SHIFT);

#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART9_M2 Switch iomux */
	rk_clrsetreg(&bus_ioc->gpio3d_iomux_sel_h,
			GPIO3D4_MASK | GPIO3D5_MASK,
			GPIO3D4_UART9_RX_M2 << GPIO3D4_SHIFT |
			GPIO3D5_UART9_TX_M2 << GPIO3D5_SHIFT);

#endif

#endif
}

#ifdef CONFIG_SPL_BUILD
void rockchip_stimer_init(void)
{
	/* If Timer already enabled, don't re-init it */
	u32 reg = readl(CONFIG_ROCKCHIP_STIMER_BASE + 0x4);

	if (reg & 0x1)
		return;

	asm volatile("msr CNTFRQ_EL0, %0" : : "r" (COUNTER_FREQUENCY));
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 0x14);
	writel(0xffffffff, CONFIG_ROCKCHIP_STIMER_BASE + 0x18);
	writel(0x1, CONFIG_ROCKCHIP_STIMER_BASE + 0x4);
}

static u32 gpio4d_iomux_sel_l = 0xffffffff;
static u32 gpio4d_iomux_sel_h;
static u32 gpio0a_iomux_sel_h;

void spl_board_sd_iomux_save(void)
{
	struct rk3588_bus_ioc * const bus_ioc = (void *)BUS_IOC_BASE;
	struct rk3588_pmu1_ioc * const pmu1_ioc = (void *)PMU1_IOC_BASE;

	gpio4d_iomux_sel_l = readl(&bus_ioc->gpio4d_iomux_sel_l);
	gpio4d_iomux_sel_h = readl(&bus_ioc->gpio4d_iomux_sel_h);
	gpio0a_iomux_sel_h = readl(&pmu1_ioc->gpio0a_iomux_sel_h);
}

void spl_board_storages_fixup(struct spl_image_loader *loader)
{
	int ret = 0;

	if (!loader)
		return;

	if (loader->boot_device == BOOT_DEVICE_MMC2 && gpio4d_iomux_sel_l != 0xffffffff) {
		struct rk3588_bus_ioc * const bus_ioc = (void *)BUS_IOC_BASE;
		struct rk3588_pmu1_ioc * const pmu1_ioc = (void *)PMU1_IOC_BASE;
		struct mmc *mmc = NULL;
		bool no_card;

		ret = spl_mmc_find_device(&mmc, BOOT_DEVICE_MMC2);
		if (ret)
			return;

		no_card = mmc_getcd(mmc) == 0;
		if (no_card) {
			writel(0xffffuL << 16 | gpio4d_iomux_sel_l, &bus_ioc->gpio4d_iomux_sel_l);
			writel(0xffffuL << 16 | gpio4d_iomux_sel_h, &bus_ioc->gpio4d_iomux_sel_h);
			writel(0xffffuL << 16 | gpio0a_iomux_sel_h, &pmu1_ioc->gpio0a_iomux_sel_h);
		}
	}
}
#endif

#ifndef CONFIG_TPL_BUILD
int arch_cpu_init(void)
{
#ifdef CONFIG_SPL_BUILD
	int secure_reg;

	/* Set the SDMMC eMMC crypto_ns FSPI access secure area */
	secure_reg = readl(FIREWALL_DDR_BASE + FW_DDR_MST5_REG);
	secure_reg &= 0xffff;
	writel(secure_reg, FIREWALL_DDR_BASE + FW_DDR_MST5_REG);
	secure_reg = readl(FIREWALL_DDR_BASE + FW_DDR_MST13_REG);
	secure_reg &= 0xffff;
	writel(secure_reg, FIREWALL_DDR_BASE + FW_DDR_MST13_REG);
	secure_reg = readl(FIREWALL_DDR_BASE + FW_DDR_MST21_REG);
	secure_reg &= 0xffff;
	writel(secure_reg, FIREWALL_DDR_BASE + FW_DDR_MST21_REG);
	secure_reg = readl(FIREWALL_DDR_BASE + FW_DDR_MST26_REG);
	secure_reg &= 0xffff;
	writel(secure_reg, FIREWALL_DDR_BASE + FW_DDR_MST26_REG);
	secure_reg = readl(FIREWALL_DDR_BASE + FW_DDR_MST27_REG);
	secure_reg &= 0xffff0000;
	writel(secure_reg, FIREWALL_DDR_BASE + FW_DDR_MST27_REG);

	secure_reg = readl(FIREWALL_SYSMEM_BASE + FW_SYSM_MST5_REG);
	secure_reg &= 0xffff;
	writel(secure_reg, FIREWALL_SYSMEM_BASE + FW_SYSM_MST5_REG);
	secure_reg = readl(FIREWALL_SYSMEM_BASE + FW_SYSM_MST13_REG);
	secure_reg &= 0xffff;
	writel(secure_reg, FIREWALL_SYSMEM_BASE + FW_SYSM_MST13_REG);
	secure_reg = readl(FIREWALL_SYSMEM_BASE + FW_SYSM_MST21_REG);
	secure_reg &= 0xffff;
	writel(secure_reg, FIREWALL_SYSMEM_BASE + FW_SYSM_MST21_REG);
	secure_reg = readl(FIREWALL_SYSMEM_BASE + FW_SYSM_MST26_REG);
	secure_reg &= 0xffff;
	writel(secure_reg, FIREWALL_SYSMEM_BASE + FW_SYSM_MST26_REG);
	secure_reg = readl(FIREWALL_SYSMEM_BASE + FW_SYSM_MST27_REG);
	secure_reg &= 0xffff0000;
	writel(secure_reg, FIREWALL_SYSMEM_BASE + FW_SYSM_MST27_REG);

	/* Select clk_tx source as default for i2s2/i2s3 */
	writel(0x03400340, SYS_GRF_BASE + SYS_GRF_SOC_CON6);

	if (readl(BUS_IOC_BASE + BUS_IOC_GPIO2D_IOMUX_SEL_L) == 0x2222) {
		/* Set the fspi m0 io ds level to 55ohm */
		writel(0x00070002, EMMC_IOC_BASE + EMMC_IOC_GPIO2A_DS_L);
		writel(0x77772222, EMMC_IOC_BASE + EMMC_IOC_GPIO2D_DS_L);
		writel(0x07000200, EMMC_IOC_BASE + EMMC_IOC_GPIO2D_DS_H);
	} else if (readl(BUS_IOC_BASE + BUS_IOC_GPIO2D_IOMUX_SEL_L) == 0x1111) {
		/*
		 * set the emmc io drive strength:
		 * data and cmd: 50ohm
		 * clock: 25ohm
		 */
		writel(0x00770052, EMMC_IOC_BASE + EMMC_IOC_GPIO2A_DS_L);
		writel(0x77772222, EMMC_IOC_BASE + EMMC_IOC_GPIO2D_DS_L);
		writel(0x77772222, EMMC_IOC_BASE + EMMC_IOC_GPIO2D_DS_H);
	} else if ((readl(BUS_IOC_BASE + BUS_IOC_GPIO2B_IOMUX_SEL_L) & 0xf0ff) == 0x3033) {
		/* Set the fspi m1 io ds level to 55ohm */
		writel(0x33002200, VCCIO3_5_IOC_BASE + IOC_VCCIO3_5_GPIO2A_DS_H);
		writel(0x30332022, VCCIO3_5_IOC_BASE + IOC_VCCIO3_5_GPIO2B_DS_L);
		writel(0x00030002, VCCIO3_5_IOC_BASE + IOC_VCCIO3_5_GPIO2B_DS_H);
	} else if (readl(BUS_IOC_BASE + BUS_IOC_GPIO3A_IOMUX_SEL_L) == 0x5555) {
		/* Set the fspi m2 io ds level to 55ohm */
		writel(0x77772222, VCCIO3_5_IOC_BASE + IOC_VCCIO3_5_GPIO3A_DS_L);
		writel(0x00700020, VCCIO3_5_IOC_BASE + IOC_VCCIO3_5_GPIO3A_DS_H);
		writel(0x00070002, VCCIO3_5_IOC_BASE + IOC_VCCIO3_5_GPIO3C_DS_H);
	}

	/*
	 * Assert reset the pipephy0, pipephy1 and pipephy2,
	 * and de-assert reset them in Kernel combphy driver.
	 */
	writel(0x01c001c0, CRU_BASE + CRU_SOFTRST_CON77);

	/*
	 * Assert SIDDQ for USB 2.0 PHY1, PHY2 and PHY3 to
	 * power down all analog block to save power. And
	 * PHY0 for OTG0 interface still in normal mode.
	 */
	writel(0x20002000, USB2PHY1_GRF_BASE + USB2PHY_GRF_CON2);
	writel(0x20002000, USB2PHY2_GRF_BASE + USB2PHY_GRF_CON2);
	writel(0x20002000, USB2PHY3_GRF_BASE + USB2PHY_GRF_CON2);

	/* Assert hdptxphy init,cmn,lane reset */
	writel(0xb800b800, PMU1CRU_BASE + PMU1CRU_SOFTRST_CON03);
	writel(0x00030003, PMU1CRU_BASE + PMU1CRU_SOFTRST_CON04);

	spl_board_sd_iomux_save();
#else /* U-Boot */
	/* uboot: config iomux */
#ifdef CONFIG_ROCKCHIP_EMMC_IOMUX
	/* Set emmc iomux for good extention if the emmc is not the boot device */
	writel(0xffff1111, BUS_IOC_BASE + BUS_IOC_GPIO2A_IOMUX_SEL_L);
	writel(0xffff1111, BUS_IOC_BASE + BUS_IOC_GPIO2D_IOMUX_SEL_L);
	writel(0xffff1111, BUS_IOC_BASE + BUS_IOC_GPIO2D_IOMUX_SEL_H);
#endif

#endif

	/* Select usb otg0 phy status to 0 that make rockusb can work at high-speed */
	writel(0x00080008, USBGRF_BASE + USB_GRF_USB3OTG0_CON1);

	return 0;
}
#endif

#define BAD_CPU(mask, n)	((mask) & (1 << (n)))
#define BAD_RKVENC(mask, n)	((mask) & (1 << (n)))

static void fdt_rm_path(void *blob, const char *path)
{
	fdt_del_node(blob, fdt_path_offset(blob, path));
}

static void fdt_rm_cooling_map(const void *blob, u8 cpu_mask)
{
	int map1, map2;
	int cpub1_phd;
	int cpub3_phd;
	int node;
	u32 *pp;

	node = fdt_path_offset(blob, "/cpus/cpu@500");
	cpub1_phd = fdtdec_get_uint(blob, node, "phandle", -1);
	node = fdt_path_offset(blob, "/cpus/cpu@700");
	cpub3_phd = fdtdec_get_uint(blob, node, "phandle", -1);

	if (BAD_CPU(cpu_mask, 4)) {
		map1 = fdt_path_offset(blob, "/thermal-zones/soc-thermal/cooling-maps/map1");
		if (map1 > 0) {
			if (BAD_CPU(cpu_mask, 5)) {
				debug("rm: cooling-device map1\n");
				fdt_del_node((void *)blob, map1);
			} else {
				pp = (u32 *)fdt_getprop(blob, map1, "cooling-device", NULL);
				if (pp) {
					pp[0] = cpu_to_fdt32(cpub1_phd);
					debug("fix: cooling-device cpub0->cpub1\n");
				}
			}
		}
	}

	if (BAD_CPU(cpu_mask, 6)) {
		map2 = fdt_path_offset(blob, "/thermal-zones/soc-thermal/cooling-maps/map2");
		if (map2 > 0) {
			if (BAD_CPU(cpu_mask, 7)) {
				debug("rm: cooling-device map2\n");
				fdt_del_node((void *)blob, map2);
			} else {
				pp = (u32 *)fdt_getprop(blob, map2, "cooling-device", NULL);
				if (pp) {
					pp[0] = cpu_to_fdt32(cpub3_phd);
					debug("fix: cooling-device cpub2->cpub3\n");
				}
			}
		}
	}
}

static void fdt_rm_cpu_affinity(const void *blob, u8 cpu_mask)
{
	int i, remain, arm_pmu;
	u32 new_aff[8];
	u32 *aff;

	arm_pmu = fdt_path_offset(blob, "/arm-pmu");
	if (arm_pmu > 0) {
		aff = (u32 *)fdt_getprop(blob, arm_pmu, "interrupt-affinity", NULL);
		if (!aff)
			return;

		for (i = 0, remain = 0; i < 8; i++) {
			if (!BAD_CPU(cpu_mask, i)) {
				new_aff[remain++] = aff[i];
				debug("new_aff: 0x%08x\n", (u32)aff[i]);
			}
		}

		fdt_setprop((void *)blob, arm_pmu, "interrupt-affinity", new_aff, remain * 4);
	}
}

static void fdt_rm_cpu(const void *blob, u8 cpu_mask)
{
	const char *cpu_node_name[] = {
		"cpu@0", "cpu@100", "cpu@200", "cpu@300",
		"cpu@400", "cpu@500", "cpu@600", "cpu@700",
	};
	const char *cluster_core_name[] = {
		"core0", "core1", "core2", "core3",
		"core0", "core1", "core0", "core1",
	};
	const char *cluster_core, *cpu_node;
	int root_cpus, cpu;
	int cluster;
	int i;

	root_cpus = fdt_path_offset(blob, "/cpus");
	if (root_cpus < 0)
		return;

	for (i = 0; i < 8; i++) {
		if (!BAD_CPU(cpu_mask, i))
			continue;

		if (i < 4)
			cluster = fdt_path_offset(blob, "/cpus/cpu-map/cluster0");
		else if (i < 6)
			cluster = fdt_path_offset(blob, "/cpus/cpu-map/cluster1");
		else
			cluster = fdt_path_offset(blob, "/cpus/cpu-map/cluster2");

		if (cluster < 0)
			return;

		cpu_node = cpu_node_name[i];
		cluster_core = cluster_core_name[i];
		debug("rm: %s, %s\n", cpu_node, cluster_core);

		cpu = fdt_subnode_offset(blob, cluster, cluster_core);
		if (cpu > 0)
			fdt_del_node((void *)blob, cpu);

		cpu = fdt_subnode_offset(blob, root_cpus, cpu_node);
		if (cpu > 0)
			fdt_del_node((void *)blob, cpu);
	}

	cluster = fdt_path_offset(blob, "/cpus/cpu-map/cluster1");
	if (BAD_CPU(cpu_mask, 4) && BAD_CPU(cpu_mask, 5)) {
		debug("rm: cpu cluster1\n");
		fdt_del_node((void *)blob, cluster);
	}

	cluster = fdt_path_offset(blob, "/cpus/cpu-map/cluster2");
	if (BAD_CPU(cpu_mask, 6) && BAD_CPU(cpu_mask, 7)) {
		debug("rm: cpu cluster2\n");
		fdt_del_node((void *)blob, cluster);
	} else {
		/* rename, otherwise linux only handles cluster0 */
		if (fdt_path_offset(blob, "/cpus/cpu-map/cluster1") < 0)
			fdt_set_name((void *)blob, cluster, "cluster1");
	}
}

static void fdt_rm_cpus(const void *blob, u8 cpu_mask)
{
	/*
	 * policy:
	 *
	 * 1. both of cores within the same cluster should be normal, otherwise
	 *    remove both of them.
	 * 2. if core4~7 are all normal, remove core6 and core7 anyway.
	 */
	if (BAD_CPU(cpu_mask, 4) || BAD_CPU(cpu_mask, 5))
		cpu_mask |= BIT(4) | BIT(5);
	if (BAD_CPU(cpu_mask, 6) || BAD_CPU(cpu_mask, 7))
		cpu_mask |= BIT(6) | BIT(7);

	if (!BAD_CPU(cpu_mask, 4) & !BAD_CPU(cpu_mask, 5) &&
	    !BAD_CPU(cpu_mask, 6) & !BAD_CPU(cpu_mask, 7))
		cpu_mask |= BIT(6) | BIT(7);

	fdt_rm_cooling_map(blob, cpu_mask);
	fdt_rm_cpu_affinity(blob, cpu_mask);
	fdt_rm_cpu(blob, cpu_mask);
}

static void fdt_rm_gpu(void *blob)
{
	/*
	 * policy:
	 *
	 * Remove GPU by default.
	 */
	fdt_rm_path(blob, "/gpu@fb000000");
	fdt_rm_path(blob, "/thermal-zones/soc-thermal/cooling-maps/map3");
	debug("rm: gpu\n");
}

static void fdt_rm_rkvdec01(void *blob)
{
	/*
	 * policy:
	 *
	 * Remove rkvdec0 and rkvdec1 by default.
	 */
	fdt_rm_path(blob, "/rkvdec-core@fdc38000");
	fdt_rm_path(blob, "/iommu@fdc38700");
	fdt_rm_path(blob, "/rkvdec-core@fdc48000");
	fdt_rm_path(blob, "/iommu@fdc48700");
	debug("rm: rkvdec0, rkvdec1\n");
}

static void fdt_rm_rkvenc01(void *blob, u8 mask)
{
	/*
	 * policy:
	 *
	 * 1. remove bad.
	 * 2. if both of rkvenc0 and rkvenc1 are normal, remove rkvenc1 by default.
	 */
	if (!BAD_RKVENC(mask, 0) && !BAD_RKVENC(mask, 1)) {
		/* rkvenc1 */
		fdt_rm_path(blob, "/rkvenc-core@fdbe0000");
		fdt_rm_path(blob, "/iommu@fdbef000");
		debug("rm: rkvenv1\n");
	} else {
		if (BAD_RKVENC(mask, 0)) {
			fdt_rm_path(blob, "/rkvenc-core@fdbd0000");
			fdt_rm_path(blob, "/iommu@fdbdf000");
			debug("rm: rkvenv0\n");

		}
		if (BAD_RKVENC(mask, 1)) {
			fdt_rm_path(blob, "/rkvenc-core@fdbe0000");
			fdt_rm_path(blob, "/iommu@fdbef000");
			debug("rm: rkvenv1\n");
		}
	}
}

#define CHIP_ID_OFF	2
#define IP_STATE_OFF	29

static int fdt_fixup_modules(void *blob)
{
	struct udevice *dev;
	u8 ip_state[3];
	u8 chip_id[2];
	u8 rkvenc_mask;
	u8 cpu_mask;
	int ret;

	ret = uclass_get_device_by_driver(UCLASS_MISC,
					  DM_GET_DRIVER(rockchip_otp), &dev);
	if (ret) {
		printf("can't get otp device, ret=%d\n", ret);
		return ret;
	}

	ret = misc_read(dev, CHIP_ID_OFF, &chip_id, sizeof(chip_id));
	if (ret) {
		printf("can't read chip id, ret=%d\n", ret);
		return ret;
	}

	debug("# chip: rk%02x%02x\n", chip_id[0], chip_id[1]);

	/* only rk3582 goes further */
	if (!(chip_id[0] == 0x35 && chip_id[1] == 0x82))
		return 0;

	ret = misc_read(dev, IP_STATE_OFF, &ip_state, sizeof(ip_state));
	if (ret) {
		printf("can't read ip state, ret=%d\n", ret);
		return ret;
	}

	/* ip_state[0]: bit0~7 */
	cpu_mask = ip_state[0];
	/* ip_state[2]: bit0,2 */
	rkvenc_mask = (ip_state[2] & 0x1) | ((ip_state[2] & 0x4) >> 1);
#if 0
	/* ip_state[1]: bit1~4 */
	gpu_mask = (ip_state[1] & 0x1e) >> 1;
	/* ip_state[1]: bit6,7 */
	rkvdec_mask = (ip_state[1] & 0xc0) >> 6;
#endif

	debug("hwmask: 0x%02x, 0x%02x, 0x%02x\n", ip_state[0], ip_state[1], ip_state[2]);
	debug("swmask: 0x%02x, 0x%02x\n", cpu_mask, rkvenc_mask);

	/*
	 * RK3582 Policy: gpu/rkvdec are removed by default, the same for other
	 * IP under some condition.
	 *
	 * So don't use pattern like "if (rkvenc_mask) then fdt_rm_rkvenc01()",
	 * just go through all of them as this chip is rk3582.
	 *
	 *		FIXUP WARNING!
	 *
	 * The node delete changes the fdt structure, a node offset you already
	 * got before maybe not right by now. Make sure always reading the node
	 * offset exactly before you are going to use.
	 */
	fdt_rm_gpu(blob);
	fdt_rm_rkvdec01(blob);
	fdt_rm_rkvenc01(blob, rkvenc_mask);
	fdt_rm_cpus(blob, cpu_mask);

	return 0;
}

int rk_board_dm_fdt_fixup(const void *blob)
{
	return fdt_fixup_modules((void *)blob);
}

int rk_board_fdt_fixup(const void *blob)
{
	int node;

	/* set hdmirx to low power mode */
	node = fdt_path_offset(blob, HDMIRX_NODE_FDT_PATH);
	if (node >= 0) {
		if (fdtdec_get_int(blob, node, "low-power-mode", 0)) {
			printf("hdmirx low power mode\n");
			writel(0x00000100, RK3588_PHY_CONFIG);
		}
	}

	return 0;
}

#ifdef CONFIG_SPL_BUILD
int spl_fit_standalone_release(char *id, uintptr_t entry_point)
{
	/* gpll enable */
	writel(0x00f00042, 0xfd7c01c4);
	/* set start addr, pmu_mcu_code_addr_start */
	writel(0xFFFF0000 | (entry_point >> 16), 0xFD582024);
	/* pmu_mcu_sram_addr_start */
	writel(0xFFFF2000, 0xFD582028);
	/* pmu_mcu_tcm_addr_start */
	writel(0xFFFF2000, 0xFD582034);
	/* set mcu secure */
	writel(0x00080000, 0xFD582000);
	/* set cache cache_peripheral_addr */
	writel(0xffff0000, 0xFD582018);
	writel(0xffffee00, 0xFD58201c);
	writel(0x00ff00ff, 0xFD582020);  /* 0xf0000000 ~ 0xfee00000 */
	/* mcupmu access DDR secure control, each bit for a region. */
	writel(0x0000ffff, 0xFE03008C);
	/* mcupmu access DDR secure control, each bit for a region. */
	writel(0x000000ff, 0xFE03808C);
	/* PMU WDT reset system enable */
	writel(0x02000200, 0xFD586008);
	/* WDT trigger global reset. */
	writel(0x08400840, 0xFD7C0C10);
	/* Spl helps to load the mcu image, but not need to release
	 * mcu for rk3588.
	 */

	return 0;
}
#endif
