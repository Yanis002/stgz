#pragma once

#include <types.h>

typedef void (*GZAction)(u32 params);
typedef bool (*GZCheckCallback)(int itemIndex);

// from oot-gc
extern "C" char* strcat(char* dst, const char* src);
