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
in%dw%d-h%d-%s-afbc.bin
out%dw%d-h%d-%s-afbc.bin

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

| 配置                         | 说明                                                         |
| :--------------------------- | :----------------------------------------------------------- |
| IM2D_SLT_THREAD_EN           | 使能该配置后，将使能多线程模式，每个case都单独在一个线程运行。 |
| IM2D_SLT_THREAD_MAX          | 多线程模式有效，配置最大的线程数量。                         |
| IM2D_SLT_WHILE_EN            | 使能该配置后，将使能测试case循环模式。                       |
| IM2D_SLT_WHILE_NUM           | 循环模式有效，测试case循环次数。                             |
| IM2D_SLT_DRM_BUFFER_EN       | 使能该配置后，测试工具的内存分配器将选择DRM。                |
| IM2D_SLT_GRAPHICBUFFER_EN    | 使能该配置后，测试工具的内存分配器将选择Gralloc。            |
| IM2D_SLT_RK_DMA_HEAP_EN      | 使能该配置后，测试工具的内存分配器将选择rk_dma_deap。        |
| IM2D_SLT_BUFFER_CACHEABLE    | 使能该配置后，测试工具将申请cacheable的内存。                |
| IM2D_SLT_BUFFER_PHY_EN       | 使能该配置后，测试工具将申请使用物理地址。                   |
| IM2D_SLT_TEST_RGA2_EN        | 使能该配置后，将使能RGA2 拷贝测试case。                      |
| IM2D_SLT_TEST_RGA3_0_EN      | 使能该配置后，将使能RGA3 core0 拷贝测试case。                |
| IM2D_SLT_TEST_RGA3_1_EN      | 使能该配置后，将使能RGA3 core1 拷贝测试case。                |
| IM2D_SLT_TEST_RGA3_0_FBC_EN  | 使能该配置后，将使能RGA3 core0 FBC模式 拷贝测试case。        |
| IM2D_SLT_TEST_RGA3_1_FBC_EN  | 使能该配置后，将使能RGA3 core1 FBC模式 拷贝测试case。        |
| IM2D_SLT_DEFAULT_WIDTH       | 默认的图像宽。                                               |
| IM2D_SLT_DEFAULT_HWIGHT      | 默认的图像高。                                               |
| IM2D_SLT_DEFAULT_FORMAT      | 默认的图像格式。                                             |
| IM2D_SLT_DEFAULT_INPUT_PATH  | 默认的输入图像路径                                           |
| IM2D_SLT_DEFAULT_OUTPUT_PATH | 默认的输出图像路径                                           |



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
ID[1]: RGA3_core0 imcopy 1 time success!
ID[1]: RGA3_core0 check buffer 1 time success!
ID[1]: RGA3_core0 imcopy 2 time success!
ID[1]: RGA3_core0 check buffer 2 time success!
ID[1]: RGA3_core0 imcopy 3 time success!
ID[1]: RGA3_core0 check buffer 3 time success!
ID[1] RGA3_core0 run end!
-------------------------------------------------
im2d api slt success!
```

