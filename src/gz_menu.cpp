#include "gz_menu.hpp"
#include "gz.hpp"
#include "build.hpp"

#include <Item/ItemManager.hpp>
#include <regs.h>
#include <Unknown/UnkStruct_ov000_020b50c0.hpp>
#include <System/OverlayManager.hpp>

extern "C" void DisplayDebugText(int, void*, int, int, const char*);
extern "C" void DisplayDebugTextF(int, void*, int, int, const char*, ...);
extern "C" void DC_FlushAll();
extern "C" void func_020131ec();
extern "C" void func_02023770(int);
extern "C" void GX_SetGraphicsMode(int, int, int);
extern "C" void GXS_SetGraphicsMode(int);
extern "C" void func_0201b180(bool, bool);
extern "C" unk32 func_020147a8();
extern "C" void func_0201b278(bool, bool);
extern "C" void SetBrightColor(void *, int);

static void Quit(u32 params);
static void Back(u32 params);
static void UpdateInventory(u32 params);
static void UpdateSettings(u32 params);

extern GZMenu sMainMenu;
extern GZMenu sInventoryMenu;
extern GZMenu sCollectionMenu;
extern GZMenu sSettingsMenu;
extern GZMenu sAboutMenu;

static GZMenuItem sInventoryMenuItems[] = {
    {"Back", Back, 0, NULL, 0},
    {"Whirlwind", UpdateInventory, ItemFlag_Whirlwind, NULL},
    {"Boomerang", UpdateInventory, ItemFlag_Boomerang, NULL},
    {"Whip", UpdateInventory, ItemFlag_Whip, NULL},
    {"Bow", UpdateInventory, ItemFlag_Bow, NULL},
    {"Bombs", UpdateInventory, ItemFlag_Bombs, NULL},
    {"SandRod", UpdateInventory, ItemFlag_SandRod, NULL},
};

static GZMenuItem sCollectionMenuItems[] = {
    {"Back", Back, 0, NULL, 0},
    {"Shield", UpdateInventory, ItemFlag_Shield, NULL},
    {"Sword", UpdateInventory, ItemFlag_Sword, NULL},
    {"LokomoSword", UpdateInventory, ItemFlag_LokomoSword, NULL},
    {"RecruitUniform", UpdateInventory, ItemFlag_RecruitUniform, NULL},
    {"ScrollBeam", UpdateInventory, ItemFlag_ScrollBeam, NULL},
    {"ScrollSpinAttack", UpdateInventory, ItemFlag_ScrollSpinAttack, NULL},
    {"AncientShield", UpdateInventory, ItemFlag_AncientShield, NULL},
    {"PanFlute", UpdateInventory, ItemFlag_PanFlute, NULL},
};

static GZMenuItem sSettingsMenuItems[] = {
    {"Back", Back, 0, NULL, 0},
};

static GZMenuItem sAboutMenuItems[] = {
    {"Back", Back, 0, NULL, 0},
};

static GZMenuItem sMainMenuItems[] = {
    {"Quit", Quit, 0, NULL, 0 },
    {"Inventory", NULL, 0, &sInventoryMenu},
    {"Collection", NULL, 0, &sCollectionMenu},
    {"Settings", NULL, 0, &sSettingsMenu},
    {"About", NULL, 0, &sAboutMenu},
};

// pointer to the item list, number of items, pointer to the previous menu
GZMenu sMainMenu = {sMainMenuItems, ARRAY_LEN(sMainMenuItems), NULL};
GZMenu sInventoryMenu = {sInventoryMenuItems, ARRAY_LEN(sInventoryMenuItems), &sMainMenu};
GZMenu sCollectionMenu = {sCollectionMenuItems, ARRAY_LEN(sCollectionMenuItems), &sMainMenu};
GZMenu sSettingsMenu = {sSettingsMenuItems, ARRAY_LEN(sSettingsMenuItems), &sMainMenu};
GZMenu sAboutMenu = {sAboutMenuItems, ARRAY_LEN(sAboutMenuItems), &sMainMenu};

static void Quit(u32 params) {
    gMenuManager.Quit();
}

static void Back(u32 params) {
    gMenuManager.AssignPrevMenu();
}

static void UpdateInventory(u32 params) {

}

static void UpdateSettings(u32 params) {

}

static u32 prevDispCnt;
static u32 prevDispCnt_Sub;
static u8 prevVRAM_CNT_B;

GZMenuManager gMenuManager;

GZMenuManager::GZMenuManager() {
    this->mState.Init();
    this->mpActiveMenu = &sMainMenu;
    this->mpButtons = &data_02049b18.mButtons;
}

void GZMenuManager::Update() {
    // update the inputs
    gGZ.UpdateInputs();

    // toggle the menu when necessary
    if (this->mControls.toggleMenu.Executed(this->mpButtons)) {
        if (!this->mState.isOpened) {
            this->SetupScreen();
            this->StartDraw();
            this->mState.isOpened = true;
        } else {
            this->Quit();
        }

        return;
    }

    // selection update
    if (this->mState.isOpened) {
        if (this->mControls.up.Executed(this->mpButtons)) {
            this->mState.itemIndex--;

            if (this->mState.itemIndex < 0) {
                this->mState.itemIndex = 0;
            }

            this->mState.requestRedraw = true;
        }

        if (this->mControls.down.Executed(this->mpButtons)) {
            if (this->mState.itemIndex + 1 < this->mpActiveMenu->mCount) {
                this->mState.itemIndex++;
                this->mState.requestRedraw = true;
            }
        }
    }

    // handle confirmation stuff
    if (this->mControls.ok.Executed(this->mpButtons)) {
        GZMenuItem* pActiveMenuItem = &this->mpActiveMenu->mpItems[this->mState.itemIndex];

        if (pActiveMenuItem->mActionCallback != NULL) {
            pActiveMenuItem->mActionCallback(pActiveMenuItem->params);
        } else if (pActiveMenuItem->mSubMenu != NULL) {
            this->mpActiveMenu = pActiveMenuItem->mSubMenu;
            this->mState.itemIndex = 0;
            this->mState.requestRedraw = true;
        }
    }

    // redraw the menu if necessary
    if (this->mState.requestRedraw) {
        this->mState.requestRedraw = false;
        this->SetupScreen();
        this->Draw();
    }
}

void GZMenuManager::SetupScreen() {
    // reset the top screen buffer
    memset((void*)0x0204D9D0, 0, 0x600);

    // send the menu item strings to the buffer
    Vec2b elemPos = this->mState.menuPos;
    for (s16 i = 0; i < this->mpActiveMenu->mCount; i++) {
        // 0 = white, 1 = red, 2 = darker red, 3 = dark green
        DisplayDebugText(0, &elemPos, 0, i == this->mState.itemIndex, this->mpActiveMenu->mpItems[i].mName);
        elemPos.y++;
    }

    // send the arrow to the buffer
    Vec2b arrowPos = this->mState.menuPos;
    arrowPos.x--;
    arrowPos.y += this->mState.itemIndex;
    DisplayDebugText(0, &arrowPos, 0, 1, ">");
    arrowPos.y++;

    // special handling for the about screen
    if (this->mpActiveMenu == &sAboutMenu) {
        Vec2b aboutPos = this->mState.menuPos;
        aboutPos.y = elemPos.y + 1; // start wherever the item list ended
        DisplayDebugTextF(0, &aboutPos, 0, 0, "Code Version: %s", gBuildGitVersion);

        aboutPos.y++;
        DisplayDebugTextF(0, &aboutPos, 0, 0, "Build Author: %s", gBuildAuthor);

        aboutPos.y++;
        DisplayDebugTextF(0, &aboutPos, 0, 0, "Commit Author: %s", gCommitAuthor);

        aboutPos.y++;
        DisplayDebugTextF(0, &aboutPos, 0, 0, "Commit Name: %s", gCommitGitString);

        aboutPos.y += 2;
        DisplayDebugText(0, &aboutPos, 0, 0, "License: GPL-3.0");

        aboutPos.y++;
        DisplayDebugText(0, &aboutPos, 0, 0, "Made with <3 by Yanis2.");
    }
}

void GZMenuManager::StartDraw() {
    // don't ask me I have no idea how this works, feel free to improve

    prevDispCnt = REG_DISPCNT;
    prevDispCnt_Sub = REG_DISPCNT_SUB;
    prevVRAM_CNT_B = REG_VRAM_CNT_B;

    DC_FlushAll();
    func_020131ec();
    REG_POWER_CNT |= 0x8000;
    func_02023770(2); // no corruption when commented but no text
    GX_SetGraphicsMode(1, 0, 0);
    GXS_SetGraphicsMode(0);
    
    REG_DISPCNT     = ((REG_DISPCNT & ~0x1F00) | 0x100);
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x1F00) | 0x100;
    func_0201b180(true, false); // loads the font (participate in the corruption also?)
}

void GZMenuManager::Draw() {
    func_0201b278(true, false); // copy screen
    func_020131ec();
    SetBrightColor((void *) &REG_MASTER_BRIGHT, 0);
    SetBrightColor((void *) &REG_MASTER_BRIGHT_SUB, 0);

    if (func_020147a8() != 0) {
        data_ov000_020b50c0.func_ov000_02069f58();
    }

    while (this->mState.isOpened && !this->mState.requestRedraw) {
        func_020131ec();
        this->Update();
    }
}

void GZMenuManager::StopDraw() {
    if (gOverlayManager.mLoadedOverlays[OverlaySlot_4] == OverlayIndex_Title) {
        GX_SetGraphicsMode(1, 0, 1);
        GXS_SetGraphicsMode(5);
    }

    memset((void*)0x068A0000, 0, 0x21A0);
    REG_DISPCNT = prevDispCnt;
    REG_DISPCNT_SUB = prevDispCnt_Sub;
    REG_VRAM_CNT_B = prevVRAM_CNT_B;
}

