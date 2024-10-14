#!/bin/sh

help()
{
	echo "platform:"
	echo "	SIGMASTAR	:313E(3,18,30),325(4.9.84),335(4.9.84),377(5.10.61)"
	echo "	ANYKA		:AV100(4.4),37E(4.4)"
	echo "	FULLHAN		:FH8856(4.9.129)"
	echo "	XIONMAI		:XM530(3.10.103)"
	echo "	ALLWINNER	:A31(3.3.0)"
	echo "	ANBA		:anba(5.4)"
	echo "	ITOP		:4412(5.3.18)"
	echo "compiler demo: If empty, compile all by default"
	echo "	ble_demo : Compile only ble_demo,Be used for BLE set Master&Slave "
	echo "	ble_coex : Compile only ble_coex ,For WIFI BLE coexistence"
	echo "such as test on SIGMASTAR 313E platform"
	echo "If you want to compile the whole thing please run : "
	echo -e "	build_driver.sh 313E\n"
	echo "If you only compile ble_demo:"
	echo -e "	build_driver.sh 313E ble_demo\n" 
	echo "if other platform ,please run:"
	echo "	build_driver.sh other KERDIR=kernel_path CROSS_COMPILE=Compiler_path/compiler sys=soc_run_sys arch=system_architecture platform=PLATFORM_OTHER"
}

run_compile()
{

	if [ "$1" = "313E" ] || [ "$1" = "313e" ];then
		KDIR=/usr/lchome/yuzhihuang/Mstar/IPC_I3/linux3.18_i3/ 
		CC=/usr/lchome/yuzhihuang/Mstar/IPC_I3/arm-linux-gnueabihf-4.8.3-201404/bin/arm-linux-gnueabihf- 
		architecture=arm 
		system=Linux 
		platform_soc=PLATFORM_313E
	elif [ "$1" = "325" ];then
		export LD_LIBRARY_PATH='/usr/lchome/yuzhihuang/Mstar/325/lib/libiconv/lib'
		KDIR=/usr/lchome/yuzhihuang/Mstar/325/kernel 
		CC=/usr/lchome/yuzhihuang/Mstar/325/arm-buildroot-linux-uclibcgnueabihf-4.9.4/bin/arm-buildroot-linux-uclibcgnueabihf- 
		architecture=arm 
		system=Linux 
		platform_soc=PLATFORM_325
	elif [ "$1" = "335" ];then
                export LD_LIBRARY_PATH='/usr/lchome/yuzhihuang/Mstar/325/lib/libiconv/lib'
                KDIR=/usr/lchome/yuzhihuang/Mstar/335/kernel
                CC=/usr/lchome/yuzhihuang/Mstar/335/arm-buildroot-linux-uclibcgnueabihf-4.9.4/bin/arm-buildroot-linux-uclibcgnueabihf-
                architecture=arm
                system=Linux
                platform_soc=PLATFORM_335
	elif [ "$1" = "I7" ] || [ "$1" = "377" ];then
		KDIR=/usr/lchome/yuzhihuang/Mstar/I7/377/kernel 
		CC=/usr/lchome/yuzhihuang/Mstar/I7/377/arm-sigmastar-linux-uclibcgnueabihf-9.1.0/bin/arm-sigmastar-linux-uclibcgnueabihf-
		architecture=arm
		system=Linux 
		platform_soc=PLATFORM_I7
	elif [ "$1" = "FH8856" ] || [ "$1" = "fh8856" ];then
		KDIR=/usr/lchome/yuzhihuang/FullH/8856/FH8856_IPC_LINUX49_V1.0.2_20200708/board_support/kernel/linux-4.9 
		CC=/usr/lchome/yuzhihuang/FullH/8856/FH8856_IPC_LINUX49_V1.0.2_20200708/board_support/toolchain/arm-fullhanv3-linux-uclibcgnueabi-b2/bin/arm-fullhanv3-linux-uclibcgnueabi- 
		architecture=arm 
		system=Linux 
		platform_soc=PLATFORM_FH8856
	elif [ "$1" = "XM530" ] || [ "$1" = "xm530" ];then
		KDIR=/usr/lchome/yuzhihuang/xm/single_IPC_small_time/111_cfg80211 
		CC=/usr/lchome/yuzhihuang/xm/single_IPC_small_time/opt/xm_toolchain/arm-xm-linux/usr/bin/arm-xm-linux-uclibcgnueabi- 
		architecture=arm 
		system=Linux 
		platform_soc=PLATFORM_XM530
	elif [ "$1" = "AV100" ] || [ "$1" = "av100" ];then
		export ATBM_WIFI__EXT_CCFLAGS = -DATBM_WIFI_PLATFORM=24
		Compile_parameter=
		KDIR=/usr/lchome/yuzhihuang/ankai/Linux/anykaAV100/AnyCloud39AV100_AK3918AV100_SDK_V1.02/AK3918AV100_SDK_V1.02/os/bd_cfg80211  
		CC=/usr/lchome/yuzhihuang/ankai/Linux/anykaAV100/AnyCloud39AV100_AK3918AV100_SDK_V1.02/AK3918AV100_SDK_V1.02/tools/arm-anycloud-linux-uclibcgnueabi/bin/arm-anycloud-linux-uclibcgnueabi- 
		architecture=arm 
		system=Linux 
		platform_soc=PLATFORM_AV100
	elif [ "$1" = "37E" ] || [ "$1" = "37e" ];then
		KDIR=/usr/lchome/yuzhihuang/ankai/Linux/anyka37E/AK37E_SDK_V1.01_1/os/bd_cfg80211 
		CC=/usr/lchome/yuzhihuang/ankai/Linux/anyka37E/AK37E_SDK_V1.01_1/tools/arm-anykav500-linux-uclibcgnueabi/bin/arm-anykav500-linux-uclibcgnueabi- 
		architecture=arm 
		system=Linux 
		platform_soc=PLATFORM_37E
	elif [ "$1" = "A31" ] || [ "$1" = "a31" ];then
		KDIR=/usr/lchome/yuzhihuang/allWinner/BJ_A31/linux-3.3/  
		CC=/usr/lchome/yuzhihuang/allWinner/BJ_A31/tools/toolschain/gcc-linaro/bin/arm-linux-gnueabi- 
		architecture=arm 
		system=Linux 
		platform_soc=PLATFORM_A31
	elif [ "$1" = "anba" ];then
		KDIR=/usr/lchome/lishicheng/ambarella/cv2x_linux_pure_sdk_3.0/ambarella/out/cv25_hazelnut/kernel/linux-5.4_cv25
                CC=/usr/lchome/lishicheng/ambarella/linaro-aarch64-2020.09-gcc10.2-linux5.4/bin/aarch64-linux-gnu-
                architecture=arm64
                system=Linux
                platform_soc=PLATFORM_ANBA

	elif [ "$1" = "4412" ];then
		KDIR=/usr/lchome/jiashunan/4412/4412_SCP_5.3.18/kernel
		CC=/usr/lchome/jiashunan/4412/gcc-arm-9.2-2019.12-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-
		architecture=arm
        system=Linux
        platform_soc=PLATFORM_ITOP4412
	
	else
		echo "$1"
		help
		return -1;
	fi
	echo -e "\n\n$1 $2\n\n"
	if [ "$2" == "ble_demo" ];then
		make -f Makefile_SZ KERDIR=$KDIR  CROSS_COMPILE=$CC  arch=$architecture  sys=$system  platform=$platform_soc ble_demo
		
		
	elif [ "$2" == "ble_coex" ];then
		make -f Makefile_SZ KERDIR=$KDIR  CROSS_COMPILE=$CC  arch=$architecture  sys=$system  platform=$platform_soc ble_coex
	
	#elif [ "$2" -eq "all" ];then
	else
		
		make -f Makefile_SZ KERDIR=$KDIR  CROSS_COMPILE=$CC  arch=$architecture  sys=$system  platform=$platform_soc;
		make -f Makefile_SZ KERDIR=$KDIR  CROSS_COMPILE=$CC  arch=$architecture  sys=$system  platform=$platform_soc strip;
		echo -e "\n\nbuild ble demo\n\n"
		make -f Makefile_SZ KERDIR=$KDIR  CROSS_COMPILE=$CC  arch=$architecture  sys=$system  platform=$platform_soc ble_demo;
		make -f Makefile_SZ KERDIR=$KDIR  CROSS_COMPILE=$CC  arch=$architecture  sys=$system  platform=$platform_soc ble_coex;
	
	fi
	return 0;
}

main()
{
	if [ "$1" = "other" ];then
		echo "$2"
		echo "$3"
		echo "$4"
		echo "$5"
		echo "$6"
		make $2 $3 $4 $5 $6 
		make $2 $3 $4 $5 $6 strip
		make $2 $3 $4 $5 $6 ble_demo
		make $2 $3 $4 $5 $6 ble_coex
	elif [ "$1" = "help" ];then
		help;
	elif [ "$1" = "" ];then
		help;
	else
		echo -e " \n\n $1 $2 \n\n"
		if [ "$2" ];then
			run_compile $1 $2
		else
			run_compile $1
		fi
	fi


	return 0;
}


main $1 $2 $3 $4 $5 $6
