
1. 编译说明：

清除编译文件:
./build_meta.sh -c
./build_meta.sh --clean

编译sc230ai对应的meta.img：
./build_meta.sh  --main_sensor_iq sensor_iqfile_bin/sc230ai_CMK-OT2115-PC1_30IRC-F16.bin

编译主摄是sc3338，副摄是sc230ai对应的meta.img：
./build_meta.sh --main_sensor_iq sensor_iqfile_bin/sc3338_FKO1_30IRC-F16.bin \
	--second_sensor_iq    sensor_iqfile_bin/sc230ai_CMK-OT2115-PC1_30IRC-F16.bin

注意： sensor_iqfile_bin目录下的IQ文件需要单独更新。

2. 命令说明:
make_meta_host 运行在服务器端
make_meta 运行在板端
make_meta -h 查看具体命令

通过rz把init_sensor.bin下载到板端

板端更新init_sensor.bin的命令
make_meta --update --meta_path /path-to-meta --sensor_init ./init_sensor.bin

板端更新ae_awb_table.bin的命令
make_meta --update --meta_path /path-to-meta --ae_awb_tab ./ae_awb_table.bin

板端更新sc230ai_CMK-OT2115-PC1_30IRC-F16.bin的命令
然后rz sc230ai_CMK-OT2115-PC1_30IRC-F16.bin到板端，运行如下命令：
make_meta --update --meta_path /path-to-meta --sensor_iq_bin ./sc230ai_CMK-OT2115-PC1_30IRC-F16.bin

对于双摄像头:
make_meta --update --meta_path /path-to-meta \
	--sensor_iq_bin ./sc230ai_CMK-OT2115-PC1_30IRC-F16.bin \
	--secondary_sensor_iq_bin sc3338_FKO1_30IRC-F16.bin

make_meta --update --meta_path /path-to-meta \
	--secondary_sensor_iq_bin sc3338_FKO1_30IRC-F16.bin

make_meta --update --meta_path /path-to-meta \
	--sensor_iq_bin ./sc230ai_CMK-OT2115-PC1_30IRC-F16.bin

也可以同时更新并且支持修改其他参数
make_meta --update --meta_path /path-to-meta \
	--sensor_init ./init_sensor.bin \
	--ae_awb_tab ./ae_awb_table.bin \
	--sensor_iq_bin ./sc230ai_CMK-OT2115-PC1_30IRC-F16.bin \
	--rk_led_value 23

dump meta数据：
make_meta -d /path-to-meta

恢复meta数据（IQ bin不支持）
make_meta -r /path-to-meta

注意：
1. /path-to-meta参数是meta分区的设备地址
2. 可以通过cat /proc/cmdline 查看meta分区，如下meta分区在第5个分区
	mtdparts=sfc_nor:64K(env),256K@64K(idblock),256K(uboot),64K(misc),384K(meta),8M(boot),3584K(recovery),3M(userdata)
3. 对于双摄方案，meta分区大小要改为704KB

对于spi nor，第5个分区节点是/dev/mtdblock4
对于emmc, 第5个分区节点是/dev/mmcblk0p5

3. 如何新增sensor支持
a. 拷贝新IQ bin文件到 sensor_iqfile_bin/ 目录
b. 参考其他sensor，在 sensor_ae_awb_table和 sensor_init/目录增加对应的文件
c. 在build_meta.sh 里增加支持 support_sensors="sc230ai sc3338"

