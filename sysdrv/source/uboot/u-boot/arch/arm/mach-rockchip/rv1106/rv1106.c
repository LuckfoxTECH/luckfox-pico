/*
 * Copyright (c) 2022 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <boot_rkimg.h>
#include <cli.h>
#include <debug_uart.h>
#include <asm/io.h>
#include <asm/arch/hardware.h>
#include <asm/arch/grf_rv1106.h>
#include <asm/arch/ioc_rv1106.h>

DECLARE_GLOBAL_DATA_PTR;

#define PERI_GRF_BASE			0xff000000
#define PERI_GRF_PERI_CON1		0x0004

#define CORE_GRF_BASE			0xff040000
#define CORE_GRF_CACHE_PERI_ADDR_START	0x0024
#define CORE_GRF_CACHE_PERI_ADDR_END	0x0028
#define CORE_GRF_MCU_CACHE_MISC		0x002c

#define PERI_GRF_BASE			0xff000000
#define PERI_GRF_USBPHY_CON0		0x0050

#define PERI_SGRF_BASE			0xff070000
#define PERI_SGRF_FIREWALL_CON0		0x0020
#define PERI_SGRF_FIREWALL_CON1		0x0024
#define PERI_SGRF_FIREWALL_CON2		0x0028
#define PERI_SGRF_FIREWALL_CON3		0x002c
#define PERI_SGRF_FIREWALL_CON4		0x0030
#define PERI_SGRF_SOC_CON3		0x00bc

#define CORE_SGRF_BASE			0xff076000
#define CORE_SGRF_FIREWALL_CON0		0x0020
#define CORE_SGRF_FIREWALL_CON1		0x0024
#define CORE_SGRF_FIREWALL_CON2		0x0028
#define CORE_SGRF_FIREWALL_CON3		0x002c
#define CORE_SGRF_FIREWALL_CON4		0x0030
#define CORE_SGRF_CPU_CTRL_CON		0x0040
#define CORE_SGRF_HPMCU_BOOT_ADDR	0x0044

#define PMU_SGRF_BASE			0xff080000

/* QoS Generator Base Address */
#define QOS_CPU_BASE			0xff110000
#define QOS_CRYPTO_BASE			0xff120000
#define QOS_DECOM_BASE			0xff120080
#define QOS_DMAC_BASE			0xff120100
#define QOS_EMMC_BASE			0xff120180
#define QOS_FSPI_BASE			0xff120200
#define QOS_IVE_RD_BASE			0xff120280
#define QOS_IVE_WR_BASE			0xff120300
#define QOS_USB_BASE			0xff120380
#define QOS_ISP_BASE			0xff130000
#define QOS_SDMMC0_BASE			0xff130080
#define QOS_VICAP_BASE			0xff130100
#define QOS_NPU_BASE			0xff140000
#define QOS_VENC_BASE			0xff150000
#define QOS_VEPU_PP_BASE		0xff150080
#define QOS_MAC_BASE			0xff160000
#define QOS_RGA_RD_BASE			0xff160080
#define QOS_RGA_WR_BASE			0xff160100
#define QOS_SDIO_BASE			0xff160280
#define QOS_VOP_BASE			0xff160300

#define QOS_PRIORITY			0x0008
#define QOS_MODE			0x000c
#define QOS_BANDWIDTH			0x0010
#define QOS_SATURATION			0x0014
#define QOS_EXTCONTROL			0x0018

/* Shaping Base Address */
#define SHAPING_CPU_BASE		0xff110080
#define SHAPING_DECOM_BASE		0xff110400
#define SHAPING_IVE_RD_BASE		0xff120480
#define SHAPING_IVE_WR_BASE		0xff120500
#define SHAPING_ISP_BASE		0xff130180
#define SHAPING_VICAP_BASE		0xff130200
#define SHAPING_NPU_BASE		0xff140080
#define SHAPING_VENC_BASE		0xff150100
#define SHAPING_VEPU_PP_BASE		0xff150180
#define SHAPING_RGA_RD_BASE		0xff160380
#define SHAPING_RGA_WR_BASE		0xff160400
#define SHAPING_VOP_BASE		0xff160580

#define SHAPING_NBPKTMAX		0x0008

#define FW_DDR_BASE			0xff900000
#define FW_DDR_MST3_REG			0x4c
#define FW_SHRM_BASE			0xff910000
#define FW_SHRM_MST1_REG		0x44

#define PMU_BASE			0xff300000
#define PMU_BIU_IDLE_ST			0x00d8

#define CRU_BASE			0xff3b0000
#define CRU_GLB_RST_CON			0x0c10
#define CRU_PVTPLL0_CON0_L		0x1000
#define CRU_PVTPLL0_CON1_L		0x1008
#define CRU_PVTPLL1_CON0_L		0x1030
#define CRU_PVTPLL1_CON1_L		0x1038

#define CORECRU_BASE			0xff3b8000
#define CORECRU_CORESOFTRST_CON01	0xa04

#define USBPHY_APB_BASE			0xff3e0000
#define USBPHY_FSLS_DIFF_RECEIVER	0x0100

#define GPIO0_IOC_BASE			0xFF388000
#define GPIO1_IOC_BASE			0xFF538000
#define GPIO2_IOC_BASE			0xFF548000
#define GPIO3_IOC_BASE			0xFF558000
#define GPIO4_IOC_BASE			0xFF568000

#define GPIO3A_IOMUX_SEL_L		0x0040
#define GPIO3A_IOMUX_SEL_H		0x0044

#define GPIO4A_IOMUX_SEL_L		0x000
#define GPIO4A_IOMUX_SEL_H		0x004
#define GPIO4B_IOMUX_SEL_L		0x008

#define GPIO4_IOC_GPIO4B_DS0		0x0030

/* OS_REG1[2:0]: chip ver */
#define CHIP_VER_REG			0xff020204
#define CHIP_VER_MSK			0x7
#define V(x)				((x) - 1)
#define ROM_VER_REG			0xffff4ffc
#define ROM_V2				0x30303256

/* uart0 iomux */
/* gpio0a0 */
#define UART0_RX_M0			1
#define UART0_RX_M0_OFFSET		0
#define UART0_RX_M0_ADDR		(GPIO1_IOC_BASE)
/* gpio0a1 */
#define UART0_TX_M0			1
#define UART0_TX_M0_OFFSET		4
#define UART0_TX_M0_ADDR		(GPIO1_IOC_BASE)

/* gpio2b0 */
#define UART0_RX_M1			1
#define UART0_RX_M1_OFFSET		0
#define UART0_RX_M1_ADDR		(GPIO2_IOC_BASE + 0x28)
/* gpio2b1 */
#define UART0_TX_M1			1
#define UART0_TX_M1_OFFSET		4
#define UART0_TX_M1_ADDR		(GPIO2_IOC_BASE + 0x28)

/* gpio4a0 */
#define UART0_RX_M2			3
#define UART0_RX_M2_OFFSET		0
#define UART0_RX_M2_ADDR		(GPIO4_IOC_BASE)
/* gpio4a1 */
#define UART0_TX_M2			3
#define UART0_TX_M2_OFFSET		4
#define UART0_TX_M2_ADDR		(GPIO4_IOC_BASE)

/* uart1 iomux */
/* gpio1a4 */
#define UART1_RX_M0			1
#define UART1_RX_M0_OFFSET		0
#define UART1_RX_M0_ADDR		(GPIO1_IOC_BASE + 0x4)
/* gpio1a3 */
#define UART1_TX_M0			1
#define UART1_TX_M0_OFFSET		12
#define UART1_TX_M0_ADDR		(GPIO1_IOC_BASE)

/* gpio2a5 */
#define UART1_RX_M1			4
#define UART1_RX_M1_OFFSET		4
#define UART1_RX_M1_ADDR		(GPIO2_IOC_BASE + 0x24)
/* gpio2a4 */
#define UART1_TX_M1			4
#define UART1_TX_M1_OFFSET		0
#define UART1_TX_M1_ADDR		(GPIO2_IOC_BASE + 0x24)

/* gpio4a7 */
#define UART1_RX_M2			3
#define UART1_RX_M2_OFFSET		12
#define UART1_RX_M2_ADDR		(GPIO4_IOC_BASE + 0x4)
/* gpio4a5 */
#define UART1_TX_M2			3
#define UART1_TX_M2_OFFSET		4
#define UART1_TX_M2_ADDR		(GPIO4_IOC_BASE + 0x4)

/* uart2 iomux */
/* gpio3a3 */
#define UART2_RX_M0			2
#define UART2_RX_M0_OFFSET		12
#define UART2_RX_M0_ADDR		(GPIO3_IOC_BASE + 0x40)
/* gpio3a2 */
#define UART2_TX_M0			2
#define UART2_TX_M0_OFFSET		8
#define UART2_TX_M0_ADDR		(GPIO3_IOC_BASE + 0x40)

/* gpio1b3 */
#define UART2_RX_M1			2
#define UART2_RX_M1_OFFSET		12
#define UART2_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x8)
/* gpio1b2 */
#define UART2_TX_M1			2
#define UART2_TX_M1_OFFSET		8
#define UART2_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x8)

/* uart3 iomux */
/* gpio1a1 */
#define UART3_RX_M0			1
#define UART3_RX_M0_OFFSET		4
#define UART3_RX_M0_ADDR		(GPIO1_IOC_BASE)
/* gpio1a0 */
#define UART3_TX_M0			1
#define UART3_TX_M0_OFFSET		0
#define UART3_TX_M0_ADDR		(GPIO1_IOC_BASE)

/* gpio1d1 */
#define UART3_RX_M1			5
#define UART3_RX_M1_OFFSET		4
#define UART3_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x18)
/* gpio1d0 */
#define UART3_TX_M1			5
#define UART3_TX_M1_OFFSET		0
#define UART3_TX_M1_ADDR		(GPIO2_IOC_BASE + 0x18)

/* uart4 iomux */
/* gpio1b0 */
#define UART4_RX_M0			1
#define UART4_RX_M0_OFFSET		0
#define UART4_RX_M0_ADDR		(GPIO1_IOC_BASE + 0x8)
/* gpio1b1 */
#define UART4_TX_M0			1
#define UART4_TX_M0_OFFSET		4
#define UART4_TX_M0_ADDR		(GPIO1_IOC_BASE + 0x8)

/* gpio1c4 */
#define UART4_RX_M1			4
#define UART4_RX_M1_OFFSET		0
#define UART4_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x14)
/* gpio1c5 */
#define UART4_TX_M1			4
#define UART4_TX_M1_OFFSET		4
#define UART4_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x14)

/* uart5 iomux */
/* gpio3a7 */
#define UART5_RX_M0			2
#define UART5_RX_M0_OFFSET		11
#define UART5_RX_M0_ADDR		(GPIO3_IOC_BASE + 0x44)
/* gpio3a6 */
#define UART5_TX_M0			1
#define UART5_TX_M0_OFFSET		8
#define UART5_TX_M0_ADDR		(GPIO1_IOC_BASE + 0x44)

/* gpio1d2 */
#define UART5_RX_M1			4
#define UART5_RX_M1_OFFSET		8
#define UART5_RX_M1_ADDR		(GPIO1_IOC_BASE + 0x18)
/* gpio1d3 */
#define UART5_TX_M1			4
#define UART5_TX_M1_OFFSET		12
#define UART5_TX_M1_ADDR		(GPIO1_IOC_BASE + 0x18)

/* gpio3d0 */
#define UART5_RX_M2			2
#define UART5_RX_M2_OFFSET		0
#define UART5_RX_M2_ADDR		(GPIO3_IOC_BASE + 0x58)
/* gpio3c7 */
#define UART5_TX_M2			2
#define UART5_TX_M2_OFFSET		12
#define UART5_TX_M2_ADDR		(GPIO4_IOC_BASE + 0x54)

#define set_uart_iomux(bits_offset, bits_val, addr) \
	writel(GENMASK(bits_offset + 19, bits_offset + 16) | (bits_val << bits_offset) , addr)

#define set_uart_iomux_rx(ID, MODE) \
	set_uart_iomux(UART##ID##_RX_M##MODE##_OFFSET, UART##ID##_RX_M##MODE, UART##ID##_RX_M##MODE##_ADDR);
#define set_uart_iomux_tx(ID, MODE) \
	set_uart_iomux(UART##ID##_TX_M##MODE##_OFFSET, UART##ID##_TX_M##MODE, UART##ID##_TX_M##MODE##_ADDR);

void board_debug_uart_init(void)
{
/* UART 0 */
#if defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff4a0000)

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART0_M0 Switch iomux */
	set_uart_iomux_rx(0, 0);
	set_uart_iomux_tx(0, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART0_M1 Switch iomux */
	set_uart_iomux_rx(0, 1);
	set_uart_iomux_tx(0, 1);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART0_M2 Switch iomux */
	set_uart_iomux_rx(0, 2);
	set_uart_iomux_tx(0, 2);
#endif

/* UART 1 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff4b0000)

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART1_M0 Switch iomux */
	set_uart_iomux_rx(1, 0);
	set_uart_iomux_tx(1, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART1_M1 Switch iomux */
	set_uart_iomux_rx(1, 1);
	set_uart_iomux_tx(1, 1);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART1_M2 Switch iomux */
	set_uart_iomux_rx(1, 2);
	set_uart_iomux_tx(1, 2);
#endif
/* UART 2 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff4c0000)

#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART2_M0 Switch iomux */
	set_uart_iomux_rx(2, 0);
	set_uart_iomux_tx(2, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART2_M1 Switch iomux */
	set_uart_iomux_rx(2, 1);
	set_uart_iomux_tx(2, 1);
#endif

/* UART 3 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff4d0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART3_M0 Switch iomux */
	set_uart_iomux_rx(3, 0);
	set_uart_iomux_tx(3, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART3_M1 Switch iomux */
	set_uart_iomux_rx(3, 1);
	set_uart_iomux_tx(3, 1);
#endif
/* UART 4 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff4e0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART4_M0 Switch iomux */
	set_uart_iomux_rx(4, 0);
	set_uart_iomux_tx(4, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART4_M1 Switch iomux */
	set_uart_iomux_rx(4, 1);
	set_uart_iomux_tx(4, 1);
#endif
/* UART 5 */
#elif defined(CONFIG_DEBUG_UART_BASE) && (CONFIG_DEBUG_UART_BASE == 0xff4f0000)
#if defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 0)

	/* UART5_M0 Switch iomux */
	set_uart_iomux_rx(5, 0);
	set_uart_iomux_tx(5, 0);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 1)

	/* UART5_M1 Switch iomux */
	set_uart_iomux_rx(5, 1);
	set_uart_iomux_tx(5, 1);
#elif defined(CONFIG_ROCKCHIP_UART_MUX_SEL_M) && \
	(CONFIG_ROCKCHIP_UART_MUX_SEL_M == 2)

	/* UART5_M2 Switch iomux */
	set_uart_iomux_rx(5, 2);
	set_uart_iomux_tx(5, 2);
#endif
#endif
}

int arch_cpu_init(void)
{
#ifdef CONFIG_SPL_BUILD
	/* Save chip version to OS_REG1[2:0] */
	if (readl(ROM_VER_REG) == ROM_V2)
		writel((readl(CHIP_VER_REG) & ~CHIP_VER_MSK) | V(2), CHIP_VER_REG);
	else
		writel((readl(CHIP_VER_REG) & ~CHIP_VER_MSK) | V(1), CHIP_VER_REG);

	/* Set all devices to Non-secure */
	writel(0xffff0000, PERI_SGRF_BASE + PERI_SGRF_FIREWALL_CON0);
	writel(0xffff0000, PERI_SGRF_BASE + PERI_SGRF_FIREWALL_CON1);
	writel(0xffff0000, PERI_SGRF_BASE + PERI_SGRF_FIREWALL_CON2);
	writel(0xffff0000, PERI_SGRF_BASE + PERI_SGRF_FIREWALL_CON3);
	writel(0xffff0000, PERI_SGRF_BASE + PERI_SGRF_FIREWALL_CON4);
	writel(0x000f0000, PERI_SGRF_BASE + PERI_SGRF_SOC_CON3);
	writel(0xffff0000, CORE_SGRF_BASE + CORE_SGRF_FIREWALL_CON0);
	writel(0xffff0000, CORE_SGRF_BASE + CORE_SGRF_FIREWALL_CON1);
	writel(0xffff0000, CORE_SGRF_BASE + CORE_SGRF_FIREWALL_CON2);
	writel(0xffff0000, CORE_SGRF_BASE + CORE_SGRF_FIREWALL_CON3);
	writel(0xffff0000, CORE_SGRF_BASE + CORE_SGRF_FIREWALL_CON4);
	writel(0x00030002, CORE_SGRF_BASE + CORE_SGRF_CPU_CTRL_CON);
	writel(0x20000000, PMU_SGRF_BASE);

	/* Set the emmc and fspi to access secure area */
	writel(0x00000000, FW_DDR_BASE + FW_DDR_MST3_REG);
	writel(0xff00ffff, FW_SHRM_BASE + FW_SHRM_MST1_REG);

	/* Set fspi clk 6mA */
	if ((readl(GPIO4_IOC_BASE + GPIO4B_IOMUX_SEL_L) & 0x70) == 0x20)
		writel(0x3f000700, GPIO4_IOC_BASE + GPIO4_IOC_GPIO4B_DS0);

	/*
	 * Set the USB2 PHY in suspend mode and turn off the
	 * USB2 PHY FS/LS differential receiver to save power:
	 * VCC1V8_USB : reduce 3.8 mA
	 * VDD_0V9 : reduce 4.4 mA
	 */
	writel(0x01ff01d1, PERI_GRF_BASE + PERI_GRF_USBPHY_CON0);
	writel(0x00000000, USBPHY_APB_BASE + USBPHY_FSLS_DIFF_RECEIVER);

	/* release the wdt */
	writel(0x2000200, PERI_GRF_BASE + PERI_GRF_PERI_CON1);
	writel(0x400040, CRU_BASE + CRU_GLB_RST_CON);

	/*
	 * When venc/npu use pvtpll, reboot will fail, because
	 * pvtpll is reset before venc/npu reset, so venc/npu
	 * is not completely reset, system will block when access
	 * NoC in SPL.
	 * Enable pvtpll can make venc/npu reset go on, wait
	 * until venc/npu is reset completely.
	 */
	writel(0xffff0018, CRU_BASE + CRU_PVTPLL0_CON1_L);
	writel(0x00030003, CRU_BASE + CRU_PVTPLL0_CON0_L);
	writel(0xffff0018, CRU_BASE + CRU_PVTPLL1_CON1_L);
	writel(0x00030003, CRU_BASE + CRU_PVTPLL1_CON0_L);
	udelay(2);

	if (readl(PMU_BASE + PMU_BIU_IDLE_ST)) {
		printascii("BAD PMU_BIU_IDLE_ST: ");
		printhex8(readl(PMU_BASE + PMU_BIU_IDLE_ST));
	}

	/*
	 * Limits npu max transport packets to 4 for route to scheduler,
	 * give much more chance for other controllers to access memory.
	 * such as VENC.
	 */
	writel(0x4, SHAPING_NPU_BASE + SHAPING_NBPKTMAX);

	/* Improve VENC QOS PRIORITY */
	writel(0x303, QOS_VENC_BASE + QOS_PRIORITY);

#ifdef CONFIG_ROCKCHIP_IMAGE_TINY
	/* Pinctrl is disabled, set sdmmc0 iomux here */
	writel(0xfff01110, GPIO3_IOC_BASE + GPIO3A_IOMUX_SEL_L);
	writel(0xffff1111, GPIO3_IOC_BASE + GPIO3A_IOMUX_SEL_H);
#endif

#if defined(CONFIG_ROCKCHIP_SFC_IOMUX)
	/* fspi iomux */
	writel(0x0f000700, GPIO4_IOC_BASE + 0x0030);
	writel(0xff002200, GPIO4_IOC_BASE + GPIO4A_IOMUX_SEL_L);
	writel(0x0f0f0202, GPIO4_IOC_BASE + GPIO4A_IOMUX_SEL_H);
	writel(0x00ff0022, GPIO4_IOC_BASE + GPIO4B_IOMUX_SEL_L);
#elif defined(CONFIG_ROCKCHIP_EMMC_IOMUX)
	/* emmc iomux */
	writel(0xffff1111, GPIO4_IOC_BASE + GPIO4A_IOMUX_SEL_L);
	writel(0xffff1111, GPIO4_IOC_BASE + GPIO4A_IOMUX_SEL_H);
	writel(0x00ff0011, GPIO4_IOC_BASE + GPIO4B_IOMUX_SEL_L);
#endif

#endif
	return 0;
}

#ifdef CONFIG_SPL_BUILD
int spl_fit_standalone_release(char *id, uintptr_t entry_point)
{
	/* set the mcu uncache area, usually set the devices address */
	writel(0xff000, CORE_GRF_BASE + CORE_GRF_CACHE_PERI_ADDR_START);
	writel(0xffc00, CORE_GRF_BASE + CORE_GRF_CACHE_PERI_ADDR_END);
	/* Reset the hp mcu */
	writel(0x1e001e, CORECRU_BASE + CORECRU_CORESOFTRST_CON01);
	/* set the mcu addr */
	writel(entry_point, CORE_SGRF_BASE + CORE_SGRF_HPMCU_BOOT_ADDR);
	/* release the mcu */
	writel(0x1e0000, CORECRU_BASE + CORECRU_CORESOFTRST_CON01);

	return 0;
}

void rk_meta_process(void)
{
	writel(0x00080008, CORE_GRF_BASE + CORE_GRF_MCU_CACHE_MISC);
}
#endif

#ifdef CONFIG_ROCKCHIP_IMAGE_TINY
int rk_board_scan_bootdev(void)
{
	char *devtype, *devnum;

	if (!run_command("blk dev mmc 1", 0) &&
	    !run_command("rkimgtest mmc 1", 0)) {
		devtype = "mmc";
		devnum = "1";
	} else {
		run_command("blk dev mtd 2", 0);
		devtype = "mtd";
		devnum = "2";
	}
	env_set("devtype", devtype);
	env_set("devnum", devnum);

	return 0;
}
#endif

int rk_board_late_init(void)
{
#if defined(CONFIG_CMD_SCRIPT_UPDATE)
	struct blk_desc *desc;

	desc = rockchip_get_bootdev();
	if (desc && desc->if_type == IF_TYPE_MMC && desc->devnum == 1)
		run_command("sd_update", 0);
#endif
	return 0;
}

