#pragma once

#include "gz_controls.hpp"
#include "gz_menu.hpp"

#include <nitro/math.h>

typedef void (*GZCmdInit)(void);
typedef void (*GZCmdAction)(void);

struct GZCmdItem {
    ButtonCombo btnCombo;
    GZCmdAction actionCallback;
};

class GZCommandManager {
public:
    Input* mpButtons;
    GZCmdItem* mpCommands;

    GZCommandManager();

    void Update();
    void Draw(Vec2b* pPos);
};

extern GZCommandManager gCommandManager;
