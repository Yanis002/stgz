#pragma once

#include "gz.hpp"
#include "gz_menu.hpp"

#include <Game/Game.hpp>
#include <System/OverlayManager.hpp>
#include <mem.h>
#include <types.h>

extern "C" void func_02030d48(u16 lockID); // CARD_LockBackup
extern "C" int func_020313b4(u32 type); // CARD_IdentifyBackup
extern "C" int func_02030cfc(); // CARD_GetResultCode
extern "C" void func_02030d58(u16 lockID); // CARD_UnlockBackup

#define MAX_POS_SLOTS 8
#define MAX_SAVE_PROFILES 8

extern "C" {
// IMPORTANT: do not reorder, retype or resize members!!! otherwise it will break older versions

typedef struct GZProfileHeader {
    u8 curProfileIndex;
} GZProfileHeader;

// the save file is large so we don't care about packing stuff to save space
typedef struct GZProfile {
    bool mFasterTitleScreen; // goes into the "touch or start to exit" state immediately
    bool mSkipTitleScreen; // same as above except it jump straight into the file select without requiring any input
    s16 mPositionIndex;
    Vec3p mLandPosSlots[MAX_POS_SLOTS];
    Vec3p mTrainPosSlots[MAX_POS_SLOTS];
    u32 mCheatBitfield[4]; // way more than enough
} GZProfile;

// profiles first because they might take a lot of space
#define GZ_PROFILES_OFFSET 0xF5000

// profile header at the end because the space it will take shouldn't be that large
#define GZ_PROFILE_HEADER_OFFSET 0xFFF00
}

class GZSettings {
  private:
    GZProfileHeader mProfileHeader;
    GZProfile mProfiles[MAX_SAVE_PROFILES];
    GZMenu mMenu;
    u16 lockID;
    bool error;
    int errorCode;
    u8 mSaveIndex; // save file to use (intiially meant for file select quick boot, unused for now)

  public:
    GZSettings();
    ~GZSettings();

    GZProfileHeader* GetProfileHeader() { return &this->mProfileHeader; }

    u16 GetLockID() { return this->lockID; }

    bool GetError() { return this->error; }
    void SetError(bool error) { this->error = error; }

    int GetErrorCode() { return this->errorCode; }
    void SetErrorCode(int code) { this->errorCode = code; }

    int GetProfileCount() { return ARRAY_LEN(this->mProfiles); }
    GZProfile* GetProfiles() { return this->mProfiles; }
    GZProfile* GetProfile(u8 index) { return &this->mProfiles[index]; }
    GZProfile* GetProfile() { return this->GetProfile(this->mProfileHeader.curProfileIndex); }

    Vec3p* GetPosArray() {
        GZProfile* pProfile = this->GetProfile();

        if (gGZ.IsOnLand()) {
            return pProfile->mLandPosSlots;
        }

        return pProfile->mTrainPosSlots;
    }

    GZMenu* GetMenu() { return &this->mMenu; }

    void Update();
    void Draw(Vec2b* pPos);
    void ProcessTitleScreen();
    void LoadDefaultProfile();
    void ResetAllProfiles();
    void InitSave();
    void WriteSave();
    void ReadSave();
};

extern GZSettings gSettings;

// automatic lock and unlock for convenience
struct CardLock {
    CardLock() {
        if (gSettings.GetLockID() != -3) {
            func_02030d48(gSettings.GetLockID());

            if (func_020313b4(0x1402) == 0) {
                gSettings.SetError(true);
            }
        }
    }

    ~CardLock() {
        int errorCode = func_02030cfc();
        gSettings.SetErrorCode(errorCode);
        gSettings.SetError(errorCode != 0);

        if (gSettings.GetLockID() != -3) {
            func_02030d58(gSettings.GetLockID());
        }
    }
};
