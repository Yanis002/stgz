#include "gz.hpp"
#include "build.hpp"

#include <nitro/button.h>
#include <System/Random.hpp>
#include <regs.h>

GZ gGZ;

void GZ::Init() {

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
}

void GZ::OnGameModeInit() {

}

void GZ::OnGameModeUpdate() {

}
