#include "gz_settings.hpp"
#include "gz_cheats.hpp"
#include "gz_menu.hpp"

#include <Game/Game.hpp>
#include <TitleScreen/TitleScreen.hpp>
#include <Unknown/UnkStruct_0204a060.hpp>
#include <Unknown/UnkStruct_ov000_020b5214.hpp>

extern "C" u16 func_02026738(); // OS_GetLockID
extern "C" void func_020312b8(u32 offset, void* buf, u32 size, void*, void*, u32, u32, u32,
                              u32); // CARD_ReadWriteBackupAsync

static inline void ReadSaveImpl(u32 offset, void* buf, u32 size, void* param4, void* param5) {
    func_020312b8(offset, buf, size, param4, param5, 1, 6, 1, 0);
}

static inline void WriteSaveImpl(u32 offset, void* buf, u32 size, void* param4, void* param5) {
    func_020312b8((u32)buf, (void*)offset, size, param4, param5, 1, 7, 10, 2);
}

static void PrevProfile(u32 params) {
    if (gSettings.mProfileHeader.curProfileIndex - 1 > 0) {
        gSettings.mProfileHeader.curProfileIndex--;
    } else {
        gSettings.mProfileHeader.curProfileIndex = 0;
    }

    gCheatManager.SetCheatBitfieldPtr(gSettings.mProfiles[gSettings.mProfileHeader.curProfileIndex].mCheatBitfield);
}

static void NextProfile(u32 params) {
    gSettings.mProfileHeader.curProfileIndex++;

    if (gSettings.mProfileHeader.curProfileIndex >= ARRAY_LEN(gSettings.mProfiles)) {
        gSettings.mProfileHeader.curProfileIndex = ARRAY_LEN(gSettings.mProfiles) - 1;
    }

    gCheatManager.SetCheatBitfieldPtr(gSettings.mProfiles[gSettings.mProfileHeader.curProfileIndex].mCheatBitfield);
}

static void LoadDefaultProfile(u32 params) {
    gSettings.LoadDefaultProfile();
    gMenuManager.mState.successTimer = 90;
}

static void SaveSettings(u32 params) {
    gSettings.WriteSave();
    gMenuManager.mState.successTimer = 90;
}

static GZMenuItem sSettingsMenuItems[] = {
    {"Prev Profile", GZMenuItemType_Default, NULL, PrevProfile, 0, NULL, 0},
    {"Next Profile", GZMenuItemType_Default, NULL, NextProfile, 0, NULL, 0},
    {"Load Default Profile", GZMenuItemType_Default, NULL, LoadDefaultProfile, 0, NULL, 0},
    {"Save Settings", GZMenuItemType_Default, NULL, SaveSettings, 0, NULL, 0},
};

GZSettings gSettings;

GZSettings::GZSettings() {
    this->mMenu.title = "Settings";
    this->mMenu.parent = gMenuManager.GetMainMenu();
    this->mMenu.entries = sSettingsMenuItems;
    this->mMenu.mCount = ARRAY_LEN(sSettingsMenuItems);
    this->mMenu.needSaveFile = true;
    this->mMenu.itemIndex = 0;

    this->mSaveIndex = 0;
    this->InitSave();
    this->ReadSave();
}

GZSettings::~GZSettings() {}

void GZSettings::Update() { this->ProcessTitleScreen(); }

void GZSettings::Draw(Vec2b* pPos) {
    this->mMenu.Draw(pPos);

    Vec2b settingsPos = gMenuManager.mState.menuPos;
    settingsPos.y = pPos->y + 1;
    DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &settingsPos, 0, 0, "Current Profile: %d",
                      gSettings.mProfileHeader.curProfileIndex + 1);

    settingsPos.y = 21;
    if (gSettings.error) {
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &settingsPos, 0, 1, "Error detected: 0x%X", gSettings.errorCode);
    } else if (gMenuManager.mState.successTimer > 0) {
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &settingsPos, 0, 0, "Success!", gSettings.errorCode);
    }
}

void GZSettings::ProcessTitleScreen() {
    GZProfile* pProfile = this->GetProfile();

    if (data_027e0994 == NULL || (!pProfile->mFasterTitleScreen && !pProfile->mSkipTitleScreen)) {
        return;
    }

    TitleScreen* pTitleScreen = data_027e0994->GetTitleScreen();

    if (!gGZ.IsTitleScreen() || pTitleScreen == NULL || pTitleScreen->mShowUI) {
        return;
    }

    pTitleScreen->func_ov025_020c4e54();

    if (pProfile->mSkipTitleScreen) {
        data_ov000_020b5214.func_ov000_0206db44(0x0B);
        pTitleScreen->func_ov025_020c4ea0(TitleScreenState_ToFileSelect);
    }
}

static GZProfile sDefaultProfile = {
    .mFasterTitleScreen = true,
    .mSkipTitleScreen = true,
    .mPositionIndex = 0,
};

void GZSettings::LoadDefaultProfile() {
    GZProfile* pProfile = this->GetProfile();

    memcpy(pProfile, &sDefaultProfile, sizeof(GZProfile));

    for (int i = 0; i < MAX_POS_SLOTS; i++) {
        pProfile->mLandPosSlots[i] = {0, 0, 0};
        pProfile->mTrainPosSlots[i] = {0, 0, 0};
    }
}

void GZSettings::ResetAllProfiles() {
    memset(&this->mProfileHeader, 0, sizeof(GZProfileHeader));

    for (int i = 0; i < ARRAY_LEN(this->mProfiles); i++) {
        GZProfile* pProfile = &this->mProfiles[i];

        memcpy(pProfile, &sDefaultProfile, sizeof(GZProfile));

        for (int j = 0; j < MAX_POS_SLOTS; j++) {
            pProfile->mLandPosSlots[j] = {0, 0, 0};
            pProfile->mTrainPosSlots[j] = {0, 0, 0};
        }
    }
}

void GZSettings::InitSave() {
    memset(&this->mProfileHeader, 0, sizeof(GZProfileHeader));
    memset(this->mProfiles, 0, sizeof(this->mProfiles));

    this->lockID = func_02026738();

    if (this->lockID != -3) {
        this->error = true;
        this->errorCode = 0x544F4F;
    }

    // automatic call of func_020313b4 & func_02030cfc for error checks
    CardLock lock;
}

void GZSettings::WriteSave() {
    CardLock lock;
    WriteSaveImpl(GZ_PROFILE_HEADER_OFFSET, &this->mProfileHeader, sizeof(GZProfileHeader), NULL, NULL);
    WriteSaveImpl(GZ_PROFILES_OFFSET, this->mProfiles, sizeof(this->mProfiles), NULL, NULL);
}

void GZSettings::ReadSave() {
    {
        CardLock lock;
        ReadSaveImpl(GZ_PROFILE_HEADER_OFFSET, &this->mProfileHeader, sizeof(GZProfileHeader), NULL, NULL);
        ReadSaveImpl(GZ_PROFILES_OFFSET, this->mProfiles, sizeof(this->mProfiles), NULL, NULL);
    }

    // if we read 0xFF it means the save file was never written to before, so load defaults for everything
    if (this->mProfileHeader.curProfileIndex == 0xFF) {
        this->ResetAllProfiles();
    }
}
