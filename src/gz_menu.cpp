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

/*
TODO LIST:
- OBJ toggle (~0x1400)
- reimplement about menu and back/quit
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
extern "C" void DC_FlushAll();
extern "C" void func_020131ec();
extern "C" void GX_SetGraphicsMode(int, int, int);
extern "C" void GXS_SetGraphicsMode(int);
extern "C" void func_0201b180(bool, bool);
extern "C" unk32 func_020147a8();
extern "C" void func_0201b278(bool, bool);
extern "C" void SetBrightColor(void*, int);

static void Quit(u32 params);
static void Back(u32 params);
static void UpdateInventory(u32 params);
static void UpdateAmounts(u32 params);

// settings menu
static void PrevProfile(u32 params);
static void NextProfile(u32 params);
static void LoadDefaultProfile(u32 params);
static void SaveSettings(u32 params);

extern GZMenu sMainMenu;
extern GZMenu sInventoryMenu;
extern GZMenu sAmountsMenu;
extern GZMenu sCollectionMenu;
extern GZMenu sSettingsMenu;
extern GZMenu sAboutMenu;

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

static GZMenuItem sSettingsMenuItems[] = {
    {"Prev Profile", PrevProfile, 0, NULL, 0},
    {"Next Profile", NextProfile, 0, NULL, 0},
    {"Load Default Profile", LoadDefaultProfile, 0, NULL, 0},
    {"Save Settings", SaveSettings, 0, NULL, 0},
};

static GZMenuItem sMainMenuItems[] = {
    {"Inventory", NULL, 0, &sInventoryMenu, 0},
    {"Collection", NULL, 0, &sCollectionMenu, 0},
    {"Settings", NULL, 0, &sSettingsMenu, 0},
    {"Commands", NULL, 0, &gCommandManager.mMenu, 0},
};

// pointer to the item list, number of items, pointer to the previous menu
GZMenu sMainMenu = {sMainMenuItems, ARRAY_LEN(sMainMenuItems), false, 0};
GZMenu sInventoryMenu = {sInventoryMenuItems, ARRAY_LEN(sInventoryMenuItems), true, 0};
GZMenu sAmountsMenu = {sAmountsMenuItems, ARRAY_LEN(sAmountsMenuItems), true, 0};
GZMenu sCollectionMenu = {sCollectionMenuItems, ARRAY_LEN(sCollectionMenuItems), true, 0};
GZMenu sSettingsMenu = {sSettingsMenuItems, ARRAY_LEN(sSettingsMenuItems), false, 0};
// GZMenu sCommandsMenu = {sCommandsMenuItems, ARRAY_LEN(sCommandsMenuItems), 0};
// GZMenu sAboutMenu = {sAboutMenuItems, ARRAY_LEN(sAboutMenuItems), 0};

static void Quit(u32 params) { gMenuManager.Quit(); }

static void Back(u32 params) { gMenuManager.AssignPrevMenu(); }

static void UpdateInventory(u32 params) {
    GZMenuItem* pActiveMenuItem = gMenuManager.GetActiveMenuItem();
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

    if (data_027e0ce0 == NULL || data_027e0ce0->mUnk_28 == NULL) {
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

static u32 prevDispCnt_Sub;

GZMenuManager gMenuManager;

GZMenuManager::GZMenuManager() {
    this->mState.Init();
    this->mpActiveMenu = &sMainMenu;
    this->mpButtons = &gGZ.mButtons;
    memset(&data_0204d9d0[DRAW_TO_TOP_SCREEN], 0, sizeof(Screen));
}

bool GZMenuManager::IsInventoryMenuActive() { return this->mpActiveMenu == &sInventoryMenu; }

bool GZMenuManager::IsAmountsMenuActive() { return this->mpActiveMenu == &sAmountsMenu; }

bool GZMenuManager::IsCommandsMenuActive() { return this->mpActiveMenu == &gCommandManager.mMenu; }

bool GZMenuManager::IsSettingsMenuActive() { return this->mpActiveMenu == &sSettingsMenu; }

bool GZMenuManager::IsAboutMenuActive() { return false; } // this->mpActiveMenu == &sAboutMenu; }

void GZMenuManager::ValidateNewIncrement() {
    GZMenuItem* pActiveMenuItem = this->GetActiveMenuItem();

    if (this->mState.itemIndex == 0) {
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
    if (this->mpPrevMenu != NULL) {
        GZMenu* pPrevMenu = this->mpActiveMenu;
        this->mpActiveMenu = this->mpPrevMenu;
        this->mpPrevMenu = pPrevMenu;
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
        if (this->mState.itemIndex + 1 < this->mpActiveMenu->mCount) {
            this->mState.itemIndex++;
            this->mState.requestRedraw = true;
        }
    }

    if (this->IsAmountsMenuActive()) {
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

void GZMenuManager::SetAmountString(s16 index, Vec2b* pPos, bool selected) {
    u8 maxArrows = data_027e0ce0->mUnk_28->func_ov000_020a8728();
    u8 maxBombs = data_027e0ce0->mUnk_28->func_ov000_020a8748();

    static const char* szValueToPotion[] = {
        "None", // PotionType_None
        "Red Potion", // PotionType_Red
        "Purple Potion", // PotionType_Purple
        "Yellow Potion", // PotionType_Yellow
    };

    switch (index - 1) {
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
    // reset the top screen buffer
    memset(&data_0204d9d0[DRAW_TO_TOP_SCREEN], 0, sizeof(Screen));

    // send the menu item strings to the buffer
    Vec2b elemPos = this->mState.menuPos;
    if (this->IsCommandsMenuActive()) {
        gCommandManager.Draw(&elemPos);
    } else {
        for (s16 i = 0; i < this->mpActiveMenu->mCount; i++) {
            GZMenuItem* pActiveMenuItem = &this->mpActiveMenu->entries[i];
            const char* szName = pActiveMenuItem->name;
            Vec2b extraPos = elemPos;
            bool selected = i == this->mState.itemIndex;
            extraPos.x += strlen(szName);

            // 0 = white, 1 = red, 2 = darker red, 3 = dark green
            DisplayDebugText(DRAW_TO_TOP_SCREEN, &elemPos, 0, selected, szName);

            if (this->IsAmountsMenuActive()) {
                this->SetAmountString(i, &extraPos, selected);
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

        aboutPos.y += 16;
        DisplayDebugText(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Licensed under GPL-3.0");

        aboutPos.y++;
        DisplayDebugText(DRAW_TO_TOP_SCREEN, &aboutPos, 0, 0, "Made with <3 by Yanis2");
    }
}

void GZMenuManager::StartDraw() {
    prevDispCnt_Sub = REG_DISPCNT_SUB;
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x0400) | 0x100; // disable BG2 and make sure BG0 is enabled
    func_0201b180(false, true); // loads the font (participate in the corruption also?)
}

void GZMenuManager::Draw() {
    func_0201b278(false, true); // copy screen
}

void GZMenuManager::StopDraw() {
    REG_DISPCNT_SUB = prevDispCnt_Sub;

    // good solution to fix the map but the issue is also fixable with the painting stuff
    // might better to figure that out instead
    // if (data_027e0994 != NULL && data_027e09a4 != NULL && gGZ.IsAdventureMode()) {
    //     99 because we load map99.bin
    //     data_027e0994->vfunc_38(data_027e09a4->mSceneIndex, 99, 0, 0);
    // }
}
