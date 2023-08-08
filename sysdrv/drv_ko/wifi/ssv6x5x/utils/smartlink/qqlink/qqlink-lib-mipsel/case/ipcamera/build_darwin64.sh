test -e libtxdevicesdk.so && rm libtxdevicesdk.so
test -e ipcdemo && rm ipcdemo

test -e libcrypto.1.0.0.dylib && rm libcrypto.1.0.0.dylib
test -e libcrypto.dylib && rm libcrypto.dylib
test -e libssl.1.0.0.dylib && rm libssl.1.0.0.dylib
test -e libssl.dylib && rm libssl.dylib


make -C ../../txdevicesdk/build PLATFORM=darwin64

gcc './alarm.c' './audiofile.c' './audiovideo.c' './ota.c' 'ipcrotate.c' './main.c' -o ipcdemo -I'../../txdevicesdk/sdkimpl/include' -L'../../txdevicesdk/build/darwin64/lib' -O0 -g3 -ltxdevicesdk -lpthread -ldl -m64 -std=c99

cp ../../txdevicesdk/third_part/openssl-osx/lib/libcrypto.1.0.0.dylib ../../txdevicesdk/third_part/openssl-osx/lib/libcrypto.dylib ../../txdevicesdk/third_part/openssl-osx/lib/libssl.1.0.0.dylib ../../txdevicesdk/third_part/openssl-osx/lib/libssl.dylib ./