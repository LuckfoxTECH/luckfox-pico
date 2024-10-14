#/bin/bash
#ignore ctrl+c
trap '' INT

echo "=================================================="
echo "1.Unload Module"
echo "=================================================="
./unload.sh

echo "=================================================="
echo "1.Load MMC Module"
echo "=================================================="

modprobe sdhci-pci
modprobe sdhci

modprobe mmc_core sdiomaxclock=25000000
modprobe mmc_block

echo "=================================================="
echo "2.Load SSV6200 Driver"
echo "=================================================="
insmod ssv6x5x.ko stacfgpath=ssv6x5x-wifi.cfg

