#! /bin/bash



HOSTPAD_DIR=../hostapd/hostapd/
BLUE='\e[1;34m'
GREEN='\e[1;32m'
CYAN='\e[1;36m'
RED='\e[1;31m'
PURPLE='\e[1;35m'
YELLOW='\e[1;33m'
# No Color
NC='\e[0m'


check_package(){	
	for i in {1..5}
	do
		echo -en "${GREEN}Check package - $1...${NC}"
		status=$(dpkg -s $1 | grep "Status: install ok installed")
		if [ "$status" == "" ]; then
				echo -e "${PURPLE}Try to intall package $1...${NC}"
				apt-get install -y $1
				retval=2
		else
				echo -e "${GREEN}OK${NC}"
				retval=1
				break;
		fi
	done
	return "$retval"		
}

check_dhcp_server_config(){

	dhcp_config_file="/etc/dhcp/dhcpd.conf"
	echo -en "${YELLOW}Check DHCPD config...${NC}"
	dhcp_config=$(grep "subnet 192.168.0.0 netmask 255.255.255.0" $dhcp_config_file)

	if [ "$dhcp_config" == "" ]; then
		echo -en "${YELLOW}Config $dhcp_config_file.....${NC}"
		echo "subnet 192.168.0.0 netmask 255.255.255.0" >> $dhcp_config_file
		echo "{" >> $dhcp_config_file
		echo "	range 192.168.0.2 192.168.0.10;" >> $dhcp_config_file
		echo "	option routers 192.168.0.1;" >> $dhcp_config_file
		echo "	option domain-name-servers 168.95.1.1;" >> $dhcp_config_file
		echo "}" >> $dhcp_config_file
		
		echo -e "${YELLOW}OK${NC}"
	else
		echo -e "${YELLOW}OK${NC}"
	fi
}




check()
{
	check_package $1
	ret=$? 
	if test $ret -eq 2 
	then
		echo -e "${RED}#############FAIL to CHECK PACKAGE for AP MODE#############${NC}"
		exit 0;
	fi
}

#############################################==MAIN==#############################################
#
if [ ! -d "$HOSTPAD_DIR" ]; then
  echo -e "${RED}#############HOSTPAD needs to be put at the same level of the wireless driver#############${NC}"
  exit 0;
fi


#
echo -e "${YELLOW}Check internet access...${NC}"
wget --spider http://www.google.com

if [ "$?" != 0 ]; then
  	echo -e "${RED}Please connect to internet firstly......${NC}"
  	exit 0;
fi

#
echo -e "${YELLOW}Check necessary packages to run AP mode${NC}"
check libnl1
check libnl-dev
check libssl-dev
check isc-dhcp-server
check_dhcp_server_config

#
cd $HOSTPAD_DIR
make

