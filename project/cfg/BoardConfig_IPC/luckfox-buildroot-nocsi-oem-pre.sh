#!/bin/bash

function lf_rm() {
    for file in "$@"; do
        if [ -e "$file" ]; then
            echo "Deleting: $file"
            rm -rf "$file"  
        #else
            #echo "File not found: $file" 
        fi
    done
}

# remove unused files
function remove_data()
{
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/*.aiisp
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/*.data
    
    # drm
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/libdrm*
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/libdrm_rockchip*

    # kms
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/libkms*

    # freetype
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/libfreetype*

    # iconv
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/libiconv*

    # rkAVS
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/librkAVS*
    
    # jpeg
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/libjpeg*

    # png
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/lib/libpng*

    # vqefiles
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/share/vqefiles/*

    # ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/phy-rockchip-csi2-dphy-hw.ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/phy-rockchip-csi2-dphy.ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/rga3.ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/rknpu.ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/rockit.ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/sc3336.ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/video_rkcif.ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/video_rkisp.ko
    lf_rm $RK_PROJECT_PACKAGE_OEM_DIR/usr/ko/mpp_vcodec.ko
    
}

#=========================
# run
#=========================
remove_data