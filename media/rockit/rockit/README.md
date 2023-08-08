# README

## 前言

**概述**

Rockchip多媒体处理平台(rockit)对外应用接口说明。

---

## 目录

[TOC]

---
## Rockit对外接口说明
Rockit对外提供TGI(TaskGraph Interface)和MPI(Media Process Interface)两套接口，用于应用程序开发，两套接口可单独使用，也可结合使用。

### TGI
- 基于TGI接口可以通过配置文件方式构建应用Pipeline，详见tgi/doc/目录文档说明。
- 适用于UVC、UAC等较为固定Pipeline应用场景。
- 已应用于RV1109/1126 RK356X UVC、UAC类产品。

### MPI

- MPI接口是模块化接口，包括VI/VO/VDEC/VENC/VPSS/VGS/TDE/SYS/MB/AI/AO/ADEC/AENC等模块，详见mpi/doc/目录文档说明。
- MPI支持单模块使用，也支持多模块组合使用，可采用模块间绑定或者手动取/送流方式，灵活构建应用Pipeline。

- 已应用于RV1109/1126 IPC、RK356X NVR类产品，并在后续RK3588等新芯片上持续支持。

## Rockit对外接口特点

|          | TGI                    | MPI                  |
| -------- | ---------------------- | -------------------- |
| 灵活性   | Pipeline构建后较为固定 | 灵活，模块可灵活拼接 |
| 使用场景 | UVC、UAC               | NVR、IPC             |

## Rockit接口编译配置

SDK默认仅开启TGI接口头文件、动态库安装，可根据产品应用需求，通过修改external/rockit目录下CMakeLists.txt，将USE_ROCKIT_MPI置为ON，打开MPI接口编译安装。

```c
option(USE_ROCKIT_TGI  "enable rockit tgi" ON)
if (${USE_ROCKIT_TGI})
    message("Build rockit tgi")
    add_subdirectory(tgi)
endif()

option(USE_ROCKIT_MPI  "enable rockit mpi" OFF)
if (${USE_ROCKIT_MPI})
    message("Build rockit mpi")
    add_subdirectory(mpi)
endif()
```

