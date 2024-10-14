target_sources(utils_obj
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/dma_alloc.cpp
        ${CMAKE_CURRENT_LIST_DIR}/drm_alloc.cpp
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include/dma_alloc.h
        ${CMAKE_CURRENT_LIST_DIR}/include/drm_alloc.h
)

target_include_directories(utils_obj
    PUBLIC
        ${THIRD_PARTY_PATH}/libdrm/include
        ${THIRD_PARTY_PATH}/libdrm/include/libdrm
        ${CMAKE_CURRENT_LIST_DIR}/include
)
