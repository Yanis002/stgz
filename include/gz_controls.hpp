#pragma once

#include <nitro/button.h>

#define BTN_NONE 0

struct ButtonCombo {
    u16 held;
    u16 pressed;

    ButtonCombo() {
        this->Assign(BTN_NONE, BTN_NONE);
    }

    ButtonCombo(u16 held, u16 pressed) {
        this->Assign(held, pressed);
    }

    // assign new button combo
    void Assign(u16 held, u16 pressed) {
        this->held = held;
        this->pressed = pressed;
    }

    void Assign(u16 pressed) {
        this->held = BTN_NONE;
        this->pressed = pressed;
    }

    // checks if the button combo has been performed
    bool Executed(u16 cur, u16 press) {
        if (this->held != BTN_NONE) {
            if (this->pressed != BTN_NONE) {
                return CHECK_BUTTON_COMBO(cur, this->held) && CHECK_BUTTON_COMBO(press, this->pressed);
            }

            return CHECK_BUTTON_COMBO(cur, this->held) != 0;
        } 

        return CHECK_BUTTON_COMBO(press, this->pressed) != 0;
    }

    bool Executed(Input* pButtons) {
        return this->Executed(pButtons->cur, pButtons->press);
    }
};
