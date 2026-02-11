#include "gz_commands.hpp"
#include "gz_menu.hpp"
#include "gz.hpp"

#include <Unknown/UnkStruct_027e0478.hpp>
#include <Unknown/UnkStruct_02049b18.hpp>
#include <nitro/math.h>
#include <regs.h>

static void ExecuteLevitate();
static void ExecutePause();
static void ExecuteFrameAdvance();

// commands with default button combos
static GZCmdItem sCommands[] = {
    {"Levitate", ButtonCombo(BTN_X, BTN_NONE), ExecuteLevitate},
    {"Pause/Unpause", ButtonCombo(BTN_NONE, BTN_DDOWN), ExecutePause},
    {"Frame Advance", ButtonCombo(BTN_NONE, BTN_DUP), ExecuteFrameAdvance},
};

static void ExecuteLevitate() {
    data_027e0478.mPlayer.mVel.y = FLOAT_TO_Q20(0.334375f);
}

static void ExecutePause() {
    if (!gGZ.mState.isPaused) {
        gGZ.mState.isPaused = true;
    } else {
        gGZ.mState.isPaused = false;
    }
}

static void ExecuteFrameAdvance() {
    gGZ.mState.requestedFrames++;
}

GZCommandManager gCommandManager;

GZCommandManager::GZCommandManager() {
    this->mpButtons = &gGZ.mButtons;
    this->mpCommands = sCommands;
}

void GZCommandManager::Update() {
    if (this->mpCommands == NULL || gMenuManager.mState.isOpened) {
        return;
    }

    for (int i = 0; i < ARRAY_LEN(sCommands); i++) {
        GZCmdItem* pCmd = &this->mpCommands[i];

        if (pCmd->btnCombo.Executed(this->mpButtons)) {
            if (pCmd->actionCallback != NULL) {
                pCmd->actionCallback();
            }
        }
    }
}
