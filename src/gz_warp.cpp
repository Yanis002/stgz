#include "gz_warp.hpp"
#include "common.hpp"
#include "gz.hpp"
#include "gz_menu.hpp"

#include <Unknown/UnkStruct_027e09a4.hpp>

static void UpdateSceneIndex(u32 params) {
    GZMenuItem* pActiveMenuItem = gMenuManager.GetActiveMenuItem();
    gWarpManager.SetSceneName(pActiveMenuItem->name);
    gWarpManager.SetSceneIndex(params & 0xFF);
}

static void UpdateSpawnIndex(u32 params) {
    GZMenuItem* pActiveMenuItem = gMenuManager.GetActiveMenuItem();

    if (pActiveMenuItem->value < 0) {
        pActiveMenuItem->value = 255;
    } else if (pActiveMenuItem->value >= 256) {
        pActiveMenuItem->value = 0;
    }

    gWarpManager.SetSpawnIndex(pActiveMenuItem->value);
}

static void UpdateRoomIndex(u32 params) {
    GZMenuItem* pActiveMenuItem = gMenuManager.GetActiveMenuItem();
    gWarpManager.SetRoomIndex(pActiveMenuItem->value);
}

static void UpdateCutsceneIndex(u32 params) {
    GZMenuItem* pActiveMenuItem = gMenuManager.GetActiveMenuItem();

    if (pActiveMenuItem->value < 0) {
        pActiveMenuItem->value = 0x2B - 1;
    } else if (pActiveMenuItem->value >= 0x2B) {
        pActiveMenuItem->value = 0;
    }

    gWarpManager.SetCutsceneIndex(pActiveMenuItem->value);
}

static void ExecuteWarp(u32 params) {
    UnkStruct_SceneChange1 infos;
    infos.mNextSceneIndex = gWarpManager.GetSceneIndex();

    // do nothing if it's a test scene since they are missing
    if (infos.mNextSceneIndex >= SceneIndex_test_trn && infos.mNextSceneIndex <= SceneIndex_test_trn2) {
        return;
    }

    if (infos.mNextSceneIndex >= SceneIndex_test_hiratsu && infos.mNextSceneIndex <= SceneIndex_test_slope) {
        return;
    }

    // necessary to avoid crashes with specific scenes
    switch (infos.mNextSceneIndex) {
        case SceneIndex_b_last22:
        case SceneIndex_e3_train:
        case SceneIndex_e3_smount:
            //! TODO: those 3 are crashing, need a fix
            return;
        default:
            infos.mSpawnIndex = gWarpManager.GetSpawnIndex();
            infos.mRoomIndex = gWarpManager.GetRoomIndex();
            infos.mCutsceneIndex = gWarpManager.GetCutsceneIndex();
            break;
    }

    // disable pause and frame advance
    GZState* pState = gGZ.GetState();
    pState->isPaused = false;
    pState->requestedFrames = 0;

    // close menu
    gMenuManager.Quit();

    // copy the informations and execute the warp
    memcpy(&data_027e09a4->mUnk_14, &infos, sizeof(UnkStruct_SceneChange1));
    func_ov000_02071000(data_027e09a4->mpWarpUnk1, &data_027e09a4->mUnk_14, 2);
}

static GZMenuItem sWarpExecuteItems[] = {
    {"Execute Warp", GZMenuItemType_Default, NULL, ExecuteWarp, 0, NULL, 0},
    {"Spawn Index", GZMenuItemType_Increment, NULL, UpdateSpawnIndex, WarpIndexType_Spawn, NULL, 0},
    {"Room Index", GZMenuItemType_Increment, NULL, UpdateRoomIndex, WarpIndexType_Room, NULL, 0},
    {"Cutscene Index", GZMenuItemType_Increment, NULL, UpdateCutsceneIndex, WarpIndexType_Cutscene, NULL, 0},
};

static GZMenu sWarpExecuteMenu = {
    "Warp - Execute", gWarpManager.GetMenu(), sWarpExecuteItems, ARRAY_LEN(sWarpExecuteItems), true, 0,
};

static GZMenuItem sTrainMenuItems[] = {
    {"t_area0", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_area0, &sWarpExecuteMenu, 0},
    {"t_area1", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_area1, &sWarpExecuteMenu, 0},
    {"t_area2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_area2, &sWarpExecuteMenu, 0},
    {"t_area3", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_area3, &sWarpExecuteMenu, 0},
    {"t_tutorial", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_tutorial, &sWarpExecuteMenu, 0},
    {"t_forest", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_forest, &sWarpExecuteMenu, 0},
    {"t_smarine", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_smarine, &sWarpExecuteMenu, 0},
    {"t_smount", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_smount, &sWarpExecuteMenu, 0},
    {"t_smount2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_smount2, &sWarpExecuteMenu, 0},
    {"t_smount3", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_smount3, &sWarpExecuteMenu, 0},
    {"t_minigame", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_minigame, &sWarpExecuteMenu, 0},
    {"t_dark", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_dark, &sWarpExecuteMenu, 0},
    {"t_eviltrain", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_eviltrain, &sWarpExecuteMenu, 0},
    {"t_eviltrain2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_eviltrain2, &sWarpExecuteMenu, 0},
    {"t_eviltrain3", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_t_eviltrain3, &sWarpExecuteMenu, 0},
};

static GZMenu sTrainMenu = {
    "Warp - Train", gWarpManager.GetMenu(), sTrainMenuItems, ARRAY_LEN(sTrainMenuItems), true, 0,
};

static GZMenuItem sDungeonMenuItems[] = {
    {"d_main", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_main, &sWarpExecuteMenu, 0},
    {"d_main_f", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_main_f, &sWarpExecuteMenu, 0},
    {"d_main_s", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_main_s, &sWarpExecuteMenu, 0},
    {"d_main_a", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_main_a, &sWarpExecuteMenu, 0},
    {"d_main_w", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_main_w, &sWarpExecuteMenu, 0},
    {"d_tutorial", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_tutorial, &sWarpExecuteMenu, 0},
    {"d_forest", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_forest, &sWarpExecuteMenu, 0},
    {"d_snow26", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_snow26, &sWarpExecuteMenu, 0},
    {"d_water27", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_water27, &sWarpExecuteMenu, 0},
    {"d_flame", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_flame, &sWarpExecuteMenu, 0},
    {"d_sand", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_d_sand, &sWarpExecuteMenu, 0},
};

static GZMenu sDungeonMenu = {
    "Warp - Dungeon", gWarpManager.GetMenu(), sDungeonMenuItems, ARRAY_LEN(sDungeonMenuItems), true, 0,
};

static GZMenuItem sBossMenuItems[] = {
    {"b_forest", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_forest, &sWarpExecuteMenu, 0},
    {"b_snow", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_snow, &sWarpExecuteMenu, 0},
    {"b_water", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_water, &sWarpExecuteMenu, 0},
    {"b_flame", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_flame, &sWarpExecuteMenu, 0},
    {"b_sand", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_sand, &sWarpExecuteMenu, 0},
    {"b_deago", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_deago, &sWarpExecuteMenu, 0},
    {"b_last1", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_last1, &sWarpExecuteMenu, 0},
    {"b_last2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_last2, &sWarpExecuteMenu, 0},
    {"b_last22", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_last22, &sWarpExecuteMenu, 0},
    {"b_last23", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_b_last23, &sWarpExecuteMenu, 0},
};

static GZMenu sBossMenu = {
    "Warp - Boss", gWarpManager.GetMenu(), sBossMenuItems, ARRAY_LEN(sBossMenuItems), true, 0,
};

static GZMenuItem sFieldMenuItems[] = {
    {"f_hyral", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_hyral, &sWarpExecuteMenu, 0},
    {"f_htown", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_htown, &sWarpExecuteMenu, 0},
    {"f_forest1", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_forest1, &sWarpExecuteMenu, 0},
    {"f_snow", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_snow, &sWarpExecuteMenu, 0},
    {"f_water", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_water, &sWarpExecuteMenu, 0},
    {"f_flame", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_flame, &sWarpExecuteMenu, 0},
    {"f_flame5", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_flame5, &sWarpExecuteMenu, 0},
    {"f_first", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_first, &sWarpExecuteMenu, 0},
    {"f_forest2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_forest2, &sWarpExecuteMenu, 0},
    {"f_snow2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_snow2, &sWarpExecuteMenu, 0},
    {"f_water2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_water2, &sWarpExecuteMenu, 0},
    {"f_flame2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_flame2, &sWarpExecuteMenu, 0},
    {"f_sand", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_sand, &sWarpExecuteMenu, 0},
    {"f_tetsuo", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_tetsuo, &sWarpExecuteMenu, 0},
    {"f_bridge", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_bridge, &sWarpExecuteMenu, 0},
    {"f_bridge2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_bridge2, &sWarpExecuteMenu, 0},
    {"f_forest3", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_forest3, &sWarpExecuteMenu, 0},
    {"f_water3", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_water3, &sWarpExecuteMenu, 0},
    {"f_ajito", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_ajito, &sWarpExecuteMenu, 0},
    {"f_ajito2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_ajito2, &sWarpExecuteMenu, 0},
    {"f_flame3", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_flame3, &sWarpExecuteMenu, 0},
    {"f_flame4", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_flame4, &sWarpExecuteMenu, 0},
    {"f_rabbit", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_rabbit, &sWarpExecuteMenu, 0},
    {"f_kakushi1", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_kakushi1, &sWarpExecuteMenu, 0},
    {"f_kakushi2", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_kakushi2, &sWarpExecuteMenu, 0},
    {"f_kakushi3", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_kakushi3, &sWarpExecuteMenu, 0},
    {"f_kakushi4", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_kakushi4, &sWarpExecuteMenu, 0},
    {"f_pirate", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_pirate, &sWarpExecuteMenu, 0},
    {"f_passenger", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_passenger, &sWarpExecuteMenu, 0},
    {"f_trnnpc", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_f_trnnpc, &sWarpExecuteMenu, 0},
};

static GZMenu sFieldMenu = {
    "Warp - Field", gWarpManager.GetMenu(), sFieldMenuItems, ARRAY_LEN(sFieldMenuItems), true, 0,
};

static GZMenuItem sTakeEmAllOnMenuItems[] = {
    {"tekiya00", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya00, &sWarpExecuteMenu, 0},
    {"tekiya01", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya01, &sWarpExecuteMenu, 0},
    {"tekiya02", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya02, &sWarpExecuteMenu, 0},
    {"tekiya03", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya03, &sWarpExecuteMenu, 0},
    {"tekiya04", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya04, &sWarpExecuteMenu, 0},
    {"tekiya05", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya05, &sWarpExecuteMenu, 0},
    {"tekiya06", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya06, &sWarpExecuteMenu, 0},
    {"tekiya07", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya07, &sWarpExecuteMenu, 0},
    {"tekiya08", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya08, &sWarpExecuteMenu, 0},
    {"tekiya09", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_tekiya09, &sWarpExecuteMenu, 0},
};

static GZMenu sTakeEmAllOnMenu = {
    "Warp - TakeEmAllOn", gWarpManager.GetMenu(), sTakeEmAllOnMenuItems, ARRAY_LEN(sTakeEmAllOnMenuItems), true, 0,
};

static GZMenuItem sE3MenuItems[] = {
    {"e3_train", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_e3_train, &sWarpExecuteMenu, 0},
    {"e3_dungeon", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_e3_dungeon, &sWarpExecuteMenu, 0},
    {"e3_boss", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_e3_boss, &sWarpExecuteMenu, 0},
    {"e3_bossm", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_e3_bossm, &sWarpExecuteMenu, 0},
    {"e3_smount", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_e3_smount, &sWarpExecuteMenu, 0},
};

static GZMenu sE3Menu = {
    "Warp - E3", gWarpManager.GetMenu(), sE3MenuItems, ARRAY_LEN(sE3MenuItems), true, 0,
};

static GZMenuItem sBattleMenuItems[] = {
    {"battle01", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle01, &sWarpExecuteMenu, 0},
    {"battle02", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle02, &sWarpExecuteMenu, 0},
    {"battle03", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle03, &sWarpExecuteMenu, 0},
    {"battle04", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle04, &sWarpExecuteMenu, 0},
    {"battle05", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle05, &sWarpExecuteMenu, 0},
    {"battle06", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle06, &sWarpExecuteMenu, 0},
    {"battle07", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle07, &sWarpExecuteMenu, 0},
    {"battle08", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle08, &sWarpExecuteMenu, 0},
    {"battle09", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle09, &sWarpExecuteMenu, 0},
    {"battle10", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle10, &sWarpExecuteMenu, 0},
    {"battle11", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle11, &sWarpExecuteMenu, 0},
    {"battle12", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_battle12, &sWarpExecuteMenu, 0},
};

static GZMenu sBattleMenu = {
    "Warp - Battle", gWarpManager.GetMenu(), sBattleMenuItems, ARRAY_LEN(sBattleMenuItems), true, 0,
};

static GZMenuItem sMiscMenuItems[] = {
    {"demo_train", GZMenuItemType_Default, NULL, UpdateSceneIndex, SceneIndex_demo_train, &sWarpExecuteMenu, 0},
};

static GZMenu sMiscMenu = {
    "Warp - Misc", gWarpManager.GetMenu(), sMiscMenuItems, ARRAY_LEN(sMiscMenuItems), true, 0,
};

static GZMenuItem sWarpMenuItems[] = {
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
    this->mpSceneName = NULL;
    this->InitMenu();
}

void GZWarpManager::SetRoomIndex(int value) {
    const RoomIndicesInfos* pInfos = &gRoomIndicesPtrs[this->mSceneIndex];

    if (pInfos->count == 1 && pInfos->ptr[0] == 0xFF) {
        value = 0;
    } else {
        if (value < 0) {
            value = pInfos->count - 1;
        } else if (value >= pInfos->count) {
            value = 0;
        }
    }

    gMenuManager.GetActiveMenuItem()->value = value;
    this->mRoomIndex = pInfos->ptr[value];
}

void GZWarpManager::UpdateRoomIndexRange() {
    const RoomIndicesInfos* pInfos = &gRoomIndicesPtrs[this->mSceneIndex];
    this->mRoomIndexMin = this->mRoomIndexMax = 0;

    if (pInfos->count > 1) {
        this->mRoomIndexMin = pInfos->ptr[0];
        this->mRoomIndexMax = pInfos->ptr[pInfos->count - 1];
    } else if (pInfos->count == 1 && pInfos->ptr[0] != 0xFF) {
        this->mRoomIndexMin = this->mRoomIndexMax = pInfos->ptr[0];
    }

    this->mRoomIndex = this->mRoomIndexMin;
}

bool GZWarpManager::IsMenuActive() {
    if (gMenuManager.GetActiveMenu() == &sWarpExecuteMenu) {
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

    if (gMenuManager.GetActiveMenu() == &this->mMenu) {
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
    bool isExecute = pMenu == &sWarpExecuteMenu;

    for (s16 i = 0; i < pMenu->count; i++) {
        GZMenuItem* pItem = &pMenu->entries[i];
        bool selected = i + 1 == itemIndex;
        int max = 0;
        int value = pItem->value;

        if (isExecute && pItem->eType == GZMenuItemType_Increment) {
            switch (pItem->params) {
                case WarpIndexType_Spawn:
                    //! TODO: figure this out
                    max = 0xFF;
                    break;
                case WarpIndexType_Room:
                    max = this->mRoomIndexMax;
                    value = this->mRoomIndex;
                    break;
                case WarpIndexType_Cutscene:
                    //! TODO: figure out if we can set a range per-scene like room indices
                    max = 0x2B;
                    break;
                default:
                    break;
            }

            DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &elemPos, 0, selected, "%s (%d/%d)", pItem->name, value, max);
        } else {
            DisplayDebugText(DRAW_TO_TOP_SCREEN, &elemPos, 0, selected, pItem->name);
        }

        elemPos.y++;

        if (i == 20) {
            elemPos.x += 14;
            elemPos.y = pPos->y;
        }
    }

    if (isExecute && this->mpSceneName != NULL) {
        Vec2b bottom = *pPos;
        bottom.y += 20;
        DisplayDebugTextF(DRAW_TO_TOP_SCREEN, &bottom, 0, 0, "Selected Warp: %s", this->mpSceneName);
    }

    *pPos = elemPos;
}
