#pragma once

#include <nitro/button.h>

#define BTN_NONE 0

struct ButtonCombo {
    const char* name;
    u16 held;
    u16 pressed;
    char fullName[40]; // internal use, we need a buffer to host the string for the commands menu

    ButtonCombo();
    ButtonCombo(u16 held, u16 pressed);
    ButtonCombo(const char* name, u16 held, u16 pressed);

    // assign new button combo
    void Assign(u16 held, u16 pressed);
    void Assign(u16 pressed);

    // checks if the button combo has been performed
    bool Executed(u16 cur, u16 press);
    bool Executed(Input* pButtons);

    const char* ButtonToString(u16 button, bool shortNames);
    void SetComboString();
};
