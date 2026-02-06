#pragma once

#include "mem.hpp"

#include <Player/TouchControl.hpp>
#include <nitro/button.h>
#include <Unknown/UnkStruct_02049b18.hpp>

class GZ {
public:
    Input* mpButtons;
    TouchControl* mpTouchControl;

    GZ() : mpButtons(&data_02049b18.mButtons), mpTouchControl(&data_02049b18.mUnk_06.mTouchControl) {}
    ~GZ() {}

    void Init();
    void Update();
};

extern GZ gGZ;
