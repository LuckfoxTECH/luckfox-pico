import os
ARCH     = 'risc-v'
CPU      = 'scr1'
# toolchains options
CROSS_TOOL  = 'gcc'

#------- toolchains path -------------------------------------------------------
if os.getenv('RTT_CC'):
    CROSS_TOOL = os.getenv('RTT_CC')

if  CROSS_TOOL == 'gcc':
    PLATFORM    = 'gcc'
    EXEC_PATH   = '../../../../prebuilts/gcc/linux-x86/riscv64/xpack-riscv-none-embed-gcc-10.2.0-1.2/bin'

if os.getenv('RTT_EXEC_PATH'):
    EXEC_PATH = os.getenv('RTT_EXEC_PATH')

BUILD = 'release'

CORE = 'risc-v'
MAP_FILE = 'rtthread.map'
LINK_FILE = './link.lds'
TARGET_NAME = 'rtthread.bin'

#------- GCC settings ----------------------------------------------------------
if PLATFORM == 'gcc':
    # toolchains
    PREFIX = 'riscv-none-embed-'
    CC = PREFIX + 'gcc'
    AS = PREFIX + 'gcc'
    AR = PREFIX + 'ar'
    LINK = PREFIX + 'gcc'
    TARGET_EXT = 'elf'
    SIZE = PREFIX + 'size'
    OBJDUMP = PREFIX + 'objdump'
    OBJCPY = PREFIX + 'objcopy'

    DEVICE = ' -march=rv32imc -mabi=ilp32 -DUSE_PLIC -DUSE_M_TIME -DNO_INIT -mcmodel=medany -msmall-data-limit=8 -L. -nostartfiles  -lc'
    CFLAGS = DEVICE + ' -std=gnu99 -g'
    CFLAGS += ' -ffunction-sections -fdata-sections -save-temps=obj -D__RT_THREAD__'
    CFLAGS += ' -Wformat=2 -Wall'
    CFLAGS += ' -Wno-unused-parameter -Wno-unused-function -Wno-unused-value'
    CFLAGS += ' -Werror=uninitialized -Werror=maybe-uninitialized'
    CFLAGS += ' -Wstack-usage=1024'
    CFLAGS += ' -Wno-implicit-fallthrough -Wno-cast-function-type -Wno-missing-field-initializers'
    AFLAGS = DEVICE + ' -c -x assembler-with-cpp -D__ASSEMBLY__'
    LFLAGS = DEVICE + ' --specs=nano.specs -Wl,--gc-sections,-cref,-Map=' + MAP_FILE
    LFLAGS += ' -T ' + LINK_FILE

    CPATH = ''
    LPATH = ''

    if BUILD == 'debug':
        CFLAGS += ' -O0 -g3'
        AFLAGS += ' -g3'
    else:
        CFLAGS += ' -Os'

    POST_ACTION = OBJCPY + ' -O binary $TARGET ' + TARGET_NAME + '\n'
    POST_ACTION += SIZE + ' $TARGET\n'
