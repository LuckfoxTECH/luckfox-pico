ndk-build clean NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk APP_PLATFORM=android-22
ndk-build NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk APP_PLATFORM=android-22 LDFLAGS="-L../optee_client/libs/armeabi/ -lteec" TA_DEV_KIT_DIR=export-user_ta
