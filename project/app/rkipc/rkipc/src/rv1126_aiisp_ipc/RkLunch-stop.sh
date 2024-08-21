#!/bin/sh

echo "Stop Application ..."
killall rkipc

while [ 1 ];
do
	ps|grep rkipc|grep -v grep
	if [ $? -ne 0 ]; then
		echo "rkipc exit"
		break
	else
		echo "rkipc active"
	fi
	sleep 1
done
