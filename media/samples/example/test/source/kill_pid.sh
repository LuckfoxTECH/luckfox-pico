#!/bin/sh
set -x

__chk_cma_free()
{
	local f
	if [ ! -f "/proc/rk_dma_heap/alloc_bitmap" ];then
		echo "[$0] not found /proc/rk_dma_heap/alloc_bitmap, ignore"
		return
	fi
	f=`head  /proc/rk_dma_heap/alloc_bitmap |grep Used|awk '{print $2}'`
	if [ $f -gt 12 ];then
		echo "[$0] free cma error"
		exit 2
	fi
}

dump_log(){
	local log_dir cnt
	log_dir=$1
	cnt=$2
	mkdir -p $log_dir
	(cat /dev/mpi/vlog; cat /dev/mpi/valloc; cat /dev/mpi/vmcu; cat /dev/mpi/vrgn; cat /dev/mpi/vsys; echo ==========================================; cat /dev/mpi/vlog; cat /dev/mpi/valloc; cat /dev/mpi/vmcu; cat /dev/mpi/vrgn; cat /dev/mpi/vsys;) |tee > $log_dir/dev-mpi-$cnt.log
	cat /proc/vcodec/enc/venc_info &> $log_dir/proc-vcodec-enc-venc_info-$cnt-1
	cat /proc/vcodec/enc/venc_info &> $log_dir/proc-vcodec-enc-venc_info-$cnt-2
}
if mount|grep "\/mnt\/sdcard";then
	has_sdcard="/mnt/sdcard"
fi

killall nginx || echo "Not found nginx"
sleep 10
counter=0
while [ $counter -lt 10000 ]
do
    rkipc_pid=$(ps |grep rkipc|grep -v grep |awk '{print $1}')
    kill -9 "$rkipc_pid"
    while true
    do
        ps|grep rkipc |grep -v grep
        if [ $? -ne 0 ]; then
            echo "kill -9 rkipc exit, run once: count = $counter"
            break
        else
            sleep 1
            echo "rkipc active"
        fi
    done
	__chk_cma_free
    rkipc -a /oem/usr/share/iqfiles &
    sleep 5
    counter=$(( counter + 1 ))
	echo ""
	echo ""
	echo "----------------------------------------"
	echo "$0 counter [$counter]"
	if [ -n "$has_sdcard" ];then
		log1_dir=$has_sdcard/kill_pid/log1_$counter
		dump_log $log1_dir $counter
	fi
	echo "----------------------------------------"
	echo ""
	echo ""
done

killall nginx
sleep 10
counter=0
while [ $counter -lt 10000 ]
do
    killall rkipc
    while true
    do
        ps|grep rkipc |grep -v grep
        if [ $? -ne 0 ]; then
            echo "killall rkipc exit, run once: count = $counter"
            break
        else
            sleep 1
            echo "killall rkipc active"
        fi
    done
	__chk_cma_free
    rkipc -a /oem/usr/share/iqfiles &
    sleep 5
    counter=$(( counter + 1 ))
	echo ""
	echo ""
	echo "----------------------------------------"
	echo "$0 counter [$counter]"
	if [ -n "$has_sdcard" ];then
		log2_dir=$has_sdcard/kill_pid/log2_$counter
		dump_log $log2_dir $counter
	fi
	echo "----------------------------------------"
	echo ""
	echo ""
done
