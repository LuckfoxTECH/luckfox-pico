cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 11)
add_definitions(-DDBUG)
add_definitions(-g)
PROJECT(rk_mpi_uvc)
include(FindPkgConfig)

include_directories(uvc)
include_directories(uvc/cJSON)
include_directories(isp)
include_directories(param)

include_directories(${PROJECT_SOURCE_DIR}/include
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/uAPI2
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/algos
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/common
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/xcore
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser
                                        ${CMAKE_SYSROOT}/usr/include/rkaiq/iq_parser_v2
                                        ${CMAKE_SYSROOT}/usr/include/libdrm
                                        )

option(COMPILE_FOR_UVC_UAC "compile for uac" OFF)
if(COMPILE_FOR_UVC_UAC)
  include(uac/uac.cmake)
endif()

set(LIB_SOURCE
    uvc/uvc-gadget.c
    uvc/uvc_video.cpp
    uvc/uvc_control.cpp
    uvc/uvc_process.cpp
    uvc/uvc_mpi_config.c
    uvc/uvc_mpi_venc.cpp
    uvc/uvc_mpi_vi.cpp
    uvc/uvc_mpi_vpss.cpp
    uvc/uevent.c
    uvc/uvc_configfs.c
    uvc/camera_control.c
    uvc/camera_pu_control.c
    uvc/cJSON/cJSON.c
    uvc/osd.c
    isp/isp.c
    param/dictionary.c
    param/iniparser.c
    param/param.c
)

if(COMPILE_FOR_UVC_UAC)
add_definitions(-DCOMPILE_FOR_UVC_UAC)
set(SOURCE
    main.c
    ${LIB_SOURCE}
    ${UAC_SOURCE}
)

else()

set(SOURCE
    main.c
    ${LIB_SOURCE}
)

endif()

if (RK_ENABLE_FASTBOOT)
set(RK_MPI_UVC_DEPENDENT_LIBS
    pthread
    rt
    rkaiq
    librockit.a
    librockchip_mpp.a
    librkaudio_detect.a
    libaec_bf_process.a
    librga.a
)
    message(STATUS "RK_ENABLE_FASTBOOT is enabled.")
else()
    set(RK_MPI_UVC_DEPENDENT_LIBS
        pthread
        rt
        rkaiq
        rockit
    )
    message(STATUS "RK_ENABLE_FASTBOOT is disabled.")
endif()
set(RK_MPI_UVC_INC
    ${CMAKE_CURRENT_SOURCE_DIR}/uvc/
)

add_definitions(-fno-rtti)

ADD_EXECUTABLE(rk_mpi_uvc ${SOURCE})
link_directories(${PROJECT_SOURCE_DIR}/lib/)
target_link_libraries(rk_mpi_uvc ${RK_MPI_UVC_DEPENDENT_LIBS})

install(DIRECTORY ./uvc DESTINATION include
        FILES_MATCHING PATTERN "*.h")

install(TARGETS rk_mpi_uvc DESTINATION bin)
install(FILES uvc_mpi_cfg.conf DESTINATION share)
install(FILES rkuvc.ini DESTINATION share)
install(FILES usb_config.sh DESTINATION bin
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
        GROUP_READ GROUP_WRITE GROUP_EXECUTE
        WORLD_READ WORLD_WRITE WORLD_EXECUTE)
#install(FILES RkLunch.sh DESTINATION bin
#        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
#        GROUP_READ GROUP_WRITE GROUP_EXECUTE
#        WORLD_READ WORLD_WRITE WORLD_EXECUTE)
#install(FILES RkLunch-stop.sh DESTINATION bin
#        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE
#        GROUP_READ GROUP_WRITE GROUP_EXECUTE
#        WORLD_READ WORLD_WRITE WORLD_EXECUTE)