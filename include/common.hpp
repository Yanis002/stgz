#pragma once

#include <types.h>

typedef void (*GZAction)(u32 params);
typedef bool (*GZCheckCallback)(int itemIndex);

// from oot-gc
extern "C" char* strcat(char* dst, const char* src);

extern "C" void DisplayDebugText(int, void*, int, int, const char*);
extern "C" void DisplayDebugTextF(int, void*, int, int, const char*, ...);
extern "C" void GXS_SetGraphicsMode(int);
extern "C" void func_0201b180(bool, bool);
extern "C" void func_02027654(void*, int); // DC_FlushRange
extern "C" void func_020252ec(void*, int, int); // GX0_LoadBG0Scr
