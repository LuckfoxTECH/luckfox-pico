#!/bin/bash
# AP defines the AP the station is going to connect to.
AP=N66
# The FW_LOG is the firmware console log file. Used to keep track of firmware status of each run
FW_LOG=~ssv/Desktop/fw.log
# WIF is the wireless interface of the loaded driver 
WIF=wlan5
# RUN is the number of load/unload tests
RUN=20000

redo=0
total=0
failed=0
for i in {1..$RUN}; do
    echo "Unloading" | tee -a /var/log/kern.log
    ./unload.sh 2&>1 > /dev/null
    truncate -s 0 ~ssv/Desktop/fw.log
    echo "#$i run" | tee -a /var/log/kern.log
    ./sta_usb.sh 2&>1 > /dev/null
    sleep 1
    this_total=`cat $FW_LOG | sed -e 's/\r//g' | grep -iac loopback`
    this_redo=`cat $FW_LOG | sed -e 's/\r//g' | grep -iac redo`
    redo=$(( $redo + $this_redo ))
    total=$(( $total + $this_total ))
    [[ $this_redo -gt 0 ]] && failed=$(( $failed + 1 ))
    echo "  > $this_redo / $this_total"
    echo -n "  > Connecting to ${AP}"
    connected=0
    waits=0
    while [[ $connected -eq 0 && waits -lt 60 ]]; do
        nmcli con status id ${AP} 2&>1 > /dev/null && connected=1
        [ $connected -eq 0 ] && echo -n "." && sleep 1
        waits=$(( $waits + 1 ))
        [[ $(( $waits % 8 )) == 0 ]] && wpa_cli -i $WIF scan
    done
    if [[ $connected -eq 1 && $waits -lt 60 ]]; then
        echo "done ($failed/$redo/$total)" | tee -a /var/log/kern.log
        sleep 1
        echo ">> Disconnecting" | tee -a /var/log/kern.log
        wpa_cli -i $WIF disconnect
        nmcli con down id ${AP}
        sleep 1
        echo ">> IF down" | tee -a /var/log/kern.log
        ifconfig $WIF down
    else
        echo "failed"
    fi
    cp ~ssv/Desktop/fw.log fw.log/fw_$i.log
done
