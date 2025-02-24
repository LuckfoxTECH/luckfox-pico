##PROJECT(uac)

include_directories(uac/src)
include_directories(uac/src/include)

option(UAC_GRAPH "uac open graph" OFF)
option(UAC_MPI   "uac open mpi" ON)

if (${UAC_GRAPH})
    add_definitions(-DUAC_GRAPH)
    set(SOURCE_FILES_GRAPH
        ${CMAKE_CURRENT_LIST_DIR}/src/graph/graph_control.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/graph/uac_control_graph.cpp
    )
    message(STATUS "Build With Rockit Graph")
else()
    message(STATUS "Build None Rockit Graph")
endif()

if (${UAC_MPI})
    add_definitions(-DUAC_MPI)
    set(SOURCE_FILES_MPI
        ${CMAKE_CURRENT_LIST_DIR}/src/mpi/uac_control_mpi.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/mpi_common/uac_amixer.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/mpi_common/mpi_control_common.cpp
    )
    message(STATUS "Build With Rockit Mpi")
else()
    message(STATUS "Build None Rockit Mpi")
endif()

set(UAC_SOURCE
    ${CMAKE_CURRENT_LIST_DIR}/src/uac_uevent.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/uac_control.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/uac_common_def.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/uac_control_factory.cpp
    ${SOURCE_FILES_GRAPH}
    ${SOURCE_FILES_MPI}
)

#set(UAC_DEPENDENT_LIBS
#    pthread
#    rockit
#)

#set(UAC_AUDIO_ALGORITHM
#    ${CMAKE_CURRENT_LIST_DIR}/libs/libaec_bf_process.so)
#install(FILES ${UAC_AUDIO_ALGORITHM} DESTINATION lib)

#ADD_EXECUTABLE(uac_app ${UAC_SOURCE})
#target_link_libraries(uac_app ${UAC_APP_DEPENDENT_LIBS})

install(DIRECTORY ./uac DESTINATION include
        FILES_MATCHING PATTERN "*.h")

option(ENABLE_DEMO_BOARD  "use demo board conf" OFF)
if (${ENABLE_DEMO_BOARD})
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/configs/demo/ DESTINATION share/uac_app FILES_MATCHING PATTERN "*.json")
else()
    install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/configs/ DESTINATION share/uac_app FILES_MATCHING PATTERN "configs_skv.json")
endif()
