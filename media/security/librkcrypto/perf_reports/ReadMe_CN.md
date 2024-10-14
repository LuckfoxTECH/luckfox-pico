## 吞吐率测试方法

### CPU定频

-    cpu用户空间设置  ：`echo userspace > /sys/devices/system/cpu/cpufreq/policy0/scaling_governor`
-    查看是否设置成功 ：`cat /sys/devices/system/cpu/cpufreq/policy0/scaling_governor`
-    cpu频率列表      ：`cat /sys/devices/system/cpu/cpufreq/policy0/scaling_available_frequencies`
-    cpu频率设置      ：`echo 408000 > /sys/devices/system/cpu/cpufreq/policy0/scaling_setspeed`
-    cpu查看当前频率  ：`cat /sys/devices/system/cpu/cpufreq/policy0/cpuinfo_cur_freq`

### DDR定频（某些平台不可设置）

- ddr用户空间设置 ：`echo userspace > /sys/class/devfreq/dmc/governor`
- 查看是否关成功  ：`cat /sys/class/devfreq/dmc/governor`
- ddr频率列表     ：`cat /sys/class/devfreq/dmc/available_frequencies`
- ddr当前频率设置 ：`echo 528000000 > /sys/class/devfreq/dmc/userspace/set_freq`
- ddr查看当前频率 ：`cat /sys/class/devfreq/dmc/cur_freq`

### 执行`librkcrypto_test -t`即可执行吞吐率测试。

### 测试原理：不断对1M Byte数据执行加密或解密操作，计算1秒内处理的数据量。