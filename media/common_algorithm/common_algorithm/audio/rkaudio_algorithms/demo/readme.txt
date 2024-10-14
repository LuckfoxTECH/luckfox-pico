执行命令：
./test_sed 输入音频文件地址  输出音频文件地址
例如： ./test_sed  ./dbg_capin.wav ./dbg_capin_out.wav

带有outfile的lib  在设定环境变量PATH_ORI_IN ， PATH_NET_IN ，PATH_NET_OUT 后
将在对应位置生成ori_in.wav  net_in.wav  net_out.wav用于分析样例
例如：export PATH_ORI_IN=/USERDATA/ori_in.wav


md5sum
814f25624fd064613b136b35d8db211a  librkaudio_detect.so
99d6bce7d46fd2c6e2fe298f43ef4b02  librkaudio_detect.a
