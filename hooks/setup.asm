.nds
.relativeinclude on
.erroronwarning on

.definelabel FS_LoadOverlay, 0x0202DC08
.definelabel RTC_Init, 0x0202FF34
.definelabel func_02014d98, 0x02014D98
.definelabel data_02049bd4, 0x02049BD4

; create gz overlay data
.open OVLGZ_BIN, OVLGZ_ADDR
    .arm
    .org OVLGZ_ADDR
        .area OVLGZ_SIZE, 0xEE
        .importobj "build/eur/src/main.o"
        .endarea
.close

.open ARM9_BIN, ARM9_MOD_BIN, 0x02000000
    ; load the hooks into the main module
    .org ARM9_NEW_CODE_STORE_ADDR
        .area 0x9C, 0xFF
        .importobj "hooks/build/eur/src/hooks.o"
        .endarea

    ; apply update hook
    .org HOOK_UPDATE
        .area 0x04
        .arm
            bl GZ_UpdateHook
        .endarea
.close

; apply init hook
.open OVL018_BIN, OVL018_MOD_BIN, OVL018_ADDR
    .org HOOK_INIT
        .area 0x04
        .arm
            bl GZ_InitHook
        .endarea
.close
