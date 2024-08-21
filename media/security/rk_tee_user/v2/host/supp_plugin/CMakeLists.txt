project (f07bfc66-958c-4a15-99c0-260e4e7375dd.plugin C)

set (CFG_TEE_PLUGIN_LOAD_PATH "/usr/lib/tee-supplicant/plugins/")
set (CMAKE_SHARED_LIBRARY_PREFIX "")

add_library(${PROJECT_NAME} SHARED test_supp_plugin.c)
target_include_directories(${PROJECT_NAME} PRIVATE ./include)

install (TARGETS ${PROJECT_NAME} DESTINATION ${CFG_TEE_PLUGIN_LOAD_PATH})
