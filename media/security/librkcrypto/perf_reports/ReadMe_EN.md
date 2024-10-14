## Throughput test method

### Set the CPU to work at the highest frequency

-    Set to userspace mode          : `echo userspace > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor`
-    Check mode set succeed         : `cat /sys/devices/system/cpu/cpufreq/policy0/scaling_governor`
-    List all available frequencies : `cat /sys/devices/system/cpu/cpufreq/policy0/scaling_available_frequencies`
-    Set highest frequency          : `echo 408000 > /sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed`
-    Check frequency                : `cat /sys/devices/system/cpu/cpufreq/policy0/cpuinfo_cur_freq`

### Set the CPU to work at the highest frequency(Some platforms cannot be set).

-    Set to userspace mode          : `echo userspace > /sys/class/devfreq/dmc/governor`
-    Check mode set succeed         : `cat /sys/class/devfreq/dmc/governor`
-    List all available frequencies : `cat /sys/class/devfreq/dmc/available_frequencies`
-    Set highest frequency          : `echo 528000000 > /sys/class/devfreq/dmc/userspace/set_freq`
-    Check frequency                : `cat /sys/class/devfreq/dmc/cur_freq`

### Run `librkcrypto_test -t` to perform the throughput test.

### Test principle: Perform encryption or decryption operations on 1M Byte data continuously and calculate the amount of data processed in 1 second.