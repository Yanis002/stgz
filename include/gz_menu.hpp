#pragma once

#include "mem.h"

#include <types.h>
#include <nitro/math.h>
#include <nitro/button.h>

typedef void (*GZMenuAction)(u32 params);

struct GZMenu;

struct GZMenuItem {
    const char* mName;
    GZMenuAction mActionCallback;
    u32 params;
    GZMenu* mSubMenu;
    u32 mSubMenuCount;
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

    GZMenuControls() {
        // default controls
        this->toggleMenu.Assign(BTN_R, BTN_L);
        this->back.Assign(BTN_R, BTN_DLEFT);
        this->up.Assign(0, BTN_DUP);
        this->down.Assign(0, BTN_DDOWN);
        this->ok.Assign(0, BTN_A);
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

    void AssignPrevMenu() {
        if (this->mpActiveMenu->mPrev != NULL) {
            this->mpActiveMenu = this->mpActiveMenu->mPrev;
            this->mState.itemIndex = 0;
            this->mState.requestRedraw = true;
        }
    }

    void Quit() {
        this->StopDraw();
        this->mState.isOpened = false;
    }

    void Update(); // update routine
    void SetupScreen(); // creates the strings etc
    void StartDraw(); // prepares the game to draw the menu
    void Draw(); // execute the draw
    void StopDraw(); // quit and restore state
};

extern GZMenuManager gMenuManager;
