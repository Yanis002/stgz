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

//! TODO: make it one setting set per save file

extern "C" {
typedef struct GZProfileHeader {
    u8 curProfileIndex;
} GZProfileHeader __attribute__((aligned(32)));

// the save file is large so we don't care about packing stuff to save space
typedef struct GZProfile {
    u8 profileIndex;
    bool mFasterTitleScreen; // goes into the "touch or start to exit" state immediately
    bool mSkipTitleScreen; // same as above except it jump straight into the file select without requiring any input
    s8 mPositionIndex;
    Vec3p mLandPosSlots[MAX_POS_SLOTS];
    Vec3p mTrainPosSlots[MAX_POS_SLOTS];
} GZProfile;

#define GZ_SAVE_OFFSET 0xF5000
#define GZ_PROFILE_HEADER_OFFSET (GZ_SAVE_OFFSET)
#define GZ_PROFILES_OFFSET (GZ_SAVE_OFFSET + sizeof(GZProfileHeader))
}

class GZSettings {
  public:
    GZProfileHeader mProfileHeader;
    GZProfile mProfiles[MAX_SAVE_PROFILES];
    u16 lockID;
    bool error;
    int errorCode;
    u8 mSaveIndex; // save file to use (intiially meant for file select quick boot, unused for now)

    GZSettings();
    ~GZSettings();

    GZProfile* GetProfile() { return &this->mProfiles[this->mProfileHeader.curProfileIndex]; }

    bool IsStartUp() { return gGZ.prevGameModeOvl == OverlayIndex_StartUp; }

    Vec3p* GetPosArray() {
        GZProfile* pProfile = this->GetProfile();

        if (gGZ.IsOnLand()) {
            return pProfile->mLandPosSlots;
        }

        return pProfile->mTrainPosSlots;
    }

    void Update();
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
        if (gSettings.lockID != -3) {
            func_02030d48(gSettings.lockID);

            if (func_020313b4(0x1402) == 0) {
                gSettings.error = true;
            }
        }
    }

    ~CardLock() {
        gSettings.errorCode = func_02030cfc();
        gSettings.error = gSettings.errorCode != 0;

        if (gSettings.lockID != -3) {
            func_02030d58(gSettings.lockID);
        }
    }
};
