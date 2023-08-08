#!/bin/bash
prompt="Pick the target platform:"
chip_options=("a33" \
              "a64" \
              "h8" \
              "h3")
PLATFORM=""

select opt in "${chip_options[@]}" "Quit"; do 
    case "$REPLY" in

    1 ) echo "${chip_options[$REPLY-1]} is option";PLATFORM=${chip_options[$REPLY-1]};break;;
    2 ) echo "${chip_options[$REPLY-1]} is option";PLATFORM=${chip_options[$REPLY-1]};break;;
    3 ) echo "${chip_options[$REPLY-1]} is option";PLATFORM=${chip_options[$REPLY-1]};break;;
    4 ) echo "${chip_options[$REPLY-1]} is option";PLATFORM=${chip_options[$REPLY-1]};break;;

    $(( ${#chip_options[@]}+1 )) ) echo "Goodbye!"; break;;
    *) echo "Invalid option. Try another one.";continue;;
    esac
done

if [ "$PLATFORM" != "" ]; then
./ver_info.pl include/ssv_version.h
cp Makefile.cross_linux Makefile
cp platforms/${chip_options[$REPLY-1]}.cfg ssv6051.cfg
cp platforms/platform-config.mak platform-config.mak
cp platforms/${chip_options[$REPLY-1]}-generic-wlan.c ssv6051-generic-wlan.c
cp platforms/${chip_options[$REPLY-1]}-wifi.cfg ssv6051-wifi.cfg
rm -rf platforms
echo "Done Makefile!"
else
echo "Fail!"
fi

