test -e libtxdevicesdk.so && rm libtxdevicesdk.so
test -e ipcdemo && rm ipcdemo

test -e libcrypto.so.1.0.0 && rm libcrypto.so.1.0.0
test -e libcrypto.so && rm libcrypto.so
test -e libssl.so.1.0.0 && rm libssl.so.1.0.0
test -e libssl.so && rm libssl.so


make -C ../../txdevicesdk/build PLATFORM=x86

gcc './alarm.c' './audiofile.c' './audiovideo.c' './ota.c' 'ipcrotate.c' './main.c' -o ipcdemo -I'../../txdevicesdk/sdkimpl/include' -L'../../txdevicesdk/build/x86/lib' -O0 -g3 -ltxdevicesdk -lpthread -ldl -m32 -std=c99

cp ../../txdevicesdk/third_part/openssl/lib/libcrypto.so.1.0.0 ../../txdevicesdk/third_part/openssl/lib/libcrypto.so ../../txdevicesdk/third_part/openssl/lib/libssl.so.1.0.0 ../../txdevicesdk/third_part/openssl/lib/libssl.so ./
