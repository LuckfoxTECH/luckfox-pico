#!/bin/bash

HOSTPAD_DIR=../../../../3rd-party/hostapd/hostapd2.0/
WPA_SUPPLICANT_DIR=../../../../3rd-party/wpa_supplicant-2.4/wpa_supplicant

## unload ap
service isc-dhcp-server stop
killall dhcpd
killall hostapd
killall wpa_supplicant

# load driver
./load.sh

# find wlan1
ssv_wlan_1=`script/find_ssv_wlan`

sleep 1

ssv_phy=`script/find_ssv_phy`
if [ -z "$ssv_phy" ]; then
    echo SSV PHY device not found.;
    exit 1;
fi

ssv_wlan_1=`script/find_ssv_wlan`
if [ -z "$ssv_wlan_1" ]; then
    echo SSV primary WLAN device not found.;
    exit 1;
fi

echo "Primary SSV WLAN interface is $ssv_wlan_1"

# add wlan2
ssv_wlan_2=`echo $ssv_wlan_1 | sed -e s/wlan//g`
ssv_wlan_2=`expr $ssv_wlan_2 + 1`
ssv_wlan_2="wlan$ssv_wlan_2"
echo Second WLAN interface is $ssv_wlan_2

echo "Add second interface $ssv_wlan_2 to SSV PHY device $ssv_phy"
iw $ssv_phy interface add $ssv_wlan_2 type station

sleep 1

ssv_wlans="`script/find_ssv_wlan`"
for ssv_wlan in $ssv_wlans; do
    if [ $ssv_wlan != $ssv_wlan_1 ]; then
        echo Second SSV WLAN device is actually $ssv_wlan
        break;
    fi
done


echo -e "${YELLOW}Config wireless AP...${NC}"
#rm -rf load_dhcp.sh
#rm -rf hostapd.conf
#relpace wlan@@ to real device name
cp script/template/load_dhcp.sh load_dhcp.sh

awk 'NF' script/template/hostapd.conf | grep -v '#' > hostapd.conf
awk 'NF' ap.cfg | grep -v '#' >> hostapd.conf

sed -i "s/wlan@@/$ssv_wlan_1/" load_dhcp.sh
sed -i "s/wlan@@/$ssv_wlan_1/" hostapd.conf

chmod 777 load_dhcp.sh

#move to right position
#mv load_dhcp.sh $HOSTPAD_DIR
#mv hostapd.conf $HOSTPAD_DIR/hostapd/

dhcp_config_file="/etc/default/isc-dhcp-server"
dhcp_config=$(grep "$ssv_wlan_1" $dhcp_config_file)
if [ "$dhcp_config" == "" ]; then
	echo -en "${YELLOW}Config $dhcp_config_file.....${NC}"
	
	rm -rf tmp
	sed '/INTERFACE/d' /etc/default/isc-dhcp-server >>tmp
	echo "INTERFACES=\"$ssv_wlan_1\"" >>tmp	
	rm -rf $dhcp_config_file	
	mv tmp /etc/default/isc-dhcp-server
	
	echo -e "${YELLOW}OK${NC}"
fi
	
	
dir=$(pwd)
echo -e "${YELLOW}Wireless Done. ${NC}"
trap handle_stop INT

function version_great() { test "$(printf '%s\n' "$@" | sort -V | head -n 1)" != "$1"; }
nmcli_version=$(nmcli -v | cut -d ' ' -f 4)
chk_nmcli_version=0.9.8.999

function handle_stop() {
#    popd
    if version_great $nmcli_version $chk_nmcli_version; then
        nmcli radio wifi on
    else
        nmcli nm wifi on
    fi
    
    echo -e "${YELLOW}Shutting down AP.${NC}"
    ./ap_shutdown.sh
}
        

if version_great $nmcli_version $chk_nmcli_version; then
    nmcli radio wifi off
else
    nmcli nm wifi off
fi

sudo rfkill unblock wlan

./load_dhcp.sh &

PID=$!
wait $PID

echo -e "${YELLOW}Load AP...${NC}"
echo -e "${GREEN}Launch hostapd.${NC}"
#run hostapd2.0
$HOSTPAD_DIR/hostapd/hostapd -t hostapd.conf &
#hostapd -t hostapd.conf

ifconfig $ssv_wlan_2 192.168.1.33 up
sleep 2
#$WPA_SUPPLICANT_DIR/wpa_supplicant -Dnl80211 -i $ssv_wlan_2 -c ./wpa_supplicant.conf -dd -B -f /var/log/wpa_supplicant.log