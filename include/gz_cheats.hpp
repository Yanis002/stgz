#pragma once

#include "gz_menu.hpp"

#include <types.h>

#define DEFINE_CHEAT(enum, _) GZCheat_##enum,
typedef u32 GZCheat;
enum GZCheat_ {
#include "cheats.inl"
    GZCheat_Max
};
#undef DEFINE_CHEAT

class GZCheatManager {
  private:
    GZMenu mMenu;
    u32* mpCheatBitfield;

  public:
    GZMenu* GetMenu() { return &this->mMenu; }

    GZCheatManager();
    void SetCheatBitfieldPtr(u32* pBitfield);
    bool Check(GZCheat eCheat);
    void Toggle(GZCheat eCheat);
    void Update();
};

extern GZCheatManager gCheatManager;
