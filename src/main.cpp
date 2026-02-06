#include "Player/TouchControl.hpp"

class GZ {
public:
    TouchControl mTouchControl;

    GZ();
    ~GZ();
};

GZ gGZ;

GZ::GZ() {}
GZ::~GZ() {}

extern "C" {

// this function is called by the init hook, see `GZ_InitHook` in hook
void GZ_Init() {}

// this function is called by the update hook, see `GZ_UpdateHook` in hook
void GZ_Update() {
    gGZ.mTouchControl.func_02014414(0x10, true);
}

}
