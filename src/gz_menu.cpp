#include "gz_menu.hpp"
#include "gz.hpp"
#include "build.hpp"

#include <regs.h>
#include <Unknown/UnkStruct_ov000_020b50c0.hpp>
#include <Unknown/UnkStruct_027e0ce0.hpp>
#include <System/OverlayManager.hpp>
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
        - (TBD)
    - About
*/

struct Screen {
    u8 unk_00[0x0C];
    u8 data[0x600];
};
extern Screen data_0204d9d0[2];

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
static void UpdateAmounts(u32 params);
static void UpdateSettings(u32 params);

extern GZMenu sMainMenu;
extern GZMenu sInventoryMenu;
extern GZMenu sAmountsMenu;
extern GZMenu sCollectionMenu;
extern GZMenu sSettingsMenu;
extern GZMenu sAboutMenu;

static GZMenuItem sInventoryMenuItems[] = {
    {"Back", Back, 0, NULL, false, 0},
    {"Whirlwind", UpdateInventory, ItemFlag_Whirlwind, NULL, true, 0},
    {"Boomerang", UpdateInventory, ItemFlag_Boomerang, NULL, true, 0},
    {"Whip", UpdateInventory, ItemFlag_Whip, NULL, true, 0},
    {"Bow", UpdateInventory, ItemFlag_Bow, NULL, true, 0},
    {"Bombs", UpdateInventory, ItemFlag_Bombs, NULL, true, 0},
    {"SandRod", UpdateInventory, ItemFlag_SandRod, NULL, true, 0},
    {"Amounts", NULL, 0, &sAmountsMenu, true, 0},
};

static GZMenuItem sAmountsMenuItems[] = {
    {"Back", Back, 0, NULL, false, 0},
    {"Bow", UpdateAmounts, InventoryAmountType_Bow, NULL, true, 0},
    {"Bombs", UpdateAmounts, InventoryAmountType_Bombs, NULL, true, 0},
    {"Quiver Capacity", UpdateAmounts, InventoryAmountType_QuiverCapacity, NULL, true, 0},
    {"Bomb Bag Capacity", UpdateAmounts, InventoryAmountType_BombCapacity, NULL, true, 0},
    {"Potion 1", UpdateAmounts, InventoryAmountType_Potion1, NULL, true, 0},
    {"Potion 2", UpdateAmounts, InventoryAmountType_Potion2, NULL, true, 0},
    {"Small Keys", UpdateAmounts, InventoryAmountType_SmallKeys, NULL, true, 0},
    {"Light Tears", UpdateAmounts, InventoryAmountType_LightTears, NULL, true, 0},
};

static GZMenuItem sCollectionMenuItems[] = {
    {"Back", Back, 0, NULL, false, 0},
    {"Shield", UpdateInventory, ItemFlag_Shield, NULL, true, 0},
    {"Sword", UpdateInventory, ItemFlag_Sword, NULL, true, 0},
    {"LokomoSword", UpdateInventory, ItemFlag_LokomoSword, NULL, true, 0},
    {"RecruitUniform", UpdateInventory, ItemFlag_RecruitUniform, NULL, true, 0},
    {"ScrollBeam", UpdateInventory, ItemFlag_ScrollBeam, NULL, true, 0},
    {"ScrollSpinAttack", UpdateInventory, ItemFlag_ScrollSpinAttack, NULL, true, 0},
    {"AncientShield", UpdateInventory, ItemFlag_AncientShield, NULL, true, 0},
    {"PanFlute", UpdateInventory, ItemFlag_PanFlute, NULL, true, 0},
};

static GZMenuItem sSettingsMenuItems[] = {
    {"Back", Back, 0, NULL, false, 0},
};

static GZMenuItem sAboutMenuItems[] = {
    {"Back", Back, 0, NULL, false, 0},
};

static GZMenuItem sMainMenuItems[] = {
    {"Quit", Quit, 0, NULL, false , 0},
    {"Inventory", NULL, 0, &sInventoryMenu, true, 0},
    {"Collection", NULL, 0, &sCollectionMenu, true, 0},
    {"Settings", NULL, 0, &sSettingsMenu, false, 0},
    {"About", NULL, 0, &sAboutMenu, false, 0},
};

// pointer to the item list, number of items, pointer to the previous menu
GZMenu sMainMenu = {sMainMenuItems, ARRAY_LEN(sMainMenuItems), NULL};
GZMenu sInventoryMenu = {sInventoryMenuItems, ARRAY_LEN(sInventoryMenuItems), &sMainMenu};
GZMenu sAmountsMenu = {sAmountsMenuItems, ARRAY_LEN(sAmountsMenuItems), &sInventoryMenu};
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

static void UpdateSettings(u32 params) {

}

static u32 prevDispCnt;
static u32 prevDispCnt_Sub;
static u8 prevVRAM_CNT_B;

GZMenuManager gMenuManager;

GZMenuManager::GZMenuManager() {
    this->mState.Init();
    this->mpActiveMenu = &sMainMenu;
    this->mpButtons = &gGZ.mButtons;
    memset(&data_0204d9d0[0], 0, sizeof(Screen));
    memset(&data_0204d9d0[1], 0, sizeof(Screen));
}

bool GZMenuManager::IsInventoryMenuActive() {
    return this->mpActiveMenu == &sInventoryMenu;
}

bool GZMenuManager::IsAmountsMenuActive() {
    return this->mpActiveMenu == &sAmountsMenu;
}

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

void GZMenuManager::AssignPrevMenu()  {
    if (this->mpActiveMenu->mPrev != NULL) {
        this->mpActiveMenu = this->mpActiveMenu->mPrev;
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

        if (changed && this->mState.itemIndex > 0 && pActiveMenuItem->mActionCallback != NULL) {
            pActiveMenuItem->mActionCallback(pActiveMenuItem->params);
            this->mState.requestRedraw = true;
        }
    }

    if (this->mControls.ok.Executed(this->mpButtons)) {
        // handle confirmation stuff

        if (!pActiveMenuItem->needSaveFile || (pActiveMenuItem->needSaveFile && gGZ.IsAdventureMode())) {
            if ((!this->IsAmountsMenuActive() || this->mState.itemIndex == 0) && pActiveMenuItem->mActionCallback != NULL) {
                pActiveMenuItem->mActionCallback(pActiveMenuItem->params);

                if (this->IsInventoryMenuActive()) {
                    this->mState.requestRedraw = true;
                }
            } else if (pActiveMenuItem->mSubMenu != NULL) {
                this->mpActiveMenu = pActiveMenuItem->mSubMenu;
                this->mState.itemIndex = 0;
                this->mState.requestRedraw = true;
            }
        }
    }

    if (this->mControls.back.Executed(this->mpButtons)) {
        // handle cancel stuff
        this->AssignPrevMenu();
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
        "None",          // PotionType_None
        "Red Potion",    // PotionType_Red
        "Purple Potion", // PotionType_Purple
        "Yellow Potion", // PotionType_Yellow
    };

    switch (index - 1) {
        case InventoryAmountType_Bow:
            if (data_027e0ce0 != NULL && data_027e0ce0->mUnk_28 != NULL) {
                DisplayDebugTextF(0, pPos, 0, selected, " (%d/%d)", data_027e0ce0->mUnk_28->mArrowAmount, maxArrows);
            }
            break;
        case InventoryAmountType_Bombs:
            if (data_027e0ce0 != NULL && data_027e0ce0->mUnk_28 != NULL) {
                DisplayDebugTextF(0, pPos, 0, selected, " (%d/%d)", data_027e0ce0->mUnk_28->mBombAmount, maxBombs);
            }
            break;
        case InventoryAmountType_QuiverCapacity:
            DisplayDebugTextF(0, pPos, 0, selected, " (%d)", maxArrows);
            break;
        case InventoryAmountType_BombCapacity:
            DisplayDebugTextF(0, pPos, 0, selected, " (%d)", maxBombs);
            break;
        case InventoryAmountType_Potion1:
            DisplayDebugTextF(0, pPos, 0, selected, " (%s)", szValueToPotion[data_027e0ce0->mUnk_28->mPotions[0]]);
            break;
        case InventoryAmountType_Potion2:
            DisplayDebugTextF(0, pPos, 0, selected, " (%s)", szValueToPotion[data_027e0ce0->mUnk_28->mPotions[1]]);
            break;
        case InventoryAmountType_SmallKeys:
            DisplayDebugTextF(0, pPos, 0, selected, " (%d/%d)", data_027e0ce0->mUnk_28->mKeyAmount, MAX_KEYS);
            break;
        case InventoryAmountType_LightTears:
            DisplayDebugTextF(0, pPos, 0, selected, " (%d/%d)", data_027e0ce0->mUnk_28->mTearsAmount, MAX_TEARS_OF_LIGHT);
            break;
        default:
            break;
    }
}

void GZMenuManager::SetupScreen() {
    // reset the top screen buffer
    memset(&data_0204d9d0[0], 0, sizeof(Screen));

    // send the menu item strings to the buffer
    Vec2b elemPos = this->mState.menuPos;
    for (s16 i = 0; i < this->mpActiveMenu->mCount; i++) {
        GZMenuItem* pActiveMenuItem = &this->mpActiveMenu->mpItems[i];
        const char* szName = pActiveMenuItem->mName;
        Vec2b extraPos = elemPos;
        bool selected = i == this->mState.itemIndex;
        extraPos.x += strlen(szName);

        // 0 = white, 1 = red, 2 = darker red, 3 = dark green
        DisplayDebugText(0, &elemPos, 0, selected, szName);

        if (i > 0) {
            if (this->IsAmountsMenuActive()) {
                this->SetAmountString(i, &extraPos, selected);
            } else if (this->IsInventoryMenuActive() && pActiveMenuItem->mSubMenu == NULL) {
                bool hasItem = GET_FLAG(data_027e0ce0->mUnk_28->mUnk_08, pActiveMenuItem->params & 0xFF);
                DisplayDebugText(0, &extraPos, 0, selected, hasItem ? " (obtained)" : " (not obtained)");
            } else if (pActiveMenuItem->needSaveFile && !gGZ.IsAdventureMode()) {
                DisplayDebugText(0, &extraPos, 0, selected, " (disabled)");
            }
        }

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

        aboutPos.y += 16;
        DisplayDebugText(0, &aboutPos, 0, 0, "Licensed under GPL-3.0");

        aboutPos.y++;
        DisplayDebugText(0, &aboutPos, 0, 0, "Made with <3 by Yanis2");
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
    func_0201b180(true, true); // loads the font (participate in the corruption also?)
}

void GZMenuManager::Draw() {
    func_0201b278(true, true); // copy screen
    func_020131ec();
    SetBrightColor((void *) &REG_MASTER_BRIGHT, 0);
    SetBrightColor((void *) &REG_MASTER_BRIGHT_SUB, 0);

    if (func_020147a8() != 0) {
        data_ov000_020b50c0.func_ov000_02069f58();
    }

    while (this->mState.isOpened && !this->mState.requestRedraw) {
        func_020131ec();
        gGZ.Update();
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

