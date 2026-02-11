#pragma once

#include "mem.h"

#include <types.h>
#include <nitro/math.h>
#include <nitro/button.h>
#include <System/OverlayManager.hpp>
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
    struct ButtonCombo {
        u16 held;
        u16 pressed;

        void Assign(u16 held, u16 pressed) {
            this->held = held;
            this->pressed = pressed;
        }

        bool Executed(Input* pButtons) {
            if (this->held != 0) {
                return CHECK_BUTTON_COMBO(pButtons->cur, this->held) && CHECK_BUTTON_COMBO(pButtons->press, this->pressed);
            }

            return CHECK_BUTTON_COMBO(pButtons->press, this->pressed);
        }
    };

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
        this->up.Assign(0, BTN_DUP);
        this->down.Assign(0, BTN_DDOWN);
        this->ok.Assign(0, BTN_A);
        this->back.Assign(0, BTN_B);
        this->decrease.Assign(0, BTN_DLEFT);
        this->increase.Assign(0, BTN_DRIGHT);
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

    bool IsAdventureMode() {
        return gOverlayManager.mLoadedOverlays[OverlaySlot_4] == OverlayIndex_MainGame;
    }

    bool IsBattleMode() {
        return gOverlayManager.mLoadedOverlays[OverlaySlot_4] == OverlayIndex_BattleGame;
    }

    bool IsFileSelect() {
        return gOverlayManager.mLoadedOverlays[OverlaySlot_4] == OverlayIndex_MainSelect;
    }

    bool IsTitleScreen() {
        return gOverlayManager.mLoadedOverlays[OverlaySlot_4] == OverlayIndex_Title;
    }

    GZMenuItem* GetActiveMenuItem() {
        return &this->mpActiveMenu->mpItems[this->mState.itemIndex];
    }

    bool IsInventoryMenuActive();
    bool IsAmountsMenuActive();
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
