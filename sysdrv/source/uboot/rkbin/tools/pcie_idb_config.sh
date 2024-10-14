#!/bin/bash

soc=$1
gen=$2
lanes=$3
baundary=$4
input=$9

magic0=70
magic1=63
magic2=69
magic3=65
code0=00
code1=43
code2=00
code3=00
# case: vid=0x1d87 did=0x356a
vid_l=$5
vid_h=$6
did_l=$7
did_h=$8

function usage()
{
	echo "./pcie_idb_config.sh <soc> <gen> <lanes> <uart_baundary> <vid_l> <vid_h> <did_l> <did_h> <input>"
	echo "like following:"
	echo "./pcie_idb_config.sh RK3588 3 4 1500000 87 1d 6a 35 bin/rk35/rk3588_pcie_v*.bin"
	echo "./pcie_idb_config.sh RK3588 3 2 1500000 87 1d 6a 35 bin/rk35/rk3588_pcie_v*.bin"
	echo "./pcie_idb_config.sh RK3588 3 4 115200  87 1d 6a 35 bin/rk35/rk3568_pcie_v*.bin"
	echo "./pcie_idb_config.sh RK3568 3 2 1500000 87 1d 6a 35 bin/rk35/rk3588_pcie_v*.bin"
}

#define PCIE_IDB_CFG_PHY_MODE_SHIFT       0
#define PCIE_IDB_CFG_PHY_MODE_MASK        3
#define PCIE_IDB_CFG_PHY_MODE_AGGREGATION PHY_MODE_PCIE_AGGREGATION /**< 4 PCIE3x4 */
#define PCIE_IDB_CFG_PHY_MODE_NANBNB      PHY_MODE_PCIE_NANBNB      /**< 0 P1:PCIE3x2  +  P0:PCIE3x2 */
#define PCIE_IDB_CFG_PHY_MODE_NANBBI      PHY_MODE_PCIE_NANBBI      /**< 1 P1:PCIE3x2  +  P0:PCIE3x1*2 */
#define PCIE_IDB_CFG_PHY_MODE_NABINB      PHY_MODE_PCIE_NABINB      /**< 2 P1:PCIE3x1*2 + P0:PCIE3x2 */
#define PCIE_IDB_CFG_PHY_MODE_NABIBI      PHY_MODE_PCIE_NABIBI      /**< 3 P1:PCIE3x1*2 + P0:PCIE3x1*2 */
#define PCIE_IDB_CFG_GEN_SHIFT            8
#define PCIE_IDB_CFG_GEN_MASK             3
#define PCIE_IDB_CFG_LANE_SHIFT           12
#define PCIE_IDB_CFG_LANE_MASK            3
#define PCIE_IDB_CFG_UART_ID_SHIFT        16
#define PCIE_IDB_CFG_UART_ID_MASK         3
#define PCIE_IDB_CFG_UART_MUX_SHIFT       20
#define PCIE_IDB_CFG_UART_MUX_MASK        3
#define PCIE_IDB_CFG_UART_RATE_SHIFT      24
#define PCIE_IDB_CFG_UART_RATE_MASK       2
#define PCIE_IDB_CFG_UART_RATE_DEFAULT    0
#define PCIE_IDB_CFG_UART_RATE_15000000   1
#define PCIE_IDB_CFG_UART_RATE_1152000    2

if [[ $soc != RK3588 && $soc != RK3568 ]]; then
	echo "input param soc=$soc invalid, support RK3588/RK3568"
	usage
	exit
fi

if [[ $soc == RK3588 ]]; then
	if [[ $gen != 3 ]]; then
		echo "input param gen=$gen invalid, support 3"
		usage
		exit
	fi

	if [[ $lanes != 4 && $lanes != 2 ]]; then
		echo "input param lanes=$lanes invalid, support 4/2"
		usage
		exit
	fi

	if [[ $baundary != 1500000 && $baundary != 115200 ]]; then
		echo "input param baundary=$baundary invalid, support 1500000/115200"
		usage
		exit
	fi
fi

if [[ $soc == RK3568 ]]; then
	if [[ $gen != 3 ]]; then
		echo "input param gen=$gen invalid, support 3"
		usage
		exit
	fi

	if [[ $lanes != 2 ]]; then
		echo "input param lanes=$lanes invalid, support 2"
		usage
		exit
	fi

	if [[ $baundary != 1500000 ]]; then
		echo "input param baundary=$baundary invalid, support 1500000"
		usage
		exit
	fi
fi

if [ -f "$input" ]; then
	echo "input param file=$input"
else
	echo "input param file=$input invalid"
	exit
fi 

if [[ $lanes == 4 ]]; then
	if [[ $soc == RK3588 ]]; then
		code0=04
	fi
elif [[ $lanes == 2 ]]; then
	code1=23
fi

if [[ $baundary == 1500000 ]]; then
	code3=01
fi

if [[ $baundary == 115200 ]]; then
	code3=02

fi

if [[ $baundary == 115200 ]]; then
	code3=02

fi

echo "code0=$code0"
echo "code1=$code1"
echo "code2=$code2"
echo "code3=$code3"
echo "vid=0x${vid_h}${vid_l} did=0x${did_h}${did_l}"

echo -e -n "\x${magic0}\x${magic1}\x${magic2}\x${magic3}" > .pcie_idb_cfg.bak
echo -e -n "\x${code0}\x${code1}\x${code2}\x${code3}\x${vid_l}\x${vid_h}\x${did_l}\x${did_h}" >> .pcie_idb_cfg.bak
dd if=$input of=.temp bs=1 count=4 > /dev/null
dd if=.pcie_idb_cfg.bak of=$input bs=1 seek=4 conv=notrunc > /dev/null

