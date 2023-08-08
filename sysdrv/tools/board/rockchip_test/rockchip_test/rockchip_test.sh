#!/bin/sh
### file: rockchip_test.sh
### author: yhx@rock-chips.com wxt@rock-chips.com
### function: ddr cpu gpio flash bt audio recovery s2r sdio/pcie(wifi)
###           ethernet reboot ddrfreq npu camera video
### date: 20190107

cmd=`realpath $0`
CURRENT_DIR=`dirname $cmd`

export PATH=$CURRENT_DIR:$PATH

moudle_env()
{
	export  MODULE_CHOICE
}

module_choice()
{
	echo "*****************************************************"
	echo "***                                               ***"
	echo "***        ********************                   ***"
	echo "***       *ROCKCHIPS TEST TOOLS*                  ***"
	echo "***        *                  *                   ***"
	echo "***        ********************                   ***"
	echo "***                                               ***"
	echo "*****************************************************"


	echo "*****************************************************"
	echo "ddr test :            1 (memtester & stressapptest)"
	echo "cpufreq test:         2 (cpufreq stresstest)"
	echo "flash stress test:    3"
	echo "auto reboot test:     4"
	echo "nand power lost test: 5"
	echo "*****************************************************"

	echo  "please input your test moudle: "
	read -t 30  MODULE_CHOICE
}

ddr_test()
{
	sh ${CURRENT_DIR}/ddr/ddr_test.sh
}

cpufreq_test()
{
	sh ${CURRENT_DIR}/cpu/cpufreq_test.sh
}

flash_stress_test()
{
	sh ${CURRENT_DIR}/flash_test/flash_stress_test.sh 5 20000&
}

auto_reboot_test()
{
	fcnt=/data/cfg/rockchip_test/reboot_cnt;
	if [ -e "$fcnt" ]; then
		rm -rf $fcnt;
	fi
	sh ${CURRENT_DIR}/auto_reboot/auto_reboot.sh
}

power_lost_test()
{
	fcnt=/data/config/rockchip_test/reboot_cnt;
	if [ -e "$fcnt" ]; then
		rm -rf $fcnt;
	fi
	sh ${CURRENT_DIR}/flash_test/power_lost_test.sh &
}

module_test()
{
	case ${MODULE_CHOICE} in
		1)
			ddr_test
			;;
		2)
			cpufreq_test
			;;
		3)
			flash_stress_test
			;;
		4)
			auto_reboot_test
			;;
		5)
			power_lost_test
			;;
	esac
}

module_choice
module_test
