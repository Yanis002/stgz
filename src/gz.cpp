#include "gz.hpp"

#include <nitro/button.h>
#include <System/Random.hpp>
#include <regs.h>

// extern u32 data_0204a110;
// extern "C" u32 func_01ff9b1c(void*, s16);

GZ gGZ;

// wchar_t str[] = L"Hello World";

void GZ::Init() {
    // mTest.func_0201fb78(str);
}

void GZ::Update() {
    if (CHECK_BUTTON_COMBO(this->mpButtons->press, BTN_A)) {
        if (this->mState.isPaused) {
            this->mState.isPaused = false;
        } else {
            this->mState.isPaused = true;
        }
    }

    if (CHECK_BUTTON_COMBO(this->mpButtons->cur, BTN_B)) {
        this->mState.requestedFrames++;
    }

    if (CHECK_BUTTON_COMBO(this->mpButtons->press, BTN_R)) {
        this->mState.requestedFrames++;
    }

    // if (CHECK_BUTTON_COMBO(this->mpButtons->cur, BTN_B)) {
    //     *(u32*)0x0218A900 = func_01ff9b1c(&data_0204a110, this->mTest.mUnk_008) != 0;
    //     *(u32*)0x0218A904 = this->mTest.mUnk_14C != 0;
    //     *(u32*)0x0218A908 = this->mTest.mUnk_14D != 0;

    //     if (this->mTest.mUnk_14C == 0) {
    //         this->mTest.mUnk_14C = 1;
    //     }
    // }

    // if (CHECK_BUTTON_COMBO(this->mpButtons->press, BTN_SELECT)) {
    //     mTest.func_0201f730(0x00020026);
    //     mTest.func_0201fb78(str);
    // }

    // if (CHECK_BUTTON_COMBO(this->mpButtons->press, BTN_A)) {
    //     mTest.mUnk_140 = 10;
    //     mTest.mUnk_142 = 10;
    //     mTest.func_0201f4b4(0);
    // }

    // if (CHECK_BUTTON_COMBO(this->mpButtons->cur, BTN_L)) {
    //     mTest.func_0201f9c4();

    //     // REG_DISPCNT &= 0xFFFFE0FF;
    //     // REG_DISPCNT |= 0x1C00;
    // }

    // if (CHECK_BUTTON_COMBO(this->mpButtons->cur, BTN_R)) {
    //     mTest.func_0201f498();

    //     // REG_DISPCNT &= 0xFFFFE0FF;
    //     // REG_DISPCNT |= 0x1F00;
    // }
}

void GZ::OnGameModeInit() {

}

void GZ::OnGameModeUpdate() {

}
