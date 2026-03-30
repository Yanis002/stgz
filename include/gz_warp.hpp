#pragma once

#include "common.hpp"
#include "gz_controls.hpp"
#include "gz_menu.hpp"

#include <Unknown/UnkStruct_027e09a4.hpp>
#include <nitro/math.h>

enum {
    WarpIndexType_Spawn,
    WarpIndexType_Room,
    WarpIndexType_Cutscene,
};

class GZWarpManager {
  private:
    Input* mpButtons;
    const char* mpSceneName;
    GZMenu mMenu;
    s16 mRoomIndexMin;
    s16 mRoomIndexMax;
    s16 mSceneIndex;
    s16 mSpawnIndex;
    s16 mRoomIndex;
    s16 mCutsceneIndex;

  public:
    GZWarpManager();

    GZMenu* GetMenu() { return &this->mMenu; }
    void SetSceneName(const char* name) { this->mpSceneName = name; }
    void SetSceneIndex(s16 value) {
        this->mSceneIndex = value;
        this->UpdateRoomIndexRange();
    }
    void SetSpawnIndex(s16 value) { this->mSpawnIndex = value; }
    void SetCutsceneIndex(s16 value) { this->mCutsceneIndex = value; }
    s16 GetSceneIndex() { return this->mSceneIndex; }
    s16 GetSpawnIndex() { return this->mSpawnIndex; }
    s16 GetRoomIndex() { return this->mRoomIndex; }
    s16 GetCutsceneIndex() { return this->mCutsceneIndex; }

    void SetRoomIndex(int value);
    void UpdateRoomIndexRange();
    bool IsMenuActive();
    void InitMenu();
    void Update();
    void Draw(Vec2b* pPos);
};

extern GZWarpManager gWarpManager;
