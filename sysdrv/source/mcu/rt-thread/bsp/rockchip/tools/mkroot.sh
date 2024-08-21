#! /bin/bash

export LC_ALL=C.UTF-8
export LANG=C.UTF-8

usage() {
	echo "usage: ./mkroot_ab.sh <userdata_path> [partition_setting]"
}

if [ "$1" == "" ]
then
    usage
    exit
fi

CUR_DIR=$(pwd)
IMAGE=$(pwd)/Image

ROOT_PATH="$1"
echo "$ROOT_PATH"
RESOURCE_PATH=$ROOT_PATH
ROOT_NAME=root.img
ROOTA_NAME=root_a.img
ROOTB_NAME=root_b.img

if [ ! -n "$2" ] ;then
FW_MERGER_CFG=$(pwd)/board/common/setting.ini
echo $FW_MERGER_CFG
else
FW_MERGER_CFG="$2"
echo $FW_MERGER_CFG
fi

#Del file whether exist
if [ ! -f "$IMAGE/$ROOT_NAME" ] || [ ! -f "$IMAGE/$ROOTA_NAME" ] || [ ! -f "$IMAGE/$ROOTB_NAME" ];then
    echo "root File not exist"
else
    rm -rf $IMAGE/root*.img
fi

ROOT_PART_SIZE=`grep -r -A 4 "^Name*=*root" $FW_MERGER_CFG |grep -wi "^PartSize" |cut -d '=' -f 2| cut -d 'x' -f 2`
echo "$ROOT_PART_SIZE"

ROOT_CNT=0
# if [ $ROOT_CNT -eq 1 ]
# then
    # TRANS=`echo $ROOT_PART_SIZE | tr 'a-z' 'A-Z'`
    # ROOT_EACH_SIZE=`echo "ibase=16;$TRANS"| tr -d $'\r' | bc`
    # echo "userdata part size is :$ROOT_EACH_SIZE sector"
# fi

for each_size in $ROOT_PART_SIZE
do
    ROOT_CNT=$[$ROOT_CNT + 1]

    TRANS=`echo $each_size | tr 'a-z' 'A-Z'`
    ROOT_EACH_SIZE=`echo "ibase=16;$TRANS" |tr -d $'\r' | bc`
    echo "userdata part size is :$ROOT_EACH_SIZE sector"

    #transform to KB unit
    ROOT_PART_SIZE=`echo "ibase=10;$ROOT_EACH_SIZE/2" | bc`
    echo "userdata each part size is :$ROOT_PART_SIZE KB"

    #transform to 4096B one sector unit
    ROOT_SECTOR_SIZE=`echo "ibase=10;$ROOT_PART_SIZE/4" | bc`
    echo "userdata each part size is :$ROOT_SECTOR_SIZE *4KB"

    echo "Making $ROOT_NAME from $RESOURCE_PATH with size($ROOT_PART_SIZE K) with $ROOT_SECTOR_SIZE sectors"

    ROOT_NAME=root.img
    dd of=$ROOT_NAME bs=4K seek=$ROOT_SECTOR_SIZE count=0 2>&1 || fatal "Failed to dd image!"
    mkfs.fat -S 4096 $ROOT_NAME
    MTOOLS_SKIP_CHECK=1 mcopy -bspmn -D s -i $ROOT_NAME $RESOURCE_PATH/* ::/

done

if [ $ROOT_CNT -eq 2 ]
then
    for i in a b
    do
        cp -f $ROOT_NAME $IMAGE/root_$i.img
    done
    echo "remove $ROOT_NAME"
    rm -rf $ROOT_NAME
    echo "make root a/b image file done!"
else
    mv $ROOT_NAME $IMAGE
    echo "make root.image file done!"
fi
