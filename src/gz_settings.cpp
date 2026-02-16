#include "gz_settings.hpp"
#include "gz_menu.hpp"

#include <Game/Game.hpp>
#include <TitleScreen/TitleScreen.hpp>
#include <Unknown/UnkStruct_0204a060.hpp>
#include <Unknown/UnkStruct_ov000_020b5214.hpp>

extern "C" u16 func_02026738(); // OS_GetLockID
extern "C" void func_020312b8(u32 offset, void* buf, u32 size, void*, void*, u32, u32, u32,
                              u32); // CARD_ReadWriteBackupAsync

inline void ReadSaveImpl(u32 offset, void* buf, u32 size, void* param4, void* param5) {
    func_020312b8(offset, buf, size, param4, param5, 1, 6, 1, 0);
}

inline void WriteSaveImpl(u32 offset, void* buf, u32 size, void* param4, void* param5) {
    func_020312b8((u32)buf, (void*)offset, size, param4, param5, 1, 7, 10, 2);
}

GZSettings gSettings;

GZSettings::GZSettings() {
    this->mSaveIndex = 0;
    this->InitSave();
    this->ReadSave();
}

GZSettings::~GZSettings() {}

void GZSettings::Update() { this->ProcessTitleScreen(); }

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
