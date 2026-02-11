#pragma once

#include "gz_controls.hpp"

typedef void (*GZCmdInit)(void);
typedef void (*GZCmdAction)(void);

struct GZCmdItem {
    const char* name;
    ButtonCombo btnCombo;
    GZCmdAction actionCallback;
};

class GZCommandManager {
public:
    Input* mpButtons;
    GZCmdItem* mpCommands;

    GZCommandManager();

    void Update();
};

extern GZCommandManager gCommandManager;
