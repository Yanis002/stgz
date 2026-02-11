#include "gz_settings.hpp"

#include <TitleScreen/TitleScreen.hpp>
#include <Unknown/UnkStruct_0204a060.hpp>
#include <Game/Game.hpp>
#include <Unknown/UnkStruct_ov000_020b5214.hpp>

GZSettings gSettings;

GZSettings::GZSettings() {
    this->mSaveIndex = 0;
    this->mFasterTitleScreen = true;
    this->mSkipTitleScreen = true;
}

GZSettings::~GZSettings() {

}

void GZSettings::ProcessTitleScreen() {
    if (!this->mFasterTitleScreen && !this->mSkipTitleScreen) {
        return;
    }

    if (!gGZ.IsTitleScreen()) {
        return;
    }

    if (data_027e0994 == NULL && data_027e0994->mpTitleScreen == NULL) {
        return;
    }

    if (data_027e0994->mpTitleScreen->mShowUI) {
        return;
    }

    data_027e0994->mpTitleScreen->func_ov025_020c4e54();

    if (gSettings.mSkipTitleScreen) {
        data_ov000_020b5214.func_ov000_0206db44(0x0B);
        data_027e0994->mpTitleScreen->func_ov025_020c4ea0(TitleScreenState_ToFileSelect);
    }
}
