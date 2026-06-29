# Luckfox Pico SDK — Ubuntu 22.04 build host (for macOS via Docker)
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive \
    LC_ALL=C \
    LANG=C.UTF-8

# Packages mirror README "Installing Dependencies" plus tools used by docker-build.sh
RUN apt-get update && apt-get install -y --no-install-recommends \
        git ssh make gcc gcc-multilib g++-multilib module-assistant expect \
        g++ gawk texinfo libssl-dev bison flex fakeroot cmake unzip gperf \
        autoconf device-tree-compiler libncurses5-dev pkg-config bc \
        python-is-python3 passwd openssl openssh-client vim file cpio rsync \
        wget ca-certificates xz-utils bzip2 patch libtool \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /sdk

# Rockchip / Luckfox cross toolchain (Linux ELF; only runs inside this container)
ENV PATH="/sdk/tools/linux/toolchain/arm-rockchip830-linux-uclibcgnueabihf/bin:${PATH}"

CMD ["/bin/bash"]
