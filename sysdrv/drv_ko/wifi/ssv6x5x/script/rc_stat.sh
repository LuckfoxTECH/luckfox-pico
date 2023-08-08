#!/bin/sh

phy_dirs="/sys/class/ieee80211/*"
net_dirs="/sys/class/net/*"
SSV_IF="SSV|RSV"

ssv_phy=""
ssv_wlan=""

## find ssv phy 
for phy_dir in $phy_dirs; do
	drv_name=`ls ${phy_dir}/device/driver | grep -E ${SSV_IF}`
    if [ ${drv_name} ]; then
    	ssv_phy=`basename $phy_dir`;
    	break;
    fi
done

if [ -z "$ssv_phy" ]; then
    echo SSV PHY device not found.;
    exit 1;
fi

## find ssv wlan 
for net_dir in $net_dirs; do
	drv_dir="${net_dir}/phy80211/device/driver"
	if [ ! -e ${drv_dir} ] ; then
		continue;
	fi
	drv_name=`ls ${drv_dir} | grep -E ${SSV_IF}`
    if [ ${drv_name} ]; then
    	ssv_wlan=`basename ${net_dir}`;
    	break;
    fi
done

if [ -z "$ssv_wlan" ]; then
    echo SSV wlan device not found.;
    exit 1;
fi

rc_stat=/sys/kernel/debug/ieee80211/${ssv_phy}/netdev:${ssv_wlan}/stations/*/rc_stats
if [ -f ${rc_stat} ] ; then
    cat ${rc_stat}
fi
