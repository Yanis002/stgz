#pragma once

#include "gz.hpp"

#include <Game/Game.hpp>
#include <System/OverlayManager.hpp>
#include <types.h>

typedef enum GZBootMode {
    /* 0 */ BOOT_MODE_TITLE,
    /* 0 */ BOOT_MODE_FILE_SELECT,
    /* 1 */ BOOT_MODE_ADVENTURE,
} GZBootMode;

class GZSettings {
public:
    GZBootMode bootMode; // where we should jump after the startup screen
    u8 saveIndex; // save file to use

    GZSettings();
    ~GZSettings();

    bool IsStartUp() {
        return gGZ.prevGameModeOvl == OverlayIndex_StartUp;
    }

    void ProcessBootMode();
};

extern GZSettings gSettings;
