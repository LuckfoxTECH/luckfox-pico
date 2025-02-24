#!/bin/sh
program_kill()
{
        P_PID=`ps | grep $1 | grep -v grep | awk '{print $1}'`
        test -z ${P_PID} || kill -9 ${P_PID}
}

echo "Stop Application ..."
program_kill RkLunch.sh
killall rk_mpi_uvc &

cnt=0
while [ 1 ];
do
	sleep 1
	cnt=$(( cnt + 1 ))
	if [ $cnt -eq 8 ]; then
		echo "killall -9 rk_mpi_uvc"
		killall -9 rk_mpi_uvc
		sleep 0.1
		break
	fi
	ps|grep rk_mpi_uvc|grep -v grep
	if [ $? -ne 0 ]; then
		echo "rk_mpi_uvc exit"
		break
	else
		echo "rk_mpi_uvc active"
	fi
done
