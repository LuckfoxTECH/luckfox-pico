#!/bin/sh

usage()
{
  echo "Usage: cpu_freq_test.sh [test_second] [every_freq_stay_second]"
  echo "example: cpu_freq_test.sh  3600 30"
  echo "means cpu_freq_test.sh will run 1 hour and every cpu frequency stay 10s"
}

echo "test will run $1 seconds"
echo "every cpu frqeucny will stay $2 seconds"

# disalbe thermal
if [ -e /sys/class/thermal/thermal_zone0 ]; then
  echo user_space >/sys/class/thermal/thermal_zone0/policy
fi

if [ -e /sys/class/thermal/thermal_zone1 ]; then
  echo user_space > /sys/class/thermal/thermal_zone1/policy
fi

# caculate how many cpu core
cpu_cnt=`cat /proc/cpuinfo | grep processor | sort | uniq | wc -l`

# get memory free size
mem_avail_size=$(cat /proc/meminfo | grep MemAvailable | awk '{print $2}')
if [ $mem_avail_size -gt "131072" ]; then
	mem_test_size="128"
else
	mem_test_size=$((mem_avail_size/1024/4*4-4))
fi

stressapptest -s $1 --pause_delay 10 --pause_duration 1 -W --stop_on_errors -M $mem_test_size &

cd /sys/devices/system/cpu/cpu0/cpufreq
unset FREQS
FREQS=$(cat scaling_available_frequencies)
FREQS_NUM=$(echo $FREQS | awk '{print NF}')
RANDOM=$(date +%s)
time_cnt=0

while true; do
  if [ $time_cnt -ge $1 ]
  then
    echo "======TEST SUCCESSFUL, QUIT====="
    exit 0
  fi
  echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
  FREQ_NO=$(($RANDOM % $FREQS_NUM + 1))
  FREQ=$(echo $FREQS | awk -v val=$FREQ_NO '{print $val}')
  echo "Set ${FREQ}"
  echo ${FREQ} > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed
  cur_freq=`cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq`
  echo "Get ***$cur_freq***"
  sleep $2
  time_cnt=$(($time_cnt+$2))
done
