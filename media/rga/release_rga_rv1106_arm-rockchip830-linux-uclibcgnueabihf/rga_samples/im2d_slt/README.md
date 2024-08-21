# RGA模块SLT测试工具



## 概述

该工具用于芯片SLT阶段使用，可以通过配置实现对RGA模块硬件进行测验。



## 使用说明

### 前置条件

#### 图像准备

由于RGA硬件执行需要外部输入图像进行处理，所以需要提前准备对应的图片并确保将图像存入设备指定目录。

图像存储的目录可以在slt_config.h中进行配置，文件命名规则如下：

- Raster mode

```
in%dw%d-h%d-%s.bin
out%dw%d-h%d-%s.bin

示例：
1280×720 RGBA8888的输入图像： in0w1280-h720-rgba8888.bin
1280×720 RGBA8888的输出图像： out0w1280-h720-rgba8888.bin
```

- FBC mode

```
in%dw%d-h%d-%s-fbc.bin
out%dw%d-h%d-%s-fbc.bin

示例：
1280×720 RGBA8888的输入图像： in0w1280-h720-rgba8888-afbc.bin
1280×720 RGBA8888的输出图像： out0w1280-h720-rgba8888-afbc.bin
```

> 参数解释如下：
>
> 输入文件为 in ， 输出文件为 out
> --->第一个%d 是文件的索引， 一般为 0， 用于区别格式及宽高完全相同的文件
> --->第二个%d 是宽的意思， 这里的宽一般指虚宽
> --->第三个%d 是高的意思， 这里的高一般指虚高
> --->第四个%s 是格式的名字。



#### 内存限制

由于RGA2硬件不支持大于32位的物理地址，所以建议测试环境保证地址映射在0~4G地址空间以内。



#### 编译配置

在工具编译前，可以通过修改 slt_config.h 配置测试工具，该文件配置可以参考chip_config中的template进行替换使用。

> slt配置

| 配置                | 说明                                                         |
| ------------------- | ------------------------------------------------------------ |
| IM2D_SLT_THREAD_EN  | 使能该配置后，将使能多线程模式，每个case都单独在一个线程运行。 |
| IM2D_SLT_THREAD_MAX | 多线程模式有效，配置最大的线程数量。                         |
| IM2D_SLT_WHILE_EN   | 使能该配置后，将使能测试case循环模式。                       |
| IM2D_SLT_WHILE_NUM  | 循环模式有效，测试case循环次数。                             |

> raster-test配置

| 配置                    | 说明                                          |
| ----------------------- | --------------------------------------------- |
| IM2D_SLT_TEST_RGA2_0_EN | 使能该配置后，将使能RGA2core0拷贝测试case。   |
| IM2D_SLT_TEST_RGA2_0_EN | 使能该配置后，将使能RGA2core1拷贝测试case。   |
| IM2D_SLT_TEST_RGA3_0_EN | 使能该配置后，将使能RGA3 core0 拷贝测试case。 |
| IM2D_SLT_TEST_RGA3_1_EN | 使能该配置后，将使能RGA3 core1 拷贝测试case。 |

> special-test配置

| 配置                              | 说明                                                        |
| --------------------------------- | ----------------------------------------------------------- |
| IM2D_SLT_TEST_SPECIAL_EN          | 使能该配置后，将使能special-test。                          |
| IM2D_SLT_TEST_RGA2_0_TILE_EN      | 使能该配置后，将使能RGA2 core0 TILE4x4模式 拷贝测试case。   |
| IM2D_SLT_TEST_RGA2_0_AFBC32x8_EN  | 使能该配置后，将使能RGA2 core0 AFBC32x8模式 拷贝测试case。  |
| IM2D_SLT_TEST_RGA2_0_RKFBC64x4_EN | 使能该配置后，将使能RGA2 core0 RKFBC64x4模式 拷贝测试case。 |
| IM2D_SLT_TEST_RGA2_1_TILE_EN      | 使能该配置后，将使能RGA2 core1 TILE4x4模式 拷贝测试case。   |
| IM2D_SLT_TEST_RGA2_1_AFBC32x8_EN  | 使能该配置后，将使能RGA2 core1 AFBC32x8模式 拷贝测试case。  |
| IM2D_SLT_TEST_RGA2_1_RKFBC64x4_EN | 使能该配置后，将使能RGA2 core1 RKFBC64x4模式 拷贝测试case。 |
| IM2D_SLT_TEST_RGA3_0_FBC_EN       | 使能该配置后，将使能RGA3 core0 FBC模式 拷贝测试case。       |
| IM2D_SLT_TEST_RGA3_1_FBC_EN       | 使能该配置后，将使能RGA3 core1 FBC模式 拷贝测试case。       |

> perf-test

| 配置                  | 说明                            |
| --------------------- | ------------------------------- |
| IM2D_SLT_TEST_PERF_EN | 使能该配置后，将使能perf-test。 |

> 环境配置

| 配置                         | 说明                                                     |
| :--------------------------- | :------------------------------------------------------- |
| IM2D_SLT_DEFAULT_WIDTH       | 默认的图像宽。                                           |
| IM2D_SLT_DEFAULT_HWIGHT      | 默认的图像高。                                           |
| IM2D_SLT_DEFAULT_FORMAT      | 默认的图像格式。                                         |
| IM2D_SLT_DMA_HEAP_PATH       | 默认的dma-buf heap路径。（用于分配slt需要使用的dma-buf） |
| IM2D_SLT_DEFAULT_INPUT_PATH  | 默认的输入图像路径。                                     |
| IM2D_SLT_DEFAULT_OUTPUT_PATH | 默认的输出图像路径。                                     |



### golden获取

#### 生成golden

在RGA正常的环境下，通过配置golden相关配置，启动获取golden数据的运行模式，获取的golden数据将输出到对应配置的路径下。

| 配置                                | 说明                                     |
| ----------------------------------- | ---------------------------------------- |
| IM2D_SLT_GENERATE_CRC               | 使能该配置后，运行模式切换为生成golden。 |
| IM2D_SLT_GENERATE_CRC_GOLDEN_PREFIX | golden数据的可自定义前缀。               |
| IM2D_SLT_DEFAULT_GOLDEN_PATH        | 默认的golden文件路径。                   |



### 编译

#### Android

在配置好Android SDK编译环境后，在源码目录下使用如下命令编译即可。

```
mm
```



#### Linux

- cmake

  - 修改librga源码根目录下的**/cmake/buildroot.cmake**文件。执行以下脚本完成编译:

  ```
  $ chmod +x ./cmake-linux.sh
  $ ./cmake-linux.sh
  ```

   **[编译选项]**

  1. 指定TOOLCHAIN_HOME为交叉编译工具的路径
  2. 指定CMAKE_C_COMPILER为gcc编译命令的路径
  3. 指定CMAKE_CXX_COMPILER为g++编译命令的路径

  - 前级librga目录未编译时，需要修改librga.so的链接路径：

  ```
  vim CmakeList.txt +82
  修改 target_link_libraries(im2d_slt librga.so）路径
  ```



### 工具运行

将编译生成的im2d_slt 推入设备后，直接运行即可。



## 结果说明

如果出现以下日志，则说明测试成功。

```
:/ # im2d_slt
-------------------------------------------------
creat Sync pthread[0x7605a80cb0] = 1, id = 1
creat Sync pthread[0x7604982cb0] = 2, id = 2
ID[1]: RGA2_core0 running success!
ID[2]: RGA2_core1 running success!
-------------------------------------------------
RGA raster-test success!
-------------------------------------------------
creat Sync pthread[0x7605a80cb0] = 1, id = 1
creat Sync pthread[0x7605982cb0] = 2, id = 2
creat Sync pthread[0x7604884cb0] = 3, id = 3
creat Sync pthread[0x7603786cb0] = 4, id = 4
creat Sync pthread[0x7602688cb0] = 5, id = 5
creat Sync pthread[0x76013c7cb0] = 6, id = 6
ID[6]: RGA2_core1_rkfbc64x4 running success!
ID[3]: RGA2_core0_rkfbc64x4 running success!
ID[2]: RGA2_core0_afbc32x8 running success!
ID[5]: RGA2_core1_afbc32x8 running success!
ID[4]: RGA2_core1_tile4x4 running success!
ID[1]: RGA2_core0_tile4x4 running success!
-------------------------------------------------
RGA special-test success!
-------------------------------------------------
creat Sync pthread[0x7605a80cb0] = 1, id = 1
creat Sync pthread[0x7605982cb0] = 2, id = 2
creat Sync pthread[0x7604884cb0] = 3, id = 3
creat Sync pthread[0x7602786cb0] = 4, id = 4
creat Sync pthread[0x7601688cb0] = 5, id = 5
creat Sync pthread[0x760158acb0] = 6, id = 6
creat Sync pthread[0x7600108cb0] = 7, id = 7
creat Sync pthread[0x75fe57ecb0] = 8, id = 8
creat Sync pthread[0x75fdd78cb0] = 9, id = 9
ID[1]: perf_test running success!
ID[6]: perf_test running success!
ID[3]: perf_test running success!
ID[9]: perf_test running success!
ID[2]: perf_test running success!
ID[7]: perf_test running success!
ID[5]: perf_test running success!
ID[4]: perf_test running success!
ID[8]: perf_test running success!
-------------------------------------------------
RGA perf-test success!
-------------------------------------------------
```

