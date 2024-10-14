global-incdirs-y += include
srcs-y += os_test_lib_dl.c
ifneq ($(COMPILER),clang)
srcs-y += os_test_lib_dl_cxx.cpp
endif
