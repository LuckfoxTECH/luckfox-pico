************************
 RKIVE simulator
 ---------------
 rve.lib version 1.1.0
************************

说明:
1. 目录结构：
  a. Visual Studio 2022工程文件位于以下目录：
     rkive_simulator/rkive_simulator.sln
  b. rve.lib库文件位于以下目录:
     lib/win/x86/rve.lib
  c. 资源文件位于data目录

2. 建议OpenCV版本：2.4.13.4
  a. windows版本可前往官网下载解压：https://opencv.org/releases/
  b. 安装后配置环境变量OPENCV_DIR，指向OpenCV软件包的build目录
  c. simulator工程默认使用x86 vc14软件库

3. 工程可模拟RKIVE 22个算子，在项目->属性->调试->命令参数中配置算子名称，可配置的命令参数如下：
gmm, matchupdatebg, canny, ccl, stcorner, lkopticalflow, integral, lbp, filter, sobel, dma, csc, eqhist, logicop, magandang, map, morph, ncc, cast, sad, threshold, median

ChangeLog:
----------v1.1.0------------
Release date: 2022-05-07
Expired date: 2024-12-31
1. First release