#pragma once

#include "common.hpp"
#include "gz_controls.hpp"

#include <Item/ItemManager.hpp>
#include <mem.h>
#include <nitro/button.h>
#include <nitro/math.h>
#include <types.h>

#define DRAW_TO_TOP_SCREEN 1

struct GZMenu;

typedef u32 InventoryAmountType;
enum InventoryAmountType_ {
    InventoryAmountType_Bow,
    InventoryAmountType_Bombs,
    InventoryAmountType_QuiverCapacity,
    InventoryAmountType_BombCapacity,
    InventoryAmountType_Potion1,
    InventoryAmountType_Potion2,
    InventoryAmountType_SmallKeys,
    InventoryAmountType_LightTears,
    InventoryAmountType_Max
};

typedef u32 GZMenuItemType;
enum GZMenuItemType_ {
    GZMenuItemType_Default,
    GZMenuItemType_Bool,
    GZMenuItemType_Increment,
};

struct GZMenuItem {
    const char* name;
    GZMenuItemType eType;
    GZCheckCallback checkCallback; // must be set when using `GZMenuItemType_Bool`
    GZAction action;
    u32 params;
    GZMenu* submenu;

    // internal
    int value;

    void Draw(Vec2b* pPos, s16 index, bool selected, bool needSaveFile);
};

struct GZMenu {
    const char* title;
    GZMenu* parent;
    GZMenuItem* entries;
    s32 count;
    bool needSaveFile;

    // internal
    s16 itemIndex;

    void Draw(Vec2b* pPos);
};

struct GZMenuState {
    bool isOpened;
    bool requestRedraw;
    s16 itemIndex;
    Vec2b menuPos;
    s16 successTimer;

    GZMenuState() { memset(this, 0, sizeof(GZMenuState)); }

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
  private:
    GZMenuState mState;
    GZMenu* mpActiveMenu;
    GZMenuControls mControls;
    Input* mpButtons;

  public:
    GZMenuManager();

    GZMenuState* GetState() { return &this->mState; }

    GZMenu* GetActiveMenu() { return this->mpActiveMenu; }

    bool IsActive() { return this->mState.isOpened; }

    void Quit() {
        this->StopDraw();
        this->mState.isOpened = false;
    }

    GZMenuItem* GetActiveMenuItem() {
        return this->mState.itemIndex == 0 ? NULL : &this->mpActiveMenu->entries[this->mState.itemIndex - 1];
    }

    bool IsMainMenuActive();
    bool IsInventoryMenuActive();
    bool IsAmountsMenuActive();
    bool IsCommandsMenuActive();
    bool IsSettingsMenuActive();
    bool IsAboutMenuActive();

    GZMenu* GetMainMenu();
    void SetAmountString(InventoryAmountType eType, Vec2b* pPos, bool selected);
    void ValidateAmountIncrement();
    void AssignPrevMenu();
    void Update(); // update routine
    void SetupScreen(); // creates the strings etc
    void StartDraw(); // prepares the game to draw the menu
    void CopyScreen(); // execute the draw
    void StopDraw(); // quit and restore state
};

extern GZMenuManager gMenuManager;
