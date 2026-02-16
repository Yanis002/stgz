#include "gz_menu.hpp"
#include "build.hpp"
#include "gz.hpp"
#include "gz_commands.hpp"
#include "gz_settings.hpp"

#include <Game/GameModeManager.hpp>
#include <MainGame/MainGame.hpp>
#include <System/OverlayManager.hpp>
#include <Unknown/UnkStruct_027e09a4.hpp>
#include <Unknown/UnkStruct_027e0ce0.hpp>
#include <Unknown/UnkStruct_ov000_020b50c0.hpp>
#include <regs.h>
#include <string.h>

/*
Layout:
- Main Menu
    - Inventory
        - Whirlwind
        - Boomerang
        - Whip
        - Bow
        - Bombs
        - SandRod
        - Amounts
            - Bow
            - Bombs
            - Quiver Capacity
            - Bomb Bag Capacity
            - Potion 1
            - Potion 2
            - Small Keys
            - Light Tears
    - Collection
        - Shield
        - Sword
        - LokomoSword
        - RecruitUniform
        - ScrollBeam
        - ScrollSpinAttack
        - AncientShield
        - PanFlute
    - Settings
        - Prev Profile
        - Next Profile
        - Load Default Profile
        - Save Settings
        - Current Profile
        - Success or Error Code (conditional)
    - About
*/

struct Screen {
    /* 00 */ u16 width;
    /* 02 */ u16 height;
    /* 04 */ u16 mUnk_04;
    /* 06 */ u16 mUnk_06; // thing passed to func_0200a4a8
    /* 08 */ u32 mUnk_08;
    /* 0C */ u8 data[0x600];
};
extern Screen data_0204d9d0[2];

extern "C" void DisplayDebugText(int, void*, int, int, const char*);
extern "C" void DisplayDebugTextF(int, void*, int, int, const char*, ...);
extern "C" void GXS_SetGraphicsMode(int);
extern "C" void func_0201b180(bool, bool);
extern "C" void func_02027654(void*, int); // DC_FlushRange
extern "C" void func_020252ec(void*, int, int); // GX0_LoadBG0Scr

static void UpdateInventory(u32 params);
static void UpdateAmounts(u32 params);

// settings menu
static void PrevProfile(u32 params);
static void NextProfile(u32 params);
static void LoadDefaultProfile(u32 params);
static void SaveSettings(u32 params);

// display menu

static void UpdateRegs(u32 params);

extern GZMenu sMainMenu;
extern GZMenu sInventoryMenu;
extern GZMenu sAmountsMenu;
extern GZMenu sCollectionMenu;
extern GZMenu sSettingsMenu;
extern GZMenu sDebugMenu;
extern GZMenu sRegsMenu;
extern GZMenu sAboutMenu;

// clang-format off

// -- main menu items --

static GZMenuItem sMainMenuItems[] = {
    {"Inventory", NULL, 0, &sInventoryMenu, 0},
    {"Collection", NULL, 0, &sCollectionMenu, 0},
    {"Commands", NULL, 0, &gCommandManager.mMenu, 0},
    {"Debug", NULL, 0, &sDebugMenu, 0},
    {"Settings", NULL, 0, &sSettingsMenu, 0},
    {"About", NULL, 0, &sAboutMenu, 0},
};

// -- inventory menu items --

static GZMenuItem sInventoryMenuItems[] = {
    {"Whirlwind", UpdateInventory, ItemFlag_Whirlwind, NULL, 0},
    {"Boomerang", UpdateInventory, ItemFlag_Boomerang, NULL, 0},
    {"Whip", UpdateInventory, ItemFlag_Whip, NULL, 0},
    {"Bow", UpdateInventory, ItemFlag_Bow, NULL, 0},
    {"Bombs", UpdateInventory, ItemFlag_Bombs, NULL, 0},
    {"SandRod", UpdateInventory, ItemFlag_SandRod, NULL, 0},
    {"Amounts", NULL, 0, &sAmountsMenu, 0},
};

static GZMenuItem sAmountsMenuItems[] = {
    {"Bow", UpdateAmounts, InventoryAmountType_Bow, NULL, 0},
    {"Bombs", UpdateAmounts, InventoryAmountType_Bombs, NULL, 0},
    {"Quiver Capacity", UpdateAmounts, InventoryAmountType_QuiverCapacity, NULL, 0},
    {"Bomb Bag Capacity", UpdateAmounts, InventoryAmountType_BombCapacity, NULL, 0},
    {"Potion 1", UpdateAmounts, InventoryAmountType_Potion1, NULL, 0},
    {"Potion 2", UpdateAmounts, InventoryAmountType_Potion2, NULL, 0},
    {"Small Keys", UpdateAmounts, InventoryAmountType_SmallKeys, NULL, 0},
    {"Light Tears", UpdateAmounts, InventoryAmountType_LightTears, NULL, 0},
};

// -- collection menu items --

static GZMenuItem sCollectionMenuItems[] = {
    {"Shield", UpdateInventory, ItemFlag_Shield, NULL, 0},
    {"Sword", UpdateInventory, ItemFlag_Sword, NULL, 0},
    {"LokomoSword", UpdateInventory, ItemFlag_LokomoSword, NULL, 0},
    {"RecruitUniform", UpdateInventory, ItemFlag_RecruitUniform, NULL, 0},
    {"ScrollBeam", UpdateInventory, ItemFlag_ScrollBeam, NULL, 0},
    {"ScrollSpinAttack", UpdateInventory, ItemFlag_ScrollSpinAttack, NULL, 0},
    {"AncientShield", UpdateInventory, ItemFlag_AncientShield, NULL, 0},
    {"PanFlute", UpdateInventory, ItemFlag_PanFlute, NULL, 0},
};

// -- settings menu items --

static GZMenuItem sSettingsMenuItems[] = {
    {"Prev Profile", PrevProfile, 0, NULL, 0},
    {"Next Profile", NextProfile, 0, NULL, 0},
    {"Load Default Profile", LoadDefaultProfile, 0, NULL, 0},
    {"Save Settings", SaveSettings, 0, NULL, 0},
};

// -- debug menu items --

static GZMenuItem sDebugMenuItems[] = {
    {"Regs", NULL, 0, &sRegsMenu, 0},
};

static GZMenuItem sRegsMenuItems[] = {
    {"Init. State", UpdateRegs, 0xFF, NULL, 0},
    {"Toggle BG1", UpdateRegs, 9, NULL, 0},
    {"Toggle BG2", UpdateRegs, 10, NULL, 0},
    {"Toggle BG3", UpdateRegs, 11, NULL, 0},
    {"Toggle OBJ", UpdateRegs, 12, NULL, 0},
};

// -- menu list --

// pointer to parent menu, pointer to items, number of items, does it require adventure mode, internal value
GZMenu sMainMenu = {NULL, sMainMenuItems, ARRAY_LEN(sMainMenuItems), false, 0};
GZMenu sInventoryMenu = {&sMainMenu, sInventoryMenuItems, ARRAY_LEN(sInventoryMenuItems), true, 0};
GZMenu sAmountsMenu = {&sInventoryMenu, sAmountsMenuItems, ARRAY_LEN(sAmountsMenuItems), true, 0};
GZMenu sCollectionMenu = {&sMainMenu, sCollectionMenuItems, ARRAY_LEN(sCollectionMenuItems), true, 0};
GZMenu sSettingsMenu = {&sMainMenu, sSettingsMenuItems, ARRAY_LEN(sSettingsMenuItems), false, 0};
GZMenu sAboutMenu = {&sMainMenu, NULL, 0, false, 0};
GZMenu sDebugMenu = {&sMainMenu, sDebugMenuItems, ARRAY_LEN(sDebugMenuItems), false, 0};
GZMenu sRegsMenu = {&sDebugMenu, sRegsMenuItems, ARRAY_LEN(sRegsMenuItems), false, 0};

// clang-format on

static void UpdateInventory(u32 params) {
    ItemFlag eFlag = params & 0xFF;

    if (data_027e0ce0 == NULL || data_027e0ce0->mUnk_28 == NULL) {
        return;
    }

    switch (eFlag) {
        case ItemFlag_Whirlwind:
        case ItemFlag_Boomerang:
        case ItemFlag_Whip:
        case ItemFlag_Bow:
        case ItemFlag_Bombs:
        case ItemFlag_SandRod:
            if (!GET_FLAG(data_027e0ce0->mUnk_28->mUnk_08, eFlag)) {
                data_027e0ce0->mUnk_28->func_ov000_020a863c(eFlag);
            } else {
                data_027e0ce0->mUnk_28->func_ov000_020a865c(eFlag);
            }
            break;
        default:
            break;
    }
}

static void UpdateAmounts(u32 params) {
    GZMenuItem* pActiveMenuItem = gMenuManager.GetActiveMenuItem();
    InventoryAmountType eType = (InventoryAmountType)(params & 0xFF);

    if (pActiveMenuItem == NULL || data_027e0ce0 == NULL || data_027e0ce0->mUnk_28 == NULL) {
        return;
    }

    gMenuManager.ValidateNewIncrement();

    switch (eType) {
        case InventoryAmountType_Bow:
            data_027e0ce0->mUnk_28->mArrowAmount = pActiveMenuItem->value;
            break;
        case InventoryAmountType_Bombs:
            data_027e0ce0->mUnk_28->mBombAmount = pActiveMenuItem->value;
            break;
        case InventoryAmountType_QuiverCapacity:
            data_027e0ce0->mUnk_28->mQuiverCapacity = pActiveMenuItem->value;
            break;
        case InventoryAmountType_BombCapacity:
            data_027e0ce0->mUnk_28->mBombBagCapacity = pActiveMenuItem->value;
            break;
        case InventoryAmountType_Potion1:
            data_027e0ce0->mUnk_28->mPotions[0] = pActiveMenuItem->value;
            break;
        case InventoryAmountType_Potion2:
            data_027e0ce0->mUnk_28->mPotions[1] = pActiveMenuItem->value;
            break;
        case InventoryAmountType_SmallKeys:
            data_027e0ce0->mUnk_28->mKeyAmount = pActiveMenuItem->value;
            break;
        case InventoryAmountType_LightTears:
            data_027e0ce0->mUnk_28->mTearsAmount = pActiveMenuItem->value;
            break;
        default:
            break;
    }
}

static void PrevProfile(u32 params) {
    if (gSettings.mProfileHeader.curProfileIndex - 1 > 0) {
        gSettings.mProfileHeader.curProfileIndex--;
    } else {
        gSettings.mProfileHeader.curProfileIndex = 0;
    }

    gMenuManager.mState.requestRedraw = true;
}

static void NextProfile(u32 params) {
    gSettings.mProfileHeader.curProfileIndex++;

    if (gSettings.mProfileHeader.curProfileIndex >= ARRAY_LEN(gSettings.mProfiles)) {
        gSettings.mProfileHeader.curProfileIndex = ARRAY_LEN(gSettings.mProfiles) - 1;
    }

    gMenuManager.mState.requestRedraw = true;
}

static void LoadDefaultProfile(u32 params) {
    gSettings.LoadDefaultProfile();
    gMenuManager.mState.successTimer = 90;
    gMenuManager.mState.requestRedraw = true;
}

static void SaveSettings(u32 params) {
    gSettings.WriteSave();
    gMenuManager.mState.successTimer = 90;
    gMenuManager.mState.requestRedraw = true;
}

static void UpdateRegs(u32 params) {
    static u32 dispCnt = 0xFF;

    if (params == 0xFF) {
        REG_DISPCNT_SUB = dispCnt;
    } else {
        if (dispCnt == 0xFF) {
            dispCnt = REG_DISPCNT_SUB;
        }

        u32 value = 1 << params;

        if (REG_DISPCNT_SUB & value) {
            REG_DISPCNT_SUB &= ~value;
        } else {
            REG_DISPCNT_SUB |= value;
        }
    }
}

static u32 prevDispCnt_Sub;
static vu16 prevBG0Cnt_Sub;

//! TODO: find a better way to avoid display glitches
// for now we back up BG0 VRAM and palettes data and restore them when done
static u8 prevBG0VRAM[0x1430]; // size of "DbgFntM.NCGR"
static u8 prevBGPalettes[0x400]; // size of sub palettes space

GZMenuManager gMenuManager;

GZMenuManager::GZMenuManager() {
    this->mState.Init();
    this->mpActiveMenu = &sMainMenu;
    this->mpButtons = &gGZ.mButtons;
    memset(&data_0204d9d0[DRAW_TO_TOP_SCREEN], 0, sizeof(Screen));
}

GZMenu* GZMenuManager::GetMainMenu() { return &sMainMenu; }

bool GZMenuManager::IsMainMenuActive() { return this->mpActiveMenu == &sMainMenu; }

bool GZMenuManager::IsInventoryMenuActive() { return this->mpActiveMenu == &sInventoryMenu; }

bool GZMenuManager::IsAmountsMenuActive() { return this->mpActiveMenu == &sAmountsMenu; }

bool GZMenuManager::IsCommandsMenuActive() { return this->mpActiveMenu == &gCommandManager.mMenu; }

bool GZMenuManager::IsSettingsMenuActive() { return this->mpActiveMenu == &sSettingsMenu; }

bool GZMenuManager::IsAboutMenuActive() { return this->mpActiveMenu == &sAboutMenu; }

void GZMenuManager::ValidateNewIncrement() {
    GZMenuItem* pActiveMenuItem = this->GetActiveMenuItem();

    // redundant but whatever
    if (pActiveMenuItem == NULL || this->mState.itemIndex == 0) {
        return;
    }

    if (pActiveMenuItem->value < 0) {
        pActiveMenuItem->value = 0;
    } else {
        switch (this->mState.itemIndex - 1) {
            case InventoryAmountType_Bow:
                if (data_027e0ce0 != NULL && data_027e0ce0->mUnk_28 != NULL) {
                    u8 max = data_027e0ce0->mUnk_28->func_ov000_020a8728();

                    if (pActiveMenuItem->value > max) {
                        pActiveMenuItem->value = max;
                    }
                }
                break;
            case InventoryAmountType_Bombs:
                if (data_027e0ce0 != NULL && data_027e0ce0->mUnk_28 != NULL) {
                    u8 max = data_027e0ce0->mUnk_28->func_ov000_020a8748();

                    if (pActiveMenuItem->value > max) {
                        pActiveMenuItem->value = max;
                    }
                }
                break;
            case InventoryAmountType_QuiverCapacity:
            case InventoryAmountType_BombCapacity:
                if (pActiveMenuItem->value > UpgradeCapacity_Tier3) {
                    pActiveMenuItem->value = UpgradeCapacity_Tier3;
                }
                break;
            case InventoryAmountType_Potion1:
            case InventoryAmountType_Potion2:
                if (pActiveMenuItem->value > PotionType_Yellow) {
                    pActiveMenuItem->value = PotionType_Yellow;
                }
                break;
            case InventoryAmountType_SmallKeys:
                if (pActiveMenuItem->value > MAX_KEYS) {
                    pActiveMenuItem->value = MAX_KEYS;
                }
                break;
            case InventoryAmountType_LightTears:
                if (pActiveMenuItem->value > MAX_TEARS_OF_LIGHT) {
                    pActiveMenuItem->value = MAX_TEARS_OF_LIGHT;
                }
                break;
            default:
                break;
        }
    }
}

void GZMenuManager::AssignPrevMenu() {
    if (this->mpActiveMenu->parent != NULL) {
        this->mpActiveMenu = this->mpActiveMenu->parent;
        this->mState.itemIndex = 0;
        this->mState.requestRedraw = true;
    }
}

void GZMenuManager::Update() {
    GZMenuItem* pActiveMenuItem = this->GetActiveMenuItem();

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

    if (!this->mState.isOpened) {
        return;
    }

    // selection update
    if (this->mControls.up.Executed(this->mpButtons)) {
        this->mState.itemIndex--;

        if (this->mState.itemIndex < 0) {
            this->mState.itemIndex = 0;
        }

        this->mState.requestRedraw = true;
    }

    if (this->mControls.down.Executed(this->mpButtons)) {
        if (this->mState.itemIndex + 1 < this->mpActiveMenu->mCount + 1) {
            this->mState.itemIndex++;
            this->mState.requestRedraw = true;
        }
    }

    if (this->IsAmountsMenuActive() && pActiveMenuItem != NULL) {
        bool changed = false;

        if (this->mControls.decrease.Executed(this->mpButtons)) {
            pActiveMenuItem->value--;
            changed = true;
        } else if (this->mControls.increase.Executed(this->mpButtons)) {
            pActiveMenuItem->value++;
            changed = true;
        }

        if (changed && this->mState.itemIndex > 0 && pActiveMenuItem->action != NULL) {
            pActiveMenuItem->action(pActiveMenuItem->params);
            this->mState.requestRedraw = true;
        }
    }

    if (this->mControls.ok.Executed(this->mpButtons)) {
        // handle confirmation stuff

        // null when "back" or "quit" is selected
        if (pActiveMenuItem != NULL) {
            if (!this->mpActiveMenu->needSaveFile || (this->mpActiveMenu->needSaveFile && gGZ.IsAdventureMode())) {
                if ((!this->IsAmountsMenuActive() || this->mState.itemIndex == 0) && pActiveMenuItem->action != NULL) {
                    pActiveMenuItem->action(pActiveMenuItem->params);

                    if (this->IsInventoryMenuActive()) {
                        this->mState.requestRedraw = true;
                    }
                } else if (pActiveMenuItem->submenu != NULL) {
                    this->mpActiveMenu = pActiveMenuItem->submenu;
                    this->mState.itemIndex = 0;
                    this->mState.requestRedraw = true;
                }
            }
        } else {
            if (this->IsMainMenuActive()) {
                this->Quit();
            } else {
                this->AssignPrevMenu();
            }
        }
    }

    if (this->mControls.back.Executed(this->mpButtons)) {
        // handle cancel stuff
        this->AssignPrevMenu();
    }

    if (this->mState.successTimer > 0) {
        // handle settings stuff
        this->mState.successTimer--;
        this->mState.requestRedraw = true;
    }

    if (this->mState.requestRedraw) {
        // redraw the menu if necessary
        this->mState.requestRedraw = false;
        this->SetupScreen();
        this->Draw();
    }
}

void GZMenuManager::SetAmountString(InventoryAmountType eType, Vec2b* pPos, bool selected) {
    u8 maxArrows = data_027e0ce0->mUnk_28->func_ov000_020a8728();
    u8 maxBombs = data_027e0ce0->mUnk_28->func_ov000_020a8748();

    static const char* szValueToPotion[] = {
        "None", // PotionType_None
        "Red Potion", // PotionType_Red
        "Purple Potion", // PotionType_Purple
        "Yellow Potion", // PotionType_Yellow
    };

    switch (eType) {
        case InventoryAmountType_Bow:
            if (data_027e0ce0 != NULL && data_027e0ce0->mUnk_28 != NULL) {
                DisplayDebugTextF(DRAW_TO_TOP_SCREEN, pPos, 0, selected, " (%d/%d)",
                                  data_027e0ce0->mUnk_28->mArrowAmount, maxArrows);
            }
            break;
        case InventoryAmountType_Bombs:
            if (data_027e0ce0 != NULL && data_027e0ce0->mUnk_28 != NULL) {
                DisplayDebugTextF(DRAW_TO_TOP_SCREEN, pPos, 0, selected, " (%d/%d)",
                                  data_027e0ce0->mUnk_28->mBombAmount, maxBombs);
            }
            break;
        case InventoryAmountType_QuiverCapacity:
            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, pPos, 0, selected, " (%d)", maxArrows);
            break;
        case InventoryAmountType_BombCapacity:
            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, pPos, 0, selected, " (%d)", maxBombs);
            break;
        case InventoryAmountType_Potion1:
            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, pPos, 0, selected, " (%s)",
                              szValueToPotion[data_027e0ce0->mUnk_28->mPotions[0]]);
            break;
        case InventoryAmountType_Potion2:
            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, pPos, 0, selected, " (%s)",
                              szValueToPotion[data_027e0ce0->mUnk_28->mPotions[1]]);
            break;
        case InventoryAmountType_SmallKeys:
            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, pPos, 0, selected, " (%d/%d)", data_027e0ce0->mUnk_28->mKeyAmount,
                              MAX_KEYS);
            break;
        case InventoryAmountType_LightTears:
            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, pPos, 0, selected, " (%d/%d)", data_027e0ce0->mUnk_28->mTearsAmount,
                              MAX_TEARS_OF_LIGHT);
            break;
        default:
            break;
    }
}

void GZMenuManager::SetupScreen() {
    Vec2b elemPos = this->mState.menuPos;

    // reset the top screen buffer
    memset(&data_0204d9d0[DRAW_TO_TOP_SCREEN], 0, sizeof(Screen));

    // send the menu item strings to the buffer
    const char* retStr = this->IsMainMenuActive() ? "Quit" : "Back";
    DisplayDebugText(DRAW_TO_TOP_SCREEN, &elemPos, 0, this->mState.itemIndex == 0, retStr);
    elemPos.y++;

    if (this->IsCommandsMenuActive()) {
        gCommandManager.Draw(&elemPos);
    } else {
        for (s16 i = 0; i < this->mpActiveMenu->mCount; i++) {
            GZMenuItem* pActiveMenuItem = &this->mpActiveMenu->entries[i];
            const char* szName = pActiveMenuItem->name;
            Vec2b extraPos = elemPos;
            bool selected = i + 1 == this->mState.itemIndex;
            extraPos.x += strlen(szName);

            // 0 = white, 1 = red, 2 = darker red, 3 = dark green
            DisplayDebugText(DRAW_TO_TOP_SCREEN, &elemPos, 0, selected, szName);

            if (this->IsAmountsMenuActive()) {
                this->SetAmountString((InventoryAmountType)(pActiveMenuItem->params & 0xFF), &extraPos, selected);
            } else if (this->IsInventoryMenuActive() && pActiveMenuItem->submenu == NULL) {
                bool hasItem = GET_FLAG(data_027e0ce0->mUnk_28->mUnk_08, pActiveMenuItem->params & 0xFF);
                DisplayDebugText(DRAW_TO_TOP_SCREEN, &extraPos, 0, selected,
                                 hasItem ? " (obtained)" : " (not obtained)");
            } else if (this->mpActiveMenu->needSaveFile && !gGZ.IsAdventureMode()) {
                DisplayDebugText(DRAW_TO_TOP_SCREEN, &extraPos, 0, selected, " (disabled)");
            }

            elemPos.y++;
        }
    }

    // send the arrow to the buffer
    Vec2b arrowPos = this->mState.menuPos;
    arrowPos.x--;
    arrowPos.y += this->mState.itemIndex;
    DisplayDebugText(DRAW_TO_TOP_SCREEN, &arrowPos, 0, 1, ">");
    arrowPos.y++;

    if (this->IsSettingsMenuActive()) {
        // special handling for the settings screen
        Vec2b settingsPos = this->mState.menuPos;
        settingsPos.y = elemPos.y + 1;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &settingsPos, 0, 0, "Current Profile: %d",
                          gSettings.mProfileHeader.curProfileIndex + 1);

        settingsPos.y += 15;
        if (gSettings.error) {
            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &settingsPos, 0, 1, "Error detected: 0x%X", gSettings.errorCode);
        } else if (this->mState.successTimer > 0) {
            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &settingsPos, 0, 0, "Success!", gSettings.errorCode);
        }
    } else if (this->IsAboutMenuActive()) {
        // special handling for the about screen
        Vec2b aboutPos = this->mState.menuPos;
        aboutPos.y = elemPos.y + 1; // start wherever the item list ended
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Code Version: %s", gBuildGitVersion);

        aboutPos.y++;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Build Author: %s", gBuildAuthor);

        aboutPos.y++;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Commit Author: %s", gCommitAuthor);

        aboutPos.y++;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Commit Name: %s", gCommitGitString);

        aboutPos.y += 15;
        DisplayDebugText(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Licensed under GPL-3.0");

        aboutPos.y++;
        DisplayDebugText(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Made with <3 by Yanis2");
    }
}

void GZMenuManager::StartDraw() {
    prevDispCnt_Sub = REG_DISPCNT_SUB;
    prevBG0Cnt_Sub = REG_BG0CNT_SUB;
    Copy256((void*)0x06208000, prevBG0VRAM, sizeof(prevBG0VRAM));
    Copy256((void*)0x05000400, prevBGPalettes, sizeof(prevBGPalettes));

    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x0400) | 0x100; // disable BG2 and make sure BG0 is enabled
    REG_BG0CNT_SUB &= ~3; // make BG0 on top of everything else

    if (gGZ.IsAdventureMode() && !gGZ.IsOnLand()) {
        //! TODO: fix issue where the font is displayed on the screen
        REG_DISPCNT_SUB &= ~0x0200; // disable BG1 for the overworld
    }

    func_0201b180(false, true); // loads the font
}

void GZMenuManager::Draw() {
    // copy screen
    func_02027654(data_0204d9d0[DRAW_TO_TOP_SCREEN].data, sizeof(data_0204d9d0[DRAW_TO_TOP_SCREEN].data));
    func_020252ec(data_0204d9d0[DRAW_TO_TOP_SCREEN].data, 0, sizeof(data_0204d9d0[DRAW_TO_TOP_SCREEN].data));
}

void GZMenuManager::StopDraw() {
    REG_DISPCNT_SUB = prevDispCnt_Sub;
    REG_BG0CNT_SUB = prevBG0Cnt_Sub;
    func_02027654(prevBG0VRAM, sizeof(prevBG0VRAM));
    Copy256(prevBG0VRAM, (void*)0x06208000, sizeof(prevBG0VRAM));
    func_02027654(prevBGPalettes, sizeof(prevBGPalettes));
    Copy256(prevBGPalettes, (void*)0x05000400, sizeof(prevBGPalettes));
}
