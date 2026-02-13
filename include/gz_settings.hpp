#pragma once

#include "gz.hpp"

#include <Game/Game.hpp>
#include <System/OverlayManager.hpp>
#include <types.h>

#define MAX_POS_SLOTS 8

class GZSettings {
public:
    u8 mSaveIndex; // save file to use
    bool mFasterTitleScreen; // goes into the "touch or start to exit" state immediately
    bool mSkipTitleScreen; // same as above except it jump straight into the file select without requiring any input
    Vec3p mLandPosSlots[8];
    Vec3p mTrainPosSlots[8];
    s8 mPositionIndex;

    GZSettings();
    ~GZSettings();

    bool IsStartUp() {
        return gGZ.prevGameModeOvl == OverlayIndex_StartUp;
    }

    Vec3p* GetPosArray() {
        if (gGZ.IsOnLand()) {
            return this->mLandPosSlots;
        }

        return this->mTrainPosSlots;
    }

    void ProcessTitleScreen();
};

extern GZSettings gSettings;
