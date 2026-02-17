#include "gz_cheats.hpp"
#include "gz_settings.hpp"
#include "mem.hpp"

#include <Unknown/UnkStruct_027e0ce0.hpp>
#include <Unknown/UnkStruct_ov024_020d86b0.hpp>

static bool CheckCheat(int itemIndex) {
    if (itemIndex < GZCheat_Max) {
        return gCheatManager.Check(itemIndex);
    }

    return false;
}

static void ToggleCheat(u32 params) {
    if (params < GZCheat_Max) {
        gCheatManager.Toggle(params);
    }
}

#define DEFINE_CHEAT(enum, name) {name, GZMenuItemType_Bool, CheckCheat, ToggleCheat, GZCheat_##enum, NULL, 0},
static GZMenuItem sCheatMenuItems[GZCheat_Max] = {
#include "cheats.inl"
};
#undef DEFINE_CHEAT

GZCheatManager gCheatManager;

GZCheatManager::GZCheatManager() {
    this->mMenu.title = "Cheats";
    this->mMenu.parent = gMenuManager.GetMainMenu();
    this->mMenu.entries = sCheatMenuItems;
    this->mMenu.mCount = ARRAY_LEN(sCheatMenuItems);
    this->mMenu.needSaveFile = true;
    this->mMenu.itemIndex = 0;
    gCheatManager.SetCheatBitfieldPtr(gSettings.GetProfile()->mCheatBitfield);
}

void GZCheatManager::SetCheatBitfieldPtr(u32* pBitfield) { this->mpCheatBitfield = pBitfield; }

bool GZCheatManager::Check(GZCheat eCheat) {
    return this->mpCheatBitfield != NULL ? this->mpCheatBitfield[0] & (1 << eCheat) : false;
}

void GZCheatManager::Toggle(GZCheat eCheat) {
    u32 value = 1 << eCheat;

    if (this->mpCheatBitfield == NULL) {
        return;
    }

    if (this->mpCheatBitfield[0] & value) {
        this->mpCheatBitfield[0] &= ~value;
    } else {
        this->mpCheatBitfield[0] |= value;
    }
}

void GZCheatManager::Update() {
    if (data_027e0ce0 != NULL && data_027e0ce0->mUnk_28 != NULL) {
        if (this->Check(GZCheat_Health)) {
            //! TODO: find out what updates the hearts visually
            data_027e0ce0->mHealth = data_027e0ce0->mHealthMax;
        }

        if (this->Check(GZCheat_Rupees)) {
            data_027e0ce0->mUnk_28->mNumRupees = 9999;
        }

        if (this->Check(GZCheat_Arrows)) {
            data_027e0ce0->mUnk_28->mArrowAmount = data_027e0ce0->mUnk_28->func_ov000_020a8728();
        }

        if (this->Check(GZCheat_Bombs)) {
            data_027e0ce0->mUnk_28->mBombAmount = data_027e0ce0->mUnk_28->func_ov000_020a8748();
        }
    }

    if (gGZ.IsAdventureMode() && data_ov024_020d86b0 != NULL) {
        if (this->Check(GZCheat_Postcards)) {
            data_ov024_020d86b0->mNumPostcards = 127;
        }
    }
}
