# librga IM2D API快速演示DEMO

​	librga IM2D API快速演示DEMO可以通过配置指令快速演示RGA功能。

## 编译说明

​	本仓库示例代码支持CMAKE编译，可以通过修改toolchain_*.cmake文件以及编译脚本实现快速编译。

### 工具链修改

- **Android NDK（build for android）**

​	参考librga源码目录下**toolchains/toolchain_android_ndk.cmake**写法，修改NDK路径、Android版本信息等。

| 工具链选项                          | 描述                                         |
| ----------------------------------- | -------------------------------------------- |
| CMAKE_ANDROID_NDK                   | NDK编译包路径                                |
| CMAKE_SYSTEM_NAME                   | 平台名，默认为Android                        |
| CMAKE_SYSTEM_VERSION                | Android版本                                  |
| CMAKE_ANDROID_ARCH_ABI              | 处理器版本                                   |
| CMAKE_ANDROID_NDK_TOOLCHAIN_VERSION | 工具链选择（clang/gcc）                      |
| CMAKE_ANDROID_STL_TYPE              | NDK C++库的链接方式（c++_static/c++_shared） |

- **Linux（buildroot/debian）**

​	参考librga源码目录下**toolchains/toolchain_linux.cmake**写法，修改工具链路径、名称。

| 工具链选项     | 描述       |
| -------------- | ---------- |
| TOOLCHAIN_HOME | 工具链目录 |
| TOOLCHAIN_NAME | 工具链名称 |

### 编译脚本修改

​	修改samples目录或需要编译的示例代码目录下**cmake_*.sh**，指定toolchain路径。

| 编译选项       | 描述                                                         |
| -------------- | ------------------------------------------------------------ |
| TOOLCHAIN_PATH | toolchain的绝对路径，即《工具链修改》小节中修改后的toolchain_*.cmake文件的绝对路径 |
| LIBRGA_PATH    | 需要链接的librga.so的绝对路径，默认为librga cmake编译时的默认打包路径 |
| BUILD_DIR      | 编译生成文件存放的相对路径                                   |

### 执行编译脚本

- **Android NDK（build for android）**

```bash
$ chmod +x ./cmake_android.sh
$ ./cmake_android.sh
```

- **Linux（buildroot/debian）**

```bash
$ chmod +x ./cmake_linux.sh
$ ./cmake_linux.sh
```

### 使用说明


见docs目录下《Rockchip_Developer_Guide_RGA_CN.md》——测试用例及调试方法 章节。