#! /bin/bash

CUR_DIR=$(pwd)

usage() {
    echo "usage: ./build.sh <cpu_id 0~3 or all>"
}

CPU0_MEM_BASE=0x02600000
CPU1_MEM_BASE=0x00800000
CPU2_MEM_BASE=0x01200000
CPU3_MEM_BASE=0x01c00000
CPU0_MEM_SIZE=0x00900000
CPU1_MEM_SIZE=0x00a00000
CPU2_MEM_SIZE=0x00a00000
CPU3_MEM_SIZE=0x00a00000

make_hal() {
    export FIRMWARE_CPU_BASE=$(eval echo \$CPU$1_MEM_BASE)
    export DRAM_SIZE=$(eval echo \$CPU$1_MEM_SIZE)
    export SHMEM_BASE=0x02f00000
    export SHMEM_SIZE=0x00100000
    export NC_MEM_BASE=0x03000000
    export NC_MEM_SIZE=0x00100000
    export CUR_CPU=$1
    make clean
    rm $CUR_DIR/hal$1.elf $CUR_DIR/hal$1.bin
    make -j8
    cp $CUR_DIR/TestDemo.elf $CUR_DIR/hal$1.elf
    mv $CUR_DIR/TestDemo.bin $CUR_DIR/hal$1.bin
}

case $1 in
    0|1|2|3) make_hal $1 ;;
    all) make_hal 0; make_hal 1; make_hal 2; make_hal 3 ;;
    *) usage; exit ;;
esac
