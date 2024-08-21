cppflags-$(WITH_TLS_TESTS) += -DWITH_TLS_TESTS=1
global-incdirs-y += include
srcs-y += os_test_lib.c
ifneq ($(COMPILER),clang)
srcs-y += os_test_lib_cxx.cpp
endif
