#include <dlfcn.h>
#include <iostream>
#include <string>

int main() {
    std::cout << "[MAIN] Loading WebRTC module...\n";

    void* handle = dlopen("./libwebrtc_module.so", RTLD_LAZY);
    if (!handle) {
        std::cerr << "Cannot load module: " << dlerror() << std::endl;
        return 1;
    }

    // Lấy function start_webrtc
    typedef void (*start_func_t)();
    start_func_t start_webrtc = (start_func_t)dlsym(handle, "start_webrtc");
    char* error;
    if ((error = dlerror()) != nullptr) {
        std::cerr << "Cannot find start_webrtc: " << error << std::endl;
        return 1;
    }

    std::cout << "[MAIN] Starting WebRTC...\n";
    start_webrtc();  // chạy module

    dlclose(handle);
    return 0;
}
