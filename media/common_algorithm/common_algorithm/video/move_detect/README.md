# Rockchip MoveDetect 算法

## compile

mkdir -p build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/video/rv1109/buildroot/output/rockchip_puma_toolchain/host/share/buildroot/toolchainfile.cmake
make

优化选项： -O3 -mfpu=neon -mfloat-abi=hard

## 资源占用情况(640x360)

### 测试环境

* RV1808 EVB

* cpu:  关闭ARM核保留一个核，定频 1Ghz

   echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

   echo 1008000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed

   echo 0 > /sys/devices/system/cpu/cpu1/online

* ddr : LPDDR3 933MHz

### sigle ref mode

* 内存占用情况： 2 frame cached, 450K
* 计算耗时

  * /test_md 1 1    //单例
    new thread
    Test: is_single_ref 1, raw 1920 x 1080, ds 640 x 360
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
* /test_md 1 2   //2个实例
    new thread
    new thread
    Test: is_single_ref 1, raw 1920 x 1080, ds 640 x 360
    Test: is_single_ref 1, raw 1920 x 1080, ds 640 x 360
    elapse 3 ms
    elapse 12 ms
    elapse 3 ms
    elapse 10 ms
    elapse 11 ms
    elapse 11 ms
* 稳定性测试
  	* 多实例循环压力测试通过：没有内存泄露，可以稳定长时运行

### multi ref mode:

* 内存占用情况：5 frame cached, 1125k
* 计算耗时：
  * ./test_md 0 1   //单例
    new thread
    Test: is_single_ref 0, raw 1920 x 1080, ds 640 x 360
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
    elapse 3 ms
    elapse 4 ms
    elapse 3 ms
    elapse 4 ms
  *  ./test_md 0 2  //2个实例
    new thread
    new thread
    Test: is_single_ref 0, raw 1920 x 1080, ds 640 x 360
    Test: is_single_ref 0, raw 1920 x 1080, ds 640 x 360
    elapse 10 ms
    elapse 10 ms
    elapse 3 ms
    elapse 3 ms
    elapse 10 ms
    elapse 10 ms
    elapse 3 ms
    elapse 10 ms
    elapse 3 ms
    elapse 10 ms
    elapse 10 ms

* 稳定性测试
  * 多实例循环压力测试通过：没有内存泄露，可以稳定长时运行
