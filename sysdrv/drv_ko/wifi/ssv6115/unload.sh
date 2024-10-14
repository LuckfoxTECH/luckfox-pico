#/bin/bash

echo "=================================================="
echo "1.Unload Module"
echo "=================================================="

rmmod ssv6x5x
rmmod mmc_core
rmmod mmc_block

rmmod sdhci_pci
rmmod sdhci
