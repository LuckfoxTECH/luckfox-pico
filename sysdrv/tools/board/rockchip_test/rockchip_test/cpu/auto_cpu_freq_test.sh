#!/bin/sh
### file: stress_test_dvfs.sh
### author: xxx@rock-chips.com
### function: cpu dvfs stress test
### date: 20180409

echo "**********************stress dvfs test****************************"

delay_time=$1
echo "delay_time: $delay_time"

echo userspace >  /sys/devices/system/cpu/cpufreq/policy0/scaling_governor
governor=`cat /sys/devices/system/cpu/cpufreq/policy0/scaling_governor`

echo "*********scaling_governor: $governor!"

freq_list=`cat /sys/devices/system/cpu/cpufreq/policy0/scaling_available_frequencies`

echo "freq_list: $freq_list"

while true
  do
    for i in $freq_list;
    do
      echo "will set $i!"
      echo $i >  /sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed
      cur_freq=`cat /sys/devices/system/cpu/cpufreq/policy0/cpuinfo_cur_freq`
      echo "cur_freq: $cur_freq"
      sleep $delay_time
    done
done



