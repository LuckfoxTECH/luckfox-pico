#!/bin/sh

FILE_TO_CHECK="/etc/firstboot"

update_config() {
    # Mount essential filesystems
    mount -t proc proc /proc
    
    # Remount root filesystem with read-write permissions
    mount -o remount,rw /
    
    # Configure network
    ifconfig eth0 up
    udhcpc -i eth0
    
    # Wait for network connectivity
    while ! ip a show eth0 | grep 'inet ' > /dev/null 2>&1; do
        sleep 1
    done
    
    # Sync system time
    ntpd -q && hwclock -w
    
    # Update package repositories and system
    sed -i 's/dl-cdn.alpinelinux.org/mirrors.aliyun.com/g' /etc/apk/repositories
    apk update
    
    # Install and configure OpenRC
    apk add openrc
    rc-update add devfs boot
    rc-update add procfs boot
    rc-update add sysfs boot
    rc-update add ubiattach boot
    chmod a+x /etc/local.d/crond.start
    rc-update add local default
    rc-update add networking default
    
    # Start essential services
    #rc-service devfs start
    #rc-service procfs start
    #rc-service sysfs start
    #rc-service local start
    
    # Configure timezone
    apk add tzdata && \
    if [ -f "/etc/timezone" ]; then
        TIMEZONE=$(cat /etc/timezone)
        cp /usr/share/zoneinfo/$TIMEZONE /etc/localtime && apk del tzdata
    fi

    # Configure terminal
    apk add agetty && printf "luckfox\nluckfox\n" | passwd root && \
        sed -i 's|ttyFIQ0::respawn:/bin/sh -l|ttyFIQ0::respawn:/sbin/agetty --noclear ttyFIQ0 115200|' /etc/inittab
#    cat > /etc/profile.d/terminal.sh << 'EOF'
##!/bin/sh
#if [ -t 0 ]; then
#    stty rows 24 cols 80
#fi
#EOF
#    chmod +x /etc/profile.d/terminal.sh

    # Install and configure bash
    apk add bash bash-completion && \
    sed -i 's|root:x:0:0:root:/root:/bin/sh|root:x:0:0:root:/root:/bin/bash|' /etc/passwd
    
    # Install and configure SSH
    apk add openssh && \
    sed -i 's/#PermitRootLogin .*/PermitRootLogin yes/' /etc/ssh/sshd_config && \
    sed -i 's/#PasswordAuthentication .*/PasswordAuthentication yes/' /etc/ssh/sshd_config && \
    rc-update add sshd default
    
    # Install additional utilities
    apk add util-linux dialog dtc i2c-tools mtd-utils e2fsprogs e2fsprogs-extra
    apk add btop ncurses ncurses-terminfo-base terminus-font musl-locales
    apk add sqlite
    apk add python3 py3-pip py3-smbus
    #apk add gcc musl-dev build-base gcompat linux-headers
    apk add speedtest-cli

    # Remove dev dependency
    sed -i 's/need sysfs dev/need sysfs/' /etc/init.d/hwdrivers
    sed -i 's/need sysfs dev/need sysfs/' /etc/init.d/machine-id
     
    # Clean and reboot
    rm $FILE_TO_CHECK
    sync
    reboot
}

if [ -f "$FILE_TO_CHECK" ]; then
    update_config
fi
