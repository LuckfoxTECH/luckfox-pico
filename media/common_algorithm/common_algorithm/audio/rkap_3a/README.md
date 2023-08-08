# Rockchip AEC_ANR_AGC算法

## compile

mkdir -p build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=~/video/rv1109/buildroot/output/rockchip_puma_toolchain/host/share/buildroot/toolchainfile.cmake
make

优化选项： -O3 -mfpu=neon -mfloat-abi=hard

## 资源占用情况

### 测试环境

* RV1808 EVB

* cpu:  关闭ARM核保留一个核，定频 1Ghz

   echo userspace > /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor

   echo 1008000 > /sys/devices/system/cpu/cpu0/cpufreq/scaling_setspeed

   echo 0 > /sys/devices/system/cpu/cpu1/online

* ddr : LPDDR3 933MHz

### Test

* 内存占用情况：RSS 1.6M

* 计算耗时 cpu 37%

  * ./test_aec_anr_agc csq_0_1mic.wav csq_0_1mic-out.wav
    filename=csq_0_1mic.wav format=1 num_channels=2 sample_rate=16000 sample_bits=16 num_samples=128001
    mic_channel = 1, ref channel = 1
    init ch 0
    init ch 0 succeed
    init ok
    alloc ok
    elapse 6 ms    //PROCESS TX  16ms数据

    elapse 0 ms   //PROCESS RX
    elapse 6 ms
    elapse 0 ms
    elapse 6 ms
    elapse 0 ms
    elapse 6 ms
    elapse 0 ms
    elapse 6 ms
    elapse 0 ms

* 稳定性测试
  	* 多实例循环压力测试通过：没有内存泄露，可以稳定长时运行

	
	# cat /proc/673/status
	Name:   test_aec_anr_ag
	State:  R (running)
	Tgid:   673
	Ngid:   0
	Pid:    673
	PPid:   609
	TracerPid:      0
	Uid:    0       0       0       0
	Gid:    0       0       0       0
	FDSize: 64
	Groups:
	VmPeak:     2444 kB
	VmSize:     2444 kB
	VmLck:         0 kB
	VmPin:         0 kB
	VmHWM:      1672 kB
	VmRSS:      1672 kB
	VmData:      292 kB
	VmStk:       132 kB
	VmExe:         8 kB
	VmLib:      1784 kB
	VmPTE:        20 kB
	VmPMD:         8 kB
	VmSwap:        0 kB
	Threads:        1
	SigQ:   0/7992
	SigPnd: 0000000000000000
	ShdPnd: 0000000000000000
	SigBlk: 0000000000000000
	SigIgn: 0000000000000000
	SigCgt: 0000000000000000
	CapInh: 0000000000000000
	CapPrm: 0000003fffffffff
	CapEff: 0000003fffffffff
	CapBnd: 0000003fffffffff
	CapAmb: 0000000000000000
	Speculation_Store_Bypass:       unknown
	Cpus_allowed:   3
	Cpus_allowed_list:      0-1
	Mems_allowed:   1
	Mems_allowed_list:      0
	voluntary_ctxt_switches:        1
	nonvoluntary_ctxt_switches:     38
