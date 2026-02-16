#pragma once

#include "common.hpp"
#include "gz_controls.hpp"
#include "gz_menu.hpp"

#include <nitro/math.h>

typedef void (*GZCmdInit)(void);

struct GZCmdItem {
    ButtonCombo btnCombo;
    GZAction actionCallback;
};

class GZCommandManager {
  public:
    Input* mpButtons;
    GZCmdItem* mpCommands;
    GZMenu mMenu;

    GZCommandManager();

    void InitMenu();
    void Update();
    void Draw(Vec2b* pPos);
};

extern GZCommandManager gCommandManager;
