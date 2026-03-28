#pragma once

#include "common.hpp"
#include "gz_controls.hpp"
#include "gz_menu.hpp"

#include <nitro/math.h>

class GZWarpManager {
  private:
    Input* mpButtons;
    GZMenu mMenu;
    u8 mSpawnIndex;
    u8 mHouseIndex;

  public:
    GZWarpManager();

    GZMenu* GetMenu() { return &this->mMenu; }

    bool IsMenuActive();
    void InitMenu();
    void Update();
    void Draw(Vec2b* pPos);
};

extern GZWarpManager gWarpManager;
