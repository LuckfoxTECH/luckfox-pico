#!/bin/sh

rcK()
{
	for i in $(ls /oem/usr/etc/init.d/S??*) ;do

		# Ignore dangling symlinks (if any).
		[ ! -f "$i" ] && continue

		case "$i" in
			*.sh)
				# Source shell script for speed.
				(
					trap - INT QUIT TSTP
					set stop
					. $i
				)
				;;
			*)
				# No sh extension, so fork subprocess.
				$i stop
				;;
		esac
	done
}

echo "Stop Application ..."
killall rkipc
killall udhcpc

cnt=0
while [ 1 ];
do
	sleep 1
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
done

rcK
