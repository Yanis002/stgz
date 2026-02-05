.nds
.relativeinclude on
.erroronwarning on

.definelabel FS_LoadOverlay, 0x0202DC08
.definelabel RTC_Init, 0x0202FF34

; create gz overlay data
.open OVLGZ_BIN, OVLGZ_ADDR
    .arm
    .org OVLGZ_ADDR
        .area OVLGZ_SIZE, 0xEE
        .importobj "build/eur/src/test.o"
        .endarea
.close

; load the hooks into the main module
.open ARM9_BIN, ARM9_MOD_BIN, 0x02000000
    .org ARM9_NEW_CODE_STORE_ADDR
        .area 0x9C, 0xFF
        .importobj "hooks/build/eur/src/hooks.o"
        .endarea
.close

; apply init hook
.open OVL018_BIN, OVL018_MOD_BIN, OVL018_ADDR
    .org HOOK_RTCINIT
        .area 0x04
        .arm
            bl RTCInit_hook
        .endarea
.close
