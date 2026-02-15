#include "gz_commands.hpp"
#include "gz.hpp"
#include "gz_menu.hpp"
#include "gz_settings.hpp"

#include <Unknown/UnkStruct_02049b18.hpp>
#include <Unknown/UnkStruct_027e0478.hpp>
#include <nitro/math.h>
#include <regs.h>
#include <string.h>

extern "C" void DisplayDebugText(int, void*, int, int, const char*);
extern "C" void DisplayDebugTextF(int, void*, int, int, const char*, ...);

static void ExecuteLevitate(u32 params);
static void ExecutePause(u32 params);
static void ExecuteFrameAdvance(u32 params);
static void ExecutePrevPosition(u32 params);
static void ExecuteNextPosition(u32 params);
static void ExecuteSavePosition(u32 params);
static void ExecuteLoadPosition(u32 params);
static void ExecuteVoidOut(u32 params);
static void ExecuteTurbo(u32 params);

// commands with default button combos, assigning the held btn to none means it's not required (and vice versa)
static GZCmdItem sCommands[] = {
    {ButtonCombo("Levitate", BTN_X, BTN_NONE), ExecuteLevitate},
    {ButtonCombo("Pause/Unpause", BTN_NONE, BTN_DDOWN), ExecutePause},
    {ButtonCombo("Frame Advance", BTN_NONE, BTN_DUP), ExecuteFrameAdvance},
    {ButtonCombo("Prev Position", BTN_R, BTN_DLEFT), ExecutePrevPosition},
    {ButtonCombo("Next Position", BTN_R, BTN_DRIGHT), ExecuteNextPosition},
    {ButtonCombo("Save Position", BTN_R, BTN_X), ExecuteSavePosition},
    {ButtonCombo("Load Position", BTN_R, BTN_A), ExecuteLoadPosition},
    {ButtonCombo("Void Out", BTN_R, BTN_Y), ExecuteVoidOut},
    {ButtonCombo("Turbo", BTN_R, BTN_B), ExecuteTurbo},
};

static void ExecuteLevitate(u32 params) { data_027e0478.mPlayer.mVel.y = FLOAT_TO_Q20(0.334375f); }

static void ExecutePause(u32 params) {
    if (!gGZ.mState.isPaused) {
        gGZ.mState.isPaused = true;
    } else {
        gGZ.mState.isPaused = false;
    }
}

static void ExecuteFrameAdvance(u32 params) { gGZ.mState.requestedFrames++; }

static void ExecutePrevPosition(u32 params) {
    GZProfile* pProfile = gSettings.GetProfile();

    pProfile->mPositionIndex--;

    if (pProfile->mPositionIndex < 0) {
        pProfile->mPositionIndex = 0;
    }
}

static void ExecuteNextPosition(u32 params) {
    GZProfile* pProfile = gSettings.GetProfile();

    pProfile->mPositionIndex++;

    if (pProfile->mPositionIndex > MAX_POS_SLOTS) {
        pProfile->mPositionIndex = MAX_POS_SLOTS;
    }
}

static void ExecuteSavePosition(u32 params) {
    GZProfile* pProfile = gSettings.GetProfile();
    Vec3p* pPosArray = gSettings.GetPosArray();
    pPosArray[pProfile->mPositionIndex] = data_027e0478.mPlayer.mPos;
}

static void ExecuteLoadPosition(u32 params) {
    GZProfile* pProfile = gSettings.GetProfile();
    Vec3p* pPosArray = gSettings.GetPosArray();
    data_027e0478.mPlayer.mPos = pPosArray[pProfile->mPositionIndex];
}

static void ExecuteVoidOut(u32 params) {
    if (gGZ.IsOnLand()) {
        data_027e0478.mPlayer.mPos.y = FLOAT_TO_Q20(-4000.0f);
    }
}

static void ExecuteTurbo(u32 params) {}

GZCommandManager gCommandManager;

GZCommandManager::GZCommandManager() {
    this->mpButtons = &gGZ.mButtons;
    this->mpCommands = sCommands;
    this->CreateMenuItems();
}

void GZCommandManager::CreateMenuItems() {
    this->mMenu.entries = new GZMenuItem[ARRAY_LEN(sCommands)];

    for (int i = 0; i < ARRAY_LEN(sCommands); i++) {
        this->mMenu.entries[i].name = sCommands[i].btnCombo.name;
        this->mMenu.entries[i].action = sCommands[i].actionCallback;
        this->mMenu.entries[i].submenu = NULL;
    }
}

void GZCommandManager::Update() {
    if (this->mpCommands == NULL || gMenuManager.mState.isOpened) {
        return;
    }

    for (int i = 0; i < ARRAY_LEN(sCommands); i++) {
        GZCmdItem* pCmd = &this->mpCommands[i];

        if (pCmd->btnCombo.Executed(this->mpButtons)) {
            if (pCmd->actionCallback != NULL) {
                pCmd->actionCallback(0);
            }
        }
    }
}

void GZCommandManager::Draw(Vec2b* pPos) {
    Vec2b elemPos = *pPos;

    for (s16 i = 0; i < ARRAY_LEN(sCommands); i++) {
        GZCmdItem* pCmd = &this->mpCommands[i];
        bool selected = i + 1 == gMenuManager.mState.itemIndex;

        pCmd->btnCombo.SetComboString();
        DisplayDebugText(DRAW_TO_TOP_SCREEN, &elemPos, 0, selected, pCmd->btnCombo.fullName);
        elemPos.y++;
    }
}
