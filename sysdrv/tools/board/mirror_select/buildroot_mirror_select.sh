#!/bin/bash
BACKUP_MIRROR_SITE="http://sources.buildroot.net"

#PRIMARY_MIRROR_SITES=("http://sources.buildroot.net" "https://mirrors.lzu.edu.cn/buildroot")
PRIMARY_MIRROR_SITES=("http://sources.buildroot.net" "https://mirrors.nju.edu.cn/buildroot/")

# Uncomment and use other mirror sites if needed
# KERNEL_MIRROR_SITES=(
#     "https://cdn.kernel.org/pub"
#     "https://mirror.bjtu.edu.cn/kernel/"
# )
#
# GNU_MIRROR_SITES=(
#     "http://ftpmirror.gnu.org"
#     "http://mirrors.nju.edu.cn/gnu/"
# )
#
# LUAROCKS_MIRROR_SITES=(
#     "http://rocks.moonscript.org"
#     "https://luarocks.cn"
# )
#
# CPAN_MIRROR_SITES=(
#     "https://cpan.metacpan.org"
#     "http://mirrors.nju.edu.cn/CPAN/"
# )

function get_fastest_mirror() {
	local MIRROR_SITES=("${!1}")
	declare -A CONNECT_TIMES

	for site in "${MIRROR_SITES[@]}"; do
		time=$(timeout 1.5 curl -s -w "%{time_connect}\n" -o /dev/null "$site")
		if [ $? -eq 0 ]; then
			CONNECT_TIMES["$site"]=$time
		else
			CONNECT_TIMES["$site"]=10 # Assign a high value if timeout occurs
		fi
	done

	if [ ${#CONNECT_TIMES[@]} -eq 0 ]; then
		fastest_site="${MIRROR_SITES[0]}"
		echo "$fastest_site"
		return 1
	fi

	fastest_site=""
	min_time=10

	for site in "${!CONNECT_TIMES[@]}"; do
		time="${CONNECT_TIMES[$site]}"
		if awk "BEGIN {exit !($time < $min_time)}"; then
			min_time="$time"
			fastest_site="$site"
		fi
	done
	echo "$fastest_site"
}

PRIMARY_FAST_MIRROR=$(get_fastest_mirror PRIMARY_MIRROR_SITES[@])
echo "Fast mirror is $PRIMARY_FAST_MIRROR"

PRIMARY_MIRROR_STRING='BR2_PRIMARY_SITE='
BACKUP_MIRROR_STRING='BR2_BACKUP_SITE='

CONFIG_PATH="$1"
sed -i "/$PRIMARY_MIRROR_STRING/c\BR2_PRIMARY_SITE=\"$PRIMARY_FAST_MIRROR\"" "$CONFIG_PATH"
sed -i "/$BACKUP_MIRROR_STRING/c\BR2_BACKUP_SITE=\"$BACKUP_MIRROR_SITE\"" "$CONFIG_PATH"
