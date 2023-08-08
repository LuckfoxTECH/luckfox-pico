include(CMakeDependentOption)

cmake_dependent_option(RKAIQ_ENABLE_ASAN "Build libraries/executable with address santilizer" OFF "WIN32" OFF)
set(RKAIQ_ENABLE_LIBDRM      TRUE  CACHE INTERNAL "Build rkaiq with libdrm")
