# RK SPI Standalone Bin

This is the readme for the Das U-Boot standalone program rkspi


How To Use
------------------------
### Compile

1.Define the standalone load address in includes/configs/rkxxxxx_common.h

```shell
#define CONFIG_STANDALONE_LOAD_ADDR    0x40000000
```

2.Enable rkspi in defconfig

```
CONFIG_ROCKCHIP_SPI=y
```

### Setting SPI hardware

1.Setting the iomux and spiclk through:

- u-boot shell command
- define it in rkspi.c spi_hw_init

Note:

- spiclk is the clock for spi controller, output to IO after internal frequency division of the controller.

### Load And Executable

1. load the bin by serial or tftp, take tftp as example:

```shell
setenv ipaddr 172.16.12.157
setenv serverip 172.16.12.167
tftp 0x40000000 rkspi.bin		# 0x40000000 is define by CONFIG_STANDALONE_LOAD_ADDR
```

2. execute it

```shell
go 0x40000000		# 0x40000000 is define by CONFIG_STANDALONE_LOAD_ADDR
```

## Abort Codes

### Introduction

```c
int rockchip_spi_probe(u8 bus, uintptr_t base_addr, u32 rsd, u32 clock_div, u32 mode);
```

- bus: spi bus
- base_addr: spi register base address
- rsd: read sample clock shift with spiclk which is controller working rate
- clock_div: internal frequency division of the controller
- mode: spi mode, support:

```c
#define SPI_CPHA	BIT(0)			/* clock phase */
#define SPI_CPOL	BIT(1)			/* clock polarity */
#define SPI_MODE_0	(0 | 0)			/* (original MicroWire) */
#define SPI_MODE_1	(0 | SPI_CPHA)
#define SPI_MODE_2	(SPI_CPOL | 0)
#define SPI_MODE_3	(SPI_CPOL | SPI_CPHA)
```



```c
int rockchip_spi_claim_bus(u8 bus);
```

- bus: spi bus



```c
void rockchip_spi_release_bus(u8 bus);
```

- bus: spi bus



```c
int rockchip_spi_xfer(u8 bus, u8 cs, unsigned int bitlen, const void *dout, void *din, unsigned long flags);
```

- bus: spi bus
- cs: spi cs
- bitlen: the transfer length in bits
- dout: write buffer (if exits)
- din: read buffer (if exits), if the dout and din both defined, spi work in duplex mode
- flags: operation chip select, support:

```c
#define SPI_XFER_BEGIN		BIT(0)	/* Assert CS before transfer */
#define SPI_XFER_END		BIT(1)	/* Deassert CS after transfer */
#define SPI_XFER_ONCE		(SPI_XFER_BEGIN | SPI_XFER_END)
```



```c
int rockchip_spi_write_then_read(u8 bus, u8 cs,
				 const u8 *opcode, size_t n_opcode,
				 const u8 *txbuf, u8 *rxbuf, size_t n_buf);
```

- bus: spi bus
- cs: spi cs
- opcode: command code
- n_opcode: the numbers of command code in bytes
- txbuf: write buffer (if exits)
- rxbuf: read buffer (if exits), if the dout and din both defined, spi work in duplex mode
- n_buf: the transfer length in bytes

### Demo

Is right in the main function of rkspi.

