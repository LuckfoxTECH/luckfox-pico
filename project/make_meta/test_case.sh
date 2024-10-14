#!/bin/bash
set -e

cwd=$(dirname $(readlink -f $0))
cd ${cwd}
test_out=$cwd/outdir_test
mkdir -p $test_out

CMD_VERSION="1.0.0"

TEST_CASE()
{
	md5=$1
	shift
	$cwd/make_meta/make_meta_host --update --meta_path=$test_out/meta.img $@ 1>/dev/null
	if md5sum $test_out/meta.img |grep -w $md5 1>/dev/null; then
		echo "TEST SUCCESS $@"
	else
		echo ""
		echo "TEST FAILURE $@"
		md5sum $test_out/meta.img
		echo ""
		exit 1
	fi
	$cwd/make_meta/make_meta_host -r $test_out/meta.img 1>/dev/null
}

$cwd/build_meta.sh \
	--main_sensor_iq $cwd/sensor_iqfile_bin/sc230ai_CMK-OT2115-PC1_30IRC-F16.bin \
	--second_sensor_iq $cwd/sensor_iqfile_bin/sc230ai_CMK-OT2115-PC1_30IRC-F16.bin \
	--output $test_out

TEST_CASE 231ddc91b57a434828dfad8e3775b702 --rk_cam_hdr 5
TEST_CASE 0b13a54234f081d442cefc56c1b0d5d3 --rk_cam_w 1234
TEST_CASE 74ca94f41009c72e265d1c35cf119b19 --rk_cam_h 1234
TEST_CASE 79d44bc5543daf026b9547cb1dddbb5b --rk_cam_fps 20
TEST_CASE aeed3c074fb3988f97fa32a70db379e8 --rk_led_value 40
TEST_CASE 68e27cf6e2675dfe05d3fe111c9367aa --rk_night_mode 2
TEST_CASE ebf8ad59371bc3d09f4bfbaf412a3d4f --rk_venc_w 1234
TEST_CASE 1cf264aea7b0f25b52dec32bfff32710 --rk_venc_h 1234
TEST_CASE 54a5050f24ce0024d4ac88153a1081b0 --rk_venc_type 2
TEST_CASE f644041fe9867f88621658c5f572e28a --rk_cam_mirror_flip 3
TEST_CASE 8b23f337229dd4f988c4f0599b320209 --rk_venc_bitrate 1234
TEST_CASE 225279c0cb705e056a720c32ce2e8692 --rk_color_mode 1
TEST_CASE d48ced092a25fc05800ee5b4e361f56b --cmdline "test=123"
TEST_CASE f86e41726a086aa547e8660d84807bfb --rk_fastae_max_frame 12
TEST_CASE a04662a449297d6eb3b06571e09da385 --ae_awb_tab sensor_ae_awb_table/ae_awb_table.bin
TEST_CASE a04662a449297d6eb3b06571e09da385 --sensor_init sensor_init/init_sensor.bin
TEST_CASE a04662a449297d6eb3b06571e09da385 --sensor_iq_bin sensor_iqfile_bin/sc230ai_CMK-OT2115-PC1_30IRC-F16.bin

TEST_CASE 9db3c781d8e065d56543794a61a180ef --rk_cam2_w 1234
TEST_CASE 357402f6010d1207573f587c2dae2c32 --rk_cam2_h 1234
TEST_CASE aded89870476c0b8f9d33ff257dd9533 --rk_cam2_fps 12
TEST_CASE 68b3690d644ba169948705328194e3e2 --rk_venc2_w 1234
TEST_CASE ea2abe2ad6403be7fc9526ae72538087 --rk_venc2_h 1234
TEST_CASE 58267bf23bcc8a877163fccc30dc5755 --rk_venc2_type 2
TEST_CASE 4782a360c0282a528050bf9ed7396416 --rk_venc2_bitrate 1234
TEST_CASE a766c2e22247c13cce5d85dc4816210a --rk_cam2_mirror_flip 2
TEST_CASE 4aeead9eaf5858934ce8972f0fecc361 --rk_cam2_hdr 5
TEST_CASE ef996314296c86c417b1f2361e59ea03 --rk_cam_index 1
TEST_CASE a04662a449297d6eb3b06571e09da385 --secondary_sensor_init sensor_init/init_secondary_sensor.bin
TEST_CASE a04662a449297d6eb3b06571e09da385 --secondary_sensor_iq_bin sensor_iqfile_bin/sc230ai_CMK-OT2115-PC1_30IRC-F16.bin

TEST_CASE 28692be45408d544ec37e978e01a6e16 \
	--rk_cam_mirror_flip 3 \
	--rk_cam_w 1234 \
	--rk_cam_h 1234 \
	--rk_venc_w 1234  \
	--rk_venc_h 1234 \
	--rk_venc_type 2  \
	--rk_venc_bitrate 1234  \
	--rk_cam_fps 20 \
	--rk_cam_hdr 5 \
	--rk_color_mode 1 \
	--rk_cam2_w 1234 \
	--rk_cam2_h 1234 \
	--rk_cam2_fps 12 \
	--rk_venc2_w 1234 \
	--rk_venc2_h 1234 \
	--rk_venc2_type 2 \
	--rk_venc2_bitrate 1234 \
	--rk_cam2_mirror_flip 2 \
	--rk_cam2_hdr 5 \
	--rk_led_value 40  \
	--rk_night_mode 2  \
	--rk_cam_index 1

rm -rf $test_out
