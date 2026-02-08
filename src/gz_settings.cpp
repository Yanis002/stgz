#include "gz_settings.hpp"

GZSettings gSettings;

GZSettings::GZSettings() {
    this->bootMode = BOOT_MODE_FILE_SELECT;
    this->saveIndex = 0;
}

GZSettings::~GZSettings() {

}

void GZSettings::ProcessBootMode() {
    switch (this->bootMode) {
        case BOOT_MODE_TITLE:
            break;
        case BOOT_MODE_FILE_SELECT:
            gGame.createCallback = (GameModeCreateCallback)UnkStruct_0204a060::func_02018424;
            break;
        case BOOT_MODE_ADVENTURE:
            break;
        default:
            break;
    }
}
