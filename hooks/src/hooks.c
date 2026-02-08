/**
 * @file: hooks.c
 * 
 * This file implements the hooks that are replacing vanilla functions so we can run custom code.
 * Make sure to make it as small as possible!
 */

extern void _ZN18UnkStruct_02049a2c19func_ov018_020c48f8Ev(void* ptr);
extern void FS_LoadOverlay(int param1, int overlayID);
extern void GZ_Init();
extern void GZ_Update();
extern void _ZN18UnkStruct_02049bd413func_02014d98Ev(void* param1);

#ifndef __INTELLISENSE__
#ifndef GZ_OVL_ID
#error "overlay id is not defined!"
#endif
#else
#define GZ_OVL_ID 0
#endif

// init hook: replace the `func_ov018_020c48f8` call from `GameModeStartUp::vfunc_0C` so we can load and init the gz overlay
void GZ_InitHook(void* ptr) {
    _ZN18UnkStruct_02049a2c19func_ov018_020c48f8Ev(ptr);

    // IMPORTANT: the gz overlay must load AFTER overlay 0 is loaded
    if (*(unsigned int*)0x02043E50 == 0) {
        // load our overlay
        FS_LoadOverlay(0, GZ_OVL_ID);

        // call the init function
        GZ_Init();
    }
}

// update hook: replace the `func_02014d98` call from `UnkStruct_02049a2c::Run` so we can run the gz update function
void GZ_UpdateHook(void* ptr) {
    if (*(unsigned int*)0x02043E50 == 0) {
        // call the update function
        GZ_Update();
    }

    // end with original function call
    _ZN18UnkStruct_02049bd413func_02014d98Ev(ptr);
}
