#include "gz_controls.hpp"

#include <mem.h>
#include <string.h>

// from oot-gc
char* strcat(char* dst, const char* src) {
    const u8* p = (u8*)src - 1;
    u8* q = (u8*)dst - 1;

    while (*++q)
        ;

    q--;

    while (*++q = *++p)
        ;

    return (dst);
}

ButtonCombo::ButtonCombo() {
    this->name = NULL;
    this->Assign(BTN_NONE, BTN_NONE);
}

ButtonCombo::ButtonCombo(u16 held, u16 pressed) {
    this->name = NULL;
    this->Assign(held, pressed);
}

ButtonCombo::ButtonCombo(const char* name, u16 held, u16 pressed) {
    this->name = name;
    this->Assign(held, pressed);
}

// assign new button combo
void ButtonCombo::Assign(u16 held, u16 pressed) {
    this->held = held;
    this->pressed = pressed;
}

void ButtonCombo::Assign(u16 pressed) {
    this->held = BTN_NONE;
    this->pressed = pressed;
}

// checks if the button combo has been performed
bool ButtonCombo::Executed(u16 cur, u16 press) {
    if (this->held != BTN_NONE) {
        if (this->pressed != BTN_NONE) {
            return CHECK_BUTTON_COMBO(cur, this->held) && CHECK_BUTTON_COMBO(press, this->pressed);
        }

        return CHECK_BUTTON_COMBO(cur, this->held) != 0;
    } 

    return CHECK_BUTTON_COMBO(press, this->pressed) != 0;
}

bool ButtonCombo::Executed(Input* pButtons) {
    return this->Executed(pButtons->cur, pButtons->press);
}

const char* ButtonCombo::ButtonToString(u16 button, bool shortNames) {
    switch (button) {
        case BTN_A:
            return "A";
        case BTN_B:
            return "B";
        case BTN_SELECT:
            if (shortNames) {
                return "Sel.";
            }
            return "Select";
        case BTN_START:
            if (shortNames) {
                return "St.";
            }
            return "Start";
        case BTN_DRIGHT:
            if (shortNames) {
                return "DR";
            }
            return "D-Right";
        case BTN_DLEFT:
            if (shortNames) {
                return "DL";
            }
            return "D-Left";
        case BTN_DUP:
            if (shortNames) {
                return "DU";
            }
            return "D-Up";
        case BTN_DDOWN:
            if (shortNames) {
                return "DD";
            }
            return "D-Down";
        case BTN_R:
            return "R";
        case BTN_L:
            return "L";
        case BTN_X:
            return "X";
        case BTN_Y:
            return "Y";
        default:
            break;
    }

    return NULL;
}

void ButtonCombo::SetComboString() {
    char fullName[sizeof(this->fullName)];
    const char* heldStr = this->ButtonToString(this->held, false);
    const char* pressedStr = this->ButtonToString(this->pressed, false);

    memset(fullName, 0, sizeof(fullName));
    strcpy(fullName, this->name);
    strcat(fullName, " (");

    if (heldStr != NULL) {
        if (strlen(fullName) + strlen(heldStr) < sizeof(this->fullName)) {
            strcat(fullName, heldStr);
        } else {
            strcat(fullName, this->ButtonToString(this->held, true));
        }

        if (pressedStr != NULL) {
            strcat(fullName, " + ");
        }
    }

    if (pressedStr != NULL) {
        if (strlen(fullName) + strlen(pressedStr) < sizeof(this->fullName)) {
            strcat(fullName, pressedStr);
        } else {
            strcat(fullName, this->ButtonToString(this->pressed, true));
        }
    }

    strcat(fullName, ")");
    strcpy(this->fullName, fullName);
}
