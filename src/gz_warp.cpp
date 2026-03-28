#include "gz_warp.hpp"
#include "common.hpp"
#include "gz.hpp"
#include "gz_menu.hpp"

#include <Unknown/UnkStruct_027e09a4.hpp>

static void ExecuteWarp(u32 params) {
    UnkStruct_SceneChange1 infos;
    infos.mNextSceneIndex = params & 0xFF;
    infos.mSpawnIndex = 0;

    // necessary to avoid crashes with specific scenes
    switch (infos.mNextSceneIndex) {
        case SceneIndex_d_main_f:
            infos.mRoomIndex = 1;
            break;
        case SceneIndex_f_flame:
            infos.mRoomIndex = 3; // 2 also works
            break;
        case SceneIndex_f_water3:
            infos.mRoomIndex = 10;
            break;
        case SceneIndex_f_kakushi4:
            infos.mRoomIndex = 2; // above will spawn on specific floors of the dungeon
            break;
        case SceneIndex_b_last22:
        case SceneIndex_e3_train:
        case SceneIndex_e3_smount:
            //! TODO: those 3 are crashing, need a fix
            return;
        default:
            infos.mRoomIndex = 0;
            break;
    }

    gMenuManager.Quit();
    memcpy(&data_027e09a4->mUnk_14, &infos, sizeof(UnkStruct_SceneChange1));
    func_ov000_02071000(data_027e09a4->mpWarpUnk1, &data_027e09a4->mUnk_14, 2);
}

static GZMenuItem sTestMenuItems[] = {
    {"test_trn", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_trn, NULL, 0},
    {"test_trn2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_trn2, NULL, 0},
    {"test_pre", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_pre, NULL, 0},
    {"test_iwa", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_iwa, NULL, 0},
    {"test_hiratsu", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_hiratsu, NULL, 0},
    {"test_sik", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_sik, NULL, 0},
    {"test_fuj", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_fuj, NULL, 0},
    {"test_nit", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_nit, NULL, 0},
    {"test_mri", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_mri, NULL, 0},
    {"test_morita", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_morita, NULL, 0},
    {"test_yamaz", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_yamaz, NULL, 0},
    {"test_sako", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_sako, NULL, 0},
    {"test_kita", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_kita, NULL, 0},
    {"test_take", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_take, NULL, 0},
    {"test_suzuki", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_suzuki, NULL, 0},
    {"test_okane", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_okane, NULL, 0},
    {"test_dera", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_dera, NULL, 0},
    {"test_hosaka", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_hosaka, NULL, 0},
    {"test_hosaka_f", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_hosaka_f, NULL, 0},
    {"test_kato", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_kato, NULL, 0},
    {"test_okane_f", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_okane_f, NULL, 0},
    {"test_yamaz_f", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_yamaz_f, NULL, 0},
    {"test_sako_f", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_sako_f, NULL, 0},
    {"test_take_f", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_take_f, NULL, 0},
    {"test_kiuchi", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_kiuchi, NULL, 0},
    {"test_dera_f", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_dera_f, NULL, 0},
    {"test_slope", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_test_slope, NULL, 0},
};

static GZMenu sTestMenu = {
    "Warp - Test", gWarpManager.GetMenu(), sTestMenuItems, ARRAY_LEN(sTestMenuItems), true, 0,
};

static GZMenuItem sTrainMenuItems[] = {
    {"t_area0", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_area0, NULL, 0},
    {"t_area1", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_area1, NULL, 0},
    {"t_area2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_area2, NULL, 0},
    {"t_area3", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_area3, NULL, 0},
    {"t_tutorial", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_tutorial, NULL, 0},
    {"t_forest", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_forest, NULL, 0},
    {"t_smarine", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_smarine, NULL, 0},
    {"t_smount", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_smount, NULL, 0},
    {"t_smount2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_smount2, NULL, 0},
    {"t_smount3", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_smount3, NULL, 0},
    {"t_minigame", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_minigame, NULL, 0},
    {"t_dark", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_dark, NULL, 0},
    {"t_eviltrain", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_eviltrain, NULL, 0},
    {"t_eviltrain2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_eviltrain2, NULL, 0},
    {"t_eviltrain3", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_t_eviltrain3, NULL, 0},
};

static GZMenu sTrainMenu = {
    "Warp - Train", gWarpManager.GetMenu(), sTrainMenuItems, ARRAY_LEN(sTrainMenuItems), true, 0,
};

static GZMenuItem sDungeonMenuItems[] = {
    {"d_main", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_main, NULL, 0},
    {"d_main_f", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_main_f, NULL, 0},
    {"d_main_s", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_main_s, NULL, 0},
    {"d_main_a", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_main_a, NULL, 0},
    {"d_main_w", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_main_w, NULL, 0},
    {"d_tutorial", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_tutorial, NULL, 0},
    {"d_forest", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_forest, NULL, 0},
    {"d_snow26", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_snow26, NULL, 0},
    {"d_water27", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_water27, NULL, 0},
    {"d_flame", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_flame, NULL, 0},
    {"d_sand", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_d_sand, NULL, 0},
};

static GZMenu sDungeonMenu = {
    "Warp - Dungeon", gWarpManager.GetMenu(), sDungeonMenuItems, ARRAY_LEN(sDungeonMenuItems), true, 0,
};

static GZMenuItem sBossMenuItems[] = {
    {"b_forest", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_forest, NULL, 0},
    {"b_snow", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_snow, NULL, 0},
    {"b_water", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_water, NULL, 0},
    {"b_flame", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_flame, NULL, 0},
    {"b_sand", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_sand, NULL, 0},
    {"b_deago", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_deago, NULL, 0},
    {"b_last1", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_last1, NULL, 0},
    {"b_last2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_last2, NULL, 0},
    {"b_last22", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_last22, NULL, 0},
    {"b_last23", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_b_last23, NULL, 0},
};

static GZMenu sBossMenu = {
    "Warp - Boss", gWarpManager.GetMenu(), sBossMenuItems, ARRAY_LEN(sBossMenuItems), true, 0,
};

static GZMenuItem sFieldMenuItems[] = {
    {"f_hyral", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_hyral, NULL, 0},
    {"f_htown", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_htown, NULL, 0},
    {"f_forest1", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_forest1, NULL, 0},
    {"f_snow", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_snow, NULL, 0},
    {"f_water", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_water, NULL, 0},
    {"f_flame", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_flame, NULL, 0},
    {"f_flame5", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_flame5, NULL, 0},
    {"f_first", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_first, NULL, 0},
    {"f_forest2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_forest2, NULL, 0},
    {"f_snow2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_snow2, NULL, 0},
    {"f_water2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_water2, NULL, 0},
    {"f_flame2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_flame2, NULL, 0},
    {"f_sand", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_sand, NULL, 0},
    {"f_tetsuo", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_tetsuo, NULL, 0},
    {"f_bridge", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_bridge, NULL, 0},
    {"f_bridge2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_bridge2, NULL, 0},
    {"f_forest3", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_forest3, NULL, 0},
    {"f_water3", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_water3, NULL, 0},
    {"f_ajito", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_ajito, NULL, 0},
    {"f_ajito2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_ajito2, NULL, 0},
    {"f_flame3", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_flame3, NULL, 0},
    {"f_flame4", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_flame4, NULL, 0},
    {"f_rabbit", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_rabbit, NULL, 0},
    {"f_kakushi1", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_kakushi1, NULL, 0},
    {"f_kakushi2", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_kakushi2, NULL, 0},
    {"f_kakushi3", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_kakushi3, NULL, 0},
    {"f_kakushi4", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_kakushi4, NULL, 0},
    {"f_pirate", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_pirate, NULL, 0},
    {"f_passenger", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_passenger, NULL, 0},
    {"f_trnnpc", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_f_trnnpc, NULL, 0},
};

static GZMenu sFieldMenu = {
    "Warp - Field", gWarpManager.GetMenu(), sFieldMenuItems, ARRAY_LEN(sFieldMenuItems), true, 0,
};

static GZMenuItem sTakeEmAllOnMenuItems[] = {
    {"tekiya00", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya00, NULL, 0},
    {"tekiya01", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya01, NULL, 0},
    {"tekiya02", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya02, NULL, 0},
    {"tekiya03", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya03, NULL, 0},
    {"tekiya04", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya04, NULL, 0},
    {"tekiya05", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya05, NULL, 0},
    {"tekiya06", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya06, NULL, 0},
    {"tekiya07", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya07, NULL, 0},
    {"tekiya08", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya08, NULL, 0},
    {"tekiya09", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_tekiya09, NULL, 0},
};

static GZMenu sTakeEmAllOnMenu = {
    "Warp - TakeEmAllOn", gWarpManager.GetMenu(), sTakeEmAllOnMenuItems, ARRAY_LEN(sTakeEmAllOnMenuItems), true, 0,
};

static GZMenuItem sE3MenuItems[] = {
    {"e3_train", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_e3_train, NULL, 0},
    {"e3_dungeon", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_e3_dungeon, NULL, 0},
    {"e3_boss", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_e3_boss, NULL, 0},
    {"e3_bossm", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_e3_bossm, NULL, 0},
    {"e3_smount", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_e3_smount, NULL, 0},
};

static GZMenu sE3Menu = {
    "Warp - E3", gWarpManager.GetMenu(), sE3MenuItems, ARRAY_LEN(sE3MenuItems), true, 0,
};

static GZMenuItem sBattleMenuItems[] = {
    {"battle01", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle01, NULL, 0},
    {"battle02", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle02, NULL, 0},
    {"battle03", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle03, NULL, 0},
    {"battle04", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle04, NULL, 0},
    {"battle05", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle05, NULL, 0},
    {"battle06", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle06, NULL, 0},
    {"battle07", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle07, NULL, 0},
    {"battle08", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle08, NULL, 0},
    {"battle09", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle09, NULL, 0},
    {"battle10", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle10, NULL, 0},
    {"battle11", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle11, NULL, 0},
    {"battle12", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_battle12, NULL, 0},
};

static GZMenu sBattleMenu = {
    "Warp - Battle", gWarpManager.GetMenu(), sBattleMenuItems, ARRAY_LEN(sBattleMenuItems), true, 0,
};

static GZMenuItem sMiscMenuItems[] = {
    {"demo_train", GZMenuItemType_Default, NULL, ExecuteWarp, SceneIndex_demo_train, NULL, 0},
};

static GZMenu sMiscMenu = {
    "Warp - Misc", gWarpManager.GetMenu(), sMiscMenuItems, ARRAY_LEN(sMiscMenuItems), true, 0,
};

static GZMenuItem sWarpMenuItems[] = {
    {"Test", GZMenuItemType_Default, NULL, NULL, 0, &sTestMenu, 0},
    {"Train", GZMenuItemType_Default, NULL, NULL, 0, &sTrainMenu, 0},
    {"Dungeon", GZMenuItemType_Default, NULL, NULL, 0, &sDungeonMenu, 0},
    {"Boss", GZMenuItemType_Default, NULL, NULL, 0, &sBossMenu, 0},
    {"Field", GZMenuItemType_Default, NULL, NULL, 0, &sFieldMenu, 0},
    {"TakeEmAllOn", GZMenuItemType_Default, NULL, NULL, 0, &sTakeEmAllOnMenu, 0},
    {"E3", GZMenuItemType_Default, NULL, NULL, 0, &sE3Menu, 0},
    {"Battle", GZMenuItemType_Default, NULL, NULL, 0, &sBattleMenu, 0},
    {"Misc", GZMenuItemType_Default, NULL, NULL, 0, &sMiscMenu, 0},
};

GZWarpManager gWarpManager;

GZWarpManager::GZWarpManager() {
    this->mpButtons = gGZ.GetInput();
    this->InitMenu();
}

bool GZWarpManager::IsMenuActive() {
    if (gMenuManager.GetActiveMenu() == &sTestMenu) {
        return true;
    }

    if (gMenuManager.GetActiveMenu() == &sTrainMenu) {
        return true;
    }

    if (gMenuManager.GetActiveMenu() == &sDungeonMenu) {
        return true;
    }

    if (gMenuManager.GetActiveMenu() == &sBossMenu) {
        return true;
    }

    if (gMenuManager.GetActiveMenu() == &sFieldMenu) {
        return true;
    }

    if (gMenuManager.GetActiveMenu() == &sTakeEmAllOnMenu) {
        return true;
    }

    if (gMenuManager.GetActiveMenu() == &sE3Menu) {
        return true;
    }

    if (gMenuManager.GetActiveMenu() == &sBattleMenu) {
        return true;
    }

    if (gMenuManager.GetActiveMenu() == &sMiscMenu) {
        return true;
    }

    return false;
}

void GZWarpManager::InitMenu() {
    this->mMenu.title = "Warp";
    this->mMenu.parent = gMenuManager.GetMainMenu();
    this->mMenu.entries = sWarpMenuItems;
    this->mMenu.count = ARRAY_LEN(sWarpMenuItems);
    this->mMenu.needSaveFile = true;
}

void GZWarpManager::Update() {}

void GZWarpManager::Draw(Vec2b* pPos) {
    Vec2b elemPos = *pPos;
    GZMenu* pMenu = gMenuManager.GetActiveMenu();
    s16 itemIndex = gMenuManager.GetState()->itemIndex;

    for (s16 i = 0; i < pMenu->count; i++) {
        bool selected = i + 1 == itemIndex;
        DisplayDebugText(DRAW_TO_TOP_SCREEN, &elemPos, 0, selected, pMenu->entries[i].name);
        elemPos.y++;

        if (i == 20) {
            elemPos.x += 14;
            elemPos.y = pPos->y;
        }
    }

    *pPos = elemPos;
}
