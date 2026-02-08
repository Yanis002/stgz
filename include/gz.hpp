#pragma once

#include "mem.hpp"

#include <mem.h>
#include <Player/TouchControl.hpp>
#include <nitro/button.h>
#include <Unknown/UnkStruct_02049b18.hpp>
#include <Unknown/Common.hpp>
#include <System/OverlayManager.hpp>

class GameGZ;

struct GZState {
    bool isPaused; // pauses the game
    u32 requestedFrames; // how many frames to allow for frame advance
    bool doRNGUpdatesDuringPause; // allow the RNG seed to be updated in `ExecutePause`
    bool isRNGPaused; // stops the RNG seed from updating in the main loop (TODO: entirely stop RNG from updating?)

    GZState() {
        memset(this, 0, sizeof(GZState));
    }
};

class GZ {
public:
    Input* mpButtons;
    TouchControl* mpTouchControl;
    OverlayIndex prevGameModeOvl;
    GZState mState;
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
