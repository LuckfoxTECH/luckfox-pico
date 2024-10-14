This is a simple memory benchmark program, which tries to measure the peak
bandwidth of sequential memory accesses and the latency of random memory
accesses. Bandwidth is measured by running different assembly code for
the aligned memory blocks and attempting different prefetch strategies.

The benchmark results for some hardware can be found in the wiki page:
    https://github.com/ssvb/tinymembench/wiki

This program can be compiled in either linux or windows (via mingw32 and msys)
by simply running make:
    $ make

Adding extra optimization options is possible (in linux):
    $ CFLAGS="-O2 -march=atom -mtune=atom" make

Example of crosscompiling for ARM (also in linux):
    $ CC=arm-linux-gnueabihf-gcc CFLAGS="-O2 -mcpu=cortex-a9" make

Example of crosscompiling and running the benchmark on android device:
    $ CC=arm-linux-gnueabihf-gcc CFLAGS="-O2 -mcpu=cortex-a8 -static" make
    $ adb push tinymembench /data/local/tmp/tinymembench
    $ adb shell /data/local/tmp/tinymembench
