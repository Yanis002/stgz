#include "gz.hpp"
#include "gz_game.hpp"

extern "C" int __aeabi_atexit(void*, void (*)(void*), void*) { return 0; }

// this function is called by the init hook, see `GZ_InitHook`
extern "C" void GZ_Init() {
    gGZ.Init();
}

// this function is called by the main hook, see `GameReplacement::Run`
extern "C" void GZ_Main(int param1) {
    ((CustomGame*)&data_02049a2c)->Run(param1);
}
