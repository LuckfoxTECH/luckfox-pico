#!/bin/sh

echo "Stop Application ..."
killall rkipc
killall udhcpc

while [ 1 ];
do
	sleep 1
	ps|grep rkipc|grep -v grep
	if [ $? -ne 0 ]; then
		echo "rkipc exit"
		break
	else
		echo "rkipc active"
	fi
done
