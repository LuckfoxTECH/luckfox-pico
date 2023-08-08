
idb_bootconfig is used for spi nor ONLY.

Step 1: config CONFIG_MTD_SPI_NOR_MISC=y for kernel's defconfig
Step 2: run ./idb_bootconfig /dev/mtdblock1 or ./idb_bootconfig
		/dev/mtdblock1 is the partition which named "idblock"
