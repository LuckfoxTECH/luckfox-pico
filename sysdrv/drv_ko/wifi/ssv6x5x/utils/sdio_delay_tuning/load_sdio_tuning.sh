#/bin/bash

KVERSION="`uname -r`"
SSV_MOD_PATH=/lib/modules/$KVERSION/kernel/drivers/net/wireless/ssv6200/

echo "=================================================="
echo "1.Unload SSV Module"
echo "=================================================="
../../unload.sh
rm -rf $SSV_MOD_PATH/*

echo "=================================================="
echo "2.Load MMC Module"
echo "=================================================="

modprobe sdhci-pci
modprobe sdhci

modprobe mmc_core sdiomaxclock=25000000
modprobe mmc_block

echo "=================================================="
echo "3.Load SDIO Tuning Module"
echo "=================================================="
rmmod ssv6200_sdio_delay_tuning.ko
sleep 1
insmod ./ssv6200_sdio_delay_tuning.ko
