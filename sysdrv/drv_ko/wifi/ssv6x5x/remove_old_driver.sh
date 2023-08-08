#/bin/bash

KVERSION=$(uname -r)
rm -fr /lib/modules/$KVERSION/kernel/drivers/net/wireless/ssv/
rm -fr /lib/modules/$KVERSION/kernel/drivers/net/wireless/ssv6200/
