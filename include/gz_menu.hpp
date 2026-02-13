#pragma once

#include "gz_controls.hpp"

#include <mem.h>
#include <types.h>
#include <nitro/math.h>
#include <nitro/button.h>
#include <Item/ItemManager.hpp>

typedef void (*GZMenuAction)(u32 params);
struct GZMenu;

typedef enum InventoryAmountType {
    InventoryAmountType_Bow,
    InventoryAmountType_Bombs,
    InventoryAmountType_QuiverCapacity,
    InventoryAmountType_BombCapacity,
    InventoryAmountType_Potion1,
    InventoryAmountType_Potion2,
    InventoryAmountType_SmallKeys,
    InventoryAmountType_LightTears,
    InventoryAmountType_Max
} InventoryAmountType;

struct GZMenuItem {
    const char* mName; // menu item name
    GZMenuAction mActionCallback; // associated action
    u32 params; // parameters for the action callback
    GZMenu* mSubMenu; // tied submenu
    bool needSaveFile; // does it require the save data
    s32 value; // misc value for internal use
};

struct GZMenu {
    GZMenuItem* mpItems;
    s32 mCount;
    GZMenu* mPrev;
};

struct GZMenuState {
    bool isOpened;
    bool requestRedraw;
    s16 itemIndex;
    Vec2b menuPos;

    GZMenuState() {
        memset(this, 0, sizeof(GZMenuState));
    }

    void Init() {
        menuPos.x = 2;
        menuPos.y = 1;
    }
};

struct GZMenuControls {
    ButtonCombo toggleMenu; // open/close the menu
    ButtonCombo back; // go back to the previous submenu
    ButtonCombo up; // move the selection upward
    ButtonCombo down; // move the selection downward
    ButtonCombo ok; // confirm, accept, OK
    ButtonCombo decrease; // decrease mState.changeBy
    ButtonCombo increase; // increase mState.changeBy

    GZMenuControls() {
        // default controls
        this->toggleMenu.Assign(BTN_R, BTN_L);
        this->up.Assign(BTN_DUP);
        this->down.Assign(BTN_DDOWN);
        this->ok.Assign(BTN_A);
        this->back.Assign(BTN_B);
        this->decrease.Assign(BTN_DLEFT);
        this->increase.Assign(BTN_DRIGHT);
    }
};

class GZMenuManager {
public:
    GZMenuState mState;
    GZMenu* mpActiveMenu;
    GZMenuControls mControls;
    Input* mpButtons;

    GZMenuManager();

    bool IsActive() {
        return this->mState.isOpened;
    }

    void Quit() {
        this->StopDraw();
        this->mState.isOpened = false;
    }

    GZMenuItem* GetActiveMenuItem() {
        return &this->mpActiveMenu->mpItems[this->mState.itemIndex];
    }

    bool IsInventoryMenuActive();
    bool IsAmountsMenuActive();
    bool IsCommandsMenuActive();
    void SetAmountString(s16 index, Vec2b* pPos, bool selected);
    void ValidateNewIncrement();
    void AssignPrevMenu();
    void Update(); // update routine
    void SetupScreen(); // creates the strings etc
    void StartDraw(); // prepares the game to draw the menu
    void Draw(); // execute the draw
    void StopDraw(); // quit and restore state
};

extern GZMenuManager gMenuManager;
