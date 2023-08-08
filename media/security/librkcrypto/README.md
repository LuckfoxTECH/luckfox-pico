# librkcrypto

librkcrypto提供基于硬件的算法接口，支持使用DMA的方式对数据进行计算，可用于各种加解密、认证等场景。

librkcrypto依赖于kernel crypto驱动实现，驱动开发和应用API开发请参考文档`Rockchip_Developer_Guide_Crypto_HWRNG_CN.pdf`。

## 版本号查询

通过以下方式查询API版本号。

- **strings命令查询**

```bash
# 以linux平台64位为例
$ strings /lib64/librkcrypto.so |grep api |grep version
rkcrypto api version 1.2.0
```

- **日志打印**

当每个进程首次调用librkcrypto时，会打印版本号

```bash
RKCRYPTO I[rk_crypto_init, 262]: rkcrypto api version 1.2.0
```

## 适用芯片平台

RK3588 | RK356x | RV1109 | RV1126 | RK3326/PX30 | RK3308 | RV1106/03

部分API不适用于部分芯片平台，详见应用开发说明文档。

## 版本依赖

### V1.2.0

kernel-4.19 需更新至以下提交：

```shell
c255a0aa097a crypto: rockchip: rk3326/px30 add aes gcm support
```

kernel-5.10 需更新至以下提交：

```shell
47e85085826d crypto: rockchip: rk3326/px30 add aes gcm support
```

### V1.1.0

kernel-4.19 需更新至以下提交：

```shell
1e549d833bc3 crypto: rockchip: v2: ahash init/update/final use hardware crypto
```

kernel-5.10 需更新至以下提交：

```shell
4d2020372e7e crypto: rockchip: v2: ahash fix hash_algo2name setting error.
```

若需要使用 otp key 加解密功能，rkbin需更新至以下提交：

1. RK3588

   ```shell
   23ca562 rk3588: bl32: update version to v1.07
   ```

2. RK356x

   ```shell
   86e9bb7 rk3568: bl32: update version to v2.07
   ```

3. RV1109/RV1126

   ```shell
   42eea81 rv1126: tee: update version to v2.05
   ```

## 目录说明

**demo:** API使用示例

**docs:** 应用开发说明文档

**include:** 头文件

**src:** 用户空间的驱动及API实现

**test:** API测试代码

**third_party:** 第三方开源代码

## 编译说明

### Android

- **编译lib、test、demo**

```bash
# 在android工程目录下执行
$ source build/envsetup.sh
$ lunch rk3588_s-userdebug    # 以RK3588为例

# cd 到librkcypto目录，执行
$ mm
```

编译成功后，根据配置的芯片平台编译出32-bit或64-bit目标文件librkcrypto.so、librkcrypto_test、librkcrypto_demo。Android编译日志将打印目标文件所在的目录。

### Linux

- **编译lib、test**

```bash
# 在linux工程中的librkcrypto目录下执行
$ ./build.sh       # 编译32-bit和64-bit
$ ./build.sh 32    # 只编译32-bit
$ ./build.sh 64    # 只编译64-bit
```

编译成功后，在librkcrypto/out/target目录生成目标文件librkcrypto.so、librkcrypto.a、librkcrypto_test。

- **编译demo**

```bash
# 在librkcrypto工程目录下执行
$ cd demo
$ make 32    # 或 $ make，只编译32-bit
$ make 64    # 只编译64-bit
$ make clean # 清除目标文件
```

编译成功后，在librkcrypto/demo目录生成目标文件librkcrypto_demo。

## 使用说明

- **头文件**

以下是外部程序调用librkcrypto API所需的头文件。

```c
#include "rkcrypto_common.h"     // 通用
#include "rkcrypto_core.h"       // 调用cipher、hash、hmac等接口时引用
#include "rkcrypto_mem.h"        // 调用支持dma_fd的接口时引用
#include "rkcrypto_otp_key.h"    // 调用otp_key相关接口时引用
#include "rkcrypto_random.h"     // 调用随机数接口时引用
```

- **库文件**

  `librkcrypto.so`

  `librkcrypto.a` (linux平台)

- **日志打印**

  rkcrypto的日志等级划分如下：

  等级1 - TRACE_ERROR：错误信息

  等级2 - TRACE_INFO：常用信息，例如版本号信息等，默认是等级2

  等级3 - TRACE_DEBUG：一般的调试信息

  等级4 - TRACE_VERBOSE：冗长的调试信息

  可以通过以下方式，按需设置日志等级，设置后将打印当前和低于当前等级的日志，注意设备重启后默认是等级2：

  - 使用rkcrypto提供的API：

  ```c
  RK_RES rkcrypto_set_trace_level(enum RKCRYPTO_TRACE_LEVEL level);
  ```

  - 使用指令：

  ```shell
  # Android
  setprop vendor.rkcrypto.trace.level 1/2/3/4

  # Linux
  export rkcrypto_trace_level=1/2/3/4
  ```

- **应用开发说明文档**

  `Rockchip_Developer_Guide_Crypto_HWRNG_CN.pdf`

## FAQ

- **编译链依赖**

  CMake中默认编译链为gcc 10.3版本，即`gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf`和`gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu`。

  若没有该编译链版本，编译时会报类似如下错误：

  ```makefile
  make[2]: librkcrypto/../../prebuilts/gcc/linux-x86/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin/arm-none-linux-gnueabihf-gcc: Command not found
  ```

  可以修改CMakeLists.txt中的编译链路径和版本：

  ```makefile
  set (TOOLCHAIN_PREBUILTS "${CMAKE_CURRENT_SOURCE_DIR}/../../prebuilts")
  set (TOOLCHAIN_PATH_ARM32 "gcc/linux-x86/arm/gcc-arm-10.3-2021.07-x86_64-arm-none-linux-gnueabihf/bin")
  set (TOOLCHAIN_PATH_AARCH64 "gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin")
  ```

  若运行时出现如下报错，则是编译链GLIBC版本与设备上GLIBC版本不一致，请修改编译链版本或者修改设备GLIBC版本。

  ```shell
  version 'GLIBC_2.29' not found (required by /lib/librkcrypto.so)
  ```

