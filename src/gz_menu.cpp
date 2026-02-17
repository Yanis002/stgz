#include "gz_menu.hpp"
#include "build.hpp"
#include "common.hpp"
#include "gz.hpp"
#include "gz_cheats.hpp"
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
    - Cheats
        - Health
        - Rupees
        - Arrows
        - Bombs
        - Postcards
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

extern GZMenu sMainMenu;
extern GZMenu sInventoryMenu;
extern GZMenu sAmountsMenu;
extern GZMenu sCollectionMenu;
extern GZMenu sDebugMenu;
extern GZMenu sRegsMenu;
extern GZMenu sAboutMenu;

// clang-format off

// -- main menu items --

static GZMenuItem sMainMenuItems[] = {
    {"Cheats", GZMenuItemType_Default, NULL, NULL, 0, gCheatManager.GetMenu(), 0},
    {"Inventory", GZMenuItemType_Default, NULL, NULL, 0, &sInventoryMenu, 0},
    {"Collection", GZMenuItemType_Default, NULL, NULL, 0, &sCollectionMenu, 0},
    {"Commands", GZMenuItemType_Default, NULL, NULL, 0, gCommandManager.GetMenu(), 0},
    {"Settings", GZMenuItemType_Default, NULL, NULL, 0, gSettings.GetMenu(), 0},
    {"Debug", GZMenuItemType_Default, NULL, NULL, 0, &sDebugMenu, 0},
    {"About", GZMenuItemType_Default, NULL, NULL, 0, &sAboutMenu, 0},
};

// -- inventory menu items --

static void UpdateInventory(u32 params);
static bool HasObtainedItem(int itemIndex);

static GZMenuItem sInventoryMenuItems[] = {
    {"Amounts", GZMenuItemType_Default, NULL, NULL, 0, &sAmountsMenu, 0},
    {"Whirlwind", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_Whirlwind, NULL, 0},
    {"Boomerang", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_Boomerang, NULL, 0},
    {"Whip", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_Whip, NULL, 0},
    {"Bow", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_Bow, NULL, 0},
    {"Bombs", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_Bombs, NULL, 0},
    {"SandRod", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_SandRod, NULL, 0},
};

static void UpdateAmounts(u32 params);

static GZMenuItem sAmountsMenuItems[] = {
    {"Bow", GZMenuItemType_Increment, NULL, UpdateAmounts, InventoryAmountType_Bow, NULL, 0},
    {"Bombs", GZMenuItemType_Increment, NULL, UpdateAmounts, InventoryAmountType_Bombs, NULL, 0},
    {"Quiver Capacity", GZMenuItemType_Increment, NULL, UpdateAmounts, InventoryAmountType_QuiverCapacity, NULL, 0},
    {"Bomb Bag Capacity", GZMenuItemType_Increment, NULL, UpdateAmounts, InventoryAmountType_BombCapacity, NULL, 0},
    {"Potion 1", GZMenuItemType_Increment, NULL, UpdateAmounts, InventoryAmountType_Potion1, NULL, 0},
    {"Potion 2", GZMenuItemType_Increment, NULL, UpdateAmounts, InventoryAmountType_Potion2, NULL, 0},
    {"Small Keys", GZMenuItemType_Increment, NULL, UpdateAmounts, InventoryAmountType_SmallKeys, NULL, 0},
    {"Light Tears", GZMenuItemType_Increment, NULL, UpdateAmounts, InventoryAmountType_LightTears, NULL, 0},
};

// -- collection menu items --

static GZMenuItem sCollectionMenuItems[] = {
    {"Shield", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_Shield, NULL, 0},
    {"Sword", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_Sword, NULL, 0},
    {"Lokomo Sword", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_LokomoSword, NULL, 0},
    {"Recruit Uniform", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_RecruitUniform, NULL, 0},
    {"Scroll Beam", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_ScrollBeam, NULL, 0},
    {"Scroll Spin Attack", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_ScrollSpinAttack, NULL, 0},
    {"Ancient Shield", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_AncientShield, NULL, 0},
    {"Pan Flute", GZMenuItemType_Bool, HasObtainedItem, UpdateInventory, ItemFlag_PanFlute, NULL, 0},
};

// -- debug menu items --

static GZMenuItem sDebugMenuItems[] = {
    {"Regs", GZMenuItemType_Default, NULL, NULL, 0, &sRegsMenu, 0},
};

static void UpdateRegs(u32 params);
static bool IsLayerEnabled(int itemIndex);

static GZMenuItem sRegsMenuItems[] = {
    {"Init. State", GZMenuItemType_Default, NULL, UpdateRegs, 0xFF, NULL, 0},
    {"Toggle BG1", GZMenuItemType_Bool, IsLayerEnabled, UpdateRegs, 9, NULL, 0},
    {"Toggle BG2", GZMenuItemType_Bool, IsLayerEnabled, UpdateRegs, 10, NULL, 0},
    {"Toggle BG3", GZMenuItemType_Bool, IsLayerEnabled, UpdateRegs, 11, NULL, 0},
    {"Toggle OBJ", GZMenuItemType_Bool, IsLayerEnabled, UpdateRegs, 12, NULL, 0},
};

// -- menu list --

// pointer to parent menu, pointer to items, number of items, does it require adventure mode, internal value
GZMenu sMainMenu = {"Main Menu", NULL, sMainMenuItems, ARRAY_LEN(sMainMenuItems), false, 0};
GZMenu sInventoryMenu = {"Inventory", &sMainMenu, sInventoryMenuItems, ARRAY_LEN(sInventoryMenuItems), true, 0};
GZMenu sAmountsMenu = {"Inventory - Amounts", &sInventoryMenu, sAmountsMenuItems, ARRAY_LEN(sAmountsMenuItems), true, 0};
GZMenu sCollectionMenu = {"Collection", &sMainMenu, sCollectionMenuItems, ARRAY_LEN(sCollectionMenuItems), true, 0};
GZMenu sAboutMenu = {"About", &sMainMenu, NULL, 0, false, 0};
GZMenu sDebugMenu = {"Debug", &sMainMenu, sDebugMenuItems, ARRAY_LEN(sDebugMenuItems), false, 0};
GZMenu sRegsMenu = {"Debug - Regs", &sDebugMenu, sRegsMenuItems, ARRAY_LEN(sRegsMenuItems), false, 0};

// clang-format on

static void UpdateInventory(u32 params) {
    ItemFlag eFlag = params & 0xFF;

    if (data_027e0ce0 == NULL || data_027e0ce0->mUnk_28 == NULL) {
        return;
    }

    if (!GET_FLAG(data_027e0ce0->mUnk_28->mUnk_08, eFlag)) {
        data_027e0ce0->mUnk_28->func_ov000_020a863c(eFlag);
    } else {
        data_027e0ce0->mUnk_28->func_ov000_020a865c(eFlag);
    }
}

static bool HasObtainedItem(int itemIndex) {
    if (data_027e0ce0 == NULL || data_027e0ce0->mUnk_28 == NULL) {
        return false;
    }

    return GET_FLAG(data_027e0ce0->mUnk_28->mUnk_08, gMenuManager.GetActiveMenu()->entries[itemIndex].params & 0xFF);
}

static void UpdateAmounts(u32 params) {
    GZMenuItem* pActiveMenuItem = gMenuManager.GetActiveMenuItem();
    InventoryAmountType eType = params & 0xFF;

    if (pActiveMenuItem == NULL || data_027e0ce0 == NULL || data_027e0ce0->mUnk_28 == NULL) {
        return;
    }

    gMenuManager.ValidateAmountIncrement();

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

static bool IsLayerEnabled(int itemIndex) {
    GZMenuItem* pActiveItem = &gMenuManager.GetActiveMenu()->entries[itemIndex];
    u32 value = 1 << (pActiveItem->params & 0x0F);
    return REG_DISPCNT_SUB & value;
}

static u32 prevDispCnt_Sub;
static vu16 prevBG0Cnt_Sub;

//! TODO: find a better way to avoid display glitches
// for now we back up BG0 VRAM and palettes data and restore them when done
static u8 prevBG0VRAM[0x1430]; // size of "DbgFntM.NCGR"
static u8 prevBGPalettes[0x400]; // size of sub palettes space

GZMenuManager gMenuManager;

void GZMenuItem::Draw(Vec2b* pPos, s16 index, bool selected, bool needSaveFile) {
    const char* szName = this->name;
    Vec2b extraPos = *pPos;
    extraPos.x += strlen(szName);

    if (this->eType == GZMenuItemType_Bool && this->checkCallback != NULL) {
        Vec2b boolPos = *pPos;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &boolPos, 0, selected, "[%s]%s", this->checkCallback(index) ? "x" : " ",
                          szName);
    } else {
        // 0 = white, 1 = red, 2 = darker red, 3 = dark green, seems to be palette indices?
        DisplayDebugText(DRAW_TO_TOP_SCREEN, pPos, 0, selected, szName);
    }

    if (gMenuManager.IsAmountsMenuActive()) {
        gMenuManager.SetAmountString(this->params & 0xFF, &extraPos, selected);
    } else if (needSaveFile && !gGZ.IsAdventureMode()) {
        DisplayDebugText(DRAW_TO_TOP_SCREEN, &extraPos, 0, selected, " (disabled)");
    }
}

void GZMenu::Draw(Vec2b* pPos) {
    for (s16 i = 0; i < this->mCount; i++) {
        this->entries[i].Draw(pPos, i, i + 1 == gMenuManager.GetState()->itemIndex, this->needSaveFile);
        pPos->y++;
    }
}

GZMenuManager::GZMenuManager() {
    this->mState.Init();
    this->mpActiveMenu = &sMainMenu;
    this->mpButtons = gGZ.GetInput();
    memset(&data_0204d9d0[DRAW_TO_TOP_SCREEN], 0, sizeof(Screen));
}

GZMenu* GZMenuManager::GetMainMenu() { return &sMainMenu; }

bool GZMenuManager::IsMainMenuActive() { return this->mpActiveMenu == &sMainMenu; }

bool GZMenuManager::IsInventoryMenuActive() { return this->mpActiveMenu == &sInventoryMenu; }

bool GZMenuManager::IsAmountsMenuActive() { return this->mpActiveMenu == &sAmountsMenu; }

bool GZMenuManager::IsCommandsMenuActive() { return this->mpActiveMenu == gCommandManager.GetMenu(); }

bool GZMenuManager::IsSettingsMenuActive() { return this->mpActiveMenu == gSettings.GetMenu(); }

bool GZMenuManager::IsAboutMenuActive() { return this->mpActiveMenu == &sAboutMenu; }

void GZMenuManager::ValidateAmountIncrement() {
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
            this->mState.requestRedraw = true;
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

    if (pActiveMenuItem != NULL && pActiveMenuItem->eType == GZMenuItemType_Increment) {
        bool changed = false;

        if (this->mControls.decrease.Executed(this->mpButtons)) {
            pActiveMenuItem->value--;
            changed = true;
        } else if (this->mControls.increase.Executed(this->mpButtons)) {
            pActiveMenuItem->value++;
            changed = true;
        }

        // for now only amounts is using increments
        if (this->IsAmountsMenuActive() && changed && this->mState.itemIndex > 0 && pActiveMenuItem->action != NULL) {
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
                } else if (pActiveMenuItem->submenu != NULL) {
                    this->mpActiveMenu = pActiveMenuItem->submenu;
                    this->mState.itemIndex = 0;
                }

                this->mState.requestRedraw = true;
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
        this->CopyScreen();
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

    // reset the top screen buffer then send the menu item strings to the buffer
    memset(&data_0204d9d0[DRAW_TO_TOP_SCREEN], 0, sizeof(Screen));

    // current menu title
    Vec2b titlePos = elemPos;
    const char* titleStr = "stgz - ";
    titlePos.x += (28 - strlen(titleStr) - strlen(this->mpActiveMenu->title)) / 2;
    titlePos.y--;
    DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &titlePos, 0, 0, "%s%s", titleStr, this->mpActiveMenu->title);

    // return element
    const char* retStr = this->IsMainMenuActive() ? "Quit" : "Back";
    DisplayDebugText(DRAW_TO_TOP_SCREEN, &elemPos, 0, this->mState.itemIndex == 0 ? 3 : 0, retStr);
    elemPos.y++;

    // menu items
    if (this->IsCommandsMenuActive()) {
        gCommandManager.Draw(&elemPos);
    } else if (this->IsSettingsMenuActive()) {
        gSettings.Draw(&elemPos);
    } else {
        this->mpActiveMenu->Draw(&elemPos);
    }

    // current selection arrow
    Vec2b arrowPos = this->mState.menuPos;
    arrowPos.x--;
    arrowPos.y += this->mState.itemIndex;
    DisplayDebugText(DRAW_TO_TOP_SCREEN, &arrowPos, 0, this->mState.itemIndex == 0 ? 3 : 1, ">");
    arrowPos.y++;

    // about screen
    if (this->IsAboutMenuActive()) {
        // special handling for the about screen
        Vec2b aboutPos = this->mState.menuPos;

        aboutPos.y += 2;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Code Version: %s", gBuildGitVersion);

        aboutPos.y++;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Build Author: %s", gBuildAuthor);

        aboutPos.y++;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Commit Author: %s", gCommitAuthor);

        aboutPos.y++;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Commit Name: %s", gCommitGitString);

        aboutPos.y = 21;
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

void GZMenuManager::CopyScreen() {
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
