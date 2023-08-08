1.How to use Makefile of directory media:

(1) Compile the entire media directory:
    make  clean
    make

(2) See make configure
    make  info

(3) Clear all compiled files under media directory:
    make  clean

(4) Compile the module singly：
    for example: compile RKAIQ (isp directory)
        (4.1) confirm cfg/cfg.mk configure CONFIG_RK_RKAIQ=y
        (4.2) cd isp; make clean; make

2. Output directory description
out
├── avs_calib --------------------------------- The calibration data of panoramic stitching
├── bin---------------------------------------- Tests for each module sample
├── include------------------------------------ The header file for each module
├── isp_iqfiles-------------------------------- The IQ file for Camera Sensor
├── lib---------------------------------------- The library file for each module
├── root--------------------------------------- The files which MUST locate on the root filesystem
└── share-------------------------------------- The config file for each module

3. media directory structure：
media
├── alsa-lib----------------------------------- Advanced Linux Sound Architecture (ALSA) library
├── cfg/cfg.mk--------------------------------- Use for different SOC platform
├── common_algorithm--------------------------- Audio 3A algorithm
│                                               Move detection algorithm
│                                               Occlusion detection algorithm
│
├── isp---------------------------------------- Isp image processing algorithm module
├── libdrm------------------------------------- Direct Rendering Manager
├── libv4l------------------------------------- The user level interface of video4linux2 device
├── Makefile----------------------------------- media Makefile
├── Makefile.param----------------------------- media Makefile's config file
├── out---------------------------------------- media compile output directory
├── rga---------------------------------------- RGA is a independent 2D hardware accelerator,
│                                                  used to speed up point and line drawing,
│                                                  perform common 2D graphics operations such as image
│                                                  zooming, rotation, bitBlt, alpha blending, and more
│
├── mpp---------------------------------------- The interface of encoder and decoder
│                                                  use for the rkmedia or the rockit
│                                                  It is not recommended to call mpp directly
│
├── rkmedia------------------------------------ Multimedia processing framework (Applicable to some platforms)
│                                                  encapsulating isp, rga, mpp and other
│                                                  multimedia-related interfaces
│
└── rockit------------------------------------- Multimedia processing framework
                                                the rockit and the rkmedia are two sets of external interfaces

