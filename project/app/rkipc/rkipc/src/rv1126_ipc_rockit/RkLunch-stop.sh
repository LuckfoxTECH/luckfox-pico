#!/bin/sh

echo "Stop Application ..."
killall rkipc

cnt=0
while [ 1 ];
do
	cnt=$(( cnt + 1 ))
	if [ $cnt -eq 8 ]; then
		echo "killall -9 rkipc"
		killall -9 rkipc
		sleep 0.1
		break
	fi
	ps|grep rkipc|grep -v grep
	if [ $? -ne 0 ]; then
		echo "rkipc exit"
		break
	else
		echo "rkipc active"
	fi
	sleep 1
done
