io -w -f /oem/usr/share/rtthread.bin 0x40000
io -w -f /oem/usr/share/meta.img 0x800000 
io -w -f /oem/usr/share/sc3338_FKO1_30IRC-F16.bin 0x810040
while true; do
sleep .5;
echo mem > /sys/power/state;
done
