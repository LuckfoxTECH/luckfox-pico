
set(RockIva_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR}/include)

if (CMAKE_SYSTEM_NAME MATCHES "Android")
    file(GLOB RockX_LIBS ${CMAKE_CURRENT_LIST_DIR}/${CMAKE_ANDROID_ARCH_ABI}/*.so)
else ()
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set (TARGET_LIB_ARCH lib64)
    else()
        set (TARGET_LIB_ARCH lib)
    endif()
    file(GLOB RockX_LIBS ${CMAKE_CURRENT_LIST_DIR}/${TARGET_LIB_ARCH}/*.so)
endif()

set(RockIva_DATA_DIRS "${CMAKE_CURRENT_LIST_DIR}/../../models/rockiva-data-rv1106")
if(EXISTS ${RockIva_DATA_DIRS})
    file(GLOB ROCKIVA_DATA_FILES ${RockIva_DATA_DIRS})
endif()

set(RockIva_DATA ${ROCKIVA_DATA_FILES})
