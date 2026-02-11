#pragma once

#include "gz.hpp"

#include <Game/Game.hpp>
#include <System/OverlayManager.hpp>
#include <types.h>

class GZSettings {
public:
    u8 mSaveIndex; // save file to use
    bool mFasterTitleScreen; // goes into the "touch or start to exit" state immediately
    bool mSkipTitleScreen; // same as above except it jump straight into the file select without requiring any input

    GZSettings();
    ~GZSettings();

    bool IsStartUp() {
        return gGZ.prevGameModeOvl == OverlayIndex_StartUp;
    }

    void ProcessTitleScreen();
};

extern GZSettings gSettings;
