
cp 88-rockusb.rules /etc/udev/rules.d/

restart udev的命令:
udevadm control --reload-rules
service udev restart

