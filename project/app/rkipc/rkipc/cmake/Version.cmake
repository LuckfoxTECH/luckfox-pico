# ----------------------------------------------------------------------------
# Create git version information
# ----------------------------------------------------------------------------
set(VERSION_CNT         0)
set(VERSION_MAX_CNT     5)
set(VERSION_INFO        "\"unknown rkipc version for missing VCS info\"")
set(BUILD_INFO          "\"unknown rkipc build info\"")
set(RKIPC_TYPE          "\"unknown rkipc type\"")

foreach (CNT RANGE ${VERSION_MAX_CNT})
    set(VERSION_HISTORY_${CNT} "NULL")
endforeach(CNT)

if(COMPILE_FOR_RV1103_IPC STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1103_IPC\"")
endif()
if(COMPILE_FOR_RV1106_IPC STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1106_IPC\"")
endif()
if(COMPILE_FOR_RV1106_BATTERY_IPC_TUYA STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1106_BATTERY_IPC_TUYA\"")
endif()
if(COMPILE_FOR_RV1106_BATTERY_IPC_CLIENT STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1106_BATTERY_IPC_CLIENT\"")
endif()
if(COMPILE_FOR_RV1106_DUAL_IPC STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1106_DUAL_IPC\"")
endif()
if(COMPILE_FOR_RV1106_WAKEUP_IPC STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1106_WAKEUP_IPC\"")
endif()
if(COMPILE_FOR_RV1106_AIISP STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1106_AIISP\"")
endif()

if(COMPILE_FOR_RV1126_RKMEDIA STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1126_RKMEDIA\"")
endif()
if(COMPILE_FOR_RV1126_ROCKIT STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1126_ROCKIT\"")
endif()
if(COMPILE_FOR_RV1126_BATTERY_IPC STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1126_BATTERY_IPC\"")
endif()
if(COMPILE_FOR_RV1126_SNAPSHOT STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1126_SNAPSHOT\"")
endif()
if(COMPILE_FOR_RV1126_DUAL_IPC STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1126_DUAL_IPC\"")
endif()
if(COMPILE_FOR_RV1126_AIISP_IPC STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RV1126_AIISP_IPC\"")
endif()

if(COMPILE_FOR_RK3588 STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RK3588\"")
endif()
if(COMPILE_FOR_RK3588_MULTI_IPC STREQUAL "ON")
    set(RKIPC_TYPE "\"COMPILE_FOR_RK3588_MULTI_IPC\"")
endif()

if(EXISTS "${PROJECT_SOURCE_DIR}/.git")
    find_package(Git)
    if(GIT_FOUND)
        # get current version info
        set(GIT_LOG_FORMAT "git-%h %cd")

        execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --oneline --pretty=format:${GIT_LOG_FORMAT}
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE EXEC_OUT
            ERROR_VARIABLE EXEC_ERROR
            RESULT_VARIABLE EXEC_RET
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE)

        if (NOT EXEC_RET)
            set(VERSION_INFO ${EXEC_OUT})
            message(STATUS "git current version:")
            message(STATUS "${VERSION_INFO}")
            string(REPLACE "\"" "\\\"" VERSION_INFO ${VERSION_INFO})
            set(VERSION_INFO "\"${VERSION_INFO}\"")
        else()
            message(STATUS "git ret ${EXEC_RET}")
            message(STATUS "${EXEC_ERROR}")
        endif()

        # get version build info
        execute_process(COMMAND ${GIT_EXECUTABLE} config user.name
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            OUTPUT_VARIABLE EXEC_OUT
            ERROR_VARIABLE EXEC_ERROR
            RESULT_VARIABLE EXEC_RET
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_STRIP_TRAILING_WHITESPACE)

        if (NOT EXEC_RET)
            set(BUILD_AUTHOR ${EXEC_OUT})
            string(SUBSTRING ${BUILD_AUTHOR} 0 3 BUILD_AUTHOR)
            string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")
            set(BUILD_INFO "built time: ${BUILD_TIME}")
            set(BUILD_INFO "\"${BUILD_INFO}\"")
            message(STATUS "${BUILD_INFO}")
        else()
            message(STATUS "git ret ${EXEC_RET}")
            message(STATUS "${EXEC_ERROR}")
        endif()

        # get history version info
        message(STATUS "git history version:")
        foreach (CNT RANGE ${VERSION_MAX_CNT})
            execute_process(COMMAND ${GIT_EXECUTABLE} log HEAD~${CNT} -1 --oneline --pretty=format:${GIT_LOG_FORMAT}
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
                OUTPUT_VARIABLE EXEC_OUT
                ERROR_VARIABLE EXEC_ERROR
                RESULT_VARIABLE EXEC_RET
                OUTPUT_STRIP_TRAILING_WHITESPACE
                ERROR_STRIP_TRAILING_WHITESPACE)

            if (NOT EXEC_RET)
                set(VERSION_LOG ${EXEC_OUT})
                string(REPLACE "\"" "\\\"" VERSION_LOG ${VERSION_LOG})
                message(STATUS ${VERSION_LOG})
                set(VERSION_HISTORY_${CNT} "\"${VERSION_LOG}\"")
                math(EXPR VERSION_CNT "${VERSION_CNT}+1")
            endif()
        endforeach(CNT)
        message(STATUS "total ${VERSION_CNT} git version generated")

        configure_file(
            "${PROJECT_SOURCE_DIR}/cmake/version.in"
            "${PROJECT_SOURCE_DIR}/common/version.h"
        )
    endif()
else()
    configure_file(
        "${PROJECT_SOURCE_DIR}/cmake/version.in"
        "${PROJECT_SOURCE_DIR}/common/version.h"
    )
endif(EXISTS "${PROJECT_SOURCE_DIR}/.git")