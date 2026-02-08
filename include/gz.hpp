#pragma once

#include "mem.hpp"

#include <Player/TouchControl.hpp>
#include <nitro/button.h>
#include <Unknown/UnkStruct_02049b18.hpp>
#include <Unknown/Common.hpp>
#include <System/OverlayManager.hpp>

class GameGZ;

struct GZState {
    bool isPaused;

    GZState() {
        this->isPaused = false;
    }
};

class GZ {
public:
    Input* mpButtons;
    TouchControl* mpTouchControl;
    GZState mState;
    OverlayIndex prevGameModeOvl;
    // UnkSystem2_UnkSubSystem5 mTest;

    GZ() : mpButtons(&data_02049b18.mButtons), mpTouchControl(&data_02049b18.mUnk_06.mTouchControl), prevGameModeOvl(OverlayIndex_None) /* , mTest(0x89, 0x01) */ {
        // mTest.func_0201f730(0x00000001);
    }
    ~GZ() {}

    // global init
    void Init();

    // global update
    void Update();

    // called on new game mode init
    void OnGameModeInit();

    // called on game mode update
    void OnGameModeUpdate();
};

extern GZ gGZ;
