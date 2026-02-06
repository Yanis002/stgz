#include "gz.hpp"

extern "C" void* __dso_handle = (void*)&__dso_handle;
extern "C" int __aeabi_atexit(void*, void (*)(void*), void*) { return 0; }

// this function is called by the init hook, see `GZ_InitHook` in hook
extern "C" void GZ_Init() {
    gGZ.Init();
}

// this function is called by the update hook, see `GZ_UpdateHook` in hook
extern "C" void GZ_Update() {
    gGZ.Update();
}
