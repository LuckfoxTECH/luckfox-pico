#/bin/bash
BLUE='\e[1;34m'
GREEN='\e[1;32m'
CYAN='\e[1;36m'
RED='\e[1;31m'
PURPLE='\e[1;35m'
YELLOW='\e[1;33m'
# No Color
NC='\e[0m'

echo -e "${YELLOW}Load DHCP...${NC}"
echo -e "${YELLOW}Check DHCP Config...${NC}"
dhcp_config_file="/etc/dhcp/dhcpd.conf"
dhcp_domain="subnet 192.168.0.0 netmask 255.255.255.0"
dhcp_config=$(grep "subnet 192.168.0.0 netmask 255.255.255.0" $dhcp_config_file)
if [ "$dhcp_config" != "$dhcp_domain" ]; then
	echo -en "${YELLOW}Config $dhcp_config_file.....${NC}"
	echo "subnet 192.168.0.0 netmask 255.255.255.0" >> $dhcp_config_file
	echo "{" >> $dhcp_config_file
	echo "  range 192.168.0.2 192.168.0.10;" >> $dhcp_config_file
	echo "  option routers 192.168.0.1;" >> $dhcp_config_file
	echo "  option domain-name-servers 168.95.1.1;" >> $dhcp_config_file
	echo "}" >> $dhcp_config_file

	echo -e "${YELLOW}OK${NC}"
else
	echo -e "${YELLOW}192.168.0.x domain is existed${NC}"
fi

echo -e "${GREEN}Config IP.${NC}"
ifconfig wlan@@ up
ifconfig wlan@@ 192.168.0.1 netmask 255.255.255.0

echo -e "${GREEN}Config DHCP Server.${NC}"
service isc-dhcp-server start
sleep 3

echo -e "${GREEN}Config routing table.${NC}"
bash -c "echo 1 >/proc/sys/net/ipv4/ip_forward"
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
