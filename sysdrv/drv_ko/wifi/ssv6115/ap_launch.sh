#! /bin/bash -x


BLUE='\e[1;34m'
GREEN='\e[1;32m'
CYAN='\e[1;36m'
RED='\e[1;31m'
PURPLE='\e[1;35m'
YELLOW='\e[1;33m'
# No Color
NC='\e[0m'

#usage......
#./ap_launch.sh [wlanx]
if [ -z "$1" ]; then
echo -e "${RED}Please input the intfeface name${NC}"
echo -e "${RED}Usage:${NC}"
echo -e "${RED}      ./ap_launch.sh [wlanx]${NC}"
echo -e ""
echo -e ""
echo -e ""
echo -e "${PURPLE}Please follow this procedure to lauch ap on ubuntu platform${NC}"
echo -e "${PURPLE}1. ./ap_shutdown.sh${NC}"
echo -e "${PURPLE}2. ./load.sh${NC}"
echo -e "${PURPLE}3. Disable ubuntu's network managerment${NC}"
echo -e "${PURPLE}4. rfkill unblock wifi${NC}"
#echo -e "${PURPLE}5. service isc-dhcp-server stop${NC}"
#echo -e "${PURPLE}6. service isc-dhcp-server start${NC}"
echo -e "${PURPLE}5. ps aux | grep dhcp (find the number of dhcp-server)${NC}"
echo -e "${PURPLE}6. kill [number] ${NC}"
echo -e "${PURPLE}7. edit ap.cfg for your requirement. ex:SSID, channel ${NC}"
echo -e "${PURPLE}8. use iwconfig to get your interface name${NC}"
echo -e "${PURPLE}9. ./ap_launch.sh [wlanx]${NC}"
exit
fi

    
dir=$(pwd)
cd $dir

#echo -e "${YELLOW}load wireless driver...${NC}"
#./load.sh

#echo -e "${YELLOW}Unoad wireless driver...${NC}"
#./unload.sh
#PID=$!
#wait $PID
#sleep 2

#echo -e "${YELLOW}Load wireless driver...${NC}"
#./load.sh
#PID=$!
#wait $PID
#sleep 2

#devName=wlan0
devName=$1
echo "device name=$devName"

echo -e "${YELLOW}Config wireless AP...${NC}"
#rm -rf load_dhcp.sh
#rm -rf hostapd.conf
#relpace wlan@@ to real device name
#cp script/template/load_dhcp.sh load_dhcp.sh
#cp script/template/hostapd.conf hostapd.conf
#awk 'NF' script/template/hostapd.conf | grep -v '#' > hostapd.conf
#awk 'NF' ap.cfg | grep -v '#' >> hostapd.conf
 
sed -i -r "s/wlan[0-9]+/$devName/g" script/load_dhcp.sh
sed -i -r "s/wlan[0-9]+/$devName/g" hostapd.conf

chmod 777 script/load_dhcp.sh

#move to right position
#mv load_dhcp.sh $HOSTPAD_DIR
#mv hostapd.conf $HOSTPAD_DIR/hostapd/

dhcp_config_file="/etc/default/isc-dhcp-server"
dhcp_config=$(grep "$devName" $dhcp_config_file)
if [ "$dhcp_config" != "$devName" ]; then
	echo -en "${YELLOW}Config $dhcp_config_file.....${NC}"
	
	rm -rf tmp
	sed '/INTERFACE/d' /etc/default/isc-dhcp-server >>tmp
	echo "INTERFACES=\"$devName\"" >>tmp	
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
    
    #echo "@@killall hostapd..."
    hostapd_pid=`pgrep hostapd`
    [ $? -eq 0 ] && (echo "\nKilling hostapd..."; kill INT $hostapd_pid)
    PID=$!
    wait $PID
    sleep 2

    ### kill dhcp server
    dhcpd_pid=`pgrep dhcpd`
    [ $? -eq 0 ] && (echo "\nKilling dhcpd..."; kill -KILL $dhcpd_pid)
    PID=$!
    wait $PID
    sleep 2

    ### ifconfig down
    ifconfig $devName down
    PID=$!
    wait $PID
    sleep 1

    ### unload dirver
    ./unload.sh
    PID=$!
    wait $PID
    sleep 2
    
    echo -e "${YELLOW}done to shutdown AP.${NC}"
}
        
if version_great $nmcli_version $chk_nmcli_version; then
    nmcli radio wifi off
else
    nmcli nm wifi off
fi

sudo rfkill unblock wlan

#pushd $HOSTPAD_DIR
#. ./load_ap.sh
#$HOSTPAD_DIR/load_dhcp.sh &
script/load_dhcp.sh &
PID=$!
wait $PID

echo -e "${YELLOW}Load AP...${NC}"
echo -e "${GREEN}Launch hostapd.${NC}"
#run hostapd2.0
#$HOSTPAD_DIR/hostapd -t hostapd.conf
hostapd -t hostapd.conf 
