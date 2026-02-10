#include "gz_game.hpp"
#include "gz.hpp"
#include "gz_settings.hpp"
#include "gz_menu.hpp"

#include <Game/Game.hpp>
#include <System/OverlayManager.hpp>
#include <System/Random.hpp>
#include <Unknown/UnkStruct_02049b74.hpp>
#include <Unknown/UnkStruct_02049bd4.hpp>
#include <Unknown/UnkStruct_0204a110.hpp>
#include <Unknown/UnkStruct_0204e64c.hpp>
#include <Unknown/UnkStruct_027e0208.hpp>
#include <regs.h>

extern "C" void func_020196fc();
extern "C" unk32 func_02031e58();
extern "C" void func_02031e68();
extern "C" void func_01ff8d38();
extern "C" void func_020132c8();
extern "C" void func_020132dc();
extern "C" void func_02013354();
extern "C" void func_0201328c();
extern "C" int func_020280ec();
extern "C" void func_02028100(int enabled);
extern Mat3p data_027e02c4;

struct SomeSaveFileStruct {
    /* 00 */ SaveFile* mpSaveFiles[MAX_SAVE_SLOTS];

    SomeSaveFileStruct(unk32 param1);
    ~SomeSaveFileStruct();
};

void CustomGame::ExecutePause() {
    data_02049b18.func_02013840(data_0204a110.mUnk_004, data_0204a110.func_02019300(data_0204a110.mUnk_DF8));

    if (this->mFrameCounter + data_0204a110.mUnk_004 - (s32)REG_FRAME_COUNTER > 1) {
        func_0201328c();
    }

    while (this->mFrameCounter + data_0204a110.mUnk_004 - (s32)REG_FRAME_COUNTER > 1) {
        func_020132c8();
    }

    {
        int enabled = func_020280ec();
        this->mUnk_1C.func_02013e18((void*)func_020132dc, 0);
        func_02028100(enabled);
    }

    func_020132c8();

    if (gGZ.mState.doRNGUpdatesDuringPause && !gGZ.mState.isRNGPaused) {
        gRandom.UpdateRandomValue();
    }
}

void CustomGame::Run() {
    gGZ.prevGameModeOvl = OverlayIndex_StartUp;

    do {
        {
            gMenuManager.Update();

            if (gMenuManager.IsActive()) {
                gMenuManager.Draw();
            } 
        }

        // stgz: pause and frame advance block
        {
            u32 curFrameCount = REG_FRAME_COUNTER;

            // if we have game frames to draw and it's not pausing enable the pause then decrease the queue value
            if (gGZ.mState.requestedFrames > 0) {
                if (!gGZ.mState.isPaused) {
                    gGZ.mState.isPaused = true;
                }

                gGZ.mState.requestedFrames--;
            } else {
                // if it's paused and there is no frames left to execute then do the pause
                if (gGZ.mState.isPaused) {
                    REG_FRAME_COUNTER = curFrameCount; // freeze frame count
                    this->ExecutePause(); // execute the necessary code to avoid crashes
                    gGZ.Update(); // keep updating gz
                    continue;
                }
            }
        }

        // initialization of the next game mode
        if (this->createCallback != NULL) {
            data_0204999c.func_02013014();

            {
                SomeSaveFileStruct local_28(0x1300);
                this->mpSaveFile = local_28.mpSaveFiles[0];

                if (this->mpCurrentGameMode != NULL) {
                    delete this->mpCurrentGameMode;
                }

                func_020196fc();
                data_02049b18.func_02013768();
                this->mFrameCounter = REG_FRAME_COUNTER;

                this->mpCurrentGameMode = this->createCallback();
                this->createCallback = NULL;
                this->mpCurrentGameMode->vfunc_08();
                this->mpSaveFile = NULL;
                gGZ.prevGameModeOvl = gOverlayManager.mLoadedOverlays[OverlaySlot_4];
            }

            data_0204999c.func_02013070();

            gGZ.OnGameModeInit();
        }

        // update of the current game mode
        if (this->mpCurrentGameMode != NULL) {
            if (this->mUnk_08 != NULL) {
                this->mUnk_08();
                this->mUnk_08 = NULL;
            }

            if (func_02031e58() != 0) {
                func_02031e68();
            }

            data_02049bd4.func_02014d98();
            data_0204a110.func_02019300(data_0204a110.mUnk_DF8);

            // stgz: stop updating the seed each frame
            if (!gGZ.mState.isRNGPaused) {
                gRandom.UpdateRandomValue();
            }

            unk32 uVar4 = data_0204a110.func_02019300(data_0204a110.mUnk_DF8);
            data_02049b74.func_02013a44(data_0204a110.mUnk_004);

            if (this->mUnk_14 == NULL) {
                data_02049b18.func_02013840(data_0204a110.mUnk_004, uVar4);
            } else {
                data_02049b18.func_020138f4(uVar4);
            }

            data_0204e64c.func_020201c4(1);
            data_0204a110.func_02019350();

            if (data_0204a110.func_02019514() == 0 && data_0204e64c.mUnk_00.mUnk_0B == 0) {
                this->mpCurrentGameMode->vfunc_0C();
            }

            data_027e0208.mUnk_0EC = 0x1000;
            data_027e0208.mUnk_0F0 = 0x1000;
            data_027e0208.mUnk_0F4 = 0x1000;

            data_027e0208.mUnk_0E0 = 0;
            data_027e0208.mUnk_0E4 = 0;
            data_027e0208.mUnk_0E8 = 0;

            Mat3p_InitIdentity(&data_027e02c4);
            data_027e0208.mUnk_0FC = 0;
            func_01ff8d38();
            this->mpCurrentGameMode->vfunc_18();
            data_0204a110.func_020194dc();

            if (data_0204a110.func_02019514() == 0) {
                this->mpCurrentGameMode->vfunc_10();
            }

            if (data_0204e64c.mUnk_00.mUnk_0B == 0) {
                data_0204a110.func_02019408();
            }

            this->mpCurrentGameMode->vfunc_1C();
            func_01ff8d38();
            data_0204a110.func_02019454(); // draw obj?
            this->mpCurrentGameMode->vfunc_20();

            if (gOverlayManager.mLoadedOverlays[OverlaySlot_Second] == OverlayIndex_Second) {
                this->func_ov000_020576d0();
                this->func_ov000_0205770c();
            }

            gGZ.OnGameModeUpdate();
        }

        //! TODO: decomp has regalloc issues on those operators but oh well...
        if (this->mFrameCounter + data_0204a110.mUnk_004 - (s32)REG_FRAME_COUNTER > 1) {
            func_0201328c();
        }

        // testing notes: this seems to be used to slow down the execution of the game, commenting this out makes
        // adventure mode run at full speed
        while (this->mFrameCounter + data_0204a110.mUnk_004 - (s32)REG_FRAME_COUNTER > 1) {
            func_020132c8();
        }

        {
            int enabled = func_020280ec();
            this->mUnk_1C.func_02013e18((void*)func_020132dc, 0);
            REG_SWAP_BUFFERS = 3;
            func_02028100(enabled);
        }

        func_020132c8();

        if (this->mUnk_18 != NULL) {
            while (this->mUnk_18() != 0) {
                while (this->mUnk_1C.func_02013e18((void*)func_02013354, 0) == 0) {}

                func_020132c8();
            }
        }

        this->mFrameCounter = REG_FRAME_COUNTER;

        gGZ.Update();
    } while (true);
}
