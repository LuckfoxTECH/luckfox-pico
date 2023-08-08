#!/bin/sh
#############################################
#
#   Authors: @rock-chips.com
#
#############################################

version=0.1

get_cpu_usage() {
	#脚本功能描述：依据/proc/stat文件获取并计算CPU使用率
	#CPU时间计算公式：CPU_TIME=user+system+nice+idle+iowait+irq+softirq
	#CPU使用率计算公式：cpu_usage=[(user_2 +sys_2+nice_2) - (user_1 + sys_1+nice_1)]/(total_2 - total_1)*100
	#默认时间间隔
	TIME_INTERVAL=5
	time=$(date "+%s")
	LAST_CPU_INFO=$(cat /proc/stat | grep -w cpu | awk '{print $2,$3,$4,$5,$6,$7,$8}')
	LAST_SYS_IDLE=$(echo $LAST_CPU_INFO | awk '{print $4}')
	LAST_USER_BUSY=$(echo $LAST_CPU_INFO | awk '{print $1}')
	LAST_SYS_BUSY=$(echo $LAST_CPU_INFO | awk '{print $3}')
	LAST_TOTAL_CPU_T=$(echo $LAST_CPU_INFO | awk '{print $1+$2+$3+$4+$5+$6+$7}')
	LAST_CPU_USAGE=$(echo $LAST_CPU_INFO | awk '{print $1+$2+$3}')
	sleep ${TIME_INTERVAL}
	NEXT_CPU_INFO=$(cat /proc/stat | grep -w cpu | awk '{print $2,$3,$4,$5,$6,$7,$8}')
	NEXT_SYS_IDLE=$(echo $NEXT_CPU_INFO | awk '{print $4}')
	NEXT_USER_BUSY=$(echo $NEXT_CPU_INFO | awk '{print $1}')
	NEXT_SYS_BUSY=$(echo $NEXT_CPU_INFO | awk '{print $3}')
	NEXT_TOTAL_CPU_T=$(echo $NEXT_CPU_INFO | awk '{print $1+$2+$3+$4+$5+$6+$7}')
	NExT_CPU_USAGE=$(echo $NEXT_CPU_INFO | awk '{print $1+$2+$3}')

	#系统空闲时间
	SYSTEM_IDLE=$(echo ${NEXT_SYS_IDLE} ${LAST_SYS_IDLE} | awk '{print $1-$2}')
	#系统使用时间
	SYSTEM_BUSY=$(echo ${NEXT_SYS_BUSY} ${LAST_SYS_BUSY} | awk '{print $1-$2}')
	#用户使用时间
	USER_BUSY=$(echo ${NEXT_USER_BUSY} ${LAST_USER_BUSY} | awk '{print $1-$2}')
	#用户+系统+nice时间
	TOTAL_BUSY=$(echo ${NExT_CPU_USAGE} ${LAST_CPU_USAGE} | awk '{print $1-$2}')
	#CPU总时间
	TOTAL_TIME=$(echo ${NEXT_TOTAL_CPU_T} ${LAST_TOTAL_CPU_T} | awk '{print $1-$2}')

	#CPU总时间百分比
	CPU_USAGE=$(echo ${TOTAL_BUSY} ${TOTAL_TIME} | awk '{printf "%.2f", $1/$2*100}')
	#用户时间百分比
	CPU_USER_USAGE=$(echo ${USER_BUSY} ${TOTAL_TIME} | awk '{printf "%.2f", $1/$2*100}')
	#系统时间百分比
	CPU_sys_USAGE=$(echo ${SYSTEM_BUSY} ${TOTAL_TIME} | awk '{printf "%.2f", $1/$2*100}')

	echo "CPU Usage:${CPU_USAGE}%" $time
	echo "CPU UserUsage:${CPU_USER_USAGE}%" $time
	echo "CPU SysUsage:${CPU_sys_USAGE}%" $time
}

get_ion_info() {
	if [ -f "/sys/kernel/debug/ion/heaps/cma" ]; then
		echo "CMA:"
		cat /sys/kernel/debug/ion/heaps/cma
	else
		echo "ion's cma heap read failed"
	fi

	if [ -f "/sys/kernel/debug/ion/heaps/vmalloc" ]; then
		echo "Vmalloc:"
		cat /sys/kernel/debug/ion/heaps/vmalloc
	else
		echo "ion's vmalloc heap read failed"
	fi
}

get_drm_info() {
	if [ -f "/sys/kernel/debug/dri/0/mm_dump" ]; then
		echo "DRM:"
		cat /sys/kernel/debug/dri/0/mm_dump
	else
		echo "drm read failed"
	fi
}

do_exit() {
	echo "do_exit"
	exit 0
}

show_usage() {
	echo ""
	echo "Usage: $0 [-i seconds] [-p \"pid list\"]..."
	echo "  -i \t\tinterval(second),default value is 30"
	echo "  -p \t\tpid list"
	echo ""
	echo "example: $0 -i 30 -p \"233 513 679\""
}

#trap 'do_exit' 1 2 3 6 15

INTERVAL_S=10 # record every 300 seconds defaultly
PIDs=""

while getopts "i:p:o:" arg; do
	case $arg in
	i)
		INTERVAL_S=$OPTARG
		;;
	p)
		PIDs=$OPTARG
		;;
	?)
		show_usage
		exit 1
		;;
	esac
done

while true; do

	echo "********** uptime **********"
	uptime
	echo "================================================================================================================================================================================================================================================"
	echo "********** cat dev/mpi/vsys **********"
	if [ -f "cat /dev/mpi/vsys" ]; then
	cat /dev/mpi/vsys
	fi
	echo "********** cat /dev/mpi/vlog **********"
	if [ -f "cat /dev/mpi/vlog" ]; then
	cat /dev/mpi/vlog
	fi
	echo "********** cat dev/mpi/vsys **********"
	if [ -f "/dev/mpi/vsys" ]; then
	cat /dev/mpi/vsys
	fi
	if [ -f "/proc/vcodec/enc/venc_info" ]; then
	echo "********** cat /proc/vcodec/enc/venc_info **********"
	fi
	if [ -f "/proc/vcodec/enc/venc_info" ]; then
	cat /proc/vcodec/enc/venc_info
	fi
	echo "********** cat /proc/rk_dma_heap/alloc_bitmap **********"
	if [ -f "/proc/rk_dma_heap/alloc_bitmap" ]; then
	cat /proc/rk_dma_heap/alloc_bitmap
	fi
	echo "********** cat /proc/rk_dma_heap/dma_heap_info **********"
	if [ -f "/proc/rk_dma_heap/dma_heap_info" ]; then
	cat /proc/rk_dma_heap/dma_heap_info
	fi
	echo "********** cat /proc/rk_cma/rk-dma-heap-cma **********"
	if [ -f "/proc/rk_cma/rk-dma-heap-cma" ]; then
	cat /proc/rk_cma/rk-dma-heap-cma
	fi
	echo "********** cat /proc/rk_dmabuf/sgt **********"
	if [ -f "/proc/rk_dmabuf/sgt" ]; then
	cat /proc/rk_dmabuf/sgt
	fi
	echo "********** cat /proc/rk_dmabuf/dev **********"
	if [ -f "/proc/rk_dmabuf/dev" ]; then
		cat /proc/rk_dmabuf/dev
	fi

	echo "********** cat /proc/rkcif-mipi-lvds **********"
	if [ -f "/proc/rkcif-mipi-lvds" ]; then
	cat /proc/rkcif-mipi-lvds
	fi
	echo "********** cat /proc/rkisp-vir0 **********"
	if [ -f "/proc/rkisp-vir0" ]; then
	cat /proc/rkisp-vir0
	fi
	sleep $INTERVAL_S

done

do_exit
