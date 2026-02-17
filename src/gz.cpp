#include "gz.hpp"
#include "build.hpp"
#include "gz_cheats.hpp"
#include "gz_commands.hpp"
#include "gz_menu.hpp"
#include "gz_settings.hpp"

#include <System/Random.hpp>
#include <nitro/button.h>
#include <regs.h>

GZ gGZ;

void GZ::Init() {}

void GZ::Update() { this->UpdateInputs(); }

void GZ::OnGameModeInit() {}

void GZ::OnGameModeUpdate() {
    gMenuManager.Update();
    gCommandManager.Update();
    gSettings.Update();
    gCheatManager.Update();
}
