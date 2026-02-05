/**
 * @file: hooks.c
 * 
 * This file implements the hooks that are replacing vanilla functions so we can run custom code.
 * Make sure to make it as small as possible!
 */

//! TODO: remove when decomp names this
#define RTC_Init func_0202ff34

extern void FS_LoadOverlay(int param1, int overlayID);
extern void RTC_Init();
extern void func_02014d98(void* param1);
extern void GZ_Init();
extern void GZ_Update();

extern unsigned int data_02049bd4;

#ifndef __INTELLISENSE__
#ifndef GZ_OVL_ID
#error "overlay id is not defined!"
#endif
#else
#define GZ_OVL_ID 0
#endif

// init hook: replace the `RTC_Init` call from `func_ov018_020c4e8c` so we can load and init the gz overlay
void GZ_InitHook() {
    // load our overlay
    FS_LoadOverlay(0, GZ_OVL_ID);

    // call the init function
    GZ_Init();

    // end with original function call
    RTC_Init();
}

// update hook: replace the `func_02014d98` call from `UnkStruct_02049a2c::Run` so we can run the gz update function
void GZ_UpdateHook() {
    // call the update function
    GZ_Update();

    // end with original function call
    func_02014d98(&data_02049bd4);
}
