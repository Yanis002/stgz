#pragma once

#include "mem.hpp"
#include "gz_menu.hpp"

#include <mem.h>
#include <Player/TouchControl.hpp>
#include <nitro/button.h>
#include <Unknown/UnkStruct_02049b18.hpp>
#include <Unknown/Common.hpp>
#include <System/OverlayManager.hpp>
#include <Unknown/UnkStruct_02049b74.hpp>
#include <Unknown/UnkStruct_0204a110.hpp>

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

    GZ() : mpButtons(&data_02049b18.mButtons), mpTouchControl(&data_02049b18.mUnk_06.mTouchControl), prevGameModeOvl(OverlayIndex_None) {}
    ~GZ() {}

    void UpdateInputs() {
        data_02049b74.func_02013a44(data_0204a110.mUnk_004);
        data_02049b18.func_02013840(data_0204a110.mUnk_004, data_0204a110.func_02019300(data_0204a110.mUnk_DF8));
    }

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
