#include "System/Random.hpp"

extern "C" {

// this function is called by the init hook, see `GZ_InitHook` in hook
void GZ_Init() {}

// this function is called by the update hook, see `GZ_UpdateHook` in hook
void GZ_Update() {
    // *(u32*)0x0218A3B0 = (*(((u32 *) (0x027FF000 | 0xC3C))));
    *(u32*)0x0218B3B0 = gRandom.Next32(0, 128);
}

}
