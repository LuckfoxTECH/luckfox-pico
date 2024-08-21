global-incdirs-y += include
srcs-y += $(wildcard *.c)
srcs-y += $(wildcard ../rk_public_api/*.c)
cflags-y += -Wno-error
